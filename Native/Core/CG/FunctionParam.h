/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_CG_FUNCTION_PARAM_H
#define _FABRIC_CG_FUNCTION_PARAM_H

#include <Fabric/Core/CG/ExprType.h>
#include <Fabric/Base/Exception.h>

#include <string>

namespace Fabric
{
  namespace CG
  {
    class FunctionParam
    {
    public:
    
      FunctionParam( std::string const &name, ExprType const &exprType )
        : m_name( name )
        , m_exprType( exprType )
      {
      }
    
      FunctionParam( std::string const &name, RC::ConstHandle<Adapter> const &adapter, Usage usage )
        : m_name( name )
        , m_exprType( adapter, usage )
      {
      }
      
      FunctionParam( FunctionParam const &that )
        : m_name( that.m_name )
        , m_exprType( that.m_exprType )
      {
      }
      
      FunctionParam &operator =( FunctionParam const &that )
      {
        m_name = that.m_name;
        m_exprType = that.m_exprType;
        return *this;
      }
    
      std::string const &getName() const
      {
        return m_name;
      }
      
      ExprType const &getExprType() const
      {
        return m_exprType;
      }
      
      std::string const &getTypeUserName() const
      {
        return getExprType().getUserName();
      }
      
      std::string const &getTypeCodeName() const
      {
        return getExprType().getCodeName();
      }
      
      RC::ConstHandle<Adapter> getAdapter() const
      {
        return getExprType().getAdapter();
      }
      
      Usage getUsage() const
      {
        return getExprType().getUsage();
      }
      
      llvm::Type const *getLLVMType( RC::Handle<Context> const &context ) const
      {
        switch ( getUsage() )
        {
          case USAGE_RVALUE:
            return getAdapter()->llvmRType( context );
          case USAGE_LVALUE:
            return getAdapter()->llvmLType( context );
          case USAGE_UNSPECIFIED:
            FABRIC_ASSERT( false );
            throw Exception( "usage unspecified" );
        }
        return 0;
      }
      
      std::string desc() const
      {
        return m_name + ":" + m_exprType.desc();
      }

    private:
    
      std::string m_name;
      ExprType m_exprType;
    };
  }
}

#endif //_FABRIC_CG_FUNCTION_PARAM_H
