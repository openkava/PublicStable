/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_NPAPI_NP_VIEW_PORT_H
#define _FABRIC_NPAPI_NP_VIEW_PORT_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Core/Util/UnorderedMap.h>

#include <GL/glew.h>
#include <npapi/npapi.h>
#include <npapi/npruntime.h>
#if defined( FABRIC_POSIX )
# include <sys/time.h>
#elif defined( FABRIC_WIN32 )
# include <windows.h>
#include <GL/glew.h>
#endif
#include <vector>

namespace Fabric
{
  namespace JSON
  {
    class Value;
    class Object;
  };
  
  namespace MT
  {
    class LogCollector;
  };
  
  namespace RT
  {
    class IntegerDesc;
  };
  
  namespace DG
  {
    class Context;
    class Node;
    class Event;
    class LogCollector;
  };
  
  namespace NPAPI
  {
    class Interface;
    
    class ViewPort : public RC::Object
    {
      friend class Interface;

    public:
      
      virtual void getWindowSize( size_t &width, size_t &height ) const = 0;
    
      RC::Handle<DG::Node> getWindowNode() const;
      RC::Handle<DG::Event> getRedrawEvent() const;
      
      float getFPS() const
      {
        return m_fps;
      }
    
      void setRedrawFinishedCallback( NPObject *npObject );
      virtual void needsRedraw() = 0;
      
      virtual NPError nppGetValue( NPPVariable variable, void *value );
      virtual NPError nppSetWindow( NPWindow *npWindow );
      virtual int16_t nppHandleEvent( void *event );
      virtual NPError nppDestroy( NPSavedData** save );
      
      RC::Handle<MT::LogCollector> getLogCollector() const;
      virtual void redrawFinished();
      
      virtual RC::ConstHandle<JSON::Value> jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      void jsonExecAddPopupItem( RC::ConstHandle<JSON::Value> const &arg );
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      void jsonNotify( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg ) const;
      void jsonNotifyPopUpItem( RC::ConstHandle<JSON::Value> const &arg ) const;

      virtual void pushOGLContext() = 0;
      virtual void popOGLContext() = 0;

      void drawWatermark( size_t width, size_t height );

      virtual std::string getPathFromSaveAsDialog( std::string const& defaultFilename, std::string const& extension ) = 0;

    protected:
    
      ViewPort( RC::ConstHandle<Interface> const &interface, uint32_t timerInterval = 0 );
      ~ViewPort();
      
      RC::ConstHandle<Interface> getInterface() const;
      
      virtual void didResize( size_t width, size_t height ); 

      virtual void timerFired();
      static void TimerFiredCallback( NPP npp, uint32_t timer );
    
      struct PopUpItem
      {
        std::string desc;
        RC::ConstHandle<JSON::Value> value;
      };
      typedef std::vector<PopUpItem> PopUpItems;

      PopUpItems m_popUpItems;

    private:
    
      RC::Handle<JSON::Value> jsonExecGetFPS() const;

      void issuePendingRedrawFinishedCallbacks();
      static void IssuePendingRedrawFinishedCallbacks( void *_this )
      {
        static_cast<ViewPort *>(_this)->issuePendingRedrawFinishedCallbacks();
      }
    
      NPP m_npp;
      std::string m_name;
      Interface const *m_interface;
      
      typedef Util::UnorderedMap< NPP, Util::UnorderedMap< uint32_t, ViewPort *> > Timers;
      uint32_t m_timer;
      static Timers s_timers;

      RC::Handle<MT::LogCollector> m_logCollector;
      
      RC::ConstHandle<RT::IntegerDesc> m_integerDesc;
      RC::Handle<DG::Node> m_windowNode;
      RC::Handle<DG::Event> m_redrawEvent;
      
      NPObject *m_redrawFinishedCallback;
#if defined(FABRIC_OS_WINDOWS)
      long volatile m_redrawFinishedCallbackPendingInvokeCount;
      LARGE_INTEGER m_fpsStart;
      double        m_fpsTimerFreq;
#else  
      size_t volatile m_redrawFinishedCallbackPendingInvokeCount;
      struct timeval m_fpsStart;
#endif
      
      unsigned m_fpsCount; 
      float m_fps;
      
      GLuint m_watermarkShaderProgram;
      GLuint m_watermarkTextureBuffer;
      GLuint m_watermarkPositionsBufferID;
      bool m_watermarkNeedPositionsVBOUpload;
      GLuint m_watermarkUVsBufferID;
      GLuint m_watermarkIndexesBufferID;
      size_t m_watermarkLastWidth;
      size_t m_watermarkLastHeight;
    };
  };
};

#endif //_FABRIC_NPAPI_NP_VIEW_PORT_H
