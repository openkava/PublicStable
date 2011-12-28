/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_ARRAY_PRODUCER_H
#define _FABRIC_MR_ARRAY_PRODUCER_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace Util
  {
    class JSONGenerator;
  }
  
  namespace RT
  {
    class Desc;
  }
  
  namespace MR
  {
    class ArrayProducer : public RC::Object
    {
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
            
      // Virtual functions: ArrayProducer
    
    public:
      
      virtual RC::ConstHandle<RT::Desc> getElementDesc() const = 0;
      virtual size_t getCount() const = 0;
      virtual const RC::Handle<ComputeState> createComputeState() const = 0;
    
    protected:
    
      ArrayProducer();
    };
  }
}

#endif //_FABRIC_MR_ARRAY_PRODUCER_H
