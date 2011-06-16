/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/IRCache.h>
#include <Fabric/Core/IO/Dir.h>
#include <Fabric/Core/Util/MD5.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Build.h>

namespace Fabric
{
  
  
  namespace DG
  {
    IRCache::IRCache()
    {
      RC::ConstHandle<IO::Dir> rootDir = IO::Dir::IRCache();
      RC::ConstHandle<IO::Dir> fabricInternalDir = IO::Dir::Create( rootDir, ".fabric-internal" );
      m_dir = IO::Dir::Create( fabricInternalDir, "irCache" );
    }
    
    void IRCache::subDirAndEntryFromSourceCode( std::string const &sourceCode, RC::ConstHandle<IO::Dir> &subDir, std::string &entry ) const
    {
      std::string prefixedSourceCode = buildVersion + sourceCode;
      std::string prefixedSourceCodeMD5HexDigest = Util::md5HexDigest( prefixedSourceCode.data(), prefixedSourceCode.length() );
      subDir = IO::Dir::Create( m_dir, prefixedSourceCodeMD5HexDigest.substr( 0, 2 ) );
      entry = prefixedSourceCodeMD5HexDigest.substr( 2, 30 );
    }
    
    std::string IRCache::get( std::string const &sourceCode ) const
    {
      RC::ConstHandle<IO::Dir> subDir;
      std::string entry;
      subDirAndEntryFromSourceCode( sourceCode, subDir, entry );
      
      std::string result;
      try
      {
        result = subDir->getFileContents( entry );
      }
      catch ( Exception e )
      {
      }
      return result;
    }
    
    void IRCache::put( std::string const &sourceCode, std::string const &ir )
    {
      RC::ConstHandle<IO::Dir> subDir;
      std::string entry;
      subDirAndEntryFromSourceCode( sourceCode, subDir, entry );
      
      try
      {
        subDir->putFileContents( entry, ir );
      }
      catch ( Exception e )
      {
        // [pzion 20110323] This is a cache; it's OK to fail to cache an entry.
      }
    }
  };
};
