/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_ARRAY_DESC_H
#define _FABRIC_RT_ARRAY_DESC_H

#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace RT
  {
    class ArrayImpl;
    
    class ArrayDesc : public Desc
    {
      friend class Manager;
      
    public:
    
      virtual bool isArrayDesc() const { return true; }
      virtual bool isFixedArrayDesc() const { return false; }
      virtual bool isVariableArrayDesc() const { return false; }
      
      RC::ConstHandle<ArrayImpl> getImpl() const;
      RC::ConstHandle<Desc> getMemberDesc() const;

      size_t getNumMembers( void const *data ) const;     
      void const *getMemberData( void const *data, size_t index ) const;
      void *getMemberData( void *data, size_t index ) const;
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
            
    protected:
    
      ArrayDesc( std::string const &name, RC::ConstHandle<ArrayImpl> const &arrayImpl, RC::ConstHandle<Desc> const &memberDesc );
      
    private:
    
      RC::ConstHandle<ArrayImpl> m_arrayImpl;
      RC::ConstHandle<Desc> m_memberDesc;
   };
  };
};

#endif //_FABRIC_RT_ARRAY_DESC_H
