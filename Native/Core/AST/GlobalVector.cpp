/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/GlobalVector.h>
#include <Fabric/Core/AST/Global.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    RC::Handle<GlobalVector> GlobalVector::Create()
    {
      return new GlobalVector;
    }
    
    RC::Handle<GlobalVector> GlobalVector::Create( RC::ConstHandle<Global> const &first, RC::ConstHandle<GlobalVector> const &remaining )
    {
      RC::Handle<GlobalVector> globalVector = new GlobalVector;
      globalVector->push_back( first );
      for ( GlobalVector::const_iterator it=remaining->begin(); it!=remaining->end(); ++it )
        globalVector->push_back( *it );
      return globalVector;
    }
    
    GlobalVector::GlobalVector()
    {
    }
    
    RC::Handle<JSON::Array> GlobalVector::toJSON() const
    {
      RC::Handle<JSON::Array> result = JSON::Array::Create();
      for ( size_t i=0; i<size(); ++i )
        result->push_back( get(i)->toJSON() );
      return result;
    }
          
    void GlobalVector::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( size_t i=0; i<size(); ++i )
        get(i)->llvmCompileToModule( moduleBuilder, diagnostics, false );
      for ( size_t i=0; i<size(); ++i )
        get(i)->llvmCompileToModule( moduleBuilder, diagnostics, true );
    }
  };
};
