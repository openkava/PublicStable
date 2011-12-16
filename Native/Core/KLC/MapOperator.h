/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_MAP_OPERATOR_H
#define _FABRIC_KL_MAP_OPERATOR_H

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
    class MapOperator : public Operator
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    public:
    
      static RC::Handle<MapOperator> Create(
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Operator> const &astOperator,
        GenericFunctionPtr functionPtr
        );
      
      virtual char const *getKind() const;
        
      RC::ConstHandle<RT::Desc> getInputDesc() const;
      RC::ConstHandle<RT::Desc> getOutputDesc() const;
      
      void call(
        size_t index,
        void const *inputData,
        void *outputData
        ) const;
      
    protected:
    
      MapOperator(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Operator> const &astOperator,
        GenericFunctionPtr functionPtr
        );
    
    private:
    
      void callBoolean( size_t index, void const *inputData, void *outputData );
      void callUInt8( size_t index, void const *inputData, void *outputData );
      void callSInt8( size_t index, void const *inputData, void *outputData );
      void callUInt16( size_t index, void const *inputData, void *outputData );
      void callSInt16( size_t index, void const *inputData, void *outputData );
      void callUInt32( size_t index, void const *inputData, void *outputData );
      void callSInt32( size_t index, void const *inputData, void *outputData );
      void callUInt64( size_t index, void const *inputData, void *outputData );
      void callSInt64( size_t index, void const *inputData, void *outputData );
      void callFloat32( size_t index, void const *inputData, void *outputData );
      void callFloat64( size_t index, void const *inputData, void *outputData );
      void callString( size_t index, void const *inputData, void *outputData );
      void callRef( size_t index, void const *inputData, void *outputData );
    
      RC::ConstHandle<CG::Adapter> m_inputAdapter;
      RC::ConstHandle<CG::Adapter> m_outputAdapter;
      
      void (MapOperator::*m_call)( size_t index, void const *inputData, void *outputData );
    };
  }
}

#endif //_FABRIC_KL_MAP_OPERATOR_H
