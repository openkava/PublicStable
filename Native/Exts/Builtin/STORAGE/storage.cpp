/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>
#include <Fabric/Base/RC/Object.h>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <boost/filesystem.hpp>

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

#if defined(FABRIC_OS_WINDOWS)
#define SEPARATOR '\\'
#define WRONGSEPARATOR '/'
#else
#define SEPARATOR '/'
#define WRONGSEPARATOR '\\'
#endif

FABRIC_EXT_KL_STRUCT( FileHandle, {
  class LocalData : public Fabric::RC::Object {
  public:
  
    LocalData();
    
    void init(const std::string & path);
    bool valid() const;
    bool exists() const;
    bool isFolder();
    size_t size();
    std::string absolute() const;
    std::string filename() const;
    std::string filebasename();
    std::string fileextension();
    std::string fileextensionlower();
    void push(const std::string & segment);
    void pop();
    void getSubHandles(KL::VariableArray<FileHandle> & handles);
    
    void loadBinary(KL::VariableArray<KL::Byte> & data);
    void saveBinary(KL::VariableArray<KL::Byte> & data);
    void loadAscii(KL::String::IO data);
    void saveAscii(KL::String::IO data);

  private:
  
    virtual ~LocalData();

    std::vector<std::string> mParts;
    int mIsFolder;
  };

  LocalData * localData;
} );

FileHandle::LocalData::LocalData()
{
  mIsFolder = -1;
}

FileHandle::LocalData::~LocalData()
{
  mParts.clear();
}

void FileHandle::LocalData::init(const std::string & path)
{
  mParts.clear();
  mIsFolder = -1;

  if(path.length() < 2)
    return;

  push(path);
}

bool FileHandle::LocalData::valid() const
{
  return mParts.size() > 0;
}

bool FileHandle::LocalData::isFolder()
{
  // we will cache the result of this
  if(mIsFolder == -1 && valid())
  {
    std::string path = absolute();
    if(path.length() == 0)
      return false;

    if(boost::filesystem::is_directory(path.c_str()))
      mIsFolder = 1;
    else if(boost::filesystem::is_regular_file(path.c_str()))
      mIsFolder = 0;
    else
      mIsFolder = -1;
  }
  
  return mIsFolder == 1;
}

size_t FileHandle::LocalData::size()
{
  // we will cache the result of this
  std::string path = absolute();
  if(path.length() == 0)
    return 0;
  
  if(!exists())
    return 0;
  if(isFolder())
    return 0;
  
  return (size_t)boost::filesystem::file_size(path.c_str());
}

bool FileHandle::LocalData::exists() const
{
  std::string path = absolute();
  if(path.length() == 0)
    return false;
  
  return boost::filesystem::exists(path.c_str());
}

std::string FileHandle::LocalData::absolute() const
{
  if(!valid())
    return std::string();
    
  std::string result;
#if defined(FABRIC_OS_WINDOWS)
  if(mParts[0].length() > 1)
  {
    if(mParts[0][1] != ':') // UNC
    {
      result += SEPARATOR;
      result += SEPARATOR;
    }
  }
#else
  result += SEPARATOR;
#endif
  result += mParts[0];
  for(size_t i=1;i<mParts.size();i++) {
    result += SEPARATOR;
    result += mParts[i];
  }
  return result;
}

std::string FileHandle::LocalData::filename() const
{
  if(!valid())
    return std::string();
  return mParts[mParts.size()-1];
}

std::string FileHandle::LocalData::filebasename()
{
  if(!valid())
    return std::string();
  if(isFolder())
    return mParts[mParts.size()-1];

  std::vector<std::string> parts;
  boost::split(parts, mParts[mParts.size()-1], boost::is_any_of("."));
  parts.pop_back();
  return boost::join(parts,".");
}

std::string FileHandle::LocalData::fileextension()
{
  if(!valid() || isFolder())
    return std::string();
  std::vector<std::string> parts;
  boost::split(parts, mParts[mParts.size()-1], boost::is_any_of("."));
  return parts[parts.size()-1];
}

