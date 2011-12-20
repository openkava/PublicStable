/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>
#include <Fabric/Base/RC/Object.h>

#include <utility>
#include <limits>
#include <zlib.h>

#include "blowfish.h"

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

FABRIC_EXT_KL_STRUCT( SecureElement, {
  KL::Integer index;
  KL::String name;
  KL::String type;
  KL::Integer slicecount;
  KL::Integer sliceindex;
  KL::Integer datacount;
  KL::Integer version;
} );

#define baseOffset 16
#define DEFAULT_KEY "0123456789ABCDEF"
enum offsetType{
  offsetType_version,
  offsetType_nbElements,
  offsetType_elementName,
  offsetType_elementType,
  offsetType_elementVersion,
  offsetType_elementSliceCount,
  offsetType_elementSliceIndex,
  offsetType_elementDataCount,
  offsetType_elementData,
  offsetType_MAXENUM
};

const int SECURECONTAINER_VERSION_FIRST = 100;
const int SECURECONTAINER_VERSION_CURRENT = SECURECONTAINER_VERSION_FIRST;

FABRIC_EXT_KL_STRUCT( SecureContainer, {
  class LocalData : public Fabric::RC::Object {
  public:
  
    LocalData();

    void decode(void * data, size_t size, const KL::String & key);
    void encode(KL::VariableArray<KL::Byte> & data, KL::Integer level, const KL::String & key);
    unsigned int getVersion();
    unsigned int getNbElements();
    SecureElement getElementDescription(size_t index);
    void getElementData(size_t index, void**data, size_t size);
    void addElement(SecureElement & element, void * data, size_t size);

  protected:
    
    size_t getOffset(offsetType type, size_t index = 0);
    virtual ~LocalData();

    bool mAllocated;
    char * mData;
    size_t mSize;
  };

  LocalData * localData;
} );

SecureContainer::LocalData::LocalData() {
  mAllocated = false;
  mData = NULL;
  mSize = 0;
}

void SecureContainer::LocalData::encode(KL::VariableArray<KL::Byte> & data, KL::Integer level, const KL::String & key)
{
  if(mData == NULL || mSize == 0)
    return;
  
#ifndef NDEBUG
  printf("  { FabricSECURE } : Uncompressed size: %d.\n",(int)mSize);
#endif
  data.resize(24+mSize);
  uLongf compressedSize = mSize;
  if(level > 9) level = 9;
  if(level < 0) level = 0;
  if(compress2((Bytef*)&data[24],&compressedSize,(const Bytef*)mData,mSize,level) != Z_OK)
  {
    printf("  { FabricSECURE } : ERROR during compressing!\n");
    data.resize(0);
    return;
  }
  
#ifndef NDEBUG
  printf("  { FabricSECURE } : Compressed size: %d.\n",(int)compressedSize);
#endif

  // store the size pre compression
  ((unsigned int*)&data[0])[0] = mSize;
  ((unsigned int*)&data[0])[1] = compressedSize;

  // create a key of maximum size 16
  KL::String key16 = DEFAULT_KEY;
  memcpy((char*)key16.data(),key.data(),key.length() > 16 ? 16 : key.length());
  memcpy(&data[8],key16.data(),16);

  // ensure to be multiple of 8
  size_t cryptSize = 16+compressedSize;
  if(cryptSize % 8 > 0)
    cryptSize += 8 - compressedSize % 8;
  data.resize(8+cryptSize);

#ifndef NDEBUG
  printf("  { FabricSECURE } : Encryption size: %d.\n",(int)cryptSize);
#endif

  // now encrypt (if the key has a length)
  if(key.length() > 0)
  {
#ifndef NDEBUG
    printf("  { FabricSECURE } : Encryption key used '%s'.\n",key16.data());
#endif
    BlowFish::CBlowFish fish((unsigned char*)key16.data(),16);
    fish.Encrypt((unsigned char*)&data[8],cryptSize);
#ifndef NDEBUG
    printf("  { FabricSECURE } : Encrypted in place.\n");
#endif
  }
}

