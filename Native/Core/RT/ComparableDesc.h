/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_COMPARABLE_DESC_H
#define _FABRIC_RT_COMPARABLE_DESC_H

#include <Fabric/Core/RT/Desc.h>

namespace Fabric
{
  namespace RT
  {
    class ComparableImpl;
    
    class ComparableDesc : public Desc
    {
      friend class Manager;
      
    public:
    
      RC::ConstHandle<RT::ComparableImpl> getImpl() const;
      
    protected:
    
      ComparableDesc(
        std::string const &name,
        RC::ConstHandle<ComparableImpl> const &comparableImpl
        );
      
    private:
    
      RC::ConstHandle<ComparableImpl> m_comparableImpl;
   };
  };
};

#endif //_FABRIC_RT_COMPARABLE_DESC_H
