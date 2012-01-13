/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_JSON_INTEGER_H
#define _FABRIC_JSON_INTEGER_H

#include <Fabric/Base/JSON/Value.h>
#include <yajl_gen.h>
#include <stdint.h>

namespace Fabric
{
  namespace JSON
  {
    class Integer : public Value
    {
    public:
    
      static RC::Handle<Integer> Create( int32_t value )
      {
        return new Integer( value );
      }
      
      virtual bool isInteger() const { return true; }
      
      int32_t value() const
      {
        return m_value;
      }
      
      virtual void generate( yajl_gen yajlHandle ) const
      {
        yajl_gen_integer( yajlHandle, (long int)m_value );
      }
    
    protected:
    
      Integer( long value )
        : m_value(value)
      {
      }
    
    private:
    
      int32_t m_value;
    };
  };
};

#endif //_FABRIC_JSON_INTEGER_H
