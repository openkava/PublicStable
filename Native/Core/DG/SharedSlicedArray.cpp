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
    RC::Handle<SharedSlicedArray> SharedSlicedArray::Create( RC::ConstHandle<RT::SlicedArrayImpl> const &slicedArrayImpl, void *variableArrayData )
    {
      return new SharedSlicedArray( slicedArrayImpl, variableArrayData );
    }
    
    SharedSlicedArray::SharedSlicedArray( RC::ConstHandle<RT::SlicedArrayImpl> const &slicedArrayImpl, void *variableArrayData )
      : m_slicedArrayImpl( slicedArrayImpl )
    {
      m_slicedArrayData.resize( m_slicedArrayImpl->getSize(), 0 );
      RC::ConstHandle<RT::VariableArrayImpl> variableArrayImpl = m_slicedArrayImpl->getVariableArrayImpl();
      m_slicedArrayImpl->set( 0, variableArrayImpl->getNumMembers( variableArrayData ), variableArrayData, &m_slicedArrayData[0] );
    }
    
    SharedSlicedArray::~SharedSlicedArray()
    {
      m_slicedArrayImpl->disposeData( &m_slicedArrayData[0] );
    }
  };
};
