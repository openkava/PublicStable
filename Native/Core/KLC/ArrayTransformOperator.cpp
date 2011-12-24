/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/ArrayTransformOperator.h>
#include <Fabric/Core/KLC/Executable.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/SizeAdapter.h>
#include <Fabric/Core/AST/Operator.h>
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/AST/ParamVector.h>
#include <Fabric/Core/RT/Impl.h>
#include <Fabric/Core/RT/IntegerDesc.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace KLC
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( ArrayTransformOperator, Operator )

    RC::Handle<ArrayTransformOperator> ArrayTransformOperator::Create(
      RC::ConstHandle<Executable> const &executable,
      RC::ConstHandle<AST::Operator> const &astOperator,
      GenericFunctionPtr functionPtr
      )
    {
      return new ArrayTransformOperator(
        FABRIC_GC_OBJECT_MY_CLASS,
        executable,
        astOperator,
        functionPtr
        );
    }

#define FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK() \
          do \
          { \
            if ( numParams == 4 ) \
              m_call3 = &ArrayTransformOperator::call3; \
            else if ( numParams == 3 ) \
              m_call2 = &ArrayTransformOperator::call2; \
            else \
              m_call1 = &ArrayTransformOperator::call1; \
          } while ( false )

#define FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( sharedTypeName ) \
          do \
          { \
            if ( numParams == 5 ) \
              m_call4 = &ArrayTransformOperator::call4##sharedTypeName; \
            else FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK(); \
          } while ( false )

#define FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_SD( sharedDesc ) \
        do \
        { \
          if ( sharedDesc ) \
          { \
            RT::ImplType sharedType = sharedDesc->getType(); \
            if ( RT::isBoolean(sharedType) ) \
              FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( Boolean ); \
            else if ( RT::isInteger(sharedType) ) \
            { \
              size_t size = sharedDesc->getAllocSize(); \
              bool isSigned = RC::ConstHandle<RT::IntegerDesc>::StaticCast( sharedDesc )->isSigned(); \
              switch ( size ) \
              { \
                case 1: \
                  if ( isSigned ) \
                    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( SInt8 ); \
                  else \
                    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( UInt8 ); \
                  break; \
                case 2: \
                  if ( isSigned ) \
                    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( SInt16 ); \
                  else \
                    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( UInt16 ); \
                  break; \
                case 4: \
                  if ( isSigned ) \
                    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( SInt32 ); \
                  else \
                    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( UInt32 ); \
                  break; \
                case 8: \
                  if ( isSigned ) \
                    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( SInt64 ); \
                  else \
                    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( UInt64 ); \
                  break; \
                default: \
                  FABRIC_ASSERT( false && "Unsupported integer size" ); \
              } \
            } \
            else if ( RT::isFloat(sharedType) ) \
            { \
              if ( sharedDesc->getAllocSize() == 8 ) \
                FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( Float64 ); \
              else \
                FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( Float32 ); \
            } \
            else FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_STN( Default ); \
          } \
          else FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK(); \
        } while ( false )

    ArrayTransformOperator::ArrayTransformOperator(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<Executable> const &executable,
      RC::ConstHandle<AST::Operator> const &astOperator,
      GenericFunctionPtr functionPtr
      )
      : Operator(
        FABRIC_GC_OBJECT_CLASS_ARG,
        executable,
        astOperator,
        functionPtr
        )
      , m_call1( 0 )
      , m_call2( 0 )
      , m_call3( 0 )
      , m_call4( 0 )
    {
      static const Exception exception("operator must have prototype: in InputType inputData, io OutputType outputData[, in Size index[, in Size count[, in SharedType sharedData]]]");
      
      if ( astOperator )
      {
        RC::Handle<CG::Manager> cgManager = executable->getCGManager();
        
        RC::ConstHandle<AST::ParamVector> params = astOperator->getParams( cgManager );
        size_t numParams = params->size();
        if ( numParams < 1 || numParams > 4 )
          throw exception;
        
        RC::ConstHandle<AST::Param> valueParam = params->get(0);
        if ( valueParam->getUsage() != CG::USAGE_LVALUE )
          throw exception;
        m_valueAdapter = valueParam->getAdapter( cgManager );
        
        if ( numParams >= 2 )
        {
          RC::ConstHandle<AST::Param> indexParam = params->get(1);
          if ( indexParam->getUsage() != CG::USAGE_RVALUE
            || indexParam->getAdapter( cgManager ) != cgManager->getSizeAdapter()
            )
            throw exception;
        
          if ( numParams >= 3 )
          {
            RC::ConstHandle<AST::Param> countParam = params->get(2);
            if ( countParam->getUsage() != CG::USAGE_RVALUE
              || countParam->getAdapter( cgManager ) != cgManager->getSizeAdapter()
              )
              throw exception;
          
            if ( numParams >= 4 )
            {
              RC::ConstHandle<AST::Param> sharedParam = params->get(3);
              if ( sharedParam->getUsage() != CG::USAGE_RVALUE )
                throw exception;
              m_sharedAdapter = sharedParam->getAdapter( cgManager );
            }
          }
        }

        RC::ConstHandle<RT::Desc> sharedDesc;
        if ( m_sharedAdapter )
          sharedDesc = m_sharedAdapter->getDesc();
        FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_PICK_SD( sharedDesc );
      }
    }
        
    RC::ConstHandle<RT::Desc> ArrayTransformOperator::getValueDesc() const
    {
      RC::ConstHandle<RT::Desc> result;
      if ( m_valueAdapter )
        result = m_valueAdapter->getDesc();
      return result;
    }
    
    RC::ConstHandle<RT::Desc> ArrayTransformOperator::getSharedDesc() const
    {
      RC::ConstHandle<RT::Desc> result;
      if ( m_sharedAdapter )
        result = m_sharedAdapter->getDesc();
      return result;
    }
    
