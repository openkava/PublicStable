/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Core/CG/PencilSymbol.h>

namespace Fabric
{
  namespace CG
  {
    RC::Handle<PencilSymbol> PencilSymbol::Create( std::string const &friendlyName )
    {
      return new PencilSymbol( friendlyName );
    }
    
    PencilSymbol::PencilSymbol( std::string const &friendlyName )
      : m_friendlyName( friendlyName )
    {
    }
          
    bool PencilSymbol::isPencil() const
    {
      return true;
    }
      
    Function const *PencilSymbol::maybeGetPreciseFunction( ExprTypeVector const &argTypes ) const
    {
      for ( FunctionVector::const_iterator it=m_functions.begin(); it!=m_functions.end(); ++it )
      {
        if ( it->isExactMatch( argTypes ) )
        {
          return &*it;
        }
      }
        
      return 0;
    }
      
    void PencilSymbol::add( Function const &function )
    {
      ExprTypeVector argTypes;
      function.appendParamTypes( argTypes );
      if ( maybeGetPreciseFunction( argTypes ) )
        throw Exception( "function " + m_friendlyName + "(" + argTypes.desc() + ") already exists" );
      
      m_functions.push_back( function );
    }
    
    Function const *PencilSymbol::maybeGetFunction( ModuleBuilder const &moduleBuilder, Location const &location, ExprTypeVector const &argTypes ) const
    {
      Function const *function = maybeGetPreciseFunction( argTypes );
      
      if ( !function )
      {
        std::vector<Function const *> candidates;
        
        for ( FunctionVector::const_iterator it=m_functions.begin(); it!=m_functions.end(); ++it )
        {
          if ( it->isLValueToRValueMatch( argTypes ) )
            candidates.push_back( &*it );
        }
        
        if ( candidates.size() == 1 )
          function = candidates[0];
        else if ( candidates.size() > 1 )
        {
          std::string message = "ambiguous call for " + m_friendlyName + "(" + argTypes.desc() + ")";
          message += "\ncandidates are:";
          for ( std::vector<Function const *>::const_iterator it=candidates.begin(); it!=candidates.end(); ++it )
            message += "\n  " + m_friendlyName + "(" + (*it)->paramsDesc() + ")";
          throw CG::Error( location, message );
        }
      }
      
      if ( !function )
      {
        size_t lowestCost = SIZE_MAX;
        std::vector<Function const *> candidates;
        
        for ( FunctionVector::const_iterator it=m_functions.begin(); it!=m_functions.end(); ++it )
        {
          size_t thisCost;
          if ( it->isImplicitCastMatch( argTypes, moduleBuilder, thisCost ) )
          {
            if ( thisCost < lowestCost )
            {
              candidates.clear();
              lowestCost = thisCost;
            }
            if ( thisCost == lowestCost )
              candidates.push_back( &*it );
          }
        }
        
        if ( candidates.size() == 1 )
          function = candidates[0];
        else if ( candidates.size() > 1 )
        {
          std::string message = "ambiguous call for " + m_friendlyName + "(" + argTypes.desc() + ")";
          message += "\ncandidates are:";
          for ( std::vector<Function const *>::const_iterator it=candidates.begin(); it!=candidates.end(); ++it )
            message += "\n  " + m_friendlyName + "(" + (*it)->paramsDesc() + ")";
          throw CG::Error( location, message );
        }
      }
        
      return function;
    }
    
    Function const *PencilSymbol::getFunction( ModuleBuilder const &moduleBuilder, Location const &location, ExprTypeVector const &argTypes ) const
    {
      Function const *result = maybeGetFunction( moduleBuilder, location, argTypes );
      if ( !result )
        throw CG::Error( location, "no such " + m_friendlyName + "(" + argTypes.desc() + ")" );
      return result;
    }
      
    Function const *PencilSymbol::getUniqueFunction( Location const &location ) const
    {
      if ( m_functions.empty() )
        throw CG::Error( location, "no such " + m_friendlyName );
      
      if ( m_functions.size() > 1 )
        throw CG::Error( location, m_friendlyName + " is not unique" );
      
      return &m_functions[0];
    }
  }
}
