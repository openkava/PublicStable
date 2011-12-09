/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_PRODUCER_H
#define _FABRIC_MR_PRODUCER_H

#include <Fabric/Base/RC/Object.h>

namespace Fabric
{
  namespace Util
  {
    class JSONGenerator;
    class JSONObjectGenerator;
  };
  
  namespace MR
  {
    class Producer : public RC::Object
    {
      // Virtual functions: Producer
      
    public:
    
      virtual char const *getKind() const = 0;
      virtual bool isValueProducer() const;
      virtual bool isArrayProducer() const;

    protected:
    
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const = 0;
      
      // Non-virtual functions
    
    public:
    
      void toJSON( Util::JSONGenerator &jg ) const;
    };
  };
};

#endif //_FABRIC_MR_PRODUCER_H
