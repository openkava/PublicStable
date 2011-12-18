/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_REDUCE_OPERATOR_H
#define _FABRIC_KL_REDUCE_OPERATOR_H

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
#define FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( typeName ) \
      void call2_##typeName( void const *inputData, void *outputData ); \
      void call3_##typeName( void const *inputData, void *outputData, size_t index ); \
      void call4_##typeName( void const *inputData, void *outputData, size_t index, size_t count ); \
  
    class ReduceOperator : public Operator
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    public:
    
      static RC::Handle<ReduceOperator> Create(
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Operator> const &astOperator,
        GenericFunctionPtr functionPtr
        );
      
      virtual char const *getKind() const;
        
      RC::ConstHandle<RT::Desc> getInputDesc() const;
      RC::ConstHandle<RT::Desc> getOutputDesc() const;
      
      bool takesIndex() const
      {
        return m_call3 != 0;
      }
      bool takesCount() const
      {
        return m_call4 != 0;
      }
      
      void call(
        void const *inputData,
        void *outputData
        ) const;
      void call(
        void const *inputData,
        void *outputData,
        size_t index
        ) const;
      void call(
        void const *inputData,
        void *outputData,
        size_t index,
        size_t count
        ) const;
      
    protected:
    
      ReduceOperator(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Operator> const &astOperator,
        GenericFunctionPtr functionPtr
        );
    
    private:
    
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( Boolean )
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( UInt8 )
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( SInt8 )
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( UInt16 )
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( SInt16 )
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( UInt32 )
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( SInt32 )
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( UInt64 )
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( SInt64 )
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( Float32 )
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( Float64 )
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( String )
      FABRIC_KLC_REDUCE_OPERATOR_CALL_DECL( Default )
    
      RC::ConstHandle<CG::Adapter> m_inputAdapter;
      RC::ConstHandle<CG::Adapter> m_outputAdapter;
      
      void (ReduceOperator::*m_call2)( void const *inputData, void *outputData );
      void (ReduceOperator::*m_call3)( void const *inputData, void *outputData, size_t index );
      void (ReduceOperator::*m_call4)( void const *inputData, void *outputData, size_t index, size_t count );
    };
  }
}

#endif //_FABRIC_KL_REDUCE_OPERATOR_H
