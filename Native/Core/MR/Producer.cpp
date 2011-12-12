/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/Producer.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_GET_CLASS_IMPL( Producer, GC::Object );

    Producer::Producer( GC::Object::Class const *myClass, GC::Container *container )
      : GC::Object( myClass, container )
    {
    }

    void Producer::toJSON( Util::JSONGenerator &jg ) const
    {
      Util::JSONObjectGenerator jog = jg.makeObject();
      
      {
        Util::JSONGenerator typeNameJG = jog.makeMember( "kind" );
        typeNameJG.makeString( getKind() );
      }
     
      toJSONImpl( jog );
    }
  };
};
