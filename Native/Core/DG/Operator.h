/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_OPERATOR_H
#define _FABRIC_DG_OPERATOR_H

#include <Fabric/Core/DG/NamedObject.h>
#include <Fabric/Core/DG/Prototype.h>
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/MT/ParallelCall.h>

#include <set>
#include <llvm/ADT/OwningPtr.h>

namespace llvm
{
  class LLVMContext;
  class Module;
  class Function;
  class Value;
  class ExecutionEngine;
};

namespace Fabric
{
  namespace RT
  {
    class Manager;
  };
  
  namespace CG
  {
    class Manager;
  };
  
  namespace AST
  {
    class GlobalList;
    class Operator;
  };
  
  namespace DG
  {
    class Context;
    class CompiledObject;
    class BindingList;
    class Binding;
    class Scope;
    class Code;
    class Function;
    
    class Operator : public NamedObject
    {
      friend class BindingList;
      
    public:
    
      static RC::Handle<Operator> Create( std::string const &name, RC::Handle<Context> const &context );
      
      void addBinding( Binding *binding );
      void removeBinding( Binding *binding );

      virtual bool isOperator() const { return true; }

      std::string const &getEntryFunctionName() const;
      void setEntryFunctionName( std::string const &entryFunctionName );
      
      std::string const &getFilename() const;
      std::string const &getSourceCode() const;
      void setFilenameAndSourceCode( std::string const &filename, std::string const &sourceCode );
      
#if defined(FABRIC_BUILD_DEBUG)
      std::string const &getByteCode() const;
#endif

      bool getMainThreadOnly() const
      {
        return m_mainThreadOnly;
      }
      void setMainThreadOnly( bool mainThreadOnly )
      {
        m_mainThreadOnly = mainThreadOnly;
      }
      
      RC::ConstHandle<AST::GlobalList> getAST() const;
      void setAST( RC::ConstHandle<AST::GlobalList> const &ast );
      
      CG::Diagnostics const &getDiagnostics() const;

      RC::Handle<MT::ParallelCall> bind(
        std::vector<std::string> &errors,
        Prototype *prototype,
        Scope const &scope,
        size_t *newSize,
        unsigned prefixCount,
        void * const *prefixes
        ) const;

      virtual void jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      static void jsonExecCreate( RC::ConstHandle<JSON::Value> const &arg, RC::Handle<Context> const &context, Util::JSONArrayGenerator &resultJAG );
      void jsonExecSetSourceCode( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecSetEntryFunctionName( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecSetMainThreadOnly( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonDesc( Util::JSONGenerator &resultJG ) const;
      virtual void jsonDesc( Util::JSONObjectGenerator &resultJOG ) const;
      virtual void jsonDescType( Util::JSONGenerator &resultJG ) const;
      void jsonDescFilename( Util::JSONGenerator &resultJG ) const;
      void jsonDescSourceCode( Util::JSONGenerator &resultJG ) const;
      void jsonDescEntryFunctionName( Util::JSONGenerator &resultJG ) const;
      void jsonDescDiagnostics( Util::JSONGenerator &resultJG ) const;
      void jsonDescMainThreadOnly( Util::JSONGenerator &resultJG ) const;
      
    protected:
    
      Operator( std::string const &name, RC::Handle<Context> const &context );
      ~Operator();
      
      void compile();
      void resolveEntryFunction();
      void resolveASTOperator();
    
      virtual void propagateMarkForRecompileImpl( unsigned generation );    
      virtual void propagateMarkForRefreshImpl( unsigned generation );    
      virtual void collectErrors();
      virtual void invalidateRunState();
      virtual void refreshRunState();
      virtual void collectTasksImpl( unsigned generation, MT::TaskGroupStream &taskGroupStream ) const;
      virtual bool canExecute() const;

    private:
    
      Context *m_context;
    
      std::string m_entryFunctionName;
      std::string m_filename;
      std::string m_sourceCode;
      RC::ConstHandle<Code> m_code;
      RC::ConstHandle<Function> m_function;
      bool m_mainThreadOnly;
      
      RC::ConstHandle<AST::Operator> m_astOperator;
    
      typedef std::multiset<Binding *> Bindings; 
      Bindings m_bindings;
    };
  };
};

#endif //_FABRIC_DG_OPERATOR_H
