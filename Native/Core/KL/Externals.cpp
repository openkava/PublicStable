/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/KL/Externals.h>
#include <Fabric/Core/CG/ArrayProducerAdapter.h>
#include <Fabric/Core/CG/ArrayAdapter.h>
#include <Fabric/Core/CG/SizeAdapter.h>
#include <Fabric/Core/CG/ValueProducerAdapter.h>
#include <Fabric/Core/MR/ArrayCache.h>
#include <Fabric/Core/MR/ArrayGenerator.h>
#include <Fabric/Core/MR/ArrayGeneratorOperator.h>
#include <Fabric/Core/MR/ArrayMap.h>
#include <Fabric/Core/MR/ArrayMapOperator.h>
#include <Fabric/Core/MR/ArrayTransform.h>
#include <Fabric/Core/MR/ArrayTransformOperator.h>
#include <Fabric/Core/MR/ConstArray.h>
#include <Fabric/Core/MR/ConstValue.h>
#include <Fabric/Core/MR/Reduce.h>
#include <Fabric/Core/MR/ReduceOperator.h>
#include <Fabric/Core/MR/ValueCache.h>
#include <Fabric/Core/MR/ValueGenerator.h>
#include <Fabric/Core/MR/ValueGeneratorOperator.h>
#include <Fabric/Core/MR/ValueMap.h>
#include <Fabric/Core/MR/ValueMapOperator.h>
#include <Fabric/Core/MR/ValueTransform.h>
#include <Fabric/Core/MR/ValueTransformOperator.h>
#include <Fabric/Core/RT/ArrayDesc.h>
#include <Fabric/Core/RT/ArrayProducerDesc.h>
#include <Fabric/Core/RT/ValueProducerDesc.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Base/Config.h>

#include <string>
#include <map>
#include <math.h>
#include <stdlib.h>

#if defined(FABRIC_OS_WINDOWS)
extern "C" void _chkstk();
    
static float imp_roundf( float x )
{
  return floorf( x + 0.5f );
}
    
static double imp_round( double x )
{
  return floor( x + 0.5 );
}
#endif

static double fp64_fabs( double x )
{
  return fabs( x );
}

static double fp64_fmod( double x, double y )
{
  return fmod( x, y );
}

static double fp64_sin( double x )
{
  return sin( x );
}

static double fp64_cos( double x )
{
  return cos( x );
}

static double fp64_tan( double x )
{
  return tan( x );
}

static double fp64_sqrt( double x )
{
  return sqrt( x );
}

static double fp64_ceil( double x )
{
  return ceil( x );
}

static double fp64_floor( double x )
{
  return floor( x );
}

static double fp64_asin( double x )
{
  return asin( x );
}

static double fp64_acos( double x )
{
  return acos( x );
}

static double fp64_atan( double x )
{
  return atan( x );
}

static double fp64_pow( double x, double y )
{
  return pow( x, y );
}

static double fp64_atan2( double x, double y )
{
  return atan2( x, y );
}

namespace Fabric
{
  static void MRCreateValueCache(
    MR::ValueProducer *&input,
    MR::ValueProducer const *&valueProducer
    )
  {
    if ( valueProducer )
      valueProducer->release();
    valueProducer = MR::ValueCache::Create(input).take();
  }

  static void MRCreateArrayCache(
    MR::ArrayProducer *&input,
    MR::ArrayProducer const *&arrayProducer
    )
  {
    if ( arrayProducer )
      arrayProducer->release();
    arrayProducer = MR::ArrayCache::Create(input).take();
  }

  static void MRCreateConstValue(
    CG::ValueProducerAdapter *valueProducerAdapter,
    void const *data,
    MR::ValueProducer const *&valueProducer
    )
  {
    if ( valueProducer )
      valueProducer->release();
    valueProducer = MR::ConstValue::Create(
      valueProducerAdapter->getRTManager(),
      RC::ConstHandle<RT::ValueProducerDesc>::StaticCast( valueProducerAdapter->getDesc() )->getValueDesc(),
      data
      ).take();
  }

  static void MRCreateValueTransform1(
    void (*operator_)(...),
    MR::ValueProducer *&input,
    MR::ValueProducer const *&valueProducer
    )
  {
    if ( valueProducer )
      valueProducer->release();
    valueProducer = MR::ValueTransform::Create(
      input,
      MR::ValueTransformOperator::Create(
        operator_,
        1,
        input->getValueDesc(),
        0
        ),
      0
      ).take();
  }

