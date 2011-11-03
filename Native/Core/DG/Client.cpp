/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/Client.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/DG/CompiledObject.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Base/JSON/Encode.h>
#include <Fabric/Base/JSON/Decode.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/JSONGenerator.h>
#include <Fabric/Core/Util/Timer.h>

namespace Fabric
{
  namespace DG
  {
    RC::Handle<Context> Client::getContext() const
    {
      return m_context;
    }
    
    Client::Client( RC::Handle<Context> const &context )
      : m_context( context )
    {
      m_context->registerClient( this );
    }
    
    Client::~Client()
    {
      m_context->unregisterClient( this );
    }

    void Client::jsonExec(
      char const *jsonEncodedCommandsData,
      size_t jsonEncodedCommandsLength,
      Util::JSONGenerator &resultJG
      ) const
    {
      Context::NotificationBracket notificationBracket( m_context );
      
      RC::ConstHandle<JSON::Array> cmdsAndArgsJSONArray;
      cmdsAndArgsJSONArray = JSON::decode( jsonEncodedCommandsData, jsonEncodedCommandsLength )->toArray();
      size_t cmdsAndArgsJSONArraySize = cmdsAndArgsJSONArray->size();
      
      Util::JSONArrayGenerator resultJSONArray = resultJG.makeArray();
      for ( size_t i=0; i<cmdsAndArgsJSONArraySize; ++i )
      {
        RC::ConstHandle<JSON::Object> cmdAndArgJSONObject = cmdsAndArgsJSONArray->get(i)->toObject();
        
        Util::JSONObjectGenerator resultJSONObject = resultJSONArray.makeElement().makeObject();
        try
        {
          std::vector<std::string> dst;
          try
          {
            RC::ConstHandle<JSON::Array> dstJSONArray = cmdAndArgJSONObject->get( "dst" )->toArray();
            size_t dstJSONArraySize = dstJSONArray->size();
            for ( size_t i=0; i<dstJSONArraySize; ++i )
            {
              try
              {
                dst.push_back( dstJSONArray->get(i)->toString()->value() );
              }
              catch ( Exception e )
              {
                throw "index "+_(i)+": " + e;
              }
            }
          }
          catch ( Exception e )
          {
            throw "'dst': " + e;
          }
          
          std::string cmd;
          try
          {
            cmd = cmdAndArgJSONObject->get( "cmd" )->toString()->value();
          }
          catch ( Exception e )
          {
            throw "'cmd': " + e;
          }
          
          RC::ConstHandle<JSON::Value> arg = cmdAndArgJSONObject->maybeGet( "arg" );
        
          try
          {
            Util::SimpleString resultJSON;
            Util::JSONGenerator resultJG( &resultJSON );
            Util::JSONArrayGenerator resultJAG = resultJG.makeArray();
            m_context->jsonRoute( dst, 0, cmd, arg, resultJAG );
            if ( resultJAG.getCount() > 0 )
            {
              resultJAG.flush();
              resultJG.flush();
              Util::JSONGenerator memberJG = resultJSONObject.makeMember( "result", 6 );
              memberJG.appendJSON( resultJSON.data()+1, resultJSON.length()-2 );
            }
          }
          catch ( Exception e )
          {
            std::string dstString;
            for ( size_t i=0; i<dst.size(); ++i )
            {
              dstString += dst[i];
              dstString += '.';
            }
            
            std::string prefix;
            if ( arg )
              prefix = dstString + cmd + "(" + _( JSON::encode(arg), 32 ) + "): ";
            else prefix = dstString + cmd + "(): ";
            
            throw prefix + e;
          }
        }
        catch ( Exception e )
        {
          resultJSONObject.makeMember( "exception", 9 ).makeString( e.getDesc() );
          break;
        }
      }
      
      CompiledObject::PrepareForExecution();
    }
    
    void Client::notifyInitialState() const
    {
      Util::SimpleString json;
      {
        Util::JSONGenerator arrayJG( &json );
        Util::JSONArrayGenerator arrayJAG = arrayJG.makeArray();
        
        {
          Util::JSONGenerator jg = arrayJAG.makeElement();
          Util::JSONObjectGenerator jog = jg.makeObject();
          
          {
            Util::JSONGenerator srcJG = jog.makeMember( "src", 3 );
            Util::JSONArrayGenerator srcJAG = srcJG.makeArray();
          }
          
          {
            Util::JSONGenerator cmdJG = jog.makeMember( "cmd", 3 );
            cmdJG.makeString( "state", 5 );
          }
          
          {
            Util::JSONGenerator argJG = jog.makeMember( "arg", 3 );
            jsonDesc( argJG );
          }
        }
      }
      notify( json );
    }

    void Client::jsonRoute(
      std::vector<std::string> const &dst,
      size_t dstOffset,
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( dst.size() - dstOffset == 1 && dst[dstOffset] == "client" )
      {
        try
        {
          jsonExec( cmd, arg, resultJAG );
        }
        catch ( Exception e )
        {
          throw "'client': command " + _(cmd) + ": " + e;
        }
      }
      else m_context->jsonRoute( dst, dstOffset, cmd, arg, resultJAG );
    }
    
    void Client::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      throw Exception( "unknown command" );
    }
    
    void Client::jsonDesc( Util::JSONGenerator &resultJG ) const
    {
      return m_context->jsonDesc( resultJG );
    }
  };
};
