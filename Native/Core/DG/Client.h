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
  namespace Util
  {
    class JSONGenerator;
    class JSONArrayGenerator;
    class SimpleString;
  };
  
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
      
      void jsonExec(
        char const *jsonEncodedCommandsData,
        size_t jsonEncodedCommandsLength,
        Util::JSONGenerator &resultJG
        ) const;

      virtual void notify( Util::SimpleString const &jsonEncodedNotifications ) const = 0;
      
      void jsonRoute(
        std::vector<std::string> const &dst,
        size_t dstOffset,
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      virtual void jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonDesc( Util::JSONGenerator &resultJG ) const;

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
