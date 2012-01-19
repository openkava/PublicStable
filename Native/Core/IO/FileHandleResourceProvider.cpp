/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "FileHandleResourceProvider.h"
#include "FileHandleManager.h"
#include "FileResourceProvider.h"
#include "Helpers.h"

namespace Fabric
{
  namespace IO
  {
    RC::Handle<FileHandleResourceProvider> FileHandleResourceProvider::Create( RC::Handle<FileHandleManager> fileHandleManager )
    {
      return new FileHandleResourceProvider( fileHandleManager);
    }

    FileHandleResourceProvider::FileHandleResourceProvider( RC::Handle<FileHandleManager> fileHandleManager )
      : m_fileHandleManager( fileHandleManager )
      , m_fileResourceProvider( FileResourceProvider::Create() )
    {
    }

    char const* FileHandleResourceProvider::getUrlScheme() const
    {
      return m_fileHandleManager->getUrlScheme();
    }

    void FileHandleResourceProvider::get( char const *url, bool getAsFile, void* userData )
    {
      if( !m_fileHandleManager->isFile( url ) || !m_fileHandleManager->targetExists( url ) )
      {
          throw Exception( "Error: not a valid FileHandle or not an existing file" );
      }
      std::string fileWithPath = "file:///" + ChangeSeparatorsFileToURL( m_fileHandleManager->getPath( url ) );
      m_fileResourceProvider->get( fileWithPath.c_str(), getAsFile, userData );
    }
  };
};
