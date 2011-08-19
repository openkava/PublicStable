/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_IR_CACHE_H
#define _FABRIC_DG_IR_CACHE_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <string>

namespace Fabric
{
  namespace AST
  {
    class GlobalList;
  };
  
  namespace IO
  {
    class Dir;
  };
  
  namespace DG
  {
    class IRCache : public RC::Object
    {
    public:
    
      static RC::Handle<IRCache> Instance();
      
      std::string keyForAST( RC::ConstHandle<AST::GlobalVector> const &ast ) const;
      
      std::string get( std::string const &key ) const;
      void put( std::string const &key, std::string const &ir ) const;
      
    protected:
    
      IRCache();
      
      void subDirAndEntryFromKey( std::string const &key, RC::ConstHandle<IO::Dir> &subDir, std::string &entry ) const;
      
    private:
    
      RC::ConstHandle<IO::Dir> m_dir;
    };
  };
};

#endif //_FABRIC_DG_IR_CACHE_H
