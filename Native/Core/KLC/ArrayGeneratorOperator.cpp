/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/ArrayGeneratorOperator.h>
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
    FABRIC_GC_OBJECT_CLASS_IMPL( ArrayGeneratorOperator, Operator )
      
    RC::Handle<ArrayGeneratorOperator> ArrayGeneratorOperator::Create(
      RC::ConstHandle<Executable> const &executable,
      RC::ConstHandle<AST::Operator> const &astOperator,
      GenericFunctionPtr functionPtr
      )
    {
      return new ArrayGeneratorOperator(
        FABRIC_GC_OBJECT_MY_CLASS,
        executable,
        astOperator,
        functionPtr
        );
    }

    ArrayGeneratorOperator::ArrayGeneratorOperator(
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
      static const Exception exception("operator must take 2 parameters: in Size index, io OutputType outputData");
      
      if ( astOperator )
      {
        RC::Handle<CG::Manager> cgManager = executable->getCGManager();
        
        RC::ConstHandle<AST::ParamVector> params = astOperator->getParams( cgManager );
        if ( params->size() != 2 )
          throw exception;
        
        RC::ConstHandle<AST::Param> sizeParam = params->get(0);
        if ( sizeParam->getUsage() != CG::USAGE_RVALUE
          || sizeParam->getAdapter( cgManager ) != cgManager->getSizeAdapter()
          )
          throw exception;
        
        RC::ConstHandle<AST::Param> outputParam = params->get(1);
        if ( outputParam->getUsage() != CG::USAGE_LVALUE )
          throw exception;
        m_outputAdapter = outputParam->getAdapter( cgManager );

        /*
        RC::ConstHandle<RT::Desc> inputDesc = m_inputAdapter->getDesc();
        RT::ImplType inputType = inputDesc->getType();
        if ( RT::isBoolean(inputType) )
        {
          m_call = &ArrayGeneratorOperator::callBoolean;
        }
        else if ( RT::isInteger(inputType) )
        {
          size_t size = inputDesc->getAllocSize();
          bool isSigned = RC::ConstHandle<RT::IntegerDesc>::StaticCast( inputDesc )->isSigned();
          switch ( size )
          {
            case 1:
              m_call = isSigned? &ArrayGeneratorOperator::callSInt8: &ArrayGeneratorOperator::callUInt8;
              break;
            case 2:
              m_call = isSigned? &ArrayGeneratorOperator::callSInt16: &ArrayGeneratorOperator::callUInt16;
              break;
            case 4:
              m_call = isSigned? &ArrayGeneratorOperator::callSInt32: &ArrayGeneratorOperator::callUInt32;
              break;
            case 8:
              m_call = isSigned? &ArrayGeneratorOperator::callSInt64: &ArrayGeneratorOperator::callUInt64;
              break;
            default:
              FABRIC_ASSERT( false && "Unsupported integer size" );
          }
        }
        else if ( RT::isFloat( m_inputAdapter->getType() ) )
        {
          if ( m_inputAdapter->getImpl()->getAllocSize() == 8 )
            m_call = &ArrayGeneratorOperator::callFloat64;
          else
            m_call = &ArrayGeneratorOperator::callFloat32;
        }
        else m_call = &ArrayGeneratorOperator::callRef;
        */
      }
    }
    
    char const *ArrayGeneratorOperator::getKind() const
    {
      return "ArrayGeneratorOperator";
    }
    
    RC::ConstHandle<RT::Desc> ArrayGeneratorOperator::getOutputDesc() const
    {
      return m_outputAdapter->getDesc();
    }

    /*
    void ArrayGeneratorOperator::callRef( size_t index, void const *inputData, void *outputData )
    {
      getGenericFunctionPtr()( index, inputData, outputData );
    }
    
    void ArrayGeneratorOperator::callBoolean( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, bool, void * ))getGenericFunctionPtr())( index, *(bool const *)inputData, outputData );
    }
    
    void ArrayGeneratorOperator::callUInt8( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, uint8_t, void * ))getGenericFunctionPtr())( index, *(uint8_t const *)inputData, outputData );
    }
    
    void ArrayGeneratorOperator::callSInt8( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, int8_t, void * ))getGenericFunctionPtr())( index, *(int8_t const *)inputData, outputData );
    }
    
    void ArrayGeneratorOperator::callUInt16( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, uint16_t, void * ))getGenericFunctionPtr())( index, *(uint16_t const *)inputData, outputData );
    }
    
    void ArrayGeneratorOperator::callSInt16( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, int16_t, void * ))getGenericFunctionPtr())( index, *(int16_t const *)inputData, outputData );
    }
    
    void ArrayGeneratorOperator::callUInt32( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, uint32_t, void * ))getGenericFunctionPtr())( index, *(uint32_t const *)inputData, outputData );
    }
    
    void ArrayGeneratorOperator::callSInt32( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, int32_t, void * ))getGenericFunctionPtr())( index, *(int32_t const *)inputData, outputData );
    }
    
    void ArrayGeneratorOperator::callUInt64( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, uint64_t, void * ))getGenericFunctionPtr())( index, *(uint64_t const *)inputData, outputData );
    }
    
    void ArrayGeneratorOperator::callSInt64( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, int64_t, void * ))getGenericFunctionPtr())( index, *(int64_t const *)inputData, outputData );
    }
    
    void ArrayGeneratorOperator::callFloat32( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, float, void * ))getGenericFunctionPtr())( index, *(float const *)inputData, outputData );
    }
    
    void ArrayGeneratorOperator::callFloat64( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, double, void * ))getGenericFunctionPtr())( index, *(double const *)inputData, outputData );
    }
    
    void ArrayGeneratorOperator::callString( size_t index, void const *inputData, void *outputData )
    {
      ((void (*)( size_t, void *, void * ))getGenericFunctionPtr())( index, *(void * const *)inputData, outputData );
    }
    */
    
    void ArrayGeneratorOperator::call(
      size_t index,
      void *outputData
      ) const
    {
      //(const_cast<ArrayGeneratorOperator *>(this)->*m_call)( index, outputData );
      getGenericFunctionPtr()( index, outputData );
    }
  }
}
