/*
 *  TaskGroupStream.cpp
 *  Fabric
 *
 *  Created by Peter Zion on 10-09-28.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#include "TaskGroupStream.h"
#include "Debug.h"

namespace Fabric
{
  namespace MT
  {
    TaskGroupStream::TaskGroupStream()
    {
      FABRIC_MT_TRACE( "TaskGroupStream::TaskGroupStream()" );
    }
    
    void TaskGroupStream::clear()
    {
      FABRIC_MT_TRACE( "TaskGroupStream::clear()" );
      for ( std::vector< TaskGroup >::iterator it=m_taskGroups.begin(); it!=m_taskGroups.end(); ++it )
        it->clear();
    }
    
    void TaskGroupStream::add( unsigned rank, TaskBase const *task )
    {
      FABRIC_MT_TRACE( "TaskGroupStream::add()" );
      if ( rank >= m_taskGroups.size() )
        m_taskGroups.resize( rank+1 );
      m_taskGroups[rank].add( task );
    }
    
    void TaskGroupStream::execute( void *userdata ) const
    {
      FABRIC_MT_TRACE( "TaskGroupStream::execute( %p )", userdata );
      for ( std::vector< TaskGroup >::const_iterator it=m_taskGroups.begin(); it!=m_taskGroups.end(); ++it )
        it->execute( userdata );
    }

    std::string TaskGroupStream::desc() const
    {
      FABRIC_MT_TRACE( "TaskGroupStream::desc()" );
      std::string result = "[";
      for ( std::vector< TaskGroup >::const_iterator it=m_taskGroups.begin(); it!=m_taskGroups.end(); ++it )
      {
        if ( result.length() > 1 )
          result += ",";
        result += it->desc();
      }
      return result + "]";
    }
  };
};
