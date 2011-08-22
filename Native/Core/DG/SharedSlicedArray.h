/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_DG_SHARED_SLICED_ARRAY_H
#define _FABRIC_DG_SHARED_SLICED_ARRAY_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <stdint.h>
#include <vector>

namespace Fabric
{
  namespace RT
  {
    class SlicedArrayImpl;
  };
  
  namespace DG
  {
    class SharedSlicedArray : public RC::Object
    {
    public:
    
      static RC::Handle<SharedSlicedArray> Create( RC::ConstHandle<RT::SlicedArrayImpl> const &slicedArrayImpl, void *variableArrayData );
      
      void *getData()
      {
        return &m_slicedArrayData[0];
      }

    protected:
      
      SharedSlicedArray( RC::ConstHandle<RT::SlicedArrayImpl> const &slicedArrayImpl, void *variableArrayData );
      ~SharedSlicedArray();

    private:
    
      RC::ConstHandle<RT::SlicedArrayImpl> m_slicedArrayImpl;
      std::vector<uint8_t> m_slicedArrayData;
    };
  };
};

#endif //_FABRIC_DG_SHARED_SLICED_ARRAY_H
