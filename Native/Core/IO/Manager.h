/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_IO_MANAGER_H
#define _FABRIC_IO_MANAGER_H

#include <Fabric/Core/IO/Stream.h>
#include <Fabric/Core/IO/Dir.h>
#include <Fabric/Base/JSON/Decoder.h>

#include <map>
#include <vector>

namespace Fabric
{
  namespace JSON
  {
    class ArrayEncoder;
    class Encoder;
  };

  namespace IO
  {
    class Dir;

    class Manager : public RC::Object
    {
      struct DirInfo
      {
        DirInfo()
          :m_writeAccess(false)
        {}

        RC::ConstHandle<Dir> m_dir;
        bool m_writeAccess;
      };

      typedef std::map<std::string, DirInfo > HandleToDirMap;

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

      virtual void jsonRoute( std::vector<JSON::Entity> const &dst, size_t dstOffset, JSON::Entity const &cmd, JSON::Entity const &arg, JSON::ArrayEncoder &resultArrayEncoder );
      virtual void jsonExec( JSON::Entity const &cmd, JSON::Entity const &arg, JSON::ArrayEncoder &resultArrayEncoder );

      void jsonExecPutFile( JSON::Entity const &arg, size_t size, const void* data, JSON::ArrayEncoder &resultArrayEncoder ) const;
      void jsonExecGetFile( JSON::Entity const &arg, ByteContainer& bytes, bool binary, std::string& filename, std::string& extension, JSON::ArrayEncoder &resultArrayEncoder ) const;

      void jsonExecPutUserFile( JSON::Entity const &arg, size_t size, const void* data, const char* defaultExtension, JSON::ArrayEncoder &resultArrayEncoder ) const;
      void jsonExecGetUserFile( JSON::Entity const &arg, ByteContainer& bytes, bool binary, std::string& filename, std::string& extension, JSON::ArrayEncoder &resultArrayEncoder ) const;

    protected:

      virtual std::string queryUserFilePath(
        bool existingFile,
        std::string const &title,
        std::string const &defaultFilename,
        std::string const &extension
        ) const = 0;

    private:

      void putFile( RC::ConstHandle<Dir>& dir, std::string const &filename, size_t size, const void* data ) const;
      void getFile( RC::ConstHandle<Dir>& dir, std::string const &filename, bool binary, ByteContainer& bytes ) const;

      void jsonQueryUserFileAndDir( JSON::Entity const &arg, bool *existingFile, const char *defaultExtension, RC::ConstHandle<Dir>& dir, std::string& filename, bool& writeAccess ) const;
      void jsonGetFileAndDirFromHandlePath( JSON::Entity const &arg, bool existingFile, RC::ConstHandle<Dir>& dir, std::string& file ) const;

      void jsonExecQueryUserFileAndFolder( JSON::Entity const &arg, JSON::ArrayEncoder &resultArrayEncoder );
      void jsonExecGetUserTextFile( JSON::Entity const &arg, JSON::ArrayEncoder &resultArrayEncoder ) const;
      void jsonExecPutUserTextFile( JSON::Entity const &arg, JSON::ArrayEncoder &resultArrayEncoder ) const;
      void jsonExecGetTextFile( JSON::Entity const &arg, JSON::ArrayEncoder &resultArrayEncoder ) const;
      void jsonExecPutTextFile( JSON::Entity const &arg, JSON::ArrayEncoder &resultArrayEncoder );

      HandleToDirMap m_handleToDirMap;
    };
  };
};

#endif //_FABRIC_IO_MANAGER_H
