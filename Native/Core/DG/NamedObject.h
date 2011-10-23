/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_DG_NAMED_OBJECT_H
#define _FABRIC_DG_NAMED_OBJECT_H

#include <Fabric/Core/DG/CompiledObject.h>
#include <Fabric/Core/DG/Context.h>

#include <string>

namespace Fabric
{
  namespace Util
  {
    class JSONGenerator;
  };
  
  namespace JSON
  {
    class Value;
    class Object;
    class Array;
  };
  
  namespace DG
  {
    class NamedObject : public CompiledObject
    {
    public:

      std::string const &getName() const;
      
      virtual bool isNamedObject() const { return true; }
      virtual bool isContainer() const { return false; }
      virtual bool isOperator() const { return false; }
            
      virtual void jsonRoute(
        std::vector<std::string> const &dst,
        size_t dstOffset,
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
        
      virtual void jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonDesc( Util::JSONGenerator &resultJG ) const;
      virtual void jsonDesc( Util::JSONObjectGenerator &resultJOG ) const;
      virtual void jsonDescType( Util::JSONGenerator &resultJG ) const = 0;
      
    protected:
    
      NamedObject( std::string const &friendlyName, RC::Handle<Context> const &context );
      
      void jsonNotify( char const *cmdData, size_t cmdLength, Util::SimpleString const *arg = 0 ) const;
      void jsonNotifyDelta( Util::SimpleString const &deltaJSON ) const;
      void jsonNotifyMemberDelta( char const *memberData, size_t memberLength, Util::SimpleString const &memberDeltaJSON ) const;
      virtual void jsonNotifyErrorDelta() const;
      
    private:
    
      Context *m_context;
      std::vector<std::string> m_notificationSrc;
    };
  };
};

#endif //_FABRIC_DG_NAMED_OBJECT_H
