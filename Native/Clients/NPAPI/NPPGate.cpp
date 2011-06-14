// Copyright 2010 The Native Client SDK Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include <Fabric/Clients/NPAPI/Interface.h>
#if defined(FABRIC_OS_MACOSX)
# include <Fabric/Clients/NPAPI/Darwin/WindowedCAViewPort.h>
# include <Fabric/Clients/NPAPI/Darwin/WindowedInvalidatingCAViewPort.h>
# if !defined(__x86_64__)
#  include <Fabric/Clients/NPAPI/Darwin/WindowlessCGViewPort.h>
# endif
#elif defined(FABRIC_OS_LINUX)
# include <Fabric/Clients/NPAPI/Linux/X11ViewPort.h>
#elif defined(FABRIC_OS_WINDOWS)
# include <Fabric/Clients/NPAPI/Windows/WindowsViewport.h>
#endif
#include <Fabric/Clients/NPAPI/Context.h>
#include <Fabric/Clients/NPAPI/IOManager.h>
#include <Fabric/Core/Build.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/Plug/Manager.h>
#include <Fabric/Core/Util/Assert.h>
#include <Fabric/Core/MT/IdleTaskQueue.h>
#include <Fabric/Core/IO/Helpers.h>
#if defined(FABRIC_MODULE_OCL)
# include <Fabric/Core/OCL/OCL.h>
#endif
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Core/Util/Debug.h>
#include <Fabric/Core/Build.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <npapi/npapi.h>
#include <npapi/npfunctions.h>
#include <new>

class CALayer;

namespace Fabric
{
  namespace NPAPI
  {
    NPError NPP_New(
      NPMIMEType mime_type,
      NPP npp,
      uint16_t mode,
      int16_t argc,
      char *argn[],
      char *argv[],
      NPSavedData *saved
      )
    {
      if ( !npp )
        return NPERR_INVALID_INSTANCE_ERROR;

      FABRIC_LOG( "Fabric version %s", buildVersion );
      struct tm const *lt = localtime( &buildExpiry );
      char buf[1024];
      strftime( buf, 1024, "This build of Fabric will expire on %Y-%m-%d at %H:%M:%S", lt );
      FABRIC_LOG( "%s", buf );

      std::string contextID;
      enum
      {
        VPT_EMPTY,
        VPT_3D
      } viewPortType = VPT_EMPTY;
      bool compositing = false;
      for ( int16_t i=0; i<argc; ++i )
      {
        if ( strcmp( argn[i], "windowType" ) == 0 )
        {
          if ( strcmp( argv[i], "3d" ) == 0 )
            viewPortType = VPT_3D;
          else if ( strcmp( argv[i], "empty" ) == 0 )
            viewPortType = VPT_EMPTY;
        }
        else if ( strcmp( argn[i], "contextID" ) == 0 )
          contextID = argv[i];
        else if ( strcmp( argn[i], "compositing" ) == 0 )
          compositing = true;
      }
      
      RC::Handle<Context> context;
      if ( contextID.length() > 0 )
      {
        context = RC::Handle<Context>::StaticCast( DG::Context::Bind( contextID ) );
        FABRIC_DEBUG_LOG( "Bound to existing context '%s'", contextID.c_str() );
      }
      else
      {
        std::vector<std::string> pluginDirs;
#if defined(FABRIC_OS_MACOSX)
        char const *home = getenv("HOME");
        if ( home && *home )
          pluginDirs.push_back( std::string(home) + "/Library/Fabric/Exts" );
        pluginDirs.push_back( "/Library/Fabric/Exts" );
#elif defined(FABRIC_OS_LINUX)
        char const *home = getenv("HOME");
        if ( home && *home )
          pluginDirs.push_back( IO::joinPath( IO::joinPath( std::string(home), "FabricIDE" ), "Exts" ) );
#elif defined(FABRIC_OS_WINDOWS)
        char const *appData = getenv("APPDATA");
        if ( appData && *appData )
          pluginDirs.push_back( IO::joinPath( IO::joinPath( std::string(appData), "Fabric" ), "Exts" ) );
#endif
      
        RC::Handle<IOManager> ioManager = IOManager::Create( npp );
        context = Context::Create( ioManager, pluginDirs );
        context->getPlugManager()->loadBuiltInPlugins();
        
        contextID = context->getContextID();
        FABRIC_DEBUG_LOG( "Created new context '%s'", contextID.c_str() );
    
#if defined(FABRIC_MODULE_OCL)
        OCL::registerTypes( context->getRTManager() );
#endif
      }
      
      RC::Handle<Interface> interface = Interface::Create( npp, context );

      RC::Handle<ViewPort> viewPort;
      if ( viewPortType != VPT_EMPTY )
      {
#if defined(FABRIC_OS_NACL)
        viewPort = NPNaClViewPort::Create();
#elif defined(FABRIC_OS_MACOSX)
        if ( compositing )
        {
# if !defined(__x86_64__)
          viewPort = WindowlessCGViewPort::Create( interface );
# else
          FABRIC_LOG( "Compositing viewport not supported in 64-bit mode" );
          return NPERR_GENERIC_ERROR;
# endif
        }
        else
        {
          viewPort = WindowedInvalidatingCAViewPort::Create( interface );
          if ( !viewPort )
            viewPort = WindowedCAViewPort::Create( interface );
        }
#elif defined(FABRIC_OS_LINUX)
        viewPort = X11ViewPort::Create( interface );
#elif defined(FABRIC_OS_WINDOWS)
        viewPort = WindowsViewPort::Create( interface );
#endif
        if ( !viewPort )
        {
          FABRIC_LOG( "Unable to create a viewport for your browser!" );
          return NPERR_GENERIC_ERROR;
        }
        else
        {
          interface->setViewPort( viewPort );
      
          std::vector<std::string> src;
          src.push_back("VP");
          src.push_back("viewPort");
          
          context->jsonNotify( src, "delta", viewPort->jsonDesc() ); 
        }
      }
      
      npp->pdata = interface.take();

      return NPERR_NO_ERROR;
    }

