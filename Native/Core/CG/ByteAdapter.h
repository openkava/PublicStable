/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_BYTE_ADAPTER_H
#define _FABRIC_CG_BYTE_ADAPTER_H

#include <Fabric/Core/CG/SimpleAdapter.h>

namespace Fabric
{
  namespace RT
  {
    class NumericDesc;
  };
  
  namespace CG
  {
    class ByteAdapter : public SimpleAdapter
    {
      friend class Manager;
      
    public:
      
      virtual std::string toString( void const *data ) const;
      
      llvm::Constant *llvmConst( RC::Handle<Context> const &context, uint8_t value ) const;
      
    protected:
    
      ByteAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::NumericDesc> const &byteDesc );
      
      virtual llvm::Type const *buildLLVMRawType( RC::Handle<Context> const &context ) const;
      
      virtual void llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const;
      
    private:
    
      RC::ConstHandle<RT::NumericDesc> m_byteDesc;
    };
  };
};

#endif //_FABRIC_CG_BYTE_ADAPTER_H
