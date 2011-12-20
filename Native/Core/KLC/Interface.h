/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KLC_INTERFACE_H
#define _FABRIC_KLC_INTERFACE_H

#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/GC/Container.h>
#include <Fabric/Core/CG/CompileOptions.h>
#include <Fabric/Core/CG/Manager.h>

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
  
  namespace KLC
  {
    class Interface
    {
    public:
    
      Interface(
        GC::Container *gcContainer,
        RC::Handle<CG::Manager> const &cgManager,
        CG::CompileOptions const &compileOptions
        );
    
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
      
      void jsonExecCreateCompilation(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
      void jsonExecCreateExecutable(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
      void jsonExecCreateMapOperator(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
      void jsonExecCreateReduceOperator(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
      void jsonExecCreateArrayGeneratorOperator(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
      void jsonExecCreateValueMapOperator(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
        
    private:
    
      GC::Container *m_gcContainer;
      RC::Handle<CG::Manager> m_cgManager;
      CG::CompileOptions m_compileOptions;
    };
  };
};

#endif //_FABRIC_KLC_INTERFACE_H