    NPError NPP_Destroy( NPP npp, NPSavedData** save )
    {
      if ( !npp )
        return NPERR_INVALID_INSTANCE_ERROR;
        
      MT::IdleTaskQueue::Instance()->wait();
      
      Interface *interface = static_cast<Interface *>( npp->pdata );
      // [pzion 20110228] Safari sometimes calls NPP_Destroy with an uninitialized instance
      if ( interface )
      {
        NPError npError = interface->nppDestroy( npp, save );
        interface->release();
        return npError;
      }
      else return NPERR_NO_ERROR;
    }

    NPError NPP_GetValue( NPP npp, NPPVariable variable, void *value )
    {
#if defined(FABRIC_OS_LINUX)
      // [pzion 20110211] Special case: Linux plugin requires XEmbed no matter what
      if ( variable == NPPVpluginNeedsXEmbed )
      {
        *(static_cast<NPBool*>(value)) = true;
        return NPERR_NO_ERROR;
      }
#endif
      
      if ( !npp )
        return NPERR_INVALID_INSTANCE_ERROR;
      Interface *interface = static_cast<Interface *>( npp->pdata );
        
      return interface->nppGetValue( npp, variable, value );
    }

    // |event| just took place in this plugin's window in the browser.  This
    // function should return true if the event was handled, false if it was
    // ignored.
    // Declaration: npapi.h
    // Documentation URL: https://developer.mozilla.org/en/NPP_HandleEvent
    int16_t NPP_HandleEvent( NPP npp, void* event )
    {
      if ( !npp )
        return NPERR_INVALID_INSTANCE_ERROR;
      Interface *interface = static_cast<Interface *>( npp->pdata );
      
      return interface->nppHandleEvent( npp, event );
    }

    NPError NPP_SetWindow( NPP npp, NPWindow *window )
    {
      if ( !npp )
        return NPERR_INVALID_INSTANCE_ERROR;
      Interface *interface = static_cast<Interface *>( npp->pdata );
      return interface->nppSetWindow( npp, window );
    }

    NPError NPP_NewStream( NPP npp, NPMIMEType type, NPStream *stream, NPBool seekable, uint16_t *stype )
    {
      if( !npp )
        return NPERR_INVALID_INSTANCE_ERROR;
      Interface *interface = static_cast<Interface *>( npp->pdata );
      return interface->nppNewStream( npp, type, stream, seekable, stype );
    }

    void NPP_StreamAsFile( NPP npp, NPStream *stream, const char *fname )
    {
      FABRIC_ASSERT( npp );
      Interface *interface = static_cast<Interface *>( npp->pdata );

#if defined( FABRIC_OS_MACOSX )
      // [hi20110420  - For some braindamaged reason, Safari only gives us HFS-style paths
      // rather than POSIX absolute paths. Let's try to autoconvert]
      if( fname[0] != '/' && strchr( fname, ':' ) )
      {
        CFStringRef pathRef = CFStringCreateWithCString( kCFAllocatorDefault, fname, kCFStringEncodingUTF8 );
        CFURLRef urlRef = CFURLCreateWithFileSystemPath( kCFAllocatorDefault, pathRef, kCFURLHFSPathStyle, false );
        CFRelease( pathRef );
        pathRef = CFURLCopyFileSystemPath(urlRef, kCFURLPOSIXPathStyle);
        CFRelease( urlRef );
        CFIndex   len = strlen( fname ) * 2 + 1;
        char *newfname = (char *)alloca( len );
        CFStringGetCString( pathRef, newfname, len, kCFStringEncodingUTF8 );
        CFRelease( pathRef );
        fname = newfname;
      }
      
#endif

      interface->nppStreamAsFile( npp, stream, fname );
    }
    
    NPError NPP_DestroyStream( NPP npp, NPStream *stream, NPReason reason )
    {
      if( !npp )
        return NPERR_INVALID_INSTANCE_ERROR;
      Interface *interface = static_cast<Interface *>( npp->pdata );
      return interface->nppDestroyStream( npp, stream, reason );
    }
    
    void NPP_URLNotify(NPP npp, const char* url, NPReason reason, void* notifyData)
    {
      // It's just here for Safari.
    }
  };
};

extern "C" NPError InitializePluginFunctions( NPPluginFuncs *npPluginFuncs )
{
  memset( npPluginFuncs, 0, sizeof(*npPluginFuncs) );
  npPluginFuncs->version = NPVERS_HAS_PLUGIN_THREAD_ASYNC_CALL;
  npPluginFuncs->size = sizeof(*npPluginFuncs);
  
  npPluginFuncs->newp = &Fabric::NPAPI::NPP_New;
  npPluginFuncs->destroy = &Fabric::NPAPI::NPP_Destroy;
  npPluginFuncs->setwindow = &Fabric::NPAPI::NPP_SetWindow;
  npPluginFuncs->event = &Fabric::NPAPI::NPP_HandleEvent;
  npPluginFuncs->getvalue = &Fabric::NPAPI::NPP_GetValue;
  npPluginFuncs->newstream = &Fabric::NPAPI::NPP_NewStream;
  npPluginFuncs->asfile = &Fabric::NPAPI::NPP_StreamAsFile;
  npPluginFuncs->destroystream = &Fabric::NPAPI::NPP_DestroyStream;
  npPluginFuncs->urlnotify = &Fabric::NPAPI::NPP_URLNotify;
  
  return NPERR_NO_ERROR;
}
