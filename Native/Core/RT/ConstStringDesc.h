/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_CONST_STRING_DESC_H
#define _FABRIC_RT_CONST_STRING_DESC_H

#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace RT
  {
    class ConstStringImpl;
    
    class ConstStringDesc : public Desc
    {
      friend class Manager;
      
    public:
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
            
      std::string toString( void const *data ) const;
      
    protected:
    
      ConstStringDesc( std::string const &name, RC::ConstHandle<ConstStringImpl> const &constStringImpl );
      
    private:
    
      RC::ConstHandle<ConstStringImpl> m_constStringImpl;
   };
  };
};

#endif //_FABRIC_RT_CONST_STRING_DESC_H
