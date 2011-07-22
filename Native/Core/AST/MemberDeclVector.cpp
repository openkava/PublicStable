/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "MemberDeclVector.h"
#include "MemberDecl.h"
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    RC::ConstHandle<MemberDeclVector> MemberDeclVector::Create( RC::ConstHandle<MemberDecl> const &first, RC::ConstHandle<MemberDeclVector> const &remaining )
    {
      MemberDeclVector *result = new MemberDeclVector;
      if ( first )
        result->push_back( first );
      if ( remaining )
      {
        for ( MemberDeclVector::const_iterator it=remaining->begin(); it!=remaining->end(); ++it )
          result->push_back( *it );
      }
      return result;
    }
    
    MemberDeclVector::MemberDeclVector()
    {
    }
    
    RC::Handle<JSON::Array> MemberDeclVector::toJSON() const
    {
      RC::Handle<JSON::Array> result = JSON::Array::Create();
      for ( size_t i=0; i<size(); ++i )
        result->push_back( get(i)->toJSON() );
      return result;
    }
    
    void MemberDeclVector::buildStructMemberInfoVector( RC::ConstHandle<RT::Manager> const &rtManager, RT::StructMemberInfoVector &structMemberInfoVector ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
      {
        RT::StructMemberInfo structMemberInfo;
        (*it)->buildStructMemberInfo( rtManager, structMemberInfo );
        structMemberInfoVector.push_back( structMemberInfo );
      }
    }
    
    void MemberDeclVector::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->llvmPrepareModule( moduleBuilder, diagnostics );
    }
  };
};