std::string FileHandle::LocalData::fileextensionlower()
{
  std::string ext = fileextension();
  std::transform(ext.begin(), ext.end(),ext.begin(), ::tolower);
  return ext;
}

void FileHandle::LocalData::push(const std::string & segment)
{
  std::string osPath = segment;
  for(size_t i=0;i<osPath.length();i++)
  {
    if(osPath[i] == WRONGSEPARATOR)
      osPath[i] = SEPARATOR;
  }
  
#if defined(FABRIC_OS_WINDOWS)
  // check UNC path
  if(mParts.size() == 0 && osPath[0] == SEPARATOR)
  {
    // if the first is a separator,
    // the second has to be one too
    if(osPath[1] != SEPARATOR)
      return;
  }
#endif

  std::vector<std::string> parts;
  boost::split(parts, osPath, boost::is_any_of("/\\"));
  for(size_t i=0;i<parts.size();i++)
  {
    if(parts[i].length() > 0)
      mParts.push_back(parts[i]);
  }
  mIsFolder = -1;
}

void FileHandle::LocalData::pop()
{
  if(!valid())
    return;
  mParts.pop_back();
  if(mParts.size() > 0)
    mIsFolder = 1;
  else
    mIsFolder = -1;
}

void FileHandle::LocalData::getSubHandles(KL::VariableArray<FileHandle> & handles)
{
  handles.resize(0);
  if(!exists())
    return;
  if(!isFolder())
    return;
  
  std::string parent = absolute();
  boost::filesystem::directory_iterator it(parent.c_str()), end;
  std::vector<std::string> paths;
  while(it != end)
  {
    paths.push_back(it->path().string());
    it++;
  }
  
  handles.resize(paths.size());
  for(size_t i=0;i<handles.size();i++)
  {
    handles[i].localData = new FileHandle::LocalData();
    handles[i].localData->retain();
    handles[i].localData->init(paths[i]);
  }
}

void FileHandle::LocalData::loadBinary(KL::VariableArray<KL::Byte> & data)
{
  data.resize(0);

  if(!valid())
    return;

  // get the file size
  if(!exists())
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR loadBinary: '%s' does not exist.\n",absolute().c_str());
#endif
    return;
  }
  if(isFolder())
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR loadBinary: '%s' is a directory.\n",absolute().c_str());
#endif
    return;
  }
  size_t fileSize = size();
  if(fileSize == 0)
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR loadBinary: '%s' has zero size.\n",absolute().c_str());
#endif
    return;
  }

  FILE * file = fopen(absolute().c_str(),"rb");
  if(file == NULL)
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR loadBinary: '%s' could not be opened for reading.\n",absolute().c_str());
#endif
    return;
  }
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : loadBinary: File size is '%d'.\n",(int)fileSize);
#endif
  data.resize(fileSize);
  size_t result = fread(&data[0],1,fileSize,file);
  fclose(file);
  if(result != fileSize)
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR loadBinary: '%s', incorrect amount of bytes read.\n",absolute().c_str());
#endif
    data.resize(0);
    return;
  }
}

void FileHandle::LocalData::loadAscii(KL::String::IO data)
{
  data.reserve(0);

  if(!valid())
    return;

  // get the file size
  if(!exists())
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR loadAscii: '%s' does not exist.\n",absolute().c_str());
#endif
    return;
  }
  if(isFolder())
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR loadAscii: '%s' is a directory.\n",absolute().c_str());
#endif
    return;
  }
  size_t fileSize = size();
  if(fileSize == 0)
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR loadAscii: '%s' has zero size.\n",absolute().c_str());
#endif
    return;
  }

  FILE * file = fopen(absolute().c_str(),"r");
  if(file == NULL)
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR loadAscii: '%s' could not be opened for reading.\n",absolute().c_str());
#endif
    return;
  }
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : loadAscii: File size is '%d'.\n",(int)fileSize);
#endif
  data.reserve(fileSize);
  size_t result = fread((void*)data.data(),1,fileSize,file);
  fclose(file);
  if(result != fileSize)
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR loadAscii: '%s', incorrect amount of bytes read.\n",absolute().c_str());
#endif
    data.reserve(0);
    return;
  }
}

