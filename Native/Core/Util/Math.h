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
		static const float kNAN = float( ~(uint32_t)0 );

    bool isinf( float value )
    {
#if defined (FABRIC_WIN32)
      return value == (float)10e50 || value == (float)-10e50;
#else
      return std::isinf( value );
#endif
    }

    bool isnan( float value )
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
