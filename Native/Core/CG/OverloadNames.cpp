#include "OverloadNames.h"
#include "Adapter.h"

namespace Fabric
{
  namespace CG
  {
    std::string constructOverloadName( RC::ConstHandle<CG::Adapter> const &dstAdapter, std::vector< RC::ConstHandle<CG::Adapter> > const &paramExprTypes )
    {
      std::string result = "__construct_" + dstAdapter->getCodeName();
      for ( size_t i=0; i<paramExprTypes.size(); ++i )
        result += "__" + paramExprTypes[i]->getCodeName();
      return result;
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
      return "__operator_" + std::string( uniOpCodeName(type) ) + "__" + adapter->getCodeName();
    }

    std::string binOpOverloadName( BinOpType type, RC::ConstHandle< CG::Adapter > const &lhsAdapter, RC::ConstHandle< CG::Adapter > const &rhsAdapter )
    {
      return "__operator_" + binOpCodeName(type) + "__" + lhsAdapter->getCodeName() + "__" + rhsAdapter->getCodeName();
    }

    std::string binOpOverloadName( BinOpType type, RC::Handle<CG::Manager> const &cgManager, std::string const &lhsAdapterName, std::string const &rhsAdapterName )
    {
      return binOpOverloadName( type, cgManager->getAdapter( lhsAdapterName ), cgManager->getAdapter( rhsAdapterName ) );
    }

    std::string methodOverloadName( std::string const &name, RC::ConstHandle<CG::Adapter> const &thisType, std::vector< RC::ConstHandle<CG::Adapter> > const &paramTypes )
    {
      std::string result = "__method_" + name + "__" + thisType->getCodeName();
      for ( size_t i=0; i<paramTypes.size(); ++i )
      {
        std::string codeName = paramTypes[i]->getCodeName();
        
        result += "__" + codeName;
      }
      return result;
    }

    std::string destructorOverloadName( RC::ConstHandle<CG::Adapter> const &thisType )
    {
      std::string result = "__destructor__" + thisType->getCodeName();
    }
  };
};
