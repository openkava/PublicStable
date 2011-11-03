/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ComparableDesc.h"
#include "ComparableImpl.h"

namespace Fabric
{
  namespace RT
  {
    ComparableDesc::ComparableDesc(
      std::string const &name,
      RC::ConstHandle<ComparableImpl> const &comparableImpl
      )
      : Desc( name, comparableImpl )
      , m_comparableImpl( comparableImpl )
    {
    }

    RC::ConstHandle<RT::ComparableImpl> ComparableDesc::getImpl() const
    {
      return m_comparableImpl;
    }
  };
};
