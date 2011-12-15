/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_GC_OBJECT_H
#define _FABRIC_GC_OBJECT_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>

#include <vector>
#include <string>

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
  
  namespace GC
  {
    class Container;

    class Object : public RC::Object
    {
      friend class Container;
      
    public:

      Object();
      ~Object();
      
      void reg( Container *container, std::string const &id_ );

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
    
      void dispose();
      
    private:
    
      Container *m_container;
      std::string m_id;
    };
  }
}

#endif //_FABRIC_GC_OBJECT_H
