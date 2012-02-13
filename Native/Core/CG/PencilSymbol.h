/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_CG_PENCIL_SYMBOL_H
#define _FABRIC_CG_PENCIL_SYMBOL_H

#include <Fabric/Core/CG/Symbol.h>
#include <Fabric/Core/CG/Function.h>

#include <vector>

namespace Fabric
{
  namespace CG
  {
    class PencilSymbol : public Symbol
    {
    public:
      
      static RC::Handle<PencilSymbol> Create( std::string const &friendlyName );
      
      virtual bool isPencil() const;
      
      Function const *findPreciseFunction( ExprTypeVector const &argTypes ) const;
      
      void add( Function const &function );
      
      Function const *maybeGetPreciseFunction( ExprTypeVector const &argTypes ) const;
      
      Function const *maybeGetPreciseFunction() const
      {
        ExprTypeVector argTypes;
        return maybeGetPreciseFunction( argTypes );
      }
      
      Function const *maybeGetPreciseFunction( ExprType const &argType ) const
      {
        ExprTypeVector argTypes;
        argTypes.push_back( argType );
        return maybeGetPreciseFunction( argTypes );
      }
      
      Function const *maybeGetPreciseFunction( ExprType const &argType1, ExprType const &argType2 ) const
      {
        ExprTypeVector argTypes;
        argTypes.push_back( argType1 );
        argTypes.push_back( argType2 );
        return maybeGetPreciseFunction( argTypes );
      }
      
      Function const &getFunction( CG::Location const &location, ExprTypeVector const &argTypes ) const;
      
      Function const &getFunction( CG::Location const &location ) const
      {
        ExprTypeVector argTypes;
        return getFunction( location, argTypes );
      }
      
      Function const &getFunction( CG::Location const &location, ExprType const &argType ) const
      {
        ExprTypeVector argTypes;
        argTypes.push_back( argType );
        return getFunction( location, argTypes );
      }
      
      Function const &getFunction( CG::Location const &location, ExprType const &argType1, ExprType const &argType2 ) const
      {
        ExprTypeVector argTypes;
        argTypes.push_back( argType1 );
        argTypes.push_back( argType2 );
        return getFunction( location, argTypes );
      }
      
      Function const &getUniqueFunction(
        CG::Location const &location
        ) const;
      
    protected:
      
      PencilSymbol( std::string const &friendlyName );
            
    private:
    
      std::string m_friendlyName;
      FunctionVector m_functions;
    };
  }
}

#endif //_FABRIC_CG_PENCIL_SYMBOL_H
