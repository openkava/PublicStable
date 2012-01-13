/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_VALUE_PRODUCER_H
#define _FABRIC_MR_VALUE_PRODUCER_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace Util
  {
    class JSONGenerator;
    class JSONObjectGenerator;
  };
  
  namespace RT
  {
    class Desc;
  };
  
  namespace MR
  {
    class ValueProducer : public RC::Object
    {
    public:
    
      class ComputeState : public RC::Object
      {
      public:
      
        virtual void produce( void *data ) const = 0;
        virtual void produceJSON( Util::JSONGenerator &jg ) const;
        void produceJSONAsync(
          Util::JSONObjectGenerator &jsonObjectGenerator,
          void (*finishedCallback)( void * ),
          void *finishedUserdata
          );
        
      protected:
      
        ComputeState( RC::ConstHandle<ValueProducer> const &valueProducer );
    
        static void ProduceJSONAsyncCallback(
          void *userdata,
          size_t index
          );
        
      protected:
      
        RC::ConstHandle<ValueProducer> m_valueProducer;
      };

      virtual RC::ConstHandle<RT::Desc> getValueDesc() const = 0;
      virtual const RC::Handle<ComputeState> createComputeState() const = 0;
    
    protected:
    
      ValueProducer();
    };
  }
}

#endif //_FABRIC_MR_VALUE_PRODUCER_H
