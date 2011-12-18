/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/ReduceOperator.h>
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
    FABRIC_GC_OBJECT_CLASS_IMPL( ReduceOperator, Operator )
      
    RC::Handle<ReduceOperator> ReduceOperator::Create(
      RC::ConstHandle<Executable> const &executable,
      RC::ConstHandle<AST::Operator> const &astOperator,
      GenericFunctionPtr functionPtr
      )
    {
      return new ReduceOperator(
        FABRIC_GC_OBJECT_MY_CLASS,
        executable,
        astOperator,
        functionPtr
        );
    }

#define FABRIC_KLC_REDUCE_OPERATOR_PICK( typeName ) \
          do \
          { \
            if ( numParams == 4 ) \
              m_call4 = &ReduceOperator::call4_##typeName; \
            else if ( numParams == 3 ) \
              m_call3 = &ReduceOperator::call3_##typeName; \
            else \
              m_call2 = &ReduceOperator::call2_##typeName; \
          } while ( false )

    ReduceOperator::ReduceOperator(
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
      , m_call2( 0 )
      , m_call3( 0 )
      , m_call4( 0 )
    {
      static const Exception exception("operator must have prototype: in InputType inputData, io OutputType outputData[, in Size index[, in Size count]]");
      
      if ( astOperator )
      {
        RC::Handle<CG::Manager> cgManager = executable->getCGManager();
        
        RC::ConstHandle<AST::ParamVector> params = astOperator->getParams( cgManager );
        size_t numParams = params->size();
        if ( numParams < 2 || numParams > 4 )
          throw exception;
        
        RC::ConstHandle<AST::Param> inputParam = params->get(0);
        if ( inputParam->getUsage() != CG::USAGE_RVALUE )
          throw exception;
        m_inputAdapter = inputParam->getAdapter( cgManager );
        
        RC::ConstHandle<AST::Param> outputParam = params->get(1);
        if ( outputParam->getUsage() != CG::USAGE_LVALUE )
          throw exception;
        m_outputAdapter = outputParam->getAdapter( cgManager );
        
        if ( numParams >= 3 )
        {
          RC::ConstHandle<AST::Param> sizeParam = params->get(2);
          if ( sizeParam->getUsage() != CG::USAGE_RVALUE
            || sizeParam->getAdapter( cgManager ) != cgManager->getSizeAdapter()
            )
            throw exception;
        }

        RC::ConstHandle<RT::Desc> inputDesc = m_inputAdapter->getDesc();
        RT::ImplType inputType = inputDesc->getType();
        if ( RT::isBoolean(inputType) )
          FABRIC_KLC_REDUCE_OPERATOR_PICK( Boolean );
        else if ( RT::isInteger(inputType) )
        {
          size_t size = inputDesc->getAllocSize();
          bool isSigned = RC::ConstHandle<RT::IntegerDesc>::StaticCast( inputDesc )->isSigned();
          switch ( size )
          {
            case 1:
              if ( isSigned )
                FABRIC_KLC_REDUCE_OPERATOR_PICK( SInt8 );
              else
                FABRIC_KLC_REDUCE_OPERATOR_PICK( UInt8 );
              break;
            case 2:
              if ( isSigned )
                FABRIC_KLC_REDUCE_OPERATOR_PICK( SInt16 );
              else
                FABRIC_KLC_REDUCE_OPERATOR_PICK( UInt16 );
              break;
            case 4:
              if ( isSigned )
                FABRIC_KLC_REDUCE_OPERATOR_PICK( SInt32 );
              else
                FABRIC_KLC_REDUCE_OPERATOR_PICK( UInt32 );
              break;
            case 8:
              if ( isSigned )
                FABRIC_KLC_REDUCE_OPERATOR_PICK( SInt64 );
              else
                FABRIC_KLC_REDUCE_OPERATOR_PICK( UInt64 );
              break;
            default:
              FABRIC_ASSERT( false && "Unsupported integer size" );
          }
        }
        else if ( RT::isFloat( m_inputAdapter->getType() ) )
        {
          if ( m_inputAdapter->getImpl()->getAllocSize() == 8 )
            FABRIC_KLC_REDUCE_OPERATOR_PICK( Float64 );
          else
            FABRIC_KLC_REDUCE_OPERATOR_PICK( Float32 );
        }
        else if ( RT::isString( m_inputAdapter->getType() ) )
          FABRIC_KLC_REDUCE_OPERATOR_PICK( String );
        else FABRIC_KLC_REDUCE_OPERATOR_PICK( Default );
      }
    }
    
    char const *ReduceOperator::getKind() const
    {
      return "ReduceOperator";
    }
        
    RC::ConstHandle<RT::Desc> ReduceOperator::getInputDesc() const
    {
      RC::ConstHandle<RT::Desc> result;
      if ( m_inputAdapter )
        result = m_inputAdapter->getDesc();
      return result;
    }
    
    RC::ConstHandle<RT::Desc> ReduceOperator::getOutputDesc() const
    {
      RC::ConstHandle<RT::Desc> result;
      if ( m_outputAdapter )
        result = m_outputAdapter->getDesc();
      return result;
    }
    
#define FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL(typeName, typeType) \
    void ReduceOperator::call2_##typeName( void const *inputData, void *outputData ) \
    { \
      ((void (*)( typeType, void * ))getGenericFunctionPtr())( *(typeType const *)inputData, outputData ); \
    } \
    void ReduceOperator::call3_##typeName( void const *inputData, void *outputData, size_t index ) \
    { \
      ((void (*)( typeType, void *, size_t ))getGenericFunctionPtr())( *(typeType const *)inputData, outputData, index ); \
    } \
    void ReduceOperator::call4_##typeName( void const *inputData, void *outputData, size_t index, size_t count ) \
    { \
      ((void (*)( typeType, void *, size_t, size_t ))getGenericFunctionPtr())( *(typeType const *)inputData, outputData, index, count ); \
    }

    FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL( Boolean, bool )
    FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL( UInt8, uint8_t )
    FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL( SInt8, int8_t )
    FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL( UInt16, uint16_t )
    FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL( SInt16, int16_t )
    FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL( UInt32, uint32_t )
    FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL( SInt32, int32_t )
    FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL( UInt64, uint64_t )
    FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL( SInt64, int64_t )
    FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL( Float32, float )
    FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL( Float64, double )
    FABRIC_KLC_REDUCE_OPERATOR_CALL_IMPL( String, void * )
    
    void ReduceOperator::call2_Default( void const *inputData, void *outputData )
    {
      getGenericFunctionPtr()( inputData, outputData );
    }
    
    void ReduceOperator::call3_Default( void const *inputData, void *outputData, size_t index )
    {
      getGenericFunctionPtr()( inputData, outputData, index );
    }
    
    void ReduceOperator::call4_Default( void const *inputData, void *outputData, size_t index, size_t count )
    {
      getGenericFunctionPtr()( inputData, outputData, index, count );
    }
    
    void ReduceOperator::call(
      void const *inputData,
      void *outputData
      ) const
    {
      (const_cast<ReduceOperator *>(this)->*m_call2)( inputData, outputData );
    }
    
    void ReduceOperator::call(
      void const *inputData,
      void *outputData,
      size_t index
      ) const
    {
      (const_cast<ReduceOperator *>(this)->*m_call3)( inputData, outputData, index );
    }
    
    void ReduceOperator::call(
      void const *inputData,
      void *outputData,
      size_t index,
      size_t count
      ) const
    {
      (const_cast<ReduceOperator *>(this)->*m_call4)( inputData, outputData, index, count );
    }
  }
}
