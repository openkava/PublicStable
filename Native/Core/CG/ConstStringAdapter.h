/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_CONST_STRING_ADAPTER_H
#define _FABRIC_CG_CONST_STRING_ADAPTER_H

#include <Fabric/Core/CG/Adapter.h>

namespace Fabric
{
  namespace RT
  {
    class ConstStringDesc;
  };
  
  namespace CG
  {
    class ConstStringAdapter : public Adapter
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
      
      virtual void llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const;
      virtual void *llvmResolveExternalFunction( std::string const &functionName ) const;
      
      virtual std::string toString( void const *data ) const;
      
      // ConstStringAdapter
      
      llvm::Value *llvmConst( BasicBlockBuilder &basicBlockBuilder, char const *data, size_t length ) const;
      llvm::Value *llvmConst( BasicBlockBuilder &basicBlockBuilder, char const *cString ) const
      {
        return llvmConst( basicBlockBuilder, cString, strlen(cString) );
      }
      llvm::Value *llvmConst( BasicBlockBuilder &basicBlockBuilder, std::string const &string ) const
      {
        return llvmConst( basicBlockBuilder, string.data(), string.length() );
      }

    protected:
    
      ConstStringAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::ConstStringDesc> const &constStringDesc );
      
      virtual llvm::Type const *buildLLVMRawType( RC::Handle<Context> const &context ) const;

    private:
    
      size_t m_length;
      std::string m_codeName;
   };
  };
};

#endif //_FABRIC_CG_CONST_STRING_ADAPTER_H
