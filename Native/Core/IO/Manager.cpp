/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Base/Exception.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Core/Util/Base64.h>
#include <Fabric/Base/Util/Format.h>
#include <Fabric/Base/JSON/Encoder.h>
#include <Fabric/Core/Util/Random.h>

#include <fstream>

namespace Fabric
{
  namespace IO
  {
    void Manager::jsonRoute(
      std::vector<JSON::Entity> const &dst,
      size_t dstOffset,
      JSON::Entity const &cmd,
      JSON::Entity const &arg,
      JSON::ArrayEncoder &resultArrayEncoder
      )
    {
      if ( dst.size() - dstOffset == 0 )
      {
        try
        {
          jsonExec( cmd, arg, resultArrayEncoder );
        }
        catch ( Exception e )
        {
          throw "command " + _(cmd) + ": " + e;
        }
      }
      else throw Exception( "unroutable" );
    }

    void Manager::jsonExec(
      JSON::Entity const &cmd,
      JSON::Entity const &arg,
      JSON::ArrayEncoder &resultArrayEncoder
      )
    {
      if ( cmd.stringIs( "getUserTextFile", 15 ) )
        jsonExecGetUserTextFile( arg, resultArrayEncoder );
      else if ( cmd.stringIs( "putUserTextFile", 15 ) )
        jsonExecPutUserTextFile( arg, resultArrayEncoder );
      else if ( cmd.stringIs( "getTextFile", 11 ) )
        jsonExecGetTextFile( arg, resultArrayEncoder );
      else if ( cmd.stringIs( "putTextFile", 11 ) )
        jsonExecPutTextFile( arg, resultArrayEncoder );
      else if ( cmd.stringIs( "queryUserFileAndFolder", 22 ) )
        jsonExecQueryUserFileAndFolder( arg, resultArrayEncoder );
      else
        throw Exception( "unknown command" );
    }

    void Manager::jsonQueryUserFileAndDir(
      JSON::Entity const &arg,
      bool *existingFile,
      const char *defaultExtension,
      RC::ConstHandle<Dir>& dir,
      std::string& filename,
      bool& writeAccess
      ) const
    {
      std::string defaultFilename;
      std::string extension;
      std::string title;
      bool existing = false;
      writeAccess = false;

      if ( existingFile )
      {
        existing = *existingFile;
        writeAccess = !existing;
      }

      arg.requireObject();
      JSON::ObjectDecoder argObjectDecoder( arg );
      JSON::Entity keyString, valueEntity;
      while ( argObjectDecoder.getNext( keyString, valueEntity ) )
      {
        try
        {
          if ( !existingFile && keyString.stringIs( "existingFile", 12 ) )
          {
            valueEntity.requireBoolean();
            existing = valueEntity.booleanValue();//mandatory in this case
          }
          else if ( !existingFile && keyString.stringIs( "writeAccess", 11 ) )
          {
            valueEntity.requireBoolean();
            writeAccess = valueEntity.booleanValue();
          }
          else if ( keyString.stringIs( "uiOptions", 9 ) )
          {
            valueEntity.requireObject();
            JSON::ObjectDecoder uiOptionsObjectDecoder( valueEntity );
            JSON::Entity uiOptionKeyString, uiOptionValueEntity;
            while ( uiOptionsObjectDecoder.getNext( uiOptionKeyString, uiOptionValueEntity ) )
            {
              try
              {
                if ( uiOptionKeyString.stringIs( "defaultFileName", 15 ) )
                {
                  uiOptionValueEntity.requireString();
                  defaultFilename = uiOptionValueEntity.stringToStdString();
                }
                else if ( uiOptionKeyString.stringIs( "extension", 9 ) )
                {
                  uiOptionValueEntity.requireString();
                  extension = uiOptionValueEntity.stringToStdString();
                }
                else if ( uiOptionKeyString.stringIs( "title", 5 ) )
                {
                  uiOptionValueEntity.requireString();
                  title = uiOptionValueEntity.stringToStdString();
                }
              }
              catch ( Exception e )
              {
                uiOptionsObjectDecoder.rethrow( e );
              }
            }
          }
        }
        catch ( Exception e )
        {
          argObjectDecoder.rethrow( e );
        }
      }

      if ( !existing && !writeAccess )
        throw Exception("Error: trying to save a file with writeAccess == false");

      if ( existing && defaultExtension == NULL )
        defaultExtension = "*";

      if( extension.empty() && defaultExtension )
        extension = defaultExtension;

      if( title.empty() )
      {
        if( existing )
        {
          if( writeAccess )
            title = "Open File with read & write access to folder...";
          else
            title = "Open File...";
        }
        else
          title = "Save File...";
      }
      else
      {
        //[JCG 20110915] I'm prepending by Open/Save to ensure users can't be mislead, but maybe it's too much control?
        if( existing )
        {
          if( writeAccess )
            title = "Open File with read & write access to folder: " + title;
          else
            title = "Open File: " + title;
        }
        else
          title = "Save File: " + title;
      }

      if( !defaultFilename.empty() && !extension.empty() )
      {
        size_t extPos = defaultFilename.rfind( extension );
        if( !(extPos == defaultFilename.size() - extension.size() && extPos > 0 && defaultFilename[ extPos-1 ] == L'.') )
          defaultFilename += '.' + extension;
      }
      std::string fullPath = queryUserFilePath( existing, title, defaultFilename, extension );

      std::string dirString;
      IO::SplitPath( fullPath, dirString, filename );

      if( !DirExists( dirString ) )
        throw Exception("Error: user selected directory not found");//Important: don't display the directory as it is private information

      dir = IO::Dir::Create( dirString, false );
    }

