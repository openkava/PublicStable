/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "filestream.h"

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

FABRIC_EXT_EXPORT void FabricFileStream_Free(
  FabricFileStream & stream
)
{
  if(stream.m_data != NULL)
  {
    if(stream.m_data->mFile != NULL)
      fclose(stream.m_data->mFile);
    stream.m_data->release();
    stream.m_data = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricFileStream_Open(
  FabricFileStream & stream,
  KL::FabricFileHandle & file,
  KL::String & mode
)
{
  std::string modeStr = mode.data();
  FabricFileStream_Free(stream);
  if(modeStr == "r" || modeStr == "w" || modeStr == "a")
  {
    std::string str(file.getFullPath());
    if(str.empty() || str.length()==0)
      return;
    
    boost::filesystem::path path = str;
    if(modeStr == "r")
    {
      if(!boost::filesystem::exists(path))
        return;
      if(!boost::filesystem::is_regular_file(path))
        return;
    }
    else
    {
      if(!file.hasReadWriteAccess())
        return;
      if(!boost::filesystem::exists(path.parent_path()))
        return;
      if(!boost::filesystem::is_directory(path.parent_path()))
        return;
    }
    
    stream.m_data = new FabricFileStream::LocalData();
    stream.m_data->retain();
    stream.m_data->mMode = modeStr;
    stream.m_data->mReadable = modeStr == "r";
    stream.m_data->mWriteable = !stream.m_data->mReadable;
    stream.m_data->mCloseOnFullyRead = true;
    stream.m_data->mSizeRead = 0;
    if(stream.m_data->mReadable)
    {
      stream.m_data->mSize = boost::filesystem::file_size(path);
      stream.m_data->mSeek = 0;
    }
    else if(modeStr == "w")
    {
      stream.m_data->mSize = 0;
      stream.m_data->mSeek = 0;
    }
    else // "a"
    {
      stream.m_data->mSize = boost::filesystem::file_size(path);
      stream.m_data->mSeek = stream.m_data->mSize;
    }
    stream.m_data->mFile = fopen(path.string().c_str(),modeStr.c_str());
  }
}

FABRIC_EXT_EXPORT void FabricFileStream_CloseOnFullyRead(
  FabricFileStream & stream,
  KL::Boolean & close
)
{
  if(stream.m_data != NULL)
  {
    if(stream.m_data->mReadable)
      stream.m_data->mCloseOnFullyRead = close;
  }
}

FABRIC_EXT_EXPORT KL::Boolean FabricFileStream_IsValid(
  FabricFileStream & stream
)
{
  return stream.m_data != NULL;
}

FABRIC_EXT_EXPORT KL::Size FabricFileStream_GetSize(
  FabricFileStream & stream
)
{
  if(!FabricFileStream_IsValid(stream))
    return 0;
  return stream.m_data->mSize;
}

FABRIC_EXT_EXPORT KL::Size FabricFileStream_GetSizeRead(
  FabricFileStream & stream
)
{
  if(!FabricFileStream_IsValid(stream))
    return 0;
  return stream.m_data->mSizeRead;
}

FABRIC_EXT_EXPORT KL::Size FabricFileStream_GetSeek(
  FabricFileStream & stream
)
{
  if(!FabricFileStream_IsValid(stream))
    return 0;
  return stream.m_data->mSeek;
}

FABRIC_EXT_EXPORT void FabricFileStream_SetSeek(
  FabricFileStream & stream,
  KL::Size & seek
)
{
  if(!FabricFileStream_IsValid(stream))
    return;
  if(!stream.m_data->mReadable)
    return;
  if(seek >= stream.m_data->mSize)
    return;
  fseek(stream.m_data->mFile,(long int)seek,SEEK_SET);
  stream.m_data->mSeek = seek;
}

FABRIC_EXT_EXPORT void FabricFileStream_WriteData(
  FabricFileStream & stream,
  KL::Data & data,
  KL::Size & size
)
{
  if(!FabricFileStream_IsValid(stream))
    return;
  if(!stream.m_data->mWriteable)
    return;
  fwrite(data,size,1,stream.m_data->mFile);
  stream.m_data->mSize += size;
  stream.m_data->mSeek += size;
}

FABRIC_EXT_EXPORT void FabricFileStream_ReadData(
  FabricFileStream & stream,
  KL::Data & data,
  KL::Size & size
)
{
  if(!FabricFileStream_IsValid(stream))
    return;
  if(!stream.m_data->mReadable)
    return;
  if(stream.m_data->mSeek + size > stream.m_data->mSize)
    return;
  size_t readSize = fread(data,size,1,stream.m_data->mFile);
  stream.m_data->mSizeRead += readSize;
  stream.m_data->mSeek += readSize;
  if(stream.m_data->mCloseOnFullyRead && stream.m_data->mSizeRead >= stream.m_data->mSize)
    FabricFileStream_Free(stream);
}

FABRIC_EXT_EXPORT void FabricFileStream_WriteString(
  FabricFileStream & stream,
  KL::String & string
)
{
  if(!FabricFileStream_IsValid(stream))
    return;
  if(!stream.m_data->mWriteable)
    return;
  size_t length = string.length();
  fwrite(&length,sizeof(size_t),1,stream.m_data->mFile);
  stream.m_data->mSize += sizeof(size_t);
  stream.m_data->mSeek += sizeof(size_t);
  if(length > 0)
  {
    fwrite(string.data(),length,1,stream.m_data->mFile);
    stream.m_data->mSize += length;
    stream.m_data->mSeek += length;
  }
}

FABRIC_EXT_EXPORT void FabricFileStream_ReadString(
  FabricFileStream & stream,
  KL::String & string
)
{
  if(!FabricFileStream_IsValid(stream))
    return;
  if(!stream.m_data->mReadable)
    return;
  if(stream.m_data->mSeek + sizeof(size_t) > stream.m_data->mSize)
    return;
  size_t length = 0;
  size_t readSize = fread(&length,sizeof(size_t),1,stream.m_data->mFile);
  stream.m_data->mSizeRead += readSize;
  stream.m_data->mSeek += readSize;

  if(stream.m_data->mSeek + length <= stream.m_data->mSize)
  {
    string.reserve(length);
    if(length > 0)
    {
      readSize = fread((void*)string.data(),length,1,stream.m_data->mFile);
      stream.m_data->mSizeRead += readSize;
      stream.m_data->mSeek += readSize;
    }
  }
  if(stream.m_data->mCloseOnFullyRead && stream.m_data->mSizeRead >= stream.m_data->mSize)
    FabricFileStream_Free(stream);
}

FABRIC_EXT_EXPORT void FabricFileStream_WriteStringArray(
  FabricFileStream & stream,
  KL::VariableArray<KL::String> & strings
)
{
  if(!FabricFileStream_IsValid(stream))
    return;
  if(!stream.m_data->mWriteable)
    return;
  size_t count = strings.size();
  fwrite(&count,sizeof(size_t),1,stream.m_data->mFile);
  stream.m_data->mSize += sizeof(size_t);
  stream.m_data->mSeek += sizeof(size_t);
  for(size_t i=0;i<count;i++)
    FabricFileStream_WriteString(stream,strings[i]);
}

FABRIC_EXT_EXPORT void FabricFileStream_ReadStringArray(
  FabricFileStream & stream,
  KL::VariableArray<KL::String> & strings
)
{
  if(!FabricFileStream_IsValid(stream))
    return;
  if(!stream.m_data->mReadable)
    return;
  if(stream.m_data->mSeek + sizeof(size_t) > stream.m_data->mSize)
    return;
  size_t count = 0;
  size_t readSize = fread(&count,sizeof(size_t),1,stream.m_data->mFile);
  stream.m_data->mSizeRead += readSize;
  stream.m_data->mSeek += readSize;
  strings.resize(count);
  for(size_t i=0;i<count;i++)
    FabricFileStream_ReadString(stream,strings[i]);
}