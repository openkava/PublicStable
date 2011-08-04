#ifndef _FABRIC_RT_FIXED_ARRAY_IMPL_H
#define _FABRIC_RT_FIXED_ARRAY_IMPL_H

#include <Fabric/Core/RT/ArrayImpl.h>

namespace Fabric
{
  namespace RT
  {
    class FixedArrayImpl : public ArrayImpl
    {
      friend class Manager;
      friend class Impl;
      
    public:
          
      // Impl
    
      virtual void setData( void const *src, void *dst ) const;
      virtual void disposeData( void *data ) const;
      virtual void const *getDefaultData() const;
      
      virtual RC::Handle<JSON::Value> getJSONValue( void const *data ) const;
      virtual void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &value, void *data ) const;

      virtual bool isEquivalentTo( RC::ConstHandle< RT::Impl > const &desc ) const;
      virtual bool isShallow() const;

      // ArrayImpl
      
      virtual bool isFixedArrayImpl() const { return true; }

      virtual size_t getNumMembers( void const *data ) const;     
      virtual void const *getMemberData( void const *data, size_t index ) const;
      virtual void *getMemberData( void *data, size_t index ) const;
      
      // FixedArrayImpl
      
      virtual size_t getNumMembers() const;
            
    protected:
    
      FixedArrayImpl( std::string const &codeName, RC::ConstHandle<RT::Impl> const &memberImpl, size_t length );
      ~FixedArrayImpl();
            
      void const *getMemberData_NoCheck( void const *data, size_t index ) const
      { 
        uint8_t const *members = static_cast<uint8_t const *>( data );
        return &members[m_memberSize * index];
      }
      
      void *getMemberData_NoCheck( void *data, size_t index ) const
      { 
        uint8_t *members = static_cast<uint8_t *>( data );
        return &members[m_memberSize * index];
      }    
      
    private:
    
      RC::ConstHandle<Impl> m_memberImpl;
      size_t m_memberSize;
      bool m_memberIsShallow;
      size_t m_length;
      void *m_defaultData;
   };
  };
};

#endif //_FABRIC_RT_FIXED_ARRAY_IMPL_H
