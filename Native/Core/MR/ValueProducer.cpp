/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ValueProducer.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/MT/Impl.h>
#include <Fabric/Core/Util/JSONGenerator.h>

#include <alloca.h>
#include <string.h>

namespace Fabric
{
  namespace MR
  {
    ValueProducer::ValueProducer()
    {
    }
    
    ValueProducer::ComputeState::ComputeState( RC::ConstHandle<ValueProducer> const &valueProducer )
      : m_valueProducer( valueProducer )
    {
    }
    
    void ValueProducer::ComputeState::produceJSON( Util::JSONGenerator &jg ) const
    {
      RC::ConstHandle<RT::Desc> valueDesc = m_valueProducer->getValueDesc();
      
      size_t allocSize = valueDesc->getAllocSize();
      void *valueData = alloca( allocSize );
      memset( valueData, 0, allocSize );
      produce( valueData );
      valueDesc->generateJSON( valueData, jg );
      valueDesc->disposeData( valueData );
    }
    
    struct ProduceJSONAsyncCallbackData
    {
      RC::Handle<ValueProducer::ComputeState> computeState;
      Util::JSONObjectGenerator *jsonObjectGenerator;
    };

    void ValueProducer::ComputeState::produceJSONAsync(
      Util::JSONObjectGenerator &jsonObjectGenerator,
      void (*finishedCallback)( void * ),
      void *finishedUserdata
      )
    {
      ProduceJSONAsyncCallbackData *produceJSONAsyncCallbackData = new ProduceJSONAsyncCallbackData;
      produceJSONAsyncCallbackData->computeState = this;
      produceJSONAsyncCallbackData->jsonObjectGenerator = &jsonObjectGenerator;
      
      MT::ThreadPool::Instance()->executeParallelAsync(
        MT::tlsLogCollector.get(),
        1,
        &ValueProducer::ComputeState::ProduceJSONAsyncCallback,
        produceJSONAsyncCallbackData,
        MT::ThreadPool::Idle,
        finishedCallback,
        finishedUserdata
        );
    }
    
    void ValueProducer::ComputeState::ProduceJSONAsyncCallback(
      void *userdata,
      size_t index
      )
    {
      ProduceJSONAsyncCallbackData *produceJSONAsyncCallbackData = static_cast<ProduceJSONAsyncCallbackData *>( userdata );
      {
        Util::JSONGenerator jg = produceJSONAsyncCallbackData->jsonObjectGenerator->makeMember( "result" );
        produceJSONAsyncCallbackData->computeState->produceJSON( jg );
      }
      delete produceJSONAsyncCallbackData;
    }
  };
};
