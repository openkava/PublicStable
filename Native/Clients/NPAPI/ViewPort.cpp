/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "ViewPort.h"
#include "Interface.h"
#include "Watermark.h"
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/IntegerDesc.h>
#include <Fabric/Clients/NPAPI/Context.h>
#include <Fabric/Core/DG/Node.h>
#include <Fabric/Core/DG/Event.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Base/JSON/Scalar.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

#if defined(FABRIC_OS_WINDOWS)
# include <intrin.h>
#endif

namespace Fabric
{
  namespace NPAPI
  {
    
    ViewPort::ViewPort( RC::ConstHandle<Interface> const &interface, uint32_t timerInterval )
      : m_npp( interface->getNPP() )
      , m_name( "viewPort" )
      , m_interface( interface.ptr() )
      , m_logCollector( interface->getContext()->getLogCollector() )
      , m_redrawFinishedCallback( 0 )
      , m_redrawFinishedCallbackPendingInvokeCount( 0 )
      , m_fpsCount( 0 )
      , m_fps( 0.0 )
      , m_watermarkShaderProgram( 0 )
    {
      if ( timerInterval )
      {
        m_timer = NPN_ScheduleTimer( m_npp, timerInterval, true, &ViewPort::TimerFiredCallback );
        s_timers[m_npp].insert( Util::UnorderedMap< uint32_t, ViewPort *>::value_type( m_timer, this ) );
      }
      else m_timer = 0;

      RC::Handle<DG::Context> context = interface->getContext();
      RC::Handle<RT::Manager> rtManager = context->getRTManager();
      m_integerDesc = rtManager->getIntegerDesc();
      
      m_windowNode = DG::Node::Create( "FABRIC.window", context );
      m_windowNode->addMember( "width", m_integerDesc, m_integerDesc->getDefaultData() );
      m_windowNode->addMember( "height", m_integerDesc, m_integerDesc->getDefaultData() );
       
      m_redrawEvent = DG::Event::Create( "FABRIC.window.redraw", context );

#if defined(FABRIC_OS_WINDOWS)
      LARGE_INTEGER      fpsTimerFreq;
      ::QueryPerformanceFrequency( &fpsTimerFreq );
      m_fpsTimerFreq = double( fpsTimerFreq.QuadPart );
      ::QueryPerformanceCounter( &m_fpsStart );
#else
      gettimeofday( &m_fpsStart, NULL );
#endif

      m_interface->getContext()->registerViewPort( m_name, this );
    }
    
    ViewPort::~ViewPort()
    {
      m_interface->getContext()->unregisterViewPort( m_name, this );

      if ( m_redrawFinishedCallback )
        NPN_ReleaseObject( m_redrawFinishedCallback );
    }
    
    void ViewPort::jsonNotify( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg ) const
    {
      std::vector<std::string> src;
      src.push_back("VP");
      src.push_back("viewPort");
      
      m_interface->getContext()->jsonNotify( src, cmd, arg );
    }
    
    void ViewPort::issuePendingRedrawFinishedCallbacks()
    {
      while ( m_redrawFinishedCallbackPendingInvokeCount > 0 )
      {
        jsonNotify( "redrawFinished", 0 );
        
#if defined(FABRIC_OS_WINDOWS)
        ::_InterlockedDecrement( &m_redrawFinishedCallbackPendingInvokeCount );
#else
        __sync_sub_and_fetch( &m_redrawFinishedCallbackPendingInvokeCount, 1 );
#endif
      }
    }
    
    void ViewPort::timerFired()
    {
    }
    
    void ViewPort::TimerFiredCallback( NPP npp, uint32_t timer )
    {
      Util::UnorderedMap< uint32_t, ViewPort *> &timers = s_timers[npp];
      Util::UnorderedMap< uint32_t, ViewPort *>::iterator it = timers.find( timer );
      FABRIC_ASSERT( it != timers.end() );
      FABRIC_ASSERT( it->second->m_npp == npp );
      it->second->timerFired();
    }

