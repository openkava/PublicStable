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
    Class Object::s_class = { 0 };

    Object::Object( FABRIC_GC_OBJECT_CLASS_PARAM )
      : m_class( FABRIC_GC_OBJECT_CLASS_ARG )
      , m_container( 0 )
    {
    }

    Object::~Object()
    {
      dispose();
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
        dispose();
      else throw Exception( _(cmd) + ": unknown command" );
    }
    
    void Object::dispose()
    {
      if ( m_container )
      {
        // [pzion 20111215] Note that this may end up calling the
        // destructor so we make sure we won't get here again
        Container *container = m_container;
        m_container = 0;
        container->disposeObject( m_id, this );
      }
    }
  }
}
