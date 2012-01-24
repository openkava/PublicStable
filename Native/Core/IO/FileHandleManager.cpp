/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "FileHandleManager.h"
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Util/Assert.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Core/IO/Dir.h>
#include <Fabric/Core/Util/Base64.h>
#include <Fabric/Core/Util/Random.h>
#include <string.h>
#include <fstream>

namespace Fabric
{
  namespace IO
  {
    char const* fileHandleScheme = "fabricio";
    char const* fileHandleSchemeAndSeparator = "fabricio://";

    RC::Handle<FileHandleManager> FileHandleManager::Create()
    {
      return new FileHandleManager();
    }

    FileHandleManager::FileHandleManager()
      : m_encodedHandleLength(0)
    {
    }

    char const *FileHandleManager::getUrlScheme() const
    {
      return fileHandleScheme;
    }

    std::string FileHandleManager::createHandle( std::string const &path, bool folder, bool readOnly )
    {
      Data data;
      data.m_path = path;
      data.m_isFolder = folder;
      data.m_readOnly = readOnly;

      validateAbsolutePath( data.m_path );//Ensure no '..'

      std::pair< PathToHandleMap::const_iterator, PathToHandleMap::const_iterator > existingRange = m_pathToHandle.equal_range( data.m_path );
      while( existingRange.first != existingRange.second )
      {
        const Data& existingData = existingRange.first->second->second;
        if( existingData.m_isFolder == data.m_isFolder && existingData.m_readOnly == data.m_readOnly )
          return existingRange.first->second->first;
        ++existingRange.first;
      }

      std::string handle;
      while( true )
      {
        static const size_t folderIDByteCount = 24;
        uint8_t folderIDBytes[folderIDByteCount];
        Util::generateSecureRandomBytes( folderIDByteCount, folderIDBytes );
        handle = std::string( fileHandleSchemeAndSeparator ) + Util::encodeBase64( folderIDBytes, folderIDByteCount );

        if( m_encodedHandleLength == 0 )
          m_encodedHandleLength = handle.size();
        else
        {
          FABRIC_ASSERT( m_encodedHandleLength == handle.size() );
        }

        if( m_handleToData.find( handle ) == m_handleToData.end() )
          break;

        //If we are here, either we are really not lucky, or the random algo is really bad!
      }

      std::pair<HandleToDataMap::iterator, bool> result = m_handleToData.insert( std::make_pair( handle, data ) );
      m_pathToHandle.insert( std::make_pair( data.m_path, result.first ) );
      return handle;
    }

    std::string FileHandleManager::createRelativeHandle( std::string const &handlePlusRelativePath, bool folder )
    {
      std::string relativePathPostfix;
      Data const &data = validateHandleAndGetData( handlePlusRelativePath, relativePathPostfix );
      if( relativePathPostfix.empty() )
      {
        if( folder != data.m_isFolder )
          throw Exception( "Invalid: trying to change a Folder handle to a File handle or vice-versa" );
        return handlePlusRelativePath;
      }
      FABRIC_ASSERT( data.m_isFolder );//checked by validateHandleAndGetData
      return createHandle( data.m_path + relativePathPostfix, folder, data.m_readOnly );
    }

    bool FileHandleManager::isValid( std::string const &handle ) const
    {
      if( handle.length() < m_encodedHandleLength )
        return false;

      HandleToDataMap::const_iterator iter = m_handleToData.find( std::string( handle, 0, m_encodedHandleLength ) );
      if( iter == m_handleToData.end() )
        return false;

      return handle.length() == m_encodedHandleLength || ( iter->second.m_isFolder && handle[m_encodedHandleLength] == '/' );
    }

    bool FileHandleManager::hasRelativePath( std::string const &handle ) const
    {
      return handle.length() > m_encodedHandleLength;
    }

    FileHandleManager::Data const &FileHandleManager::validateHandleAndGetData( std::string const &handle, std::string& relativePathPostfix ) const
    {
      if( handle.length() < m_encodedHandleLength )
        throw Exception( "Invalid FileHandle" );

      HandleToDataMap::const_iterator iter = m_handleToData.find( handle.substr( 0, m_encodedHandleLength ) );
      if( iter == m_handleToData.end() )
        throw Exception( "Invalid FileHandle" );

      if( handle.length() > m_encodedHandleLength )
      {
        if( !iter->second.m_isFolder )
          throw Exception( "File handles cannot have relative paths" );
        relativePathPostfix.assign( handle.data() + m_encodedHandleLength, handle.length() - m_encodedHandleLength );

        if( relativePathPostfix[0] != '/' )
          throw Exception( "FileHandle relative paths must start with '/'" );

        relativePathPostfix = ChangeSeparatorsURLToFile( relativePathPostfix );
        validateAbsolutePath( relativePathPostfix );//ensure no '..'
      }
      return iter->second;
    }

    std::string FileHandleManager::getPath( std::string const &handle ) const
    {
      std::string relativePathPostfix;
      Data const &data = validateHandleAndGetData( handle, relativePathPostfix );
      return getPathInternal( data, relativePathPostfix );
    }

    std::string FileHandleManager::getPathInternal( Data const &data, std::string const &relativePathPostfix ) const
    {
      return relativePathPostfix.empty() ? data.m_path : data.m_path + relativePathPostfix;
    }