    RC::Handle<MT::LogCollector> ViewPort::getLogCollector() const
    {
      return m_logCollector;
    }
      
    void ViewPort::redrawFinished()
    {
      ++m_fpsCount;
       
      double secDiff;

#if defined(FABRIC_OS_WINDOWS)
      LARGE_INTEGER    fpsNow;
      ::QueryPerformanceCounter( &fpsNow );

      secDiff = double(fpsNow.QuadPart - m_fpsStart.QuadPart) / m_fpsTimerFreq;
#else
      struct timeval fpsNow;
      gettimeofday( &fpsNow, NULL );
      secDiff = double((int)fpsNow.tv_sec - (int)m_fpsStart.tv_sec) + (double((int)fpsNow.tv_usec - (int)m_fpsStart.tv_usec) / 1.0e6);
#endif

      if ( secDiff >= 1.0 )
      {
        m_fps = float( m_fpsCount / secDiff );
        char buffer[64];
        sprintf( buffer, "%0.2f fps", m_fps );
        static const bool logFPS = false;
        if ( logFPS )
        {
          if ( m_logCollector )
          {
            m_logCollector->add( buffer );
            m_logCollector->flush();
          }
        }
        else FABRIC_DEBUG_LOG( "%s", buffer );

        memcpy( &m_fpsStart, &fpsNow, sizeof(m_fpsStart) );
        m_fpsCount = 0;
      }
      
      m_logCollector->flush();

#if defined(FABRIC_OS_WINDOWS)
      ::_InterlockedIncrement( &m_redrawFinishedCallbackPendingInvokeCount );
#else
      __sync_fetch_and_add( &m_redrawFinishedCallbackPendingInvokeCount, 1 );
#endif

      NPN_PluginThreadAsyncCall( m_npp, &ViewPort::IssuePendingRedrawFinishedCallbacks, this );
    }
    
    void ViewPort::setRedrawFinishedCallback( NPObject *npObject )
    {
      if ( m_redrawFinishedCallback != npObject )
      {
        if ( m_redrawFinishedCallback )
          NPN_ReleaseObject(m_redrawFinishedCallback);
        m_redrawFinishedCallback = npObject;
        if ( m_redrawFinishedCallback )
          NPN_RetainObject(m_redrawFinishedCallback);
      }
    }
    
    void ViewPort::didResize( size_t width, size_t height )
    {
      m_windowNode->setData( "width", 0, &width );
      m_windowNode->setData( "height", 0, &height );    
    }
    
    NPError ViewPort::nppGetValue( NPPVariable variable, void *value )
    {
      return NPERR_INVALID_PARAM;
    }
 
    NPError ViewPort::nppSetWindow( NPWindow *npWindow )
    {
      return NPERR_NO_ERROR;
    }
    
    int16_t ViewPort::nppHandleEvent( void *event )
    {
      return false;
    }

    NPError ViewPort::nppDestroy( NPSavedData** save )
    {
      if ( m_timer )
      {
        Util::UnorderedMap< uint32_t, ViewPort *> &timers = s_timers[m_npp];
        Util::UnorderedMap< uint32_t, ViewPort *>::iterator it = timers.find( m_timer );
        FABRIC_ASSERT( it != timers.end() );
        timers.erase( it );
        NPN_UnscheduleTimer( m_npp, m_timer );
      }
      
      return NPERR_NO_ERROR;
    }

    ViewPort::Timers ViewPort::s_timers;

    RC::Handle<DG::Node> ViewPort::getWindowNode() const
    {
      return m_windowNode;
    }
    
    RC::Handle<DG::Event> ViewPort::getRedrawEvent() const
    {
      return m_redrawEvent;
    }

    RC::ConstHandle<Interface> ViewPort::getInterface() const
    {
      return m_interface;
    }
    
    RC::Handle<JSON::Value> ViewPort::jsonExecGetFPS() const
    {
      return JSON::Scalar::Create( m_fps );
    }

