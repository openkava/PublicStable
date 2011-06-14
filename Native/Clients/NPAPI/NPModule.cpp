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

#if defined(FABRIC_OS_NACL)
FABRIC_NPAPI_EXPORT NPError OSCALL NP_Initialize(NPNetscapeFuncs* browser_functions,
                             NPPluginFuncs* plugin_functions)
{
  llvmInitialize();
  NPError np_err = NP_GetEntryPoints(plugin_functions);
  if (NPERR_NO_ERROR == np_err)
    pglInitialize();
  return np_err;
}
#elif defined(FABRIC_OS_LINUX)
FABRIC_NPAPI_EXPORT NPError OSCALL NP_Initialize(NPNetscapeFuncs* browser_functions,
                      NPPluginFuncs* plugin_functions)
{
  FABRIC_LOG( "Plugin loaded." );
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
  FABRIC_LOG( "Plugin loaded." );
  FABRIC_DEBUG_LOG( "Debug with: gdb --pid=%d", getpid() );
  llvmInitialize();
  extern void InitializeBrowserFunctions(NPNetscapeFuncs* browser_functions);
  InitializeBrowserFunctions(browser_functions);
  return NPERR_NO_ERROR;
}
#elif defined(FABRIC_OS_WINDOWS)
FABRIC_NPAPI_EXPORT NPError OSCALL NP_Initialize(NPNetscapeFuncs* browser_functions) {
  llvmInitialize();
  extern void InitializeBrowserFunctions(NPNetscapeFuncs* browser_functions);
  InitializeBrowserFunctions(browser_functions);
  return NPERR_NO_ERROR;
}
#else
# error "Unrecognized platform"
#endif

FABRIC_NPAPI_EXPORT NPError OSCALL NP_Shutdown()
{
#if defined (__native_client__)
  pglTerminate();
#endif
#if defined( FABRIC_WIN32)
  Fabric::MT::ThreadPool::Term( );
#endif

  llvm::llvm_stop_multithreaded();
  
  return NPERR_NO_ERROR;
}

#if defined(FABRIC_OS_LINUX)
FABRIC_NPAPI_EXPORT NPError OSCALL NP_GetValue(NPP instance, NPPVariable variable, void* value)
{
  extern NPError NPP_GetValue(NPP instance, NPPVariable variable, void* value);
  NPError err = NPERR_NO_ERROR;
  switch (variable)
  {
    case NPPVpluginNameString:
      *(static_cast<const char**>(value)) = "Fabric v" FABRIC_VERSION_STRING;
      break;
    case NPPVpluginDescriptionString:
      *(static_cast<const char**>(value)) = "http://fabric-engine.com";
      break;
    default:
      err = NPP_GetValue(instance, variable, value);
      break;
  }
  return err;
}

// Called by the browser to get the MIME Type for this plugin.
// Note that this MIME type has to match the type in the <embed> tag used to
// load the develop version of the module.  See the Mozilla docs for more info
// on the MIME type format:
//   https://developer.mozilla.org/En/NP_GetMIMEDescription
// Declaration: npupp.h
// Documentation URL: https://developer.mozilla.org/En/NP_GetMIMEDescription
FABRIC_NPAPI_EXPORT const char* OSCALL NP_GetMIMEDescription(void)
{
  return "application/fabric::Fabric application";
}
#endif

}  // extern "C"
