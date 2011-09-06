/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/Code.h>
#include <Fabric/Core/DG/ExecutionEngine.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/OCL/OCL.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/CG/Context.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/AST/GlobalList.h>
#include <Fabric/Core/AST/Function.h>
#include <Fabric/Core/AST/Operator.h>
#include <Fabric/Core/AST/UseInfo.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/RT/Impl.h>
#include <Fabric/Core/Plug/Manager.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/DG/Function.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Core/MT/IdleTaskQueue.h>
#include <Fabric/Core/DG/IRCache.h>
#include <Fabric/Core/KL/StringSource.h>
#include <Fabric/Core/KL/Scanner.h>
#include <Fabric/Core/KL/Parser.hpp>
#include <Fabric/Core/Util/Log.h>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Assembly/Parser.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/PassManager.h>
#include <llvm/Support/StandardPasses.h>

// [pzion 20110307] Include this last because it does lots of
// #defines on Linux that mess up llvm
// #include <Fabric/Core/OGL/OGL.h>

namespace Fabric
{
  

  namespace DG
  {
    static MT::Mutex s_globalCompileLock( "Global Compile Lock" );
  
    RC::ConstHandle<Code> Code::Create( RC::ConstHandle<Context> const &context, std::string const &sourceCode )
    {
      return new Code( context, sourceCode );
    }

    Code::Code( RC::ConstHandle<Context> const &context, std::string const &sourceCode )
      : m_context( context.ptr() )
      , m_mutex( "DG::Code" )
      , m_sourceCode( sourceCode )
      , m_registeredFunctionSetMutex( "DG::Code::m_registeredFunctionSet" )
    {
      compileSourceCode();
    }
    
    Code::~Code()
    {
    }

    void Code::compileSourceCode()
    {
      MT::Mutex::Lock mutexLock( m_mutex );

      llvm::InitializeNativeTarget();
      LLVMLinkInJIT();
      
      FABRIC_ASSERT( m_sourceCode.length() > 0 );
      
      RC::ConstHandle<KL::Source> source = KL::StringSource::Create( m_sourceCode );
      RC::Handle<KL::Scanner> scanner = KL::Scanner::Create( source );
      m_ast = AST::GlobalList::Create( m_ast, KL::Parse( scanner, m_diagnostics ) );
      if ( !m_diagnostics.containsError() )
        compileAST( false );
    }
    
