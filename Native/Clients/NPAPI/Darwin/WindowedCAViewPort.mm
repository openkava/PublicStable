/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Clients/NPAPI/Darwin/WindowedCAViewPort.h>
#include <Fabric/Clients/NPAPI/Interface.h>
#include <Fabric/Clients/NPAPI/Context.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/DG/Event.h>
#include <Fabric/Core/MT/LogCollector.h>

#include <Cocoa/Cocoa.h>

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
  if ( self = [super init] )
  {
    // [pzion 20100820] We don't retain the view port because it retains us.
    viewPort = _viewPort;
    
    context = _context;
    context->retain();

    self.asynchronous = NO;
    self.needsDisplayOnBoundsChange = NO;
    self.autoresizingMask = kCALayerWidthSizable | kCALayerHeightSizable;
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
  //FABRIC_LOG( "copyCGLPixelFormatForDisplayMask" );
  CGLPixelFormatAttribute pixelFormatAttributes[] =
  {
    kCGLPFADisplayMask, (CGLPixelFormatAttribute)mask,
    kCGLPFAAccelerated,
    //kCGLPFADoubleBuffer,
    kCGLPFAColorSize, (CGLPixelFormatAttribute)24,
    kCGLPFAAlphaSize, (CGLPixelFormatAttribute)8,
    kCGLPFADepthSize, (CGLPixelFormatAttribute)16,
    (CGLPixelFormatAttribute)0
  };
  CGLPixelFormatObj pixelFormat;
  GLint numPixelFormats = 0;
  CGLError error = CGLChoosePixelFormat( pixelFormatAttributes, &pixelFormat, &numPixelFormats );
  return pixelFormat;
}

-(void) releaseCGLPixelFormat:(CGLPixelFormatObj)pixelFormat
{
  Fabric::NPAPI::ContextToCGLContextMap::iterator it = Fabric::NPAPI::s_contextToCGLContextMap.find( context );
  if ( it != Fabric::NPAPI::s_contextToCGLContextMap.end() )
    Fabric::NPAPI::s_contextToCGLContextMap.erase( it );
}

-(CGLContextObj) copyCGLContextForPixelFormat:(CGLPixelFormatObj)pixelFormat
{
  Fabric::NPAPI::ContextToCGLContextMap::const_iterator it = Fabric::NPAPI::s_contextToCGLContextMap.find( context );
  if ( it == Fabric::NPAPI::s_contextToCGLContextMap.end() )
  {
    CGLContextObj cglContextObj = [super copyCGLContextForPixelFormat:pixelFormat];
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

-(void) drawInCGLContext:(CGLContextObj)cglContext
  pixelFormat:(CGLPixelFormatObj)pixelFormat
  forLayerTime:(CFTimeInterval)timeInterval
  displayTime:(CVTimeStamp const *)timeStamp
{
  if ( viewPort )
  {
    size_t width, height;
    viewPort->getWindowSize( width, height );
    //FABRIC_LOG( "NPCAOpenGLLayer::drawInCGLContext: width=%u, height=%u, glContext=%p", (unsigned)width, (unsigned)height, (void *)cglContext );

    // [pzion 20110303] Fill the viewport with 18% gray adjusted to a
    // gamma of 2.2.  0.46 ~= 0.18^(1/2.2)
    glViewport( 0, 0, width, height );
    glClearColor( 0.46, 0.46, 0.46, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    try
    {
      Fabric::DG::Context::NotificationBracket notificationBracket(context);
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
    
    glFinish();
  }
  
  [super drawInCGLContext:cglContext pixelFormat:pixelFormat forLayerTime:timeInterval displayTime:timeStamp];

  if ( viewPort )
    viewPort->redrawFinished();
}

-(void) didChangeValueForKey:(NSString *)key
{
  if ( [key isEqualToString:@"bounds"] )
  {
    CGSize size = self.bounds.size;
    //FABRIC_LOG( "didChangeValueForKey:'bounds' size=(%g,%g)", size.width, size.height );
    viewPort->setWindowSize( size.width, size.height );
  }
}
@end

@interface MenuItem : NSMenuItem
{
@private
  /*
  Fabric::LIB::Callback const *callback;
  Fabric::LIB::Value *arg;
  */
}

+(id) menuItemWithTitle:(NSString *)title callback:(Fabric::LIB::Callback const *)callback arg:(Fabric::LIB::Value *)arg;
-(id) initWithTitle:(NSString *)title callback:(Fabric::LIB::Callback const *)callback arg:(Fabric::LIB::Value *)arg;
-(void) runCallback:(id)sender;
@end

@implementation MenuItem

+(id) menuItemWithTitle:(NSString *)title callback:(Fabric::LIB::Callback const *)_callback arg:(Fabric::LIB::Value *)_arg
{
  return [[[MenuItem alloc] initWithTitle:title callback:_callback arg:_arg] autorelease];
}

-(id) initWithTitle:(NSString *)title callback:(Fabric::LIB::Callback const *)_callback arg:(Fabric::LIB::Value *)_arg
{
  if ( self = [super initWithTitle:title action:@selector(runCallback:) keyEquivalent:@""] )
  {
    [self setTarget:self];
    
    /*
    callback = _callback;
    callback->retain();
    
    arg = _arg;
    arg->retain();
    */
  }
  return self;
}

-(void) dealloc
{
  /*
  arg->release();
  callback->release();
  */
  [super dealloc];
}

-(void) runCallback:(id)sender
{
  /*
  std::vector< Fabric::RC::Handle<Fabric::LIB::Value> > args;
  args.push_back( arg );
  callback->invoke( args );
  */
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
        [m_npCAOpenGLLayer renderInContext:npCocoaEvent->data.draw.context];
        return true;
        
      case NPCocoaEventMouseDown:
        if ( npCocoaEvent->data.mouse.buttonNumber == 1 )
        {
          Context::PopUpItems const &popupItems = getInterface()->getContext()->getPopUpItems();
          if ( !popupItems.empty() )
          {
            NSMenu *nsMenu = [[[NSMenu alloc] initWithTitle:@"Fabric Pop-Up Menu"] autorelease];
            [nsMenu setAutoenablesItems:NO];
          
            for ( Context::PopUpItems::const_iterator it=popupItems.begin(); it!=popupItems.end(); ++it )
            {
              /*
              NSMenuItem *nsMenuItem = [MenuItem menuItemWithTitle:[NSString stringWithCString:it->desc.c_str() encoding:NSUTF8StringEncoding] callback:it->callback.ptr() arg:it->arg.ptr()];
              [nsMenuItem setEnabled:YES];
              [nsMenu addItem:nsMenuItem];
              */
            }
            
            NPN_PopUpContextMenu( m_npp, (NPMenu *)nsMenu );
            
            return true;
          }
        }
        break;
      }
      return false;
    }
    
    void WindowedCAViewPort::setWindowSize( size_t width, size_t height )
    {
      if ( width != m_width || height != m_height )
      {
        m_width = width;
        m_height = height;
        needsRedraw();
        didResize( width, height );
      }
    }
    
    void WindowedCAViewPort::getWindowSize( size_t &width, size_t &height ) const
    {
      width = m_width;
      height = m_height;
    }
  };
};
