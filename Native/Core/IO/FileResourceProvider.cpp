/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "FileResourceProvider.h"
#include "Helpers.h"
#include <string.h>
#include <memory.h>

#if defined(FABRIC_POSIX)
# include <errno.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <aio.h>
#elif defined(FABRIC_WIN32)
# include <windows.h>
#endif 

namespace Fabric
{
  namespace IO
  {
    RC::Handle<FileResourceProvider> FileResourceProvider::Create()
    {
      return new FileResourceProvider();
    }

    FileResourceProvider::FileResourceProvider()
    {
    }

    char const* FileResourceProvider::getUrlScheme() const
    {
      return "file";
    }

    const int MaxFileReadBufferSize = 1<<19;//512K

    struct FileStream
    {
#if defined(FABRIC_POSIX)
      FileStream() : m_fileHandle( -1 ) {}
      ~FileStream()
      {
        if( m_fileHandle >= 0 )
          close( m_fileHandle );
      }

      aiocb m_readParams;//Note: our logic assumes this is the 1st member
      int m_fileHandle;

#elif defined(FABRIC_WIN32)
      FileStream() : m_fileHandle( INVALID_HANDLE_VALUE ) {}
      ~FileStream()
      {
        if( m_fileHandle != INVALID_HANDLE_VALUE )
          CloseHandle( m_fileHandle );
      }

      OVERLAPPED m_readParams;//Note: our logic assumes this is the 1st member
      HANDLE m_fileHandle;

#endif
      size_t m_totalSize, m_nbRead;
      std::string m_mimeType;
      std::vector<char> m_readBuffer;
      void* m_userData;
    };

    void ReadNextData( FileStream* fileStream );

    void CommonFileReadCallback( FileStream* fileStream, bool failed, size_t nbRead, size_t offset )
    {
      if( failed )
      {
        ResourceManager::onFailure( "File read operation failed", streamStruct->m_userData );//Don't put filename as it might be private
        delete fileStream;
        return;
      }
      if( nbRead )
      {
        ResourceManager::onData( offset, nbRead, &streamStruct->m_readBuffer.front(), streamStruct->m_userData );
        FABRIC_ASSERT( offset == streamStruct->m_nbRead );
        streamStruct->m_nbRead += nbRead;
      }
      ResourceManager::onProgress( fileStream->m_mimeType.c_str(), fileStream->m_nbRead, fileStream->m_totalSize, fileStream->m_userData );
      if( fileStream->m_nbRead < fileStream->totalSize )
        ReadNextData( fileStream );
      else
        delete fileStream;
    }

#if defined(FABRIC_POSIX)

    void FileReadCallback( sigval_t sigval )
    {
      FileStream* fileStream = (FileStream*)sigval.sival_ptr;
      size_t nbRead = 0;
      bool failed = true;
      if (aio_error( fileStream->m_readParams ) == 0)
      {
        ssize_t res = aio_return( fileStream->m_readParams );
        if( res >= 0 )
        {
          failed = false;
          nbRead = (size_t)res;
        }
      }
      CommonFileReadCallback( fileStream, failed, nbRead, fileStream->m_readParams.aio_offset );
    }

#elif defined(FABRIC_WIN32)

    VOID CALLBACK FileReadCallback(
      DWORD dwErrorCode,
      DWORD dwNumberOfBytesTransfered,
      LPOVERLAPPED lpOverlapped)
    {
      FileStream* fileStream = (FileStream*)lpOverlapped;
      CommonFileReadCallback( fileStream, dwErrorCode != ERROR_SUCCESS, dwNumberOfBytesTransfered, lpOverlapped->Offset );
    }

#endif

    void ReadNextData( FileStream* fileStream )
    {
      FABRIC_ASSERT( fileStream->m_nbRead < fileStream->totalSize );

      memset( fileStream->m_readParams, 0, sizeof(fileStream->m_readParams) );
      size_t nbToRead = std::min( fileStream->m_readBuffer.size(), fileStream->totalSize - fileStream->m_nbRead );
      bool readFailed = false;

#if defined(FABRIC_POSIX)
      fileStream->m_readParams.aio_fildes = fileStream->m_fileHandle;
      fileStream->m_readParams.aio_buf = &(fileStream->m_readBuffer.front());
      fileStream->m_readParams.aio_nbytes = nbToRead;
      fileStream->m_readParams.aio_offset = fileStream->m_nbRead;

      fileStream->m_readParams.aio_sigevent.sigev_notify = SIGEV_THREAD;
      fileStream->m_readParams.aio_sigevent.notify_function = FileReadCallback;
      fileStream->m_readParams.aio_sigevent.notify_attributes = NULL;
      fileStream->m_readParams.aio_sigevent.sigev_value.sival_ptr = &(fileStream->m_readParams);

      readFailed = aio_read( &(fileStream->m_readParams) ) < 0;
#elif defined(FABRIC_WIN32)
      fileStream->m_readParams.Offset = (DWORD)fileStream->m_nbRead;
      readFailed = !ReadFileEx( fileStream->m_fileHandle, &(fileStream->m_readBuffer.front()), nbToRead, &(fileStream->m_readParams), FileReadCallback );
#else
# error "Unsupported platform"
#endif
      if( readFailed )
      {
        ResourceManager::onFailureAsynchThreadCall( "Unable to read file", fileStream->m_userData );//Don't put filename as it might be private. Note: schedule asynch since the 1st call is synchronous
        delete fileStream;
      }
    }

    void FileResourceProvider::get( char const *url, bool getAsFile, void* userData )
    {
      if( strncmp( "file:///", url, 8 ) != 0 )
          throw Exception( "Error: URL not properly formatted for local files" );//Don't put filename as it might be private

      std::string fileWithPath = ChangeSeparatorsURLToFile( std::string( url+8 ) );
      if( !FileExists( fileWithPath ) )
          throw Exception( "Error: file doesn't exist" );//Don't put filename as it might be private

      std::string extension = GetExtension( fileWithPath );
      size_t fileSize = GetFileSize( fileWithPath );

      if( getAsFile )
      {
        //To avoid client reentrancy problems, schedule a asynch callbacks
        ResourceManager::onFileAsynchThreadCall( fileWithPath.c_str(), userData );
        ResourceManager::onProgressAsynchThreadCall( extension.c_str(), fileSize, fileSize, userData );
      }
      else if( fileSize == 0 )
        ResourceManager::onProgressAsynchThreadCall( extension.c_str(), fileSize, fileSize, userData );
      else
      {
        FileStream *fileStream = new FileStream();
        fileStream->m_nbRead = 0;
        fileStream->m_totalSize = fileSize;
        fileStream->m_mimeType = extension;
        fileStream->m_readBuffer.resize( std::min( MaxFileReadBufferSize, fileSize ) );

        bool openFailed = false;
#if defined(FABRIC_POSIX)
        fileStream->m_fileHandle = open( fileWithPath.c_str(), O_RDONLY );
        openFailed = fileStream->m_fileHandle < 0;
#elif defined(FABRIC_WIN32)
        fileStream->m_fileHandle = CreateFile( fileWithPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_SEQUENTIAL_SCAN, NULL ); 
        openFailed = fileStream->m_fileHandle < INVALID_HANDLE_VALUE;
#else
# error "Unsupported platform"
#endif
        if( openFailed )
        {
          delete fileStream;
          throw Exception("Unable to open file");//Don't put filename as it might be private
        }
        ReadNextData( fileStream );
      }
    }

  };
