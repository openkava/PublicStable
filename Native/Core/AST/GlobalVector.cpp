/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/GlobalVector.h>
#include <Fabric/Core/AST/Global.h>
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    RC::ConstHandle<GlobalVector> GlobalVector::Create( RC::ConstHandle<Global> const &first, RC::ConstHandle<GlobalVector> const &remaining )
    {
      GlobalVector *result = new GlobalVector;
      if ( first )
        result->push_back( first );
      if ( remaining )
      {
        for ( GlobalVector::const_iterator it=remaining->begin(); it!=remaining->end(); ++it )
          result->push_back( *it );
      }
      return result;
    }
    
    RC::ConstHandle<GlobalVector> GlobalVector::Create( RC::ConstHandle<GlobalVector> const &lhs, RC::ConstHandle<GlobalVector> const &rhs )
    {
      GlobalVector *result = new GlobalVector;
      if ( lhs )
      {
        for ( GlobalVector::const_iterator it=lhs->begin(); it!=lhs->end(); ++it )
          result->push_back( *it );
      }
      if ( rhs )
      {
        for ( GlobalVector::const_iterator it=rhs->begin(); it!=rhs->end(); ++it )
          result->push_back( *it );
      }
      return result;
    }
    
    GlobalVector::GlobalVector()
    {
    }
    
    Util::SimpleString GlobalVector::toJSON() const
    {
      Util::SimpleString ss;
      ss.append( '[' );
      for ( const_iterator it=begin(); it!=end(); ++it )
      {
        if ( it != begin() )
          ss.append( ',' );
        (*it)->appendJSON( ss );
      }
      ss.append( ']' );
      return ss;
    }
    
    void GlobalVector::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
      {
        try
        {
          (*it)->llvmPrepareModule( moduleBuilder, diagnostics );
        }
        catch ( CG::Error e )
        {
          diagnostics.addError( e.getLocation(), e.getDesc() );
        }
      }
    }
    
    void GlobalVector::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctions ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
      {
        try
        {
          (*it)->llvmCompileToModule( moduleBuilder, diagnostics, buildFunctions );
        }
        catch ( CG::Error e )
        {
          diagnostics.addError( e.getLocation(), e.getDesc() );
        }
      }
    }
  };
};
