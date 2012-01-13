/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_VALUE_PRODUCER_WRAPPER_H
#define _FABRIC_MR_VALUE_PRODUCER_WRAPPER_H

#include <Fabric/Core/MR/ProducerWrapper.h>

namespace Fabric
{
  namespace MR
  {
    class ValueProducer;
    
    class ValueProducerWrapper : public ProducerWrapper
    {
      FABRIC_GC_OBJECT_CLASS_DECL()

      // Virtual functions: GC::Object
      
    public:
      
      virtual RC::ConstHandle<ValueProducer> getUnwrapped() const = 0;

      virtual void jsonExec(
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    
    protected:
    
      ValueProducerWrapper(
        FABRIC_GC_OBJECT_CLASS_PARAM
        );
    
      void jsonExecProduce(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );

      void jsonExecProduceAsync(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );

    private:
    
      static void JSONExecProduceAsyncFinishedCallback( void *userdata );
    };
  }
}

#endif //_FABRIC_MR_VALUE_PRODUCER_WRAPPER_H
