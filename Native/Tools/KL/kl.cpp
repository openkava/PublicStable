/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <stdio.h>
#include <Fabric/Core/KL/Compiler.h>
#include <Fabric/Core/KL/Parser.hpp>
#include <Fabric/Core/KL/Scanner.h>
#include <Fabric/Core/KL/StringSource.h>
#include <Fabric/Core/AST/GlobalList.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/FloatDesc.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/RT/OpaqueDesc.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/OCL/OCL.h>
#include <Fabric/Core/OCL/Debug.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Base/JSON/Encode.h>

#include <memory>

#undef DestroyAll
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Target/TargetRegistry.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/Target/TargetData.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/PassManager.h>
#include <llvm/Support/StandardPasses.h>
#include <llvm/Support/raw_ostream.h>

#include <getopt.h>

using namespace Fabric;
using namespace Fabric::KL;

enum RunFlags
{
  RF_Run         = 1 << 0,
  RF_Verbose     = 1 << 1,
  RF_ShowASM     = 1 << 2,
  RF_ShowAST     = 1 << 3,
  RF_ShowBison   = 1 << 4,
  RF_ShowIR      = 1 << 5,
  RF_ShowOptIR   = 1 << 6,
  RF_ShowOptASM	 = 1 << 7,
  RF_ShowTokens	 = 1 << 8
};


void dumpDiagnostics( CG::Diagnostics const &diagnostics )
{
  for ( CG::Diagnostics::const_iterator it=diagnostics.begin(); it!=diagnostics.end(); ++it )
  {
    CG::Location const &location = it->first;
    CG::Diagnostic const &diagnostic = it->second;
    printf( "%u:%u: %s: %s\n", (unsigned)location.getLine(), (unsigned)location.getColumn(), diagnostic.getLevelDesc(), diagnostic.getDesc().c_str() );
  }
}

#if defined(FABRIC_OS_WINDOWS)

extern "C" void _chkstk( );

static float imp_roundf( float x )
{
  return( floorf( x + 0.5f ) );
}
#endif

RC::Handle<CG::Manager> cgManager;

static float externalSquare( float value )
{
  return value * value;
}

static void report( char const *data, size_t length )
{
  fwrite( data, 1, length, stdout );
  fputc( '\n', stdout );
  fflush( stdout );
}

static void *LazyFunctionCreator( std::string const &functionName )
{
  //fprintf( stderr, "LazyFunctionCreator('%s')\n", functionName.c_str() );
  if ( functionName == "report" )
    return (void *)&report;
  else if ( functionName == "malloc" )
    return (void *)&malloc;
  else if ( functionName == "realloc" )
    return (void *)&realloc;
  else if ( functionName == "free" )
    return (void *)&free;
  else if ( functionName == "acosf" )
    return (void *)&acosf;
  else if ( functionName == "asinf" )
    return (void *)&asinf;
  else if ( functionName == "atanf" )
    return (void *)&atanf;
  else if ( functionName == "atan2f" )
    return (void *)&atan2f;
  else if ( functionName == "sinf" )
    return (void *)&sinf;
  else if ( functionName == "cosf" )
    return (void *)&cosf;
  else if ( functionName == "tanf" )
    return (void *)&tanf;
  else if ( functionName == "powf" )
    return (void *)&powf;
  else if ( functionName == "roundf" )
#if defined(FABRIC_OS_WINDOWS)
    return (void *)&imp_roundf;
#else
    return (void *)&roundf;
#endif
  else if( functionName == "fabs" )
    return( void *)&fabsf;
  else if ( functionName == "fmodf" )
    return (void *)&fmodf;
  else if ( functionName == "logf" )
    return (void *)&logf;
  else if ( functionName == "externalSquare" )
    return (void *)&externalSquare;
#if defined(FABRIC_OS_WINDOWS)
  else if ( functionName == "_chkstk" )
    return (void *)&_chkstk;
#endif
  else
  {
    void *result = cgManager->llvmResolveExternalFunction( functionName );
    if ( result )
      return result;
    result = OCL::llvmResolveExternalFunction( functionName );
    if ( result )
      return result;
  }
  fprintf( stderr, "Unable to look up symbol for '%s'\n", functionName.c_str() );
  return( NULL );
}


