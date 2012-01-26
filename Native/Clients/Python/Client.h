/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_PYTHON_CLIENT_H
#define _FABRIC_PYTHON_CLIENT_H

#include <Fabric/Core/DG/Client.h>
#include <Fabric/Core/Util/Log.h>

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

      void (*m_notifyCallback)(const char *);

      // map of const char * pointers that we have passed to Python
      // and their associated SimpleString, we can't delete these
      // until Python says it's finished with them
      typedef std::map<const char *, Util::SimpleString *> PassedStringMap;
      PassedStringMap m_passedStrings;
    };
  };
};

#endif //_FABRIC_PYTHON_CLIENT_H
