/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/Object.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace MR
  {
    FABRIC_GC_OBJECT_GET_CLASS_IMPL( Object, GC::Object );

    Object::Object( GC::Object::Class const *myClass, GC::Container *container, std::string const &id_ )
      : GC::Object( myClass, container, id_ )
    {
    }

    void Object::jsonRoute(
      std::vector<std::string> const &dst,
      size_t dstOffset,
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( dst.size() - dstOffset == 0 )
        return jsonExec( cmd, arg, resultJAG );
      else throw Exception( "unroutable" );
    }
      
    void Object::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      throw Exception( _(cmd) + ": unknown command" );
    }
  };
};
