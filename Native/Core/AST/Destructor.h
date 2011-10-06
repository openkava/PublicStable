/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_DESTRUCTOR_H
#define _FABRIC_AST_DESTRUCTOR_H

#include <Fabric/Core/AST/Function.h>

namespace Fabric
{
  namespace AST
  {
    class Destructor : public Function
    {
      FABRIC_AST_NODE_DECL( Destructor );

    public:

      static RC::ConstHandle<Destructor> Create(
        CG::Location const &location,
        std::string const &thisTypeName,
        std::string const &entryName,
        RC::ConstHandle<CompoundStatement> const &body
        );

      virtual bool isDestructor() const { return true; }
      
      std::string getThisTypeName() const;
      virtual std::string const *getFriendlyName( RC::Handle<CG::Manager> const &cgManager ) const;
      virtual std::string getEntryName( RC::Handle<CG::Manager> const &cgManager ) const;
      virtual RC::ConstHandle<ParamVector> getParams( RC::Handle<CG::Manager> const &cgManager ) const;
      
    protected:
    
      Destructor(
        CG::Location const &location,
        std::string const &thisTypeName,
        std::string const &entryName,
        RC::ConstHandle<CompoundStatement> const &body
        );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator, bool includeLocation ) const;

      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const;
      
    private:
    
      std::string m_thisTypeName;
      std::string m_entryName;
    };
  };
};

#endif //_FABRIC_AST_DESTRUCTOR_H
