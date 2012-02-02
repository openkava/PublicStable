/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_PYTHON_CLIENT_H
#define _FABRIC_PYTHON_CLIENT_H

#include <Fabric/Core/DG/Client.h>
#include <Fabric/Core/Util/Mutex.h>
#include <Fabric/Core/Util/Cond.h>
#include <Fabric/Core/Util/TLS.h>

#include <map>

namespace Fabric
{
  namespace Python
  {
    class Client : public DG::Client
    {
    public:
    
      static RC::Handle<Client> Create( RC::Handle<DG::Context> const &context );
      static RC::Handle<Client> Create();
      
      virtual void notify( Util::SimpleString const &jsonEncodedNotifications ) const;
      void setJSONNotifyCallback( void (*callback)(const char *) );

      void jsonExecAndAllocCStr( char const *data, const size_t length, const char **str );
      void freeJsonCStr( const char *str );

    protected:
   
      Client( RC::Handle<DG::Context> const &context );
      virtual ~Client();

      static void ScheduleAsyncUserCallback(
          void *scheduleUserData,
          void (*callbackFunc)(void *),
          void *callbackFuncUserData
          );

      void (*m_notifyCallback)(const char *);

      // map of const char * pointers that we have passed to Python
      // and their associated SimpleString, we can't delete these
      // until Python says it's finished with them
      typedef std::map<const char *, Util::SimpleString *> PassedStringMap;
      PassedStringMap m_passedStrings;

      Util::Mutex m_closeMutex;
      Util::Cond m_closeCond;
      bool m_closed;
    };
  };
};

#endif //_FABRIC_PYTHON_CLIENT_H
