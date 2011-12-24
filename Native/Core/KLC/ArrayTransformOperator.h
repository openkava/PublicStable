/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_ARRAY_TRANSFORM_OPERATOR_H
#define _FABRIC_KL_ARRAY_TRANSFORM_OPERATOR_H

#include <Fabric/Core/KLC/Operator.h>

namespace Fabric
{
  namespace RT
  {
    class Desc;
  };
  
  namespace CG
  {
    class Adapter;
  };
  
  namespace KLC
  {
#define FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( sharedTypeName ) \
      void call4##sharedTypeName( void *valueData, size_t index, size_t count, void const *sharedData ); \
      
#define FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL() \
      void call1( void *valueData ); \
      void call2( void *valueData, size_t index ); \
      void call3( void *valueData, size_t index, size_t count ); \
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( Boolean ); \
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( UInt8 ); \
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( SInt8 ); \
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( UInt16 ); \
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( SInt16 ); \
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( UInt32 ); \
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( SInt32 ); \
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( UInt64 ); \
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( SInt64 ); \
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( Float32 ); \
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( Float64 ); \
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL_STN( Default ); \

    class ArrayTransformOperator : public Operator
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    public:
    
      static RC::Handle<ArrayTransformOperator> Create(
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Operator> const &astOperator,
        GenericFunctionPtr functionPtr
        );
      
      RC::ConstHandle<RT::Desc> getValueDesc() const;
      RC::ConstHandle<RT::Desc> getSharedDesc() const;
      
      bool takesIndex() const
      {
        return m_call2 != 0 || m_call3 != 0 || m_call4 != 0;
      }
      bool takesCount() const
      {
        return m_call3 != 0 || m_call4 != 0;
      }
      bool takesSharedValue() const
      {
        return m_call4 != 0;
      }
      
      void call(
        void *valueData
        ) const;
      void call(
        void *valueData,
        size_t index
        ) const;
      void call(
        void *valueData,
        size_t index,
        size_t count
        ) const;
      void call(
        void *valueData,
        size_t index,
        size_t count,
        void const *sharedData
        ) const;
      
    protected:
    
      ArrayTransformOperator(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Operator> const &astOperator,
        GenericFunctionPtr functionPtr
        );
    
    private:
    
      FABRIC_KLC_ARRAY_TRANSFORM_OPERATOR_CALL_DECL()
    
      RC::ConstHandle<CG::Adapter> m_valueAdapter;
      RC::ConstHandle<CG::Adapter> m_sharedAdapter;
      
      void (ArrayTransformOperator::*m_call1)( void *valueData );
      void (ArrayTransformOperator::*m_call2)( void *valueData, size_t index );
      void (ArrayTransformOperator::*m_call3)( void *valueData, size_t index, size_t count );
      void (ArrayTransformOperator::*m_call4)( void *valueData, size_t index, size_t count, void const *sharedData );
    };
  }
}

#endif //_FABRIC_KL_ARRAY_TRANSFORM_OPERATOR_H
