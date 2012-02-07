/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "StructMemberOp.h"
#include <Fabric/Core/CG/StructAdapter.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/RT/StructMemberInfo.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( StructMemberOp );
    
    StructMemberOp::StructMemberOp( CG::Location const &location, RC::ConstHandle<Expr> const &structExpr, std::string const &memberName )
      : Expr( location )
      , m_structExpr( structExpr )
      , m_memberName( memberName )
    {
    }
    
    void StructMemberOp::appendJSONMembers( JSON::ObjectEncoder const &jsonObjectEncoder, bool includeLocation ) const
    {
      Expr::appendJSONMembers( jsonObjectEncoder, includeLocation );
      m_structExpr->appendJSON( jsonObjectEncoder.makeMember( "expr" ), includeLocation );
      jsonObjectEncoder.makeMember( "memberName" ).makeString( m_memberName );
    }
    
    void StructMemberOp::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_structExpr->registerTypes( cgManager, diagnostics );
    }
    
    CG::ExprType StructMemberOp::getExprType( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      CG::ExprType structExprType = m_structExpr->getExprType( basicBlockBuilder );
      if ( RT::isStruct( structExprType.getAdapter()->getType() ) )
      {
        RC::ConstHandle<CG::StructAdapter> structAdapter = RC::ConstHandle<CG::StructAdapter>::StaticCast( structExprType.getAdapter() );
        if ( structAdapter->hasMember( m_memberName ) )
        {
          size_t memberIndex = structAdapter->getMemberIndex( m_memberName );
          RC::ConstHandle<CG::Adapter> memberAdapter = structAdapter->getMemberAdapter( memberIndex );
          memberAdapter->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
          return CG::ExprType( memberAdapter, structExprType.getUsage() );
        }
      }
      
      std::string functionName = methodOverloadName( m_memberName, structExprType );
      RC::ConstHandle<CG::FunctionSymbol> functionSymbol = basicBlockBuilder.maybeGetFunction( functionName );
      if ( !functionSymbol )
        throw CG::Error( getLocation(), "type " + structExprType.getUserName() + " has no member or method named " + _(m_memberName) );
      RC::ConstHandle<CG::Adapter> returnAdapter = functionSymbol->getReturnInfo().getAdapter();
      returnAdapter->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
      return CG::ExprType( returnAdapter, CG::USAGE_RVALUE );
    }
    
    CG::ExprValue StructMemberOp::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      try
      {
        if ( usage == CG::USAGE_UNSPECIFIED )
          usage = CG::USAGE_RVALUE;
          
        CG::ExprType exprType = m_structExpr->getExprType( basicBlockBuilder );
        RC::ConstHandle<CG::Adapter> adapter = exprType.getAdapter();
        
        if ( RT::isStruct( adapter->getType() ) )
        {
          CG::ExprValue structExprValue = m_structExpr->buildExprValue( basicBlockBuilder, usage, lValueErrorDesc );
        
          RC::ConstHandle<CG::StructAdapter> structAdapter = RC::ConstHandle<CG::StructAdapter>::StaticCast( adapter );
          if ( structAdapter->hasMember( m_memberName ) )
          {
            size_t memberIndex = structAdapter->getMemberIndex( m_memberName );
            RC::ConstHandle<CG::Adapter> memberAdapter = structAdapter->getMemberAdapter( memberIndex );
            llvm::Value *memberLValue = basicBlockBuilder->CreateConstGEP2_32( structExprValue.getValue(), 0, memberIndex );
            CG::ExprValue result( basicBlockBuilder.getContext() );
            switch ( usage )
            {
              case CG::USAGE_RVALUE:
              {
                llvm::Value *memberRValue = memberAdapter->llvmLValueToRValue( basicBlockBuilder, memberLValue );
                result = CG::ExprValue( memberAdapter, CG::USAGE_RVALUE, basicBlockBuilder.getContext(), memberRValue );
              }
              break;
              
              case CG::USAGE_LVALUE:
                result = CG::ExprValue( memberAdapter, CG::USAGE_LVALUE, basicBlockBuilder.getContext(), memberLValue );
                break;
              
              case CG::USAGE_UNSPECIFIED:
                FABRIC_ASSERT( false );
                throw Exception( "unspecified usage" );
            }
            return result;
          }
        }

        std::string functionName = methodOverloadName( m_memberName, CG::ExprType( adapter, CG::USAGE_RVALUE ) );
        RC::ConstHandle<CG::FunctionSymbol> functionSymbol = basicBlockBuilder.maybeGetFunction( functionName );
        if ( functionSymbol )
        {
          std::vector<CG::FunctionParam> const &functionParams = functionSymbol->getParams();
          
          if ( functionParams.size() != 1 )
            throw CG::Error( getLocation(), "method "+_(m_memberName)+" of type +"+_(adapter->getUserName())+" takes one or more parameters" );
          
          CG::ExprValue structExprValue = m_structExpr->buildExprValue( basicBlockBuilder, functionParams[0].getUsage(), lValueErrorDesc );
          return functionSymbol->llvmCreateCall( basicBlockBuilder, structExprValue );
        }

        throw CG::Error( getLocation(), _(adapter->getUserName()) + " has no member or method named " + _(m_memberName) );
      }
      catch ( Exception e )
      {
        throw CG::Error( getLocation(), e );
      }
    }
  };
};
