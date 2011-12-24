/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/ArrayIOOperator.h>
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
    FABRIC_GC_OBJECT_CLASS_IMPL( ArrayIOOperator, Operator )

#define FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN( inputTypeName ) \
          do \
          { \
            if ( numParams == 4 ) \
              m_call4 = &ArrayIOOperator::call4##inputTypeName; \
            else if ( numParams == 3 ) \
              m_call3 = &ArrayIOOperator::call3##inputTypeName; \
            else \
              m_call2 = &ArrayIOOperator::call2##inputTypeName; \
          } while ( false )

#define FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, sharedTypeName ) \
          do \
          { \
            if ( numParams == 5 ) \
              m_call5 = &ArrayIOOperator::call5##inputTypeName##sharedTypeName; \
            else FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN( inputTypeName ); \
          } while ( false )

#define FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( inputTypeName, sharedDesc ) \
        do \
        { \
          if ( sharedDesc ) \
          { \
            RT::ImplType sharedType = sharedDesc->getType(); \
            if ( RT::isBoolean(sharedType) ) \
              FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, Boolean ); \
            else if ( RT::isInteger(sharedType) ) \
            { \
              size_t size = sharedDesc->getAllocSize(); \
              bool isSigned = RC::ConstHandle<RT::IntegerDesc>::StaticCast( sharedDesc )->isSigned(); \
              switch ( size ) \
              { \
                case 1: \
                  if ( isSigned ) \
                    FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, SInt8 ); \
                  else \
                    FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, UInt8 ); \
                  break; \
                case 2: \
                  if ( isSigned ) \
                    FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, SInt16 ); \
                  else \
                    FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, UInt16 ); \
                  break; \
                case 4: \
                  if ( isSigned ) \
                    FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, SInt32 ); \
                  else \
                    FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, UInt32 ); \
                  break; \
                case 8: \
                  if ( isSigned ) \
                    FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, SInt64 ); \
                  else \
                    FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, UInt64 ); \
                  break; \
                default: \
                  FABRIC_ASSERT( false && "Unsupported integer size" ); \
              } \
            } \
            else if ( RT::isFloat(sharedType) ) \
            { \
              if ( sharedDesc->getAllocSize() == 8 ) \
                FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, Float64 ); \
              else \
                FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, Float32 ); \
            } \
            else FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_STN( inputTypeName, Default ); \
          } \
          else FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN( inputTypeName ); \
        } while ( false )

