#include <Fabric/Core/AST/MethodOp.h>
#include <Fabric/Core/AST/ExprVector.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( MethodOp );
    
    RC::ConstHandle<MethodOp> MethodOp::Create(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<Expr> const &expr,
      RC::ConstHandle<ExprVector> const &args
      )
    {
      return new MethodOp( location, name, expr, args );
    }
    
    MethodOp::MethodOp(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<Expr> const &expr,
      RC::ConstHandle<ExprVector> const &args
      )
      : Expr( location )
      , m_name( name )
      , m_expr( expr )
      , m_args( args )
    {
    }
    
    void MethodOp::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Expr::appendJSONMembers( jsonObjectGenerator );
      m_expr->appendJSON( jsonObjectGenerator.makeMember( "expr" ) );
      jsonObjectGenerator.makeMember( "methodName" ).makeString( m_name );
      m_args->appendJSON( jsonObjectGenerator.makeMember( "args" ) );
    }
    
    RC::ConstHandle<CG::FunctionSymbol> MethodOp::getFunctionSymbol( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      RC::ConstHandle<CG::Adapter> selfType = m_expr->getType( basicBlockBuilder );
      
      std::vector< RC::ConstHandle<CG::Adapter> > paramTypes;
      m_args->appendTypes( basicBlockBuilder, paramTypes );
      
      std::string functionName = CG::methodOverloadName( m_name, selfType, paramTypes );
      RC::ConstHandle<CG::FunctionSymbol> functionSymbol = basicBlockBuilder.maybeGetFunction( functionName );
      if ( !functionSymbol )
        throw CG::Error( getLocation(), "type " + selfType->getUserName() + " has no method named " + _(m_name) );
      return functionSymbol;
    }
    
    void MethodOp::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      m_expr->llvmPrepareModule( moduleBuilder, diagnostics );
      m_args->llvmPrepareModule( moduleBuilder, diagnostics );
    }
    
    RC::ConstHandle<CG::Adapter> MethodOp::getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const
    {
      return getFunctionSymbol( basicBlockBuilder )->getReturnInfo().getAdapter();
    }
    
    CG::ExprValue MethodOp::buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const
    {
      RC::ConstHandle<CG::FunctionSymbol> functionSymbol = getFunctionSymbol( basicBlockBuilder );
      std::vector<CG::FunctionParam> const &functionParams = functionSymbol->getParams();
      
      try
      {
        CG::Usage selfUsage = functionParams[0].getUsage();
        std::vector<CG::Usage> argUsages;
        for ( size_t i=1; i<functionParams.size(); ++i )
          argUsages.push_back( functionParams[i].getUsage() );
        
        std::vector<CG::ExprValue> argExprValues;
        CG::ExprValue selfExprValue = m_expr->buildExprValue( basicBlockBuilder, selfUsage, "cannot be modified" );
        argExprValues.push_back( selfExprValue );
        m_args->appendExprValues( basicBlockBuilder, argUsages, argExprValues, "cannot be used as an io argument" );
        
        CG::ExprValue callResultExprValue = functionSymbol->llvmCreateCall( basicBlockBuilder, argExprValues );

        CG::ExprValue result;
        if ( functionSymbol->getReturnInfo().getExprType() )
          result = callResultExprValue;
        else result = selfExprValue;

        return result.castTo( basicBlockBuilder, usage );
      }
      catch ( CG::Error e )
      {
        throw "calling method " + _(m_name) + " of type " + _(functionParams[0].getAdapter()->getUserName()) + ": " + e;
      }
      catch ( Exception e )
      {
        throw CG::Error( getLocation(), "calling method " + _(m_name) + " of type " + _(functionParams[0].getAdapter()->getUserName()) + ": " + e );
      }
    }
  };
};
