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
    
    inline std::string FunctionPencilName( std::string const &functionName )
    {
      return "function " + functionName;
    }
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
    
    inline std::string ConstructorPencilName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter
      )
    {
      return "constructor " + thisAdapter->getUserName();
    }
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
      RC::Handle<CG::Manager> const &cgManager,
      std::string const &thisTypeName,
      std::vector< RC::ConstHandle<CG::Adapter> > const &paramAdapters
      )
    {
      return ConstructorDefaultSymbolName(
        cgManager->getAdapter( thisTypeName ),
        paramAdapters
        );
    }
    inline std::string ConstructorDefaultSymbolName(
      RC::Handle<CG::Manager> const &cgManager,
      std::string const &dstType,
      RC::ConstHandle<CG::Adapter> const &param1Adapter
      )
    {
      std::vector< RC::ConstHandle<CG::Adapter> > paramAdapters;
      paramAdapters.push_back( param1Adapter );
      return ConstructorDefaultSymbolName(
        cgManager,
        dstType,
        paramAdapters
        );
    }
    
    inline std::string DestructorPencilName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter
      )
    {
      return "destructor ~" + thisAdapter->getUserName();
    }
    std::string DestructorDefaultSymbolName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter
      );
    
    inline std::string AssignOpPencilName( RC::ConstHandle<CG::Adapter> const &thisAdapter, AssignOpType type )
    {
      return "assignment operator " + thisAdapter->getUserName() + "." + assignOpUserName(type);
    }
    
    std::string AssignOpDefaultSymbolName( RC::ConstHandle<CG::Adapter> const &thisAdapter, AssignOpType type, RC::ConstHandle<CG::Adapter> const &thatAdapter );
    
    inline std::string UniOpPencilName( UniOpType type )
    {
      return "unary operator " + uniOpUserName(type);
    }
    
    std::string UniOpDefaultSymbolName( UniOpType type, RC::ConstHandle<CG::Adapter> const &adapter );
    
    inline std::string BinOpPencilName( BinOpType type )
    {
      return "binary operator " + binOpUserName(type);
    }
    
    std::string BinOpDefaultSymbolName( BinOpType type, RC::ConstHandle<CG::Adapter> const &lhsAdapter, RC::ConstHandle<CG::Adapter> const &rhsAdapter );
    
    inline std::string MethodPencilName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      std::string const &name
      )
    {
      return "method " + thisAdapter->getUserName() + "." + name;
    }
    
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
