/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Function.h"
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/CG/Scope.h>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/PassManager.h>

namespace Fabric
{
  namespace AST
  {
    Function::Function(
        CG::Location const &location,
        std::string const &name,
        CG::ExprType const &returnExprType,
        RC::ConstHandle<ParamList> const &params,
        RC::ConstHandle<CompoundStatement> const &body
        )
      : Global( location )
      , m_name( name )
      , m_returnExprType( returnExprType )
      , m_params( params )
      , m_body( body )
    {
    }
    
    std::string Function::localDesc() const
    {
      return "Function( '" + m_name + "', " + m_returnExprType.desc() + " )";
    }
    
    std::string Function::deepDesc( std::string const &indent ) const
    {
      std::string subIndent = indent + "  ";
      return indent + localDesc() + "\n"
        + m_params->deepDesc(subIndent)
        + m_body->deepDesc(subIndent);
    }
    
    RC::ConstHandle<ParamList> Function::getParamList() const
    {
      return m_params;
    }

    RC::ConstHandle<CompoundStatement> Function::getBody() const
    {
      return m_body;
    }
    
    void Function::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
      if ( !buildFunctionBodies && moduleBuilder.getScope().has( m_name ) )
      {
        addError( diagnostics, "symbol '" + m_name + "' already exists" );
      }
      else
      {
        CG::FunctionBuilder functionBuilder( moduleBuilder, m_name, m_returnExprType, m_params->getFunctionParams() );
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
