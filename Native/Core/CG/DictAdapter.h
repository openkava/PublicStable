/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_DICT_ADAPTER_H
#define _FABRIC_CG_DICT_ADAPTER_H

#include <Fabric/Core/CG/Adapter.h>

namespace Fabric
{
  namespace RT
  {
    class DictDesc;
  };
  
  namespace CG
  {
    class ComparableAdapter;
    
    class DictAdapter : public Adapter
    {
      friend class Manager;
    
    public:

      // Adapter
    
      virtual void llvmRetain( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const;
      virtual void llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const;
      virtual void llvmRelease( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const;

      virtual llvm::Constant *llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const;
      virtual llvm::Constant *llvmDefaultRValue( BasicBlockBuilder &basicBlockBuilder ) const;
      virtual llvm::Constant *llvmDefaultLValue( BasicBlockBuilder &basicBlockBuilder ) const;
      
      virtual void llvmCompileToModule( ModuleBuilder &moduleBuilder ) const;
      virtual void *llvmResolveExternalFunction( std::string const &functionName ) const;

      virtual std::string toString( void const *data ) const;

      // DictAdapter

      /*
      virtual llvm::Value *llvmGetRValue(
        CG::BasicBlockBuilder &basicBlockBuilder,
        llvm::Value *dictRValue,
        llvm::Value *keyRValue,
        CG::Location const *location
        ) const;
      virtual llvm::Value *llvmGetLValue(
        CG::BasicBlockBuilder &basicBlockBuilder,
        llvm::Value *dictLValue,
        llvm::Value *keyRValue,
        CG::Location const *location
        ) const;
      */

    protected:
    
      DictAdapter(
        RC::ConstHandle<Manager> const &manager,
        RC::ConstHandle<RT::DictDesc> const &dictDesc
        );
      
      virtual llvm::Type const *buildLLVMRawType( RC::Handle<Context> const &context ) const;

      llvm::Value *llvmCallSize( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const;
      
    private:
    
      RC::ConstHandle<RT::DictDesc> m_dictDesc;
      RC::ConstHandle<ComparableAdapter> m_keyAdapter;
      RC::ConstHandle<Adapter> m_valueAdapter;
   };
  };
};

#endif //_FABRIC_CG_DICT_ADAPTER_H