#define FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ID_SD( inputDesc, sharedDesc ) \
        do \
        { \
          RT::ImplType inputType = inputDesc->getType(); \
          if ( RT::isBoolean(inputType) ) \
            FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( Boolean, sharedDesc ); \
          else if ( RT::isInteger(inputType) ) \
          { \
            size_t size = inputDesc->getAllocSize(); \
            bool isSigned = RC::ConstHandle<RT::IntegerDesc>::StaticCast( inputDesc )->isSigned(); \
            switch ( size ) \
            { \
              case 1: \
                if ( isSigned ) \
                  FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( SInt8, sharedDesc ); \
                else \
                  FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( UInt8, sharedDesc ); \
                break; \
              case 2: \
                if ( isSigned ) \
                  FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( SInt16, sharedDesc ); \
                else \
                  FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( UInt16, sharedDesc ); \
                break; \
              case 4: \
                if ( isSigned ) \
                  FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( SInt32, sharedDesc ); \
                else \
                  FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( UInt32, sharedDesc ); \
                break; \
              case 8: \
                if ( isSigned ) \
                  FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( SInt64, sharedDesc ); \
                else \
                  FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( UInt64, sharedDesc ); \
                break; \
              default: \
                FABRIC_ASSERT( false && "Unsupported integer size" ); \
            } \
          } \
          else if ( RT::isFloat(inputType) ) \
          { \
            if ( inputDesc->getAllocSize() == 8 ) \
              FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( Float64, sharedDesc ); \
            else \
              FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( Float32, sharedDesc ); \
          } \
          else FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ITN_SD( Default, sharedDesc ); \
        } while ( false )

    ArrayIOOperator::ArrayIOOperator(
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
      , m_call5( 0 )
    {
      static const Exception exception("operator must have prototype: in InputType inputData, io OutputType outputData[, in Size index[, in Size count[, in SharedType sharedData]]]");
      
      if ( astOperator )
      {
        RC::Handle<CG::Manager> cgManager = executable->getCGManager();
        
        RC::ConstHandle<AST::ParamVector> params = astOperator->getParams( cgManager );
        size_t numParams = params->size();
        if ( numParams < 2 || numParams > 5 )
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
          RC::ConstHandle<AST::Param> indexParam = params->get(2);
          if ( indexParam->getUsage() != CG::USAGE_RVALUE
            || indexParam->getAdapter( cgManager ) != cgManager->getSizeAdapter()
            )
            throw exception;
        
          if ( numParams >= 4 )
          {
            RC::ConstHandle<AST::Param> countParam = params->get(3);
            if ( countParam->getUsage() != CG::USAGE_RVALUE
              || countParam->getAdapter( cgManager ) != cgManager->getSizeAdapter()
              )
              throw exception;
          
            if ( numParams >= 5 )
            {
              RC::ConstHandle<AST::Param> sharedParam = params->get(4);
              if ( sharedParam->getUsage() != CG::USAGE_RVALUE )
                throw exception;
              m_sharedAdapter = sharedParam->getAdapter( cgManager );
            }
          }
        }

        RC::ConstHandle<RT::Desc> inputDesc = m_inputAdapter->getDesc();
        RC::ConstHandle<RT::Desc> sharedDesc;
        if ( m_sharedAdapter )
          sharedDesc = m_sharedAdapter->getDesc();
        FABRIC_KLC_ARRAY_IO_OPERATOR_PICK_ID_SD( inputDesc, sharedDesc );
      }
    }
        
    RC::ConstHandle<RT::Desc> ArrayIOOperator::getInputDesc() const
    {
      RC::ConstHandle<RT::Desc> result;
      if ( m_inputAdapter )
        result = m_inputAdapter->getDesc();
      return result;
    }
    
    RC::ConstHandle<RT::Desc> ArrayIOOperator::getOutputDesc() const
    {
      RC::ConstHandle<RT::Desc> result;
      if ( m_outputAdapter )
        result = m_outputAdapter->getDesc();
      return result;
    }
    
    RC::ConstHandle<RT::Desc> ArrayIOOperator::getSharedDesc() const
    {
      RC::ConstHandle<RT::Desc> result;
      if ( m_sharedAdapter )
        result = m_sharedAdapter->getDesc();
      return result;
    }
    
#define FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_ST( inputTypeName, inputTypeCType, sharedTypeName, sharedTypeCType ) \
    void ArrayIOOperator::call5##inputTypeName##sharedTypeName( void const *inputData, void *outputData, size_t index, size_t count, void const *sharedData ) \
    { \
      ((void (*)( inputTypeCType, void *, size_t, size_t, sharedTypeCType ))getGenericFunctionPtr())( *(inputTypeCType const *)inputData, outputData, index, count, *(sharedTypeCType const *)sharedData ); \
    } \
    
#define FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_DEFAULT( inputTypeName, inputTypeCType ) \
    void ArrayIOOperator::call5##inputTypeName##Default( void const *inputData, void *outputData, size_t index, size_t count, void const *sharedData ) \
    { \
      ((void (*)( inputTypeCType, void *, size_t, size_t, void const * ))getGenericFunctionPtr())( *(inputTypeCType const *)inputData, outputData, index, count, sharedData ); \
    } \

