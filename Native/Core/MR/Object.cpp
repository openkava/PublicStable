/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/Object.h>

namespace Fabric
{
  namespace MR
  {
    Object::Class const *Object::GetClass()
    {
      static Class myClass = { 0 };
      return &myClass;
    }

    Object::Object( Class const *class_ )
      : m_class( class_ )
    {
    }

    Object::~Object()
    {
    }
  }
}