void handleFile( FILE *fp, unsigned int runFlags )
{
  std::string sourceString;
  while ( true )
  {
    char buffer[16384];
    size_t count = fread( buffer, 1, 16384, fp );
    sourceString.append( &buffer[0], count );
    if ( count < 16384 )
      break;
  }
  
  RC::ConstHandle<KL::Source> source = KL::StringSource::Create( sourceString );
  if ( runFlags & RF_ShowTokens )
  {
    RC::Handle<KL::Scanner> scanner = KL::Scanner::Create( source );
    for (;;)
    {
      KL::Token token = scanner->nextToken();
      RC::Handle<JSON::Object> jsonObject = JSON::Object::Create();
      jsonObject->set( "token", JSON::Integer::Create( token.getType() ) );
      jsonObject->set( "value", JSON::String::Create( token.getSourceRange().toString() ) );
      printf( "%s\n", JSON::encode( jsonObject ).c_str() );
      if ( token.getType() == TOKEN_END )
        break;
    }
  }

  RC::Handle<KL::Scanner> scanner = KL::Scanner::Create( source );
  CG::Diagnostics diagnostics;
  
  llvm::CodeGenOpt::Level    optLevel = llvm::CodeGenOpt::Aggressive;

  
  llvm::InitializeNativeTarget();
  llvm::InitializeAllAsmPrinters();
  //LLVMLinkInJIT();

  RC::Handle<RT::Manager> rtManager = RT::Manager::Create( KL::Compiler::Create() );
  cgManager = CG::Manager::Create( rtManager );
  std::auto_ptr<llvm::Module> module( new llvm::Module( "kl", cgManager->getLLVMContext() ) );

  CG::ModuleBuilder moduleBuilder( cgManager, module.get() );
  cgManager->llvmPrepareModule( moduleBuilder );
  OCL::llvmPrepareModule( moduleBuilder, rtManager );
  
  RC::ConstHandle<AST::GlobalList> globalList = KL::Parse( scanner, diagnostics );

  if ( diagnostics.containsError() )
  {
    dumpDiagnostics( diagnostics );
    return;
  }
  
  if ( runFlags & RF_ShowAST )
  {
    if ( runFlags & RF_Verbose )
      printf( "-- AST --\n" );
    Util::SimpleString globalListJSONString = globalList->toJSON();
    printf( "%s\n", globalListJSONString.getCString() );
  }

  if( runFlags & (RF_ShowASM | RF_ShowIR | RF_ShowOptIR | RF_ShowOptASM | RF_Run) )
  {
    globalList->llvmPrepareModule( moduleBuilder, diagnostics );
    if ( !diagnostics.containsError() )
      globalList->llvmCompileToModule( moduleBuilder, diagnostics, false );
    if ( !diagnostics.containsError() )
      globalList->llvmCompileToModule( moduleBuilder, diagnostics, true );
    dumpDiagnostics( diagnostics );
    if ( !diagnostics.containsError() )
    {   
      if ( runFlags & RF_ShowIR )
      {
        if ( runFlags & RF_Verbose )
          printf( "-- Unoptimized IR --\n" ); 
        std::string irString;
        llvm::raw_string_ostream byteCodeStream( irString );
        module->print( byteCodeStream, 0 );
        byteCodeStream.flush();
        printf( "%s", irString.c_str() );
      }
      
      if( runFlags & RF_ShowASM )
      {
        std::string   errorStr;
        std::string   targetTriple = llvm::sys::getHostTriple();
        const llvm::Target *target = llvm::TargetRegistry::lookupTarget( targetTriple, errorStr );
        if( !target )
          throw Exception( errorStr );

        llvm::TargetMachine *targetMachine = target->createTargetMachine( targetTriple, "" );
        targetMachine->setAsmVerbosityDefault( true );

        if ( runFlags & RF_Verbose )
          printf( "-- Unoptimized %s assembly --\n", llvm::sys::getHostTriple().c_str() ); 

        llvm::FunctionPassManager *functionPassManager = new llvm::FunctionPassManager( module.get() );
        targetMachine->addPassesToEmitFile( 
          *functionPassManager, llvm::fouts(), 
          llvm::TargetMachine::CGFT_AssemblyFile, llvm::CodeGenOpt::Default );
        functionPassManager->doInitialization();
      
        llvm::Module::FunctionListType &functionList = module->getFunctionList();
        for ( llvm::Module::FunctionListType::iterator it=functionList.begin(); it!=functionList.end(); ++it )
          functionPassManager->run( *it );

        delete functionPassManager;
      }

      std::string errStr;
      llvm::EngineBuilder builder( module.get() );
      
      builder.setErrorStr( &errStr );
      builder.setEngineKind( llvm::EngineKind::JIT );
      builder.setOptLevel( optLevel );

      llvm::ExecutionEngine *executionEngine = builder.create();
      if ( !executionEngine )
        throw Fabric::Exception( "Failed to construct ExecutionEngine: " + errStr );
      
      executionEngine->InstallLazyFunctionCreator( &LazyFunctionCreator );
      cgManager->llvmAddGlobalMappingsToExecutionEngine( executionEngine, *module );
      
      // Make sure we don't search loaded libraries for missing symbols. 
      // Only symbols *we* provide should be available as calling functions.
      executionEngine->DisableSymbolSearching( );

      llvm::OwningPtr<llvm::PassManager> passManager( new llvm::PassManager );
      passManager->add( llvm::createVerifierPass() );
      llvm::createStandardFunctionPasses( passManager.get(), 2 );
      llvm::createStandardModulePasses( passManager.get(), 2, false, true, true, true, false, llvm::createFunctionInliningPass() );
      llvm::createStandardLTOPasses( passManager.get(), true, true, false );
      passManager->run( *module );

      if( runFlags & RF_ShowOptASM )
      {
        std::string   errorStr;
        std::string   targetTriple = llvm::sys::getHostTriple();
        const llvm::Target *target = llvm::TargetRegistry::lookupTarget( targetTriple, errorStr );
        if( !target )
          throw Exception( errorStr );

        llvm::TargetMachine *targetMachine = target->createTargetMachine( targetTriple, "" );
        targetMachine->setAsmVerbosityDefault( true );

        if ( runFlags & RF_Verbose )
          printf( "-- Optimized %s assembly --\n", llvm::sys::getHostTriple().c_str() ); 

        llvm::FunctionPassManager *functionPassManager = new llvm::FunctionPassManager( module.get() );
        targetMachine->addPassesToEmitFile( 
          *functionPassManager, llvm::fouts(), 
          llvm::TargetMachine::CGFT_AssemblyFile, optLevel );
        functionPassManager->doInitialization();
        llvm::Module::FunctionListType &functionList = module->getFunctionList();
        for ( llvm::Module::FunctionListType::iterator it=functionList.begin(); it!=functionList.end(); ++it )
          functionPassManager->run( *it );
        delete functionPassManager;
      }
      
      if ( runFlags & RF_ShowOptIR )
      {
        if ( runFlags & RF_Verbose )
          printf( "-- Optimized IR --\n" ); 
        std::string irString;
        llvm::raw_string_ostream byteCodeStream( irString );
        module->print( byteCodeStream, 0 );
        byteCodeStream.flush();
        printf( "%s", irString.c_str() );
      }

      if ( runFlags & RF_Run )
      {
        if ( runFlags & RF_Verbose )
          printf( "-- Run --\n" );
        llvm::Function *llvmEntry = module->getFunction( "entry" );
        if ( !llvmEntry )
          throw Exception("missing function 'entry'");
        void (*entryPtr)() = (void (*)())executionEngine->getPointerToFunction( llvmEntry );
        if ( !entryPtr )
          throw Exception("unable to get pointer to entry");
        entryPtr();
      }

      module.release();
      delete executionEngine;
    }
  }
}

