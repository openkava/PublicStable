/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_BINDING_H
#define _FABRIC_DG_BINDING_H

#include <Fabric/Core/DG/CompiledObject.h>

namespace Fabric
{
  namespace MT
  {
    class ParallelCall;
  };
  
  namespace JSON
  {
    class Value;
    class Object;
  };
  
  namespace DG
  {
    class Context;
    class Scope;
    class Prototype;
    class BindingList;
    class Operator;
    
    class Binding : public CompiledObject
    {
      typedef std::multiset<BindingList *> BindingLists; 
    
    public:
    
      static RC::Handle<Binding> Create( RC::Handle<Context> const &context );
      
      void setPrototype( std::vector<std::string> const &parameterDescs );
      std::vector<std::string> getPrototype() const;
      
      RC::Handle<Operator> getOperator() const;
      void setOperator( RC::Handle<Operator> const &op );
      
      std::string const &getOperatorName() const;
      
      void addBindingList( BindingList *bindingList );
      void removeBindingList( BindingList *bindingList );

      RC::Handle<MT::ParallelCall> bind( Scope const &scope, size_t *newSize, unsigned prefixCount=0, void * const *prefixes = 0 ) const;
      
      RC::ConstHandle<JSON::Value> jsonDesc() const;
      
    protected:
    
      Binding( RC::Handle<Context> const &context );
      ~Binding();
      
      virtual void propagateMarkForRecompileImpl( unsigned generation );    
      virtual void propagateMarkForRefreshImpl( unsigned generation );    
      virtual void collectErrors();
      virtual void invalidateRunState();
      virtual void refreshRunState();
      virtual void collectTasksImpl( unsigned generation, MT::TaskGroupStream &taskGroupStream ) const;
      virtual bool canExecute() const;

    private:
    
      BindingLists m_bindingLists;
    
      Prototype *m_prototype;
      RC::Handle<Operator> m_operator;
      
      Context *m_context;
    };
  };
};

#endif //_FABRIC_DG_BINDING_H