void SecureContainer::LocalData::decode(void * data, size_t size, const KL::String & key)
{
  if(size == 0 || data == NULL)
    return;
  if(mAllocated)
    free(mData);

#ifndef NDEBUG
  printf("  { FabricSECURE } : Uncompressing data of size %d.\n",(int)size);
#endif
    
  mSize = ((unsigned int*)data)[0];
  size_t compressedSize = ((unsigned int*)data)[1];
  KL::String existingKey = DEFAULT_KEY;
  memcpy((char*)existingKey.data(),&((char*)data)[8],16);

#ifndef NDEBUG
  printf("  { FabricSECURE } : Original compressed size %d.\n",(int)compressedSize);
  printf("  { FabricSECURE } : Original uncompressed size %d.\n",(int)mSize);
#endif

  mData = (char*)malloc(mSize);
  mAllocated = true;
  
  // decrypt in case we have a key
  KL::String key16 = DEFAULT_KEY;
  memcpy((char*)key16.data(),key.data(),key.length() > 16 ? 16 : key.length());
  
  // we can decrypt the data in place, and we will avoid to do this several times
  std::string stdKey16(key16.data());
  if(std::string(existingKey.data()) != stdKey16 && std::string(existingKey.data()) != DEFAULT_KEY)
  {
#ifndef NDEBUG
    printf("  { FabricSECURE } : Decription key: '%s'.\n",key16.data());
#endif
    // try to decode
    BlowFish::CBlowFish fish((unsigned char*)key16.data(),16);
    fish.Decrypt(&((unsigned char*)data)[8],size-8);
#ifndef NDEBUG
    printf("  { FabricSECURE } : Decrypted in place.\n");
#endif
    // check keys
    memcpy((char*)existingKey.data(),&((char*)data)[8],16);
    if(std::string(existingKey.data()) != std::string(key16.data()))
    {
      printf("  { FabricSECURE } : ERROR: Incorrect key '%s' !\n",key16.data());
      if(mData != NULL)
        free(mData);
      mSize = 0;
      return;
    }
  }
  
  uLongf uncompressedSize = mSize;
  if(uncompress((Bytef*)mData,&uncompressedSize,(const Bytef*)&((char*)data)[24],compressedSize) != Z_OK)
  {
    printf("  { FabricSECURE } : ERROR during uncompressing, probably incorrect decryption key!\n");
    if(mData != NULL)
      free(mData);
    mSize = 0;
    return;
  }
  mSize = uncompressedSize;

#ifndef NDEBUG
  printf("  { FabricSECURE } : Uncompressed size %d.\n",(int)mSize);
#endif
}

size_t SecureContainer::LocalData::getOffset(offsetType type, size_t index) {
  if(mData == NULL)
    return 0;
  switch(type)
  {
    case offsetType_version:
      return 0;
    case offsetType_nbElements:
      return 4;
    case offsetType_elementName:
    {
      size_t result = getOffset(offsetType_nbElements) + 4;
      for(size_t i=0;i<index;i++)
      {
        unsigned int nameLength = ((unsigned int*)(&mData[result]))[0];
        result += 4 + nameLength;
        unsigned int typeLength = ((unsigned int*)(&mData[result]))[0];
        result += 4 + typeLength;
        result += 4; // slicecount
        result += 4; // sliceindex
        result += 4; // datacount
        result += 4; // version
        unsigned int dataLength = ((unsigned int*)(&mData[result]))[0];
        result += 4 + dataLength;
      }
      return result;
    }
    case offsetType_elementType:
    {
      size_t result = getOffset(offsetType_elementName,index);
      unsigned int nameLength = ((unsigned int*)(&mData[result]))[0];
      result += 4 + nameLength;
      return result;
    }
    case offsetType_elementSliceCount:
    {
      size_t result = getOffset(offsetType_elementType,index);
      unsigned int typeLength = ((unsigned int*)(&mData[result]))[0];
      result += 4 + typeLength;
      return result;
    }
    case offsetType_elementSliceIndex:
    {
      size_t result = getOffset(offsetType_elementSliceCount,index) + 4;
      return result;
    }
    case offsetType_elementDataCount:
    {
      size_t result = getOffset(offsetType_elementSliceIndex,index) + 4;
      return result;
    }
    case offsetType_elementVersion:
    {
      size_t result = getOffset(offsetType_elementDataCount,index) + 4;
      return result;
    }
    case offsetType_elementData:
    {
      size_t result = getOffset(offsetType_elementVersion,index) + 4;
      return result;
    }
    default:
      return 0;
  }
}

