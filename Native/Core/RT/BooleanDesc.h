#ifndef _FABRIC_RT_BOOLEAN_DESC_H
#define _FABRIC_RT_BOOLEAN_DESC_H

#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace RT
  {
    class BooleanImpl;
    
    class BooleanDesc : public Desc
    {
      friend class Manager;
      
    public:
    
      bool getValue( void const *data ) const;
      void setValue( bool value, void *data ) const;
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
    protected:
    
      BooleanDesc( std::string const &name, RC::ConstHandle<BooleanImpl> const &booleanImpl );
      
    private:
    
      RC::ConstHandle<BooleanImpl> m_booleanImpl;
    };
  };
};

#endif //_FABRIC_RT_BOOLEAN_DESC_H
