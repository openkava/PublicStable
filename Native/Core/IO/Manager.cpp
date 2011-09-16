/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/IO/Dir.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Base/Exception.h>
#include <fstream>

namespace Fabric
{
  namespace IO
  {
    void Manager::jsonQueryUserFileAndDir( RC::ConstHandle<JSON::Value> const &arg, bool *existingFile, const char *defaultExtension, RC::ConstHandle<Dir>& dir, std::string& file ) const
    {
      std::string defaultFileName;
      std::string extension;
      std::string title;
      bool existing = false;

      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      RC::ConstHandle<JSON::Value> val;

      val = argJSONObject->maybeGet( "uiOptions" );
      if( val )
      {
        RC::ConstHandle<JSON::Object> uiOptionsJSONObject = val->toObject();

        val = uiOptionsJSONObject->maybeGet( "defaultFilename" );
        if( val )
          defaultFileName = val->toString()->value();
        else
        {
          val = uiOptionsJSONObject->maybeGet( "defaultFileName" );
          if( val )
            defaultFileName = val->toString()->value();
        }

        val = uiOptionsJSONObject->maybeGet( "extension" );
        if( val )
          extension = val->toString()->value();

        val = argJSONObject->maybeGet( "title" );
        if( val )
          title = val->toString()->value();
      }

      if( existingFile )
        existing = *existingFile;
      else
      {
        val = argJSONObject->get( "existingFile" );//mandatory in this case
        if( val )
          existing = val->toBoolean()->value();
      }

      if( extension.empty() && defaultExtension )
        extension = *defaultExtension;

      if( title.empty() )
      {
        if( existing )
          completeTitle = "Open File...";
        else
          completeTitle = "Save File...";
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
      queryUserFileAndDir( existing, title, defaultFilename, extension, dir, filename );
    }

    void Manager::putUserFile(
      std::string const &title,
      std::string const &defaultFilename,
      std::string const &extension,
      size_t size,
      const void* data
      ) const
    {
      std::string filename;
      RC::ConstHandle<Dir> dir;
      queryUserFileAndDirInternal( false, title, defaultFilename, extension, dir, filename );

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
        result = jsonExecPutUserTextFile( arg );
      else if ( cmd == "getTextFile" )
        result = jsonExecGetTextFile( arg );
      else if ( cmd == "putTextFile" )
        result = jsonExecPutTextFile( arg );
      else if ( cmd == "queryUserFileAndFolder" )
        result = jsonExecQueryUserFileAndFolder( arg );
      else throw Exception( "unroutable" );

      return result;
    }

    void parseJSONUserFileArgs(
      std::string &titleArg,
      std::string &defaultFilenameArg,
      std::string &extensionArg,
      bool *existingFileArg = NULL
      ) const
    {
    }

  };
};
