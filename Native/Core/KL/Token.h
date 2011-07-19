/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_TOKEN_H
#define _FABRIC_KL_TOKEN_H

#include <Fabric/Core/KL/SourceRange.h>

namespace Fabric
{
  namespace KL
  {
    class Token
    {
    public:
    
      enum Type
      {
        TK_EOI,

        TK_WHITESPACE,
        TK_COMMENT_BLOCK,
        TK_COMMENT_LINE,

        TK_IF,
        TK_IN,
        TK_IO,
        TK_DO,
        TK_FOR,
        TK_VAR,
        TK_CASE,
        TK_ELSE,
        TK_TRUE,
        TK_ALIAS,
        TK_BREAK,
        TK_CONST,
        TK_FALSE,
        TK_WHILE,
        TK_REPORT,
        TK_RETURN,
        TK_STRUCT,
        TK_SWITCH,
        TK_DEFAULT,
        TK_CONTINUE,
        TK_FUNCTION,
        TK_OPERATOR,

        TK_LBRACE,
        TK_RBRACE,
        TK_LPAREN,
        TK_RPAREN,
        TK_LBRACKET,
        TK_RBRACKET,
        TK_COMMA,
        TK_SEMICOLON,
        TK_DOT,
        TK_COLON,
        TK_QUESTION_MARK,

        TK_PLUS,
        TK_PLUS_PLUS,
        TK_MINUS,
        TK_MINUS_MINUS,

        TK_AST,
        TK_FSLASH,
        TK_PERCENT,

        TK_EXCL,
        TK_AMP_AMP,
        TK_PIPE_PIPE,
        TK_CARET_CARET,

        TK_TILDE,
        TK_AMP,
        TK_PIPE,
        TK_CARET,

        TK_EQUALS,
        TK_PLUS_EQUALS,
        TK_MINUS_EQUALS,
        TK_AST_EQUALS,
        TK_FSLASH_EQUALS,
        TK_PERCENT_EQUALS,
        TK_AMP_AMP_EQUALS,
        TK_PIPE_PIPE_EQUALS,
        TK_CARET_CARET_EQUALS,
        TK_AMP_EQUALS,
        TK_PIPE_EQUALS,
        TK_CARET_EQUALS,

        TK_EQUALS_EQUALS,
        TK_EXCL_EQUALS,
        TK_LANGLE,
        TK_LANGLE_EQUALS,
        TK_RANGLE,
        TK_RANGLE_EQUALS,

        TK_CONST_UI,
        TK_CONST_FP,
        TK_CONST_STRING_SQUOT,
        TK_CONST_STRING_DQUOT,
        
        TK_IDENTIFIER,
        
        NUM_TOKENS
      };
      
      Token()
      {
      }
      
      Token( Type type, SourceRange const &sourceRange )
        : m_type( type )
        , m_sourceRange( sourceRange )
      {
      }
      
      Token( Type type, RC::ConstHandle<Source> const &source, Location const &startLocation, Location const &endLocation )
        : m_type( type )
        , m_sourceRange( source, startLocation, endLocation )
      {
      }
      
      Token( Token const &that )
        : m_type( that.m_type )
        , m_sourceRange( that.m_sourceRange )
      {
      }
      
      Token &operator =( Token const &that )
      {
        m_type = that.m_type;
        m_sourceRange = that.m_sourceRange;
        return *this;
      }
      
      Type getType() const
      {
        return m_type;
      }
      
      SourceRange const &getSourceRange() const
      {
        return m_sourceRange;
      }
      
      Location const &getStartLocation() const
      {
        return m_sourceRange.getStartLocation();
      }
      
      Location const &getEndLocation() const
      {
        return m_sourceRange.getEndLocation();
      }
      
      std::string toString() const
      {
        return m_sourceRange.toString();
      }
      
      static char const *TypeName( Type type )
      {
        return s_friendlies[type].name;
      }
      
      static char const *TypeDesc( Type type )
      {
        return s_friendlies[type].desc;
      }
    
    private:
    
      Type m_type;
      SourceRange m_sourceRange;
      
      struct Friendly
      {
        char const *name;
        char const *desc;
      };
      static Friendly s_friendlies[NUM_TOKENS];
    };
  };
};

#endif //_FABRIC_KL_TOKEN_H
