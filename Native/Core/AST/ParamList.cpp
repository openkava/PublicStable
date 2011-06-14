/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include "ParamList.h"

namespace Fabric
{
  namespace AST
  {
    ParamList::ParamList( CG::Location const &location )
      : ListNode( location )
    {
    }
    
    ParamList::ParamList( CG::Location const &location, RC::ConstHandle<Param> const &item )
      : ListNode( location, item )
    {
    }
    
    ParamList::ParamList( CG::Location const &location, RC::ConstHandle<Param> const &item, RC::ConstHandle<ParamList> const &remaining )
      : ListNode( location, item, remaining )
    {
    }
    
    std::string ParamList::localDesc() const
    {
      return "ParamList";
    }

    std::vector< llvm::Type const * > ParamList::getLLVMTypes() const
    {
      std::vector< llvm::Type const * > result;
      for ( size_t i=0; i<numItems(); ++i )
        result.push_back( item(i)->getLLVMType() );
      return result;
    }
      
    std::string ParamList::getTypeDesc() const
    {
      std::string result;
      size_t count = numItems();
      for ( size_t i=0; i<count; ++i )
      {
        if ( i > 0 )
          result += "__";
        result += item(i)->getAdapter()->getCodeName();
      }
      return result;
    }
  };
};
