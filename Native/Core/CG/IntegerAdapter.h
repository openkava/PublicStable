#ifndef _FABRIC_CG_INTEGER_ADAPTER_H
#define _FABRIC_CG_INTEGER_ADAPTER_H

#include <Fabric/Core/CG/SimpleAdapter.h>

namespace llvm
{
  class Constant;
};

namespace Fabric
{
  namespace RT
  {
    class IntegerDesc;
  };
  
  namespace CG
  {
    class IntegerAdapter : public SimpleAdapter
    {
      friend class Manager;
      
    public:
      
      virtual std::string toString( void const *data ) const;
    
      llvm::Constant *llvmConst( int32_t value ) const;
          
      virtual void llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const;

    protected:

      IntegerAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::IntegerDesc> const &integerDesc );
      
    private:
    
      RC::ConstHandle<RT::IntegerDesc> m_integerDesc;
    };
  };
};

#endif //_FABRIC_CG_INTEGER_ADAPTER_H
