#include "GlobalList.h"

namespace Fabric
{
  namespace AST
  {
    GlobalList::GlobalList( CG::Location const &location )
      : ListNode( location )
    {
    }

    GlobalList::GlobalList( CG::Location const &location, RC::ConstHandle<Global> global, RC::ConstHandle<GlobalList> const &remaining )
      : ListNode( location, global, remaining )
    {
    }
    
    std::string GlobalList::localDesc() const
    {
      return "GlobalList";
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
