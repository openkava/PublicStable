/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>
#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/Util/Bits.h>

#include <utility>
#include <limits>
#include <zlib.h>

#include "blowfish.h"

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

// struct containing a single element description
FABRIC_EXT_KL_STRUCT( SecureElement, {
  KL::Integer index;      // index inside the element list
  KL::String name;        // name of the element (ex. "UniformsDGNode.indices")
  KL::String type;        // type of the element (ex. "Vec3[]")
  KL::Integer slicecount; // number of slices of the element
  KL::Integer sliceindex; // slice index for this element (-1 if all slices)
  KL::Integer datacount;  // number of datas inside this element
  KL::Integer version;    // version number of this element
} );

#define baseOffset 16
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

// default encryption key
#define DEFAULT_KEY "0123456789ABCDEF"

// define version numbers for every change in the file format
// implementation, to ensure to stay backwards compatible
const int SECURECONTAINER_VERSION_FIRST = 100;
const int SECURECONTAINER_VERSION_CURRENT = SECURECONTAINER_VERSION_FIRST;

// struct storing all of the data
// the actual data is stored in a private class called LocalData.
// that way KL cannot access the contents of it.
FABRIC_EXT_KL_STRUCT( SecureContainer, {
  class LocalData : public Fabric::RC::Object {
  public:
  
    LocalData();

    // decode based on decryption key
    void decode(void * data, size_t size, const KL::String & key);
    
    // encode based on compression level + encryption key
    void encode(KL::VariableArray<KL::Byte> & data, KL::Integer level, const KL::String & key);
    
    // version of the file format
    unsigned int getVersion();
    
    // number of elements stored in this container
    unsigned int getNbElements();
    
    // access the element description for a given element index
    SecureElement getElementDescription(size_t index);
    
    // access the data for a given element index
    void getElementData(size_t index, void**data, size_t size);
    
    // add a new element to the container
    void addElement(SecureElement & element, void * data, size_t size);

    // Allocate more memory
    void resizeBuffer(size_t newSize);

  protected:
    
    // get the offset inside the container based on an offset type
    size_t getOffset(offsetType type, size_t index = 0);
    virtual ~LocalData();

    bool mAllocated;
    char * mData;
    size_t mSize;
    size_t mAllocatedSize;
  };

  LocalData * localData;
} );

// constructor, initiate as empty
SecureContainer::LocalData::LocalData() {
  mAllocated = false;
  mData = NULL;
  mSize = 0;
  mAllocatedSize = 0;
}

// encode based on compression level and encryption key
void SecureContainer::LocalData::encode(KL::VariableArray<KL::Byte> & data, KL::Integer level, const KL::String & key)
{
  if(mData == NULL || mSize == 0)
    return;
  
#ifndef NDEBUG
  printf("  { FabricSECURE } : Uncompressed size: %d.\n",(int)mSize);
#endif

  // add 24 bytes at the front (for two unsigned int + 16 chars)
  data.resize(24+mSize);
  
  // call the zlib compress2 macro
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
  // and store it into the encoded byte array
  KL::String key16 = DEFAULT_KEY;
  memcpy((char*)key16.data(),key.data(),key.length() > 16 ? 16 : key.length());
  memcpy(&data[8],key16.data(),16);

  // ensure the size to be multiple of 8
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

    // use the blowfish algorithm to encore part of the
    // data buffer in place, leaving the initial sizes alone.
    BlowFish::CBlowFish fish((unsigned char*)key16.data(),16);
    fish.Encrypt((unsigned char*)&data[8],cryptSize);
    
    
#ifndef NDEBUG
    printf("  { FabricSECURE } : Encrypted in place.\n");
#endif
  }
}

