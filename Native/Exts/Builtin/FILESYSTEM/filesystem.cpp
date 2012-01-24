/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "filesystem.h"

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

FABRIC_EXT_EXPORT void FabricFolderHandle_Free(
  FabricFolderHandle & folder
)
{
  if(folder.m_data != NULL)
  {
    folder.m_data->release();
    folder.m_data = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricFolderHandle_SetAbsolutePath(
  FabricFolderHandle & folder,
  KL::String & path
)
{
  if(folder.m_data == NULL)
  {
    folder.m_data = new FabricFolderHandle::LocalData();
    folder.m_data->retain();
  }
  folder.m_data->mPath = std::string(path.data());
  if(boost::filesystem::is_regular_file(folder.m_data->mPath))
    folder.m_data->mPath = folder.m_data->mPath.parent_path();
}

FABRIC_EXT_EXPORT void FabricFolderHandle_GetAbsolutePath(
  FabricFolderHandle & folder,
  KL::String& result
)
{
  result = KL::String();
  if(folder.m_data != NULL)
  {
    std::string str = folder.m_data->mPath.string();
    if(!str.empty())
    {
      result.reserve(str.length());
      memcpy((void*)result.data(),str.c_str(),str.length());
    }
  }
}

FABRIC_EXT_EXPORT void FabricFolderHandle_GetBaseName(
  FabricFolderHandle & folder,
  KL::String& result
)
{
  result = KL::String();
  if(folder.m_data != NULL)
  {
    std::string str = folder.m_data->mPath.stem().string();
    if(!str.empty())
    {
      result.reserve(str.length());
      memcpy((void*)result.data(),str.c_str(),str.length());
    }
  }
}

FABRIC_EXT_EXPORT KL::Boolean FabricFolderHandle_IsValid(
  FabricFolderHandle & folder
)
{
  KL::Boolean result = false;
  if(folder.m_data != NULL)
  {
    std::string str = folder.m_data->mPath.string();
    result = !str.empty() && str.length() > 0;
  }
  return result;
}

FABRIC_EXT_EXPORT KL::Boolean FabricFolderHandle_Exists(
  FabricFolderHandle & folder
)
{
  KL::Boolean result = false;
  if(folder.m_data != NULL)
    result = boost::filesystem::exists(folder.m_data->mPath);
  return result;
}

FABRIC_EXT_EXPORT FabricFolderHandle FabricFolderHandle_GetParentFolder(
  FabricFolderHandle & folder
)
{
  FabricFolderHandle result;
  result.m_data = NULL;
  if(folder.m_data != NULL)
  {
    boost::filesystem::path path = folder.m_data->mPath.parent_path();
    std::string str = path.string();
    if(!str.empty())
    {
      result.m_data = new FabricFolderHandle::LocalData();
      result.m_data->retain();
      result.m_data->mPath = path;
    }
  }
  return result;
}

FABRIC_EXT_EXPORT void FabricFolderHandle_GetSubFolders(
  FabricFolderHandle & folder,
  KL::VariableArray<FabricFolderHandle> & result
)
{
  if(folder.m_data != NULL)
  {
    std::vector<boost::filesystem::path> paths;
    boost::filesystem::directory_iterator it(folder.m_data->mPath);
    boost::filesystem::directory_iterator end;
    while(it != end)
    {
      if(boost::filesystem::is_directory(it->path()))
        paths.push_back(it->path());
      it++;
    }
    result.resize(paths.size());
    for(size_t i=0;i<paths.size();i++)
    {
      result[i].m_data = new FabricFolderHandle::LocalData();
      result[i].m_data->retain();
      result[i].m_data->mPath = paths[i];
    }
  }
}

FABRIC_EXT_EXPORT void FabricFolderHandle_GetFiles(
  FabricFolderHandle & folder,
  KL::VariableArray<KL::String> & result
)
{
  if(folder.m_data != NULL)
  {
    std::vector<boost::filesystem::path> paths;
    boost::filesystem::directory_iterator it(folder.m_data->mPath);
    boost::filesystem::directory_iterator end;
    while(it != end)
    {
      if(boost::filesystem::is_regular_file(it->path()))
        paths.push_back(it->path());
      it++;
    }
    result.resize(paths.size());
    for(size_t i=0;i<paths.size();i++)
    {
      KL::FileHandleWrapper handle;
      handle.createFromFile(paths[i].string().c_str(),true);
      result[i] = handle;
    }
  }
}

FABRIC_EXT_EXPORT void FabricFileHandle_SetAbsolutePath(
  KL::String & file,
  KL::String & path
)
{
  std::string str(path.data());
  KL::FileHandleWrapper handle;
  handle.createFromFile(str.c_str(),false);
  file = handle;
}

FABRIC_EXT_EXPORT void FabricFileHandle_GetAbsolutePath(
  KL::String & file,
  KL::String& result
)
{
  result = KL::String();
  KL::FileHandleWrapper handle( file );
  result = handle.getPath();
}

FABRIC_EXT_EXPORT FabricFolderHandle FabricFileHandle_GetParentFolder(
  KL::String & file
)
{
  FabricFolderHandle result;
  result.m_data = NULL;
  KL::FileHandleWrapper handle( file );
  std::string str( handle.getPath().data() );
  if(!str.empty())
  {
    result.m_data = new FabricFolderHandle::LocalData();
    result.m_data->retain();
    result.m_data->mPath = str;
  }
  return result;
}

FABRIC_EXT_EXPORT void FabricFileHandle_GetName(
  KL::String & file,
  KL::String& result
)
{
  result = KL::String();
  KL::FileHandleWrapper handle( file );
  std::string str( handle.getPath().data() );
  if(!str.empty())
  {
    boost::filesystem::path path = str;
    str = path.filename().string();
    result.reserve(str.length());
    memcpy((void*)result.data(),str.c_str(),str.length());
  }
}

FABRIC_EXT_EXPORT void FabricFileHandle_GetBaseName(
  KL::String & file,
  KL::String& result
)
{
  result = KL::String();
  KL::FileHandleWrapper handle( file );
  std::string str( handle.getPath().data() );
  if(!str.empty())
  {
    boost::filesystem::path path = str;
    str = path.stem().string();
    result.reserve(str.length());
    memcpy((void*)result.data(),str.c_str(),str.length());
  }
}

FABRIC_EXT_EXPORT void FabricFileHandle_GetExtension(
  KL::String & file,
  KL::String& result
)
{
  result = KL::String();
  KL::FileHandleWrapper handle( file );
  std::string str( handle.getPath().data() );
  if(!str.empty())
  {
    boost::filesystem::path path = str;
    str = path.extension().string();
    result.reserve(str.length());
    memcpy((void*)result.data(),str.c_str(),str.length());
  }
}

FABRIC_EXT_EXPORT void FabricFileHandle_GetExtensionLower(
  KL::String & file,
  KL::String& result
)
{
  result = KL::String();
  KL::FileHandleWrapper handle( file );
  std::string str( handle.getPath().data() );
  if(!str.empty())
  {
    boost::filesystem::path path = str;
    str = path.extension().string();
    std::transform(str.begin(), str.end(),str.begin(), ::tolower);
    result.reserve(str.length());
    memcpy((void*)result.data(),str.c_str(),str.length());
  }
}

FABRIC_EXT_EXPORT KL::Boolean FabricFileHandle_IsValid(
  KL::String & file
)
{
  KL::FileHandleWrapper handle( file );
  if( !handle.isValid() || handle.isFolder() )
    return false;
  std::string str( handle.getPath().data() );
  return !str.empty();
}

FABRIC_EXT_EXPORT KL::Boolean FabricFileHandle_Exists(
  KL::String & file
)
{
  KL::Boolean result = false;
  KL::FileHandleWrapper handle( file );
  std::string str( handle.getPath().data() );
  if(!str.empty())
  {
    boost::filesystem::path path = str;
    result = boost::filesystem::exists(path);
  }
  return result;
}

FABRIC_EXT_EXPORT KL::Boolean FabricFileHandle_IsReadOnly(
  KL::String & file
)
{
  KL::FileHandleWrapper handle( file );
  return handle.isReadOnly();
}

FABRIC_EXT_EXPORT KL::Size FabricFileHandle_GetSize(
  KL::String & file
)
{
  KL::Size result = 0;
  KL::FileHandleWrapper handle( file );
  std::string str( handle.getPath().data() );
  if(!str.empty())
  {
    boost::filesystem::path path = str;
    result = boost::filesystem::file_size(path);
  }
  return result;
}