unsigned int SecureContainer::LocalData::getVersion()
{
  if(!mAllocated || mData == NULL)
    return 0;
  size_t offset = getOffset(offsetType_version);
  return ((unsigned int*)(&mData[offset]))[0];
}

unsigned int SecureContainer::LocalData::getNbElements()
{
  if(!mAllocated || mData == NULL)
    return 0;
  size_t offset = getOffset(offsetType_nbElements);
  return ((unsigned int*)(&mData[offset]))[0];
}

SecureElement SecureContainer::LocalData::getElementDescription(size_t index)
{
  SecureElement element;
  if(!mAllocated || mData == NULL)
    return element;
  element.index = index;
  size_t offset = getOffset(offsetType_elementName,index);
  element.name = &mData[offset+4]; // + length (int)
  offset += 4 + (size_t)((unsigned int*)(&mData[offset]))[0];
  element.type = &mData[offset+4]; // + length (int)
  offset += 4 + (size_t)((unsigned int*)(&mData[offset]))[0];
  element.slicecount = ((unsigned int*)(&mData[offset]))[0];
  offset += 4;
  element.sliceindex = ((unsigned int*)(&mData[offset]))[0];
  offset += 4;
  element.datacount = ((unsigned int*)(&mData[offset]))[0];
  offset += 4;
  element.version = ((unsigned int*)(&mData[offset]))[0];
  return element;
}

void SecureContainer::LocalData::getElementData(size_t index, void**data, size_t size)
{
  if(!mAllocated || mData == NULL)
    return;
  size_t offset = getOffset(offsetType_elementData,index);
  size_t storedSize = (size_t)((unsigned int*)(&mData[offset]))[0];
  if(storedSize != size)
  {
    SecureElement element = getElementDescription(index);
    printf("{  FabricSECURE  }: ERROR: Element (%d)'%s': Size (%d) does not match one in buffer (%d).\n",(int)index,element.name.data(),(int)size,(int)storedSize);
    *data = NULL;
    return;
  }
  *data = &mData[offset+4];
}

void SecureContainer::LocalData::addElement(SecureElement & element, void * data, size_t size)
{
#ifndef NDEBUG
  printf("  { FabricSECURE } : New Element (%d)'%s', type '%s', slice %d, size %d.\n",(int)getNbElements(),element.name.data(),element.type.data(),(int)element.sliceindex,(int)size);
#endif

  unsigned int nbElements = getNbElements();
  
  // compute the size to allocate
  size_t newSize = mSize;
  if(mSize == 0)
    newSize = 8; // version + nbElements
  newSize += 4 + element.name.length() + 1; // length + name + \0
  newSize += 4 + element.type.length() + 1; // length + type + \0
  newSize += 4; // elementslicecount
  newSize += 4; // elementslice
  newSize += 4; // elementcount
  newSize += 4; // elementversion
  newSize += 4 + size; // data
  char * newData = (char*)malloc(newSize);
  if(mSize >= 0)
  {
    memcpy(newData,mData,mSize);
    if(mAllocated)
      free(mData);
    mData = NULL;
  }
  ((unsigned int*)&newData[0])[0] = SECURECONTAINER_VERSION_CURRENT;
  ((unsigned int*)&newData[4])[0] = nbElements + 1;
  size_t offset = mSize == 0 ? 8 : mSize;
  ((unsigned int*)&newData[offset])[0] = element.name.length() + 1;
  offset += 4;
  memcpy(&newData[offset],element.name.data(),element.name.length());
  offset += element.name.length();
  newData[offset] = '\0';
  offset += 1;
  ((unsigned int*)&newData[offset])[0] = element.type.length() + 1;
  offset += 4;
  memcpy(&newData[offset],element.type.data(),element.type.length());
  offset += element.type.length();
  newData[offset] = '\0';
  offset += 1;
  ((unsigned int*)&newData[offset])[0] = (unsigned int)element.slicecount;
  offset += 4;
  ((unsigned int*)&newData[offset])[0] = (unsigned int)element.sliceindex;
  offset += 4;
  ((unsigned int*)&newData[offset])[0] = (unsigned int)element.datacount;
  offset += 4;
  ((unsigned int*)&newData[offset])[0] = (unsigned int)element.version;
  offset += 4;
  ((unsigned int*)&newData[offset])[0] = size;
  offset += 4;
  memcpy(&newData[offset],data,size);

  mData = newData;
  mSize = newSize;
  mAllocated = true;
}