    std::string GetSafeDisplayPath( RC::ConstHandle<Dir>& dir, std::string const &filename )
    {
      //The root parent directory corresponds to the handle; we don't want to show this as it is private information
      std::string encodedPath = filename;
      while( dir->getParentDir() )
        encodedPath = JoinPath( dir->getEntry(), encodedPath );
      return JoinPath( "[directoryHandle]", encodedPath );
    }

    void Manager::putFile( RC::ConstHandle<Dir>& dir, std::string const &filename, size_t size, const void* data ) const
    {
      std::string fullPath;
      if( dir )
        fullPath = JoinPath( dir->getFullPath(), filename );
      else
        fullPath = filename;

      std::ofstream file( fullPath.c_str(), std::ios::out | std::ios::trunc | std::ios::binary );
      if( !file.is_open() )
        throw Exception( "Unable to create file " + GetSafeDisplayPath(dir, filename) );

      file.write( (const char*)data, size );
      if( file.bad() )
        throw Exception( "Error while writing to file " + GetSafeDisplayPath(dir, filename) );
    }

    void Manager::getFile( RC::ConstHandle<Dir>& dir, std::string const &filename, bool binary, ByteContainer& bytes ) const
    {
      std::string fullPath;
      if( dir )
        fullPath = JoinPath( dir->getFullPath(), filename );
      else
        fullPath = filename;

      std::ios_base::openmode mode = std::ios::in | std::ios::ate;
      if ( binary )
        mode |= std::ios::binary;
      std::ifstream file( fullPath.c_str(), mode );
      if( !file.is_open() )
        throw Exception( "Unable to open file " + GetSafeDisplayPath(dir, filename) );

      size_t size = file.tellg();
      file.seekg (0, std::ios::beg);

      void* data = bytes.Allocate( size );
      if( !data && size )
        throw Exception( "Out of memory while reading file " + GetSafeDisplayPath(dir, filename) );

      file.read( (char*)data, size );
      if( file.bad() )
        throw Exception( "Error while reading file " + GetSafeDisplayPath(dir, filename) );
    }

    void Manager::jsonExecPutUserFile(
      JSON::Entity const &arg,
      size_t size, const void* data,
      const char* defaultExtension,
      JSON::ArrayEncoder &resultArrayEncoder
      ) const
    {
      RC::ConstHandle<Dir> dir;
      std::string filename;

      bool existingFile = false;
      bool writeAccess;
      jsonQueryUserFileAndDir( arg, &existingFile, defaultExtension, dir, filename, writeAccess );
      putFile( dir, filename, size, data );
    }

