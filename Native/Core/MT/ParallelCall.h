/*
 *
 *  Created by Peter Zion on 10-10-08.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */
 
#ifndef _FABRIC_MT_PARALLEL_CALL_H
#define _FABRIC_MT_PARALLEL_CALL_H

#include <Fabric/Core/MT/Debug.h>
#include <Fabric/Core/MT/Impl.h>
#include <Fabric/Core/MT/Function.h>
#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Core/MT/Util.h>
#include <Fabric/Core/Util/Timer.h>

#include <vector>
#include <stdint.h>

#if defined( FABRIC_WIN32 )
#include <malloc.h>
#else
#include <alloca.h>
#endif

#define FABRIC_MT_PARALLEL_CALL_MAX_PARAM_COUNT 32

namespace Fabric
{
  namespace MT
  {
    class ParallelCall : public RC::Object
    {
    public:
    
      typedef void (*FunctionPtr)( ... );

      static RC::Handle<ParallelCall> Create( RC::ConstHandle<Function> const &function, size_t paramCount, std::string const &debugDesc )
      {
        return new ParallelCall( function, paramCount, debugDesc );
      }
    
    protected:

      ParallelCall( RC::ConstHandle<Function> const &function, size_t paramCount, std::string const &debugDesc );
      ~ParallelCall();
      
    public:
      
      void setBaseAddress( size_t index, void *baseAddress )
      {
        FABRIC_ASSERT( index < m_paramCount );
        m_baseAddresses[index] = baseAddress;
      }
      
      size_t addAdjustment( size_t count, size_t batchSize )
      {
        size_t index = m_adjustments.size();
        m_adjustments.resize( index + 1 );
        Adjustment &newAdjustment = m_adjustments[index];
        
        newAdjustment.count = count;
        newAdjustment.batchSize = batchSize;
        newAdjustment.batchCount = (count + batchSize - 1) / batchSize;
        for ( size_t i=0; i<m_paramCount; ++i )
          newAdjustment.offsets[i] = 0;

        m_totalParallelCalls *= newAdjustment.batchCount;
        
        return index;
      }
      
      void setAdjustmentOffset( size_t adjustmentIndex, size_t paramIndex, size_t adjustmentOffset )
      {
        FABRIC_ASSERT( adjustmentIndex < m_adjustments.size() );
        FABRIC_ASSERT( paramIndex < m_paramCount );
        m_adjustments[adjustmentIndex].offsets[paramIndex] = adjustmentOffset;
      }
      
      void executeSerial() const
      {
        execute( 0, m_baseAddresses, NULL );
      }
      
      void executeParallel( bool mainThreadOnly ) const
      {
        MT::executeParallel( m_totalParallelCalls, &ParallelCall::ExecuteParallel, (void *)this, mainThreadOnly );
      }
      
      void onFunctionPtrChange( FunctionPtr functionPtr, RC::Object const *objectOwningFunctionPtr ) const
      {
        //FABRIC_DEBUG_LOG( "ParallelCall::onFunctionPtrChange( %p, %p )", functionPtr, objectOwningFunctionPtr );
        m_functionPtr = functionPtr;
      }
      
      void addOwnedObject( RC::ConstHandle<RC::Object> const &object )
      {
        m_ownedObjects.push_back( object );
      }
      
    protected:
    
      void execute( size_t adjustmentIndex, void * const *addresses, size_t const *iteration ) const
      {
        if ( adjustmentIndex == m_adjustments.size() )
        {
          evalWithArgs( addresses );
        }
        else
        {
          Adjustment const &adjustment = m_adjustments[adjustmentIndex];

          void **newAddresses = (void **)alloca( m_paramCount * sizeof( void * ) );
          size_t newIterations;
          size_t count;
          size_t const *newIterationsPtr;
          if ( iteration )
          {
            size_t batch = *iteration % adjustment.batchCount;
            size_t start = batch * adjustment.batchSize;
            count = adjustment.batchSize;
            if ( start + count > adjustment.count )
              count = adjustment.count - start;
            
            for ( size_t i=0; i<m_paramCount; ++i )
              newAddresses[i] = (uint8_t *)addresses[i] + start * adjustment.offsets[i];
              
            newIterations = *iteration / adjustment.batchCount;
            newIterationsPtr = &newIterations;
          }
          else
          {
            count = adjustment.count;
            
            for ( size_t i=0; i<m_paramCount; ++i )
              newAddresses[i] = addresses[i];
              
            newIterationsPtr = NULL;
          }

          for ( size_t i=0; i<count; ++i )
          {
            execute( adjustmentIndex + 1, newAddresses, newIterationsPtr );
            for ( size_t j=0; j<m_paramCount; ++j )
              newAddresses[j] = (uint8_t *)newAddresses[j] + adjustment.offsets[j];
          }
        }
      }
      
