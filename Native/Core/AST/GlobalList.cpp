#include "GlobalList.h"

namespace Fabric
{
  namespace AST
  {
    GlobalList::GlobalList( CG::Location const &location )
      : Node( location )
    {
    }

    std::string GlobalList::localDesc() const
    {
      return "GlobalList";
    }
    
    std::string GlobalList::deepDesc( std::string const &indent ) const
    {
      std::string subIndent = indent + "  ";
      std::string result = indent + localDesc() + "\n";
      for ( size_t i=0; i<numItems(); ++i )
        result += item(i)->deepDesc( subIndent );
      return result;
    }

    void GlobalList::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( size_t i=0; i<numItems(); ++i )
        item(i)->llvmCompileToModule( moduleBuilder, diagnostics, false );
      for ( size_t i=0; i<numItems(); ++i )
        item(i)->llvmCompileToModule( moduleBuilder, diagnostics, true );
    }
  };
};
