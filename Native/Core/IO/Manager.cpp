/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Base/Exception.h>
#include <fstream>

namespace Fabric
{
  namespace IO
  {
    void Manager::writeDataAtUserLocation(
      size_t size,
      const void* data,
      std::string const &defaultFilename,
      std::string const &extension
      ) const
    {
      std::string defaultFilenameWithExt( defaultFilename );

      if( !defaultFilenameWithExt.empty() && !extension.empty() )
      {
        size_t extPos = defaultFilenameWithExt.rfind( extension );
        if( !(extPos == defaultFilenameWithExt.size() - extension.size() && extPos > 0 && defaultFilenameWithExt[ extPos-1 ] == L'.') )
          defaultFilenameWithExt += '.' + extension;
      }

      std::string filePath = getUserFilePath( defaultFilenameWithExt, extension );

      std::ofstream file( filePath.c_str(), std::ios::out | std::ios::trunc | std::ios::binary );
      if( !file.is_open() )
        throw Exception( "Unable to create file " + defaultFilename );

      file.write( (const char*)data, size );
      if( file.bad() )
        throw Exception( "Error while writing to file " + defaultFilename );
    }
  };
};
