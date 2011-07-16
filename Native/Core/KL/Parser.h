/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_PARSER_H
#define _FABRIC_KL_PARSER_H

#include <Fabric/Core/KL/Token.h>
#include <Fabric/Core/CG/Diagnostics.h>

#include <list>

namespace Fabric
{
  namespace CG
  {
    class Diagnostics;
  };
  
  namespace AST
  {
    class GlobalList;
    class Alias;
  };
  
  namespace KL
  {
    class Scanner;
    
    class Parser : public RC::Object
    {
    public:
    
      static RC::Handle<Parser> Create( RC::Handle<Scanner> const &scanner, CG::Diagnostics &diagnostics );
      
      RC::Handle<AST::GlobalList> run();

    protected:
    
      class Error
      {
      public:
      
        Error( Token const &token, std::string const &msg )
          : m_cgLocation( token.getStartLocation() )
          , m_msg( msg )
        {
        }
        
        CG::Location const &getCGLocation() const
        {
          return m_cgLocation;
        }
        
        std::string getDesc() const
        {
          return m_msg;
        }
        
      private:
      
        CG::Location m_cgLocation;
        std::string m_msg;
      };
    
      Parser( RC::Handle<Scanner> const &scanner, CG::Diagnostics &diagnostics );
      
      RC::Handle<AST::GlobalList> parseGlobalList();
      RC::Handle<AST::Alias> parseAlias();
      
      Token::Type peek();
      Token next();
      
      Token::Type expect( size_t count, Token::Type const *tokenTypes, char const *desc );
      void expect( Token::Type tokenType1, char const *desc )
      {
        expect( 1, &tokenType1, desc );
      }
      Token::Type expect( Token::Type tokenType1, Token::Type tokenType2, char const *desc )
      {
        Token::Type tokenTypes[2] =
        {
          tokenType1,
          tokenType2
        };
        return expect( 2, tokenTypes, desc );
      }

      Token consume( Token::Type tokenType, char const *desc );
      Token consume( Token::Type tokenType )
      {
        return consume( tokenType, 0 );
      }
      
      void handleError( Error const &error, Token::Type skipTokenType );

    private:
    
      RC::Handle<Scanner> m_scanner;
      CG::Diagnostics m_diagnostics;
      
      bool m_havePeek;
      Token m_peek;
    };
  };
};

#endif //_FABRIC_KL_PARSER_H