    RC::Handle<JSON::Object> ViewPort::jsonDesc() const
    {
      size_t width, height;
      getWindowSize( width, height );
      
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( "fps", JSON::Scalar::Create( m_fps ) );
      result->set( "width", JSON::Integer::Create( width ) );
      result->set( "height", JSON::Integer::Create( height ) );
      result->set( "windowNode", JSON::String::Create( m_windowNode->getName() ) );
      result->set( "redrawEvent", JSON::String::Create( m_redrawEvent->getName() ) );
      return result;
    }

    RC::ConstHandle<JSON::Value> ViewPort::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;
      if ( cmd == "needsRedraw" )
        needsRedraw();
      else if ( cmd == "getFPS" )
        result = jsonExecGetFPS();
      else if ( cmd == "addPopUpMenuItem" )
        jsonExecAddPopupItem( arg );
      else throw Exception( "unrecognized command" );
      return result;
    }
    
    void ViewPort::jsonExecAddPopupItem( RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Object> argObject = arg->toObject();
      
      PopUpItem popUpItem;
      popUpItem.desc = argObject->get("desc")->toString()->value();
      popUpItem.value = argObject->get("arg");
      m_popUpItems.push_back( popUpItem );
    }
    
    void ViewPort::jsonNotifyPopUpItem( RC::ConstHandle<JSON::Value> const &arg ) const
    {
      jsonNotify( "popUpMenuItemSelected", arg );
    }

    void ViewPort::drawWatermark( size_t width, size_t height )
    {
      try
      {
        if ( !m_watermarkShaderProgram )
        {
          GLuint vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
          if ( !vertexShaderID )
            throw Exception( "glCreateShader( GL_VERTEX_SHADER ) failed" );
          static const GLsizei numVertexShaderSources = 1;
          GLchar const *vertexShaderSources[numVertexShaderSources] = {
            "\
attribute vec4 a_position;\n\
attribute vec4 a_texCoord;\n\
void main() {\n\
  gl_TexCoord[0].st = a_texCoord.xy;\n\
  gl_Position = a_position;\n\
}\n\
            "
          };
          GLint vertexShaderSourceLengths[numVertexShaderSources];
          vertexShaderSourceLengths[0] = strlen( vertexShaderSources[0] );
          glShaderSource( vertexShaderID, numVertexShaderSources, vertexShaderSources, vertexShaderSourceLengths );
          glCompileShader( vertexShaderID );
          GLint vertexShaderCompileResult[1];
          glGetShaderiv( vertexShaderID, GL_COMPILE_STATUS, vertexShaderCompileResult );
          if ( !vertexShaderCompileResult[0] )
            throw Exception( "vertex shader compilation failure" );

          GLuint fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );
          if ( !fragmentShaderID )
            throw Exception( "glCreateShader( GL_FRAGMENT_SHADER ) failed" );
          static const GLsizei numFragmentShaderSources = 1;
          GLchar const *fragmentShaderSources[numFragmentShaderSources] = {
            "\
uniform sampler2D u_rgbaImage;\n\
void main()\n\
{\n\
  gl_FragColor = texture2D( u_rgbaImage, gl_TexCoord[0].st );\n\
}\n\
            "
          };
          GLint fragmentShaderSourceLengths[numFragmentShaderSources];
          fragmentShaderSourceLengths[0] = strlen( fragmentShaderSources[0] );
          glShaderSource( fragmentShaderID, numFragmentShaderSources, fragmentShaderSources, fragmentShaderSourceLengths );
          glCompileShader( fragmentShaderID );
          GLint fragmentShaderCompileResult[1];
          glGetShaderiv( fragmentShaderID, GL_COMPILE_STATUS, fragmentShaderCompileResult );
          if ( !fragmentShaderCompileResult[0] )
            throw Exception( "fragment shader compilation failure" );
          
          m_watermarkShaderProgram = glCreateProgram();
          if ( !m_watermarkShaderProgram )
            throw Exception( "glCreateProgram() failed" );
          glAttachShader( m_watermarkShaderProgram, vertexShaderID );
          glAttachShader( m_watermarkShaderProgram, fragmentShaderID );
          glLinkProgram( m_watermarkShaderProgram );
          GLint linkResult[1];
          glGetProgramiv( m_watermarkShaderProgram, GL_LINK_STATUS, linkResult );
          if ( !linkResult[0] )
            throw Exception( "link failure" );
            
          glDeleteShader( fragmentShaderID );
          glDeleteShader( vertexShaderID );
        }
        glUseProgram( m_watermarkShaderProgram );
        
        if ( !m_watermarkTextureBuffer )
        {
          glGenTextures( 1, &m_watermarkTextureBuffer );
          if ( !m_watermarkTextureBuffer )
            throw Exception( "glGenTextures() failed" );
          glActiveTexture( GL_TEXTURE0 );
          glBindTexture( GL_TEXTURE_2D, m_watermarkTextureBuffer );
    
          glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
          glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, watermarkWidth, watermarkHeight, 0, GL_RGBA, GL_BYTE, watermarkData );

          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        }
        
      }
      catch ( Exception e )
      {
        throw "ViewPort::drawWatermark( " + _(width) + ", " + _(height) + " ): " + e;
      }
