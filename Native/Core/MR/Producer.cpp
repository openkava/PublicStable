/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/Producer.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( Producer, Object );

    Producer::Producer( FABRIC_GC_OBJECT_CLASS_PARAM )
      : Object( FABRIC_GC_OBJECT_CLASS_ARG )
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
      
    void Producer::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "getJSONDesc" )
        jsonExecGetJSONDesc( arg, resultJAG );
      else Object::jsonExec( cmd, arg, resultJAG );
    }
    
    void Producer::jsonExecGetJSONDesc(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      Util::JSONGenerator jg = resultJAG.makeElement();
      toJSON( jg );
    }
  };
};