void FileHandle::LocalData::saveBinary(KL::VariableArray<KL::Byte> & data)
{
  if(!valid())
    return;
  if(data.size() == 0)
    return;
  
  // get the folder of the file
  LocalData folder;
  folder.init(absolute());
  folder.pop();

  // only check if this is a valid folder
  if(folder.valid())
  {
    if(!folder.exists())
    {
#ifndef NDEBUG
      printf("  { FabricSTORAGE } : ERROR saveBinary: Folder '%s', does not exist.\n",folder.absolute().c_str());
      return;
#endif
    }
  }

  FILE * file = fopen(absolute().c_str(),"wb");
  if(file == NULL)
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR saveBinary: '%s' could not be opened for writing.\n",absolute().c_str());
#endif
    return;
  }
  size_t result = fwrite(&data[0],data.size(),1,file);
  fclose(file);
  if(result != data.size())
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR saveBinary: '%s', incorrect amount of bytes written.\n",absolute().c_str());
#endif
  }
}

void FileHandle::LocalData::saveAscii(KL::String::IO data)
{
  if(!valid())
    return;
  if(data.length() == 0)
    return;
  
  // get the folder of the file
  LocalData folder;
  folder.init(absolute());
  folder.pop();

  // only check if this is a valid folder
  if(folder.valid())
  {
    if(!folder.exists())
    {
#ifndef NDEBUG
      printf("  { FabricSTORAGE } : ERROR saveAscii: Folder '%s', does not exist.\n",folder.absolute().c_str());
      return;
#endif
    }
  }

  FILE * file = fopen(absolute().c_str(),"w");
  if(file == NULL)
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR saveAscii: '%s' could not be opened for writing.\n",absolute().c_str());
#endif
    return;
  }
  size_t result = fwrite(data.data(),data.length(),1,file);
  fclose(file);
  if(result != data.length())
  {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : ERROR saveAscii: '%s', incorrect amount of bytes written.\n",absolute().c_str());
#endif
  }
}

FABRIC_EXT_EXPORT void FabricSTORAGE_Handle_Free(
  FileHandle & handle
)
{
  if(handle.localData != NULL) {
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Free called.\n");
#endif
    handle.localData->release();
    handle.localData = NULL;
#ifndef NDEBUG
    printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Free completed.\n");
#endif
  }
}

FABRIC_EXT_EXPORT void FabricSTORAGE_Handle_Init(
  FileHandle & handle,
  KL::String::IO path
)
{
#ifndef NDEBUG
  if(path.length() > 0)
    printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Init called (%s).\n",path.data());
  else
    printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Init called ().\n");
#endif
  if(path.length() > 0)
  {
    if(handle.localData == NULL) {
      handle.localData = new FileHandle::LocalData();
      handle.localData->retain();
    }
    handle.localData->init(path.data());
  }
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Init completed.\n");
#endif
}

FABRIC_EXT_EXPORT KL::Boolean FabricSTORAGE_Handle_Valid( FileHandle & handle )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Valid called.\n");
#endif
  bool result = false;
  if(handle.localData != NULL)
    result = handle.localData->valid();
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Valid completed.\n");
#endif
  return result;
}

FABRIC_EXT_EXPORT KL::Boolean FabricSTORAGE_Handle_Exists( FileHandle & handle )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Exists called.\n");
#endif
  bool result = false;
  if(handle.localData != NULL)
    result = handle.localData->exists();
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Exists completed.\n");
#endif
  return result;
}

FABRIC_EXT_EXPORT KL::Boolean FabricSTORAGE_Handle_IsFolder( FileHandle & handle )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_IsFolder called.\n");
#endif
  bool result = false;
  if(handle.localData != NULL)
    result = handle.localData->isFolder();
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_IsFolder completed.\n");
#endif
  return result;
}

