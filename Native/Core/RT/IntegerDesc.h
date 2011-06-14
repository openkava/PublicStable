#ifndef _FABRIC_RT_INTEGER_DESC_H
#define _FABRIC_RT_INTEGER_DESC_H

#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace RT
  {
    class IntegerImpl;
    
    class IntegerDesc : public Desc
    {
      friend class Manager;
      
    public:
    
      int32_t getValue( void const *data ) const;
      void setValue( int32_t value, void *data ) const;
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
    protected:
    
      IntegerDesc( std::string const &name, RC::ConstHandle<IntegerImpl> const &integerImpl );
      
    private:
    
      RC::ConstHandle<IntegerImpl> m_integerImpl;
    };
  };
};

#endif //_FABRIC_RT_INTEGER_DESC_H
