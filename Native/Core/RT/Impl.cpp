#include "Impl.h"
#include "FixedArrayImpl.h"
#include "VariableArrayImpl.h"

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

    RC::ConstHandle<VariableArrayImpl> Impl::getVariableArrayImpl() const
    {
      RC::ConstHandle<VariableArrayImpl> variableArrayImpl = m_variableArrayImpl.makeStrong();
      if ( !variableArrayImpl )
      {
        variableArrayImpl = new VariableArrayImpl( m_codeName + "_VA", this );
        m_variableArrayImpl = variableArrayImpl;
      }
      return variableArrayImpl;
    }
  };
};
