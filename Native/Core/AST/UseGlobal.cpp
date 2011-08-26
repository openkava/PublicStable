/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/UseGlobal.h>
#include <Fabric/Core/AST/UseVector.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( UseGlobal );
    
    RC::ConstHandle<UseGlobal> UseGlobal::Create(
      CG::Location const &location,
      RC::ConstHandle<UseVector> const &useVector
      )
    {
      return new UseGlobal( location, useVector );
    }

    UseGlobal::UseGlobal(
      CG::Location const &location,
      RC::ConstHandle<UseVector> const &useVector
      )
      : Global( location )
      , m_useVector( useVector )
    {
    }
    
    void UseGlobal::appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const
    {
      Global::appendJSONMembers( jsonObjectGenerator );
      m_useVector->appendJSON( jsonObjectGenerator.makeMember( "useVector" ) );
    }
    
    void UseGlobal::collectUses( UseNameToLocationMap &uses ) const
    {
      m_useVector->collectUses( uses );
    }
    
    void UseGlobal::registerTypes( RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
    }
    
    void UseGlobal::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
    }
  };
};
