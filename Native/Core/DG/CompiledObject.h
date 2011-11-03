/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_DG_COMPILED_OBJECT_H
#define _FABRIC_DG_COMPILED_OBJECT_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <set>
#include <vector>

namespace Fabric
{
  namespace Util
  {
    class JSONGenerator;
    class JSONArrayGenerator;
  };
  
  namespace MT
  {
    class TaskGroupStream;
  };
  
  namespace JSON
  {
    class Value;
  };
  
  namespace DG
  {
    class Context;
    
    class CompiledObject : public RC::Object
    {
    public:

      typedef std::vector<std::string> Errors;
    
      virtual bool isNamedObject() const { return false; }
      
      Errors const &getErrors() const;
      Errors &getErrors();
      virtual bool canExecute() const = 0;
      
      void markForRecompile( unsigned generation );
      void markForRefresh( unsigned generation );
      
      static void PrepareForExecution();
      
      virtual void jsonNotifyErrorDelta() const;
      void jsonDescErrors( Util::JSONGenerator &resultJG ) const;

    protected:
    
      CompiledObject( RC::Handle<Context> const &context );
      
      void markForRecompile();
      virtual void propagateMarkForRecompileImpl( unsigned generation ) = 0;    

      static void Compile( std::set< RC::Handle<CompiledObject> > const &compiledObjectsMarkedForRecompile );
      
      virtual void collectErrors() = 0;
      virtual void invalidateRunState() = 0;
      
      void markForRefresh();
      virtual void propagateMarkForRefreshImpl( unsigned generation ) = 0;    

      static void Refresh( std::set< RC::Handle<CompiledObject> > const &compiledObjectsMarkedForRefresh );

      virtual void refreshRunState() = 0;

      void collectTasks( MT::TaskGroupStream &taskGroupStream ) const;
      void collectTasks( unsigned generation, MT::TaskGroupStream &taskGroupStream ) const;
      virtual void collectTasksImpl( unsigned generation, MT::TaskGroupStream &taskGroupStream ) const = 0;

    private:
    
      static std::set< RC::Handle<CompiledObject> > s_compiledObjectsMarkedForRecompile;
      static unsigned s_markForRecompileGlobalGeneration;
      mutable unsigned m_markForRecompileGeneration;
      
      static std::set< RC::Handle<CompiledObject> > s_compiledObjectsMarkedForRefresh;
      static unsigned s_markForRefreshGlobalGeneration;
      mutable unsigned m_markForRefreshGeneration;
      
      static unsigned s_collectTasksGlobalGeneration;
      mutable unsigned m_collectTasksGeneration;
      
      Errors m_errors;
      bool m_canExecute;
    };
  };
};

#endif //_FABRIC_DG_COMPILED_OBJECT_H