    void Manager::jsonExecGetUserFile(
      JSON::Entity const &arg,
      ByteContainer& bytes, bool binary,
      std::string& filename,
      std::string& extension,
      JSON::ArrayEncoder &resultArrayEncoder
      ) const
    {
      RC::ConstHandle<Dir> dir;
      bool existingFile = true;
      bool writeAccess;
      jsonQueryUserFileAndDir( arg, &existingFile, NULL, dir, filename, writeAccess );

      getFile( dir, filename, binary, bytes );
      extension = GetExtension( filename );
    }

    struct StringByteContainerAdapter : public IO::Manager::ByteContainer
    {
      virtual void* Allocate( size_t size )
      {
        m_string.resize( size, 0 );
        return (void*)m_string.data();
      }

      std::string m_string;
    };

    void Manager::jsonExecGetUserTextFile( JSON::Entity const &arg, JSON::ArrayEncoder &resultArrayEncoder ) const
    {
      std::string filename, extension;
      StringByteContainerAdapter stringData;
      jsonExecGetUserFile( arg, stringData, false, filename, extension, resultArrayEncoder );
      JSON::Encoder resultEncoder = resultArrayEncoder.makeElement();
      resultEncoder.makeString( stringData.m_string );
    }

    void Manager::jsonExecPutUserTextFile( JSON::Entity const &arg, JSON::ArrayEncoder &resultArrayEncoder ) const
    {
      arg.requireObject();
      JSON::ObjectDecoder argObjectDecoder( arg );
      JSON::Entity keyString, valueEntity;
      while ( argObjectDecoder.getNext( keyString, valueEntity ) )
      {
        try
        {
          if ( keyString.stringIs( "content", 7 ) )
          {
            valueEntity.requireString();
            std::string content = valueEntity.stringToStdString();
            jsonExecPutUserFile( arg, content.length(), content.data(), "txt", resultArrayEncoder );//"txt": overriden by arg's extension if provided
          }
        }
        catch ( Exception e )
        {
          argObjectDecoder.rethrow( e );
        }
      }
    }

    void Manager::jsonExecQueryUserFileAndFolder( JSON::Entity const &arg, JSON::ArrayEncoder &resultArrayEncoder )
    {
      RC::ConstHandle<Dir> dir;
      std::string filename;
      bool writeAccess;

      jsonQueryUserFileAndDir( arg, NULL, NULL, dir, filename, writeAccess );

      static const size_t folderIDByteCount = 96;
      uint8_t folderIDBytes[folderIDByteCount];
      Util::generateSecureRandomBytes( folderIDByteCount, folderIDBytes );
      std::string pathHandle = Util::encodeBase64( folderIDBytes, folderIDByteCount );

      DirInfo dirInfo;
      dirInfo.m_dir = dir;
      dirInfo.m_writeAccess = writeAccess;

      if( m_handleToDirMap.insert( std::make_pair( pathHandle, dirInfo ) ).second == false )
        throw Exception( "Unexpected failure" );

      JSON::Encoder resultEncoder = resultArrayEncoder.makeElement();
      JSON::ArrayEncoder pathHandleAndFilenameArrayEncoder = resultEncoder.makeArray();
      {
        JSON::Encoder pathHandleEncoder = pathHandleAndFilenameArrayEncoder.makeElement();
        pathHandleEncoder.makeString( pathHandle );
      }
      {
        JSON::Encoder filenameEncoder = pathHandleAndFilenameArrayEncoder.makeElement();
        filenameEncoder.makeString( filename );
      }
    }

