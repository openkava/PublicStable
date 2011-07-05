/*
 *
 *  Created by Peter Zion on 10-12-01.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "Scanner.h"
#include "Source.h"
#include "Location.h"
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/CG/Manager.h>

namespace Fabric
{
  
  
  namespace KL
  {
    bool Scanner::s_charAttribsInitialized = false;
    uint8_t Scanner::s_charAttribs[256];

    Scanner::Scanner( Source &source, RC::ConstHandle<CG::Manager> const &cgManager )
      : m_source( source )
      , m_cgManager( cgManager )
    {
      if ( !s_charAttribsInitialized )
      {
        memset( s_charAttribs, 0, 256 );
        
        for ( size_t ch='a'; ch<='z'; ++ch )
          s_charAttribs[ch] |= k_charAttribAlpha;
        for ( size_t ch='A'; ch<='Z'; ++ch )
          s_charAttribs[ch] |= k_charAttribAlpha;
        for ( size_t ch=128; ch<256; ++ch )
          s_charAttribs[ch] |= k_charAttribAlpha;        
        s_charAttribs['_'] |= k_charAttribAlpha;
        
        for ( size_t ch='0'; ch<='9'; ++ch )
          s_charAttribs[ch] |= k_charAttribDigit;
        
        for ( size_t ch='0'; ch<='9'; ++ch )
          s_charAttribs[ch] |= k_charAttribHexDigit;
        for ( size_t ch='a'; ch<='f'; ++ch )
          s_charAttribs[ch] |= k_charAttribHexDigit;
        for ( size_t ch='A'; ch<='F'; ++ch )
          s_charAttribs[ch] |= k_charAttribHexDigit;
          
        s_charAttribs[' '] |= k_charAttribSpace;
        s_charAttribs['\n'] |= k_charAttribSpace;
        s_charAttribs['\r'] |= k_charAttribSpace;
        s_charAttribs['\t'] |= k_charAttribSpace;
        s_charAttribs['\v'] |= k_charAttribSpace;
        
        s_charAttribs['~'] |= k_charAttribSymbol;
        s_charAttribs['`'] |= k_charAttribSymbol;
        s_charAttribs['!'] |= k_charAttribSymbol;
        s_charAttribs['@'] |= k_charAttribSymbol;
        s_charAttribs['#'] |= k_charAttribSymbol;
        s_charAttribs['$'] |= k_charAttribSymbol;
        s_charAttribs['%'] |= k_charAttribSymbol;
        s_charAttribs['^'] |= k_charAttribSymbol;
        s_charAttribs['&'] |= k_charAttribSymbol;
        s_charAttribs['*'] |= k_charAttribSymbol;
        s_charAttribs['('] |= k_charAttribSymbol;
        s_charAttribs[')'] |= k_charAttribSymbol;
        s_charAttribs['-'] |= k_charAttribSymbol;
        s_charAttribs['='] |= k_charAttribSymbol;
        s_charAttribs['+'] |= k_charAttribSymbol;
        s_charAttribs['|'] |= k_charAttribSymbol;
        s_charAttribs[']'] |= k_charAttribSymbol;
        s_charAttribs['}'] |= k_charAttribSymbol;
        s_charAttribs['['] |= k_charAttribSymbol;
        s_charAttribs['{'] |= k_charAttribSymbol;
        s_charAttribs[';'] |= k_charAttribSymbol;
        s_charAttribs[':'] |= k_charAttribSymbol;
        s_charAttribs['\''] |= k_charAttribSymbol;
        s_charAttribs['"'] |= k_charAttribSymbol;
        s_charAttribs['/'] |= k_charAttribSymbol;
        s_charAttribs['?'] |= k_charAttribSymbol;
        s_charAttribs['.'] |= k_charAttribSymbol;
        s_charAttribs['>'] |= k_charAttribSymbol;
        s_charAttribs[','] |= k_charAttribSymbol;
        s_charAttribs['<'] |= k_charAttribSymbol;
        
        s_charAttribsInitialized = true;
      }
    }
    
    Token Scanner::nextToken_Symbol( YYSTYPE *yys )
    {
      FABRIC_ASSERT( IsSymbol( m_source.peek() ) );
      std::string text;
      char ch = m_source.advance( &text );
      switch ( ch )
      {
        case '=':
          if ( m_source.peek() == '=' )
          {
            m_source.advance( &text );
            return TK_EQUALS_EQUALS;
          }
          else return TK_EQUALS;        
           
        case '+':
          if ( m_source.peek() == '=' )
          {
            m_source.advance( &text );
            return TK_PLUS_EQUALS;
          }
          else if ( m_source.peek() == '+' )
          {
            m_source.advance( &text );
            return TK_PLUS_PLUS;
          }
          else return TK_PLUS;        
        
        case '-':
          if ( m_source.peek() == '=' )
          {
            m_source.advance( &text );
            return TK_MINUS_EQUALS;
          }
          if ( m_source.peek() == '-' )
          {
            m_source.advance( &text );
            return TK_MINUS_MINUS;
          }
          else return TK_MINUS;
          
        case '*':
          if ( m_source.peek() == '=' )
          {
            m_source.advance( &text );
            return TK_AST_EQUALS;
          }
          else return TK_AST;
          
        case '/':
          if ( m_source.peek() == '=' )
          {
            m_source.advance( &text );
            return TK_FSLASH_EQUALS;
          }
          else if ( m_source.peek() == '*' )
          {
            m_source.advance( &text );
            while ( !m_source.isEOF() )
            {
              if ( m_source.advance( &text ) == '*' )
              {
                if ( m_source.peek() == '/' )
                {
                  m_source.advance( &text );
                  yys->valueStringPtr = new std::string( text );
                  return TK_COMMENT_BLOCK;
                }
              }
            }
            throw Exception("unclosed block comment");
          }
          else if ( m_source.peek() == '/' )
          {
            m_source.advance( &text );
            while ( m_source.peek() != '\n' )
              m_source.advance( &text );
            yys->valueStringPtr = new std::string( text );
            return TK_COMMENT_LINE;
          }
          else return TK_FSLASH;
          
        case '%':
          if ( m_source.peek() == '=' )
          {
            m_source.advance( &text );
            return TK_PERCENT_EQUALS;
          }
          else return TK_PERCENT;
          
        case '^':
          if ( m_source.peek() == '^' )
          {
            m_source.advance( &text );
            if ( m_source.peek() == '=' )
            {
              m_source.advance( &text );
              return TK_CARET_CARET_EQUALS;
            }
            else return TK_CARET_CARET;
          }
          else if ( m_source.peek() == '=' )
          {
            m_source.advance( &text );
            return TK_CARET_EQUALS;
          }
          else return TK_CARET;
          
        case '&':
          if ( m_source.peek() == '&' )
          {
            m_source.advance( &text );
            if ( m_source.peek() == '=' )
            {
              m_source.advance( &text );
              return TK_AMP_AMP_EQUALS;
            }
            else return TK_AMP_AMP;
          }
          else if ( m_source.peek() == '=' )
          {
            m_source.advance( &text );
            return TK_AMP_EQUALS;
          }
          else return TK_AMP;
          
        case '|':
          if ( m_source.peek() == '|' )
          {
            m_source.advance( &text );
            if ( m_source.peek() == '=' )
            {
              m_source.advance( &text );
              return TK_PIPE_PIPE_EQUALS;
            }
            else return TK_PIPE_PIPE;
          }
          else if ( m_source.peek() == '=' )
          {
            m_source.advance( &text );
            return TK_PIPE_EQUALS;
          }
          else return TK_PIPE;
          
        case '[':
          return TK_LBRACKET;

        case ']':
          return TK_RBRACKET;
        
        case '{':
          return TK_LBRACE;
          
        case '}':
          return TK_RBRACE;
        
        case '(':
          return TK_LPAREN;
          
        case ')':
          return TK_RPAREN;
        
        case '.':
          return TK_DOT;
        
        case ';':
          return TK_SEMICOLON;
          
        case '<':
          if ( m_source.peek() == '=' )
          {
            m_source.advance( &text );
            return TK_LANGLE_EQUALS;
          }
          else return TK_LANGLE;
          
        case '>':
          if ( m_source.peek() == '=' )
          {
            m_source.advance( &text );
            return TK_RANGLE_EQUALS;
          }
          else return TK_RANGLE;
        
        case '~':
          return TK_TILDE;
        
        case '!':
          if ( m_source.peek() == '=' )
          {
            m_source.advance( &text );
            return TK_EXCL_EQUALS;
          }
          else return TK_EXCL;
          
        case '\'':
          while ( !m_source.isEOF() )
          {
            char ch = m_source.advance( &text );
            if ( ch == '\\' )
            {
              if ( m_source.isEOF() )
                throw Exception("unterminated string constant");
              m_source.advance( &text );
            }
            else if ( ch == '\'' )
            {
              yys->valueStringPtr = new std::string( text );
              return TK_CONST_STRING_SQUOT;
            }
          }
          throw Exception("unterminated string constant");
          
        case '"':
          while ( !m_source.isEOF() )
          {
            char ch = m_source.advance( &text );
            if ( ch == '\\' )
            {
              if ( m_source.isEOF() )
                throw Exception("unterminated string constant");
              m_source.advance( &text );
            }
            else if ( ch == '"' )
            {
              yys->valueStringPtr = new std::string( text );
              return TK_CONST_STRING_DQUOT;
            }
          }
          throw Exception("unterminated string constant");
        
        case ',':
          return TK_COMMA;
          
        case '?':
          return TK_QUESTION_MARK;
        
        case ':':
          return TK_COLON;
          
        default:
          throw Exception("bad symbol");
      }
    }
    
    Token Scanner::nextToken_Digit( YYSTYPE *yys )
    {
      FABRIC_ASSERT( IsDigit( m_source.peek() ) );
      std::string text;
      
      m_source.advance( &text );
      
      Token token = TK_CONST_UI;
      
      if ( text[0] == '0' && tolower( m_source.peek() ) == 'x' )
      {
        m_source.advance( &text );
        if ( !IsHexDigit( m_source.peek() ) )
          throw Exception("malformed hexadecimal constant");
        while ( IsHexDigit( m_source.peek() ) )
          m_source.advance( &text );
      }
      else
      {
        while ( IsDigit( m_source.peek() ) )
          m_source.advance( &text );
        
        if ( m_source.peek() == '.' )
        {
          m_source.advance( &text );
          token = TK_CONST_FP;
          
          while ( IsDigit( m_source.peek() ) )
            m_source.advance( &text );
        }
        
        if ( m_source.peek() == 'e' || m_source.peek() == 'E' )
        {
          m_source.advance( &text );
          token = TK_CONST_FP;
          
          if ( m_source.peek() == '+' || m_source.peek() == '-' )
            m_source.advance( &text );
          
          if ( !IsDigit( m_source.peek() ) )
            throw Exception("malformed floating point exponent");
          while ( IsDigit( m_source.peek() ) )
            m_source.advance( &text );
        }
      }
      
      yys->valueStringPtr = new std::string( text );
      return token;
    }
    
    Token Scanner::nextToken_Space( YYSTYPE *yys )
    {
      FABRIC_ASSERT( IsSpace( m_source.peek() ) );
      std::string text;
      while ( IsSpace( m_source.peek() ) )
        m_source.advance( &text );
      yys->valueStringPtr = new std::string( text );
      return TK_WHITESPACE;
    }
    
    Token Scanner::nextToken_Alpha( YYSTYPE *yys )
    {
      FABRIC_ASSERT( IsAlpha( m_source.peek() ) );
      std::string text;

      m_source.advance( &text );
      
      while ( IsAlpha( m_source.peek() ) || IsDigit( m_source.peek() ) )
        m_source.advance( &text );
      
      switch( text.length() )
      {
        case 2:
          if ( text == "in" )
            return TK_IN;
          if ( text == "io" )
            return TK_IO;
          if ( text == "if" )
            return TK_IF;
          if ( text == "do" )
            return TK_DO;
          break;
          
        case 3:
          if ( text == "for" )
            return TK_FOR;
          if ( text == "var" )
            return TK_VAR;
          break;
          
        case 4:
          if ( text == "case" )
            return TK_CASE;
          if ( text == "else" )
            return TK_ELSE;
          if ( text == "true" )
            return TK_TRUE;
          break;
          
        case 5:
          if ( text == "alias" )
            return TK_ALIAS;
          if ( text == "break" )
            return TK_BREAK;
          if ( text == "const" )
            return TK_CONST;
          if ( text == "false" )
            return TK_FALSE;
          if ( text == "while" )
            return TK_WHILE;
          break;
        
        case 6:
          if ( text == "struct" )
            return TK_STRUCT;
          if ( text == "switch" )
            return TK_SWITCH;
          if ( text == "report" )
            return TK_REPORT;
          if ( text == "return" )
            return TK_RETURN;
          break;
        
        case 7:
          if ( text == "default" )
            return TK_DEFAULT;
          break;
        
        case 8:
          if ( text == "function" )
            return TK_FUNCTION;
          if ( text == "operator" )
            return TK_OPERATOR;
          if ( text == "continue" )
            return TK_CONTINUE;
          break;
        
        case 9:
          break;
      }
      
      RC::ConstHandle<RT::Desc> baseDesc = m_cgManager->getRTManager()->maybeGetBaseDesc( text );
      if ( baseDesc )
      {
        RC::ConstHandle<CG::Adapter> desc = m_cgManager->getAdapter( baseDesc );
        yys->cgAdapter = desc.ptr();
        yys->cgAdapter->retain();
        return TK_REGISTERED_TYPE;
      }
      
      yys->valueStringPtr = new std::string( text );
      return TK_IDENTIFIER;
    }
    
    Token Scanner::nextToken( YYSTYPE *yys )
    {
      int ch = m_source.peek();
      if ( ch == -1 )
        return TOKEN_END;
      if ( IsSpace(ch) )
        return nextToken_Space( yys );
      if ( IsAlpha(ch) )
        return nextToken_Alpha( yys );
      if ( IsDigit(ch) )
        return nextToken_Digit( yys );
      if ( IsSymbol(ch) )
        return nextToken_Symbol( yys );
      throw Exception("bad character");
    }

    RC::ConstHandle<CG::Manager> Scanner::cgManager() const
    {
      return m_cgManager;
    }
  };
};
