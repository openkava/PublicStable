/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Clients/NPAPI/Darwin/WindowedCAViewPort.h>
#include <Fabric/Clients/NPAPI/Interface.h>
#include <Fabric/Clients/NPAPI/Context.h>
#include <Fabric/Base/JSON/Value.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/DG/Event.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Core/Util/Format.h>

#include <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>

namespace Fabric
{
  namespace NPAPI
  {
    typedef Util::UnorderedMap< DG::Context const *, CGLContextObj > ContextToCGLContextMap;
    ContextToCGLContextMap s_contextToCGLContextMap;
  };
};

@interface NPCAOpenGLLayer : CAOpenGLLayer
{
@private
  Fabric::NPAPI::WindowedCAViewPort *viewPort;
  Fabric::DG::Context *context;
}

-(id) initWithViewPort:(Fabric::NPAPI::WindowedCAViewPort *)_viewPort context:(Fabric::DG::Context *)_context;
-(void) invalidate;

@end

@implementation NPCAOpenGLLayer

-(id) initWithViewPort:(Fabric::NPAPI::WindowedCAViewPort *)_viewPort context:(Fabric::DG::Context *)_context
{
  if ( (self = [super init]) )
  {
    // [pzion 20100820] We don't retain the view port because it retains us.
    viewPort = _viewPort;
    
    context = _context;
    context->retain();

    self.opaque = NO;
    self.asynchronous = NO;
    self.masksToBounds = YES;
    self.needsDisplayOnBoundsChange = YES;
    self.actions =
      [[[NSMutableDictionary alloc] initWithObjectsAndKeys:
          [NSNull null], @"bounds",
          [NSNull null], @"position",
          nil] autorelease];
  }
  return self;
}

-(void) invalidate
{
  viewPort = 0;
  if ( context )
  {
    context->release();
    context = 0;
  }
}

-(void) dealloc
{
  if ( context )
    context->release();
  [super dealloc];
}

-(CGLPixelFormatObj) copyCGLPixelFormatForDisplayMask:(uint32_t)mask
{
  CGLPixelFormatAttribute pixelFormatAttributes[] =
  {
    kCGLPFAClosestPolicy,
    kCGLPFADisplayMask, (CGLPixelFormatAttribute)mask,
    kCGLPFAAccelerated, kCGLPFANoRecovery,
    kCGLPFAColorSize, (CGLPixelFormatAttribute)24,
    kCGLPFAAlphaSize, (CGLPixelFormatAttribute)8,
    kCGLPFADepthSize, (CGLPixelFormatAttribute)24,
    kCGLPFAStencilSize, (CGLPixelFormatAttribute)8,
    //kCGLPFAMultisample, kCGLPFASamples, (CGLPixelFormatAttribute)4,
    (CGLPixelFormatAttribute)0
  };
  CGLPixelFormatObj pixelFormat;
  GLint numPixelFormats = 0;
  CGLChoosePixelFormat( pixelFormatAttributes, &pixelFormat, &numPixelFormats );
  return pixelFormat;
}

-(void) releaseCGLPixelFormat:(CGLPixelFormatObj)pixelFormat
{
  [super releaseCGLPixelFormat:pixelFormat];
}

-(CGLContextObj) copyCGLContextForPixelFormat:(CGLPixelFormatObj)pixelFormat
{
  Fabric::NPAPI::ContextToCGLContextMap::const_iterator it = Fabric::NPAPI::s_contextToCGLContextMap.find( context );
  if ( it == Fabric::NPAPI::s_contextToCGLContextMap.end() )
  {
    CGLContextObj cglContextObj = [super copyCGLContextForPixelFormat:pixelFormat];
    CGLRetainContext( cglContextObj );
    it = Fabric::NPAPI::s_contextToCGLContextMap.insert( Fabric::NPAPI::ContextToCGLContextMap::value_type( context, cglContextObj ) ).first;
  }

  CGLContextObj ctx = it->second;
  CGLRetainContext( ctx );
  return ctx;  
}

-(void) releaseCGLContext:(CGLContextObj)ctx
{
  CGLReleaseContext( ctx );
}

- (BOOL) canDrawInCGLContext:(CGLContextObj)ctx
  pixelFormat:(CGLPixelFormatObj)pf
  forLayerTime:(CFTimeInterval)t
  displayTime:(const CVTimeStamp *)ts
{
  return YES;
}

