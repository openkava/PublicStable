/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_IO_MANAGER_H
#define _FABRIC_IO_MANAGER_H

#include <Fabric/Core/IO/Stream.h>
#include <map>
#include <vector>
#include <Fabric/Core/IO/Dir.h>

namespace Fabric
{
  namespace Util
  {
    class JSONGenerator;
    class JSONArrayGenerator;
  };
  
  namespace JSON
  {
    class Value;
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

      virtual void jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      virtual void jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );

      void jsonExecPutFile( RC::ConstHandle<JSON::Value> const &arg, size_t size, const void* data, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecGetFile( RC::ConstHandle<JSON::Value> const &arg, ByteContainer& bytes, bool binary, std::string& filename, std::string& extension, Util::JSONArrayGenerator &resultJAG ) const;

      void jsonExecPutUserFile( RC::ConstHandle<JSON::Value> const &arg, size_t size, const void* data, const char* defaultExtension, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecGetUserFile( RC::ConstHandle<JSON::Value> const &arg, ByteContainer& bytes, bool binary, std::string& filename, std::string& extension, Util::JSONArrayGenerator &resultJAG ) const;

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

      void jsonQueryUserFileAndDir( RC::ConstHandle<JSON::Value> const &arg, bool *existingFile, const char *defaultExtension, RC::ConstHandle<Dir>& dir, std::string& filename, bool& writeAccess ) const;
      void jsonGetFileAndDirFromHandlePath( RC::ConstHandle<JSON::Value> const &arg, bool existingFile, RC::ConstHandle<Dir>& dir, std::string& file ) const;

      void jsonExecQueryUserFileAndFolder( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecGetUserTextFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecPutUserTextFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecGetTextFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecPutTextFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );

      HandleToDirMap m_handleToDirMap;
    };
  };
};

#endif //_FABRIC_IO_MANAGER_H
