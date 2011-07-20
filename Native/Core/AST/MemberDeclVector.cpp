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
    RC::Handle<MemberDeclVector> MemberDeclVector::Create()
    {
      return new MemberDeclVector;
    }
    
    RC::Handle<MemberDeclVector> MemberDeclVector::Create( RC::ConstHandle<MemberDecl> const &first )
    {
      RC::Handle<MemberDeclVector> result = Create();
      result->push_back( first );
      return result;
    }
    
    RC::Handle<MemberDeclVector> MemberDeclVector::Create( RC::ConstHandle<MemberDecl> const &first, RC::Handle<MemberDeclVector> const &remaining )
    {
      RC::Handle<MemberDeclVector> result = Create( first );
      for ( MemberDeclVector::const_iterator it=remaining->begin(); it!=remaining->end(); ++it )
        result->push_back( *it );
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

    void MemberDeclVector::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->llvmCompileToModule( moduleBuilder, diagnostics );
    }
  };
};
