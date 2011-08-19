/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_CODE_MANAGER_H
#define _FABRIC_DG_CODE_MANAGER_H

#include <Fabric/Core/DG/Code.h>
#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Base/RC/WeakConstHandle.h>
#include <Fabric/Core/Util/UnorderedMap.h>

namespace Fabric
{
  namespace AST
  {
    class GlobalList;
  };
  
  namespace DG
  {
    class Context;
    
    class CodeManager : public RC::Object
    {
      typedef Util::UnorderedMap< std::string, RC::WeakConstHandle<Code> > SourceCodeToCodeMap;
      
    public:
    
      static RC::Handle<CodeManager> Create()
      {
        return new CodeManager;
      }
      
      RC::ConstHandle<Code> compileSourceCode( RC::ConstHandle<Context> const &context, std::string const &sourceCode );
      
    protected:
    
      CodeManager();
      
    private:
    
      SourceCodeToCodeMap m_sourceCodeToCodeMap;
    };
  };
};

#endif //_FABRIC_DG_CODE_MANAGER_H
