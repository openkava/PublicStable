#ifndef _FABRIC_RT_SIZE_DESC_H
#define _FABRIC_RT_SIZE_DESC_H

#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace RT
  {
    class SizeImpl;
    
    class SizeDesc : public Desc
    {
      friend class Manager;
      
    public:
    
      size_t getValue( void const *data ) const;
      void setValue( size_t value, void *data ) const;
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
    protected:
    
      SizeDesc( std::string const &name, RC::ConstHandle<SizeImpl> const &sizeImpl );
      
    private:
    
      RC::ConstHandle<SizeImpl> m_sizeImpl;
    };
  };
};

#endif //_FABRIC_RT_SIZE_DESC_H
