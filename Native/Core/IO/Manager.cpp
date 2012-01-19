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
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include "ResourceManager.h"

#include <fstream>

namespace Fabric
{
  namespace IO
  {
    Manager::Manager( ScheduleAsynchCallbackFunc scheduleFunc, void *scheduleFuncUserData )
      : m_resourceManager( ResourceManager::Create( scheduleFunc, scheduleFuncUserData ) )
      , m_fileHandleManager( FileHandleManager::Create() )
      , m_fileHandleResourceProvider( FileHandleResourceProvider::Create() )
    {
      m_resourceManager->registerProvider( RC::Handle<IO::ResourceProvider>::StaticCast( m_fileHandleResourceProvider ) );
    }

    RC::Handle<ResourceManager> Manager::getResourceManager() const
    {
      return m_resourceManager;
    }

    RC::Handle<FileHandleManager> Manager::getFileHandleManager() const
    {
      return m_fileHandleManager;
    }

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
          throw Exception( "command " + _(cmd) + ": " + e );
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
      if ( cmd == "getFileInfo" )
        jsonExecGetFileInfo( arg, resultJAG );
      else if ( cmd == "queryUserFileAndFolder" )
        jsonExecQueryUserFileAndFolder( arg, resultJAG );
      else if ( cmd == "queryUserFile" )
        jsonExecQueryUserFile( arg, resultJAG );
      else if ( cmd == "createFileHandleFromRelativePath" )
        jsonExecCreateFileHandleFromRelativePath( arg, resultJAG );
      else if ( cmd == "createFolderHandleFromRelativePath" )
        jsonExecCreateFolderHandleFromRelativePath( arg, resultJAG );
      else if ( cmd == "getTextFile" )
        jsonExecGetTextFile( arg, resultJAG );
      else if ( cmd == "putTextFile" )
        jsonExecPutTextFile( arg, resultJAG );
      else
        throw Exception( "unknown command" );
    }

    void Manager::jsonQueryUserFile(
      RC::ConstHandle<JSON::Value> const &arg,
      bool *existingFile,
      const char *defaultExtension,
      std::string& fullPath,
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
      fullPath = queryUserFilePath( existing, title, defaultFilename, extension );
    }

    void Manager::jsonExecGetFileInfo( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const
    {
      std::string handle = arg->toString("File or folder handle must be a String")->value();

      Util::JSONGenerator resultJG = resultJAG.makeElement();
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();

      bool isFile = false;
      bool exists = m_fileHandleManager->itemExists( handle );

      {
        Util::JSONGenerator memberJG = resultJOG.makeMember( "type", 4 );
        if( m_fileHandleManager->hasRelativePath( handle ) && !exists )
          memberJG.makeString( "unknown", 7 );
        else if( m_fileHandleManager->isFolder( handle ) )
          memberJG.makeString( "folder", 6 );
        else
        {
          memberJG.makeString( "file", 4 );
          isFile = true;
        }
      }

      {
        Util::JSONGenerator memberJG = resultJOG.makeMember( "writeAccess", 11 );
        memberJG.makeBoolean( !m_fileHandleManager->isReadOnly( handle ) );
      }

      {
        Util::JSONGenerator memberJG = resultJOG.makeMember( "exists", 6 );
        memberJG.makeBoolean( exists );
      }

      if( isFile )
      {
        std::string fullPath = m_fileHandleManager->getPath( handle );

        std::string dir, filename;
        IO::SplitPath( fullPath, dir, filename );

        {
          Util::JSONGenerator memberJG = resultJOG.makeMember( "fileName", 8 );
          memberJG.makeString( filename );
        }

        {
          Util::JSONGenerator memberJG = resultJOG.makeMember( "fileSize", 8 );
          memberJG.makeInteger( (int32_t)GetFileSize( fullPath ) );
        }
      }
    }

    void Manager::jsonExecQueryUserFileAndFolder( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const
    {
      std::string fullPath, dir, filename;
      bool writeAccess;

      jsonQueryUserFile( arg, NULL, NULL, dir, fullPath, writeAccess );

      std::string dir, filename;
      IO::SplitPath( fullPath, dir, filename );

      std::string dirHandle = m_fileHandleManager->createHandle( dir, true, !writeAccess );
      std::string fileHandle = m_fileHandleManager->createHandle( fullPath, false, !writeAccess );

      Util::JSONGenerator resultJG = resultJAG.makeElement();
      Util::JSONObjectGenerator resultJOG = resultJG.makeObject();

      {
        Util::JSONGenerator memberJG = resultJOG.makeMember( "file", 4 );
        memberJG.makeString( fileHandle );
      }
        
      {
        Util::JSONGenerator memberJG = jog.makeMember( "folder", 6 );
        memberJG.makeInteger( dirHandle );
      }
    }

    void Manager::jsonExecQueryUserFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const
    {
      std::string fullPath;
      bool writeAccess;

      jsonQueryUserFile( arg, NULL, NULL, fullPath, writeAccess );

      std::string handle = m_fileHandleManager->createHandle( fullPath, false, !writeAccess );

      Util::JSONGenerator resultJG = resultJAG.makeElement();
      resultJG.makeString( handle );
    }

    void Manager::jsonExecCreateFileHandleFromRelativePath( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const
    {
      std::string handle = arg->toString( "Handle must be a String" )->value();
      std::string newHandle = m_fileHandleManager->createRelativeHandle( handle, false );

      Util::JSONGenerator resultJG = resultJAG.makeElement();
      resultJG.makeString( handle );
    }

    void Manager::jsonExecCreateFolderHandleFromRelativePath( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const
    {
      std::string handle = arg->toString( "Handle must be a String" )->value();
      std::string newHandle = m_fileHandleManager->createRelativeHandle( handle, true );

      Util::JSONGenerator resultJG = resultJAG.makeElement();
      resultJG.makeString( handle );
    }

    void Manager::jsonExecPutTextFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      RC::ConstHandle<JSON::String> content = argJSONObject->get( "content" )->toString( "content must be a String" );
      std::string handle = argJSONObject->get( "file" )->toString( "file handle must be a String" )->value();

      bool append = false;
      val = argJSONObject->maybeGet( "append" );
      if( val )
      {
        append = val->toBoolean( "'append' must be a Boolean" )->value();
      }
      m_fileHandleManager->putFile( handle, content->length(), content->data(), append );
    }

    void Manager::jsonExecGetTextFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const
    {
      std::string handle = arg->toString( "File handle must be a String" )->value();
      std::string fullPath = m_fileHandleManager->getPath( handle );

      if( !FileExists( fullPath ) )
        throw Exception( "Error: file not found" );

      //Note: we don't request file data from ResourceManager beause we need a synchronous response
      size_t fileSize = GetFileSize( fullPath );
      std::string content;
      content.resize( fileSize );

      std::ifstream file( fullPath.c_str(), std::ios::in );
      if( !file.is_open() )
        throw Exception( "Unable to open file" );

      file.read( content.data(), fileSize );
      if( file.bad() )
        throw Exception( "Error while reading file" );

      Util::JSONGenerator resultJG = resultJAG.makeElement();
      resultJG.makeString( content );
    }
  };
};
