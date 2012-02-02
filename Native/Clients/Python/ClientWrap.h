/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_PYTHON_CLIENT_WRAP_H
#define _FABRIC_PYTHON_CLIENT_WRAP_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Core/Util/Mutex.h>
#include <Fabric/Core/Util/Cond.h>
#include <Fabric/Core/Util/TLS.h>

#include <map>

namespace Fabric
{
  namespace Python
  {
    class Client;

    class ClientWrap : public RC::Object
    {
    public:
    
      ClientWrap();
      virtual ~ClientWrap();

      virtual void notify( Util::SimpleString const &jsonEncodedNotifications ) const;
      void setJSONNotifyCallback( void (*callback)(const char *) );
      void jsonExecAndAllocCStr( char const *data, const size_t length, const char **str );
      void freeJsonCStr( const char *str );

    protected:
   
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

      RC::Handle<Client> m_client;
    };
  };
};

#endif //_FABRIC_PYTHON_CLIENT_WRAP_H
