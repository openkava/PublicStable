#ifndef _FABRIC_AST_GLOBAL_LIST_NODE_H
#define _FABRIC_AST_GLOBAL_LIST_NODE_H

#include <Fabric/Core/AST/ListNode.h>
#include <Fabric/Core/AST/Global.h>

namespace Fabric
{
  namespace AST
  {
    class GlobalList: public ListNode
    {
    public:
    
      virtual std::string localDesc() const;

      static RC::Handle<GlobalList> Create( CG::Location const &location )
      {
        return new GlobalList( location );
      }
      
      static RC::Handle<GlobalList> Create( CG::Location const &location, RC::ConstHandle<Global> global, RC::ConstHandle<GlobalList> const &remaining )
      {
        return new GlobalList( location, global, remaining );
      }
      
      RC::ConstHandle<Global> item( size_t index ) const
      {
        return RC::ConstHandle<Global>::StaticCast( ListNode::item(index) );
      }
      
      void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;

    protected:
    
      GlobalList( CG::Location const &location );
      GlobalList( CG::Location const &location, RC::ConstHandle<Global> global, RC::ConstHandle<GlobalList> const &remaining );
    };
  };
};

#endif //_FABRIC_AST_GLOBAL_LIST_NODE_H
