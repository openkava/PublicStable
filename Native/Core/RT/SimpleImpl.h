#ifndef _FABRIC_RT_SIMPLE_IMPL_H
#define _FABRIC_RT_SIMPLE_IMPL_H

#include <Fabric/Core/RT/Impl.h>

namespace Fabric
{
  namespace RT
  {
    class SimpleImpl : public Impl
    {
    public:
    
      // Impl
    
      virtual void disposeData( void *data ) const;
      
      virtual bool isEquivalentTo( RC::ConstHandle<Impl> const &impl ) const;
      virtual bool isShallow() const;
    
    protected:
    
      SimpleImpl( std::string const &codeName, ImplType implType, size_t size )
        : Impl( codeName, implType )
      {
        setSize( size );
      }
    };
  };
};

#endif //_FABRIC_RT_SIMPLE_IMPL_H
