/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
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
      
      virtual void jsonDesc( Util::JSONObjectGenerator &resultJOG ) const;
      
    protected:
    
      OpaqueDesc( std::string const &name, RC::ConstHandle<OpaqueImpl> const &opaqueImpl );
      
    private:
    
      RC::ConstHandle<OpaqueImpl> m_opaqueImpl;
    };
  };
};

#endif //_FABRIC_RT_OPAQUE_DESC_H
