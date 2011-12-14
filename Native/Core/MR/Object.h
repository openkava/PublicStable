/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_OBJECT_H
#define _FABRIC_MR_OBJECT_H

#include <Fabric/Core/GC/Object.h>

namespace Fabric
{
  namespace MR
  {
#define FABRIC_MR_OBJECT_GET_CLASS_DECL() \
    public: \
      static Class const *GetClass();
#define FABRIC_MR_OBJECT_GET_CLASS_IMPL(class_,parentClass) \
      MR::Object::Class const *class_::GetClass() \
      { \
        static Class myClass = { parentClass::GetClass() }; \
        return &myClass; \
      }

    class Object : public GC::Object
    {
    protected:
    
      struct Class
      {
        Class const *parent;
      };
    
      FABRIC_MR_OBJECT_GET_CLASS_DECL()
      
    public:

      Object( Class const *myClass );
      ~Object();
      
      Class const *getClass() const
      {
        return m_class;
      }
      
    private:
    
      Class const *m_class;
    };

    template<class T> RC::Handle<T> DynCast( RC::Handle<Object> const &object )
    {
      RC::Handle<T> result = 0;
      if ( object )
      {
        Object::Class const *targetClass = T::GetClass();
        Object::Class const *objectClass = object->getClass();
        while ( objectClass )
        {
          if ( objectClass == targetClass )
          {
            result = RC::Handle<T>::StaticCast( object );
            break;
          }
          objectClass = objectClass->parent;
        }
      }
      return result;
    }
  
    template<class T> bool IsA( RC::Handle<Object> const &object )
    {
      return !!DynCast<T>( object );
    }

    template<class T> RC::Handle<T> Cast( RC::Handle<Object> const &object )
    {
      RC::Handle<T> result = DynCast<T>( object );
      FABRIC_ASSERT( result );
      return result;
    }
  }
}

#endif //_FABRIC_CG_OBJECT_H
