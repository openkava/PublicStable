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
      
    void Container::registerObject( std::string const &id_, Object *object )
    {
      Util::Mutex::Lock mutexLock( m_mutex );
      if ( m_idToObjectMap.find( id_ ) != m_idToObjectMap.end() )
        throw Exception( "GC::Object " + _(id_) + " already in use" );
      m_idToObjectMap[id_] = object;
    }
    
    RC::Handle<Object> Container::maybeGetObject( std::string const &id_ ) const
    {
      Util::Mutex::Lock mutexLock( m_mutex );
      RC::Handle<Object> result;
      IDToObjectMap::const_iterator it = m_idToObjectMap.find( id_ );
      if ( it != m_idToObjectMap.end() )
        result = it->second;
      return result;
    }
    
    RC::Handle<Object> Container::getObject( std::string const &id_ ) const
    {
      RC::Handle<Object> result = maybeGetObject( id_ );
      if ( !result )
        throw Exception( "GC::Object " + _(id_) + " does not exist or has already been disposed" );
      return result;
    }
    
    void Container::disposeObject( std::string const &id_, Object *object )
    {
      Util::Mutex::Lock mutexLock( m_mutex );
      IDToObjectMap::iterator it = m_idToObjectMap.find( id_ );
      if ( it == m_idToObjectMap.end() )
        throw Exception( "GC::Object " + _(id_) + " does not exist or has already been disposed" );
      FABRIC_ASSERT( it->second.ptr() == object );
      m_idToObjectMap.erase( it );
    }

    void Container::jsonRoute(
      std::vector<std::string> const &dst,
      size_t dstOffset,
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      FABRIC_ASSERT( dst.size() - dstOffset > 0 );
      std::string const &id_ = dst[dstOffset];
      getObject( id_ )->jsonRoute( dst, dstOffset + 1, cmd, arg, resultJAG );
    }
  }
}
