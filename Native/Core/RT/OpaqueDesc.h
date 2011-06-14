#ifndef _FABRIC_RT_OPAQUE_DESC_H
#define _FABRIC_RT_OPAQUE_DESC_H

#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace RT
  {
    class OpaqueImpl;
    
    class OpaqueDesc : public Desc
    {
      friend class Manager;
    
    public:
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
    protected:
    
      OpaqueDesc( std::string const &name, RC::ConstHandle<OpaqueImpl> const &opaqueImpl );
      
    private:
    
      RC::ConstHandle<OpaqueImpl> m_opaqueImpl;
    };
  };
};

#endif //_FABRIC_RT_OPAQUE_DESC_H
