/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_PYTHON_CLIENT_H
#define _FABRIC_PYTHON_CLIENT_H

#include <Fabric/Core/DG/Client.h>
#include <Fabric/Core/Util/Log.h>

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
      void notifyInitialState() const
      {
        DG::Client::notifyInitialState();
      }

      void setJSONNotifyCallback( void (*callback)(const char *) );

    protected:
   
      Client( RC::Handle<DG::Context> const &context );
      //~Client();

      void (*m_notifyCallback)(const char *);
    };
  };
};

#endif //_FABRIC_PYTHON_CLIENT_H
