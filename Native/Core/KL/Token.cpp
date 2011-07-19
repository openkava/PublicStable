/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KL/Token.h>

namespace Fabric
{
  namespace KL
  {
    Token::Friendly Token::s_friendlies[Token::NUM_TOKENS] =
    {
      { "TK_EOI", "end of input" },

      { "TK_WHITESPACE", "whitespace" },

      { "TK_COMMENT_BLOCK", "block comment" },
      { "TK_COMMENT_LINE", "line comment" },

      { "TK_IF", "if" },
      { "TK_IN", "in" },
      { "TK_IO", "io" },
      { "TK_DO", "do" },
      { "TK_FOR", "for" },
      { "TK_VAR", "var" },
      { "TK_CASE", "case" },
      { "TK_ELSE", "else" },
      { "TK_TRUE", "true" },
      { "TK_ALIAS", "alias" },
      { "TK_BREAK", "break" },
      { "TK_CONST", "const" },
      { "TK_FALSE", "false" },
      { "TK_WHILE", "while" },
      { "TK_REPORT", "report" },
      { "TK_RETURN", "return" },
      { "TK_STRUCT", "struct" },
      { "TK_SWITCH", "switch" },
      { "TK_DEFAULT", "default" },
      { "TK_CONTINUE", "continue" },
      { "TK_FUNCTION", "function" },
      { "TK_OPERATOR", "operator" },

      { "TK_LBRACE", "{" },
      { "TK_RBRACE", "}" },
      { "TK_LPAREN", "(" },
      { "TK_RPAREN", ")" },
      { "TK_LBRACKET", "[" },
      { "TK_RBRACKET", "]" },
      { "TK_COMMA", "," },
      { "TK_SEMICOLON", ";" },
      { "TK_DOT", "." },
      { "TK_COLON", ":" },
      { "TK_QUESTION_MARK", "?" },

      { "TK_PLUS", "+" },
      { "TK_PLUS_PLUS", "++" },
      { "TK_MINUS", "-" },
      { "TK_MINUS_MINUS", "--" },

      { "TK_AST", "*" },
      { "TK_FSLASH", "/" },
      { "TK_PERCENT", "%" },

      { "TK_EXCL", "!" },
      { "TK_AMP_AMP", "&&" },
      { "TK_PIPE_PIPE", "||" },
      { "TK_CARET_CARET", "^^" },

      { "TK_TILDE", "~" },
      { "TK_AMP", "&" },
      { "TK_PIPE", "|" },
      { "TK_CARET", "^" },

      { "TK_EQUALS", "=" },
      { "TK_PLUS_EQUALS", "+=" },
      { "TK_MINUS_EQUALS", "-=" },
      { "TK_AST_EQUALS", "*=" },
      { "TK_FSLASH_EQUALS", "/=" },
      { "TK_PERCENT_EQUALS", "%=" },
      { "TK_AMP_AMP_EQUALS", "&&=" },
      { "TK_PIPE_PIPE_EQUALS", "||=" },
      { "TK_CARET_CARET_EQUALS", "^^=" },
      { "TK_AMP_EQUALS", "&=" },
      { "TK_PIPE_EQUALS", "|=" },
      { "TK_CARET_EQUALS", "^=" },

      { "TK_EQUALS_EQUALS", "==" },
      { "TK_EXCL_EQUALS", "!=" },
      { "TK_LANGLE", "<" },
      { "TK_LANGLE_EQUALS", "<=" },
      { "TK_RANGLE", ">" },
      { "TK_RANGLE_EQUALS", ">=" },

      { "TK_CONST_UI", "integer constant" },
      { "TK_CONST_FP", "floating point constant" },
      { "TK_CONST_STRING_SQUOT", "single-quoted string constant" },
      { "TK_CONST_STRING_DQUOT", "double-quoted string constant" },
      
      { "TK_IDENTIFIER", "type or identifier" }
    };
  };
};
