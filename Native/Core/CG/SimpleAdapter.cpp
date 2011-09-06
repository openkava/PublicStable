#include "SimpleAdapter.h"
#include "Context.h"
#include "Manager.h"
#include "Context.h"
#include "BasicBlockBuilder.h"

#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/RT/Impl.h>

namespace Fabric
{
  namespace CG
  {
    SimpleAdapter::SimpleAdapter(
      RC::ConstHandle<Manager> const &manager,
      RC::ConstHandle<RT::Desc> const &desc
      )
      : Adapter( manager, desc, 0 )
    {
    }

    void SimpleAdapter::llvmRetain( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
    }

    void SimpleAdapter::llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      basicBlockBuilder->CreateStore( srcRValue, dstLValue );
    }
    
    void SimpleAdapter::llvmRelease( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
    }
    
    void *SimpleAdapter::llvmResolveExternalFunction( std::string const &functionName ) const
    {
      return 0;
    }
  };
};
