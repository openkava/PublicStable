#ifndef _FABRIC_RT_FLOAT_DESC_H
#define _FABRIC_RT_FLOAT_DESC_H

#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace RT
  {
    class FloatImpl;
    
    class FloatDesc : public Desc
    {
      friend class Manager;
      
    public:
    
      float getValue( void const *data ) const;
      void setValue( float value, void *data ) const;
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
    protected:
    
      FloatDesc( std::string const &name, RC::ConstHandle<FloatImpl> const &scalarImpl );
      
    private:
    
      RC::ConstHandle<FloatImpl> m_scalarImpl;
    };
  };
};

#endif //_FABRIC_RT_FLOAT_DESC_H