    bool FileHandleManager::isReadOnly( std::string const &handle ) const
    {
      std::string relativePathPostfix;
      Data const &data = validateHandleAndGetData( handle, relativePathPostfix );
      return isReadOnlyInternal( data, relativePathPostfix );
    }

    bool FileHandleManager::isReadOnlyInternal( Data const &data, std::string const &relativePathPostfix ) const
    {
      return data.m_readOnly;
    }

    bool FileHandleManager::isFolder( std::string const &handle ) const
    {
      std::string relativePathPostfix;
      Data const &data = validateHandleAndGetData( handle, relativePathPostfix );
      return isFolderInternal( data, relativePathPostfix );
    }

    bool FileHandleManager::isFolderInternal( Data const &data, std::string const &relativePathPostfix ) const
    {
      if( relativePathPostfix.empty() )
        return data.m_isFolder;
      FABRIC_ASSERT( data.m_isFolder );//checked by validateHandleAndGetData
      std::string fullPath = data.m_path + relativePathPostfix;
      return DirExists( fullPath );
    }

    bool FileHandleManager::targetExists( std::string const &handle ) const
    {
      std::string relativePathPostfix;
      Data const &data = validateHandleAndGetData( handle, relativePathPostfix );
      return targetExistsInternal( data, relativePathPostfix );
    }

    bool FileHandleManager::targetExistsInternal( Data const &data, std::string const &relativePathPostfix ) const
    {
      if( relativePathPostfix.empty() )
      {
        if( data.m_isFolder )
          return DirExists( data.m_path );
        else
          return FileExists( data.m_path );
      }
      FABRIC_ASSERT( data.m_isFolder );//checked by validateHandleAndGetData
      std::string fullPath = data.m_path + relativePathPostfix;
      return DirExists( fullPath ) || FileExists( fullPath );
    }

    void FileHandleManager::ensureTargetExists( std::string const &handle ) const
    {
      std::string relativePathPostfix;
      Data const &data = validateHandleAndGetData( handle, relativePathPostfix );
      ensureTargetExistsInternal( data, relativePathPostfix );
    }

    void FileHandleManager::ensureTargetExistsInternal( Data const &data, std::string const &relativePathPostfix ) const
    {
      if( !targetExistsInternal( data, relativePathPostfix ) )
      {
        if( data.m_readOnly )
          throw Exception( "Error: cannot create file or folders flagged as read-only" );

        if( !relativePathPostfix.empty() )
          throw Exception( "Error: ambiguous: handle's relative path could be a file or a folder; create a file handle from relative path" );

        if( data.m_isFolder )
          CreateDir( data.m_path );
        else
        {
          std::string dir, file;
          SplitPath( data.m_path, dir, file );
          if( !DirExists( dir ) )
            CreateDir( dir );
          if( !FileExists( data.m_path ) )
          {
            std::ofstream file( data.m_path.c_str() );
            if( !file.is_open() )
              throw Exception( "Unable to create file" );
          }
        }
      }
    }

    void FileHandleManager::putFile( std::string const &handle, size_t size, const void* byteData, bool append ) const
    {
      std::string relativePathPostfix;
      Data const &data = validateHandleAndGetData( handle, relativePathPostfix );

      if( isReadOnlyInternal( data, relativePathPostfix ) )
        throw Exception( "File has no write access" );

      if( isFolderInternal( data, relativePathPostfix ) )
        throw Exception( "Handle is a folder; must be a file" );

      ensureTargetExistsInternal( data, relativePathPostfix );
      std::string fullPath = getPathInternal( data, relativePathPostfix );

      std::ofstream file( fullPath.c_str(), std::ios::out | (append ? std::ios::app : std::ios::trunc) | std::ios::binary );
      if( !file.is_open() )
        throw Exception( "Unable to create file" );

      file.write( (const char*)byteData, size );
      if( file.bad() )
        throw Exception( "Error while writing to file" );
    }

    void FileHandleManager::copyFile( std::string const &source, std::string const &target ) const
    {
      if( target == source)
        return;

      std::string targetRelativePathPostfix;
      Data const &targetData = validateHandleAndGetData( target, targetRelativePathPostfix );

      if( isReadOnlyInternal( targetData, targetRelativePathPostfix ) )
        throw Exception( "Target file has no write access" );

      if( isFolderInternal( targetData, targetRelativePathPostfix ) )
        throw Exception( "Target handle is a folder; must be a file" );

      std::string targetFullPath = getPathInternal( targetData, targetRelativePathPostfix );

      std::string sourceRelativePathPostfix;
      Data const &sourceData = validateHandleAndGetData( source, sourceRelativePathPostfix );

      if( !targetExistsInternal( sourceData, sourceRelativePathPostfix ) )
        throw Exception( "Source file not found" );

      if( isFolderInternal( sourceData, sourceRelativePathPostfix ) )
        throw Exception( "Source handle is a folder; must be a file" );

      std::string sourceFullPath = getPathInternal( sourceData, sourceRelativePathPostfix );
      IO::CopyFile_( sourceFullPath, targetFullPath );
    }
  };
};
