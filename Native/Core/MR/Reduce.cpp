/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/Reduce.h>
#include <Fabric/Core/MR/ArrayProducer.h>
#include <Fabric/Core/KLC/ReduceOperator.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/MT/Impl.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Core/MT/Util.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( Reduce, ValueProducer );
    
    RC::Handle<Reduce> Reduce::Create(
      RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
      RC::ConstHandle<KLC::ReduceOperator> const &reduceOperator
      )
    {
      return new Reduce( FABRIC_GC_OBJECT_MY_CLASS, inputArrayProducer, reduceOperator );
    }
    
    Reduce::Reduce(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
      RC::ConstHandle<KLC::ReduceOperator> const &reduceOperator
      )
      : ValueProducer( FABRIC_GC_OBJECT_CLASS_ARG, reduceOperator->getOutputDesc() )
      , m_inputArrayProducer( inputArrayProducer )
      , m_reduceOperator( reduceOperator )
      , m_mutex( "Reduce" )
    {
      RC::ConstHandle<RT::Desc> inputArrayProducerElementDesc = inputArrayProducer->getElementDesc();
      if ( !inputArrayProducerElementDesc )
        throw Exception("input array producer is invalid");
      RC::ConstHandle<RT::Desc> reduceOperatorInputDesc = reduceOperator->getInputDesc();
      if ( !reduceOperatorInputDesc )
        throw Exception("reduce operator is invalid");
      if ( !reduceOperatorInputDesc->isEquivalentTo( inputArrayProducerElementDesc ) )
        throw Exception(
          "input element type ("
          + _(inputArrayProducerElementDesc->getUserName())
          + ") is not equivalent to reduce operator input type ("
          + _(reduceOperatorInputDesc->getUserName()) + ")"
          );
    }
    
    Reduce::~Reduce()
    {
    }

    char const *Reduce::getKind() const
    {
      return "Reduce";
    }
    
    void Reduce::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "inputArrayProducer" );
        m_inputArrayProducer->toJSON( jg );
      }
      
      {
        Util::JSONGenerator jg = jog.makeMember( "reduceOperator" );
        m_reduceOperator->toJSON( jg );
      }
    }
    
    class Reduce::Execution
    {
    public:
    
      Execution(
        RC::ConstHandle<Reduce> const &reduce,
        size_t count,
        void *outputData,
        Util::Mutex &mutex
        )
        : m_reduce( reduce )
        , m_count( count )
        , m_outputData( outputData )
        , m_mutex( mutex )
        , m_numJobs( std::min( m_count, 4 * MT::getNumCores() ) )
        , m_indicesPerJob( m_count / m_numJobs )
      {
      }
      
      void run()
      {
        MT::executeParallel(
          MT::tlsLogCollector.get(),
          m_numJobs,
          &Callback,
          this,
          false
          );
      }
      
    protected:
      
      void callback( size_t jobIndex )
      {
        RC::ConstHandle<ArrayProducer> inputArrayProducer = m_reduce->m_inputArrayProducer;
        RC::ConstHandle<KLC::ReduceOperator> reduceOperator = m_reduce->m_reduceOperator;
        
        RC::ConstHandle<RT::Desc> inputElementDesc = inputArrayProducer->getElementDesc();
        size_t inputElementSize = inputElementDesc->getAllocSize();
        void *inputData = alloca( inputElementSize );
        memset( inputData, 0, inputElementSize );

        size_t startIndex = jobIndex * m_indicesPerJob;
        size_t endIndex = std::min( startIndex + m_indicesPerJob, m_count );
        for ( size_t index = startIndex; index < endIndex; ++index )
        {
          inputArrayProducer->produce( index, inputData );
          {
            Util::Mutex::Lock mutexLock( m_mutex );
            reduceOperator->call( index, inputData, m_outputData );
          }
        }
      
        inputElementDesc->disposeData( inputData );
      }
      
      static void Callback( void *userdata, size_t jobIndex )
      {
        static_cast<Execution *>( userdata )->callback( jobIndex );
      }
      
    private:
    
      RC::ConstHandle<Reduce> m_reduce;
      size_t m_count;
      void *m_outputData;
      Util::Mutex &m_mutex;
      size_t m_numJobs;
      size_t m_indicesPerJob;
    };
        
    
    void Reduce::produce( void *data ) const
    {
      Execution(
        this,
        m_inputArrayProducer->count(),
        data,
        m_mutex
        ).run();
    }
  }
}
