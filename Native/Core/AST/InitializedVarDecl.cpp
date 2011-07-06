/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "InitializedVarDecl.h"

#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/OverloadNames.h>

namespace Fabric
{
  namespace AST
  {
    InitializedVarDecl::InitializedVarDecl(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<CG::Adapter> const &adapter,
      RC::ConstHandle<ArgList> const &args
      )
      : VarDecl( location, name, adapter )
      , m_argList( args )
    {
    }
    
    std::string InitializedVarDecl::localDesc() const
    {
      return "InitializedVarDecl( " + VarDecl::localDesc() + ", " + m_argList->localDesc() + " )";
    }
    
    std::string InitializedVarDecl::deepDesc( std::string const &indent ) const
    {
      return indent + localDesc() + "\n"
        + m_argList->deepDesc( indent + "  " );
    }

    void InitializedVarDecl::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      CG::ExprValue result = VarDecl::llvmAllocateVariable( basicBlockBuilder, diagnostics );
      
      std::vector< RC::ConstHandle<CG::Adapter> > argTypes;
      m_argList->appendTypes( basicBlockBuilder, argTypes );
      
      std::string initializerName = constructOverloadName( result.getAdapter(), argTypes );
        
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
        m_argList->appendExprValues( basicBlockBuilder, argUsages, exprValues, "cannot be used as an io argument" );
        
        functionSymbol->llvmCreateCall( basicBlockBuilder, exprValues );
      }
    }
  };
};
