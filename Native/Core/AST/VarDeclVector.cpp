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
    RC::ConstHandle<VarDeclVector> VarDeclVector::Create( RC::ConstHandle<VarDecl> const &first, RC::ConstHandle<VarDeclVector> const &remaining )
    {
      VarDeclVector *result = new VarDeclVector;
      if ( first )
        result->push_back( first );
      if ( remaining )
      {
        for ( const_iterator it=remaining->begin(); it!=remaining->end(); ++it )
          result->push_back( *it );
      }
      return result;
    }
    
    VarDeclVector::VarDeclVector()
    {
    }
    
    RC::ConstHandle<JSON::Value> VarDeclVector::toJSON() const
    {
      if ( !m_jsonValue )
      {
        RC::Handle<JSON::Array> result = JSON::Array::Create();
        for ( const_iterator it=begin(); it!=end(); ++it )
          result->push_back( (*it)->toJSONImpl() );
        m_jsonValue = result;
      }
      return m_jsonValue;
    }
    
    void VarDeclVector::llvmPrepareModule( std::string const &baseType, CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->llvmPrepareModule( baseType, moduleBuilder, diagnostics );
    }
    
    void VarDeclVector::llvmCompileToBuilder( std::string const &baseType, CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->llvmCompileToBuilder( baseType, basicBlockBuilder, diagnostics );
    }
  };
};
