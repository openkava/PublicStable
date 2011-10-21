/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_BOOLEAN_DESC_H
#define _FABRIC_RT_BOOLEAN_DESC_H

#include <Fabric/Core/RT/ComparableDesc.h>

namespace Fabric
{
  namespace RT
  {
    class BooleanImpl;
    
    class BooleanDesc : public ComparableDesc
    {
      friend class Manager;
      
    public:
    
      bool getValue( void const *data ) const;
      void setValue( bool value, void *data ) const;
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      
    protected:
    
      BooleanDesc( std::string const &name, RC::ConstHandle<BooleanImpl> const &booleanImpl );
      
    private:
    
      RC::ConstHandle<BooleanImpl> m_booleanImpl;
    };
  };
};

#endif //_FABRIC_RT_BOOLEAN_DESC_H
