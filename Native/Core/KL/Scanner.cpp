/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KL/Scanner.h>
#include <Fabric/Core/KL/Source.h>
#include <Fabric/Core/KL/Location.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/CG/Manager.h>

namespace Fabric
{
  namespace KL
  {
    bool Scanner::s_charAttribsInitialized = false;
    uint8_t Scanner::s_charAttribs[256];

    Scanner::Scanner( RC::ConstHandle<Source> const &source )
      : m_sourceReader( source )
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
    
    Token Scanner::nextToken_Symbol( Location const &startLocation )
    {
      FABRIC_ASSERT( IsSymbol( m_sourceReader.peek() ) );
      char ch = m_sourceReader.advance();
      switch ( ch )
      {
        case '=':
          if ( m_sourceReader.peek() == '=' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_EQUALS_EQUALS, startLocation );
          }
          else return createToken( Token::TK_EQUALS, startLocation );        
           
        case '+':
          if ( m_sourceReader.peek() == '=' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_PLUS_EQUALS, startLocation );
          }
          else if ( m_sourceReader.peek() == '+' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_PLUS_PLUS, startLocation );
          }
          else return createToken( Token::TK_PLUS, startLocation );        
        
        case '-':
          if ( m_sourceReader.peek() == '=' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_MINUS_EQUALS, startLocation );
          }
          if ( m_sourceReader.peek() == '-' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_MINUS_MINUS, startLocation );
          }
          else return createToken( Token::TK_MINUS, startLocation );
          
        case '*':
          if ( m_sourceReader.peek() == '=' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_AST_EQUALS, startLocation );
          }
          else return createToken( Token::TK_AST, startLocation );
          
        case '/':
          if ( m_sourceReader.peek() == '=' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_FSLASH_EQUALS, startLocation );
          }
          else if ( m_sourceReader.peek() == '*' )
          {
            m_sourceReader.advance();
            while ( !m_sourceReader.isEOF() )
            {
              if ( m_sourceReader.advance() == '*' )
              {
                if ( m_sourceReader.peek() == '/' )
                {
                  m_sourceReader.advance();
                  return createToken( Token::TK_COMMENT_BLOCK, startLocation );
                }
              }
            }
            throw Exception("unclosed block comment");
          }
          else if ( m_sourceReader.peek() == '/' )
          {
            m_sourceReader.advance();
            while ( m_sourceReader.peek() != '\n' )
              m_sourceReader.advance();
            return createToken( Token::TK_COMMENT_LINE, startLocation );
          }
          else return createToken( Token::TK_FSLASH, startLocation );
          
        case '%':
          if ( m_sourceReader.peek() == '=' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_PERCENT_EQUALS, startLocation );
          }
          else return createToken( Token::TK_PERCENT, startLocation );
          
        case '^':
          if ( m_sourceReader.peek() == '^' )
          {
            m_sourceReader.advance();
            if ( m_sourceReader.peek() == '=' )
            {
              m_sourceReader.advance();
              return createToken( Token::TK_CARET_CARET_EQUALS, startLocation );
            }
            else return createToken( Token::TK_CARET_CARET, startLocation );
          }
          else if ( m_sourceReader.peek() == '=' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_CARET_EQUALS, startLocation );
          }
          else return createToken( Token::TK_CARET, startLocation );
          
        case '&':
          if ( m_sourceReader.peek() == '&' )
          {
            m_sourceReader.advance();
            if ( m_sourceReader.peek() == '=' )
            {
              m_sourceReader.advance();
              return createToken( Token::TK_AMP_AMP_EQUALS, startLocation );
            }
            else return createToken( Token::TK_AMP_AMP, startLocation );
          }
          else if ( m_sourceReader.peek() == '=' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_AMP_EQUALS, startLocation );
          }
          else return createToken( Token::TK_AMP, startLocation );
          
        case '|':
          if ( m_sourceReader.peek() == '|' )
          {
            m_sourceReader.advance();
            if ( m_sourceReader.peek() == '=' )
            {
              m_sourceReader.advance();
              return createToken( Token::TK_PIPE_PIPE_EQUALS, startLocation );
            }
            else return createToken( Token::TK_PIPE_PIPE, startLocation );
          }
          else if ( m_sourceReader.peek() == '=' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_PIPE_EQUALS, startLocation );
          }
          else return createToken( Token::TK_PIPE, startLocation );
          
        case '[':
          return createToken( Token::TK_LBRACKET, startLocation );

        case ']':
          return createToken( Token::TK_RBRACKET, startLocation );
        
        case '{':
          return createToken( Token::TK_LBRACE, startLocation );
          
        case '}':
          return createToken( Token::TK_RBRACE, startLocation );
        
        case '(':
          return createToken( Token::TK_LPAREN, startLocation );
          
        case ')':
          return createToken( Token::TK_RPAREN, startLocation );
        
        case '.':
          return createToken( Token::TK_DOT, startLocation );
        
        case ';':
          return createToken( Token::TK_SEMICOLON, startLocation );
          
        case '<':
          if ( m_sourceReader.peek() == '=' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_LANGLE_EQUALS, startLocation );
          }
          else return createToken( Token::TK_LANGLE, startLocation );
          
        case '>':
          if ( m_sourceReader.peek() == '=' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_RANGLE_EQUALS, startLocation );
          }
          else return createToken( Token::TK_RANGLE, startLocation );
        
        case '~':
          return createToken( Token::TK_TILDE, startLocation );
        
        case '!':
          if ( m_sourceReader.peek() == '=' )
          {
            m_sourceReader.advance();
            return createToken( Token::TK_EXCL_EQUALS, startLocation );
          }
          else return createToken( Token::TK_EXCL, startLocation );
          
        case '\'':
          while ( !m_sourceReader.isEOF() )
          {
            char ch = m_sourceReader.advance();
            if ( ch == '\\' )
            {
              if ( m_sourceReader.isEOF() )
                throw Exception("unterminated string constant");
              m_sourceReader.advance();
            }
            else if ( ch == '\'' )
            {
              return createToken( Token::TK_CONST_STRING_SQUOT, startLocation );
            }
          }
          throw Exception("unterminated string constant");
          
        case '"':
          while ( !m_sourceReader.isEOF() )
          {
            char ch = m_sourceReader.advance();
            if ( ch == '\\' )
            {
              if ( m_sourceReader.isEOF() )
                throw Exception("unterminated string constant");
              m_sourceReader.advance();
            }
            else if ( ch == '"' )
            {
              return createToken( Token::TK_CONST_STRING_DQUOT, startLocation );
            }
          }
          throw Exception("unterminated string constant");
        
        case ',':
          return createToken( Token::TK_COMMA, startLocation );
          
        case '?':
          return createToken( Token::TK_QUESTION_MARK, startLocation );
        
        case ':':
          return createToken( Token::TK_COLON, startLocation );
          
        default:
          throw Exception("bad symbol");
      }
    }
    
    Token Scanner::nextToken_Digit( Location const &startLocation )
    {
      FABRIC_ASSERT( IsDigit( m_sourceReader.peek() ) );
      
      char ch = m_sourceReader.advance();
      
      Token::Type tokenType = Token::TK_CONST_UI;
      
      if ( ch == '0' && tolower( m_sourceReader.peek() ) == 'x' )
      {
        m_sourceReader.advance();
        if ( !IsHexDigit( m_sourceReader.peek() ) )
          throw Exception("malformed hexadecimal constant");
        while ( IsHexDigit( m_sourceReader.peek() ) )
          m_sourceReader.advance();
      }
      else
      {
        while ( IsDigit( m_sourceReader.peek() ) )
          m_sourceReader.advance();
        
        if ( m_sourceReader.peek() == '.' )
        {
          m_sourceReader.advance();
          tokenType = Token::TK_CONST_FP;
          
          while ( IsDigit( m_sourceReader.peek() ) )
            m_sourceReader.advance();
        }
        
        if ( m_sourceReader.peek() == 'e' || m_sourceReader.peek() == 'E' )
        {
          m_sourceReader.advance();
          tokenType = Token::TK_CONST_FP;
          
          if ( m_sourceReader.peek() == '+' || m_sourceReader.peek() == '-' )
            m_sourceReader.advance();
          
          if ( !IsDigit( m_sourceReader.peek() ) )
            throw Exception("malformed floating point exponent");
          while ( IsDigit( m_sourceReader.peek() ) )
            m_sourceReader.advance();
        }
      }
      
      return createToken( tokenType, startLocation );
    }
    
    Token Scanner::nextToken_Space( Location const &startLocation )
    {
      FABRIC_ASSERT( IsSpace( m_sourceReader.peek() ) );
      while ( IsSpace( m_sourceReader.peek() ) )
        m_sourceReader.advance();
      return createToken( Token::TK_WHITESPACE, startLocation );
    }
    
    Token Scanner::nextToken_Alpha( Location const &startLocation )
    {
      FABRIC_ASSERT( IsAlpha( m_sourceReader.peek() ) );

      m_sourceReader.advance();
      
      while ( IsAlpha( m_sourceReader.peek() ) || IsDigit( m_sourceReader.peek() ) )
        m_sourceReader.advance();
        
      SourceRange sourceRange = createSourceRange( startLocation );
      
      switch( sourceRange.getLength() )
      {
        case 2:
          if ( sourceRange == "in" )
            return createToken( Token::TK_IN, sourceRange );
          if ( sourceRange == "io" )
            return createToken( Token::TK_IO, sourceRange );
          if ( sourceRange == "if" )
            return createToken( Token::TK_IF, sourceRange );
          if ( sourceRange == "do" )
            return createToken( Token::TK_DO, sourceRange );
          break;
          
        case 3:
          if ( sourceRange == "for" )
            return createToken( Token::TK_FOR, sourceRange );
          if ( sourceRange == "var" )
            return createToken( Token::TK_VAR, sourceRange );
          break;
          
        case 4:
          if ( sourceRange == "case" )
            return createToken( Token::TK_CASE, sourceRange );
          if ( sourceRange == "else" )
            return createToken( Token::TK_ELSE, sourceRange );
          if ( sourceRange == "true" )
            return createToken( Token::TK_TRUE, sourceRange );
          break;
          
        case 5:
          if ( sourceRange == "alias" )
            return createToken( Token::TK_ALIAS, sourceRange );
          if ( sourceRange == "break" )
            return createToken( Token::TK_BREAK, sourceRange );
          if ( sourceRange == "const" )
            return createToken( Token::TK_CONST, sourceRange );
          if ( sourceRange == "false" )
            return createToken( Token::TK_FALSE, sourceRange );
          if ( sourceRange == "while" )
            return createToken( Token::TK_WHILE, sourceRange );
          break;
        
        case 6:
          if ( sourceRange == "struct" )
            return createToken( Token::TK_STRUCT, sourceRange );
          if ( sourceRange == "switch" )
            return createToken( Token::TK_SWITCH, sourceRange );
          if ( sourceRange == "report" )
            return createToken( Token::TK_REPORT, sourceRange );
          if ( sourceRange == "return" )
            return createToken( Token::TK_RETURN, sourceRange );
          break;
        
        case 7:
          if ( sourceRange == "default" )
            return createToken( Token::TK_DEFAULT, sourceRange );
          break;
        
        case 8:
          if ( sourceRange == "function" )
            return createToken( Token::TK_FUNCTION, sourceRange );
          if ( sourceRange == "operator" )
            return createToken( Token::TK_OPERATOR, sourceRange );
          if ( sourceRange == "continue" )
            return createToken( Token::TK_CONTINUE, sourceRange );
          break;
        
        case 9:
          break;
      }

      return createToken( Token::TK_TYPE_OR_IDENTIFIER, sourceRange );
    }
    
    Token Scanner::nextToken()
    {
      int ch = m_sourceReader.peek();
      Location startLocation = m_sourceReader.getLocationForStart();
      if ( ch == -1 )
        return createToken( Token::TK_EOI, startLocation );
      if ( IsSpace(ch) )
        return nextToken_Space( startLocation );
      if ( IsAlpha(ch) )
        return nextToken_Alpha( startLocation );
      if ( IsDigit(ch) )
        return nextToken_Digit( startLocation );
      if ( IsSymbol(ch) )
        return nextToken_Symbol( startLocation );
      throw Exception( "bad character" );
    }

    SourceRange Scanner::createSourceRange( Location const &startLocation ) const
    {
      return SourceRange( m_sourceReader.getSource(), startLocation, m_sourceReader.getLocationForEnd() );
    }

    Token Scanner::createToken( Token::Type tokenType, SourceRange const &sourceRange ) const
    {
      return Token( tokenType, sourceRange );
    }
  };
};