int main( int argc, char **argv )
{
  unsigned int runFlags = 0;
  
  while ( true )
  {
    static struct option longOptions[] =
    {
      { "ast", 0, NULL, 'a' },
      { "asm", 0, NULL, 'm' },
      { "bison", 0, NULL, 'b' },
      { "ir", 0, NULL, 'i' },
      { "optasm", 0, NULL, 'p' },
      { "optir", 0, NULL, 'o' },
      { "run", 0, NULL, 'r' },
      { "tokens", 0, NULL, 't' },
      { "verbose", 0, NULL, 'v' },
      { NULL, 0, NULL, 0 }
    };
    int option_index = 0;

    int c = getopt_long( argc, argv, "", longOptions, &option_index );
    if ( c == -1 )
      break;
    switch ( c )
    {
      case 'a':
        runFlags |= RF_ShowAST;
        break;

      case 'b':
        runFlags |= RF_ShowBison;
        
      case 'i':
        runFlags |= RF_ShowIR;
        break;
        
      case 'm':
        runFlags |= RF_ShowASM;
        break;

      case 'o':
        runFlags |= RF_ShowOptIR;
        break;
        
      case 'p':
        runFlags |= RF_ShowOptASM;
        break;
        
      case 'r':
        runFlags |= RF_Run;
        break;
      
      case 't':
        runFlags |= RF_ShowTokens;
        break;
      
      case 'v':
        runFlags |= RF_Verbose;
        break;
        
      default:
        break;
    }
  }

  if( !( runFlags & ~RF_Verbose ) )
    runFlags |= RF_Run;

  if ( optind == argc )
  {
    try
    {
      handleFile( stdin, runFlags );
    }
    catch ( Exception e )
    {
      fprintf( stderr, "Caught Exception '%s'\n", e.getDesc().c_str() );
    }
  }
  else
  {
    for ( int i=optind; i<argc; ++i )
    {
      FILE *fp = fopen( argv[i], "r" );
      if ( !fp )
        perror( argv[i] );
      else
      {
        if ( argc - optind >= 2 )
          printf( "--- %s ---\n", argv[i] );
        try
        {
          handleFile( fp, runFlags );
        }
        catch ( Exception e )
        {
          fprintf( stderr, "Caught Exception '%s'\n", e.getDesc().c_str() );
        }
        fclose( fp );
      }
    }
  }

#if defined( FABRIC_OS_WINDOWS )
  // Stupid hack so that we don't lose the shell window in VS debugger
  if( ::IsDebuggerPresent() )
    getchar();
#endif

  return 0;
}
