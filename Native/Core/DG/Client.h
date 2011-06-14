/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_CLIENT_H
#define _FABRIC_DG_CLIENT_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <vector>

namespace Fabric
{
  namespace JSON
  {
    class Value;
    class Object;
    class Array;
  };
  
  namespace DG
  {
    class Context;
    
    class Client : public RC::Object
    {
    public:
    
      RC::Handle<Context> getContext() const;
      
      std::string jsonExec( char const *jsonEncodedCommandsData, size_t jsonEncodedCommandsLength ) const;

      virtual void notify( std::string const &jsonEncodedNotifications ) const = 0;
      
      RC::ConstHandle<JSON::Value> jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      virtual RC::ConstHandle<JSON::Value> jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      RC::Handle<JSON::Object> jsonDesc() const;

    protected:
    
      Client( RC::Handle<Context> const &context );
      ~Client();
      
      void notifyInitialState() const;
      
    private:
    
      RC::Handle<Context> m_context;
    };
  };
};

#endif //_FABRIC_DG_CONTEXT_H
