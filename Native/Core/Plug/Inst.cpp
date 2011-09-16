/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "Inst.h"

#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Decode.h>
#include <Fabric/Core/KL/StringSource.h>
#include <Fabric/Core/KL/Scanner.h>
#include <Fabric/Core/KL/Parser.hpp>
#include <Fabric/Core/AST/Function.h>
#include <Fabric/Core/AST/GlobalList.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Core/IO/Dir.h>
#include <Fabric/Core/Plug/Helpers.h>
#include <Fabric/Core/Build.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/EDK/Common.h>

namespace Fabric
{
  namespace Plug
  {
    static void throwException( size_t length, char const *data )
    {
      throw Exception( length, data );
    }
    
    //typedef void (*OnLoadFn)( SDK::Value FABRIC );
    //typedef void (*OnUnloadFn)( SDK::Value FABRIC );
    
    RC::Handle<Inst> Inst::Create( RC::ConstHandle<IO::Dir> const &extensionDir, std::string const &name, std::string const &jsonDesc, std::vector<std::string> const &pluginDirs, RC::Handle<CG::Manager> const &cgManager )
    {
      return new Inst( extensionDir, name, jsonDesc, pluginDirs, cgManager );
    }
      
    Inst::Inst( RC::ConstHandle<IO::Dir> const &extensionDir, std::string const &extensionName, std::string const &jsonDesc, std::vector<std::string> const &pluginDirs, RC::Handle<CG::Manager> const &cgManager )
      : m_name( extensionName )
      , m_jsonDesc( jsonDesc )
    {
      try
      {
        RC::ConstHandle<JSON::Value> value = JSON::decode( jsonDesc );
        if ( !value->isObject() )
          throw Exception( "top level must be a JSON object" );
        RC::ConstHandle<JSON::Object> object = RC::ConstHandle<JSON::Object>::StaticCast( value );
        m_desc = parseDesc( object );
      }
      catch ( Exception e )
      {
        throw "JSON description: " + e;
      }
      
      /*
      m_fabricLIBObject = LIB::NakedObject::Create();
      m_fabricLIBObject->set( "hostTriple", LIB::ReferencedString::Create( Util::getHostTriple() ) );
      m_fabricLIBObject->set( "DependencyGraph", LIBDG::Namespace::Create( dgContext ) );
      */
      std::vector< std::string > libs;
      m_desc.libs.appendMatching( Util::getHostTriple(), libs );
      std::string libSuffix = "-" + std::string(buildOS) + "-" + std::string(buildArch);
      
      for ( size_t i=0; i<libs.size(); ++i )
      {
        std::string resolvedName;
        SOLibHandle soLibHandle = SOLibOpen( libs[i]+libSuffix, resolvedName, false, pluginDirs );
        m_resolvedNameToSOLibHandleMap.insert( ResolvedNameToSOLibHandleMap::value_type( resolvedName, soLibHandle ) );
        m_orderedSOLibHandles.push_back( soLibHandle );
      }

      void *resolvedFabricEDKInitFunction = 0;
      for ( size_t i=0; i<m_orderedSOLibHandles.size(); ++i )
      {
        resolvedFabricEDKInitFunction = SOLibResolve( m_orderedSOLibHandles[i], "FabricEDKInit" );
        if ( resolvedFabricEDKInitFunction )
          break;
      }
      if ( !resolvedFabricEDKInitFunction )
        throw Exception( "error: extension doesn't implement function FabricEDKInit through macro IMPLEMENT_FABRIC_EDK_ENTRIES" );

      Fabric::EDK::Callbacks callbacks;
      callbacks.m_malloc = malloc;
      callbacks.m_realloc = realloc;
      callbacks.m_free = free;
      callbacks.m_throwException = throwException;

      ( *( FabricEDKInitPtr )resolvedFabricEDKInitFunction )( callbacks );
      
      for ( size_t i=0; i<m_orderedSOLibHandles.size(); ++i )
      {
        /*
        OnLoadFn onLoadFn = (OnLoadFn)SOLibResolve( m_orderedSOLibHandles[i], "FabricOnLoad" );
        if ( onLoadFn )
          onLoadFn( SDK::Value::Bind( m_fabricLIBObject ) );
        */
      }
      
      /*
      for ( size_t i=0; i<m_desc.interface.methods.size(); ++i )
      {
        std::string const &methodName = m_desc.interface.methods[i];
        Method method = 0;
        for ( size_t j=0; j<m_orderedSOLibHandles.size(); ++j )
        {
          SOLibHandle soLibHandle = m_orderedSOLibHandles[j];
          method = (Method)SOLibResolve( soLibHandle, methodName );
          if ( method )
            break;
        }
        if ( !method )
          throw Exception( "method "+_(methodName)+" not found" );
        m_methodMap.insert( MethodMap::value_type( methodName, method ) );
      }
      */
      
      
      std::vector<std::string> codeFiles;
      m_desc.code.appendMatching( Util::getHostTriple(), codeFiles );
      m_code = "";
      for ( std::vector<std::string>::const_iterator it=codeFiles.begin(); it!=codeFiles.end(); ++it )
      {
        std::string const &codeEntry = *it;
        std::string code;
        try
        {
          code = extensionDir->getFileContents( codeEntry );
        }
        catch ( Exception e )
        {
          throw _(codeEntry) + ": " + e;
        }
        m_code += code + "\n";
      }
      
      RC::ConstHandle<KL::Source> source = KL::StringSource::Create( m_code );
      RC::Handle<KL::Scanner> scanner = KL::Scanner::Create( source );
      m_ast = KL::Parse( scanner, m_diagnostics );
      for ( CG::Diagnostics::const_iterator it=m_diagnostics.begin(); it!=m_diagnostics.end(); ++it )
      {
        CG::Location const &location = it->first;
        CG::Diagnostic const &diagnostic = it->second;
        FABRIC_LOG( "[%s] %u:%u: %s: %s", extensionName.c_str(), (unsigned)location.getLine(), (unsigned)location.getColumn(), diagnostic.getLevelDesc(), diagnostic.getDesc().c_str() );
      }
      if ( m_diagnostics.containsError() )
        throw Exception( "KL compile failed" );
      
      std::vector< RC::ConstHandle<AST::Function> > functions;
      m_ast->collectFunctions( functions );
      for ( std::vector< RC::ConstHandle<AST::Function> >::const_iterator it=functions.begin(); it!=functions.end(); ++it )
      {
        RC::ConstHandle<AST::Function> const &function = *it;
        
        if ( !function->getBody() )
        {
          std::string const &name = function->getEntryName( cgManager );
          void *resolvedFunction = 0;
          for ( size_t i=0; i<m_orderedSOLibHandles.size(); ++i )
          {
            resolvedFunction = SOLibResolve( m_orderedSOLibHandles[i], name );
            if ( resolvedFunction )
              break;
          }
          if ( !resolvedFunction )
            throw Exception( "error: symbol " + _(name) + ", prototyped in KL, not found in native code" );
          m_externalFunctionMap.insert( ExternalFunctionMap::value_type( name, resolvedFunction ) );
        }
      }

      m_jsConstants = m_desc.jsConstants.concatMatching( Util::getHostTriple() );
    }
    
