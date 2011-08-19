/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/KL/Externals.h>
#include <Fabric/Base/Config.h>

#include <string>
#include <map>
#include <math.h>
#include <stdlib.h>

#if defined(FABRIC_OS_WINDOWS)
extern "C" void _chkstk();
    
static float imp_roundf( float x )
{
  return floorf( x + 0.5f );
}
    
static double imp_round( double x )
{
  return floor( x + 0.5 );
}
#endif

static double fp64_fabs( double x )
{
  return fabs( x );
}

static double fp64_fmod( double x, double y )
{
  return fmod( x, y );
}

static double fp64_sin( double x )
{
  return sin( x );
}

static double fp64_cos( double x )
{
  return cos( x );
}

static double fp64_tan( double x )
{
  return tan( x );
}

static double fp64_log( double x )
{
  return log( x );
}

static double fp64_sqrt( double x )
{
  return sqrt( x );
}

static double fp64_ceil( double x )
{
  return ceil( x );
}

static double fp64_floor( double x )
{
  return floor( x );
}

static double fp64_asin( double x )
{
  return asin( x );
}

static double fp64_acos( double x )
{
  return acos( x );
}

static double fp64_atan( double x )
{
  return atan( x );
}

static double fp64_pow( double x, double y )
{
  return pow( x, y );
}

static double fp64_atan2( double x, double y )
{
  return atan2( x, y );
}

namespace Fabric
{
  namespace KL
  {
    typedef std::map<std::string, void *> SymbolNameToAddressMap;

    void *LookupExternalSymbol( std::string const &functionName )
    {
      SymbolNameToAddressMap symbolNameToAddressMap;
      if ( symbolNameToAddressMap.empty() )
      {
        symbolNameToAddressMap["malloc"] = (void *)&malloc;
        symbolNameToAddressMap["realloc"] = (void *)&realloc;
        symbolNameToAddressMap["free"] = (void *)&free;
        symbolNameToAddressMap["fp32_acos"] = (void *)&acosf;
        symbolNameToAddressMap["fp64_acos"] = (void *)&fp64_acos;
        symbolNameToAddressMap["fp32_asin"] = (void *)&asinf;
        symbolNameToAddressMap["fp64_asin"] = (void *)&fp64_asin;
        symbolNameToAddressMap["fp32_atan"] = (void *)&atanf;
        symbolNameToAddressMap["fp64_atan"] = (void *)&fp64_atan;
        symbolNameToAddressMap["fp32_atan2"] = (void *)&atan2f;
        symbolNameToAddressMap["fp64_atan2"] = (void *)&fp64_atan2;
        symbolNameToAddressMap["fp32_sin"] = (void *)&sinf;
        symbolNameToAddressMap["fp64_sin"] = (void *)&fp64_sin;
        symbolNameToAddressMap["fp32_cos"] = (void *)&cosf;
        symbolNameToAddressMap["fp64_cos"] = (void *)&fp64_cos;
        symbolNameToAddressMap["fp32_tan"] = (void *)&tanf;
        symbolNameToAddressMap["fp64_tan"] = (void *)&fp64_tan;
        symbolNameToAddressMap["fp32_pow"] = (void *)&powf;
        symbolNameToAddressMap["fp64_pow"] = (void *)&fp64_pow;
#if defined(FABRIC_OS_WINDOWS)
        symbolNameToAddressMap["fp32_round"] = (void *)&imp_roundf;
        symbolNameToAddressMap["fp64_round"] = (void *)&imp_round;
#else
        symbolNameToAddressMap["fp32_round"] = (void *)&roundf;
        symbolNameToAddressMap["fp64_round"] = (void *)&round;
#endif
        symbolNameToAddressMap["fp32_ceil"] =( void *)&ceilf;
        symbolNameToAddressMap["fp64_ceil"] =( void *)&fp64_ceil;
        symbolNameToAddressMap["fp32_floor"] =( void *)&floorf;
        symbolNameToAddressMap["fp64_floor"] =( void *)&fp64_floor;
        symbolNameToAddressMap["fp32_fabs"] =( void *)&fabsf;
        symbolNameToAddressMap["fp64_fabs"] =( void *)&fp64_fabs;
        symbolNameToAddressMap["fp32_sqrt"] =( void *)&sqrtf;
        symbolNameToAddressMap["fp64_sqrt"] =( void *)&fp64_sqrt;
        symbolNameToAddressMap["sinf"] = (void *)&sinf;
        symbolNameToAddressMap["sin"] = (void *)&fp64_sin;
        symbolNameToAddressMap["cosf"] = (void *)&cosf;
        symbolNameToAddressMap["cos"] = (void *)&fp64_cos;
        symbolNameToAddressMap["powf"] = (void *)&powf;
        symbolNameToAddressMap["pow"] = (void *)&fp64_pow;
        symbolNameToAddressMap["fmodf"] = (void *)&fmodf;
        symbolNameToAddressMap["fmod"] = (void *)&fp64_fmod;
        symbolNameToAddressMap["fp32_log"] = (void *)&logf;
        symbolNameToAddressMap["fp64_log"] = (void *)&fp64_log;
#if defined(FABRIC_OS_WINDOWS)
        symbolNameToAddressMap["_chkstk"] = (void *)&_chkstk;
#endif
      }
      SymbolNameToAddressMap::const_iterator it = symbolNameToAddressMap.find( functionName );
      if ( it != symbolNameToAddressMap.end() )
        return it->second;
      else return 0;
    }
  };
};
