#include <Fabric/Base/Config.h>
#include <Fabric/Core/Util/Log.h>
#include <Fabric/Core/Util/Debug.h>
#include <Fabric/Core/Build.h>
#if defined( FABRIC_WIN32)
#include <Fabric/Core/MT/Impl.h>
#endif

#include <npapi/npapi.h>
#include <npapi/npfunctions.h>
#include <llvm/Support/Threading.h>
#include <stdlib.h>

#if defined(FABRIC_OS_LINUX)
namespace Fabric
{
  namespace NPAPI
  {
    extern NPError NPP_GetValue(NPP instance, NPPVariable variable, void* value);
  };
};
#endif

extern "C"
{
FABRIC_NPAPI_EXPORT NPError OSCALL NP_GetEntryPoints(NPPluginFuncs* plugin_funcs)
{
  extern NPError InitializePluginFunctions(NPPluginFuncs* plugin_funcs);
  return InitializePluginFunctions(plugin_funcs);
}

static void llvmInitialize()
{
  llvm::llvm_start_multithreaded();
  if ( !llvm::llvm_is_multithreaded() )
  {
    FABRIC_LOG( "LLVM not compiled with multithreading enabled; aborting" );
    abort();
  }
}

static void displayHeader()
{
  FABRIC_LOG( "%s version %s", Fabric::buildName, Fabric::buildFullVersion );
  struct tm const *lt = localtime( &Fabric::buildExpiry );
  char buf[1024];
  strftime( buf, 1024, "This build of Fabric will expire on %Y-%m-%d at %H:%M:%S", lt );
  FABRIC_LOG( "%s", buf );
  FABRIC_LOG( "Plugin loaded." );
}

#if defined(FABRIC_OS_LINUX)
FABRIC_NPAPI_EXPORT NPError OSCALL NP_Initialize(NPNetscapeFuncs* browser_functions,
                      NPPluginFuncs* plugin_functions)
{
  displayHeader();
  FABRIC_DEBUG_LOG( "Debug with: gdb --pid=%d", getpid() );
  llvmInitialize();
  extern void InitializeBrowserFunctions(NPNetscapeFuncs* browser_functions);
  InitializeBrowserFunctions(browser_functions);
  NPError np_err = NP_GetEntryPoints(plugin_functions);
  return np_err;
}
#elif defined(FABRIC_OS_MACOSX)
FABRIC_NPAPI_EXPORT NPError OSCALL NP_Initialize(NPNetscapeFuncs* browser_functions)
{
  displayHeader();
  FABRIC_LOG( "Debug with: gdb --pid=%d", getpid() );
  llvmInitialize();
  extern void InitializeBrowserFunctions(NPNetscapeFuncs* browser_functions);
  InitializeBrowserFunctions(browser_functions);
  return NPERR_NO_ERROR;
}
#elif defined(FABRIC_OS_WINDOWS)
FABRIC_NPAPI_EXPORT NPError OSCALL NP_Initialize(NPNetscapeFuncs* browser_functions)
{
  displayHeader();
  llvmInitialize();
  extern void InitializeBrowserFunctions(NPNetscapeFuncs* browser_functions);
  InitializeBrowserFunctions(browser_functions);
  return NPERR_NO_ERROR;
}
#else
# error "Unrecognized FABRIC_OS_..."
#endif

FABRIC_NPAPI_EXPORT NPError OSCALL NP_Shutdown()
{
#if defined (__native_client__)
  pglTerminate();
#endif
#if defined( FABRIC_WIN32)
  Fabric::MT::ThreadPool::Instance()->terminate();
#endif

  llvm::llvm_stop_multithreaded();
  
  return NPERR_NO_ERROR;
}

#if defined(FABRIC_OS_LINUX)
FABRIC_NPAPI_EXPORT NPError OSCALL NP_GetValue(NPP instance, NPPVariable variable, void* value)
{
  return Fabric::NPAPI::NPP_GetValue(instance, variable, value);
}

FABRIC_NPAPI_EXPORT const char * OSCALL NP_GetPluginVersion(void)
{
  return Fabric::buildFullVersion;
}

FABRIC_NPAPI_EXPORT const char* OSCALL NP_GetMIMEDescription(void)
{
  return "application/fabric::Fabric application";
}
#endif

}  // extern "C"