    void Manager::jsonGetFileAndDirFromHandlePath(
      JSON::Entity const &arg, bool existingFile, RC::ConstHandle<Dir>& dir, std::string& file
      ) const
    {
      std::vector<std::string> path;
            
      arg.requireObject();
      JSON::ObjectDecoder argObjectDecoder( arg );
      JSON::Entity keyString, valueEntity;
      while ( argObjectDecoder.getNext( keyString, valueEntity ) )
      {
        try
        {
          if ( keyString.stringIs( "path", 4 ) )
          {
            valueEntity.requireArray();
            JSON::ArrayDecoder pathArrayDecoder( valueEntity );
            JSON::Entity elementEntity;
            while ( pathArrayDecoder.getNext( elementEntity ) )
            {
              try
              {
                elementEntity.requireString();
                path.push_back( elementEntity.stringToStdString() );
              }
              catch ( Exception e )
              {
                pathArrayDecoder.rethrow( e );
              }
            }
          }
        }
        catch ( Exception e )
        {
          argObjectDecoder.rethrow( e );
        }
      }
      
      if ( path.size() < 2 )
        throw Exception( "path must containt at least a directoryHandle and a filename (size >= 2)" );

      const std::string& dirHandle = path[0];
      HandleToDirMap::const_iterator handleIt = m_handleToDirMap.find( dirHandle );

      if( handleIt == m_handleToDirMap.end() )
        throw Exception( "Unknown directory handle: '" + dirHandle + "'. Valid directory handles can only be obtained through 'IO.queryUserFileAndFolder'." );

      if(!existingFile && !handleIt->second.m_writeAccess)
        throw Exception( "Directory handle '" + dirHandle + "' was not created with write permission." );

      dir = handleIt->second.m_dir;
      
      size_t i;
      for( i = 1; i < path.size()-1; ++i )
      {
        const std::string& pathElement = path[i];
        try
        {
          dir = Dir::Create( dir, pathElement, !existingFile );//Note: this calls validateEntry(pathElement)
          std::string fullPath = dir->getFullPath();
          if( IsLink( fullPath ) )
            throw Exception("File access through path containing symbolic links is prohibed");
          if( existingFile && !DirExists( fullPath ) )
            throw Exception("Directory not found");
        }
        catch ( Exception e )
        {
          throw Exception("Error accessing path element " + pathElement + ": " + e );
        }
      }
      file = path[path.size()-1];
    }

    void Manager::jsonExecPutFile(
      JSON::Entity const &arg,
      size_t size,
      const void* data,
      JSON::ArrayEncoder &resultArrayEncoder
      ) const
    {
      RC::ConstHandle<Dir> dir;
      std::string filename;
      jsonGetFileAndDirFromHandlePath( arg, false, dir, filename );
      putFile( dir, filename, size, data );
    }

    void Manager::jsonExecGetFile(
      JSON::Entity const &arg,
      ByteContainer& bytes, bool binary,
      std::string& filename,
      std::string& extension,
      JSON::ArrayEncoder &resultArrayEncoder
      ) const
    {
      RC::ConstHandle<Dir> dir;
      jsonGetFileAndDirFromHandlePath( arg, true, dir, filename );

      getFile( dir, filename, binary, bytes );
      extension = GetExtension( filename );
    }

    void Manager::jsonExecGetTextFile( JSON::Entity const &arg, JSON::ArrayEncoder &resultArrayEncoder ) const
    {
      RC::ConstHandle<Dir> dir;
      std::string filename;
      jsonGetFileAndDirFromHandlePath( arg, true, dir, filename );

      StringByteContainerAdapter stringData;
      getFile( dir, filename, false, stringData );
      JSON::Encoder resultEncoder = resultArrayEncoder.makeElement();
      resultEncoder.makeString( stringData.m_string );
    }

    void Manager::jsonExecPutTextFile( JSON::Entity const &arg, JSON::ArrayEncoder &resultArrayEncoder )
    {
      RC::ConstHandle<Dir> dir;
      std::string filename;
      jsonGetFileAndDirFromHandlePath( arg, false, dir, filename );

      arg.requireObject();
      JSON::ObjectDecoder argObjectDecoder( arg );
      JSON::Entity keyString, valueEntity;
      while ( argObjectDecoder.getNext( keyString, valueEntity ) )
      {
        try
        {
          if ( keyString.stringIs( "content", 7 ) )
          {
            valueEntity.requireString();
            std::string content = valueEntity.stringToStdString();
            putFile( dir, filename, content.length(), content.data() );
          }
        }
        catch ( Exception e )
        {
          argObjectDecoder.rethrow( e );
        }
      }
    }
  }
}
