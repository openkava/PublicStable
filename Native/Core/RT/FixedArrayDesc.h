/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_FIXED_ARRAY_DESC_H
#define _FABRIC_RT_FIXED_ARRAY_DESC_H

#include <Fabric/Core/RT/ArrayDesc.h>

namespace Fabric
{
  namespace RT
  {
    class FixedArrayImpl;
    
    class FixedArrayDesc : public ArrayDesc
    {
      friend class Manager;
      
    public:
    
      size_t getNumMembers() const;
      
      virtual void jsonDesc( Util::JSONObjectGenerator &resultJOG ) const;
            
    protected:
    
      FixedArrayDesc( std::string const &name, RC::ConstHandle<FixedArrayImpl> const &fixedArrayImpl, RC::ConstHandle<Desc> const &memberDesc );
      
    private:
    
      RC::ConstHandle<FixedArrayImpl> m_fixedArrayImpl;
   };
  };
};

#endif //_FABRIC_RT_FIXED_ARRAY_DESC_H
