#ifndef _FABRIC_CG_SIZE_ADAPTER_H
#define _FABRIC_CG_SIZE_ADAPTER_H

#include <Fabric/Core/CG/SimpleAdapter.h>

namespace llvm
{
  class Constant;
};

namespace Fabric
{
  namespace RT
  {
    class SizeDesc;
  };
  
  namespace CG
  {
    class SizeAdapter : public SimpleAdapter
    {
      friend class Manager;
    
    public:
      
      virtual std::string toString( void const *data ) const;
      
      llvm::Constant *llvmConst( size_t value ) const;
      
      virtual void llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const;

    protected:

      SizeAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::SizeDesc> const &sizeDesc );
      
    private:
    
      RC::ConstHandle<RT::SizeDesc> m_sizeDesc;
    };
  };
};

#endif //_FABRIC_CG_SIZE_ADAPTER_H
