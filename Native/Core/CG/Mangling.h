/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_RT_MANGLING_H
#define _FABRIC_RT_MANGLING_H

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
    
    std::string ConstructorPencilKey( RC::ConstHandle<CG::Adapter> const &thisAdapter );
    std::string ConstructorDefaultSymbolName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      AdapterVector const &paramAdapters
      );
    
    std::string DestructorPencilKey( RC::ConstHandle<CG::Adapter> const &thisAdapter );
    std::string DestructorDefaultSymbolName( RC::ConstHandle<CG::Adapter> const &thisAdapter );
    
    std::string AssignOpPencilKey(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      AssignOpType type
      );
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
  }
}

#endif //_FABRIC_RT_MANGLING_H
