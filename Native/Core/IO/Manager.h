/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_IO_MANAGER_H
#define _FABRIC_IO_MANAGER_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <map>
#include <vector>

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
    class ResourceManager;
    class FileHandleManager;
    class FileHandleResourceProvider;
    typedef void (*ScheduleAsyncCallbackFunc)( void* scheduleUserData, void (*callbackFunc)(void *), void *callbackFuncUserData );

    class Manager : public RC::Object
    {

    public:
    
      RC::Handle<ResourceManager> getResourceManager() const;
      RC::Handle<FileHandleManager> getFileHandleManager() const;

      virtual void jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      virtual void jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );

    protected:
      Manager( ScheduleAsyncCallbackFunc scheduleFunc, void *scheduleFuncUserData );

      virtual std::string queryUserFilePath(
        bool existingFile,
        std::string const &title,
        std::string const &defaultFilename,
        std::string const &extension
        ) const = 0;

    private:
      void jsonQueryUserFile( RC::ConstHandle<JSON::Value> const &arg, bool *existingFile, const char *defaultExtension, std::string& fullPath, bool& writeAccess ) const;

      void jsonExecGetFileInfo( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecQueryUserFileAndFolder( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecQueryUserFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecCreateFileHandleFromRelativePath( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecCreateFolderHandleFromRelativePath( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecPutTextFileContent( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecGetTextFileContent( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const;

      RC::Handle<ResourceManager> m_resourceManager;
      RC::Handle<FileHandleManager> m_fileHandleManager;
      RC::Handle<FileHandleResourceProvider> m_fileHandleResourceProvider;
    };
  };
};

#endif //_FABRIC_IO_MANAGER_H
