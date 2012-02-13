/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

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
    
    std::string constructorOverloadName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      std::vector< RC::ConstHandle<CG::Adapter> > const &paramAdapters
      );
    inline std::string constructorOverloadName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      RC::ConstHandle<CG::Adapter> const &param1Adapter
      )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > paramAdapters;
      paramAdapters.push_back( param1Adapter );
      return constructorOverloadName( thisAdapter, paramAdapters );
    }
    inline std::string constructorOverloadName(
      RC::Handle<CG::Manager> const &cgManager,
      std::string const &thisTypeName,
      std::vector< RC::ConstHandle<CG::Adapter> > const &paramAdapters
      )
    {
      return constructorOverloadName(
        cgManager->getAdapter( thisTypeName ),
        paramAdapters
        );
    }
    inline std::string constructorOverloadName(
      RC::Handle<CG::Manager> const &cgManager,
      std::string const &dstType,
      RC::ConstHandle<CG::Adapter> const &param1Adapter
      )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > paramAdapters;
      paramAdapters.push_back( param1Adapter );
      return constructorOverloadName(
        cgManager,
        dstType,
        paramAdapters
        );
    }
    
    std::string assignOpOverloadName( AssignOpType type, RC::ConstHandle<CG::Adapter> const &thisAdapter, RC::ConstHandle<CG::Adapter> const &thatAdapter );
    std::string uniOpOverloadName( UniOpType type, RC::ConstHandle<CG::Adapter> const &adapter );
    std::string binOpOverloadName( BinOpType type, RC::ConstHandle<CG::Adapter> const &lhsAdapter, RC::ConstHandle<CG::Adapter> const &rhsAdapter );
    
    std::string methodOverloadName(
      std::string const &name,
      CG::ExprType const &thisExprType,
      std::vector<CG::ExprType> const &paramExprTypes
      );
    inline std::string methodOverloadName(
      std::string const &name,
      CG::ExprType const &thisExprType
      )
    {
      std::vector<CG::ExprType> paramExprTypes;
      return methodOverloadName( name, thisExprType, paramExprTypes );
    }
    inline std::string methodOverloadName(
      std::string const &name,
      CG::ExprType const &thisExprType,
      CG::ExprType const &param1ExprType
      )
    {
      std::vector<CG::ExprType> paramExprTypes;
      paramExprTypes.push_back( param1ExprType );
      return methodOverloadName( name, thisExprType, paramExprTypes );
    }
    inline std::string methodOverloadName(
      std::string const &name,
      CG::ExprType const &thisExprType,
      CG::ExprType const &param1ExprType,
      CG::ExprType const &param2ExprType
      )
    {
      std::vector<CG::ExprType> paramExprTypes;
      paramExprTypes.push_back( param1ExprType );
      paramExprTypes.push_back( param2ExprType );
      return methodOverloadName( name, thisExprType, paramExprTypes );
    }
    
    std::string destructorOverloadName( RC::ConstHandle<CG::Adapter> const &thisType );
  };
};

#endif //_FABRIC_RT_OVERLOAD_NAMES_H
