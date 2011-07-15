/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_PARSER_H
#define _FABRIC_KL_PARSER_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>

namespace Fabric
{
  namespace AST
  {
    class GlobalList;
  };
  
  namespace KL
  {
    class Scanner;
    
    class Parser : public RC::Object
    {
    public:
    
      static RC::Handle<Parser> Create( RC::Handle<Scanner> const &scanner );
      
      RC::Handle<AST::GlobalList> run();

    protected:
    
      Parser( RC::Handle<Scanner> const &scanner );
      
    private:
    
      RC::Handle<Scanner> m_scanner;
    };
  };
};

#endif //_FABRIC_KL_PARSER_H