  static void MRCreateValueTransform2(
    void (*operator_)(...),
    MR::ValueProducer *&input,
    MR::ValueProducer *&shared,
    MR::ValueProducer const *&valueProducer
    )
  {
    if ( valueProducer )
      valueProducer->release();
    valueProducer = MR::ValueTransform::Create(
      input,
      MR::ValueTransformOperator::Create(
        operator_,
        2,
        input->getValueDesc(),
        shared->getValueDesc()
        ),
      shared
      ).take();
  }

  static void MRCreateValueMap2(
    void (*operator_)(...),
    MR::ValueProducer *&input,
    CG::ValueProducerAdapter *outputValueProducerAdapter,
    MR::ValueProducer const *&valueProducer
    )
  {
    if ( valueProducer )
      valueProducer->release();
    valueProducer = MR::ValueMap::Create(
      input,
      MR::ValueMapOperator::Create(
        operator_,
        2,
        input->getValueDesc(),
        RC::ConstHandle<RT::ValueProducerDesc>::StaticCast( outputValueProducerAdapter->getDesc() )->getValueDesc(),
        0
        ),
      0
      ).take();
  }

  static void MRCreateValueMap3(
    void (*operator_)(...),
    MR::ValueProducer *&input,
    CG::ValueProducerAdapter *outputValueProducerAdapter,
    MR::ValueProducer *&shared,
    MR::ValueProducer const *&valueProducer
    )
  {
    if ( valueProducer )
      valueProducer->release();
    valueProducer = MR::ValueMap::Create(
      input,
      MR::ValueMapOperator::Create(
        operator_,
        3,
        input->getValueDesc(),
        RC::ConstHandle<RT::ValueProducerDesc>::StaticCast( outputValueProducerAdapter->getDesc() )->getValueDesc(),
        shared->getValueDesc()
        ),
      shared
      ).take();
  }

  static void MRCreateArrayGenerator3(
    void (*operator_)(...),
    size_t numParams,
    MR::ValueProducer *&countValueProducer,
    CG::ArrayProducerAdapter *outputArrayProducerAdapter,
    MR::ArrayProducer const *&outputArrayProducer
    )
  {
    if ( outputArrayProducer )
      outputArrayProducer->release();
    outputArrayProducer = MR::ArrayGenerator::Create(
      countValueProducer,
      MR::ArrayGeneratorOperator::Create(
        operator_,
        numParams,
        RC::ConstHandle<RT::ArrayProducerDesc>::StaticCast( outputArrayProducerAdapter->getDesc() )->getElementDesc(),
        0
        ),
      0
      ).take();
  }

  static void MRCreateArrayGenerator4(
    void (*operator_)(...),
    size_t numParams,
    MR::ValueProducer *&countValueProducer,
    MR::ValueProducer *&sharedValueProducer,
    CG::ArrayProducerAdapter *outputArrayProducerAdapter,
    MR::ArrayProducer const *&outputArrayProducer
    )
  {
    if ( outputArrayProducer )
      outputArrayProducer->release();
    outputArrayProducer = MR::ArrayGenerator::Create(
      countValueProducer,
      MR::ArrayGeneratorOperator::Create(
        operator_,
        numParams,
        RC::ConstHandle<RT::ArrayProducerDesc>::StaticCast( outputArrayProducerAdapter->getDesc() )->getElementDesc(),
        sharedValueProducer->getValueDesc()
        ),
      sharedValueProducer
      ).take();
  }

  static void MRCreateValueGenerator1(
    void (*operator_)(...),
    size_t numParams,
    CG::ValueProducerAdapter *outputValueProducerAdapter,
    MR::ValueProducer const *&outputValueProducer
    )
  {
    if ( outputValueProducer )
      outputValueProducer->release();
    outputValueProducer = MR::ValueGenerator::Create(
      MR::ValueGeneratorOperator::Create(
        operator_,
        numParams,
        RC::ConstHandle<RT::ValueProducerDesc>::StaticCast( outputValueProducerAdapter->getDesc() )->getValueDesc(),
        0
        ),
      0
      ).take();
  }

