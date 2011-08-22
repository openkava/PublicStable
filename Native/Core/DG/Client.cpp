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

    std::string Client::jsonExec( char const *jsonEncodedCommandsData, size_t jsonEncodedCommandsLength ) const
    {
      Context::NotificationBracket notificationBracket( m_context );
      
      RC::ConstHandle<JSON::Array> cmdsAndArgsJSONArray;
      cmdsAndArgsJSONArray = JSON::decode( jsonEncodedCommandsData, jsonEncodedCommandsLength )->toArray();
      size_t cmdsAndArgsJSONArraySize = cmdsAndArgsJSONArray->size();
      
      RC::Handle<JSON::Array> resultJSONArray = JSON::Array::Create();
      for ( size_t i=0; i<cmdsAndArgsJSONArraySize; ++i )
      {
        RC::ConstHandle<JSON::Object> cmdAndArgJSONObject = cmdsAndArgsJSONArray->get(i)->toObject();
        
        RC::Handle<JSON::Object> result = JSON::Object::Create();
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
            RC::ConstHandle<JSON::Value> callResult = m_context->jsonRoute( dst, 0, cmd, arg );
            if ( callResult )
              result->set( "result", callResult );
            resultJSONArray->push_back( result );
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
          result->set( "exception", JSON::String::Create( e.getDesc().getData(), e.getDesc().getLength() ) );
          resultJSONArray->push_back( result );
          break;
        }
      }
      
      CompiledObject::PrepareForExecution();
      
      return JSON::encode( resultJSONArray );
    }
    
    void Client::notifyInitialState() const
    {
      RC::Handle<JSON::Array> srcJSONArray = JSON::Array::Create();
      
      RC::Handle<JSON::Object> notification = JSON::Object::Create();
      notification->set( "src", srcJSONArray );
      notification->set( "cmd", JSON::String::Create( "state" ) );
      notification->set( "arg", jsonDesc() );
      
      RC::Handle<JSON::Array> notifications = JSON::Array::Create();
      notifications->push_back( notification );

      notify( JSON::encode( notifications ) );
    }

    RC::ConstHandle<JSON::Value> Client::jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;
      if ( dst.size() - dstOffset == 1 && dst[dstOffset] == "client" )
      {
        try
        {
          result = jsonExec( cmd, arg );
        }
        catch ( Exception e )
        {
          throw "'client': command " + _(cmd) + ": " + e;
        }
      }
      else result = m_context->jsonRoute( dst, dstOffset, cmd, arg );
      return result;
    }
    
    RC::ConstHandle<JSON::Value> Client::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      throw Exception( "unknown command" );
    }
    
    RC::Handle<JSON::Object> Client::jsonDesc() const
    {
      return m_context->jsonDesc();
    }
  };
};
