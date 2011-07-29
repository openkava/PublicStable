/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/IRCache.h>
#include <Fabric/Core/IO/Dir.h>
#include <Fabric/Core/Util/MD5.h>
#include <Fabric/Core/AST/GlobalList.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Base/JSON/Encode.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/Log.h>
#include <Fabric/Core/Util/Timer.h>
#include <Fabric/Core/Build.h>

namespace Fabric
{
  
  
  namespace DG
  {
    IRCache::IRCache()
    {
      RC::ConstHandle<IO::Dir> rootDir = IO::Dir::Private();
      m_dir = IO::Dir::Create( IO::Dir::Create( rootDir, "IRCache" ), _(buildCacheGeneration) );
    }
    
    void IRCache::subDirAndEntryFromSourceCode( RC::ConstHandle<AST::GlobalList> const &ast, RC::ConstHandle<IO::Dir> &subDir, std::string &entry ) const
    {
      Util::Timer timer;
      Util::SimpleString astJSONString = ast->toJSON();
      FABRIC_LOG( "ast->toJSON(): %fms", timer.getElapsedMS(true) );
      std::string prefixedSourceCodeMD5HexDigest = Util::md5HexDigest( astJSONString.getData(), astJSONString.getLength() );
      FABRIC_LOG( "Util::md5HexDigest( astEncodedJSONValue ): %fms", timer.getElapsedMS(true) );
      subDir = IO::Dir::Create( m_dir, prefixedSourceCodeMD5HexDigest.substr( 0, 2 ) );
      entry = prefixedSourceCodeMD5HexDigest.substr( 2, 30 );
    }
    
    std::string IRCache::get( RC::ConstHandle<AST::GlobalList> const &ast ) const
    {
      RC::ConstHandle<IO::Dir> subDir;
      std::string entry;
      subDirAndEntryFromSourceCode( ast, subDir, entry );
      
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
    
    void IRCache::put( RC::ConstHandle<AST::GlobalList> const &ast, std::string const &ir )
    {
      RC::ConstHandle<IO::Dir> subDir;
      std::string entry;
      subDirAndEntryFromSourceCode( ast, subDir, entry );
      
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
