/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_ARRAY_PRODUCER_WRAPPER_H
#define _FABRIC_MR_ARRAY_PRODUCER_WRAPPER_H

#include <Fabric/Core/MR/ProducerWrapper.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace RT
  {
    class Desc;
  };
  
  namespace MR
  {
    class ArrayProducer;
    
    class ArrayProducerWrapper : public ProducerWrapper
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    public:
      
      virtual RC::ConstHandle<ArrayProducer> getUnwrapped() const = 0;

      virtual void jsonExec(
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
    protected:
    
      ArrayProducerWrapper(
        FABRIC_GC_OBJECT_CLASS_PARAM
        );
    
      void jsonExecGetCount(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );

      void jsonExecProduce(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
        
      void jsonExecProduceAsync(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    
      void jsonExecFlush(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    
    private:
    
      static void JSONExecProduceAsyncFinishedCallback( void *userdata );
    };
  };
};

#endif //_FABRIC_MR_ARRAY_PRODUCER_WRAPPER_H
