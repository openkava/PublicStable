#ifndef _FABRIC_AST_GLOBAL_LIST_NODE_H
#define _FABRIC_AST_GLOBAL_LIST_NODE_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Core/AST/Global.h>

namespace Fabric
{
  namespace AST
  {
    class GlobalList: public Node
    {
    public:
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;

      static RC::Handle<GlobalList> Create( CG::Location const &location )
      {
        return new GlobalList( location );
      }
      
      void append( RC::ConstHandle<Global> global )
      {
        m_items.push_back( global );
      }
      
      size_t numItems() const
      {
        return m_items.size();
      }
      RC::ConstHandle<Global> item( size_t index ) const
      {
        return m_items[index];
      }
      
      void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;

    protected:
    
      GlobalList( CG::Location const &location );
      
      std::vector< RC::ConstHandle<Global> > m_items;
    };
  };
};

#endif //_FABRIC_AST_GLOBAL_LIST_NODE_H
