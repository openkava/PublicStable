/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_OBJECT_H
#define _FABRIC_CG_OBJECT_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>

namespace Fabric
{
  namespace GC
  {
    class Container;
    
#define FABRIC_GC_OBJECT_GET_CLASS_DECL() \
    public: \
      static Class const *GetClass();
#define FABRIC_GC_OBJECT_GET_CLASS_IMPL(class_,parentClass) \
      GC::Object::Class const *class_::GetClass() \
      { \
        static Class myClass = { parentClass::GetClass() }; \
        return &myClass; \
      }

    class Object : public RC::Object
    {
    protected:
    
      struct Class
      {
        Class const *parent;
      };
    
      FABRIC_GC_OBJECT_GET_CLASS_DECL()
      
    public:

      Object( Class const *myClass, Container *container, std::string const &id_ );
      ~Object();
      
      Class const *getClass() const
      {
        return m_class;
      }
      
      void dispose();
      
    private:
    
      Class const *m_class;
      Container *m_container;
      std::string m_containerObjectID;
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
