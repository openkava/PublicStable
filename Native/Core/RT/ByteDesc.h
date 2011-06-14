#ifndef _FABRIC_RT_BYTE_DESC_H
#define _FABRIC_RT_BYTE_DESC_H

#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace RT
  {
    class ByteImpl;
    
    class ByteDesc : public Desc
    {
      friend class Manager;
      
    public:
    
      uint8_t getValue( void const *data ) const;
      void setValue( uint8_t value, void *data ) const;
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
    protected:
    
      ByteDesc( std::string const &name, RC::ConstHandle<ByteImpl> const &byteImpl );
      
    private:
    
      RC::ConstHandle<ByteImpl> m_byteImpl;
    };
  };
};

#endif //_FABRIC_RT_BYTE_DESC_H
