/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/VarDeclVector.h>
#include <Fabric/Core/AST/VarDecl.h>
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Base/Util/SimpleString.h>

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
    
    void VarDeclVector::appendJSON( Util::JSONGenerator const &jsonGenerator, bool includeLocation ) const
    {
      Util::JSONArrayGenerator jsonArrayGenerator = jsonGenerator.makeArray();
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->appendJSON( jsonArrayGenerator.makeElement(), includeLocation );
    }
    
    void VarDeclVector::registerTypes( std::string const &baseType, RC::Handle<CG::Manager> const &cgManager, CG::Diagnostics &diagnostics ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->registerTypes( baseType, cgManager, diagnostics );
    }
    
    void VarDeclVector::llvmCompileToBuilder( std::string const &baseType, CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->llvmCompileToBuilder( baseType, basicBlockBuilder, diagnostics );
    }
  };
};
