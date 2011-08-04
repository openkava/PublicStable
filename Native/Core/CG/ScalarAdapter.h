#ifndef _FABRIC_CG_SCALAR_ADAPTER_H
#define _FABRIC_CG_SCALAR_ADAPTER_H

#include <Fabric/Core/CG/SimpleAdapter.h>

namespace llvm
{
  class Constant;
};

namespace Fabric
{
  namespace RT
  {
    class NumericDesc;
  }
  namespace CG
  {
    class ScalarAdapter : public SimpleAdapter
    {
      friend class Manager;
    
    public:
      
      virtual std::string toString( void const *data ) const;
    
      virtual llvm::Constant *llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const;

      llvm::Constant *llvmConst( float value ) const;
      
      virtual void llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const;
      
    protected:
      
      ScalarAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::NumericDesc> const &scalarDesc );
      
    private:
    
      RC::ConstHandle<RT::NumericDesc> m_scalarDesc;
    };
  };
};

#endif //_FABRIC_CG_SCALAR_ADAPTER_H
