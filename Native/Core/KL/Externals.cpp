/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/KL/Externals.h>

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
        symbolNameToAddressMap["fp64_acos"] = (void *)&acos;
        symbolNameToAddressMap["fp32_asin"] = (void *)&asinf;
        symbolNameToAddressMap["fp64_asin"] = (void *)&asin;
        symbolNameToAddressMap["fp32_atan"] = (void *)&atanf;
        symbolNameToAddressMap["fp64_atan"] = (void *)&atan;
        symbolNameToAddressMap["fp32_atan2"] = (void *)&atan2f;
        symbolNameToAddressMap["fp64_atan2"] = (void *)&atan2;
        symbolNameToAddressMap["fp32_sin"] = (void *)&sinf;
        symbolNameToAddressMap["fp64_sin"] = (void *)&sin;
        symbolNameToAddressMap["fp32_cos"] = (void *)&cosf;
        symbolNameToAddressMap["fp64_cos"] = (void *)&cos;
        symbolNameToAddressMap["fp32_tan"] = (void *)&tanf;
        symbolNameToAddressMap["fp64_tan"] = (void *)&tan;
        symbolNameToAddressMap["fp32_pow"] = (void *)&powf;
        symbolNameToAddressMap["fp64_pow"] = (void *)&pow;
#if defined(FABRIC_OS_WINDOWS)
        symbolNameToAddressMap["fp32_round"] = (void *)&imp_roundf;
        symbolNameToAddressMap["fp64_round"] = (void *)&imp_round;
#else
        symbolNameToAddressMap["fp32_round"] = (void *)&roundf;
        symbolNameToAddressMap["fp64_round"] = (void *)&round;
#endif
        symbolNameToAddressMap["fp64_ceil"] =( void *)&ceil;
        symbolNameToAddressMap["fp64_floor"] =( void *)&floor;
        symbolNameToAddressMap["fp32_fabs"] =( void *)&fabsf;
        symbolNameToAddressMap["fp64_fabs"] =( void *)&fabs;
        symbolNameToAddressMap["sinf"] = (void *)&sinf;
        symbolNameToAddressMap["sin"] = (void *)&sin;
        symbolNameToAddressMap["cosf"] = (void *)&cosf;
        symbolNameToAddressMap["cos"] = (void *)&cos;
        symbolNameToAddressMap["powf"] = (void *)&powf;
        symbolNameToAddressMap["pow"] = (void *)&pow;
        symbolNameToAddressMap["fmod"] = (void *)&fmod;
        symbolNameToAddressMap["fmodf"] = (void *)&fmodf;
        symbolNameToAddressMap["fp32_log"] = (void *)&logf;
        symbolNameToAddressMap["fp64_log"] = (void *)&log;
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
