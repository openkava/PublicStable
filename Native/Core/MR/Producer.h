/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_PRODUCER_H
#define _FABRIC_MR_PRODUCER_H

#include <Fabric/Core/GC/Object.h>

namespace Fabric
{
  namespace Util
  {
    class JSONGenerator;
    class JSONObjectGenerator;
  };
  
  namespace MR
  {
    class Producer : public GC::Object
    {
      FABRIC_GC_OBJECT_GET_CLASS_DECL()
      
      // Virtual functions: Producer
      
    protected:
    
      Producer( GC::Object::Class const *myClass, GC::Container *container ); 
    
      virtual char const *getKind() const = 0;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const = 0;
      
      // Non-virtual functions
    
    public:
    
      void toJSON( Util::JSONGenerator &jg ) const;
    };
  };
};

#endif //_FABRIC_MR_PRODUCER_H
