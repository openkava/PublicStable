/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_EDK_COMMON_H
#define _FABRIC_EDK_COMMON_H

namespace Fabric
{
  namespace EDK
  {
    struct Callbacks
    {
      Callbacks()
        : m_malloc(NULL)
        , m_realloc(NULL)
        , m_free(NULL)
        , m_throwException(NULL)
        , m_fabricFileHandleCopy(NULL)
        , m_fabricFileHandleDelete(NULL)
        , m_fabricFileHandleSetFromPath(NULL)
        , m_fabricFileHandleGetFullPath(NULL)
        , m_fabricFileHandleHasReadWriteAccess(NULL)
      {}

      void *( *m_malloc )( size_t );
      void *( *m_realloc )( void *, size_t );
      void ( *m_free )( void * );
      void (*m_throwException)( size_t length, char const *data );
      void ( *m_fabricFileHandleCopy )( void **, void const * );
      void ( *m_fabricFileHandleDelete )( void ** );
      bool ( *m_fabricFileHandleSetFromPath )( void **, char const *, size_t, bool );
      char const *( *m_fabricFileHandleGetFullPath )( void const * );
      bool ( *m_fabricFileHandleHasReadWriteAccess )( void const * );
    };
  };
};

typedef void (*FabricEDKInitPtr)( const Fabric::EDK::Callbacks& callbacks );

#endif // _FABRIC_EDK_COMMON_H
