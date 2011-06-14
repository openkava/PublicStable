/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_ARRAY_ADAPTER_H
#define _FABRIC_CG_ARRAY_ADAPTER_H

#include <Fabric/Core/CG/Adapter.h>

namespace Fabric
{
  namespace RT
  {
    class ArrayDesc;
  };
  
  namespace CG
  {
    class BasicBlockBuilder;
    
    class ArrayAdapter : public Adapter
    {
    public:
      
      // Adapter
      
      virtual std::string toString( void const *data ) const;
    
      // ArrayAdapter
    
      RC::ConstHandle<Adapter> getMemberAdapter() const
      {
        return m_memberAdapter;
      }

      virtual llvm::Value *llvmConstIndexOp( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayRValue, llvm::Value *indexRValue ) const = 0;
      virtual llvm::Value *llvmNonConstIndexOp( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayLValue, llvm::Value *indexRValue ) const = 0;
      
    protected:
    
      ArrayAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::ArrayDesc> const &arrayDesc, Flags flags );
      
    private:
    
      RC::ConstHandle<RT::ArrayDesc> m_arrayDesc;
      RC::ConstHandle<Adapter> m_memberAdapter;
    };
  };
};

#endif //_FABRIC_CG_ARRAY_ADAPTER_H
