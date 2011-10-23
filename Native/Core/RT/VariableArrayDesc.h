/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_VARIABLE_ARRAY_DESC_H
#define _FABRIC_RT_VARIABLE_ARRAY_DESC_H

#include <Fabric/Core/RT/ArrayDesc.h>

namespace Fabric
{
  namespace RT
  {
    class VariableArrayImpl;
    
    class VariableArrayDesc : public ArrayDesc
    {
      friend class Manager;
      
    public:
    
      RC::ConstHandle<RT::VariableArrayImpl> getImpl() const;
      
      bool isCopyOnWrite() const;

      void setNumMembers( void *data, size_t newNumMembers, void const *defaultMemberData = 0 ) const;
      void setMembers( void *data, size_t numMembers, void const *members ) const;
      
      void split( void *data ) const;
      void push( void *dst, void const *src ) const;
      void pop( void *dst, void *result ) const;
      void append( void *dst, void const *src ) const;
      
      virtual void jsonDesc( Util::JSONObjectGenerator &resultJOG ) const;
      
    protected:
    
      VariableArrayDesc( std::string const &name, RC::ConstHandle<VariableArrayImpl> const &variableArrayImpl, RC::ConstHandle<Desc> const &memberDesc );
      
    private:
    
      RC::ConstHandle<VariableArrayImpl> m_variableArrayImpl;
   };
  };
};

#endif //_FABRIC_RT_VARIABLE_ARRAY_DESC_H