  static void MRCreateValueGenerator2(
    void (*operator_)(...),
    size_t numParams,
    MR::ValueProducer *&sharedValueProducer,
    CG::ValueProducerAdapter *outputValueProducerAdapter,
    MR::ValueProducer const *&outputValueProducer
    )
  {
    if ( outputValueProducer )
      outputValueProducer->release();
    outputValueProducer = MR::ValueGenerator::Create(
      MR::ValueGeneratorOperator::Create(
        operator_,
        numParams,
        RC::ConstHandle<RT::ValueProducerDesc>::StaticCast( outputValueProducerAdapter->getDesc() )->getValueDesc(),
        sharedValueProducer->getValueDesc()
        ),
      sharedValueProducer
      ).take();
  }

  static void MRCreateConstArray(
    CG::ArrayAdapter *arrayAdapter,
    void const *arrayData,
    CG::ArrayProducerAdapter *arrayProducerAdapter,
    MR::ArrayProducer const *&arrayProducer
    )
  {
    if ( arrayProducer )
      arrayProducer->release();
    arrayProducer = MR::ConstArray::Create(
      arrayProducerAdapter->getRTManager(),
      RC::ConstHandle<RT::ArrayDesc>::StaticCast( arrayAdapter->getDesc() ),
      arrayData
      ).take();
  }

  static void MRCreateArrayMap4(
    void (*operator_)(...),
    size_t numParams,
    MR::ArrayProducer *&inputArrayProducer,
    CG::ArrayProducerAdapter *outputArrayProducerAdapter,
    MR::ArrayProducer const *&arrayProducer
    )
  {
    if ( arrayProducer )
      arrayProducer->release();
    arrayProducer = MR::ArrayMap::Create(
      inputArrayProducer,
      MR::ArrayMapOperator::Create(
        operator_,
        numParams,
        inputArrayProducer->getElementDesc(),
        RC::ConstHandle<RT::ArrayProducerDesc>::StaticCast( outputArrayProducerAdapter->getDesc() )->getElementDesc(),
        0
        ),
      0
      ).take();
  }

  static void MRCreateArrayMap5(
    void (*operator_)(...),
    size_t numParams,
    MR::ArrayProducer *&inputArrayProducer,
    CG::ArrayProducerAdapter *outputArrayProducerAdapter,
    MR::ValueProducer *&sharedValueProducer,
    MR::ArrayProducer const *&arrayProducer
    )
  {
    if ( arrayProducer )
      arrayProducer->release();
    arrayProducer = MR::ArrayMap::Create(
      inputArrayProducer,
      MR::ArrayMapOperator::Create(
        operator_,
        numParams,
        inputArrayProducer->getElementDesc(),
        RC::ConstHandle<RT::ArrayProducerDesc>::StaticCast( outputArrayProducerAdapter->getDesc() )->getElementDesc(),
        sharedValueProducer->getValueDesc()
        ),
      sharedValueProducer
      ).take();
  }

  static void MRCreateArrayTransform3(
    void (*operator_)(...),
    size_t numParams,
    MR::ArrayProducer *&ioArrayProducer,
    MR::ArrayProducer const *&arrayProducer
    )
  {
    if ( arrayProducer )
      arrayProducer->release();
    arrayProducer = MR::ArrayTransform::Create(
      ioArrayProducer,
      MR::ArrayTransformOperator::Create(
        operator_,
        numParams,
        ioArrayProducer->getElementDesc(),
        0
        ),
      0
      ).take();
  }

  static void MRCreateArrayTransform4	(
    void (*operator_)(...),
    size_t numParams,
    MR::ArrayProducer *&ioArrayProducer,
    MR::ValueProducer *&sharedValueProducer,
    MR::ArrayProducer const *&arrayProducer
    )
  {
    if ( arrayProducer )
      arrayProducer->release();
    arrayProducer = MR::ArrayTransform::Create(
      ioArrayProducer,
      MR::ArrayTransformOperator::Create(
        operator_,
        numParams,
        ioArrayProducer->getElementDesc(),
        sharedValueProducer->getValueDesc()
        ),
      sharedValueProducer
      ).take();
  }

