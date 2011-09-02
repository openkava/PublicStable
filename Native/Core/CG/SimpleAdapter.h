#ifndef _FABRIC_CG_SIMPLE_ADAPTER_H
#define _FABRIC_CG_SIMPLE_ADAPTER_H

#include <Fabric/Core/CG/Adapter.h>

namespace Fabric
{
  namespace CG
  {
    class SimpleAdapter : public Adapter
    {
    public:

      virtual void llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const;

    protected:
    
      SimpleAdapter(
        RC::ConstHandle<Manager> const &manager,
        RC::ConstHandle<RT::Desc> const &desc
        );

      virtual void llvmRetain( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const;
      virtual void llvmRelease( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const;

      virtual void *llvmResolveExternalFunction( std::string const &functionName ) const;
    };
  };
};

#endif //_FABRIC_CG_SIMPLE_ADAPTER_H
