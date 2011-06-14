#include "IntegerDesc.h"
#include "IntegerImpl.h"
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace RT
  {
    IntegerDesc::IntegerDesc( std::string const &name, RC::ConstHandle<IntegerImpl> const &integerImpl )
      : Desc( name, integerImpl )
      , m_integerImpl( integerImpl )
    {
    }

    int32_t IntegerDesc::getValue( void const *data ) const
    {
      return m_integerImpl->getValue( data );
    }
    
    void IntegerDesc::setValue( int32_t value, void *data ) const
    {
      m_integerImpl->setValue( value, data );
    }
    
    RC::Handle<JSON::Object> IntegerDesc::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = Desc::jsonDesc();
      result->set( "internalType", JSON::String::Create("si32") );
      return result;
    }
  };
};
