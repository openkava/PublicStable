/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/InitializedVarDecl.h>
#include <Fabric/Core/AST/ExprVector.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( InitializedVarDecl );
    
    RC::Handle<InitializedVarDecl> InitializedVarDecl::Create(
      CG::Location const &location,
      std::string const &name,
      std::string const &arrayModifier,
      RC::ConstHandle<ExprVector> const &args
      )
    {
      return new InitializedVarDecl( location, name, arrayModifier, args );
    }
    
    InitializedVarDecl::InitializedVarDecl(
      CG::Location const &location,
      std::string const &name,
      std::string const &arrayModifier,
      RC::ConstHandle<ExprVector> const &args
      )
      : VarDecl( location, name, arrayModifier )
      , m_args( args )
    {
    }
    
    RC::Handle<JSON::Object> InitializedVarDecl::toJSON() const
    {
      RC::Handle<JSON::Object> result = VarDecl::toJSON();
      result->set( "args", m_args->toJSON() );
      return result;
    }
    
    void InitializedVarDecl::llvmPrepareModule( std::string const &baseType, CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      VarDecl::llvmPrepareModule( baseType, moduleBuilder, diagnostics );
      m_args->llvmPrepareModule( moduleBuilder, diagnostics );
    }

    void InitializedVarDecl::llvmCompileToBuilder( std::string const &baseType, CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      CG::ExprValue result = VarDecl::llvmAllocateVariable( baseType, basicBlockBuilder, diagnostics );
      
      std::vector< RC::ConstHandle<CG::Adapter> > argTypes;
      m_args->appendTypes( basicBlockBuilder, argTypes );
      
      RC::ConstHandle<CG::Adapter> adapter = result.getAdapter();
      std::string initializerName = constructOverloadName( adapter, argTypes );
        
      RC::ConstHandle<CG::FunctionSymbol> functionSymbol = basicBlockBuilder.maybeGetFunction( initializerName );
      if ( !functionSymbol )
        addError( diagnostics, "initializer " + _(initializerName) + " not found" );
      else
      {
        std::vector<CG::FunctionParam> const functionParams = functionSymbol->getParams();
        
        std::vector<CG::Usage> argUsages;
        for ( size_t i=1; i<functionParams.size(); ++i )
          argUsages.push_back( functionParams[i].getUsage() );
          
        std::vector<CG::ExprValue> exprValues;
        exprValues.push_back( result );
        m_args->appendExprValues( basicBlockBuilder, argUsages, exprValues, "cannot be used as an io argument" );
        
        functionSymbol->llvmCreateCall( basicBlockBuilder, exprValues );
      }
    }
  };
};
