/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_ALIAS_H
#define _FABRIC_AST_ALIAS_H

#include <Fabric/Core/AST/Global.h>

namespace Fabric
{
  namespace CG
  {
    class Adapter;
    class Manager;
  };
  
  namespace AST
  {
    class Alias : public Global
    {
      FABRIC_AST_NODE_DECL( Alias );
      
    public:

      static RC::Handle<Alias> Create(
        CG::Location const &location,
        std::string const &name,
        std::string const &adapterName
        )
      {
        return new Alias( location, name, adapterName );
      }

      RC::Handle<JSON::Object> toJSON() const;
      
      virtual void registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const;
      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const;
      
    protected:
    
      Alias(
        CG::Location const &location,
        std::string const &name,
        std::string const &adapterName
        );
    
    private:
    
      std::string m_name;
      std::string m_adapterName;
    };
  };
};

#endif //_FABRIC_AST_ALIAS_H
