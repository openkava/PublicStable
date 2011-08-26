#ifndef _FABRIC_CG_FIXED_ARRAY_ADAPTER_H
#define _FABRIC_CG_FIXED_ARRAY_ADAPTER_H

#include <Fabric/Core/CG/ArrayAdapter.h>

namespace Fabric
{
  namespace RT
  {
    class FixedArrayDesc;
  };
  
  namespace CG
  {
    class FixedArrayAdapter : public ArrayAdapter
    {
      friend class Manager;
      
    public:
      
      // Adapter
      
      virtual void llvmInit( BasicBlockBuilder &basicBlockBuilder, llvm::Value *value ) const;
      virtual void llvmRetain( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const;
      virtual void llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const;
      virtual void llvmRelease( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const;

      virtual llvm::Constant *llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const;
      virtual llvm::Constant *llvmDefaultRValue( BasicBlockBuilder &basicBlockBuilder ) const;
      virtual llvm::Constant *llvmDefaultLValue( BasicBlockBuilder &basicBlockBuilder ) const;
      
      virtual void llvmCompileToModule( ModuleBuilder &moduleBuilder ) const;
      virtual void *llvmResolveExternalFunction( std::string const &functionName ) const;

      // ArrayAdapter

      virtual llvm::Value *llvmConstIndexOp( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayRValue, llvm::Value *indexRValue ) const;
      virtual llvm::Value *llvmNonConstIndexOp( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *arrayLValue, llvm::Value *indexRValue ) const;

    protected:
    
      FixedArrayAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::FixedArrayDesc> const &fixedArrayDesc );
      
      virtual llvm::Type const *buildLLVMRawType( RC::Handle<Context> const &context ) const;

    private:
    
      size_t m_length;
      RC::ConstHandle<Adapter> m_memberAdapter;
      std::string m_codeName;
   };
  };
};

#endif //_FABRIC_CG_FIXED_ARRAY_ADAPTER_H
