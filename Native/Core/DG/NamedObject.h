/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_DG_NAMED_OBJECT_H
#define _FABRIC_DG_NAMED_OBJECT_H

#include <Fabric/Core/DG/CompiledObject.h>

#include <string>

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
    
    class NamedObject : public CompiledObject
    {
    public:
    
      std::string const &getName() const;
      
      virtual bool isNamedObject() const { return true; }
      virtual bool isContainer() const { return false; }
      virtual bool isOperator() const { return false; }
            
      virtual RC::ConstHandle<JSON::Value> jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      virtual RC::ConstHandle<JSON::Value> jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      virtual void jsonNotifyErrorDelta() const;
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      virtual RC::ConstHandle<JSON::Value> jsonDescType() const = 0;
      
    protected:
    
      NamedObject( std::string const &friendlyName, RC::Handle<Context> const &context );
      
      void notify( std::string const &cmd, RC::ConstHandle<JSON::Object> const &arg = 0 ) const;
      void notifyDelta( RC::ConstHandle<JSON::Object> const &deltas ) const;
      void notifyDelta( std::string const &propertyName, RC::ConstHandle<JSON::Value> const &propertyValue ) const;
      
    private:
    
      RC::Handle<Context> m_context;
      std::string m_name;
    };
  };
};

#endif //_FABRIC_DG_NAMED_OBJECT_H
