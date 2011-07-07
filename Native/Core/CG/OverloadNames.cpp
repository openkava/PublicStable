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

    std::string methodOverloadName( std::string const &name, RC::ConstHandle<CG::Adapter> const &selfType, std::vector< RC::ConstHandle<CG::Adapter> > const &paramTypes )
    {
      std::string result = "__method_" + name + "__" + selfType->getCodeName();
      for ( size_t i=0; i<paramTypes.size(); ++i )
      {
        // [pzion 20110706] Super big hack to auto cast ConstStringNN to String for methods
        // Will go away when we have true polymorphic functions
        std::string codeName;
        if ( RT::isConstString( paramTypes[i]->getType() ) )
          codeName = "String";
        else codeName = paramTypes[i]->getCodeName();
        
        result += "__" + codeName;
      }
      return result;
    }
  };
};
