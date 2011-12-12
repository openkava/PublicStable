/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_OBJECT_H
#define _FABRIC_CG_OBJECT_H

#include <Fabric/Base/RC/Object.h>

namespace Fabric
{
  namespace GC
  {
    class Container;
    
#define FABRIC_GC_OBJECT_GET_CLASS_DECL() \
    protected: \
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
    
    private:
    
      FABRIC_GC_OBJECT_GET_CLASS_DECL()
      
    public:
    
      template<class T> bool isa( Object *object )
      {
        Class const *targetClass = T::GetClass();
        Class const *objectClass = object->m_class;
        while ( objectClass )
        {
          if ( objectClass == targetClass )
            return true;
          objectClass = objectClass->parent;
        }
        return false;
      }

      template<class T> T *cast( Object *object )
      {
        T *result = 0;
        Class const *targetClass = T::GetClass();
        Class const *objectClass = object->m_class;
        while ( objectClass )
        {
          if ( objectClass == targetClass )
          {
            result = static_cast<T *>( object );
            break;
          }
          objectClass = objectClass->parent;
        }
        FABRIC_ASSERT( result );
        return result;
      }

      template<class T> T *dyn_cast( Object *object )
      {
        T *result = 0;
        Class const *targetClass = T::GetClass();
        Class const *objectClass = object->m_class;
        while ( objectClass )
        {
          if ( objectClass == targetClass )
          {
            result = static_cast<T *>( object );
            break;
          }
          objectClass = objectClass->parent;
        }
        return result;
      }

      Object( Class const *myClass, Container *container );
      ~Object();
      
      void dispose();
      
    private:
    
      Class const *m_class;
      Container *m_container;
      size_t m_containerObjectID;
    };
  }
}

#endif //_FABRIC_CG_OBJECT_H
