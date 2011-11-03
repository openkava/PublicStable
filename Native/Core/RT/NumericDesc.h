/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_NUMERIC_DESC_H
#define _FABRIC_RT_NUMERIC_DESC_H

#include <Fabric/Core/RT/ComparableDesc.h>
#include <Fabric/Core/RT/NumericImpl.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace RT
  {
    class NumericDesc : public ComparableDesc
    {
      friend class Manager;
      
    public:
    
      bool isInteger() const
      {
        return m_numericImpl->isInteger();
      }
      bool isFloat() const
      {
        return m_numericImpl->isFloat();
      }
      
    protected:
    
      NumericDesc( std::string const &name, RC::ConstHandle<NumericImpl> const &numericImpl )
        : ComparableDesc( name, numericImpl )
        , m_numericImpl( numericImpl )
      {
      }
      
    private:
    
      RC::ConstHandle<NumericImpl> m_numericImpl;
    };
  };
};

#endif //_FABRIC_RT_NUMERIC_DESC_H
