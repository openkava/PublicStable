#ifndef _FABRIC_RT_FIXED_ARRAY_DESC_H
#define _FABRIC_RT_FIXED_ARRAY_DESC_H

#include <Fabric/Core/RT/ArrayDesc.h>

namespace Fabric
{
  namespace RT
  {
    class FixedArrayImpl;
    
    class FixedArrayDesc : public ArrayDesc
    {
      friend class Manager;
      
    public:
    
      size_t getNumMembers() const;
      
      virtual RC::Handle<JSON::Object> jsonDesc() const;
            
    protected:
    
      FixedArrayDesc( std::string const &name, RC::ConstHandle<FixedArrayImpl> const &fixedArrayImpl, RC::ConstHandle<Desc> const &memberDesc );
      
    private:
    
      RC::ConstHandle<FixedArrayImpl> m_fixedArrayImpl;
   };
  };
};

#endif //_FABRIC_RT_FIXED_ARRAY_DESC_H
