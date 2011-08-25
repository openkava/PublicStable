/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_ADAPTER_H
#define _FABRIC_CG_ADAPTER_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Base/RC/WeakConstHandle.h>
#include <Fabric/Core/RT/ImplType.h>
#include <Fabric/Core/Util/Assert.h>

#include <stdint.h>

namespace llvm
{
  class LLVMContext;
  class Module;
  class Function;
  class Type;
  class PointerType;
  class Value;
  class Constant;
  class GlobalValue;
};

namespace Fabric
{
  namespace RT
  {
    class Manager;
    class Desc;
  };
  
  namespace CG
  {
    class Manager;
    class ModuleScope;
    class ModuleBuilder;
    class BasicBlockBuilder;
    class ExprValue;
    
    class Adapter : public RC::Object
    {
    public:
    
      typedef size_t Flags;
      
      static const Flags FL_PASS_BY_REFERENCE = Flags(1) << 0;
    
      std::string const &getUserName() const;
      std::string const &getCodeName() const { FABRIC_ASSERT( m_codeName.length() ); return m_codeName; }
      std::string const &desc() const { return getUserName(); }
      
      RT::ImplType getType() const;
      
      RC::ConstHandle<Manager> getManager() const;
      llvm::LLVMContext &getLLVMContext() const;
      RC::ConstHandle<RT::Manager> getRTManager() const;    
      RC::ConstHandle<RT::Desc> getDesc() const;
      RC::ConstHandle<RT::Impl> getImpl() const;
      
      llvm::Value *llvmAlloca( BasicBlockBuilder &basicBlockBuilder, std::string const &name ) const;
      void llvmAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const;
      llvm::Value *llvmCast( BasicBlockBuilder &basicBlockBuilder, ExprValue exprValue ) const;
      
      llvm::Value *llvmCallMalloc( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *size ) const;
      llvm::Value *llvmCallRealloc( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *data, llvm::Value *newSize ) const;
      void llvmCallFree( BasicBlockBuilder &basicBlockBuilder, llvm::Value *data ) const;
  
      llvm::Type const *llvmRawType() const;
      llvm::Type const *llvmLType() const;
      llvm::Type const *llvmRType() const;
      bool usesReturnLValue() const { return m_flags & FL_PASS_BY_REFERENCE; }
      
      llvm::Value *llvmLValueToRValue( BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const;
      llvm::Value *llvmRValueToLValue( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const;
      
      virtual void llvmInit( BasicBlockBuilder &basicBlockBuilder, llvm::Value *value ) const = 0;
      virtual void llvmRetain( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const = 0;
      virtual void llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const = 0;
      virtual void llvmRelease( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const = 0;
      
      virtual llvm::Constant *llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const = 0;
      virtual llvm::Constant *llvmDefaultRValue( BasicBlockBuilder &basicBlockBuilder ) const;
      virtual llvm::Constant *llvmDefaultLValue( BasicBlockBuilder &basicBlockBuilder ) const;

      virtual void llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const = 0;
      virtual void *llvmResolveExternalFunction( std::string const &functionName ) const = 0;
      
      virtual std::string toString( void const *data ) const = 0;
      
      llvm::GlobalValue *llvmAdapterGlobalValue( llvm::Module &module ) const;
      llvm::Value *llvmAdapterPtr( BasicBlockBuilder &basicBlockBuilder ) const;
      
    protected:
    
      Adapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::Desc> const &desc, Flags flags );
      
      void setLLVMType( llvm::Type const *llvmType );
      
      llvm::Type const *llvmSizeType() const;
      
    private:
    
      RC::WeakConstHandle<Manager> m_manager;
      RC::ConstHandle<RT::Desc> m_desc;
      Flags m_flags;
      std::string m_codeName;
      llvm::Type const *m_llvmType;
      llvm::PointerType const *m_llvmTypePtr;
    };
  };
  
  inline std::string const &_( RC::ConstHandle<CG::Adapter> const &adapter )
  {
    return adapter->desc();
  }
};

#endif // _FABRIC_CG_ADAPTER_H
