/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KLC_FUNCTION_WRAPPER_H
#define _FABRIC_KLC_FUNCTION_WRAPPER_H

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
    
    class FunctionWrapper : public GC::Object
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    public:
    
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
    
      FunctionWrapper(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<Executable> const &executable,
        RC::ConstHandle<AST::Function> const &astFunction,
        GenericFunctionPtr functionPtr
        );
       
      virtual char const *getKind() const = 0;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
   
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

#endif //_FABRIC_KLC_FUNCTION_WRAPPER_H
