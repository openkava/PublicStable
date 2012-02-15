/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_RT_OVERLOAD_NAMES_H
#define _FABRIC_RT_OVERLOAD_NAMES_H

#include <Fabric/Core/CG/Adapter.h>
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
    
    std::string FunctionPencilKey( std::string const &functionName );
    std::string FunctionDefaultSymbolName( std::string const &functionName, ExprTypeVector const &paramTypes );
    inline std::string FunctionDefaultSymbolName( std::string const &functionName )
    {
      ExprTypeVector paramTypes;
      return FunctionDefaultSymbolName( functionName, paramTypes );
    }
    inline std::string FunctionDefaultSymbolName( std::string const &functionName, ExprType const &paramType )
    {
      ExprTypeVector paramTypes;
      paramTypes.push_back( paramType );
      return FunctionDefaultSymbolName( functionName, paramTypes );
    }
    inline std::string FunctionDefaultSymbolName( std::string const &functionName, ExprType const &param1Type, ExprType const &param2Type )
    {
      ExprTypeVector paramTypes;
      paramTypes.push_back( param1Type );
      paramTypes.push_back( param2Type );
      return FunctionDefaultSymbolName( functionName, paramTypes );
    }
    
    std::string ConstructorPencilKey( RC::ConstHandle<CG::Adapter> const &thisAdapter );
    std::string ConstructorDefaultSymbolName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      std::vector< RC::ConstHandle<CG::Adapter> > const &paramAdapters
      );
    inline std::string ConstructorDefaultSymbolName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      RC::ConstHandle<CG::Adapter> const &param1Adapter
      )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > paramAdapters;
      paramAdapters.push_back( param1Adapter );
      return ConstructorDefaultSymbolName( thisAdapter, paramAdapters );
    }
    inline std::string ConstructorDefaultSymbolName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      RC::ConstHandle<CG::Adapter> const &param1Adapter,
      RC::ConstHandle<CG::Adapter> const &param2Adapter
      )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > paramAdapters;
      paramAdapters.push_back( param1Adapter );
      paramAdapters.push_back( param2Adapter );
      return ConstructorDefaultSymbolName( thisAdapter, paramAdapters );
    }
    inline std::string ConstructorDefaultSymbolName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      RC::ConstHandle<CG::Adapter> const &param1Adapter,
      RC::ConstHandle<CG::Adapter> const &param2Adapter,
      RC::ConstHandle<CG::Adapter> const &param3Adapter
      )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > paramAdapters;
      paramAdapters.push_back( param1Adapter );
      paramAdapters.push_back( param2Adapter );
      paramAdapters.push_back( param3Adapter );
      return ConstructorDefaultSymbolName( thisAdapter, paramAdapters );
    }
    inline std::string ConstructorDefaultSymbolName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      RC::ConstHandle<CG::Adapter> const &param1Adapter,
      RC::ConstHandle<CG::Adapter> const &param2Adapter,
      RC::ConstHandle<CG::Adapter> const &param3Adapter,
      RC::ConstHandle<CG::Adapter> const &param4Adapter
      )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > paramAdapters;
      paramAdapters.push_back( param1Adapter );
      paramAdapters.push_back( param2Adapter );
      paramAdapters.push_back( param3Adapter );
      paramAdapters.push_back( param4Adapter );
      return ConstructorDefaultSymbolName( thisAdapter, paramAdapters );
    }
    
    std::string DestructorPencilKey( RC::ConstHandle<CG::Adapter> const &thisAdapter );
    std::string DestructorDefaultSymbolName( RC::ConstHandle<CG::Adapter> const &thisAdapter );
    
    std::string AssignOpPencilKey( RC::ConstHandle<CG::Adapter> const &thisAdapter, AssignOpType type );
    std::string AssignOpDefaultSymbolName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      AssignOpType type,
      RC::ConstHandle<CG::Adapter> const &thatAdapter
      );
    
    std::string UniOpPencilKey( UniOpType type );
    std::string UniOpDefaultSymbolName(
      UniOpType type,
      RC::ConstHandle<CG::Adapter> const &adapter
      );
    
    std::string BinOpPencilKey( BinOpType type );
    std::string BinOpDefaultSymbolName(
      BinOpType type,
      RC::ConstHandle<CG::Adapter> const &lhsAdapter,
      RC::ConstHandle<CG::Adapter> const &rhsAdapter
      );
    
    std::string MethodPencilKey(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      std::string const &name
      );
    std::string MethodDefaultSymbolName(
      CG::ExprType const &thisType,
      std::string const &methodName,
      CG::ExprTypeVector const &paramTypes
      );
    inline std::string MethodDefaultSymbolName(
      CG::ExprType const &thisType,
      std::string const &methodName
      )
    {
      ExprTypeVector paramTypes;
      return MethodDefaultSymbolName( thisType, methodName, paramTypes );
    }
    inline std::string MethodDefaultSymbolName(
      CG::ExprType const &thisType,
      std::string const &methodName,
      CG::ExprType const &param1Type
      )
    {
      ExprTypeVector paramTypes;
      paramTypes.push_back( param1Type );
      return MethodDefaultSymbolName( thisType, methodName, paramTypes );
    }
    inline std::string MethodDefaultSymbolName(
      CG::ExprType const &thisType,
      std::string const &methodName,
      CG::ExprType const &param1Type,
      CG::ExprType const &param2Type
      )
    {
      CG::ExprTypeVector paramTypes;
      paramTypes.push_back( param1Type );
      paramTypes.push_back( param2Type );
      return MethodDefaultSymbolName( thisType, methodName, paramTypes );
    }
  }
}

#endif //_FABRIC_RT_OVERLOAD_NAMES_H
