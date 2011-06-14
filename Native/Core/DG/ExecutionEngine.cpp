/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/ExecutionEngine.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Core/Util/UnorderedMap.h>
#include <Fabric/Core/Plug/Manager.h>
#include <Fabric/Core/CG/Manager.h>
#if defined(FABRIC_MODULE_OCL)
# include <Fabric/Core/OCL/OCL.h>
#endif

#include <llvm/ExecutionEngine/JIT.h>

// [pzion 20110307] Include this last because it does lots of
// #defines on Linux that mess up llvm
#include <Fabric/Core/OGL/OGL.h>

#if defined( FABRIC_OS_WINDOWS )
extern "C" void _chkstk( );
#endif

namespace Fabric
{
  namespace DG
  {
    extern "C" void logFromByteCode( const char *format, ... )
    {
      char buffer[4096];
      va_list argList;
      
      va_start( argList, format );
      vsnprintf( buffer, 4096, format, argList );
      va_end( argList );
      
      MT::tlsLogCollector.get()->add( buffer );
    }
    
#if defined(FABRIC_OS_WINDOWS)
    static float imp_roundf( float x )
    {
      return( floorf( x + 0.5f ) );
    }
#endif

    RC::ConstHandle<Context> ExecutionEngine::s_currentContext;
    
    void ExecutionEngine::Report( char const *data, size_t length )
    {
      s_currentContext->getLogCollector()->add( data, length );
    }
    
    void *ExecutionEngine::LazyFunctionCreator( std::string const &functionName )
    {
      static Util::UnorderedMap< std::string, void * > *functionMapPtr = 0;
      if ( !functionMapPtr )
      {
        functionMapPtr = new Util::UnorderedMap< std::string, void * >;
        Util::UnorderedMap< std::string, void * > &functionMap = *functionMapPtr;
        
        functionMap["acosf"] = (void *)&acosf;
        functionMap["asinf"] = (void *)&asinf;
        functionMap["atanf"] = (void *)&atanf;
        functionMap["atan2f"] = (void *)&atan2f;
        functionMap["cosf"] = (void *)&cosf;
        functionMap["fabsf"] = (void *)&fabsf;
        functionMap["ceilf"] = (void *)&ceilf;
        functionMap["floorf"] = (void *)&floorf;
        functionMap["logf"] = (void *)&logf;
        functionMap["powf"] = (void *)&powf;
#if defined(FABRIC_OS_WINDOWS)
        functionMap["roundf"] = (void *)&imp_roundf;
#else
        functionMap["roundf"] = (void *)&roundf;
#endif
        functionMap["sinf"] = (void *)&sinf;
        functionMap["sqrtf"] = (void *)&sqrtf;
        functionMap["tanf"] = (void *)&tanf;

        functionMap["report"] = (void *)&ExecutionEngine::Report;
        functionMap["malloc"] = (void *)&malloc;
        functionMap["free"] = (void *)&free;
#if defined(FABRIC_OS_WINDOWS)
        functionMap["_chkstk"] = (void *)&_chkstk;
#endif
      }
      Util::UnorderedMap< std::string, void * >::const_iterator it = functionMapPtr->find( functionName );
      if ( it != functionMapPtr->end() )
        return it->second;
      else
      {
        void *result = 0;
        if ( !result )
          result = s_currentContext->getPlugManager()->llvmResolveExternalFunction( functionName );
        if ( !result )
          result = OGL::llvmResolveExternalFunction( functionName );
#if defined(FABRIC_MODULE_OCL)
        if ( !result )
          result = OCL::llvmResolveExternalFunction( functionName );
#endif
        if ( !result )
          result = s_currentContext->getCGManager()->llvmResolveExternalFunction( functionName );

        // We should *always* return a valid symbol. Otherwise something's
        // wrong in the KL compiler/support.
        if( !result )
          throw Exception( "LLVM lookup failed for symbol: " + functionName );

        return result;
      }
    }

    RC::ConstHandle<ExecutionEngine> ExecutionEngine::Create( RC::ConstHandle<Context> const &context, llvm::Module *llvmModule )
    {
      return new ExecutionEngine( context, llvmModule );
    }
    
    ExecutionEngine::ExecutionEngine( RC::ConstHandle<Context> const &context, llvm::Module *llvmModule )
      : m_context( context )
    {
      std::string errStr;
      m_llvmExecutionEngine.reset(
        llvm::EngineBuilder(llvmModule)
          .setErrorStr(&errStr)
          .setEngineKind(llvm::EngineKind::JIT)
          .create()
        );
      if ( !m_llvmExecutionEngine )
        throw Exception( "Failed to construct ExecutionEngine: " + errStr );

      m_llvmExecutionEngine->InstallLazyFunctionCreator( &LazyFunctionCreator );
      
      // Make sure we don't search loaded libraries for missing symbols. 
      // Only symbols *we* provide should be available as calling functions.
      m_llvmExecutionEngine->DisableSymbolSearching();
      m_context->getCGManager()->llvmAddGlobalMappingsToExecutionEngine( m_llvmExecutionEngine.get(), *llvmModule );
    }
    
    ExecutionEngine::GenericFunctionPtr ExecutionEngine::getFunctionByName( std::string const &functionName ) const
    {
      GenericFunctionPtr result = 0;
      ContextSetter contextSetter( m_context );
      llvm::Function *llvmFunction = m_llvmExecutionEngine->FindFunctionNamed( functionName.c_str() );
      if ( llvmFunction )
        result = GenericFunctionPtr( m_llvmExecutionEngine->getPointerToFunction( llvmFunction ) );
      return result;
    }

    ExecutionEngine::ContextSetter::ContextSetter( RC::ConstHandle<Context> const &context )
    {
      m_oldContext = s_currentContext;
      s_currentContext = context;
    }
    
    ExecutionEngine::ContextSetter::~ContextSetter()
    {
      s_currentContext = m_oldContext;
    }
  };
};
