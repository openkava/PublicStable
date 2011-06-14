#include "OpaqueDesc.h"
#include <Fabric/Core/RT/OpaqueImpl.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace RT
  {
    OpaqueDesc::OpaqueDesc( std::string const &name, RC::ConstHandle<OpaqueImpl> const &opaqueImpl )
      : Desc( name, opaqueImpl )
      , m_opaqueImpl( opaqueImpl )
    {
    }
    
    RC::Handle<JSON::Object> OpaqueDesc::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = Desc::jsonDesc();
      result->set( "internalType", JSON::String::Create("opaque") );
      return result;
    }
  };
};
