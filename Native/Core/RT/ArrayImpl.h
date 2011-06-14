#ifndef _FABRIC_RT_ARRAY_IMPL_H
#define _FABRIC_RT_ARRAY_IMPL_H

#include <Fabric/Core/RT/Impl.h>

namespace Fabric
{
  namespace RT
  {
    class ArrayImpl : public Impl
    {
      friend class Manager;
      
    public:
    
      // Impl
    
      virtual std::string descData( void const *data ) const;

      // ArrayImpl

      RC::ConstHandle<Impl> getMemberImpl() const
      {
        return m_memberImpl;
      }
      
      virtual size_t getNumMembers( void const *data ) const = 0;     
      virtual void const *getMemberData( void const *data, size_t index ) const = 0;
      virtual void *getMemberData( void *data, size_t index ) const = 0;
            
    protected:
    
      ArrayImpl( std::string const &codeName, ImplType implType, RC::ConstHandle<Impl> const &memberImpl )
        : Impl( codeName, implType )
        , m_memberImpl( memberImpl )
      {
      }
      
    private:
    
      RC::ConstHandle<Impl> m_memberImpl;
   };
  };
};

#endif //_FABRIC_RT_ARRAY_IMPL_H
