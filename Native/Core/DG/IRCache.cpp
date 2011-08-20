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

#define FABRIC_IR_CACHE_EXPIRY_SEC (30*24*60*60)

namespace Fabric
{
  namespace DG
  {
    RC::Handle<IRCache> IRCache::Instance()
    {
      static RC::Handle<IRCache> instance;
      if ( !instance )
        instance = new IRCache();
      return instance;
    }
      
    IRCache::IRCache()
    {
      RC::ConstHandle<IO::Dir> rootDir = IO::Dir::Private();
      RC::ConstHandle<IO::Dir> baseDir = IO::Dir::Create( rootDir, "IRCache" );
      baseDir->recursiveDeleteFilesOlderThan( time(NULL) - FABRIC_IR_CACHE_EXPIRY_SEC );
      m_dir = IO::Dir::Create( baseDir, _(buildCacheGeneration) );
    }
    
    std::string IRCache::keyForAST( RC::ConstHandle<AST::GlobalList> const &ast ) const
    {
      Util::SimpleString astJSONString = ast->toJSON();
      return Util::md5HexDigest( astJSONString.getData(), astJSONString.getLength() );
    }
    
    void IRCache::subDirAndEntryFromKey( std::string const &key, RC::ConstHandle<IO::Dir> &subDir, std::string &entry ) const
    {
      subDir = IO::Dir::Create( m_dir, key.substr( 0, 2 ) );
      entry = key.substr( 2, 30 );
    }
    
    std::string IRCache::get( std::string const &key ) const
    {
      RC::ConstHandle<IO::Dir> subDir;
      std::string entry;
      subDirAndEntryFromKey( key, subDir, entry );
      
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
    
    void IRCache::put( std::string const &key, std::string const &ir ) const
    {
      RC::ConstHandle<IO::Dir> subDir;
      std::string entry;
      subDirAndEntryFromKey( key, subDir, entry );
      
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
