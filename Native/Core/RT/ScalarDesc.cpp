#include "ScalarDesc.h"
#include "ScalarImpl.h"
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace RT
  {
    ScalarDesc::ScalarDesc( std::string const &name, RC::ConstHandle<ScalarImpl> const &scalarImpl )
      : Desc( name, scalarImpl )
      , m_scalarImpl( scalarImpl )
    {
    }

    float ScalarDesc::getValue( void const *data ) const
    {
      return m_scalarImpl->getValue( data );
    }
    
    void ScalarDesc::setValue( float value, void *data ) const
    {
      m_scalarImpl->setValue( value, data );
    }
    
    RC::Handle<JSON::Object> ScalarDesc::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = Desc::jsonDesc();
      result->set( "internalType", JSON::String::Create("fp32") );
      return result;
    }
  };
};
