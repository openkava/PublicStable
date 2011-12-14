/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/GC/Container.h>
#include <Fabric/Core/Util/Assert.h>

namespace Fabric
{
  namespace GC
  {
    Object::Object()
      : m_container( 0 )
    {
    }

    Object::~Object()
    {
      if ( m_container )
        m_container->disposeObject( m_id, this );
    }
    
    void Object::reg( Container *container, std::string const &id_ )
    {
      FABRIC_ASSERT( !m_container );
      
      m_container = container;
      m_id = id_;
      
      m_container->registerObject( m_id, this );
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
        jsonExec( cmd, arg, resultJAG );
      else throw Exception( "unroutable" );
    }
      
    void Object::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "dispose" )
      {
        FABRIC_ASSERT( m_container );
        m_container->disposeObject( m_id, this );
        m_container = 0;
      }
      else throw Exception( _(cmd) + ": unknown command" );
    }
  }
}
