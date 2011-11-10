#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint32_t initialSeed( size_t index )
{
  uint32_t seed = 1;
  uint32_t mult = 300773;
  size_t mask = 1;
  while ( mask != 0 )
  {
    if ( index & mask )
      seed = (seed * mult) % 1073741824;
    mult = (mult * mult) % 1073741824;
    mask <<= 1;
  }
  return seed;
}

double nextUniform01( uint32_t &seed )
{
  seed = (seed * 300773) % 1073741824;
  printf( "%u\n", (unsigned)seed );
  return double(seed) / double(1073741824.0);
}

int main( int argc, char **argv )
{
  uint32_t seed = initialSeed( 0 );
  for ( size_t i=0; i<100; ++i )
  {
    nextUniform01( seed );
  }
  return 0;
}
    
