/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_DG_BINDING_LIST_H
#define _FABRIC_DG_BINDING_LIST_H

#include <Fabric/Core/DG/CompiledObject.h>

#include <list>
#include <string>

namespace Fabric
{
  namespace JSON
  {
    class Value;
    class Array;
  };
  
  namespace DG
  {
    class Binding;
    class Context;
    class NamedObject;
    
    class BindingList : public CompiledObject
    {
      typedef std::list< RC::Handle<Binding> > Bindings;
      
    public:
      
      static RC::Handle<BindingList> Create( RC::Handle<Context> const &context );
      
      void addOwner( NamedObject *owner, std::string const &subName );
      void removeOwner( NamedObject *owner );
      
      RC::Handle<Binding> get( size_t idx );
      void append( RC::Handle<Binding> const &op );
      void insert( RC::Handle<Binding> const &op, size_t beforeIdx );
      void move( size_t fromIdx, size_t toBeforeIdx );
      void remove( size_t idx );
      void clear();
      size_t size() const;
      
      RC::ConstHandle<JSON::Value> jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      RC::ConstHandle<JSON::Value> jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      void jsonExecAppend( RC::ConstHandle<JSON::Value> const &arg );
      void jsonExecInsert( RC::ConstHandle<JSON::Value> const &arg );
      void jsonExecRemove( RC::ConstHandle<JSON::Value> const &arg );
      RC::Handle<JSON::Array> jsonDesc() const;
      
    protected:
    
      BindingList( RC::Handle<Context> const &context );
      ~BindingList();
    
      void jsonNotify() const;
      
      virtual void propagateMarkForRecompileImpl( unsigned generation );    
      virtual void propagateMarkForRefreshImpl( unsigned generation );    
      virtual void collectErrors();
      virtual void invalidateRunState();
      virtual void refreshRunState();
      virtual void collectTasksImpl( unsigned generation, MT::TaskGroupStream &taskGroupStream ) const;
      virtual bool canExecute() const;
      
    private:
      
      Context *m_context;
      Bindings m_bindings;
      std::string m_subName;
      NamedObject *m_owner;
    };
  }
}

#endif // _FABRIC_DG_BINDING_LIST_H