/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_PARAM_LIST_NODE_H
#define _FABRIC_AST_PARAM_LIST_NODE_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Core/AST/Param.h>

namespace Fabric
{
  namespace AST
  {
    class ParamList: public Node
    {
    public:
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;

      static RC::Handle<ParamList> Create( CG::Location const &location )
      {
        return new ParamList( location );
      }
      
      static RC::Handle<ParamList> Create( CG::Location const &location, RC::ConstHandle<Param> const &param )
      {
        RC::Handle<ParamList> result = Create( location );
        result->append( param );
        return result;
      }
      
      void append( RC::ConstHandle<Param> const &param )
      {
        m_items.push_back( param );
      }

      size_t numItems() const
      {
        return m_items.size();
      }
      
      RC::ConstHandle<Param> item( size_t index ) const
      {
        return m_items[index];
      }
      
      std::vector< llvm::Type const * > getLLVMTypes() const;
      
      std::vector< CG::FunctionParam > getFunctionParams( RC::Handle<CG::Manager> const &cgManager ) const
      {
        std::vector< CG::FunctionParam > result;
        for ( size_t i=0; i<numItems(); ++i )
          result.push_back( item(i)->getFunctionParam( cgManager ) );
        return result;
      }
      
      std::vector< RC::ConstHandle<CG::Adapter> > getTypes( RC::Handle<CG::Manager> const &cgManager ) const
      {
        std::vector< RC::ConstHandle<CG::Adapter> > result;
        for ( size_t i=0; i<numItems(); ++i )
          result.push_back( item(i)->getType( cgManager ) );
        return result;
      }
      
      std::vector< CG::ExprType > getExprTypes( RC::Handle<CG::Manager> const &cgManager ) const
      {
        std::vector< CG::ExprType > result;
        for ( size_t i=0; i<numItems(); ++i )
          result.push_back( item(i)->getExprType( cgManager ) );
        return result;
      }
      
      std::string getTypeDesc() const;
      
    protected:
    
      ParamList( CG::Location const &location );
      
      std::vector< RC::ConstHandle<Param> > m_items;
    };
  };
  
  inline std::string _( RC::ConstHandle<AST::ParamList> const &paramList )
  {
    return paramList->localDesc();
  }
};

#endif //_FABRIC_AST_PARAM_LIST_NODE_H
