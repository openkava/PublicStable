#ifndef _FABRIC_RT_IMPL_TYPE_H
#define _FABRIC_RT_IMPL_TYPE_H

#include <Fabric/Base/RC/ConstHandle.h>

#include <stddef.h>

namespace Fabric
{
  namespace RT
  {
    class Impl;
    class Desc;
    
    typedef size_t ImplType;
    
    // Simple types
    // - Simple types should be ordered in terms of casting priority. 
    //   I.e. lower data bandwidth first.
    static const ImplType DT_BOOLEAN = size_t(1) << 0;
    static const ImplType DT_BYTE = size_t(1) << 1;
    static const ImplType DT_INTEGER = size_t(1) << 2;
    static const ImplType DT_SIZE = size_t(1) << 3;
    static const ImplType DT_SCALAR = size_t(1) << 4;
    
    // Non-simple, non-compound types
    static const ImplType DT_STRING = size_t(1) << 8;
    static const ImplType DT_OPAQUE = size_t(1) << 9;
    static const ImplType DT_CONST_STRING = size_t(1) << 10;
    
    // Struct type
    static const ImplType DT_STRUCT = size_t(1) << 16;
    
    // Array types
    static const ImplType DT_FIXED_ARRAY = size_t(1) << 24;
    static const ImplType DT_VARIABLE_ARRAY = size_t(1) << 25;

    inline bool isBoolean( ImplType implType ) { return implType & DT_BOOLEAN; }
    inline bool isByte( ImplType implType ) { return implType & DT_BYTE; }
    inline bool isInteger( ImplType implType ) { return implType & DT_INTEGER; }
    inline bool isSize( ImplType implType ) { return implType & DT_SIZE; }
    inline bool isScalar( ImplType implType ) { return implType & DT_SCALAR; }
    inline bool isSimple( ImplType implType ) { return isBoolean(implType) || isByte(implType) || isInteger(implType) || isSize(implType) || isScalar(implType); }
    inline bool isString( ImplType implType ) { return implType & DT_STRING; }
    inline bool isOpaque( ImplType implType ) { return implType & DT_OPAQUE; }
    inline bool isConstString( ImplType implType ) { return implType & DT_CONST_STRING; }
    inline bool isStruct( ImplType implType ) { return implType & DT_STRUCT; }
    inline bool isFixedArray( ImplType implType ) { return implType & DT_FIXED_ARRAY; }
    inline bool isVariableArray( ImplType implType ) { return implType & DT_VARIABLE_ARRAY; }
    inline bool isArray( ImplType implType ) { return isFixedArray(implType) || isVariableArray(implType); }
  };
};

#endif //_FABRIC_RT_IMPL_TYPE_H
