/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_IO_MANAGER_H
#define _FABRIC_IO_MANAGER_H

#include <Fabric/Core/IO/Stream.h>
#include  <map>
#include  <vector>

namespace Fabric
{
  namespace JSON
  {
    class Value;
  };

  namespace IO
  {
    class Dir;

    class Manager : public RC::Object
    {
      typedef std::map<std::string, RC::ConstHandle<Dir> > HandleToDirMap;

    public:
    
      virtual RC::Handle<Stream> createStream(
        std::string const &url,
        Stream::DataCallback dataCallback,
        Stream::EndCallback endCallback,
        Stream::FailureCallback failureCallback,
        RC::Handle<RC::Object> const &target,
        void *userData = NULL
        ) const = 0;

      struct ByteContainer
      {
        virtual void* Allocate( size_t size ) = 0;
      };

      virtual RC::ConstHandle<JSON::Value> jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      virtual RC::ConstHandle<JSON::Value> jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );

      void jsonExecPutFile( RC::ConstHandle<JSON::Value> const &arg, size_t size, const void* data ) const;
      void jsonExecGetFile( RC::ConstHandle<JSON::Value> const &arg, ByteContainer& bytes, bool binary, std::string& filename, std::string& extension ) const;

      void jsonExecPutUserFile( RC::ConstHandle<JSON::Value> const &arg, size_t size, const void* data, const char* defaultExtension = NULL ) const;
      void jsonExecGetUserFile( RC::ConstHandle<JSON::Value> const &arg, ByteContainer& bytes, bool binary, std::string& filename, std::string& extension ) const;

    protected:

      virtual void queryUserFileAndDir(
        bool existingFile,
        std::string const &title,
        std::string const &defaultFilename,
        std::string const &extension,
        RC::ConstHandle<Dir>& dir,
        std::string& file
        ) const = 0;

    private:

      void putFile( RC::ConstHandle<Dir>& dir, std::string const &filename, size_t size, const void* data ) const;
      void getFile( RC::ConstHandle<Dir>& dir, std::string const &filename, bool binary, ByteContainer& bytes ) const;

      void jsonQueryUserFileAndDir( RC::ConstHandle<JSON::Value> const &arg, bool *existingFile, const char *defaultExtension, RC::ConstHandle<Dir>& dir, std::string& filename ) const;
      void jsonGetFileAndDirFromHandlePath( RC::ConstHandle<JSON::Value> const &arg, bool existingFile, RC::ConstHandle<Dir>& dir, std::string& file ) const;

      RC::ConstHandle<JSON::Value> jsonExecQueryUserFileAndFolder( RC::ConstHandle<JSON::Value> const &arg );
      RC::ConstHandle<JSON::Value> jsonExecGetUserTextFile( RC::ConstHandle<JSON::Value> const &arg ) const;
      void jsonExecPutUserTextFile( RC::ConstHandle<JSON::Value> const &arg ) const;
      RC::ConstHandle<JSON::Value> jsonExecGetTextFile( RC::ConstHandle<JSON::Value> const &arg ) const;
      void jsonExecPutTextFile( RC::ConstHandle<JSON::Value> const &arg );

      HandleToDirMap m_handleToDirMap;
    };
  };
};

#endif //_FABRIC_IO_MANAGER_H
