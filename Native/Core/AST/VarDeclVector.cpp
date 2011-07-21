/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/VarDeclVector.h>
#include <Fabric/Core/AST/VarDecl.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    RC::Handle<VarDeclVector> VarDeclVector::Create()
    {
      return new VarDeclVector;
    }
    
    RC::Handle<VarDeclVector> VarDeclVector::Create( RC::ConstHandle<VarDecl> const &first )
    {
      RC::Handle<VarDeclVector> result = Create();
      result->push_back( first );
      return result;
    }
    
    RC::Handle<VarDeclVector> VarDeclVector::Create( RC::ConstHandle<VarDecl> const &first, RC::ConstHandle<VarDeclVector> const &remaining )
    {
      RC::Handle<VarDeclVector> result = Create( first );
      for ( const_iterator it=remaining->begin(); it!=remaining->end(); ++it )
        result->push_back( *it );
      return result;
    }
    
    VarDeclVector::VarDeclVector()
    {
    }
    
    RC::Handle<JSON::Array> VarDeclVector::toJSON() const
    {
      RC::Handle<JSON::Array> result = JSON::Array::Create();
      for ( const_iterator it=begin(); it!=end(); ++it )
        result->push_back( (*it)->toJSON() );
      return result;
    }
    
    void VarDeclVector::llvmCompileToBuilder( std::string const &baseType, CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->llvmCompileToBuilder( baseType, basicBlockBuilder, diagnostics );
    }
  };
};