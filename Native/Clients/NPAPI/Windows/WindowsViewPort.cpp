/*
 *
 *  Created by Halfdan Ingvarsson on 11-01-21.
 *  Copyright 2010 Fabric 3D Inc.. All rights reserved.
 *
 */

#include "WindowsViewPort.h"

#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/DG/Event.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Clients/NPAPI/Context.h>
#include <Fabric/Clients/NPAPI/Interface.h>

#include <windowsx.h>
#include <tchar.h>

#include <GL/GL.h>

namespace Fabric
{
  namespace NPAPI
  {
    RC::Handle<ViewPort> WindowsViewPort::Create( RC::ConstHandle<Interface> const &interface )
    {
      return new WindowsViewPort( interface );
    }
    
    WindowsViewPort::WindowsViewPort( RC::ConstHandle<Interface> const &interface )
      : ViewPort( interface )
      , m_logCollector( interface->getContext()->getLogCollector() )
      , m_npp( interface->getNPP() )
      , m_windowWidth( -1 )
      , m_windowHeight( -1 )
      , m_hWnd( 0 )
      , m_hDC( 0 )
      , m_hGLRC( 0 )
    {
      m_bWindowLess = true;

      uint32_t  bSupportsWindowLess = 0;

      if( NPN_GetValue( m_npp, NPNVSupportsWindowless, (void *)&bSupportsWindowLess ) == NPERR_NO_ERROR )
        bSupportsWindowLess = 0;

      if( m_bWindowLess )
      {
        if( bSupportsWindowLess )
        {
          if ( NPN_SetValue( m_npp, NPPVpluginWindowBool, NULL ) != NPERR_NO_ERROR )
          {
            m_bWindowLess = false;
          }
        }
        else
        {
          m_bWindowLess = false;
        }
      }
    }
    
    WindowsViewPort::~WindowsViewPort()
    {
      if( m_bWindowLess )
      {
      }
      else
      {
        if( !m_hWnd )
          return;

        
        termOGLContext();
        ::ReleaseDC( m_hWnd, m_hDC );
        m_hWnd = NULL;

      }
    }
      
    void WindowsViewPort::needsRedraw()
    {
      if( m_bWindowLess )
      {
        NPRect invalidRect;
        invalidRect.left = m_windowLeft;
        invalidRect.top = m_windowTop;
        invalidRect.right = m_windowLeft+m_windowWidth;
        invalidRect.bottom = m_windowTop+m_windowHeight;
        NPN_InvalidateRect( m_npp, &invalidRect );
      }
      else
      {
        ::InvalidateRect( m_hWnd, NULL, FALSE );
      }
    }
    
    NPError WindowsViewPort::nppSetWindow( NPWindow *npWindow )
    {
      bool  needsRedraw = true;

      if( m_bWindowLess )
      {
        if( npWindow->type != NPWindowTypeDrawable )
          return( NPERR_GENERIC_ERROR );

        HDC      hDC = static_cast<HDC>( npWindow->window );
      }
      else
      {
        if( npWindow->type != NPWindowTypeWindow )
          return( NPERR_GENERIC_ERROR );

        HWND    hWnd = (HWND)npWindow->window;
        if( m_hWnd != hWnd )
        {
          if( hWnd )
          {
            ::SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG_PTR)this );

            // Let's take ownership of the window's message pump.
            m_oldWndProc = SubclassWindow( hWnd, windowsMsgCB );

            m_hDC = ::GetDC( hWnd );

            initOGLContext( );

            // Let's divine the parent window
            m_hParentWnd = ::GetParent( hWnd );

            // If we're running in Chrome multi-process mode, our plugin window
            // is proxied through another window, so we need to go to the grand-parent
            // window to get the one we need to forward mouse and keyboard messages to.
            TCHAR   tcsClassName[ 128 ];
            ::GetClassName( m_hParentWnd, tcsClassName, ARRAYSIZE( tcsClassName ) );
            if( !_tcscmp( tcsClassName, _T( "WrapperNativeWindowClass" ) ) )
            {
              m_hParentWnd = ::GetParent( m_hParentWnd );
            }
          }
          else
          {
            termOGLContext( );
            ::SetWindowLongPtr( hWnd, GWLP_USERDATA, NULL );
            SubclassWindow( m_hWnd, m_oldWndProc );
            m_hParentWnd = NULL;
          }

          m_hWnd = hWnd;
        }
      }

      if( m_hWnd &&
          ( m_windowLeft != npWindow->x || m_windowTop != npWindow->y || 
            m_windowWidth != npWindow->width || m_windowHeight != npWindow->height ) )
      {
        m_windowLeft = npWindow->x;
        m_windowTop = npWindow->y;
        m_windowWidth = npWindow->width;
        m_windowHeight = npWindow->height;

        didResize( m_windowWidth, m_windowHeight );
      }

