#ifndef _FABRIC_CG_OPAQUE_ADAPTER_H
#define _FABRIC_CG_OPAQUE_ADAPTER_H

#include <Fabric/Core/CG/SimpleAdapter.h>

namespace Fabric
{
  namespace RT
  {
    class OpaqueDesc;
  };
  
  namespace CG
  {
    class OpaqueAdapter : public SimpleAdapter
    {
      friend class Manager;
      
    protected:
      
      virtual std::string toString( void const *data ) const;

      OpaqueAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::OpaqueDesc> const &opaqueDesc );
      
      virtual void llvmInit( CG::BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const;

      virtual llvm::Constant *llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const;

      virtual void llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const;
    };
  };
};

#endif //_FABRIC_CG_OPAQUE_ADAPTER_H
