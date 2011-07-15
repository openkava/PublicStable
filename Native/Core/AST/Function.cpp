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
        std::string const &friendlyName,
        std::string const &entryName,
        CG::ExprType const &returnExprType,
        RC::ConstHandle<ParamList> const &params,
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
    
    std::string Function::localDesc() const
    {
      return "Function( '" + m_friendlyName + "' ('" + m_entryName + "'), " + m_returnExprType.desc() + " )";
    }
    
    std::string Function::deepDesc( std::string const &indent ) const
    {
      std::string subIndent = indent + "  ";
      std::string result = indent + localDesc() + "\n"
        + m_params->deepDesc(subIndent);
      if ( m_body )
        result += m_body->deepDesc(subIndent);
      return result;
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