  static void MRCreateReduce4(
    void (*operator_)(...),
    size_t numParams,
    MR::ArrayProducer *&inputArrayProducer,
    CG::ValueProducerAdapter *outputValueProducerAdapter,
    MR::ValueProducer const *&valueProducer
    )
  {
    if ( valueProducer )
      valueProducer->release();
    valueProducer = MR::Reduce::Create(
      inputArrayProducer,
      MR::ReduceOperator::Create(
        operator_,
        numParams,
        inputArrayProducer->getElementDesc(),
        RC::ConstHandle<RT::ValueProducerDesc>::StaticCast( outputValueProducerAdapter->getDesc() )->getValueDesc(),
        0
        ),
      0
      ).take();
  }

  static void MRCreateReduce5(
    void (*operator_)(...),
    size_t numParams,
    MR::ArrayProducer *&inputArrayProducer,
    CG::ValueProducerAdapter *outputValueProducerAdapter,
    MR::ValueProducer *&sharedValueProducer,
    MR::ValueProducer const *&valueProducer
    )
  {
    if ( valueProducer )
      valueProducer->release();
    valueProducer = MR::Reduce::Create(
      inputArrayProducer,
      MR::ReduceOperator::Create(
        operator_,
        numParams,
        inputArrayProducer->getElementDesc(),
        RC::ConstHandle<RT::ValueProducerDesc>::StaticCast( outputValueProducerAdapter->getDesc() )->getValueDesc(),
        sharedValueProducer->getValueDesc()
        ),
      sharedValueProducer
      ).take();
  }

  namespace KL
  {
    class SymbolNameToAddressMap : public std::map<std::string, void *>
    {
    public:
    
