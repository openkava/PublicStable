#include "SizeDesc.h"
#include "SizeImpl.h"
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace RT
  {
    SizeDesc::SizeDesc( std::string const &name, RC::ConstHandle<SizeImpl> const &sizeImpl )
      : Desc( name, sizeImpl )
      , m_sizeImpl( sizeImpl )
    {
    }

    size_t SizeDesc::getValue( void const *data ) const
    {
      return m_sizeImpl->getValue( data );
    }
    
    void SizeDesc::setValue( size_t value, void *data ) const
    {
      m_sizeImpl->setValue( value, data );
    }
    
    RC::Handle<JSON::Object> SizeDesc::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = Desc::jsonDesc();
      result->set( "internalType", JSON::String::Create("size") );
      return result;
    }
  };
};
