/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "FunctionBase.h"
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Manager.h>
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
        RC::ConstHandle<CompoundStatement> const &body
        )
      : Global( location )
      , m_returnTypeName( returnTypeName )
      , m_body( body )
    {
    }
    
    RC::Handle<JSON::Object> FunctionBase::toJSON() const
    {
      RC::Handle<JSON::Object> result = Global::toJSON();
      result->set( "returnExprType", JSON::String::Create( m_returnTypeName ) );
      result->set( "body", m_body->toJSON() );
      return result;
    }
    
    RC::ConstHandle<CompoundStatement> FunctionBase::getBody() const
    {
      return m_body;
    }
    
    std::string const *FunctionBase::getFriendlyName( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return 0;
    }
    
    void FunctionBase::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
      std::string const *friendlyName = getFriendlyName( moduleBuilder.getManager() );
      if ( !buildFunctionBodies && friendlyName && moduleBuilder.getScope().has( *friendlyName ) )
      {
        addError( diagnostics, "symbol " + _(*friendlyName) + " already exists" );
      }
      else
      {
        RC::ConstHandle<CG::Adapter> returnAdapter;
        if ( !m_returnTypeName.empty() )
          returnAdapter = moduleBuilder.maybeGetAdapter( m_returnTypeName );
        
        CG::ExprType returnExprType( returnAdapter, CG::USAGE_RVALUE );
        std::string entryName = getEntryName( moduleBuilder.getManager() );
        CG::FunctionBuilder functionBuilder( moduleBuilder, entryName, returnExprType, getParams( moduleBuilder.getManager() )->getFunctionParams( moduleBuilder.getManager() ), friendlyName );
        if ( buildFunctionBodies && m_body )
        {
          CG::BasicBlockBuilder basicBlockBuilder( functionBuilder );

          llvm::BasicBlock *basicBlock = functionBuilder.createBasicBlock( "entry" );
          basicBlockBuilder->SetInsertPoint( basicBlock );
          m_body->llvmCompileToBuilder( basicBlockBuilder, diagnostics );

          llvm::BasicBlock *bb = basicBlockBuilder->GetInsertBlock();
          if ( !bb->getTerminator() )
          {
            if ( returnExprType )
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