// decode the buffer using a decryption key
void SecureContainer::LocalData::decode(void * data, size_t size, const KL::String & key)
{
  if(size == 0 || data == NULL)
    return;
  if(mAllocated)
    free(mData);

#ifndef NDEBUG
  printf("  { FabricSECURE } : Uncompressing data of size %d.\n",(int)size);
#endif
    
  // access the first two unsigned ints to determine the
  // uncompressed and compressed sizes.
  mSize = ((unsigned int*)data)[0];
  size_t compressedSize = ((unsigned int*)data)[1];
  
  // copy the key out of the stream
  KL::String existingKey = DEFAULT_KEY;
  memcpy((char*)existingKey.data(),&((char*)data)[8],16);

#ifndef NDEBUG
  printf("  { FabricSECURE } : Original compressed size %d.\n",(int)compressedSize);
  printf("  { FabricSECURE } : Original uncompressed size %d.\n",(int)mSize);
#endif

  mData = (char*)malloc(mSize);
  mAllocated = true;
  
  // construct the decryption key (max length 16)
  KL::String key16 = DEFAULT_KEY;
  memcpy((char*)key16.data(),key.data(),key.length() > 16 ? 16 : key.length());

  // compare the decryption key with the one in the file  
  std::string stdKey16(key16.data());
  if(std::string(existingKey.data()) != stdKey16 && std::string(existingKey.data()) != DEFAULT_KEY)
  {
#ifndef NDEBUG
    printf("  { FabricSECURE } : Decription key: '%s'.\n",key16.data());
#endif

    // since the keys don't match, we need to decrypt this
    // use the blowfish algorithm to decrypt the buffer in place
    BlowFish::CBlowFish fish((unsigned char*)key16.data(),16);
    fish.Decrypt(&((unsigned char*)data)[8],size-8);
    
#ifndef NDEBUG
    printf("  { FabricSECURE } : Decrypted in place.\n");
#endif
    
    // get the key another time from the buffer
    memcpy((char*)existingKey.data(),&((char*)data)[8],16);
    
    // if the keys don't match after decryption, the
    // provided key is simply wrong
    if(std::string(existingKey.data()) != std::string(key16.data()))
    {
      printf("  { FabricSECURE } : ERROR: Incorrect key '%s' !\n",key16.data());
      if(mData != NULL)
        free(mData);
      mSize = 0;
      return;
    }
  }
  
  // use zlib decompression
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

// compute the offset of a certain piece of data inside the binary buffer
size_t SecureContainer::LocalData::getOffset(offsetType type, size_t index) {
  if(mData == NULL)
    return 0;
  switch(type)
  {
    // first unsigned int storing version number
    case offsetType_version:
      return 0;
    // second unsigned int storing nb of elements
    case offsetType_nbElements:
      return 4;
    // element description's name
    case offsetType_elementName:
    {
      // get the first element's offset
      size_t result = getOffset(offsetType_nbElements) + 4;
      
      // if we didn't ask for the first one,
      // skip through the file
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
    // element description's type
    case offsetType_elementType:
    {
      // get the offset of the that element's name
      size_t result = getOffset(offsetType_elementName,index);
      // skip the name in the buffer
      unsigned int nameLength = ((unsigned int*)(&mData[result]))[0];
      result += 4 + nameLength;
      return result;
    }
    // element description's slicecount
    case offsetType_elementSliceCount:
    {
      // get the offset of that element's type
      size_t result = getOffset(offsetType_elementType,index);
      // skip the type in the buffer
      unsigned int typeLength = ((unsigned int*)(&mData[result]))[0];
      result += 4 + typeLength;
      return result;
    }
    // element description's sliceindex
    case offsetType_elementSliceIndex:
    {
      // get the offset for that element's slicecount and skip 4 bytes
      return getOffset(offsetType_elementSliceCount,index) + 4;
    }
    // element description's datacount
    case offsetType_elementDataCount:
    {
      // get the offset for that element's sliceindex and skip 4 bytes
      return getOffset(offsetType_elementSliceIndex,index) + 4;
    }
    // element description's version
    case offsetType_elementVersion:
    {
      // get the offset for that element's datacount and skip 4 bytes
      return getOffset(offsetType_elementDataCount,index) + 4;
    }
    // element's data
    case offsetType_elementData:
    {
      // get the offset for that element's version and skip 4 bytes
      return getOffset(offsetType_elementVersion,index) + 4;
    }
    default:
      return 0;
  }
}

// access the version number from the buffer
unsigned int SecureContainer::LocalData::getVersion()
{
  if(!mAllocated || mData == NULL)
    return 0;
  size_t offset = getOffset(offsetType_version);
  return ((unsigned int*)(&mData[offset]))[0];
}

// access the nb of elements from the buffer
unsigned int SecureContainer::LocalData::getNbElements()
{
  if(!mAllocated || mData == NULL)
    return 0;
  size_t offset = getOffset(offsetType_nbElements);
  return ((unsigned int*)(&mData[offset]))[0];
}

// access an element description from the buffer
SecureElement SecureContainer::LocalData::getElementDescription(size_t index)
{
  SecureElement element;
  if(!mAllocated || mData == NULL)
    return element;
  element.index = index;
  
  // get the offset for the particular element
  size_t offset = getOffset(offsetType_elementName,index);
  
  // get the name (stored as straight chars ending in \0)
  element.name = &mData[offset+4]; // + length (int)
  // skip the unsigned int storing the length and each char in the name
  offset += 4 + (size_t)((unsigned int*)(&mData[offset]))[0];
  // get the type(stored as straight chars ending in \0)
  element.type = &mData[offset+4]; // + length (int)
  // skip the unsigned int storing the length and each char in the type
  offset += 4 + (size_t)((unsigned int*)(&mData[offset]))[0];
  // get the unsigned int storing the slicecount
  element.slicecount = ((unsigned int*)(&mData[offset]))[0];
  offset += 4;
  // get the unsigned int storing the sliceindex
  element.sliceindex = ((unsigned int*)(&mData[offset]))[0];
  offset += 4;
  // get the unsigned int storing the datacount
  element.datacount = ((unsigned int*)(&mData[offset]))[0];
  offset += 4;
  // get the unsigned int storing the version
  element.version = ((unsigned int*)(&mData[offset]))[0];
  return element;
}

// access an element's data buffer
void SecureContainer::LocalData::getElementData(size_t index, void**data, size_t size)
{
  if(!mAllocated || mData == NULL)
    return;
  
  // get the offset for that particular element
  size_t offset = getOffset(offsetType_elementData,index);
  
  // in the first 4 bytes of the element's buffer, we store the
  // buffer's size. get it and ensure it matches the expected size.
  size_t storedSize = (size_t)((unsigned int*)(&mData[offset]))[0];
  if(storedSize != size)
  {
    SecureElement element = getElementDescription(index);
    printf("{  FabricSECURE  }: ERROR: Element (%d)'%s': Size (%d) does not match one in buffer (%d).\n",(int)index,element.name.data(),(int)size,(int)storedSize);
    *data = NULL;
    return;
  }
  
  // set the output pointer to the element's data buffer
  *data = &mData[offset+4];
}

// add a new element to the SecureContainer
void SecureContainer::LocalData::addElement(SecureElement & element, void * data, size_t size)
{
#ifndef NDEBUG
  printf("  { FabricSECURE } : New Element (%d)'%s', type '%s', slice %d, size %d.\n",(int)getNbElements(),element.name.data(),element.type.data(),(int)element.sliceindex,(int)size);
#endif

  // get the number of previous elements
  unsigned int nbElements = getNbElements();
  
  // compute the size to allocate
  size_t oldSize = mSize;
  size_t newSize = mSize;
  if(mSize == 0)
    newSize = 8; // version + nbElements (2 x unsigned int)
  newSize += 4 + element.name.length() + 1; // length + name + \0
  newSize += 4 + element.type.length() + 1; // length + type + \0
  newSize += 4; // elementslicecount (unsigned int)
  newSize += 4; // elementslice (unsigned int)
  newSize += 4; // elementcount (unsigned int)
  newSize += 4; // elementversion (unsigned int)
  newSize += 4 + size; //  (unsigned int) + n bytes
  
  // allocate a new buffer
  resizeBuffer(newSize);
  
  // store the current fileformat version
  ((unsigned int*)&mData[0])[0] = SECURECONTAINER_VERSION_CURRENT;
  
  // store the number of elements + 1
  ((unsigned int*)&mData[4])[0] = nbElements + 1;
  
  // get the offset for the new element
  size_t offset = oldSize == 0 ? 8 : oldSize;
  
  // store the length of the element's name
  ((unsigned int*)&mData[offset])[0] = element.name.length() + 1;
  offset += 4;
  // store the element's name
  memcpy(&mData[offset],element.name.data(),element.name.length());
  offset += element.name.length();
  // store a string end character
  mData[offset] = '\0';
  offset += 1;
  // store the length of the element's type
  ((unsigned int*)&mData[offset])[0] = element.type.length() + 1;
  offset += 4;
  // store the element's type
  memcpy(&mData[offset],element.type.data(),element.type.length());
  offset += element.type.length();
  // store a string end character
  mData[offset] = '\0';
  offset += 1;
  // store the element's slicecount
  ((unsigned int*)&mData[offset])[0] = (unsigned int)element.slicecount;
  offset += 4;
  // store the element's sliceindex
  ((unsigned int*)&mData[offset])[0] = (unsigned int)element.sliceindex;
  offset += 4;
  // store the element's datacount
  ((unsigned int*)&mData[offset])[0] = (unsigned int)element.datacount;
  offset += 4;
  // store the element's version
  ((unsigned int*)&mData[offset])[0] = (unsigned int)element.version;
  offset += 4;
  // store the element's data buffer size
  ((unsigned int*)&mData[offset])[0] = size;
  offset += 4;
  // store the element's data buffer
  memcpy(&mData[offset],data,size);
}

// Allocate more memory
void SecureContainer::LocalData::resizeBuffer(size_t newSize) {
  if(newSize > mAllocatedSize)
  {
    mAllocatedSize = std::max( size_t(15), Fabric::Util::nextPowerOfTwoMinusOne( newSize ) );
    mData = (char*)realloc( mData, mAllocatedSize );
    mAllocated = true;
  }
  mSize = newSize;
}


// deconstructor, deallocate if necessary
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
