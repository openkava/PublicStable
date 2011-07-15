/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KL/Parser.h>
#include <Fabric/Core/KL/Scanner.h>
#include <Fabric/Core/AST/GlobalList.h>

namespace Fabric
{
  namespace KL
  {
    RC::Handle<Parser> Parser::Create( RC::Handle<Scanner> const &scanner )
    {
      return new Parser( scanner );
    }
      
    Parser::Parser( RC::Handle<Scanner> const &scanner )
      : m_scanner( scanner )
    {
    }
    
    RC::Handle<AST::GlobalList> Parser::run()
    {
      Token token = m_scanner->nextToken();
      return AST::GlobalList::Create( token.getStartLocation() );
    }
  };
};
