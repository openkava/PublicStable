/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/Function.h>
#include <Fabric/Core/KLC/Executable.h>
#include <Fabric/Core/AST/Function.h>
#include <Fabric/Core/AST/GlobalList.h>

namespace Fabric
{
  namespace KLC
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( Function, GC::Object )
      
    RC::Handle<Function> Function::Create(
      RC::ConstHandle<Executable> const &executable,
      RC::ConstHandle<AST::Function> const &astFunction,
      GenericFunctionPtr functionPtr
      )
    {
      return new Function(
        FABRIC_GC_OBJECT_MY_CLASS,
        executable,
        astFunction,
        functionPtr
        );
    }

    Function::Function(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<Executable> const &executable,
      RC::ConstHandle<AST::Function> const &astFunction,
      GenericFunctionPtr functionPtr
      )
      : GC::Object( FABRIC_GC_OBJECT_CLASS_ARG )
      , m_executable( executable )
      , m_astFunction( astFunction )
      , m_functionPtr( functionPtr )
    {
    }

    RC::ConstHandle<AST::Function> Function::getASTFunction() const
    {
      return m_astFunction;
    }
    
    GenericFunctionPtr Function::getGenericFunctionPtr() const
    {
      return m_functionPtr;
    }
    
    void Function::toJSON( Util::JSONGenerator &jg ) const
    {
      Util::JSONObjectGenerator jog = jg.makeObject();
      
      {
        Util::JSONGenerator jg = jog.makeMember( "kind" );
        jg.makeString( getKind() );
      }
      
      toJSONImpl( jog );
    }
    
    char const *Function::getKind() const
    {
      return "Function";
    }
    
    void Function::toJSONImpl( Util::JSONObjectGenerator &jog ) const
    {
      {
        Util::JSONGenerator jg = jog.makeMember( "entryName" );
        jg.makeString( m_astFunction->getEntryName( m_executable->getCGManager() ) );
      }
      
      {
        Util::JSONGenerator jg = jog.makeMember( "ast" );
        getAST()->generateJSON( false, jg );
      }
    }
    
    RC::ConstHandle<AST::GlobalList> Function::getAST() const
    {
      return m_executable->getAST();
    }
    
    CG::Diagnostics const &Function::getDiagnostics() const
    {
      return m_executable->getDiagnostics();
    }
        
    void Function::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "getDiagnostics" )
        jsonExecGetDiagnostics( arg, resultJAG );
      else if ( cmd == "toJSON" )
        jsonExecToJSON( arg, resultJAG );
      else GC::Object::jsonExec( cmd, arg, resultJAG );
    }
    
    void Function::jsonExecGetDiagnostics(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      Util::JSONGenerator jg = resultJAG.makeElement();
      getDiagnostics().generateJSON( jg );
    }
    
    void Function::jsonExecToJSON(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      Util::JSONGenerator jg = resultJAG.makeElement();
      toJSON( jg );
    }
  }
}