      void executeParallel( size_t iteration ) const
      {
        //Util::Timer timer;
        execute( 0, m_baseAddresses, &iteration );
        //FABRIC_DEBUG_LOG( "[%p] debugDesc='%s' iteration=%u elapsed=%gms", (void *)getCurrentThreadID(), m_debugDesc.c_str(), (unsigned)iteration, timer.getElapsed() );
      }
      
      static void ExecuteParallel( void *context, size_t iteration )
      {
        static_cast<ParallelCall const *>(context)->executeParallel( iteration );
      }
      
      void evalWithArgs( void * const *argv ) const
      {
        // [pzion 20110322] Note that it's possible that the function pointer
        // we call actually belongs to a newer object than the one we retain/release;
        // this is OK as long as it is impossible to change the function pointer
        // twice in the execution of the pointer, which is the case because we only
        // ever update pointers once.
        RC::Object const *objectOwningFunctionPtr = m_objectOwningFunctionPtr;
        objectOwningFunctionPtr->retain();
        size_t argc = m_paramCount;
        switch ( argc )
        {
          case 0:
            m_functionPtr();
            break;
          case 1:
            m_functionPtr(
              argv[ 0]
              );
            break;
          case 2:
            m_functionPtr(
              argv[ 0], argv[ 1]
              );
            break;
          case 3:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2]
              );
            break;
          case 4:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3]
              );
            break;
          case 5:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4]
              );
            break;
          case 6:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5]
              );
            break;
          case 7:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6]
              );
            break;
          case 8:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7]
              );
            break;
          case 9:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8]
              );
            break;
          case 10:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9]
              );
            break;
          case 11:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10]
              );
            break;
          case 12:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11]
              );
            break;
          case 13:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12]
              );
            break;
          case 14:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13]
              );
            break;
          case 15:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14]
              );
            break;
          case 16:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15]
              );
            break;
          case 17:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16]
              );
            break;
          case 18:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17]
              );
            break;
          case 19:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18]
              );
            break;
          case 20:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19]
              );
            break;
          case 21:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19],
              argv[20]
              );
            break;
          case 22:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19],
              argv[20], argv[21]
              );
            break;
          case 23:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19],
              argv[20], argv[21], argv[22]
              );
            break;
          case 24:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19],
              argv[20], argv[21], argv[22], argv[23]
              );
            break;
          case 25:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19],
              argv[20], argv[21], argv[22], argv[23],
              argv[24]
              );
            break;
          case 26:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19],
              argv[20], argv[21], argv[22], argv[23],
              argv[24], argv[25]
              );
            break;
          case 27:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19],
              argv[20], argv[21], argv[22], argv[23],
              argv[24], argv[25], argv[26]
              );
            break;
          case 28:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19],
              argv[20], argv[21], argv[22], argv[23],
              argv[24], argv[25], argv[26], argv[27]
              );
            break;
          case 29:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19],
              argv[20], argv[21], argv[22], argv[23],
              argv[24], argv[25], argv[26], argv[27],
              argv[28]
              );
            break;
          case 30:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19],
              argv[20], argv[21], argv[22], argv[23],
              argv[24], argv[25], argv[26], argv[27],
              argv[28], argv[29]
              );
            break;
          case 31:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19],
              argv[20], argv[21], argv[22], argv[23],
              argv[24], argv[25], argv[26], argv[27],
              argv[28], argv[29], argv[30]
              );
            break;
          case 32:
            m_functionPtr(
              argv[ 0], argv[ 1], argv[ 2], argv[ 3],
              argv[ 4], argv[ 5], argv[ 6], argv[ 7],
              argv[ 8], argv[ 9], argv[10], argv[11],
              argv[12], argv[13], argv[14], argv[15],
              argv[16], argv[17], argv[18], argv[19],
              argv[20], argv[21], argv[22], argv[23],
              argv[24], argv[25], argv[26], argv[27],
              argv[28], argv[29], argv[30], argv[31]
              );
            break;
          default:
            throw Exception( "a maximum of %u parameters are supported for operator calls", FABRIC_MT_PARALLEL_CALL_MAX_PARAM_COUNT );
        }
        objectOwningFunctionPtr->release();
      }
      
    private:
    
      struct Adjustment
      {
        size_t count;
        size_t batchSize;
        size_t batchCount;
        size_t offsets[FABRIC_MT_PARALLEL_CALL_MAX_PARAM_COUNT];
      };
      
      RC::ConstHandle<Function> m_function;
      mutable FunctionPtr volatile m_functionPtr;
      RC::Object const * volatile m_objectOwningFunctionPtr;
      size_t m_paramCount;
      void *m_baseAddresses[FABRIC_MT_PARALLEL_CALL_MAX_PARAM_COUNT];
      std::vector< Adjustment > m_adjustments;
      size_t m_totalParallelCalls;
      std::string m_debugDesc;
      std::vector< RC::ConstHandle<RC::Object> > m_ownedObjects;
    };
  };
};

#endif //_FABRIC_MT_PARALLEL_CALL_H
