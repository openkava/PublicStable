/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Function.h"
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Array.h>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/PassManager.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Function );
    
    RC::Handle<Function> Function::Create(
      CG::Location const &location,
      std::string const &friendlyName,
      std::string const &entryName,
      CG::ExprType const &returnExprType,
      RC::ConstHandle<ParamVector> const &params,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new Function( location, friendlyName, entryName, returnExprType, params, body );
    }
    
    Function::Function(
        CG::Location const &location,
        std::string const &friendlyName,
        std::string const &entryName,
        CG::ExprType const &returnExprType,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        )
      : Global( location )
      , m_friendlyName( friendlyName )
      , m_entryName( entryName )
      , m_returnExprType( returnExprType )
      , m_params( params )
      , m_body( body )
    {
    }
    
    RC::Handle<JSON::Object> Function::toJSON() const
    {
      RC::Handle<JSON::Object> result = Global::toJSON();
      result->set( "friendlyName", JSON::String::Create( m_friendlyName ) );
      result->set( "entryName", JSON::String::Create( m_entryName ) );
      result->set( "returnExprType", JSON::String::Create( m_returnExprType.getUserName() ) );
      result->set( "friendlyName", JSON::String::Create( m_friendlyName ) );
      result->set( "params", m_params->toJSON() );
      result->set( "body", m_body->toJSON() );
      return result;
    }
    
    RC::ConstHandle<ParamVector> Function::getParams() const
    {
      return m_params;
    }

    RC::ConstHandle<CompoundStatement> Function::getBody() const
    {
      return m_body;
    }
    
    void Function::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
      if ( !buildFunctionBodies && m_friendlyName.length()>0 && moduleBuilder.getScope().has( m_friendlyName ) )
      {
        addError( diagnostics, "symbol '" + m_entryName + "' already exists" );
      }
      else
      {
        CG::FunctionBuilder functionBuilder( moduleBuilder, m_entryName, m_returnExprType, m_params->getFunctionParams(), m_friendlyName.length()>0? &m_friendlyName: 0 );
        if ( buildFunctionBodies && m_body )
        {
          CG::BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::BasicBlock *basicBlock = functionBuilder.createBasicBlock( "entry" );
          basicBlockBuilder->SetInsertPoint( basicBlock );
          m_body->llvmCompileToBuilder( basicBlockBuilder, diagnostics );

          llvm::BasicBlock *bb = basicBlockBuilder->GetInsertBlock();
          if ( !bb->getTerminator() )
          {
            if ( m_returnExprType )
              addError( diagnostics, Exception("not all paths return a value") );
            else
            {
              functionBuilder.getScope().llvmUnwind( basicBlockBuilder );
              basicBlockBuilder->CreateRetVoid();
            }
          }
        }
      }
    }
  };
};
