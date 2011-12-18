/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/MapOperator.h>
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
    FABRIC_GC_OBJECT_CLASS_IMPL( MapOperator, Operator )
      
    RC::Handle<MapOperator> MapOperator::Create(
      RC::ConstHandle<Executable> const &executable,
      RC::ConstHandle<AST::Operator> const &astOperator,
      GenericFunctionPtr functionPtr
      )
    {
      return new MapOperator(
        FABRIC_GC_OBJECT_MY_CLASS,
        executable,
        astOperator,
        functionPtr
        );
    }

    MapOperator::MapOperator(
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
    {
      static const Exception exception("operator must take 3 parameters: in Size index, in InputType inputData, io OutputType outputData");
      
      if ( astOperator )
      {
        RC::Handle<CG::Manager> cgManager = executable->getCGManager();
        
        RC::ConstHandle<AST::ParamVector> params = astOperator->getParams( cgManager );
        if ( params->size() != 3 )
          throw exception;
        
        RC::ConstHandle<AST::Param> sizeParam = params->get(0);
        if ( sizeParam->getUsage() != CG::USAGE_RVALUE
          || sizeParam->getAdapter( cgManager ) != cgManager->getSizeAdapter()
          )
          throw exception;
        
        RC::ConstHandle<AST::Param> inputParam = params->get(1);
        if ( inputParam->getUsage() != CG::USAGE_RVALUE )
          throw exception;
        m_inputAdapter = inputParam->getAdapter( cgManager );
        
        RC::ConstHandle<AST::Param> outputParam = params->get(2);
        if ( outputParam->getUsage() != CG::USAGE_LVALUE )
          throw exception;
        m_outputAdapter = outputParam->getAdapter( cgManager );

        RC::ConstHandle<RT::Desc> inputDesc = m_inputAdapter->getDesc();
        RT::ImplType inputType = inputDesc->getType();
        if ( RT::isBoolean(inputType) )
        {
          m_call = &MapOperator::callBoolean;
        }
        else if ( RT::isInteger(inputType) )
        {
          size_t size = inputDesc->getAllocSize();
          bool isSigned = RC::ConstHandle<RT::IntegerDesc>::StaticCast( inputDesc )->isSigned();
          switch ( size )
          {
            case 1:
              m_call = isSigned? &MapOperator::callSInt8: &MapOperator::callUInt8;
              break;
            case 2:
              m_call = isSigned? &MapOperator::callSInt16: &MapOperator::callUInt16;
              break;
            case 4:
              m_call = isSigned? &MapOperator::callSInt32: &MapOperator::callUInt32;
              break;
            case 8:
              m_call = isSigned? &MapOperator::callSInt64: &MapOperator::callUInt64;
              break;
            default:
              FABRIC_ASSERT( false && "Unsupported integer size" );
          }
        }
        else if ( RT::isFloat( m_inputAdapter->getType() ) )
        {
          if ( m_inputAdapter->getImpl()->getAllocSize() == 8 )
            m_call = &MapOperator::callFloat64;
          else
            m_call = &MapOperator::callFloat32;
        }
        else m_call = &MapOperator::callRef;
      }
    }
    
    char const *MapOperator::getKind() const
    {
      return "MapOperator";
    }
        
    RC::ConstHandle<RT::Desc> MapOperator::getInputDesc() const
    {
      RC::ConstHandle<RT::Desc> result;
      if ( m_inputAdapter )
        result = m_inputAdapter->getDesc();
      return result;
    }
    
    RC::ConstHandle<RT::Desc> MapOperator::getOutputDesc() const
    {
      RC::ConstHandle<RT::Desc> result;
      if ( m_outputAdapter )
        result = m_outputAdapter->getDesc();
      return result;
    }

    void MapOperator::callRef( size_t index, void const *inputData, void *outputData )
    {
      getGenericFunctionPtr()( index, inputData, outputData );
    }
    
    void MapOperator::callBoolean( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, bool, void * ))getGenericFunctionPtr())( index, *(bool const *)inputData, outputData );
    }
    
    void MapOperator::callUInt8( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, uint8_t, void * ))getGenericFunctionPtr())( index, *(uint8_t const *)inputData, outputData );
    }
    
    void MapOperator::callSInt8( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, int8_t, void * ))getGenericFunctionPtr())( index, *(int8_t const *)inputData, outputData );
    }
    
    void MapOperator::callUInt16( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, uint16_t, void * ))getGenericFunctionPtr())( index, *(uint16_t const *)inputData, outputData );
    }
    
    void MapOperator::callSInt16( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, int16_t, void * ))getGenericFunctionPtr())( index, *(int16_t const *)inputData, outputData );
    }
    
    void MapOperator::callUInt32( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, uint32_t, void * ))getGenericFunctionPtr())( index, *(uint32_t const *)inputData, outputData );
    }
    
    void MapOperator::callSInt32( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, int32_t, void * ))getGenericFunctionPtr())( index, *(int32_t const *)inputData, outputData );
    }
    
    void MapOperator::callUInt64( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, uint64_t, void * ))getGenericFunctionPtr())( index, *(uint64_t const *)inputData, outputData );
    }
    
    void MapOperator::callSInt64( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, int64_t, void * ))getGenericFunctionPtr())( index, *(int64_t const *)inputData, outputData );
    }
    
    void MapOperator::callFloat32( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, float, void * ))getGenericFunctionPtr())( index, *(float const *)inputData, outputData );
    }
    
    void MapOperator::callFloat64( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, double, void * ))getGenericFunctionPtr())( index, *(double const *)inputData, outputData );
    }
    
    void MapOperator::callString( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, void *, void * ))getGenericFunctionPtr())( index, *(void * const *)inputData, outputData );
    }
    
    void MapOperator::call(
      size_t index,
      void const *inputData,
      void *outputData
      ) const
    {
      (const_cast<MapOperator *>(this)->*m_call)( index, inputData, outputData );
    }
  }
}
