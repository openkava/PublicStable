/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_OBJECT_H
#define _FABRIC_MR_OBJECT_H

#include <Fabric/Core/GC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <string>
#include <vector>

namespace Fabric
{
  namespace Util
  {
    class JSONArrayGenerator;
  };
  
  namespace JSON
  {
    class Value;
  };
  
  namespace MR
  {
    class Object : public GC::Object
    {
      FABRIC_GC_OBJECT_GET_CLASS_DECL()
      
      // Virtual functions: Object
    
    public:

      virtual void jsonRoute(
        std::vector<std::string> const &dst,
        size_t dstOffset,
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
        
      virtual void jsonExec(
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
    protected:
    
      Object( GC::Object::Class const *myClass, GC::Container *container, std::string const &id_ ); 
    };
  };
};

#endif //_FABRIC_MR_OBJECT_H
