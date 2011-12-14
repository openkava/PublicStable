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
    Object::Class const *Object::GetClass()
    {
      static Class myClass = { 0 };
      return &myClass;
    }

    Object::Object( Class const *class_, Container *container, std::string const &id_ )
      : m_class( class_ )
      , m_container( container )
      , m_containerObjectID( id_ )
    {
      m_container->registerObject( m_containerObjectID, this );
    }

    Object::~Object()
    {
      if ( m_container )
        dispose();
    }
    
    void Object::dispose()
    {
      FABRIC_ASSERT( m_container );
      m_container->disposeObject( m_containerObjectID );
      m_container = 0;
    }
  }
}
