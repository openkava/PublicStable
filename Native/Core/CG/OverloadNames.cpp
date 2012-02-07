#include "OverloadNames.h"
#include "Adapter.h"

namespace Fabric
{
  namespace CG
  {
    static std::string genericFunctionName(
      std::string const &genericFunctionName,
      std::vector<CG::ExprType> const &paramExprTypes
      )
    {
      std::string result = "__" + genericFunctionName;
      for ( std::vector<CG::ExprType>::const_iterator it=paramExprTypes.begin(); it!=paramExprTypes.end(); ++it )
      {
        switch ( it->getUsage() )
        {
          case CG::USAGE_LVALUE:
            result += "__"; //io_";
            break;
          case CG::USAGE_RVALUE:
            result += "__"; //in_";
            break;
          default:
            FABRIC_ASSERT( false && "usage unspeicifed" );
            break;
        }
        result += it->getAdapter()->getCodeName();
      }
      return result;
    }
    
    std::string constructorOverloadName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      std::vector< RC::ConstHandle<CG::Adapter> > const &paramAdapters
      )
    {
      std::vector<CG::ExprType> exprTypes;
      exprTypes.push_back( CG::ExprType( thisAdapter, CG::USAGE_LVALUE ) );
      for ( std::vector< RC::ConstHandle<CG::Adapter> >::const_iterator it = paramAdapters.begin(); it != paramAdapters.end(); ++it )
        exprTypes.push_back( CG::ExprType( *it, CG::USAGE_RVALUE ) );
      return genericFunctionName( "constructor", exprTypes );
    }

    std::string assignOpOverloadName( AssignOpType type, RC::ConstHandle<CG::Adapter> const &thisAdapter, RC::ConstHandle<CG::Adapter> const &thatAdapter )
    {
      std::vector<CG::ExprType> exprTypes;
      exprTypes.push_back( CG::ExprType( thisAdapter, CG::USAGE_LVALUE ) );
      exprTypes.push_back( CG::ExprType( thatAdapter, CG::USAGE_RVALUE ) );
      return genericFunctionName( "operator_assign_" + std::string( assignOpCodeName(type) ), exprTypes );
    }

    std::string uniOpOverloadName( UniOpType type, RC::ConstHandle<CG::Adapter> const &adapter )
    {
      std::vector<CG::ExprType> exprTypes;
      exprTypes.push_back( CG::ExprType( adapter, CG::USAGE_RVALUE ) );
      return genericFunctionName( "operator_uni_" + std::string( uniOpCodeName(type) ), exprTypes );
    }

    std::string binOpOverloadName( BinOpType type, RC::ConstHandle<CG::Adapter> const &lhsAdapter, RC::ConstHandle<CG::Adapter> const &rhsAdapter )
    {
      std::vector<CG::ExprType> exprTypes;
      exprTypes.push_back( CG::ExprType( lhsAdapter, CG::USAGE_RVALUE ) );
      exprTypes.push_back( CG::ExprType( rhsAdapter, CG::USAGE_RVALUE ) );
      return genericFunctionName( "operator_bin_" + std::string( binOpCodeName(type) ), exprTypes );
    }

    std::string binOpOverloadName( BinOpType type, RC::Handle<CG::Manager> const &cgManager, std::string const &lhsAdapterName, std::string const &rhsAdapterName )
    {
      return binOpOverloadName( type, cgManager->getAdapter( lhsAdapterName ), cgManager->getAdapter( rhsAdapterName ) );
    }

    std::string methodOverloadName(
      std::string const &name,
      CG::ExprType const &thisExprType,
      std::vector<CG::ExprType> const &paramTypes
      )
    {
      std::vector<CG::ExprType> exprTypes;
      exprTypes.push_back( thisExprType );
      for ( std::vector<CG::ExprType>::const_iterator it = paramTypes.begin(); it != paramTypes.end(); ++it )
        exprTypes.push_back( *it );
      return genericFunctionName( "method_" + name, exprTypes );
    }

    std::string destructorOverloadName( RC::ConstHandle<CG::Adapter> const &thisAdapter )
    {
      std::vector<CG::ExprType> exprTypes;
      exprTypes.push_back( CG::ExprType( thisAdapter, CG::USAGE_LVALUE ) );
      return genericFunctionName( "__destructor__", exprTypes );
    }
  }
}
