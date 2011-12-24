/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_VALUE_PRODUCER_H
#define _FABRIC_MR_VALUE_PRODUCER_H

#include <Fabric/Core/MR/Producer.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace RT
  {
    class Desc;
  };
  
  namespace MR
  {
    class ValueProducer : public Producer
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
      // Virtual functions: Object
      
    public:
    
      class ComputeState : public RC::Object
      {
      public:
      
        virtual void produce( void *data ) const = 0;
        virtual void produceJSON( Util::JSONGenerator &jg ) const;
        
      protected:
      
        ComputeState( RC::ConstHandle<ValueProducer> const &valueProducer );
        
      protected:
      
        RC::ConstHandle<ValueProducer> m_valueProducer;
      };

      virtual void jsonExec(
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
    public:
      
      // Virtual functions: ValueProducer
      
      virtual const RC::Handle<ComputeState> createComputeState() const = 0;
    
    public:
    
      RC::ConstHandle<RT::Desc> getValueDesc() const;
    
    protected:
    
      ValueProducer(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<RT::Desc> const &valueDesc
        );
    
    private:
    
      void jsonExecProduce(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    
      RC::ConstHandle<RT::Desc> m_valueDesc;
    };
  };
};

#endif //_FABRIC_MR_VALUE_PRODUCER_H
