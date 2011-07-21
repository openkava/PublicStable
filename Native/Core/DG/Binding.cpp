/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/Binding.h>
#include <Fabric/Core/DG/Prototype.h>
#include <Fabric/Core/DG/BindingList.h>
#include <Fabric/Core/DG/Operator.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Base/JSON/Null.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace DG
  {
    RC::Handle<Binding> Binding::Create( RC::Handle<Context> const &context )
    {
      return new Binding( context );
    }

    Binding::Binding( RC::Handle<Context> const &context )
      : CompiledObject( context )
      , m_prototype( 0 )
      , m_context( context.ptr() )
    {
    }
    
    Binding::~Binding()
    {
      if ( m_operator )
        m_operator->removeBinding( this );
      FABRIC_ASSERT( m_bindingLists.empty() );
      delete m_prototype;
    }
    
    void Binding::propagateMarkForRecompileImpl( unsigned generation )
    {
      for ( BindingLists::const_iterator it=m_bindingLists.begin(); it!=m_bindingLists.end(); ++it )
        (*it)->markForRecompile( generation );
    }
      
    void Binding::propagateMarkForRefreshImpl( unsigned generation )
    {
      for ( BindingLists::const_iterator it=m_bindingLists.begin(); it!=m_bindingLists.end(); ++it )
        (*it)->markForRefresh( generation );
    }
    
    void Binding::collectErrors()
    {
      Errors &errors = getErrors();
      if ( !m_prototype )
        errors.push_back( "no prototype set" );
      if ( !m_operator )
        errors.push_back( "no operator set" );
    }
    
    void Binding::invalidateRunState()
    {
    }
    
    void Binding::refreshRunState()
    {
    }
    
    void Binding::collectTasksImpl( unsigned generation, MT::TaskGroupStream &taskGroupStream ) const
    {
    }
    
    bool Binding::canExecute() const
    {
      return m_prototype
        && m_operator;
    }
      
    void Binding::setPrototype( std::vector<std::string> const &parameterDescs )
    {
      Prototype *newPrototype = new Prototype( m_context->getCGManager() );
      newPrototype->setDescs( parameterDescs );
      delete m_prototype;
      m_prototype = newPrototype;
      markForRecompile();
    }
    
    std::vector<std::string> Binding::getPrototype() const
    {
      if ( !m_prototype )
        throw Exception( "no prototype set" );
      return m_prototype->desc();
    }
    
    RC::Handle<Operator> Binding::getOperator() const
    {
      return m_operator;
    }
    
    void Binding::setOperator( RC::Handle<Operator> const &op )
    {
      if ( m_operator )
        m_operator->removeBinding( this );
      m_operator = op;
      if ( m_operator )
        m_operator->addBinding( this );
        
      markForRecompile();
    }
    
    RC::Handle<MT::ParallelCall> Binding::bind( Scope const &scope, size_t *newSize, unsigned prefixCount, void * const *prefixes ) const
    {
      if ( !m_prototype )
        throw Exception( "no prototype set" );
      if ( !m_operator )
        throw Exception( "no operator set" );
        
      RC::Handle<MT::ParallelCall> result;
      try
      {
        result = m_operator->bind( m_prototype, scope, newSize, prefixCount, prefixes );
      }
      catch ( Exception e )
      {
        throw "operator " + _(m_operator->getName()) + ": " + e;
      }
      return result;
    }
      
    std::string const &Binding::getOperatorName() const
    {
      if ( !m_operator )
        throw Exception( "no operator set" );
      return m_operator->getName();
    }
    
    void Binding::addBindingList( BindingList *bindingList )
    {
      m_bindingLists.insert( bindingList );
    }
    
    void Binding::removeBindingList( BindingList *bindingList )
    {
      BindingLists::iterator it = m_bindingLists.find( bindingList );
      FABRIC_ASSERT( it != m_bindingLists.end() );
      m_bindingLists.erase( it );
    }
      
    RC::ConstHandle<JSON::Value> Binding::jsonDesc() const
    {
      RC::ConstHandle<JSON::Value> parameterLayoutJSONValue;
      if ( m_prototype )
        parameterLayoutJSONValue = m_prototype->jsonDesc();
      else parameterLayoutJSONValue = JSON::Null::Create();
      
      RC::ConstHandle<JSON::Value> operatorJSONValue;
      if ( m_operator )
        operatorJSONValue = JSON::String::Create( m_operator->getName() );
      else operatorJSONValue = JSON::Null::Create();
    
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( "parameterLayout", parameterLayoutJSONValue );
      result->set( "operator", operatorJSONValue );
      return result;
    }
    
    bool Binding::getMainThreadOnly() const
    {
      return m_operator && m_operator->getMainThreadOnly();
    }
  };
};
