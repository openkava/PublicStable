#include "OverloadNames.h"
#include "Adapter.h"

namespace Fabric
{
  namespace CG
  {
    static std::string genericFunctionName( std::string const &genericFunctionName, std::vector< RC::ConstHandle<CG::Adapter> > const &paramExprTypes )
    {
      std::string result = "__" + genericFunctionName;
      for ( size_t i=0; i<paramExprTypes.size(); ++i )
        result += "__" + paramExprTypes[i]->getCodeName();
      return result;
    }
    
    std::string constructOverloadName( RC::ConstHandle<CG::Adapter> const &thisType, std::vector< RC::ConstHandle<CG::Adapter> > const &paramExprTypes )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > exprTypes;
      exprTypes.push_back( thisType );
      for ( std::vector< RC::ConstHandle<CG::Adapter> >::const_iterator it = paramExprTypes.begin(); it != paramExprTypes.end(); ++it )
        exprTypes.push_back( *it );
      return genericFunctionName( "constructor", exprTypes );
    }
    
    std::string constructOverloadName( RC::Handle<CG::Manager> const &cgManager, std::string const &dstType, std::vector< RC::ConstHandle<CG::Adapter> > const &paramTypes )
    {
      return constructOverloadName( cgManager->getAdapter( dstType ), paramTypes );
    }

    std::string assignOpMethodName( AssignOpType type )
    {
      return std::string( assignOpTypeDesc(type) );
    }

    std::string uniOpOverloadName( UniOpType type, RC::ConstHandle< CG::Adapter > const &adapter )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > exprTypes;
      exprTypes.push_back( adapter );
      return genericFunctionName( "operator_" + std::string( uniOpCodeName(type) ), exprTypes );
    }

    std::string binOpOverloadName( BinOpType type, RC::ConstHandle< CG::Adapter > const &lhsAdapter, RC::ConstHandle< CG::Adapter > const &rhsAdapter )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > exprTypes;
      exprTypes.push_back( lhsAdapter );
      exprTypes.push_back( rhsAdapter );
      return genericFunctionName( "operator_" + std::string( binOpCodeName(type) ), exprTypes );
    }

    std::string binOpOverloadName( BinOpType type, RC::Handle<CG::Manager> const &cgManager, std::string const &lhsAdapterName, std::string const &rhsAdapterName )
    {
      return binOpOverloadName( type, cgManager->getAdapter( lhsAdapterName ), cgManager->getAdapter( rhsAdapterName ) );
    }

    std::string methodOverloadName( std::string const &name, RC::ConstHandle<CG::Adapter> const &thisType, std::vector< RC::ConstHandle<CG::Adapter> > const &paramTypes )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > exprTypes;
      exprTypes.push_back( thisType );
      for ( std::vector< RC::ConstHandle<CG::Adapter> >::const_iterator it = paramTypes.begin(); it != paramTypes.end(); ++it )
        exprTypes.push_back( *it );
      return genericFunctionName( "method_" + name, exprTypes );
    }

    std::string destructorOverloadName( RC::ConstHandle<CG::Adapter> const &thisType )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > exprTypes;
      exprTypes.push_back( thisType );
      return genericFunctionName( "__destructor__", exprTypes );
    }
  }
}
