/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_PARAM_LIST_NODE_H
#define _FABRIC_AST_PARAM_LIST_NODE_H

#include <Fabric/Core/AST/ListNode.h>
#include <Fabric/Core/AST/Param.h>

namespace Fabric
{
  namespace AST
  {
    class ParamList: public ListNode
    {
    public:
    
      virtual std::string localDesc() const;

      static RC::Handle<ParamList> Create( CG::Location const &location )
      {
        return new ParamList( location );
      }
      
      static RC::Handle<ParamList> Create( CG::Location const &location, RC::ConstHandle<Param> const &item )
      {
        return new ParamList( location, item );
      }
      
      static RC::Handle<ParamList> Create( CG::Location const &location, RC::ConstHandle<Param> const &item, RC::ConstHandle<ParamList> const &remaining )
      {
        return new ParamList( location, item, remaining );
      }
      
      RC::ConstHandle<Param> item( size_t index ) const
      {
        return RC::ConstHandle<Param>::StaticCast( ListNode::item(index) );
      }
      
      std::vector< llvm::Type const * > getLLVMTypes() const;
      
      std::vector< CG::FunctionParam > getFunctionParams() const
      {
        std::vector< CG::FunctionParam > result;
        for ( size_t i=0; i<numItems(); ++i )
          result.push_back( item(i)->getFunctionParam() );
        return result;
      }
      
      std::vector< RC::ConstHandle<CG::Adapter> > getTypes() const
      {
        std::vector< RC::ConstHandle<CG::Adapter> > result;
        for ( size_t i=0; i<numItems(); ++i )
          result.push_back( item(i)->getType() );
        return result;
      }
      
      std::vector< CG::ExprType > getExprTypes() const
      {
        std::vector< CG::ExprType > result;
        for ( size_t i=0; i<numItems(); ++i )
          result.push_back( item(i)->getExprType() );
        return result;
      }
      
      std::string getTypeDesc() const;
      
    protected:
    
      ParamList( CG::Location const &location );
      ParamList( CG::Location const &location, RC::ConstHandle<Param> const &item );
      ParamList( CG::Location const &location, RC::ConstHandle<Param> const &item, RC::ConstHandle<ParamList> const &next );
    };
  };
  
  inline std::string _( RC::ConstHandle<AST::ParamList> const &paramList )
  {
    return paramList->localDesc();
  }
};

#endif //_FABRIC_AST_PARAM_LIST_NODE_H
