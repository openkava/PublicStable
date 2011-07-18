/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "MemberDeclVector.h"
#include "MemberDecl.h"
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace AST
  {
    RC::Handle<MemberDeclVector> MemberDeclVector::Create()
    {
      return new MemberDeclVector;
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
  };
};
