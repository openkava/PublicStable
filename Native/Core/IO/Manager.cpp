/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Base/Exception.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Base/JSON/Boolean.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/IO/Dir.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Core/Util/Base64.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/Random.h>
#include <fstream>

namespace Fabric
{
  namespace IO
  {

    RC::ConstHandle<JSON::Value> Manager::jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;

      if ( dst.size() - dstOffset == 0 )
      {
        try
        {
          result = jsonExec( cmd, arg );
        }
        catch ( Exception e )
        {
          throw "command " + _(cmd) + ": " + e;
        }
      }
      else throw Exception( "unroutable" );
      
      return result;
    }

    RC::ConstHandle<JSON::Value> Manager::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;

      if ( cmd == "getUserTextFile" )
        result = jsonExecGetUserTextFile( arg );
      else if ( cmd == "putUserTextFile" )
        jsonExecPutUserTextFile( arg );
      else if ( cmd == "getTextFile" )
        result = jsonExecGetTextFile( arg );
      else if ( cmd == "putTextFile" )
        jsonExecPutTextFile( arg );
      else if ( cmd == "queryUserFileAndFolder" )
        result = jsonExecQueryUserFileAndFolder( arg );
      else
        throw Exception( "unknown command" );

      return result;
    }

    void Manager::jsonQueryUserFileAndDir( RC::ConstHandle<JSON::Value> const &arg, bool *existingFile, const char *defaultExtension, RC::ConstHandle<Dir>& dir, std::string& filename ) const
    {
      std::string defaultFilename;
      std::string extension;
      std::string title;
      bool existing = false;

      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      RC::ConstHandle<JSON::Value> val;

      if( existingFile )
        existing = *existingFile;
      else
      {
        val = argJSONObject->get( "existingFile" );//mandatory in this case
        if( val )
          existing = val->toBoolean( "existingFile must be a Boolean" )->value();
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
          title = "Open File...";
        else
          title = "Save File...";
      }
      else
      {
        //[JCG 20110915] I'm prepending by Open/Save to ensure users can't be mislead, but maybe it's too much control?
        if( existing )
          title = "Open File: " + title;
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
      dir = IO::Dir::Create( dirString, false );
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
        throw Exception( "Unable to create file " + fullPath );

      file.write( (const char*)data, size );
      if( file.bad() )
        throw Exception( "Error while writing to file " + fullPath );
    }

    void Manager::getFile( RC::ConstHandle<Dir>& dir, std::string const &filename, bool binary, ByteContainer& bytes ) const
    {
      std::string fullPath;
      if( dir )
        fullPath = JoinPath( dir->getFullPath(), filename );
      else
        fullPath = filename;

      std::ifstream file( fullPath.c_str(), std::ios::in | std::ios::ate | (binary ? std::ios::binary : 0 ) );
      if( !file.is_open() )
        throw Exception( "Unable to open file " + fullPath );

      size_t size = file.tellg();
      file.seekg (0, std::ios::beg);

      void* data = bytes.Allocate( size );
      if( !data && size )
        throw Exception( "Out of memory while reading file " + fullPath );

      file.read( (char*)data, size );
      if( file.bad() )
        throw Exception( "Error while reading file " + fullPath );
    }

    void Manager::jsonExecPutUserFile( RC::ConstHandle<JSON::Value> const &arg, size_t size, const void* data, const char* defaultExtension ) const
    {
      RC::ConstHandle<Dir> dir;
      std::string filename;

      bool existingFile = false;
      jsonQueryUserFileAndDir( arg, &existingFile, defaultExtension, dir, filename );
      putFile( dir, filename, size, data );
    }

    void Manager::jsonExecGetUserFile( RC::ConstHandle<JSON::Value> const &arg, ByteContainer& bytes, bool binary, std::string& filename, std::string& extension ) const
    {
      RC::ConstHandle<Dir> dir;
      bool existingFile = true;
      jsonQueryUserFileAndDir( arg, &existingFile, NULL, dir, filename );

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

    RC::ConstHandle<JSON::Value> Manager::jsonExecGetUserTextFile( RC::ConstHandle<JSON::Value> const &arg ) const
    {
      std::string filename, extension;
      StringByteContainerAdapter stringData;
      jsonExecGetUserFile( arg, stringData, false, filename, extension );
      return JSON::String::Create( stringData.m_string );
    }

    void Manager::jsonExecPutUserTextFile( RC::ConstHandle<JSON::Value> const &arg ) const
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      RC::ConstHandle<JSON::String> content  = argJSONObject->get( "content" )->toString( "content must be a string" );
      jsonExecPutUserFile( arg, content->length(), content->data(), "txt" );//"txt": overriden by arg's extension if provided
    }

    RC::ConstHandle<JSON::Value> Manager::jsonExecQueryUserFileAndFolder( RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<Dir> dir;
      std::string filename;

      jsonQueryUserFileAndDir( arg, NULL, NULL, dir, filename );

      static const size_t folderIDByteCount = 96;
      uint8_t folderIDBytes[folderIDByteCount];
      Util::generateSecureRandomBytes( folderIDByteCount, folderIDBytes );
      std::string pathHandle = Util::encodeBase64( folderIDBytes, folderIDByteCount );

      if( m_handleToDirMap.insert( std::make_pair( pathHandle, dir ) ).second == false )
        throw Exception( "Unexpected failure" );

      RC::Handle<JSON::Array> returnVal = JSON::Array::Create();
      returnVal->push_back( JSON::String::Create( pathHandle ) );
      returnVal->push_back( JSON::String::Create( filename ) );
      return returnVal;
    }

    void Manager::jsonGetFileAndDirFromHandlePath( RC::ConstHandle<JSON::Value> const &arg, bool existingFile, RC::ConstHandle<Dir>& dir, std::string& file ) const
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

      dir = handleIt->second;
      
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

    void Manager::jsonExecPutFile( RC::ConstHandle<JSON::Value> const &arg, size_t size, const void* data ) const
    {
      RC::ConstHandle<Dir> dir;
      std::string filename;
      jsonGetFileAndDirFromHandlePath( arg, false, dir, filename );
      putFile( dir, filename, size, data );
    }

    void Manager::jsonExecGetFile( RC::ConstHandle<JSON::Value> const &arg, ByteContainer& bytes, bool binary, std::string& filename, std::string& extension ) const
    {
      RC::ConstHandle<Dir> dir;
      jsonGetFileAndDirFromHandlePath( arg, true, dir, filename );

      getFile( dir, filename, binary, bytes );
      extension = GetExtension( filename );
    }

    RC::ConstHandle<JSON::Value> Manager::jsonExecGetTextFile( RC::ConstHandle<JSON::Value> const &arg ) const
    {
      RC::ConstHandle<Dir> dir;
      std::string filename;
      jsonGetFileAndDirFromHandlePath( arg, true, dir, filename );

      StringByteContainerAdapter stringData;
      getFile( dir, filename, false, stringData );
      return JSON::String::Create( stringData.m_string );
    }

    void Manager::jsonExecPutTextFile( RC::ConstHandle<JSON::Value> const &arg )
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