-(void) drawInCGLContext:(CGLContextObj)cglContext
  pixelFormat:(CGLPixelFormatObj)pixelFormat
  forLayerTime:(CFTimeInterval)timeInterval
  displayTime:(CVTimeStamp const *)timeStamp
{
  if ( viewPort )
  {
    Fabric::DG::Context::NotificationBracket notificationBracket(context);

    size_t width = self.bounds.size.width, height = self.bounds.size.height;
    viewPort->setWindowSize( width, height );
 
    // [pzion 20110303] Fill the viewport with 18% gray adjusted to a
    // gamma of 2.2.  0.46 ~= 0.18^(1/2.2)
    glViewport( 0, 0, width, height );
    glClearColor( 0.46, 0.46, 0.46, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    try
    {
      viewPort->getRedrawEvent()->fire();
    }
    catch ( Fabric::Exception e )
    {
      viewPort->getLogCollector()->add( "redrawEvent: exception thrown: "+e );
    }
    catch ( ... )
    {
      viewPort->getLogCollector()->add( "redrawEvent: unknown exception thrown" );
    }
    
    viewPort->drawWatermark( width, height );

    viewPort->redrawFinished();
  }
  
  [super drawInCGLContext:cglContext pixelFormat:pixelFormat forLayerTime:timeInterval displayTime:timeStamp];
}
@end

@interface MenuItem : NSMenuItem
{
@private
  Fabric::NPAPI::ViewPort const *viewPort;
  Fabric::JSON::Value const *arg;
}

+(id) menuItemWithTitle:(NSString *)title viewPort:(Fabric::NPAPI::ViewPort const *)viewPort arg:(Fabric::JSON::Value const *)arg;
-(id) initWithTitle:(NSString *)title viewPort:(Fabric::NPAPI::ViewPort const *)viewPort arg:(Fabric::JSON::Value const *)arg;
-(void) runCallback:(id)sender;
@end

@implementation MenuItem

+(id) menuItemWithTitle:(NSString *)title viewPort:(Fabric::NPAPI::ViewPort const *)_viewPort arg:(Fabric::JSON::Value *)_arg
{
  return [[[MenuItem alloc] initWithTitle:title viewPort:_viewPort arg:_arg] autorelease];
}

-(id) initWithTitle:(NSString *)title viewPort:(Fabric::NPAPI::ViewPort const *)_viewPort arg:(Fabric::JSON::Value *)_arg
{
  if ( (self = [super initWithTitle:title action:@selector(runCallback:) keyEquivalent:@""]) )
  {
    [self setTarget:self];
    
    viewPort = _viewPort;
    viewPort->retain();
    
    arg = _arg;
    arg->retain();
  }
  return self;
}

-(void) dealloc
{
  arg->release();
  viewPort->release();
  [super dealloc];
}

-(void) runCallback:(id)sender
{
  viewPort->jsonNotifyPopUpItem( arg );
}

@end

namespace Fabric
{
  namespace NPAPI
  {
    RC::Handle<ViewPort> WindowedCAViewPort::Create( RC::ConstHandle<Interface> const &interface )
    {
      NPP npp = interface->getNPP();
      
      NPBool supportsEventModel;
      if ( NPN_GetValue( npp, NPNVsupportsCocoaBool, &supportsEventModel ) != NPERR_NO_ERROR )
        supportsEventModel = false;
      if ( !supportsEventModel || NPN_SetValue( npp, NPPVpluginEventModel, (void *)NPEventModelCocoa ) != NPERR_NO_ERROR )
        return 0;
        
      NPBool supportsDrawingModel;
      if ( NPN_GetValue( npp, NPNVsupportsCoreAnimationBool, &supportsDrawingModel ) != NPERR_NO_ERROR )
        supportsDrawingModel = false;
      if ( !supportsDrawingModel || NPN_SetValue( npp, NPPVpluginDrawingModel, (void *)NPDrawingModelCoreAnimation ) != NPERR_NO_ERROR )
        return 0;
      
      FABRIC_LOG( "Using CoreAnimation drawing model" );
        
      return new WindowedCAViewPort( interface );
    }
    
    WindowedCAViewPort::WindowedCAViewPort( RC::ConstHandle<Interface> const &interface )
      : ViewPort( interface )
      , m_npp( interface->getNPP() )
      , m_width( 0 )
      , m_height( 0 )
    {
      m_npCAOpenGLLayer = [[NPCAOpenGLLayer alloc] initWithViewPort:this context:interface->getContext().ptr()];
    }
    
    WindowedCAViewPort::~WindowedCAViewPort()
    {
      [m_npCAOpenGLLayer invalidate];
      [m_npCAOpenGLLayer autorelease];
    }
    
    void WindowedCAViewPort::needsRedraw()
    {
      [m_npCAOpenGLLayer setNeedsDisplay];
    }
    
    void WindowedCAViewPort::redrawFinished()
    {
      ViewPort::redrawFinished();
    }
    
    /*
    void WindowedCAViewPort::timerFired()
    {
      if ( m_displaysPending > 0 )
        needsRedraw();
    }
    */
    
    NPError WindowedCAViewPort::nppGetValue( NPPVariable variable, void *value )
    {
      switch ( variable )
      {
        case NPPVpluginCoreAnimationLayer:
          *((CALayer **)value) = m_npCAOpenGLLayer;
          // [pzion 20110303] We shouldn't need to retain this here, but we crash
          // in Safari if we don't.
          [m_npCAOpenGLLayer retain];
          return NPERR_NO_ERROR;

        default:
          return ViewPort::nppGetValue( variable, value );
      }
    }
    
    int16_t WindowedCAViewPort::nppHandleEvent( void *event )
    {
      NPCocoaEvent *npCocoaEvent = reinterpret_cast<NPCocoaEvent *>( event );
      switch ( npCocoaEvent->type )
      {
        case NPCocoaEventDrawRect:
          [m_npCAOpenGLLayer setNeedsDisplay];
          return true;
          
        case NPCocoaEventMouseDown:
          if ( npCocoaEvent->data.mouse.buttonNumber == 1 )
          {
            if ( !m_popUpItems.empty() )
            {
              NSMenu *nsMenu = [[[NSMenu alloc] initWithTitle:@"Fabric Pop-Up Menu"] autorelease];
              [nsMenu setAutoenablesItems:NO];
            
              for ( PopUpItems::const_iterator it=m_popUpItems.begin(); it!=m_popUpItems.end(); ++it )
              {
                NSMenuItem *nsMenuItem = [MenuItem menuItemWithTitle:[NSString stringWithCString:it->desc.c_str() encoding:NSUTF8StringEncoding] viewPort:this arg:it->value.ptr()];
                [nsMenuItem setEnabled:YES];
                [nsMenu addItem:nsMenuItem];
              }
              
              NPN_PopUpContextMenu( m_npp, (NPMenu *)nsMenu );
              
              return true;
            }
          }
          break;
        
        default:
          break;
      }
      return false;
    }
    
    NPError WindowedCAViewPort::nppDestroy( NPSavedData** save )
    {
      RC::Handle<Context> context = getInterface()->getContext();
    
      ContextToCGLContextMap::iterator it = s_contextToCGLContextMap.find( context.ptr() );
      if ( it != s_contextToCGLContextMap.end() )
      {
        CGLReleaseContext( it->second );
        s_contextToCGLContextMap.erase( it );
      }
      
      return ViewPort::nppDestroy( save );
    }
    
    void WindowedCAViewPort::setWindowSize( size_t width, size_t height )
    {
      if ( width != m_width || height != m_height )
      {
        m_width = width;
        m_height = height;
        didResize( width, height );
      }
    }
    
    void WindowedCAViewPort::getWindowSize( size_t &width, size_t &height ) const
    {
      width = m_width;
      height = m_height;
    }
      
    void WindowedCAViewPort::pushOGLContext()
    {
      m_cglContextStack.push_back( CGLGetCurrentContext() );
      
      RC::Handle<DG::Context> context = getInterface()->getContext();
      ContextToCGLContextMap::const_iterator it = s_contextToCGLContextMap.find( context.ptr() );
      if ( it != s_contextToCGLContextMap.end() )
        CGLSetCurrentContext( it->second );
    }
    
    void WindowedCAViewPort::popOGLContext()
    {
      FABRIC_ASSERT( !m_cglContextStack.empty() );
      CGLSetCurrentContext( m_cglContextStack.back() );
      m_cglContextStack.pop_back();
    }
  };
};