    void Code::compileAST( bool optimize )
    {
      MT::Mutex::Lock mutexLock( m_mutex );

      FABRIC_ASSERT( m_ast );
      RC::ConstHandle<AST::GlobalList> ast = m_ast;
      
      AST::UseNameToLocationMap uses;
      m_ast->collectUses( uses );

      std::set<std::string> includedUses;
      while ( !uses.empty() )
      {
        AST::UseNameToLocationMap::iterator it = uses.begin();
        std::string const &name = it->first;
        if ( includedUses.find( name ) == includedUses.end() )
        {
          RC::ConstHandle<AST::GlobalList> useAST = Plug::Manager::Instance()->maybeGetASTForExt( name );
          if ( !useAST )
          {
            RC::ConstHandle<RC::Object> typeAST;
            if ( m_context->getRTManager()->maybeGetASTForType( name, typeAST ) )
              useAST = typeAST? RC::ConstHandle<AST::GlobalList>::StaticCast( typeAST ): AST::GlobalList::Create();
          }
            
          if ( useAST )
          {
            useAST->collectUses( uses );
            ast = AST::GlobalList::Create( useAST, ast );
            includedUses.insert( name );
          }
          else m_diagnostics.addError( it->second, "no registered type or plugin named " + _(it->first) );
        }
        uses.erase( it );
      }

      if ( !m_diagnostics.containsError() )
      {
        RC::Handle<CG::Manager> cgManager = m_context->getCGManager();
        RC::Handle<CG::Context> cgContext = CG::Context::Create();
        llvm::OwningPtr<llvm::Module> module( new llvm::Module( "DG::Code", cgContext->getLLVMContext() ) );
        CG::ModuleBuilder moduleBuilder( cgManager, cgContext, module.get() );
        OCL::llvmPrepareModule( moduleBuilder, m_context->getRTManager() );

        CG::Diagnostics optimizeDiagnostics;
        CG::Diagnostics &diagnostics = (false && optimize)? optimizeDiagnostics: m_diagnostics;

        llvm::NoFramePointerElim = true;
        llvm::JITExceptionHandling = true;
        
        std::string irCacheKeyForAST = IRCache::Instance()->keyForAST( ast );
        std::string ir = IRCache::Instance()->get( irCacheKeyForAST );
        if ( ir.length() > 0 )
        {
          RC::Handle<CG::Manager> cgManager = m_context->getCGManager();
          
          llvm::SMDiagnostic error;
          llvm::ParseAssemblyString( ir.c_str(), module.get(), error, cgContext->getLLVMContext() );
          
          ast->registerTypes( cgManager, diagnostics );
          FABRIC_ASSERT( !diagnostics.containsError() );

          FABRIC_ASSERT( !llvm::verifyModule( *module, llvm::PrintMessageAction ) );

          linkModule( cgContext, module, true );
          
          return;
        }
        
        ast->registerTypes( cgManager, diagnostics );
        if ( !diagnostics.containsError() )
        {
          ast->llvmCompileToModule( moduleBuilder, diagnostics, false );
        }
        if ( !diagnostics.containsError() )
        {
          ast->llvmCompileToModule( moduleBuilder, diagnostics, true );
        }
        if ( !diagnostics.containsError() )
        {
#if defined(FABRIC_BUILD_DEBUG)
          std::string optimizeByteCode;
          std::string &byteCode = (false && optimize)? optimizeByteCode: m_byteCode;
          llvm::raw_string_ostream byteCodeStream( byteCode );
          module->print( byteCodeStream, 0 );
          byteCodeStream.flush();
#endif
          
          llvm::OwningPtr<llvm::PassManager> passManager( new llvm::PassManager );
          if ( optimize )
          {
            llvm::createStandardFunctionPasses( passManager.get(), 2 );
            llvm::createStandardModulePasses( passManager.get(), 2, false, true, true, true, false, llvm::createFunctionInliningPass() );
            llvm::createStandardLTOPasses( passManager.get(), true, true, false );
          }
#if defined(FABRIC_BUILD_DEBUG)
          passManager->add( llvm::createVerifierPass() );
#endif
          passManager->run( *module );
         
          if ( optimize )
          {
            std::string ir;
            llvm::raw_string_ostream irStream( ir );
            module->print( irStream, 0 );
            irStream.flush();
            IRCache::Instance()->put( irCacheKeyForAST, ir );
          }

          linkModule( cgContext, module, optimize );
        }
      }
    }
    
    void Code::linkModule( RC::Handle<CG::Context> const &cgContext, llvm::OwningPtr<llvm::Module> &module, bool optimize )
    {
      MT::Mutex::Lock mutexLock( m_mutex );

      RC::ConstHandle<ExecutionEngine> executionEngine = ExecutionEngine::Create( m_context, cgContext, module.take() );
      
      {
        MT::Mutex::Lock lock( m_registeredFunctionSetMutex );
        for ( RegisteredFunctionSet::const_iterator it=m_registeredFunctionSet.begin();
          it!=m_registeredFunctionSet.end(); ++it )
        {
          Function *function = *it;
          function->onExecutionEngineChange( executionEngine );
        }
      }
      
      m_executionEngine = executionEngine;
      
      if ( !optimize )
      {
        retain();
        MT::IdleTaskQueue::Instance()->submit( &Code::CompileOptimizedAST, this );
      }
    }
    
    std::string const &Code::getSourceCode() const
    {
      return m_sourceCode;
    }
    
#if defined(FABRIC_BUILD_DEBUG)      
    std::string const &Code::getByteCode() const
    {
      return m_byteCode;
    }
#endif
    
    RC::ConstHandle<AST::GlobalList> Code::getAST() const
    {
      return m_ast;
    }
    
    RC::ConstHandle<ExecutionEngine> Code::getExecutionEngine() const
    {
      return m_executionEngine;
    }
    
    CG::Diagnostics const &Code::getDiagnostics() const
    {
      return m_diagnostics;
    }

    void Code::registerFunction( Function *function ) const
    {
      MT::Mutex::Lock lock( m_registeredFunctionSetMutex );
      m_registeredFunctionSet.insert( function );
    }
    
    void Code::unregisterFunction( Function *function ) const
    {
      MT::Mutex::Lock lock( m_registeredFunctionSetMutex );
      RegisteredFunctionSet::iterator it = m_registeredFunctionSet.find( function );
      FABRIC_ASSERT( it != m_registeredFunctionSet.end() );
      m_registeredFunctionSet.erase( it );
    }
  };
};
