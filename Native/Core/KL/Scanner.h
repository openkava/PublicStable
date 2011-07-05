/*
 *
 *  Created by Peter Zion on 10-12-01.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_KL_SCANNER_H
#define _FABRIC_KL_SCANNER_H

#include <Fabric/Core/KL/Token.h>
#include <Fabric/Core/KL/Parser.hpp>
#include <string>

namespace Fabric
{
  namespace KL
  {
    class Source;
    class Location;
    
    class Scanner
    {
    public:
    
      Scanner( Source &source, RC::ConstHandle<CG::Manager> const &cgManager );
      
      Token nextToken( YYSTYPE *yys );
      
      Source &source()
      {
        return m_source;
      }
      
      RC::ConstHandle<CG::Manager> cgManager() const;
    
    protected:
    
      Token nextToken_Alpha( YYSTYPE *yys );
      Token nextToken_Digit( YYSTYPE *yys );
      Token nextToken_Space( YYSTYPE *yys );
      Token nextToken_Symbol( YYSTYPE *yys );
    
      static bool IsAlpha( int ch )
      {
        return ch != -1 && s_charAttribs[uint8_t(ch)] & k_charAttribAlpha;
      }
      
      static bool IsDigit( int ch )
      {
        return ch != -1 && s_charAttribs[uint8_t(ch)] & k_charAttribDigit;
      }
      
      static bool IsHexDigit( int ch )
      {
        return ch != -1 && s_charAttribs[uint8_t(ch)] & k_charAttribHexDigit;
      }
      
      static bool IsSpace( int ch )
      {
        return ch != -1 && s_charAttribs[uint8_t(ch)] & k_charAttribSpace;
      }
      
      static bool IsSymbol( int ch )
      {
        return ch != -1 && s_charAttribs[uint8_t(ch)] & k_charAttribSymbol;
      }
      
    private:
    
      Source &m_source;
      RC::ConstHandle<CG::Manager> m_cgManager;

      static bool s_charAttribsInitialized;
      static uint8_t s_charAttribs[256];
      static const uint8_t k_charAttribAlpha = 1;
      static const uint8_t k_charAttribDigit = 2;
      static const uint8_t k_charAttribSpace = 4;
      static const uint8_t k_charAttribSymbol = 8;
      static const uint8_t k_charAttribHexDigit = 16;
    };
  };
};

#endif //_FABRIC_KL_BUFFER_H
