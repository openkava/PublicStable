/*
 *
 *  Created by Peter Zion on 10-12-03.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_CG_OP_TYPES_H
#define _FABRIC_CG_OP_TYPES_H

#include <Fabric/Core/Util/Assert.h>

#include <string>
#include <vector>

namespace Fabric
{
  namespace CG
  {
    enum UniOpType
    {
      UNI_OP_POS,
      UNI_OP_NEG,
      UNI_OP_BIT_NOT,
      UNI_OP_PRE_INC,
      UNI_OP_PRE_DEC,
      UNI_OP_POST_INC,
      UNI_OP_POST_DEC,
      NUM_UNI_OPS
    };

    inline std::string const &uniOpCodeName( UniOpType uniOpType )
    {
      static std::vector<std::string> uniOpCodeNames;
      if ( uniOpCodeNames.empty() )
      {
        uniOpCodeNames.resize( NUM_UNI_OPS );
        uniOpCodeNames[UNI_OP_POS] = "POS";
        uniOpCodeNames[UNI_OP_NEG] = "NEG";
        uniOpCodeNames[UNI_OP_BIT_NOT] = "BIT_NOT";
        uniOpCodeNames[UNI_OP_PRE_INC] = "PRE_INC";
        uniOpCodeNames[UNI_OP_PRE_DEC] = "PRE_DEC";
        uniOpCodeNames[UNI_OP_POST_INC] = "POST_INC";
        uniOpCodeNames[UNI_OP_POST_DEC] = "POST_DEC";
      }
      std::string const &result = uniOpCodeNames[uniOpType];
      FABRIC_ASSERT( result.length() > 0 );
      return result;
    }

    inline std::string const &uniOpUserName( UniOpType uniOpType )
    {
      static std::vector<std::string> uniOpUserNames;
      if ( uniOpUserNames.empty() )
      {
        uniOpUserNames.resize( NUM_UNI_OPS );
        uniOpUserNames[UNI_OP_POS] = "+";
        uniOpUserNames[UNI_OP_NEG] = "-";
        uniOpUserNames[UNI_OP_BIT_NOT] = "~";
        uniOpUserNames[UNI_OP_PRE_INC] = "prefix ++";
        uniOpUserNames[UNI_OP_PRE_DEC] = "prefix --";
        uniOpUserNames[UNI_OP_POST_INC] = "postfix ++";
        uniOpUserNames[UNI_OP_POST_DEC] = "postfix --";
      }
      std::string const &result = uniOpUserNames[uniOpType];
      FABRIC_ASSERT( result.length() > 0 );
      return result;
    }
    
    enum BinOpType
    {
      BIN_OP_ADD,
      BIN_OP_SUB,
      BIN_OP_MUL,
      BIN_OP_DIV,
      BIN_OP_MOD,
      BIN_OP_EQ,
      BIN_OP_NE,
      BIN_OP_LT,
      BIN_OP_LE,
      BIN_OP_GT,
      BIN_OP_GE,
      BIN_OP_BIT_OR,
      BIN_OP_BIT_AND,
      BIN_OP_BIT_XOR,
      NUM_BIN_OPS
    };

    inline std::string const &binOpCodeName( BinOpType binOpType )
    {
      static std::vector<std::string> binOpCodeNames;
      if ( binOpCodeNames.empty() )
      {
        binOpCodeNames.resize( NUM_BIN_OPS );
        binOpCodeNames[BIN_OP_ADD] = "ADD";
        binOpCodeNames[BIN_OP_SUB] = "SUB";
        binOpCodeNames[BIN_OP_MUL] = "MUL";
        binOpCodeNames[BIN_OP_DIV] = "DIV";
        binOpCodeNames[BIN_OP_MOD] = "MOD";
        binOpCodeNames[BIN_OP_EQ] = "EQ";
        binOpCodeNames[BIN_OP_NE] = "NE";
        binOpCodeNames[BIN_OP_LT] = "LT";
        binOpCodeNames[BIN_OP_LE] = "LE";
        binOpCodeNames[BIN_OP_GT] = "GT";
        binOpCodeNames[BIN_OP_GE] = "GE";
        binOpCodeNames[BIN_OP_BIT_OR] = "BIT_OR";
        binOpCodeNames[BIN_OP_BIT_AND] = "BIT_AND";
        binOpCodeNames[BIN_OP_BIT_XOR] = "BIT_XOR";
      }
      std::string const &result = binOpCodeNames[binOpType];
      FABRIC_ASSERT( result.length() > 0 );
      return result;
    }

    inline std::string const &binOpUserName( BinOpType binOpType )
    {
      static std::vector<std::string> binOpUserNames;
      if ( binOpUserNames.empty() )
      {
        binOpUserNames.resize( NUM_BIN_OPS );
        binOpUserNames[BIN_OP_ADD] = "+";
        binOpUserNames[BIN_OP_SUB] = "-";
        binOpUserNames[BIN_OP_MUL] = "*";
        binOpUserNames[BIN_OP_DIV] = "/";
        binOpUserNames[BIN_OP_MOD] = "%";
        binOpUserNames[BIN_OP_EQ] = "==";
        binOpUserNames[BIN_OP_NE] = "!=";
        binOpUserNames[BIN_OP_LT] = "<";
        binOpUserNames[BIN_OP_LE] = "<=";
        binOpUserNames[BIN_OP_GT] = ">";
        binOpUserNames[BIN_OP_GE] = ">=";
        binOpUserNames[BIN_OP_BIT_OR] = "|";
        binOpUserNames[BIN_OP_BIT_AND] = "&";
        binOpUserNames[BIN_OP_BIT_XOR] = "^";
      }
      std::string const &result = binOpUserNames[binOpType];
      FABRIC_ASSERT( result.length() > 0 );
      return result;
    }
    
#define FABRIC_CG_OP_TYPES_HANDLE_DESC( value ) \
  case value: \
  { \
    static std::string desc = #value; \
    return desc; \
  }
#define FABRIC_CG_OP_TYPES_HANDLE_DEFAULT() \
  default: \
  { \
    FABRIC_ASSERT( false && "Unhandled value" ); \
    static std::string desc = "<INTERNAL ERROR>"; \
    return desc; \
  }
    
    enum TernaryOpType
    {
      TERNARY_OP_COND
    };
    
    inline char const *ternaryOpTypeDesc( TernaryOpType ternaryOpType )
    {
      switch( ternaryOpType )
      {
        case TERNARY_OP_COND:
          return "TERNARY_OP_COND";
      }
      return "<unknown>";
    }
    
    enum AssignOpType
    {
      ASSIGN_OP,
      ASSIGN_OP_ADD,
      ASSIGN_OP_SUB,
      ASSIGN_OP_MUL,
      ASSIGN_OP_DIV,
      ASSIGN_OP_MOD,
    };
    
    inline char const *assignOpTypeDesc( AssignOpType assignOpType )
    {
      switch ( assignOpType )
      {
        case ASSIGN_OP: return "ASSIGN_OP";
        case ASSIGN_OP_ADD: return "ASSIGN_OP_ADD";
        case ASSIGN_OP_SUB: return "ASSIGN_OP_SUB";
        case ASSIGN_OP_MUL: return "ASSIGN_OP_MUL";
        case ASSIGN_OP_DIV: return "ASSIGN_OP_DIV";
        case ASSIGN_OP_MOD: return "ASSIGN_OP_MOD";
      }
      return "<unknown>";
    }
    
    inline BinOpType binOpForAssignOp( AssignOpType assignOpType )
    {
      switch ( assignOpType )
      {
        case ASSIGN_OP_ADD: return BIN_OP_ADD;
        case ASSIGN_OP_SUB: return BIN_OP_SUB;
        case ASSIGN_OP_MUL: return BIN_OP_MUL;
        case ASSIGN_OP_DIV: return BIN_OP_DIV;
        case ASSIGN_OP_MOD: return BIN_OP_MOD;
        default: FABRIC_ASSERT( false ); return BIN_OP_ADD;
      }
    }
  };
  
  inline std::string const &_( CG::BinOpType binOpType )
  {
    return binOpCodeName( binOpType );
  }
};

#endif //_FABRIC_CG_OP_TYPES_H

