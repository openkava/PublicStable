/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_EXPR_TYPE_H
#define _FABRIC_CG_EXPR_TYPE_H

#include <Fabric/Base/RC/ConstHandle.h>

#include <string>
#include <vector>

namespace Fabric
{
  namespace RT
  {
    class Desc;
    class Impl;
  };
  
  namespace CG
  {
    class Adapter;
    
    enum Usage
    {
      USAGE_UNSPECIFIED,
      USAGE_RVALUE,
      USAGE_LVALUE
    };
    
    inline char const *usageDesc( Usage usage )
    {
      switch ( usage )
      {
        case USAGE_RVALUE:
          return "r-value";
        case USAGE_LVALUE:
          return "l-value";
        case USAGE_UNSPECIFIED:
          return "unspecified";
      }
      return "<unknown usage>";
    }
  
    class ExprType
    {
      RC::ConstHandle<Adapter> m_adapter;
      Usage m_usage;
      
    public:
    
      ExprType();
      ExprType( RC::ConstHandle<Adapter> const &adapter, Usage usage );
      ExprType( ExprType const &that );
      ~ExprType();
      
      ExprType &operator =( ExprType const &that );
      
      void set( RC::ConstHandle<Adapter> const &adapter, Usage usage );

      bool isValid() const;
      operator bool() const;
      bool operator !() const;
      
      RC::ConstHandle<Adapter> getAdapter() const;
      Usage getUsage() const;
      
      RC::ConstHandle<RT::Desc> getDesc() const;
      RC::ConstHandle<RT::Impl> getImpl() const;

      std::string const &getUserName() const;
      std::string const &getCodeName() const;
      
      bool operator ==( ExprType const &that ) const;
      bool operator !=( ExprType const &that ) const;
    };
    
    class ExprTypeVector : public std::vector<ExprType>
    {
    public:
    
      ExprTypeVector()
      {
      }
      
      std::string desc() const;
    };
  }
}

#endif //_FABRIC_CG_EXPR_TYPE_H
