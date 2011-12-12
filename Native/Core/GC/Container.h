/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_GC_CONTAINER_H
#define _FABRIC_GC_CONTAINER_H

#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/Util/Mutex.h>
#include <Fabric/Base/RC/Handle.h>

#include <map>

namespace Fabric
{
  namespace GC
  {
    class Container
    {
      typedef std::map< size_t, RC::Handle<GC::Object> > IDToObjectMap;
      
    public:
    
      Container();
      ~Container();
      
      size_t registerObject( RC::Handle<Object> const &object );
      RC::Handle<Object> maybeGetObject( size_t id_ ) const;
      RC::Handle<Object> getObject( size_t id_ ) const;
      void disposeObject( size_t id_ );
      
    private:
    
      mutable Util::Mutex m_mutex;
      size_t m_nextID;
      IDToObjectMap m_idToObjectMap;
    };
  }
}

#endif //_FABRIC_GC_CONTAINER_H
