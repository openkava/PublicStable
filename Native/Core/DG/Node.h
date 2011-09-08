/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_NODE_H
#define _FABRIC_DG_NODE_H

#include <Fabric/Core/DG/Container.h>
#include <Fabric/Core/MT/Task.h>
#include <Fabric/Core/MT/TaskGroupStream.h>
#include <Fabric/Core/MT/ParallelCall.h>
#include <Fabric/Core/Util/UnorderedMap.h>

#include <map>
#include <string>

namespace Fabric
{
  namespace Util
  {
    class SimpleString;
    class Encoder;
    class Decoder;
  };
  
  namespace JSON
  {
    class Value;
    class Array;
  };
  
  namespace RT
  {
    class Manager;
    class Desc;
    class Impl;
    class VariableArrayDesc;
  };
  
  namespace DG
  {
    class Prototype;
    class Operator;
    class Scope;
    class EventHandler;
    class BindingList;
    class Context;
    
    class Node : public Container
    {
      friend class EventHandler;
      friend class Prototype;
      
    public:
    
      typedef Util::UnorderedMap< std::string, RC::Handle<Node> > Dependencies;
      typedef std::multiset<Node *> Dependents;
      
      typedef std::multiset<EventHandler *> EventHandlers;
    
      static RC::Handle<Node> Create( std::string const &name, RC::Handle<Context> const &context );

      virtual bool isNode() const { return true; }

      void addDependency( RC::Handle<Node> const &dependencyNode, std::string const &dependencyName );
      void removeDependency( std::string const &dependencyName );
      RC::Handle<Node> getDependency( std::string const &dependencyName ) const;
      Dependencies const &getDependencies() const;

      void addDependent( RC::Handle<Node> const &childNode );
      void removeDependent( RC::Handle<Node> const &childNode );
      Dependents const &getDependents() const;
      
      RC::Handle<BindingList> getBindingList() const;
      void addScopeList( RC::Handle<BindingList> const &opList );
      void evaluate();
      
      void addEventHandler( EventHandler *eventHandler );
      void removeEventHandler( EventHandler *eventHandler );

      RC::Handle<Context> getContext() const;

      virtual RC::ConstHandle<JSON::Value> jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      virtual RC::ConstHandle<JSON::Value> jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      static void jsonExecCreate( RC::ConstHandle<JSON::Value> const &arg, RC::Handle<Context> const &context );
      void jsonExecAddDependency( RC::ConstHandle<JSON::Value> const &arg );
      void jsonExecEvaluate();
      virtual RC::Handle<JSON::Object> jsonDesc() const;
      virtual RC::ConstHandle<JSON::Value> jsonDescType() const;
      RC::ConstHandle<JSON::Value> jsonDescDependencies() const;
      
    protected:
    
      Node( std::string const &name, RC::Handle<Context> const &context );
      ~Node();
      
      virtual void setOutOfDate();
      
      RC::Handle<MT::ParallelCall> bind(
        RC::ConstHandle<Binding> const &binding,
        Scope const &scope,
        size_t *newCount,
        unsigned prefixCount = 0,
        void * const *prefixes = 0
        );
      
      void recalculateGlobalDependencyRank();
      void maybeIncreaseGlobalDependencyRank( size_t dependencyGlobalDependencyRank );
    
      virtual void propagateMarkForRecompileImpl( unsigned generation );    
      virtual void propagateMarkForRefreshImpl( unsigned generation );    
      virtual void collectErrors();
      virtual void invalidateRunState();
      virtual void refreshRunState();
      virtual void collectTasksImpl( unsigned generation, MT::TaskGroupStream &taskGroupStream ) const;
      virtual bool canExecute() const;
      
      virtual void evaluateLocal( void *userdata );
      
      void ensureRunState() const;

      void invalidateEvaluateParallelCall();
      MT::ParallelCall const &getEvaluateParallelCall();
      
      bool canBeDependency( RC::Handle<Node> const &node ) const;
      void colorSelfAndDependencies( size_t colorGeneration );

    private:
    
      Context *m_context;
      
      bool m_dirty;
    
      Dependencies m_dependencies;
      Dependents m_dependents;
      size_t m_globalDependencyRank;
      
      mutable RC::Handle<BindingList> m_bindingList;
      
      EventHandlers m_eventHandlers;
      
      MT::Task<Node> m_evaluateLocalTask;
      
      struct RunState;
      mutable RunState *m_runState;
      
      size_t m_colorGeneration;
      static size_t s_globalColorGeneration;
    };
  };
};

#endif //_FABRIC_DG_NODE_H
