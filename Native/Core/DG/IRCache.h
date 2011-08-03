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
    
      static RC::Handle<IRCache> Create()
      {
        return new IRCache;
      }
      
      std::string get( RC::ConstHandle<AST::GlobalList> const &ast ) const;
      void put( RC::ConstHandle<AST::GlobalList> const &ast, std::string const &ir );
      
    protected:
    
      IRCache();
      
      void subDirAndEntryFromSourceCode( RC::ConstHandle<AST::GlobalList> const &ast, RC::ConstHandle<IO::Dir> &subDir, std::string &entry ) const;
      
    private:
    
      RC::ConstHandle<IO::Dir> m_dir;
    };
  };
};

#endif //_FABRIC_DG_IR_CACHE_H
