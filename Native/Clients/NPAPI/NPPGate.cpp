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
#include <Fabric/Core/OCL/OCL.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Core/Util/Debug.h>
#include <Fabric/Core/Build.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <npapi/npapi.h>
#include <npapi/npfunctions.h>
#include <new>
#if defined(FABRIC_POSIX)
# include <signal.h>
# include <execinfo.h>
#endif

class CALayer;

namespace Fabric
{
  namespace NPAPI
  {
#if defined(FABRIC_POSIX)
    static const size_t signalStackSize = 65536;
    static uint8_t signalStack[signalStackSize];

    static void fatalSignalTrap( int signum )
    {
      char const *msg1 = "Caught fatal signal ";
      write( 2, msg1, strlen(msg1) );
      char const *signame;
      switch ( signum )
      {
        case SIGILL: signame = "SIGILL"; break;
        case SIGTRAP: signame = "SIGTRAP"; break;
        case SIGABRT: signame = "SIGABRT"; break;
        case SIGBUS: signame = "SIGBUS"; break;
        case SIGFPE: signame = "SIGFPE"; break;
        case SIGSEGV: signame = "SIGSEGV"; break;
        default: signame = "<unknown>"; break;
      }
      write( 2, signame, strlen(signame) );
      char const *msg2 = "\n---- Backtrace:\n";
      write( 2, msg2, strlen(msg2) );

      static const int maxNumBacktracePtrs = 64;
      void *backtracePtrs[maxNumBacktracePtrs];
      int numBacktracePtrs = backtrace( backtracePtrs, maxNumBacktracePtrs );
      backtrace_symbols_fd( backtracePtrs, numBacktracePtrs, 2 );

      _exit( 0 );
    }
#endif

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

#if defined(FABRIC_POSIX)
      stack_t stack;
      stack.ss_sp = &signalStack[0];
      stack.ss_flags = 0;
      stack.ss_size = signalStackSize;
      sigaltstack( &stack, NULL );

      struct sigaction sa;
      sa.sa_handler = &fatalSignalTrap;
      sigemptyset( &sa.sa_mask );
      sa.sa_flags = SA_ONSTACK;
      sigaction( SIGILL, &sa, NULL );
      sigaction( SIGTRAP, &sa, NULL );
      sigaction( SIGABRT, &sa, NULL );
      sigaction( SIGBUS, &sa, NULL );
      sigaction( SIGFPE, &sa, NULL );
      sigaction( SIGSEGV, &sa, NULL );
#endif

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
          pluginDirs.push_back( IO::joinPath( IO::joinPath( std::string(home), ".fabric" ), "Exts" ) );
        pluginDirs.push_back( "/usr/lib/fabric/Exts" );
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
    
        OCL::registerTypes( context->getRTManager() );
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
      NPError result = NPERR_NO_ERROR;
      try
      {
        result = interface->nppNewStream( npp, type, stream, seekable, stype );
      }
      catch ( Fabric::Exception e )
      {
        FABRIC_DEBUG_LOG( "NPP_NewStream: caught Fabric exception: " + e );
      }
      catch ( ... )
      {
        FABRIC_DEBUG_LOG( "NPP_NewStream: caught unknown exception" );
      }
      return result;
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

      try
      {
        interface->nppStreamAsFile( npp, stream, fname );
      }
      catch ( Fabric::Exception e )
      {
        FABRIC_DEBUG_LOG( "NPP_StreamAsFile: caught Fabric exception: " + e );
      }
      catch ( ... )
      {
        FABRIC_DEBUG_LOG( "NPP_StreamAsFile: caught unknown exception" );
      }
    }
    
    NPError NPP_DestroyStream( NPP npp, NPStream *stream, NPReason reason )
    {
      if( !npp )
        return NPERR_INVALID_INSTANCE_ERROR;
      Interface *interface = static_cast<Interface *>( npp->pdata );
      NPError result = NPERR_NO_ERROR;
      try
      {
        result = interface->nppDestroyStream( npp, stream, reason );
      }
      catch ( Fabric::Exception e )
      {
        FABRIC_DEBUG_LOG( "NPP_DestroyStream: caught Fabric exception: " + e );
      }
      catch ( ... )
      {
        FABRIC_DEBUG_LOG( "NPP_DestroyStream: caught unknown exception" );
      }
      return result;
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
