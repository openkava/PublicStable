/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Base/Exception.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Base/JSON/Boolean.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Core/Util/Base64.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Core/Util/Random.h>

#include <fstream>

namespace Fabric
{
  namespace IO
  {
    void Manager::jsonRoute(
      std::vector<std::string> const &dst,
      size_t dstOffset,
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( dst.size() - dstOffset == 0 )
      {
        try
        {
          jsonExec( cmd, arg, resultJAG );
        }
        catch ( Exception e )
        {
          throw "command " + _(cmd) + ": " + e;
        }
      }
      else throw Exception( "unroutable" );
    }

    void Manager::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "getUserTextFile" )
        jsonExecGetUserTextFile( arg, resultJAG );
      else if ( cmd == "putUserTextFile" )
        jsonExecPutUserTextFile( arg, resultJAG );
      else if ( cmd == "getTextFile" )
        jsonExecGetTextFile( arg, resultJAG );
      else if ( cmd == "putTextFile" )
        jsonExecPutTextFile( arg, resultJAG );
      else if ( cmd == "queryUserFileAndFolder" )
        jsonExecQueryUserFileAndFolder( arg, resultJAG );
      else
        throw Exception( "unknown command" );
    }

    void Manager::jsonQueryUserFileAndDir(
      RC::ConstHandle<JSON::Value> const &arg,
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

      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      RC::ConstHandle<JSON::Value> val;

      if( existingFile )
      {
        existing = *existingFile;
        writeAccess = !existing;
      }
      else
      {
        existing = argJSONObject->get( "existingFile" )->toBoolean( "existingFile must be a Boolean" )->value();//mandatory in this case

        val = argJSONObject->get( "writeAccess" );
        if( val )
        {
          writeAccess = val->toBoolean( "writeAccess must be a Boolean" )->value();
          if( !existing && !writeAccess )
            throw Exception("Error: trying to save a file with writeAccess == false");
        }
      }

      if(existing && defaultExtension == NULL)
        defaultExtension = "*";

      val = argJSONObject->maybeGet( "uiOptions" );
      if( val )
      {
        RC::ConstHandle<JSON::Object> uiOptionsJSONObject = val->toObject( "uiOptions must be an Object" );

        val = uiOptionsJSONObject->maybeGet( "defaultFileName" );
        if( val )
          defaultFilename = val->toString( "defaultFileName must be a String" )->value();

        val = uiOptionsJSONObject->maybeGet( "extension" );
        if( val )
          extension = val->toString( "extension must be a String" )->value();

        val = uiOptionsJSONObject->maybeGet( "title" );
        if( val )
          title = val->toString( "title must be a String" )->value();
      }

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
      RC::ConstHandle<JSON::Value> const &arg,
      size_t size, const void* data,
      const char* defaultExtension,
      Util::JSONArrayGenerator &resultJAG
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
      RC::ConstHandle<JSON::Value> const &arg,
      ByteContainer& bytes, bool binary,
      std::string& filename,
      std::string& extension,
      Util::JSONArrayGenerator &resultJAG
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

    void Manager::jsonExecGetUserTextFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const
    {
      std::string filename, extension;
      StringByteContainerAdapter stringData;
      jsonExecGetUserFile( arg, stringData, false, filename, extension, resultJAG );
      Util::JSONGenerator resultJG = resultJAG.makeElement();
      resultJG.makeString( stringData.m_string );
    }

    void Manager::jsonExecPutUserTextFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      RC::ConstHandle<JSON::String> content  = argJSONObject->get( "content" )->toString( "content must be a string" );
      jsonExecPutUserFile( arg, content->length(), content->data(), "txt", resultJAG );//"txt": overriden by arg's extension if provided
    }

    void Manager::jsonExecQueryUserFileAndFolder( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
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

      Util::JSONGenerator resultJG = resultJAG.makeElement();
      Util::JSONArrayGenerator pathHandleAndFilenameJAG = resultJG.makeArray();
      {
        Util::JSONGenerator pathHandleJG = pathHandleAndFilenameJAG.makeElement();
        pathHandleJG.makeString( pathHandle );
      }
      {
        Util::JSONGenerator filenameJG = pathHandleAndFilenameJAG.makeElement();
        filenameJG.makeString( filename );
      }
    }

    void Manager::jsonGetFileAndDirFromHandlePath(
      RC::ConstHandle<JSON::Value> const &arg, bool existingFile, RC::ConstHandle<Dir>& dir, std::string& file
      ) const
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      RC::ConstHandle<JSON::Array> pathJSONArray = argJSONObject->get( "path" )->toArray( "path must be an Array" );

      if( pathJSONArray->size() < 2 )
        throw Exception( "path must containt at least a directoryHandle and a filename (size >= 2)" );

      const char* pathStringError = "path array must contain strings";
      const std::string& dirHandle = pathJSONArray->get( 0 )->toString( pathStringError )->value();
      HandleToDirMap::const_iterator handleIt = m_handleToDirMap.find( dirHandle );

      if( handleIt == m_handleToDirMap.end() )
        throw Exception( "Unknown directory handle: '" + dirHandle + "'. Valid directory handles can only be obtained through 'IO.queryUserFileAndFolder'." );

      if(!existingFile && !handleIt->second.m_writeAccess)
        throw Exception( "Directory handle '" + dirHandle + "' was not created with write permission." );

      dir = handleIt->second.m_dir;
      
      size_t i;
      for( i = 1; i < pathJSONArray->size()-1; ++i )
      {
        const std::string& pathElement = pathJSONArray->get( i )->toString( pathStringError )->value();
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
      file = pathJSONArray->get( pathJSONArray->size()-1 )->toString( pathStringError )->value();
    }

    void Manager::jsonExecPutFile(
      RC::ConstHandle<JSON::Value> const &arg,
      size_t size,
      const void* data,
      Util::JSONArrayGenerator &resultJAG
      ) const
    {
      RC::ConstHandle<Dir> dir;
      std::string filename;
      jsonGetFileAndDirFromHandlePath( arg, false, dir, filename );
      putFile( dir, filename, size, data );
    }

    void Manager::jsonExecGetFile(
      RC::ConstHandle<JSON::Value> const &arg,
      ByteContainer& bytes, bool binary,
      std::string& filename,
      std::string& extension,
      Util::JSONArrayGenerator &resultJAG
      ) const
    {
      RC::ConstHandle<Dir> dir;
      jsonGetFileAndDirFromHandlePath( arg, true, dir, filename );

      getFile( dir, filename, binary, bytes );
      extension = GetExtension( filename );
    }

    void Manager::jsonExecGetTextFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const
    {
      RC::ConstHandle<Dir> dir;
      std::string filename;
      jsonGetFileAndDirFromHandlePath( arg, true, dir, filename );

      StringByteContainerAdapter stringData;
      getFile( dir, filename, false, stringData );
      Util::JSONGenerator resultJG = resultJAG.makeElement();
      resultJG.makeString( stringData.m_string );
    }

    void Manager::jsonExecPutTextFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      RC::ConstHandle<Dir> dir;
      std::string filename;
      jsonGetFileAndDirFromHandlePath( arg, false, dir, filename );

      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      RC::ConstHandle<JSON::String> content  = argJSONObject->get( "content" )->toString( "content must be a string" );
      putFile( dir, filename, content->length(), content->data() );
    }
  };
};
