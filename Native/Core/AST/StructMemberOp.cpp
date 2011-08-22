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
    
    void StructMemberOp::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator );
      m_structExpr->appendJSON( jsonObjectGenerator.makeMember( "expr" ) );
      jsonObjectGenerator.makeMember( "memberName" ).makeString( m_memberName );
    }
    
    void StructMemberOp::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      m_structExpr->llvmPrepareModule( moduleBuilder, diagnostics );
    }
    
    RC::ConstHandle<CG::Adapter> StructMemberOp::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> structType = m_structExpr->getType( basicBlockBuilder );
      if ( RT::isStruct( structType->getType() ) )
      {
        RC::ConstHandle<CG::StructAdapter> structAdapter = RC::ConstHandle<CG::StructAdapter>::StaticCast( structType );
        if ( !structAdapter->hasMember( m_memberName ) )
          throw Exception( "structure has no member named " + _(m_memberName) );
        size_t memberIndex = structAdapter->getMemberIndex( m_memberName );
        return structAdapter->getMemberAdapter( memberIndex );
      }
      else
      {
        std::string functionName = methodOverloadName( m_memberName, structType );
        RC::ConstHandle<CG::FunctionSymbol> functionSymbol = basicBlockBuilder.maybeGetFunction( functionName );
        if ( !functionSymbol )
          throw Exception( "type " + structType->getUserName() + " has no member or method named " + _(m_memberName) );
        return functionSymbol->getReturnInfo().getAdapter();
      }
    }
    
    CG::ExprValue StructMemberOp::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      try
      {
        if ( usage == CG::USAGE_UNSPECIFIED )
          usage = CG::USAGE_RVALUE;
          
        RC::ConstHandle<CG::Adapter> adapter = m_structExpr->getType( basicBlockBuilder );
        
        if ( RT::isStruct( adapter->getType() ) )
        {
          CG::ExprValue structExprValue = m_structExpr->buildExprValue( basicBlockBuilder, usage, lValueErrorDesc );
        
          RC::ConstHandle<CG::StructAdapter> structAdapter = RC::ConstHandle<CG::StructAdapter>::StaticCast( adapter );
          if ( structAdapter->hasMember( m_memberName ) )
          {
            size_t memberIndex = structAdapter->getMemberIndex( m_memberName );
            RC::ConstHandle<CG::Adapter> memberAdapter = structAdapter->getMemberAdapter( memberIndex );
            llvm::Value *memberLValue = basicBlockBuilder->CreateConstGEP2_32( structExprValue.getValue(), 0, memberIndex );
            CG::ExprValue result;
            switch ( usage )
            {
              case CG::USAGE_RVALUE:
              {
                llvm::Value *memberRValue = memberAdapter->llvmLValueToRValue( basicBlockBuilder, memberLValue );
                memberAdapter->llvmRetain( basicBlockBuilder, memberRValue );
                result = CG::ExprValue( memberAdapter, CG::USAGE_RVALUE, memberRValue );
              }
              break;
              
              case CG::USAGE_LVALUE:
                result = CG::ExprValue( memberAdapter, CG::USAGE_LVALUE, memberLValue );
                break;
              
              case CG::USAGE_UNSPECIFIED:
                FABRIC_ASSERT( false );
                throw Exception( "unspecified usage" );
            }
            structExprValue.llvmDispose( basicBlockBuilder );
            return result;
          }
        }

        std::string functionName = methodOverloadName( m_memberName, adapter );
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
