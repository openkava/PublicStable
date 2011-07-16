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
    class StructDecl;
    class StructDeclMemberList;
    class StructDeclMember;
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
    
      class EarlyEOI
      {
      public:
      
        EarlyEOI( Token const &token )
          : m_cgLocation( token.getStartLocation() )
        {
        }
        
        CG::Location const &getCGLocation() const
        {
          return m_cgLocation;
        }
        
      private:
      
        CG::Location m_cgLocation;
      };

      Parser( RC::Handle<Scanner> const &scanner, CG::Diagnostics &diagnostics );
      
      RC::Handle<AST::GlobalList> parseGlobalList();
      RC::Handle<AST::Alias> parseAlias();
      RC::Handle<AST::StructDecl> parseStruct();
      RC::Handle<AST::StructDeclMemberList> parseStructMemberList();
      RC::Handle<AST::StructDeclMember> parseStructMember();
      
      Token::Type peek();
      Token next();
      Location const &getLocation();
      
      Token::Type expect( size_t count, Token::Type const *tokenTypes, char const *desc );

      Token consume( Token::Type tokenType, char const *desc );
      Token consume( Token::Type tokenType )
      {
        return consume( tokenType, 0 );
      }
      
      void handleError( Error const &error, Token::Type skipTokenType );
      void handleEarlyEOI( EarlyEOI const &earlyEOI );

    private:
    
      RC::Handle<Scanner> m_scanner;
      CG::Diagnostics &m_diagnostics;
      
      bool m_havePeek;
      Token m_peek;
    };
  };
};

#endif //_FABRIC_KL_PARSER_H