FABRIC_EXT_EXPORT KL::Size FabricSTORAGE_Handle_Size( FileHandle & handle )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Size called.\n");
#endif
  size_t result = 0;
  if(handle.localData != NULL)
    result = handle.localData->size();
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Size completed.\n");
#endif
  return result;
}

FABRIC_EXT_EXPORT KL::String FabricSTORAGE_Handle_Absolute( FileHandle & handle )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Absolute called.\n");
#endif
  KL::String result;
  if(handle.localData != NULL)
    result = handle.localData->absolute().c_str();
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Absolute completed.\n");
#endif
  return result;
}

FABRIC_EXT_EXPORT KL::String FabricSTORAGE_Handle_FileName( FileHandle & handle )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_FileName called.\n");
#endif
  KL::String result;
  if(handle.localData != NULL)
    result = handle.localData->filename().c_str();
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_FileName completed.\n");
#endif
  return result;
}

FABRIC_EXT_EXPORT KL::String FabricSTORAGE_Handle_FileBaseName( FileHandle & handle )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_FileBaseName called.\n");
#endif
  KL::String result;
  if(handle.localData != NULL)
    result = handle.localData->filebasename().c_str();
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_FileBaseName completed.\n");
#endif
  return result;
}

FABRIC_EXT_EXPORT KL::String FabricSTORAGE_Handle_FileExtension( FileHandle & handle )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_FileExtension called.\n");
#endif
  KL::String result;
  if(handle.localData != NULL)
    result = handle.localData->fileextension().c_str();
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_FileExtension completed.\n");
#endif
  return result;
}

FABRIC_EXT_EXPORT KL::String FabricSTORAGE_Handle_FileExtensionLower( FileHandle & handle )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_FileExtensionLower called.\n");
#endif
  KL::String result;
  if(handle.localData != NULL)
    result = handle.localData->fileextensionlower().c_str();
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_FileExtensionLower completed.\n");
#endif
  return result;
}

FABRIC_EXT_EXPORT void FabricSTORAGE_Handle_Push( FileHandle & handle, KL::String::IO segment )
{
#ifndef NDEBUG
  if(segment.length() > 0)
    printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Push called (%s).\n",segment.data());
  else
    printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Push called ().\n");
#endif
  if(segment.length() > 0)
  {
    if(handle.localData == NULL) {
      handle.localData = new FileHandle::LocalData();
      handle.localData->retain();
    }
    handle.localData->push(segment.data());
  }
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Push completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricSTORAGE_Handle_Pop( FileHandle & handle )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Pop called.\n");
#endif
  if(handle.localData != NULL)
    handle.localData->pop();
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_Pop completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricSTORAGE_Handle_LoadBinary( FileHandle & handle, KL::VariableArray<KL::Byte> & data )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_LoadBinary called.\n");
#endif
  if(handle.localData != NULL)
    handle.localData->loadBinary(data);
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_LoadBinary completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricSTORAGE_Handle_SaveBinary( FileHandle & handle, KL::VariableArray<KL::Byte> & data )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_SaveBinary called.\n");
#endif
  if(handle.localData != NULL)
    handle.localData->saveBinary(data);
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_SaveBinary completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricSTORAGE_Handle_LoadAscii( FileHandle & handle, KL::String::IO data )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_LoadAscii called.\n");
#endif
  if(handle.localData != NULL)
    handle.localData->loadAscii(data);
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_LoadAscii completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricSTORAGE_Handle_SaveAscii( FileHandle & handle, KL::String::IO data )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_SaveAscii called.\n");
#endif
  if(handle.localData != NULL)
    handle.localData->saveAscii(data);
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_SaveAscii completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricSTORAGE_Handle_GetSubHandles( FileHandle & handle, KL::VariableArray<FileHandle> & subhandles )
{
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_GetSubHandles called.\n");
#endif
  if(handle.localData != NULL)
    handle.localData->getSubHandles(subhandles);
#ifndef NDEBUG
  printf("  { FabricSTORAGE } : FabricSTORAGE_Handle_GetSubHandles completed.\n");
#endif
}
