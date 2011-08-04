/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_FLOAT_DESC_H
#define _FABRIC_RT_FLOAT_DESC_H

#include <Fabric/Core/RT/NumericDesc.h>
#include <Fabric/Core/RT/FloatImpl.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace RT
  {
    class FloatDesc : public NumericDesc
    {
      friend class Manager;
      
    protected:
    
      FloatDesc( std::string const &name, RC::ConstHandle<FloatImpl> const &floatImpl )
        : NumericDesc( name, floatImpl )
        , m_floatImpl( floatImpl )
      {
      }
      
    private:
    
      RC::ConstHandle<FloatImpl> m_floatImpl;
    };

    template<typename T> class FloatDescT : public FloatDesc
    {
    public:
    
      T getValue( void const *data ) const
      {
        return m_floatImpl->getValue( data );
      }
      
      void setValue( T t, void *data ) const
      {
        m_floatImpl->setValue( t, data );
      }
          
      virtual RC::Handle<JSON::Object> jsonDesc() const
      {
        RC::Handle<JSON::Object> result = Desc::jsonDesc();
        std::string internalType = "fp" + _(m_floatImpl->getSize() * 8);
        result->set( "internalType", JSON::String::Create(internalType) );
        return result;
      }
      
    protected:
    
      FloatDescT( std::string const &name, RC::ConstHandle< FloatImplT<T> > const &floatImpl )
        : FloatDesc( name, floatImpl )
        , m_floatImpl( floatImpl )
      {
      }
      
    private:
    
      RC::ConstHandle< FloatImplT<T> > m_floatImpl;
    };
    
    class FP32Desc : public FloatDescT<float>
    {
      friend class Manager;
      
    protected:
    
      FP32Desc( std::string const &name, RC::ConstHandle<FP32Impl> fp32Impl )
        : FloatDescT<float>( name, fp32Impl )
      {
      }
    };
  };
};

#endif //_FABRIC_RT_FLOAT_DESC_H