/*
  var String name = 'a_position';
  var Integer posLocation = glGetAttribLocation(program, name);
  name = 'a_texCoord';
  var Integer texLocation = glGetAttribLocation(program, name);
  name = 'u_rgbaImage';
  var Integer smpLocation = glGetUniformLocation(program, name);

  glPushAttrib(GL_TEXTURE_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glEnable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  // PT 29-06-11 : I'm not sure why this is here.
  // Do we need to set the texture unit here?
//  glUniform1i(smpLocation, GL_TEXTURE0);
  
  // Draw a quad in the upper left with debugging info
  var Vec3 p[4];
  p[0] = Vec3(tl.x, tl.y, 0.0);
  p[1] = Vec3(br.x, tl.y, 0.0);
  p[2] = Vec3(br.x, br.y, 0.0);
  p[3] = Vec3(tl.x, br.y, 0.0);
  
  var Vec2 t[4];
  if(flipY){
    t[0] = Vec2(0.0, 0.0);
    t[1] = Vec2(1.0, 0.0);
    t[2] = Vec2(1.0, 1.0);
    t[3] = Vec2(0.0, 1.0);
  }else{
    t[0] = Vec2(0.0, 1.0);
    t[1] = Vec2(1.0, 1.0);
    t[2] = Vec2(1.0, 0.0);
    t[3] = Vec2(0.0, 0.0);
  }
  
  var Integer idx[6];
  idx[0] = 0; idx[1] = 2; idx[2] = 1;
  idx[3] = 0; idx[4] = 3; idx[5] = 2;

  var Data NULL;

  var Size bufferID0;
  var Size bufferID1;
  var Size bufferID2;
  var Size bufferIDArray[];
  bufferIDArray.resize(1);
  glGenBuffers(1, bufferIDArray);
  bufferID0 = bufferIDArray[0];
  glGenBuffers(1, bufferIDArray);
  bufferID1 = bufferIDArray[0];
  glGenBuffers(1, bufferIDArray);
  bufferID2 = bufferIDArray[0];
  
  glBindBuffer(GL_ARRAY_BUFFER, bufferID0);
  glBufferData(GL_ARRAY_BUFFER, p.dataSize(), p.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(posLocation);
  glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, bufferID1);
  glBufferData(GL_ARRAY_BUFFER, t.dataSize(), t.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(texLocation);
  glVertexAttribPointer(texLocation, 2, GL_FLOAT, GL_FALSE, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, bufferID2);
  glBufferData(GL_ARRAY_BUFFER, idx.dataSize(), idx.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID2);
  glDrawElements(GL_TRIANGLES, idx.size(), GL_UNSIGNED_INT, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glPopAttrib();
//  report("<<<drawTexture");
*/
    }
  };
};
