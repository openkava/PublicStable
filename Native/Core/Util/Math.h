#ifndef _FABRIC_UTIL_MATH_H
#define _FABRIC_UTIL_MATH_H

#include <Fabric/Base/Config.h>

#if defined (FABRIC_WIN32)
# include <math.h>
#else
# include <cmath>
#endif

namespace Fabric
{
  namespace Util
  {
    template<typename T> T nanValue();
    
    template<> inline float nanValue<float>()
    {
      static uint32_t uint32Value = ~uint32_t(0);
      return *reinterpret_cast<float const *>( &uint32Value );
    }

    template<> inline double nanValue<double>()
    {
      static uint64_t uint64Value = ~uint64_t(0);
      return *reinterpret_cast<double const *>( &uint64Value );
    }

    inline bool isinf( float value )
    {
#if defined (FABRIC_WIN32)
      return value == (float)10e50 || value == (float)-10e50;
#else
      return std::isinf( value );
#endif
    }

    inline bool isinf( double value )
    {
#if defined (FABRIC_WIN32)
      return value == (double)10e500 || value == (double)-10e500;
#else
      return std::isinf( value );
#endif
    }

    inline bool isnan( float value )
    {
#if defined (FABRIC_WIN32)
      return value != value;
#else
      return std::isnan( value );
#endif
    }

    inline bool isnan( double value )
    {
#if defined (FABRIC_WIN32)
      return value != value;
#else
      return std::isnan( value );
#endif
    }
  };
};

#endif //_FABRIC_UTIL_MATH_H
