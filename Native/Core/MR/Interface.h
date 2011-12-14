/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_INTERFACE_H
#define _FABRIC_MR_INTERFACE_H

#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/GC/Container.h>
#include <Fabric/Core/RT/Manager.h>

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
    class Interface
    {
    public:
    
      Interface( RC::ConstHandle<RT::Manager> const &rtManager );
    
      void jsonRoute(
        std::vector<std::string> const &dst,
        size_t dstOffset,
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
        
      void jsonExec(
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
      void jsonExecCreateConstArrayProducer(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
        
    private:
    
      GC::Container m_gcContainer;
      RC::ConstHandle<RT::Manager> m_rtManager;
    };
  };
};

#endif //_FABRIC_MR_INTERFACE_H
