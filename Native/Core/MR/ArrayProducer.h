/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_ARRAY_PRODUCER_H
#define _FABRIC_MR_ARRAY_PRODUCER_H

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
    class ArrayProducer : public Producer
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
      // Virtual functions: Object
      
    public:
    
      class ComputeState : public RC::Object
      {
      public:
      
        size_t getCount() const
        {
          return m_count;
        }
        virtual void produce( size_t index, void *data ) const = 0;
        virtual void produceJSON( size_t index, Util::JSONGenerator &jg ) const;
      
      protected:
      
        ComputeState( RC::ConstHandle<ArrayProducer> const &arrayProducer );
        
      private:
      
        RC::ConstHandle<ArrayProducer> m_arrayProducer;
        size_t m_count;
      };

      virtual void jsonExec(
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
            
      // Virtual functions: ArrayProducer
    
    public:
      
      virtual size_t getCount() const = 0;
      virtual const RC::Handle<ComputeState> createComputeState() const = 0;
            
      // Non-virtual functions
      
    public:
    
      RC::ConstHandle<RT::Desc> getElementDesc() const;
    
    protected:
    
      ArrayProducer(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<RT::Desc> const &elementDesc
        );
    
    private:
    
      void jsonExecProduce(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      void jsonExecGetCount(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    
      RC::ConstHandle<RT::Desc> m_elementDesc;
    };
  };
};

#endif //_FABRIC_MR_ARRAY_PRODUCER_H
