/*
 *
 *  Created by Peter Zion on 10-10-08.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_DG_PROTOTYPE_H
#define _FABRIC_DG_PROTOTYPE_H

#include <Fabric/Core/MT/ParallelCall.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <map>
#include <set>

namespace Fabric
{
  namespace JSON
  {
    class Value;
  };
  
  namespace RT
  {
    class Manager;
    class Desc;
    class Impl;
  };
  
  namespace AST
  {
    class Operator;
  };
  
  namespace DG
  {
    class Scope;
    class Function;
    
    class Prototype
    {
    public:
    
      Prototype( RC::ConstHandle<RT::Manager> const &rtManager );
      virtual ~Prototype();
      
      void setDescs( std::vector<std::string> const &descs );
      void clear();
    
      MT::ParallelCall *bind( RC::ConstHandle<AST::Operator> const &astOperator, Scope const &scope, RC::ConstHandle<Function> const &function, size_t *newSize, unsigned prefixCount=0, void * const *prefixes = 0 );
      
      std::vector<std::string> desc() const;

      RC::ConstHandle<JSON::Value> jsonDesc() const;
    
    private:
    
      class Param;
      class SizeParam;
      class NewSizeParam;
      class IndexParam;
      class MemberParam;
      class ElementParam;
      class ArrayParam;
    
      size_t m_paramCount;
      std::map< std::string, std::multimap< std::string, Param * > > m_params;
      
      RC::ConstHandle<RT::Desc> m_rtSizeDesc;
      RC::ConstHandle<RT::Impl> m_rtSizeImpl;
    };
  };
};

#endif //_FABRIC_DG_PROTOTYPE_H