    Inst::~Inst()
    {
      for ( size_t i=m_orderedSOLibHandles.size(); i--; )
      {
        /*
        OnUnloadFn onUnloadFn = (OnUnloadFn)SOLibResolve( m_orderedSOLibHandles[i], "FabricOnUnload" );
        if ( onUnloadFn )
          onUnloadFn( SDK::Value::Bind( m_fabricLIBObject ) );
        */
      }
      
      for ( ResolvedNameToSOLibHandleMap::const_iterator it=m_resolvedNameToSOLibHandleMap.begin(); it !=m_resolvedNameToSOLibHandleMap.end(); ++it )
      {
        SOLibClose( it->second, it->first );
      }
    }
    
    void *Inst::llvmResolveExternalFunction( std::string const &name ) const
    {
      void *result = 0;
      ExternalFunctionMap::const_iterator it = m_externalFunctionMap.find( name );
      if ( it != m_externalFunctionMap.end() )
        result = it->second;
      return result;
    }

    bool Inst::hasMethod( std::string const &methodName ) const
    {
      /*
      MethodMap::const_iterator it = m_methodMap.find( methodName );
      return it != m_methodMap.end();
      */
      return false;
    }
    
    void Inst::enumerateMethods( std::vector<std::string> &result ) const
    {
      /*
      for ( MethodMap::const_iterator it=m_methodMap.begin(); it!=m_methodMap.end(); ++it )
        result.push_back( it->first );
      */
    }
    
      /*
    RC::Handle<LIB::Value> Inst::invokeMethod( std::string const &methodName, std::vector< RC::Handle<LIB::Value> > const &args )
    {
      MethodMap::const_iterator it = m_methodMap.find( methodName );
      FABRIC_ASSERT( it != m_methodMap.end() );
      return SDK::Value::Unbind( it->second( SDK::Value::Bind( m_fabricLIBObject ), SDK::Value::LIBArgsToSDKArgs( args ) ) );
    }
      */

    RC::Handle<JSON::Object> Inst::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( "code", JSON::String::Create( m_code ) );
      result->set( "jsConstants", JSON::String::Create( m_jsConstants ) );
      return result;
    }

      
    RC::ConstHandle<AST::GlobalList> Inst::getAST() const
    {
      return m_ast;
    }
  };
};
