/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KL/Parser.h>
#include <Fabric/Core/KL/Scanner.h>
#include <Fabric/Core/AST/Alias.h>
#include <Fabric/Core/AST/GlobalList.h>
#include <Fabric/Core/AST/StructDecl.h>
#include <Fabric/Core/AST/StructDeclMember.h>
#include <Fabric/Core/AST/StructDeclMemberList.h>

namespace Fabric
{
  namespace KL
  {
    RC::Handle<Parser> Parser::Create( RC::Handle<Scanner> const &scanner, CG::Diagnostics &diagnostics )
    {
      return new Parser( scanner, diagnostics );
    }
      
    Parser::Parser( RC::Handle<Scanner> const &scanner, CG::Diagnostics &diagnostics )
      : m_scanner( scanner )
      , m_diagnostics( diagnostics )
      , m_havePeek( false )
    {
    }
    
    RC::Handle<AST::GlobalList> Parser::run()
    {
      return parseGlobalList();
    }

    Token::Type Parser::peek()
    {
      if ( !m_havePeek )
      {
        for (;;)
        {
          m_peek = m_scanner->nextToken();
          if ( m_peek.getType() != Token::TK_WHITESPACE
            && m_peek.getType() != Token::TK_COMMENT_LINE
            && m_peek.getType() != Token::TK_COMMENT_BLOCK )
            break;
        }
        m_havePeek = true;
      }
      return m_peek.getType();
    }
    
    Token Parser::next()
    {
      if ( !m_havePeek )
        peek();
      Token result = m_peek;
      m_havePeek = false;
      return result;
    }
    
    Token Parser::consume( Token::Type tokenType, char const *desc )
    {
      expect( 1, &tokenType, desc );
      return next();
    }
    
    Token::Type Parser::expect( size_t count, Token::Type const *tokenTypes, char const *desc )
    {
      Token::Type nextTokenType = peek();
      for ( size_t i=0; i<count; ++i )
      {
        if ( nextTokenType == tokenTypes[i] )
          return nextTokenType;
      }
      if ( nextTokenType == Token::TK_EOI )
        throw EarlyEOI( m_peek );
      else throw Error( m_peek, "expecting " + std::string(desc) );
    }
    
    bool Parser::accept( Token::Type tokenType )
    {
      peek();
      return m_peek.getType() == tokenType;
    }
    
    void Parser::handleError( Error const &error, Token::Type skipTokenType )
    {
      m_diagnostics.addError( error.getCGLocation(), error.getDesc() );
      for (;;)
      {
        Token token = next();
        if ( token.getType() == skipTokenType
          || token.getType() == Token::TK_EOI )
          break;
      }
    }
    
    void Parser::handleEarlyEOI( EarlyEOI const &earlyEOI )
    {
      m_diagnostics.addError( earlyEOI.getCGLocation(), "unexpected end of input" );
    }
    
    Location const &Parser::getLocation()
    {
      peek();
      return m_peek.getStartLocation();
    }

    RC::Handle<AST::GlobalList> Parser::parseGlobalList()
    {
      RC::Handle<AST::GlobalList> result = AST::GlobalList::Create( getLocation() );
      bool done;
      while ( !done )
      {
        try
        {
          static Token::Type tokenTypes[3] =
          {
            Token::TK_ALIAS,
            Token::TK_STRUCT,
            Token::TK_EOI
          };
          Token::Type tokenType = expect( 3, tokenTypes, "'alias' or 'struct'" );
          switch ( tokenType )
          {
            case Token::TK_ALIAS:
              result->append( parseAlias() );
              break;
            case Token::TK_STRUCT:
              result->append( parseStruct() );
              break;
            case Token::TK_EOI:
              done = true;
              break;
          }
        }
        catch ( Error error )
        {
          handleError( error, Token::TK_SEMICOLON );
        }
        catch ( EarlyEOI earlyEOI )
        {
          handleEarlyEOI( earlyEOI );
        }
      }
      return result;
    }
    
    RC::Handle<AST::Alias> Parser::parseAlias()
    {
      Token aliasToken = consume( Token::TK_ALIAS );
      Token oldTypeNameToken = consume( Token::TK_TYPE_OR_IDENTIFIER, "type name" );
      Token newTypeNameToken = consume( Token::TK_TYPE_OR_IDENTIFIER, "identifier" );
      std::string arrayModifier = parseArrayModifier();
      consume( Token::TK_SEMICOLON, "';'" );
      return AST::Alias::Create( aliasToken.getStartLocation(), newTypeNameToken.toString(), oldTypeNameToken.toString() + arrayModifier );
    }
    
    RC::Handle<AST::StructDecl> Parser::parseStruct()
    {
      Token structToken = consume( Token::TK_STRUCT );
      Token nameToken = consume( Token::TK_TYPE_OR_IDENTIFIER, "identifier" );
      consume( Token::TK_LBRACE, "'{'" );
      RC::Handle<AST::StructDeclMemberList> memberList = parseStructMemberList();
      consume( Token::TK_RBRACE, "'}'" );
      consume( Token::TK_SEMICOLON, "';'" );
      return AST::StructDecl::Create( structToken.getStartLocation(), nameToken.toString(), memberList );
    }
    
    RC::Handle<AST::StructDeclMemberList> Parser::parseStructMemberList()
    {
      RC::Handle<AST::StructDeclMemberList> result = AST::StructDeclMemberList::Create( getLocation() );
      bool done = false;
      while ( !done )
      {
        try
        {
          static Token::Type tokenTypes[2] =
          {
            Token::TK_TYPE_OR_IDENTIFIER,
            Token::TK_RBRACE
          };
          switch ( expect( 2, tokenTypes, "identifier or '}'" ) )
          {
            case Token::TK_TYPE_OR_IDENTIFIER:
              result->append( parseStructMember() );
              break;
            case Token::TK_RBRACE:
              done = true;
              break;
          }
        }
        catch ( Error error )
        {
          handleError( error, Token::TK_RBRACE );
        }
      }
      return result;
    }
    
    RC::Handle<AST::StructDeclMember> Parser::parseStructMember()
    {
      Token typeToken = consume( Token::TK_TYPE_OR_IDENTIFIER, "type name" );
      Token nameToken = consume( Token::TK_TYPE_OR_IDENTIFIER, "identifier" );
      std::string arrayModifier = parseArrayModifier();
      consume( Token::TK_SEMICOLON, "';'" );
      return AST::StructDeclMember::Create( typeToken.getStartLocation(), nameToken.toString(), typeToken.toString() + arrayModifier );
    }

    std::string Parser::parseArrayModifier()
    {
      std::string result = "";
      if ( accept( Token::TK_LBRACKET ) )
      {
        consume( Token::TK_LBRACKET );
        static Token::Type tokenTypes[2] =
        {
          Token::TK_CONST_UI,
          Token::TK_RBRACKET
        };
        Token::Type tokenType = expect( 2, tokenTypes, "']' or unsigned integer constant" );
        switch ( tokenType )
        {
          case Token::TK_CONST_UI:
          {
            Token token = consume( Token::TK_CONST_UI );
            result += "[" + token.toString() + "]";
          }
          break;
          
          case Token::TK_RBRACKET:
            result += "[]";
            break;
        }
        consume( Token::TK_RBRACKET );
        result = parseArrayModifier() + result;
      }
      return result;
    }
  };
};
