/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
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
      virtual bool equalsData( void const *lhs, void const *rhs ) const;

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
        , m_memberIsShallow( memberImpl->isShallow() )
      {
      }

      bool isMemberShallow() const{ return m_memberIsShallow; }
      
    private:
    
      RC::ConstHandle<Impl> m_memberImpl;
      bool m_memberIsShallow;
   };
  };
};

#endif //_FABRIC_RT_ARRAY_IMPL_H