#define FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_ST( sharedTypeName, sharedTypeCType ) \
    void ArrayTransformOperator::call4##sharedTypeName( void *valueData, size_t index, size_t count, void const *sharedData ) \
    { \
      ((void (*)( void *, size_t, size_t, sharedTypeCType ))getGenericFunctionPtr())( valueData, index, count, *(sharedTypeCType const *)sharedData ); \
    } \
    
#define FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_DEFAULT() \
    void ArrayTransformOperator::call4Default( void *valueData, size_t index, size_t count, void const *sharedData ) \
    { \
      ((void (*)( void *, size_t, size_t, void const * ))getGenericFunctionPtr())( valueData, index, count, sharedData ); \
    } \

#define FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL() \
    void ArrayTransformOperator::call1( void *valueData ) \
    { \
      ((void (*)( void * ))getGenericFunctionPtr())( valueData ); \
    } \
    void ArrayTransformOperator::call2( void *valueData, size_t index ) \
    { \
      ((void (*)( void *, size_t ))getGenericFunctionPtr())( valueData, index ); \
    } \
    void ArrayTransformOperator::call3( void *valueData, size_t index, size_t count ) \
    { \
      ((void (*)( void *, size_t, size_t ))getGenericFunctionPtr())( valueData, index, count ); \
    } \
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_ST( Boolean, bool ) \
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_ST( UInt8, uint8_t ) \
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_ST( SInt8, int8_t ) \
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_ST( UInt16, uint16_t ) \
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_ST( SInt16, int16_t ) \
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_ST( UInt32, uint32_t ) \
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_ST( SInt32, int32_t ) \
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_ST( UInt64, uint64_t ) \
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_ST( SInt64, int64_t ) \
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_ST( Float32, float ) \
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_ST( Float64, double ) \
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL_DEFAULT() \
    
    FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_IMPL()
    
    void ArrayTransformOperator::call(
      void *valueData
      ) const
    {
      if ( m_call1 )
        (const_cast<ArrayTransformOperator *>(this)->*m_call1)( valueData );
      else throw Exception( "operator is invalid" );
    }
    
    void ArrayTransformOperator::call(
      void *valueData,
      size_t index
      ) const
    {
      if ( m_call2 )
        (const_cast<ArrayTransformOperator *>(this)->*m_call2)( valueData, index );
      else throw Exception( "operator is invalid" );
    }
    
    void ArrayTransformOperator::call(
      void *valueData,
      size_t index,
      size_t count
      ) const
    {
      if ( m_call3 )
        (const_cast<ArrayTransformOperator *>(this)->*m_call3)( valueData, index, count );
      else throw Exception( "operator is invalid" );
    }
    
    void ArrayTransformOperator::call(
      void *valueData,
      size_t index,
      size_t count,
      void const *sharedData
      ) const
    {
      if ( m_call4 )
        (const_cast<ArrayTransformOperator *>(this)->*m_call4)( valueData, index, count, sharedData );
      else throw Exception( "operator is invalid" );
    }
  }
}
