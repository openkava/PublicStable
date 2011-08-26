#include "IndexOp.h"
#include <Fabric/Core/CG/ArrayAdapter.h>
#include <Fabric/Core/CG/SizeAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/RT/ImplType.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( IndexOp );
    
    RC::ConstHandle<IndexOp> IndexOp::Create(
      CG::Location const &location,
      RC::ConstHandle<Expr> const &expr,
      RC::ConstHandle<Expr> const &indexExpr
      )
    {
      return new IndexOp( location, expr, indexExpr );
    }
    
    IndexOp::IndexOp( CG::Location const &location, RC::ConstHandle<Expr> const &expr, RC::ConstHandle<Expr> const &indexExpr )
      : Expr( location )
      , m_expr( expr )
      , m_indexExpr( indexExpr )
    {
    }
    
    void IndexOp::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator );
      m_expr->appendJSON( jsonObjectGenerator.makeMember( "expr" ) );
      m_indexExpr->appendJSON( jsonObjectGenerator.makeMember( "indexExpr" ) );
    }
    
    void IndexOp::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      m_expr->registerTypes( cgManager, diagnostics );
      m_indexExpr->registerTypes( cgManager, diagnostics );
    }
    
    RC::ConstHandle<CG::Adapter> IndexOp::getType( CG::BasicBlockBuilder &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> exprType = m_expr->getType( basicBlockBuilder );
      exprType->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
      
      if ( !RT::isArray( exprType->getType() ) )
        throw Exception( "only arrays can be indexed" );
      RC::ConstHandle<CG::ArrayAdapter> arrayType = RC::ConstHandle<CG::ArrayAdapter>::StaticCast( exprType );
      
      RC::ConstHandle<CG::Adapter> memberAdapter = arrayType->getMemberAdapter();
      memberAdapter->llvmCompileToModule( basicBlockBuilder.getModuleBuilder() );
      return memberAdapter;
    }
    
    CG::ExprValue IndexOp::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      CG::ExprValue result( basicBlockBuilder.getContext() );
      try
      {
        CG::ExprValue arrayExprValue = m_expr->buildExprValue( basicBlockBuilder, usage, lValueErrorDesc );
        RC::ConstHandle<CG::Adapter> adapter = arrayExprValue.getAdapter();
        
        if ( !RT::isArray( arrayExprValue.getAdapter()->getType() ) )
          throw CG::Error( getLocation(), "only arrays can be indexed" );
        RC::ConstHandle<CG::ArrayAdapter> arrayAdapter = RC::ConstHandle<CG::ArrayAdapter>::StaticCast( adapter );
        
        CG::ExprValue indexExprValue = m_indexExpr->buildExprValue( basicBlockBuilder, CG::USAGE_RVALUE, lValueErrorDesc );
        RC::ConstHandle< CG::SizeAdapter > sizeAdapter = basicBlockBuilder.getManager()->getSizeAdapter();
        llvm::Value *indexExprRValue = sizeAdapter->llvmCast( basicBlockBuilder, indexExprValue );
        if ( indexExprRValue )
        {
          switch ( usage )
          {
            case CG::USAGE_LVALUE:
              result = CG::ExprValue( arrayAdapter->getMemberAdapter(), CG::USAGE_LVALUE, basicBlockBuilder.getContext(), arrayAdapter->llvmNonConstIndexOp( basicBlockBuilder, arrayExprValue.getValue(), indexExprRValue ) );
              break;
            default:
              result = CG::ExprValue( arrayAdapter->getMemberAdapter(), CG::USAGE_RVALUE, basicBlockBuilder.getContext(), arrayAdapter->llvmConstIndexOp( basicBlockBuilder, arrayExprValue.getValue(), indexExprRValue ) );
              break;
          }
          arrayExprValue.llvmDispose( basicBlockBuilder );
        }
      }
      catch ( CG::Error e )
      {
        throw e;
      }
      catch ( Exception e )
      {
        throw CG::Error( getLocation(), e );
      }
      return result;
    }
  };
};
