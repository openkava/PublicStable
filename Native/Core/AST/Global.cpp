/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "Global.h"

namespace Fabric
{
  namespace AST
  {
    Global::Global( CG::Location const &location )
      : Node( location )
    {
    }
    
    void Global::registerTypes( RC::Handle<RT::Manager> const &rtManager, CG::Diagnostics &diagnostics ) const
    {
    }
    
    void Global::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
    }

    Util::SimpleString const &Global::getJSONString() const
    {
      if ( m_jsonString.getLength() == 0 )
        appendJSONString( m_jsonString );
      return m_jsonString;
    }
  };
};
