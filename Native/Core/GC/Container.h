/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_GC_CONTAINER_H
#define _FABRIC_GC_CONTAINER_H

#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/Util/Mutex.h>
#include <Fabric/Base/RC/Handle.h>

#include <map>
#include <string>

namespace Fabric
{
  namespace GC
  {
    class Container
    {
      typedef std::map< std::string, RC::Handle<GC::Object> > IDToObjectMap;
      
    public:
    
      Container();
      ~Container();
      
      void registerObject( std::string const &id_, RC::Handle<Object> const &object );
      RC::Handle<Object> maybeGetObject( std::string const &id_ ) const;
      RC::Handle<Object> getObject( std::string const &id_ ) const;
      void disposeObject( std::string const &id_ );
      
    private:
    
      mutable Util::Mutex m_mutex;
      std::string m_nextID;
      IDToObjectMap m_idToObjectMap;
    };
  }
}

#endif //_FABRIC_GC_CONTAINER_H
