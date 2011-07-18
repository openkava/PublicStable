/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "FunctionBase.h"
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
    FunctionBase::FunctionBase(
        CG::Location const &location,
        std::string const &returnTypeName,
        RC::ConstHandle<ParamVector> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        )
      : Global( location )
      , m_returnTypeName( returnTypeName )
      , m_params( params )
      , m_body( body )
    {
    }
    
    RC::Handle<JSON::Object> Function::toJSON() const
    {
      RC::Handle<JSON::Object> result = Global::toJSON();
      result->set( "returnExprType", JSON::String::Create( m_returnTypeName ) );
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
      std::string const *friendlyName = getFriendlyName();
      if ( !buildFunctionBodies && friendlyName && moduleBuilder.getScope().has( *friendlyName ) )
      {
        addError( diagnostics, "symbol " + _(*friendlyName) + " already exists" );
      }
      else
      {
        RC::ConstHandle<Adapter> returnAdapter = moduleBuilder->maybeGetAdapter( m_returnTypeName );
        if ( !returnAdapter )
          addError( diagnostics, _(m_returnTypeName) + ": type not found" );
        else
        {
          CG::ExprType returnExprType( returnAdapter, CG::USAGE_RVALUE );
          CG::FunctionBuilder functionBuilder( moduleBuilder, m_entryName, returnExprType, m_params->getFunctionParams(), friendlyName );
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
    }
  };
};
