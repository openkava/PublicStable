#include "FloatDesc.h"
#include "FloatImpl.h"
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace RT
  {
    FloatDesc::FloatDesc( std::string const &name, RC::ConstHandle<FloatImpl> const &scalarImpl )
      : Desc( name, scalarImpl )
      , m_scalarImpl( scalarImpl )
    {
    }

    float FloatDesc::getValue( void const *data ) const
    {
      return m_scalarImpl->getValue( data );
    }
    
    void FloatDesc::setValue( float value, void *data ) const
    {
      m_scalarImpl->setValue( value, data );
    }
    
    RC::Handle<JSON::Object> FloatDesc::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = Desc::jsonDesc();
      result->set( "internalType", JSON::String::Create("fp32") );
      return result;
    }
  };
};
