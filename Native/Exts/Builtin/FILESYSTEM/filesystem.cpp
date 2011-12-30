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

FABRIC_EXT_EXPORT KL::String FabricFolderHandle_GetAbsolutePath(
  FabricFolderHandle & folder
)
{
  KL::String result;
  if(folder.m_data != NULL)
  {
    std::string str = folder.m_data->mPath.string();
    if(!str.empty())
    {
      result.reserve(str.length());
      memcpy((void*)result.data(),str.c_str(),str.length());
    }
  }
  return result;
}

FABRIC_EXT_EXPORT KL::String FabricFolderHandle_GetBaseName(
  FabricFolderHandle & folder
)
{
  KL::String result;
  if(folder.m_data != NULL)
  {
    std::string str = folder.m_data->mPath.stem().string();
    if(!str.empty())
    {
      result.reserve(str.length());
      memcpy((void*)result.data(),str.c_str(),str.length());
    }
  }
  return result;
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
  KL::VariableArray<KL::FabricFileHandle> & result
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
      result[i].setFromPath(paths[i].string().c_str(),true);
  }
}

FABRIC_EXT_EXPORT void FabricFileHandle_SetAbsolutePath(
  KL::FabricFileHandle & file,
  KL::String & path
)
{
  std::string str(path.data());
  file.setFromPath(str.c_str(),true);
}

FABRIC_EXT_EXPORT KL::String FabricFileHandle_GetAbsolutePath(
  KL::FabricFileHandle & file
)
{
  KL::String result;
  std::string str = file.getFullPath();
  if(!str.empty())
  {
    result.reserve(str.length());
    memcpy((void*)result.data(),str.c_str(),str.length());
  }
  return result;
}

FABRIC_EXT_EXPORT FabricFolderHandle FabricFileHandle_GetParentFolder(
  KL::FabricFileHandle & file
)
{
  FabricFolderHandle result;
  result.m_data = NULL;
  std::string str = file.getFullPath();
  if(!str.empty())
  {
    result.m_data = new FabricFolderHandle::LocalData();
    result.m_data->retain();
    result.m_data->mPath = str;
  }
  return result;
}

FABRIC_EXT_EXPORT KL::String FabricFileHandle_GetName(
  KL::FabricFileHandle & file
)
{
  KL::String result;
  std::string str = file.getFullPath();
  if(!str.empty())
  {
    boost::filesystem::path path = str;
    str = path.filename().string();
    result.reserve(str.length());
    memcpy((void*)result.data(),str.c_str(),str.length());
  }
  return result;
}

FABRIC_EXT_EXPORT KL::String FabricFileHandle_GetBaseName(
  KL::FabricFileHandle & file
)
{
  KL::String result;
  std::string str = file.getFullPath();
  if(!str.empty())
  {
    boost::filesystem::path path = str;
    str = path.stem().string();
    result.reserve(str.length());
    memcpy((void*)result.data(),str.c_str(),str.length());
  }
  return result;
}

FABRIC_EXT_EXPORT KL::String FabricFileHandle_GetExtension(
  KL::FabricFileHandle & file
)
{
  KL::String result;
  std::string str = file.getFullPath();
  if(!str.empty())
  {
    boost::filesystem::path path = str;
    str = path.extension().string();
    result.reserve(str.length());
    memcpy((void*)result.data(),str.c_str(),str.length());
  }
  return result;
}

FABRIC_EXT_EXPORT KL::String FabricFileHandle_GetExtensionLower(
  KL::FabricFileHandle & file
)
{
  KL::String result;
  std::string str = file.getFullPath();
  if(!str.empty())
  {
    boost::filesystem::path path = str;
    str = path.extension().string();
    std::transform(str.begin(), str.end(),str.begin(), ::tolower);
    result.reserve(str.length());
    memcpy((void*)result.data(),str.c_str(),str.length());
  }
  return result;
}

FABRIC_EXT_EXPORT KL::Boolean FabricFileHandle_IsValid(
  KL::FabricFileHandle & file
)
{
  KL::Boolean result = false;
  std::string str = file.getFullPath();
  result = !str.empty() && str.length() > 0;
  return result;
}

FABRIC_EXT_EXPORT KL::Boolean FabricFileHandle_Exists(
  KL::FabricFileHandle & file
)
{
  KL::Boolean result = false;
  std::string str = file.getFullPath();
  if(!str.empty())
  {
    boost::filesystem::path path = str;
    result = boost::filesystem::exists(path);
  }
  return result;
}

FABRIC_EXT_EXPORT KL::Boolean FabricFileHandle_IsReadOnly(
  KL::FabricFileHandle & file
)
{
  return !file.hasReadWriteAccess();
}

FABRIC_EXT_EXPORT KL::Size FabricFileHandle_GetSize(
  KL::FabricFileHandle & file
)
{
  KL::Size result = 0;
  std::string str = file.getFullPath();
  if(!str.empty())
  {
    boost::filesystem::path path = str;
    result = boost::filesystem::file_size(path);
  }
  return result;
}
