/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/FunctionWrapper.h>
#include <Fabric/Core/KLC/Executable.h>
#include <Fabric/Core/AST/Function.h>
#include <Fabric/Core/AST/GlobalList.h>

namespace Fabric
{
  namespace KLC
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( FunctionWrapper, GC::Object )
      
    FunctionWrapper::FunctionWrapper(
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

    RC::ConstHandle<AST::Function> FunctionWrapper::getASTFunction() const
    {
      return m_astFunction;
    }
    
    void FunctionWrapper::toJSON( Util::JSONGenerator &jg ) const
    {
      if ( !m_astFunction || !m_functionPtr )
        throw Exception("function is unresolved");
    
      Util::JSONObjectGenerator jog = jg.makeObject();
      
      {
        Util::JSONGenerator jg = jog.makeMember( "kind" );
        jg.makeString( getKind() );
      }
      
      toJSONImpl( jog );
    }
    
    void FunctionWrapper::toJSONImpl( Util::JSONObjectGenerator &jog ) const
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
    
    RC::ConstHandle<AST::GlobalList> FunctionWrapper::getAST() const
    {
      return m_executable->getAST();
    }
    
    CG::Diagnostics const &FunctionWrapper::getDiagnostics() const
    {
      return m_executable->getDiagnostics();
    }
        
    void FunctionWrapper::jsonExec(
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
    
    void FunctionWrapper::jsonExecGetDiagnostics(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      Util::JSONGenerator jg = resultJAG.makeElement();
      getDiagnostics().generateJSON( jg );
    }
    
    void FunctionWrapper::jsonExecToJSON(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      Util::JSONGenerator jg = resultJAG.makeElement();
      toJSON( jg );
    }
  }
}
