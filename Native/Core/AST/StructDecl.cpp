/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "StructDecl.h"
#include "StructDeclMemberList.h"

namespace Fabric
{
  namespace AST
  {
    StructDecl::StructDecl(
      CG::Location const &location,
      std::string const &name,
      RC::ConstHandle<StructDeclMemberList> const &structDeclMemberList
      )
      : Global( location )
      , m_name( name )
      , m_structDeclMemberList( structDeclMemberList )
    {
    }
    
    std::string StructDecl::localDesc() const
    {
      return "StructDecl( '" + m_name + "' )";
    }

    std::string StructDecl::deepDesc( std::string const &indent ) const
    {
      return indent + localDesc() + "\n"
        + m_structDeclMemberList->deepDesc( indent + "  " );
    }
    
    void StructDecl::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
    }
  };
};
