/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_FUNCTION_H
#define _FABRIC_KL_FUNCTION_H

#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/KLC/GenericFunctionPtr.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace Util
  {
    class JSONGenerator;
    class JSONObjectGenerator;
  };
  
  namespace AST
  {
    class Function;
    class GlobalList;
  };
  
  namespace CG
  {
    class Diagnostics;
  };
  
  namespace KLC
  {
    class Executable;
    
    class Function : public GC::Object
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    public:
    
      static RC::Handle<Function> Create(
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Function> const &astFunction,
        GenericFunctionPtr functionPtr
        );
    
      RC::ConstHandle<AST::GlobalList> getAST() const;
      CG::Diagnostics const &getDiagnostics() const;
      RC::ConstHandle<AST::Function> getASTFunction() const;
      
      void toJSON( Util::JSONGenerator &jg ) const;
        
      virtual void jsonExec(
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
    protected:
    
      Function(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Function> const &astFunction,
        GenericFunctionPtr functionPtr
        );
       
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;

      GenericFunctionPtr getGenericFunctionPtr() const
      {
        return m_functionPtr;
      }
   
    private:
    
      void jsonExecGetDiagnostics(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    
      void jsonExecToJSON(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    
      RC::ConstHandle<Executable> m_executable;
      RC::ConstHandle<AST::Function> m_astFunction;
      GenericFunctionPtr m_functionPtr;
    };
  }
}

#endif //_FABRIC_KL_OPERATOR_H
