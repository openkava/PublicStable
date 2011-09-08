/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_SLICED_ARRAY_DESC_H
#define _FABRIC_RT_SLICED_ARRAY_DESC_H

#include <Fabric/Core/RT/ArrayDesc.h>

namespace Fabric
{
  namespace RT
  {
    class SlicedArrayImpl;
    
    class SlicedArrayDesc : public ArrayDesc
    {
      friend class Manager;
      
    public:
    
      RC::ConstHandle<RT::SlicedArrayImpl> getImpl() const;

      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
    protected:
    
      SlicedArrayDesc( std::string const &name, RC::ConstHandle<SlicedArrayImpl> const &slicedArrayImpl, RC::ConstHandle<Desc> const &memberDesc );
      
    private:
    
      RC::ConstHandle<SlicedArrayImpl> m_slicedArrayImpl;
   };
  };
};

#endif //_FABRIC_RT_SLICED_ARRAY_DESC_H
