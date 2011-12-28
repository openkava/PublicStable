/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueTransformOperator.h>
#include <Fabric/Core/RT/Impl.h>
#include <Fabric/Core/RT/IntegerDesc.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace MR
  {
    RC::Handle<ValueTransformOperator> ValueTransformOperator::Create(
      void (*functionPtr)(...),
      size_t numParams,
      RC::ConstHandle<RT::Desc> const &valueDesc,
      RC::ConstHandle<RT::Desc> const &sharedDesc
      )
    {
      RC::Handle<ValueTransformOperator> result( new ValueTransformOperator );
      result->init( functionPtr, numParams, valueDesc, sharedDesc );
      return result;
    }
    
#define FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK() \
          do \
          { \
            m_call1 = &ValueTransformOperator::call1; \
          } while ( false )

#define FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( sharedTypeName ) \
          do \
          { \
            if ( numParams == 2 ) \
              m_call2 = &ValueTransformOperator::call2##sharedTypeName; \
            else FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK(); \
          } while ( false )

#define FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_SD( sharedDesc ) \
        do \
        { \
          if ( sharedDesc ) \
          { \
            RT::ImplType sharedType = sharedDesc->getType(); \
            if ( RT::isBoolean(sharedType) ) \
              FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( Boolean ); \
            else if ( RT::isInteger(sharedType) ) \
            { \
              size_t size = sharedDesc->getAllocSize(); \
              bool isSigned = RC::ConstHandle<RT::IntegerDesc>::StaticCast( sharedDesc )->isSigned(); \
              switch ( size ) \
              { \
                case 1: \
                  if ( isSigned ) \
                    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( SInt8 ); \
                  else \
                    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( UInt8 ); \
                  break; \
                case 2: \
                  if ( isSigned ) \
                    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( SInt16 ); \
                  else \
                    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( UInt16 ); \
                  break; \
                case 4: \
                  if ( isSigned ) \
                    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( SInt32 ); \
                  else \
                    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( UInt32 ); \
                  break; \
                case 8: \
                  if ( isSigned ) \
                    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( SInt64 ); \
                  else \
                    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( UInt64 ); \
                  break; \
                default: \
                  FABRIC_ASSERT( false && "Unsupported integer size" ); \
              } \
            } \
            else if ( RT::isFloat(sharedType) ) \
            { \
              if ( sharedDesc->getAllocSize() == 8 ) \
                FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( Float64 ); \
              else \
                FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( Float32 ); \
            } \
            else FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_STN( Default ); \
          } \
          else FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK(); \
        } while ( false )

    ValueTransformOperator::ValueTransformOperator()
      : m_functionPtr( 0 )
      , m_call1( 0 )
      , m_call2( 0 )
    {
    }
    
    void ValueTransformOperator::init(
      void (*functionPtr)(...),
      size_t numParams,
      RC::ConstHandle<RT::Desc> const &valueDesc,
      RC::ConstHandle<RT::Desc> const &sharedDesc
      )
    {
      m_functionPtr = functionPtr;
      m_valueDesc = valueDesc;
      m_sharedDesc = sharedDesc;

      if ( m_functionPtr )
        FABRIC_MR_VALUE_TRANSFORM_OPERATOR_PICK_SD( sharedDesc );
    }

    Exception ValueTransformOperator::GetPrototypeException()
    {
      static const Exception exception("operator must have prototype: io ValueType valueData[, in SharedType sharedData]");
      return exception;
    }
        
    RC::ConstHandle<RT::Desc> ValueTransformOperator::getValueDesc() const
    {
      return m_valueDesc;
    }
    RC::ConstHandle<RT::Desc> ValueTransformOperator::getSharedDesc() const
    {
      return m_sharedDesc;
    }
    
#define FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_ST( sharedTypeName, sharedTypeCType ) \
    void ValueTransformOperator::call2##sharedTypeName( void *valueData, void const *sharedData ) \
    { \
      ((void (*)( void *, sharedTypeCType ))m_functionPtr)( valueData, *(sharedTypeCType const *)sharedData ); \
    } \
    
#define FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_DEFAULT() \
    void ValueTransformOperator::call2Default( void *valueData, void const *sharedData ) \
    { \
      ((void (*)( void *, void const * ))m_functionPtr)( valueData, sharedData ); \
    } \

#define FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL() \
    void ValueTransformOperator::call1( void *valueData ) \
    { \
      ((void (*)( void * ))m_functionPtr)( valueData ); \
    } \
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_ST( Boolean, bool ) \
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_ST( UInt8, uint8_t ) \
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_ST( SInt8, int8_t ) \
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_ST( UInt16, uint16_t ) \
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_ST( SInt16, int16_t ) \
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_ST( UInt32, uint32_t ) \
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_ST( SInt32, int32_t ) \
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_ST( UInt64, uint64_t ) \
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_ST( SInt64, int64_t ) \
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_ST( Float32, float ) \
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_ST( Float64, double ) \
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL_DEFAULT() \
    
    FABRIC_MR_VALUE_TRANSFORM_OPERATOR_CALL_IMPL()
    
    void ValueTransformOperator::call(
      void *valueData
      ) const
    {
      FABRIC_ASSERT( m_call1 );
      (const_cast<ValueTransformOperator *>(this)->*m_call1)( valueData );
    }
    
    void ValueTransformOperator::call(
      void *valueData,
      void const *sharedData
      ) const
    {
      FABRIC_ASSERT( m_call2 );
      (const_cast<ValueTransformOperator *>(this)->*m_call2)( valueData, sharedData );
    }
  }
}