      SymbolNameToAddressMap()
      {
        SymbolNameToAddressMap &symbolNameToAddressMap = *this;
        symbolNameToAddressMap["malloc"] = (void *)&malloc;
        symbolNameToAddressMap["realloc"] = (void *)&realloc;
        symbolNameToAddressMap["free"] = (void *)&free;
        symbolNameToAddressMap["fp32_acos"] = (void *)&acosf;
        symbolNameToAddressMap["fp64_acos"] = (void *)&fp64_acos;
        symbolNameToAddressMap["fp32_asin"] = (void *)&asinf;
        symbolNameToAddressMap["fp64_asin"] = (void *)&fp64_asin;
        symbolNameToAddressMap["fp32_atan"] = (void *)&atanf;
        symbolNameToAddressMap["fp64_atan"] = (void *)&fp64_atan;
        symbolNameToAddressMap["fp32_atan2"] = (void *)&atan2f;
        symbolNameToAddressMap["fp64_atan2"] = (void *)&fp64_atan2;
        symbolNameToAddressMap["fp32_sin"] = (void *)&sinf;
        symbolNameToAddressMap["fp64_sin"] = (void *)&fp64_sin;
        symbolNameToAddressMap["fp32_cos"] = (void *)&cosf;
        symbolNameToAddressMap["fp64_cos"] = (void *)&fp64_cos;
        symbolNameToAddressMap["fp32_tan"] = (void *)&tanf;
        symbolNameToAddressMap["fp64_tan"] = (void *)&fp64_tan;
        symbolNameToAddressMap["fp32_pow"] = (void *)&powf;
        symbolNameToAddressMap["fp64_pow"] = (void *)&fp64_pow;
#if defined(FABRIC_OS_WINDOWS)
        symbolNameToAddressMap["fp32_round"] = (void *)&imp_roundf;
        symbolNameToAddressMap["fp64_round"] = (void *)&imp_round;
#else
        symbolNameToAddressMap["fp32_round"] = (void *)&roundf;
        symbolNameToAddressMap["fp64_round"] = (void *)&round;
#endif
        symbolNameToAddressMap["fp32_ceil"] =( void *)&ceilf;
        symbolNameToAddressMap["fp64_ceil"] =( void *)&fp64_ceil;
        symbolNameToAddressMap["fp32_floor"] =( void *)&floorf;
        symbolNameToAddressMap["fp64_floor"] =( void *)&fp64_floor;
        symbolNameToAddressMap["fp32_fabs"] =( void *)&fabsf;
        symbolNameToAddressMap["fp64_fabs"] =( void *)&fp64_fabs;
        symbolNameToAddressMap["fp32_sqrt"] =( void *)&sqrtf;
        symbolNameToAddressMap["fp64_sqrt"] =( void *)&fp64_sqrt;
        symbolNameToAddressMap["sinf"] = (void *)&sinf;
        symbolNameToAddressMap["sin"] = (void *)&fp64_sin;
        symbolNameToAddressMap["cosf"] = (void *)&cosf;
        symbolNameToAddressMap["cos"] = (void *)&fp64_cos;
        symbolNameToAddressMap["powf"] = (void *)&powf;
        symbolNameToAddressMap["pow"] = (void *)&fp64_pow;
        symbolNameToAddressMap["fmodf"] = (void *)&fmodf;
        symbolNameToAddressMap["fmod"] = (void *)&fp64_fmod;
        symbolNameToAddressMap["fp32_log10"] = (void *)&log10f;
#if defined(FABRIC_OS_WINDOWS)
        symbolNameToAddressMap["fp64_log10"] = (void *)&log10l;
#else
        symbolNameToAddressMap["fp64_log10"] = (void *)&log10;
#endif
        symbolNameToAddressMap["fp32_log"] = (void *)&logf;
#if defined(FABRIC_OS_WINDOWS)
        symbolNameToAddressMap["fp64_log"] = (void *)&logl;
#else
        symbolNameToAddressMap["fp64_log"] = (void *)&log;
#endif
        symbolNameToAddressMap["fp32_exp"] = (void *)&expf;
#if defined(FABRIC_OS_WINDOWS)
        symbolNameToAddressMap["fp64_exp"] = (void *)&expl;
#else
        symbolNameToAddressMap["fp64_exp"] = (void *)&exp;
#endif
#if defined(FABRIC_OS_WINDOWS)
        symbolNameToAddressMap["_chkstk"] = (void *)&_chkstk;
#endif
        symbolNameToAddressMap["__MR_CreateValueCache"] = (void *)&MRCreateValueCache;
        symbolNameToAddressMap["__MR_CreateArrayCache"] = (void *)&MRCreateArrayCache;
        symbolNameToAddressMap["__MR_CreateConstValue"] = (void *)&MRCreateConstValue;
        symbolNameToAddressMap["__MR_CreateValueGenerator_1"] = (void *)&MRCreateValueGenerator1;
        symbolNameToAddressMap["__MR_CreateValueGenerator_2"] = (void *)&MRCreateValueGenerator2;
        symbolNameToAddressMap["__MR_CreateValueMap_2"] = (void *)&MRCreateValueMap2;
        symbolNameToAddressMap["__MR_CreateValueMap_3"] = (void *)&MRCreateValueMap3;
        symbolNameToAddressMap["__MR_CreateValueTransform_1"] = (void *)&MRCreateValueTransform1;
        symbolNameToAddressMap["__MR_CreateValueTransform_2"] = (void *)&MRCreateValueTransform2;
        symbolNameToAddressMap["__MR_CreateConstArray"] = (void *)&MRCreateConstArray;
        symbolNameToAddressMap["__MR_CreateArrayGenerator_3"] = (void *)&MRCreateArrayGenerator3;
        symbolNameToAddressMap["__MR_CreateArrayGenerator_4"] = (void *)&MRCreateArrayGenerator4;
        symbolNameToAddressMap["__MR_CreateArrayMap_4"] = (void *)&MRCreateArrayMap4;
        symbolNameToAddressMap["__MR_CreateArrayMap_5"] = (void *)&MRCreateArrayMap5;
        symbolNameToAddressMap["__MR_CreateArrayTransform_3"] = (void *)&MRCreateArrayTransform3;
        symbolNameToAddressMap["__MR_CreateArrayTransform_4"] = (void *)&MRCreateArrayTransform4;
        symbolNameToAddressMap["__MR_CreateReduce_4"] = (void *)&MRCreateReduce4;
        symbolNameToAddressMap["__MR_CreateReduce_5"] = (void *)&MRCreateReduce5;
      }
    };

    static SymbolNameToAddressMap s_symbolNameToAddressMap;

    void *LookupExternalSymbol( std::string const &functionName )
    {
      SymbolNameToAddressMap::const_iterator it = s_symbolNameToAddressMap.find( functionName );
      if ( it != s_symbolNameToAddressMap.end() )
        return it->second;
      else return 0;
    }
  };
};
