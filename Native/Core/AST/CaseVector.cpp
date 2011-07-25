/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/CaseVector.h>
#include <Fabric/Core/AST/Case.h>
#include <Fabric/Core/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    RC::ConstHandle<CaseVector> CaseVector::Create( RC::ConstHandle<Case> const &first, RC::ConstHandle<CaseVector> const &remaining )
    {
      CaseVector *result = new CaseVector;
      if ( first )
        result->push_back( first );
      if ( remaining )
      {
        for ( const_iterator it=remaining->begin(); it!=remaining->end(); ++it )
          result->push_back( *it );
      }
      return result;
    }
    
    CaseVector::CaseVector()
    {
    }
    
    void CaseVector::appendJSON( Util::SimpleString &ss ) const
    {
      ss.append( '[' );
      for ( const_iterator it=begin(); it!=end(); ++it )
      {
        if ( it != begin() )
          ss.append( ',' );
        (*it)->appendJSON( ss );
      }
      ss.append( ']' );
    }
    
    void CaseVector::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->llvmPrepareModule( moduleBuilder, diagnostics );
    }
  };
};
