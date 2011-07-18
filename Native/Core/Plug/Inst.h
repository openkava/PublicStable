/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_PLUG_INST_H
#define _FABRIC_PLUG_INST_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Core/Plug/Desc.h>
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/Plug/Helpers.h>

namespace Fabric
{
  namespace JSON
  {
    class CommandChannel;
  };
  
  namespace CG
  {
    class Manager;
    class ModuleBuilder;
  };
  
  namespace AST
  {
    class GlobalVector;
  };

  namespace DG
  {
    class Context;
  };
  
  namespace LIB
  {
    class Value;
    class Object;
  };
  
  namespace Plug
  {
    class Inst : public RC::Object
    {
      typedef std::map< std::string, SOLibHandle > ResolvedNameToSOLibHandleMap;
      
      typedef std::map< std::string, void * > ExternalFunctionMap;
      
    public:
    
      static RC::Handle<Inst> Create( std::string const &name, std::string const &jsonDesc, RC::Handle<DG::Context> const &dgContext, std::vector<std::string> const &pluginDirs, RC::Handle<JSON::CommandChannel> const &jsonCommandChannel );
      
      std::string const &getJSONDesc() const
      {
        return m_jsonDesc;
      }
      
      Desc const &getDesc() const
      {
        return m_desc;
      }
      
      std::string const &getCode() const
      {
        return m_code;
      }
      
      void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder ) const;
      void *llvmResolveExternalFunction( std::string const &name ) const;

      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
    protected:
    
      Inst( std::string const &name, std::string const &jsonDesc, RC::Handle<DG::Context> const &dgContext, std::vector<std::string> const &pluginDirs );
      ~Inst();
      
    private:
    
      bool hasMethod( std::string const &methodName ) const;
      void enumerateMethods( std::vector<std::string> &result ) const;
      //RC::Handle<LIB::Value> invokeMethod( std::string const &methodName, std::vector< RC::Handle<LIB::Value> > const &args );
    
      std::string m_name;
      mutable bool m_disabled;
      RC::Handle<CG::Manager> m_cgManager;
      std::string m_jsonDesc;
      Desc m_desc;
      std::string m_code;
      RC::Handle<AST::GlobalVector> m_ast;
      CG::Diagnostics m_diagnostics;
      ResolvedNameToSOLibHandleMap m_resolvedNameToSOLibHandleMap;
      std::vector<SOLibHandle> m_orderedSOLibHandles;
      ExternalFunctionMap m_externalFunctionMap;
      //RC::Handle<LIB::Object> m_fabricLIBObject;
      //MethodMap m_methodMap;
      std::string m_jsConstants;
    };
  };
};

#endif //_FABRIC_PLUG_INST_H
