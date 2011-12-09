/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/Producer.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    bool Producer::isValueProducer() const
    {
      return false;
    }
    
    bool Producer::isArrayProducer() const
    {
      return false;
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
