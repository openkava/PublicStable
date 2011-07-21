#ifndef _FABRIC_RT_OVERLOAD_NAMES_H
#define _FABRIC_RT_OVERLOAD_NAMES_H

#include <Fabric/Core/CG/ExprType.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/OpTypes.h>
#include <string>
#include <vector>

namespace Fabric
{
  namespace CG
  {
    class Adapter;
    
    std::string constructOverloadName( RC::ConstHandle<CG::Adapter> const &dstAdapter, std::vector< RC::ConstHandle<CG::Adapter> > const &paramTypes );
    inline std::string constructOverloadName( RC::ConstHandle<CG::Adapter> const &dstAdapter, RC::ConstHandle<CG::Adapter> const &param1Type )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > paramTypes;
      paramTypes.push_back( param1Type );
      return constructOverloadName( dstAdapter, paramTypes );
    }
    std::string constructOverloadName( RC::Handle<CG::Manager> const &cgManager, std::string const &dstType, std::vector< RC::ConstHandle<CG::Adapter> > const &paramTypes );
    inline std::string constructOverloadName( RC::Handle<CG::Manager> const &cgManager, std::string const &dstType, RC::ConstHandle<CG::Adapter> const &param1Type )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > paramTypes;
      paramTypes.push_back( param1Type );
      return constructOverloadName( cgManager, dstType, paramTypes );
    }
    
    std::string assignOpMethodName( AssignOpType type );
    std::string uniOpOverloadName( UniOpType type, RC::ConstHandle< CG::Adapter > const &adapter );
    std::string binOpOverloadName( BinOpType type, RC::ConstHandle< CG::Adapter > const &lhsAdapter, RC::ConstHandle<CG::Adapter> const &rhsAdapter );
    
    std::string methodOverloadName( std::string const &name, RC::ConstHandle<CG::Adapter> const &selfType, std::vector< RC::ConstHandle<CG::Adapter> > const &paramTypes );
    inline std::string methodOverloadName( std::string const &name, RC::ConstHandle<CG::Adapter> const &selfType )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > paramTypes;
      return methodOverloadName( name, selfType, paramTypes );
    }
    inline std::string methodOverloadName( std::string const &name, RC::ConstHandle<CG::Adapter> const &selfType, RC::ConstHandle<CG::Adapter> const &param1Type )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > paramTypes;
      paramTypes.push_back( param1Type );
      return methodOverloadName( name, selfType, paramTypes );
    }
  };
};

#endif //_FABRIC_RT_OVERLOAD_NAMES_H
