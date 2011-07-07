#ifndef _FABRIC_CG_STRING_ADAPTER_H
#define _FABRIC_CG_STRING_ADAPTER_H

#include <Fabric/Core/CG/Adapter.h>

#include <string.h>

namespace Fabric
{
  namespace RT
  {
    class Desc;
    class StringDesc;
  };
  
  namespace CG
  {
    class StringAdapter : public Adapter
    {
      friend class Manager;
      
    public:
    
      // Adapter
    
      virtual void llvmInit( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *value ) const;
      virtual void llvmRetain( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const;
      virtual void llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const;
      virtual void llvmRelease( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const;

      virtual llvm::Constant *llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const;
      
      virtual void llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const;
      virtual void *llvmResolveExternalFunction( std::string const &functionName ) const;
      
      virtual std::string toString( void const *data ) const;

      // StringAdapter

      void llvmReport( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *stringRValue ) const;

      llvm::Value *llvmCallLength( BasicBlockBuilder &basicBlockBuilder, llvm::Value *stringRValue ) const;
      llvm::Value *llvmCallCompare( BasicBlockBuilder &basicBlockBuilder, llvm::Value *lhsRValue, llvm::Value *rhsRValue ) const;
      llvm::Value *llvmCallCast( BasicBlockBuilder &basicBlockBuilder, RC::ConstHandle<Adapter> const &adapter, llvm::Value *rValue ) const;

    protected:
      
      StringAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::StringDesc> const &stringDesc );

    private:
      
      static void Append( void *dstLValue, void const *srcRValue );
      static void *Cast( void const *lValue, Adapter const *adapter );
      
      RC::ConstHandle<RT::StringDesc> m_stringDesc;
      llvm::Type const *m_implType;
    };
  };
};

#endif //_FABRIC_CG_STRING_ADAPTER_H
