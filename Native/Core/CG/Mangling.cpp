/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/CG/Mangling.h>
#include "Adapter.h"

namespace Fabric
{
  namespace CG
  {
    static inline char const *EncodeUsage( CG::Usage usage )
    {
      switch ( usage )
      {
        case CG::USAGE_LVALUE:
          return "io";
        case CG::USAGE_RVALUE:
          return "in";
        default:
          FABRIC_ASSERT( false );
          return 0;
      }
    }
    
    static std::string EncodeParametersForDefaultSymbolName( ExprTypeVector const &paramTypes )
    {
      std::string result;
      for ( ExprTypeVector::const_iterator it=paramTypes.begin(); it!=paramTypes.end(); ++it )
        result += "__" + std::string( EncodeUsage( it->getUsage() ) ) + "_" + it->getAdapter()->getCodeName();
      return result;
    }

    std::string FunctionPencilKey( std::string const &functionName )
    {
      return "f:" + functionName;
    }

    std::string FunctionDefaultSymbolName( std::string const &functionName, ExprTypeVector const &paramTypes )
    {
      return "__function_" + functionName + EncodeParametersForDefaultSymbolName( paramTypes );
    }

    std::string ConstructorPencilKey( RC::ConstHandle<CG::Adapter> const &thisAdapter )
    {
      return "c:" + thisAdapter->getCodeName();
    }
    
    std::string ConstructorDefaultSymbolName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      AdapterVector const &otherParamAdapters
      )
    {
      CG::ExprTypeVector paramTypes;
      paramTypes.push_back( CG::ExprType( thisAdapter, CG::USAGE_LVALUE ) );
      for ( AdapterVector::const_iterator it = otherParamAdapters.begin(); it != otherParamAdapters.end(); ++it )
        paramTypes.push_back( CG::ExprType( *it, CG::USAGE_RVALUE ) );
      return "__constructor" + EncodeParametersForDefaultSymbolName( paramTypes );
    }

    std::string DestructorPencilKey( RC::ConstHandle<CG::Adapter> const &thisAdapter )
    {
      return "d:" + thisAdapter->getCodeName();
    }
    
    std::string DestructorDefaultSymbolName( RC::ConstHandle<CG::Adapter> const &thisAdapter )
    {
      CG::ExprTypeVector paramTypes;
      paramTypes.push_back( CG::ExprType( thisAdapter, CG::USAGE_LVALUE ) );
      return "__destructor" + EncodeParametersForDefaultSymbolName( paramTypes );
    }

    std::string AssignOpPencilKey( RC::ConstHandle<CG::Adapter> const &thisAdapter, AssignOpType type )
    {
      return "a:" + thisAdapter->getCodeName() + ":" + assignOpCodeName(type);
    }
    
    std::string AssignOpDefaultSymbolName(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      AssignOpType type,
      RC::ConstHandle<CG::Adapter> const &thatAdapter
      )
    {
      CG::ExprTypeVector paramTypes;
      paramTypes.push_back( CG::ExprType( thisAdapter, CG::USAGE_LVALUE ) );
      paramTypes.push_back( CG::ExprType( thatAdapter, CG::USAGE_RVALUE ) );
      return "__assop_" + assignOpCodeName( type ) + EncodeParametersForDefaultSymbolName( paramTypes );
    }

    std::string UniOpPencilKey( UniOpType type )
    {
      return "u:" + uniOpCodeName(type);
    }
    
    std::string UniOpDefaultSymbolName( UniOpType type, RC::ConstHandle<CG::Adapter> const &adapter )
    {
      CG::ExprTypeVector paramTypes;
      paramTypes.push_back( CG::ExprType( adapter, CG::USAGE_RVALUE ) );
      return "__uniop_" + uniOpCodeName(type) + EncodeParametersForDefaultSymbolName( paramTypes );
    }

    std::string BinOpPencilKey( BinOpType type )
    {
      return "b:" + binOpCodeName(type);
    }
    
    std::string BinOpDefaultSymbolName( BinOpType type, RC::ConstHandle<CG::Adapter> const &lhsAdapter, RC::ConstHandle<CG::Adapter> const &rhsAdapter )
    {
      CG::ExprTypeVector paramTypes;
      paramTypes.push_back( CG::ExprType( lhsAdapter, CG::USAGE_RVALUE ) );
      paramTypes.push_back( CG::ExprType( rhsAdapter, CG::USAGE_RVALUE ) );
      return "__binop_" + binOpCodeName(type) + EncodeParametersForDefaultSymbolName( paramTypes );
    }

    std::string MethodPencilKey(
      RC::ConstHandle<CG::Adapter> const &thisAdapter,
      std::string const &name
      )
    {
      return "m:" + thisAdapter->getCodeName() + ":" + name;
    }
    
    std::string MethodDefaultSymbolName(
      CG::ExprType const &thisType,
      std::string const &methodName,
      CG::ExprTypeVector const &otherParamTypes
      )
    {
      CG::ExprTypeVector paramTypes;
      paramTypes.push_back( thisType );
      for ( CG::ExprTypeVector::const_iterator it=otherParamTypes.begin(); it!=otherParamTypes.end(); ++it )
        paramTypes.push_back( *it );
      return "__method_" + methodName + EncodeParametersForDefaultSymbolName( paramTypes );
    }
  }
}