      return( NPERR_NO_ERROR );
    }
    
    int16_t WindowsViewPort::nppHandleEvent( void *pEvent )
    {
      NPEvent *npEvent = static_cast<NPEvent *>( pEvent );
      
      switch( npEvent->event )
      {
      case WM_PAINT:
        {
            // And this, ladies and gentlemen, is why NPAPI is not 32-bit safe.
          HDC hDC = (HDC)ULongToPtr( npEvent->wParam );
          const RECT *rcPaint = ((const RECT *)ULongToPtr( npEvent->lParam ));

          fireRedrawEvent( );
        }
        break;

      case WM_WINDOWPOSCHANGED:
        break;
      }

      return( 1 );
    }

    LRESULT WindowsViewPort::processMessage( UINT message, WPARAM wParam, LPARAM lParam )
    {
      switch( message )
      {
      case WM_PAINT:
      {
        HDC     hDC;
        RECT   rcPaint;
        PAINTSTRUCT     ps;

        ::ZeroMemory( &ps, sizeof( ps ) );
        hDC = BeginPaint( m_hWnd, &ps );
        rcPaint = ps.rcPaint;

        fireRedrawEvent( );
        
        ::glFinish( );
        ::SwapBuffers( m_hDC );

        ::EndPaint( m_hWnd, &ps );
        
        if ( m_logCollector )
          m_logCollector->flush();
        
        redrawFinished();
        return( 0 );
      }

      case WM_ERASEBKGND:
        return( 1 );

      case WM_SETFOCUS:

      case WM_KILLFOCUS:

        break;
      }

      if( message >= WM_MOUSEFIRST && 
          message <= WM_MOUSELAST &&
          message != WM_MOUSEWHEEL /* Chrome will redirect the wheel */ )
      {
        POINT   p = { GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) };
        ::MapWindowPoints( m_hWnd, m_hParentWnd, &p, 1 );

        LPARAM    lParentParam = MAKELONG( p.x, p.y );
        ::PostMessage( m_hParentWnd, message, wParam, lParentParam );
        return( 0 );
      }
      else if( message >= WM_KEYFIRST && message <= WM_KEYLAST )
      {
        ::PostMessage( m_hParentWnd, message, wParam, lParam );
        return( 0 );
      }

      return( DefWindowProc( m_hWnd, message, wParam, lParam ) );
    }

    void WindowsViewPort::fireRedrawEvent( )
    {
      RC::Handle<DG::Event> dgRedrawEvent = getRedrawEvent();
      if ( dgRedrawEvent )
      {
        try
        {
          dgRedrawEvent->fire();
        }
        catch ( Exception e )
        {
          fprintf( stderr, "redrawEvent: exception thrown: %s\n", e.getDesc().c_str() );
        }
        catch ( ... )
        {
          fprintf( stderr, "redrawEvent: unknown exception thrown\n" );
        }
      }
    }

    void WindowsViewPort::initOGLContext( )
    {
      PIXELFORMATDESCRIPTOR pfd;
      int format;

      ::ZeroMemory( &pfd, sizeof( pfd ) );

      pfd.nSize = sizeof( pfd );
      pfd.nVersion = 1;

      if( m_bWindowLess )
        pfd.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI;
      else
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

      pfd.iPixelType = PFD_TYPE_RGBA;
      pfd.cColorBits = 24;
      pfd.cDepthBits = 16;
      pfd.iLayerType = PFD_MAIN_PLANE;
      format = ::ChoosePixelFormat( m_hDC, &pfd );
      if( format == 0 )
        throw Exception( "Couldn't get preferred OGL format" );

      if( !::SetPixelFormat( m_hDC, format, &pfd ) )
        throw Exception( "Couldn't set OGL format" );

      m_hGLRC = ::wglCreateContext( m_hDC );
      if( !::wglMakeCurrent( m_hDC, m_hGLRC ) )
        throw Exception( "Couldn't activate OGL context" );
    }

    void WindowsViewPort::termOGLContext( )
    {
      ::wglMakeCurrent( 0, 0 );
      ::wglDeleteContext( m_hGLRC );
      m_hGLRC = NULL;
    }

    LRESULT WindowsViewPort::windowsMsgCB( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
    {
      WindowsViewPort *pThis = (WindowsViewPort *)::GetWindowLongPtr( hWnd, GWLP_USERDATA );

      // Ours?
      if( pThis && hWnd == pThis->m_hWnd )
        return( pThis->processMessage( message, wParam, lParam ) );
     
      return( DefWindowProc( hWnd, message, wParam, lParam ) );
    }
      
    void WindowsViewPort::pushOGLContext()
    {
      m_wglStack.push_back( WGLDCAndContext( ::wglGetCurrentDC(), ::wglGetCurrentContext() ) );
      
      if ( m_hDC && m_hGLRC )
      {
        BOOL wglMakeCurrentResult = ::wglMakeCurrent( m_hDC, m_hGLRC );
        FABRIC_ASSERT( wglMakeCurrentResult );
      }
    }
    
    void WindowsViewPort::popOGLContext()
    {
      FABRIC_ASSERT( !m_wglStack.empty() );

      BOOL wglMakeCurrentResult = ::wglMakeCurrent( m_wglStack.back().first, m_wglStack.back().second );
      FABRIC_ASSERT( wglMakeCurrentResult );

      m_wglStack.pop_back();
    }
  };
};
