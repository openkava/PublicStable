#ifndef _FABRIC_RT_SCALAR_DESC_H
#define _FABRIC_RT_SCALAR_DESC_H

#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace RT
  {
    class ScalarImpl;
    
    class ScalarDesc : public Desc
    {
      friend class Manager;
      
    public:
    
      float getValue( void const *data ) const;
      void setValue( float value, void *data ) const;
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
    protected:
    
      ScalarDesc( std::string const &name, RC::ConstHandle<ScalarImpl> const &scalarImpl );
      
    private:
    
      RC::ConstHandle<ScalarImpl> m_scalarImpl;
    };
  };
};

#endif //_FABRIC_RT_SCALAR_DESC_H