#define FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT( inputTypeName, inputTypeCType ) \
    void ArrayIOOperator::call2##inputTypeName( void const *inputData, void *outputData ) \
    { \
      ((void (*)( inputTypeCType, void * ))getGenericFunctionPtr())( *(inputTypeCType const *)inputData, outputData ); \
    } \
    void ArrayIOOperator::call3##inputTypeName( void const *inputData, void *outputData, size_t index ) \
    { \
      ((void (*)( inputTypeCType, void *, size_t ))getGenericFunctionPtr())( *(inputTypeCType const *)inputData, outputData, index ); \
    } \
    void ArrayIOOperator::call4##inputTypeName( void const *inputData, void *outputData, size_t index, size_t count ) \
    { \
      ((void (*)( inputTypeCType, void *, size_t, size_t ))getGenericFunctionPtr())( *(inputTypeCType const *)inputData, outputData, index, count ); \
    } \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_ST( inputTypeName, inputTypeCType, Boolean, bool ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_ST( inputTypeName, inputTypeCType, UInt8, uint8_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_ST( inputTypeName, inputTypeCType, SInt8, int8_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_ST( inputTypeName, inputTypeCType, UInt16, uint16_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_ST( inputTypeName, inputTypeCType, SInt16, int16_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_ST( inputTypeName, inputTypeCType, UInt32, uint32_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_ST( inputTypeName, inputTypeCType, SInt32, int32_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_ST( inputTypeName, inputTypeCType, UInt64, uint64_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_ST( inputTypeName, inputTypeCType, SInt64, int64_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_ST( inputTypeName, inputTypeCType, Float32, float ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_ST( inputTypeName, inputTypeCType, Float64, double ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT_DEFAULT( inputTypeName, inputTypeCType ) \
    
#define FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_ST( sharedTypeName, sharedTypeCType ) \
    void ArrayIOOperator::call5Default##sharedTypeName( void const *inputData, void *outputData, size_t index, size_t count, void const *sharedData ) \
    { \
      ((void (*)( void const *, void *, size_t, size_t, sharedTypeCType ))getGenericFunctionPtr())( inputData, outputData, index, count, *(sharedTypeCType const *)sharedData ); \
    } \

#define FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_DEFAULT() \
    void ArrayIOOperator::call5DefaultDefault( void const *inputData, void *outputData, size_t index, size_t count, void const *sharedData ) \
    { \
      getGenericFunctionPtr()( inputData, outputData, index, count, sharedData ); \
    } \

#define FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT() \
    void ArrayIOOperator::call2Default( void const *inputData, void *outputData ) \
    { \
      getGenericFunctionPtr()( inputData, outputData ); \
    } \
    void ArrayIOOperator::call3Default( void const *inputData, void *outputData, size_t index ) \
    { \
      getGenericFunctionPtr()( inputData, outputData, index ); \
    } \
    void ArrayIOOperator::call4Default( void const *inputData, void *outputData, size_t index, size_t count ) \
    { \
      getGenericFunctionPtr()( inputData, outputData, index, count ); \
    } \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_ST( Boolean, bool ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_ST( UInt8, uint8_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_ST( SInt8, int8_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_ST( UInt16, uint16_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_ST( SInt16, int16_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_ST( UInt32, uint32_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_ST( SInt32, int32_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_ST( UInt64, uint64_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_ST( SInt64, int64_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_ST( Float32, float ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_ST( Float64, double ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT_DEFAULT() \

#define FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL() \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT( Boolean, bool ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT( UInt8, uint8_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT( SInt8, int8_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT( UInt16, uint16_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT( SInt16, int16_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT( UInt32, uint32_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT( SInt32, int32_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT( UInt64, uint64_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT( SInt64, int64_t ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT( Float32, float ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_IT( Float64, double ) \
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL_DEFAULT() \
    
    FABRIC_KLC_ARRAY_IO_OPERATOR_CALL_IMPL()
    
    void ArrayIOOperator::call(
      void const *inputData,
      void *outputData
      ) const
    {
      if ( m_call2 )
        (const_cast<ArrayIOOperator *>(this)->*m_call2)( inputData, outputData );
      else throw Exception( "operator is invalid" );
    }
    
    void ArrayIOOperator::call(
      void const *inputData,
      void *outputData,
      size_t index
      ) const
    {
      if ( m_call3 )
        (const_cast<ArrayIOOperator *>(this)->*m_call3)( inputData, outputData, index );
      else throw Exception( "operator is invalid" );
    }
    
    void ArrayIOOperator::call(
      void const *inputData,
      void *outputData,
      size_t index,
      size_t count
      ) const
    {
      if ( m_call4 )
        (const_cast<ArrayIOOperator *>(this)->*m_call4)( inputData, outputData, index, count );
      else throw Exception( "operator is invalid" );
    }
    
    void ArrayIOOperator::call(
      void const *inputData,
      void *outputData,
      size_t index,
      size_t count,
      void const *sharedData
      ) const
    {
      if ( m_call5 )
        (const_cast<ArrayIOOperator *>(this)->*m_call5)( inputData, outputData, index, count, sharedData );
      else throw Exception( "operator is invalid" );
    }
  }
}
