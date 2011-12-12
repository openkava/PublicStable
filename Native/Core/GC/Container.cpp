/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/GC/Container.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace GC
  {
    Container::Container()
      : m_mutex( "GC::Container" )
    {
    }
      
    Container::~Container()
    {
      // [pzion 20111212] We could report leaks here, but it will be common
      // that scripts forget to call dispose() on their objects...
    }
      
    size_t Container::registerObject( RC::Handle<Object> const &object )
    {
      Util::Mutex::Lock mutexLock( m_mutex );
      size_t id_ = m_nextID++;
      m_idToObjectMap[id_] = object;
      return id_;
    }
    
    RC::Handle<Object> Container::maybeGetObject( size_t id_ ) const
    {
      Util::Mutex::Lock mutexLock( m_mutex );
      RC::Handle<Object> result;
      IDToObjectMap::const_iterator it = m_idToObjectMap.find( id_ );
      if ( it != m_idToObjectMap.end() )
        result = it->second;
      return result;
    }
    
    RC::Handle<Object> Container::getObject( size_t id_ ) const
    {
      RC::Handle<Object> result = maybeGetObject( id_ );
      if ( !result )
        throw Exception( "GC::Object " + _(id_) + " does not exist or has already been disposed" );
      return result;
    }
    
    void Container::disposeObject( size_t id_ )
    {
      Util::Mutex::Lock mutexLock( m_mutex );
      IDToObjectMap::iterator it = m_idToObjectMap.find( id_ );
      if ( it == m_idToObjectMap.end() )
        throw Exception( "GC::Object " + _(id_) + " does not exist or has already been disposed" );
      m_idToObjectMap.erase( it );
    }
  }
}
