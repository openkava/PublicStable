/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "FunctionBase.h"
#include <Fabric/Core/AST/ParamVector.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Base/Util/SimpleString.h>

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
      RC::ConstHandle<CompoundStatement> const &body,
      bool exportSymbol
      )
      : Global( location )
      , m_returnTypeName( returnTypeName )
      , m_body( body )
      , m_exportSymbol( exportSymbol )
    {
    }
    
    void FunctionBase::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const
    {
      Global::appendJSONMembers( jsonObjectGenerator, includeLocation );
      jsonObjectGenerator.makeMember( "returnExprType" ).makeString( m_returnTypeName );
      if ( m_body )
        m_body->appendJSON( jsonObjectGenerator.makeMember( "body" ), includeLocation );
    }
    
    RC::ConstHandle<CompoundStatement> FunctionBase::getBody() const
    {
      return m_body;
    }
    
    std::string const *FunctionBase::getFriendlyName( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return 0;
    }
    
    void FunctionBase::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      if ( !m_returnTypeName.empty() )
      {
        try
        {
          cgManager->getAdapter( m_returnTypeName );
        }
        catch ( Exception e )
        {
          addError( diagnostics, e );
        }
      }

      RC::ConstHandle<AST::ParamVector> params = getParams( cgManager );
      params->registerTypes( cgManager, diagnostics );
      
      if ( m_body )
        m_body->registerTypes( cgManager, diagnostics );
    }
    
    void FunctionBase::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
      std::string const *friendlyName = getFriendlyName( moduleBuilder.getManager() );
      if ( !buildFunctionBodies && friendlyName && moduleBuilder.getScope().has( *friendlyName ) )
      {
        addError( diagnostics, ("symbol " + _(*friendlyName) + " already exists").c_str() );
      }
      else
      {
        RC::ConstHandle<CG::Adapter> returnAdapter;
        if ( !m_returnTypeName.empty() )
        {
          returnAdapter = moduleBuilder.getAdapter( m_returnTypeName, getLocation() );
          returnAdapter->llvmCompileToModule( moduleBuilder );
        }
        
        CG::ExprType returnExprType( returnAdapter, CG::USAGE_RVALUE );
        std::string entryName = getEntryName( moduleBuilder.getManager() );
        RC::ConstHandle<AST::ParamVector> params = getParams( moduleBuilder.getManager() );
        params->llvmCompileToModule( moduleBuilder, diagnostics, buildFunctionBodies );
        CG::FunctionBuilder functionBuilder( moduleBuilder, entryName, returnExprType, params->getFunctionParams( moduleBuilder.getManager() ), m_exportSymbol, friendlyName, false );
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
              addError( diagnostics, "not all paths return a value" );
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
