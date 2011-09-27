/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/RT/Impl.h>
#include <Fabric/Core/RT/FixedArrayImpl.h>
#include <Fabric/Core/RT/VariableArrayImpl.h>
#include <Fabric/Core/RT/SlicedArrayImpl.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Base/Util/Bits.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/Core/Util/Format.h>

namespace Fabric
{
  namespace RT
  {
    Impl::Impl( std::string const &codeName, ImplType implType )
      : m_codeName( codeName )
      , m_implType( implType )
      , m_size( 0 )
      , m_disposeCallback( 0 )
    {
      FABRIC_ASSERT( Util::countBits( implType ) == 1 );
    }
    
    void Impl::setSize( size_t size )
    {
      m_size = size;
    }

    RC::ConstHandle<FixedArrayImpl> Impl::getFixedArrayImpl( size_t length ) const
    {
      RC::WeakConstHandle<FixedArrayImpl> &fixedArrayImplWeakHandle = m_fixedArrayImpls[length];
      RC::ConstHandle<FixedArrayImpl> fixedArrayImpl = fixedArrayImplWeakHandle.makeStrong();
      if ( !fixedArrayImpl )
      {
        fixedArrayImpl = new FixedArrayImpl( m_codeName + "_FA" + _(length), this, length );
        fixedArrayImplWeakHandle = fixedArrayImpl;
      }
      return fixedArrayImpl;
    }

    RC::ConstHandle<VariableArrayImpl> Impl::getVariableArrayImpl( size_t flags ) const
    {
      RC::WeakConstHandle<VariableArrayImpl> &variableArrayImplWeakHandle = m_variableArrayImpls[flags];
      RC::ConstHandle<VariableArrayImpl> variableArrayImpl = variableArrayImplWeakHandle.makeStrong();
      if ( !variableArrayImpl )
      {
        std::string name = m_codeName + "_VA_";
        name += (flags & VariableArrayImpl::FLAG_COPY_ON_WRITE)? "CopyOnWrite": "Shared";
        variableArrayImpl = new VariableArrayImpl( name, flags, this );
        variableArrayImplWeakHandle = variableArrayImpl;
      }
      return variableArrayImpl;
    }

    RC::ConstHandle<SlicedArrayImpl> Impl::getSlicedArrayImpl() const
    {
      RC::ConstHandle<SlicedArrayImpl> slicedArrayImpl = m_slicedArrayImpl.makeStrong();
      if ( !slicedArrayImpl )
      {
        slicedArrayImpl = new SlicedArrayImpl( m_codeName + "_SA", this );
        m_slicedArrayImpl = slicedArrayImpl;
      }
      return slicedArrayImpl;
    }
    
    void Impl::disposeData( void *lValue ) const
    {
      if ( m_disposeCallback )
        m_disposeCallback( lValue );
      disposeDataImpl( lValue );
    }
  };
};
