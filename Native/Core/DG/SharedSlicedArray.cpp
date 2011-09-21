/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/DG/SharedSlicedArray.h>
#include <Fabric/Core/RT/SlicedArrayImpl.h>
#include <Fabric/Core/RT/VariableArrayImpl.h>

namespace Fabric
{
  namespace DG
  {
    RC::Handle<SharedSlicedArray> SharedSlicedArray::Create( RC::ConstHandle<RT::SlicedArrayImpl> const &slicedArrayImpl, void *variableArrayBits, size_t size )
    {
      return new SharedSlicedArray( slicedArrayImpl, variableArrayBits, size );
    }
    
    SharedSlicedArray::SharedSlicedArray( RC::ConstHandle<RT::SlicedArrayImpl> const &slicedArrayImpl, void *variableArrayBits, size_t size )
      : m_slicedArrayImpl( slicedArrayImpl )
    {
      m_slicedArrayData.resize( m_slicedArrayImpl->getAllocSize(), 0 );
      RC::ConstHandle<RT::VariableArrayImpl> variableArrayImpl = m_slicedArrayImpl->getVariableArrayImpl();
      m_slicedArrayImpl->set( size, variableArrayImpl->getNumMembers( &variableArrayBits ), variableArrayBits, &m_slicedArrayData[0] );
    }
    
    SharedSlicedArray::~SharedSlicedArray()
    {
      m_slicedArrayImpl->disposeData( &m_slicedArrayData[0] );
    }
    
    void SharedSlicedArray::resize( size_t newSize, void *variableArrayBits )
    {
      m_slicedArrayImpl->set(
        0,
        newSize,
        variableArrayBits,
        &m_slicedArrayData[0]
        );
    }
  };
};