SecureContainer::LocalData::~LocalData() {
  if(mAllocated && mData != NULL)
  {
    free(mData);
    mAllocated = false;
    mData = NULL;
  }
}

FABRIC_EXT_EXPORT void FabricSECUREContainerDecode(
  SecureContainer & container,
  KL::Data data,
  KL::Size size,
  KL::String & key
) {
#ifndef NDEBUG
  printf("  { FabricSECURE } : FabricSECUREContainerDecode called.\n");
#endif
  if(container.localData == NULL)
  {
    container.localData = new SecureContainer::LocalData();
    container.localData->retain();
#ifndef NDEBUG
    printf("  { FabricSECURE } : SecureContainer created.\n");
#endif
    container.localData->decode(data,size,key);
#ifndef NDEBUG
    printf("  { FabricSECURE } : SecureContainer loaded.\n");
#endif
  }
#ifndef NDEBUG
  printf("  { FabricSECURE } : FabricSECUREContainerDecode completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricSECUREContainerEncode(
  SecureContainer & container,
  KL::VariableArray<KL::Byte> & data,
  KL::Integer level,
  KL::String & key
) {
#ifndef NDEBUG
  printf("  { FabricSECURE } : FabricSECUREContainerEncode called.\n");
#endif
  if(container.localData != NULL)
  {
    container.localData->encode(data,level,key);
  }
#ifndef NDEBUG
  printf("  { FabricSECURE } : FabricSECUREContainerEncode completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricSECUREContainerGetNbElements(
  SecureContainer & container,
  KL::Size & count
) {
#ifndef NDEBUG
  printf("  { FabricSECURE } : FabricSECUREContainerGetNbElements called.\n");
#endif
  if(container.localData != NULL)
    count = (KL::Size)container.localData->getNbElements();
  else
    count = 0;
#ifndef NDEBUG
  printf("  { FabricSECURE } : FabricSECUREContainerGetNbElements completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricSECUREContainerGetElement(
  SecureContainer & container,
  KL::Integer & index,
  SecureElement & element
) {
#ifndef NDEBUG
  printf("  { FabricSECURE } : FabricSECUREContainerGetElement called.\n");
#endif
  element.index = -1;
  if(container.localData != NULL)
  {
    if(index < (KL::Integer)container.localData->getNbElements())
      element = container.localData->getElementDescription(index);
  }
#ifndef NDEBUG
  printf("  { FabricSECURE } : FabricSECUREContainerGetElement completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricSECUREContainerGetElementData(
  SecureContainer & container,
  KL::Integer & index,
  KL::Data data,
  KL::Size size
) {
#ifndef NDEBUG
  printf("  { FabricSECURE } : FabricSECUREContainerGetElementData called.\n");
#endif
  if(container.localData != NULL && data != NULL)
  {
    if(index < (KL::Integer)container.localData->getNbElements())
    {
      void * buffer = NULL;
      container.localData->getElementData(index,&buffer,size);
      if(buffer != NULL)
        memcpy(data,buffer,size);
    }
  }
#ifndef NDEBUG
  printf("  { FabricSECURE } : FabricSECUREContainerGetElementData completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricSECUREContainerAddElement(
  SecureContainer & container,
  SecureElement & element,
  KL::Data data,
  KL::Size size
) {
#ifndef NDEBUG
  printf("  { FabricSECURE } : FabricSECUREContainerAddElement called.\n");
#endif
  if(container.localData == NULL)
  {
    container.localData = new SecureContainer::LocalData();
    container.localData->retain();
#ifndef NDEBUG
    printf("  { FabricSECURE } : SecureContainer created.\n");
#endif
  }
  element.index = container.localData->getNbElements();
  container.localData->addElement(element,data,size);
#ifndef NDEBUG
  printf("  { FabricSECURE } : FabricSECUREContainerAddElement completed.\n");
#endif
}

FABRIC_EXT_EXPORT void FabricSECUREContainerFree(
  SecureContainer & container
)
{
  if(container.localData != NULL) {
#ifndef NDEBUG
    printf("  { FabricSECURE } : FabricSECUREContainerFree called.\n");
#endif
    container.localData->release();
    container.localData = NULL;
#ifndef NDEBUG
    printf("  { FabricSECURE } : FabricSECUREContainerFree completed.\n");
#endif
  }
}
