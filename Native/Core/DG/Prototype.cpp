/*
 *  Prototype.cpp
 *  Fabric
 *
 *  Created by Peter Zion on 10-10-08.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#include "Prototype.h"
#include "Node.h"
#include "Scope.h"
#include "Debug.h"

#include <Fabric/Core/DG/SharedSlicedArray.h>
#include <Fabric/Core/DG/Function.h>
#include <Fabric/Core/AST/Operator.h>
#include <Fabric/Core/AST/ParamVector.h>
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/RT/IntegerDesc.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/RT/VariableArrayDesc.h>
#include <Fabric/Core/RT/VariableArrayImpl.h>
#include <Fabric/Core/RT/SlicedArrayImpl.h>
#include <Fabric/Core/RT/SlicedArrayDesc.h>
#include <Fabric/Core/MT/Util.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Array.h>

namespace Fabric
{
  namespace DG
  {
    class Prototype::Param
    {
    public:
      
      virtual ~Param()
      {
      }
      
      unsigned index() const
      {
        return m_index;
      }
      
      virtual bool isSizeParam() const { return false; }
      virtual bool isNewSizeParam() const { return false; }
      virtual bool isIndexParam() const { return false; }
      virtual bool isMemberParam() const { return false; }
      virtual bool isElementParam() const { return false; }
      virtual bool isArrayParam() const { return false; }
    
      virtual std::string desc() const = 0;
      
    protected:
    
      Param( unsigned index )
        : m_index( index )
      {
      }
      
    private:
    
      unsigned m_index;
    };
    
    class Prototype::SizeParam : public Prototype::Param
    {
    public:
    
      SizeParam( unsigned index )
        : Param( index )
      {
      }
    
      virtual bool isSizeParam() const { return true; }

      virtual std::string desc() const
      {
        return "count";
      }
    };
    
    class Prototype::NewSizeParam : public Prototype::Param
    {
    public:
    
      NewSizeParam( unsigned index )
        : Param( index )
      {
      }
    
      virtual bool isNewSizeParam() const { return true; }

      virtual std::string desc() const
      {
        return "newCount";
      }
    };
    
    class Prototype::IndexParam : public Prototype::Param
    {
    public:
    
      IndexParam( unsigned index )
        : Param( index )
      {
      }
    
      virtual bool isIndexParam() const { return true; }

      virtual std::string desc() const
      {
        return "index";
      }
    };
    
    class Prototype::MemberParam : public Prototype::Param
    {
    public:
    
      MemberParam( unsigned index, std::string const &name )
        : Param( index )
        , m_name( name )
      {
      }
      
      std::string const &name() const
      {
        return m_name;
      }

      virtual bool isMemberParam() const { return true; }
      
    private:
    
      std::string m_name;
    };
    
    class Prototype::ElementParam : public Prototype::MemberParam
    {
    public:
    
      ElementParam( unsigned index, std::string const &name )
        : MemberParam( index, name )
      {
      }
    
      virtual bool isElementParam() const { return true; }

      virtual std::string desc() const
      {
        return name();
      }
    };
    
    class Prototype::ArrayParam : public Prototype::MemberParam
    {
    public:
    
      ArrayParam( unsigned index, std::string const &name )
        : MemberParam( index, name )
      {
      }
    
      virtual bool isArrayParam() const { return true; }

      virtual std::string desc() const
      {
        return name() + "<>";
      }
    };
    
    Prototype::Prototype( RC::Handle<CG::Manager> const &cgManager )
      : m_cgManager( cgManager )
      , m_rtSizeDesc( cgManager->getRTManager()->getSizeDesc() )
      , m_rtSizeImpl( m_rtSizeDesc->getImpl() )
      , m_rtIndexDesc( cgManager->getRTManager()->getIndexDesc() )
      , m_rtIndexImpl( m_rtIndexDesc->getImpl() )
    {
    }
    
    void Prototype::setDescs( std::vector<std::string> const &descs )
    {
      clear();
      
      m_paramCount = descs.size();
      
      for ( size_t i=0; i<m_paramCount; ++i )
      {
        std::string const &desc = descs[i];
          
        try
        {
          std::string::size_type nodeNameStart = 0;
          std::string::size_type nodeNameEnd = desc.find( '.' );
          if ( nodeNameEnd == std::string::npos || nodeNameEnd - nodeNameStart < 1 )
            throw Exception( "missing node name" );
          std::string nodeName = desc.substr( nodeNameStart, nodeNameEnd - nodeNameStart );
          
          std::string::size_type memberNameStart = nodeNameEnd + 1;
          std::string::size_type memberNameEnd = desc.find( '<' );
          if ( memberNameEnd == std::string::npos )
            memberNameEnd = desc.size();
          if ( memberNameEnd - memberNameStart < 1 )
            throw Exception( "missing member name" );
          std::string memberName = desc.substr( memberNameStart, memberNameEnd - memberNameStart );
          
          Param *param;
          if ( memberName == "count" )
            param = new SizeParam(i);
          else if ( memberName == "newCount" )
            param = new NewSizeParam(i);
          else if ( memberName == "index" )
            param = new IndexParam(i);
          else if ( desc.substr( memberNameEnd ) == "<>" )
            param = new ArrayParam( i, memberName );
          else
            param = new ElementParam( i, memberName );
          m_params[nodeName].insert( std::multimap< std::string, Param * >::value_type( memberName, param ) );
        }
        catch ( Exception e )
        {
          char buf[32];
          sprintf( buf, "%u", (unsigned)i );
          throw "parameter " + std::string(buf) + ": " + e;
        }
      }
    }
    
    void Prototype::clear()
    {
      while ( !m_params.empty() )
      {
        std::map< std::string, std::multimap< std::string, Param * > >::iterator it=m_params.begin();
        for ( std::multimap< std::string, Param * >::const_iterator jt=it->second.begin(); jt!=it->second.end(); ++jt )
          delete jt->second;
        m_params.erase( it );
        --m_paramCount;
      }
    }
    
    Prototype::~Prototype()
    {
      clear();
    }

    RC::Handle<MT::ParallelCall> Prototype::bind(
      std::vector<std::string> &errors,
      RC::ConstHandle<AST::Operator> const &astOperator,
      Scope const &scope,
      RC::ConstHandle<Function> const &function,
      size_t *newSize,
      unsigned prefixCount,
      void * const *prefixes
      )
    {
      FABRIC_ASSERT( function );
      
      RC::ConstHandle<AST::ParamVector> astParamList = astOperator->getParams( m_cgManager );
      size_t numASTParams = astParamList->size();
      size_t expectedNumASTParams = prefixCount + m_paramCount;
      if ( numASTParams != expectedNumASTParams )
        errors.push_back( "operator takes incorrect number of parameters (expected "+_(expectedNumASTParams)+", actual "+_(numASTParams)+")" );

      RC::Handle<MT::ParallelCall> result = MT::ParallelCall::Create( function, prefixCount+m_paramCount, astOperator->getDeclaredName() );
      for ( unsigned i=0; i<prefixCount; ++i )
        result->setBaseAddress( i, prefixes[i] );
      for ( std::map< std::string, std::multimap< std::string, Param * > >::const_iterator it=m_params.begin(); it!=m_params.end(); ++it )
      {
        std::string const &nodeName = it->first;
        std::string const nodeErrorPrefix = "node " + _(nodeName) + ": ";
        {
          RC::Handle<Container> container = scope.find( nodeName );
          if ( !container )
          {
            errors.push_back( nodeErrorPrefix + "not found" );
            continue;
          }
          
          bool haveAdjustmentIndex = false;
          unsigned adjustmentIndex = 0;
          
          std::set<void *> elementAccessSet;
          std::set<void *> arrayAccessSet;
          
          for ( std::multimap< std::string, Param * >::const_iterator jt=it->second.begin(); jt!=it->second.end(); ++jt )
          {
            Param const *param = jt->second;

            RC::ConstHandle<AST::Param> astParam = astParamList->get( prefixCount + param->index() );
            CG::ExprType astParamExprType = astParam->getExprType( m_cgManager );
            RC::ConstHandle<RT::Desc> astParamDesc = astParamExprType.getDesc();
            RC::ConstHandle<RT::Impl> astParamImpl = astParamDesc->getImpl();
            
            std::string const parameterErrorPrefix = nodeErrorPrefix + "parameter " + _(size_t(prefixCount+param->index()+1)) + ": ";
            {
              if ( param->isSizeParam() )
              {
                if ( astParamImpl != m_rtSizeImpl
                  || astParamExprType.getUsage() != CG::USAGE_RVALUE )
                  errors.push_back( parameterErrorPrefix + "'size' parmeters must bind to operator in parameters of type "+_(m_rtSizeDesc->getName()) );
                result->setBaseAddress( prefixCount+param->index(), (void *)container->getCount() );
              }
              else if ( param->isNewSizeParam() )
              {
                if ( astParamImpl != m_rtSizeImpl
                  || astParamExprType.getUsage() != CG::USAGE_LVALUE )
                  errors.push_back( parameterErrorPrefix + "'newSize' parmeters must bind to operator io parameters of type "+_(m_rtSizeDesc->getName()) );
                if ( !newSize )
                  errors.push_back( parameterErrorPrefix + "can't access count" );
                result->setBaseAddress( prefixCount+param->index(), newSize );
              }
              else if ( param->isIndexParam() )
              {
                if ( astParamImpl != m_rtIndexImpl
                  || astParamExprType.getUsage() != CG::USAGE_RVALUE )
                  errors.push_back( parameterErrorPrefix + "'index' parmeters must bind to operator in parameters of type "+_(m_rtIndexDesc->getName()) );
                else
                {
                  result->setBaseAddress( prefixCount+param->index(), (void *)0 );
                  if ( container->getCount() != 1 )
                  {
                    if ( !haveAdjustmentIndex )
                    {
                      adjustmentIndex = result->addAdjustment( container->getCount(), std::max<size_t>( 1, container->getCount()/MT::getNumCores() ) );
                      haveAdjustmentIndex = true;
                    }
                    result->setAdjustmentOffset( adjustmentIndex, prefixCount+param->index(), 1 );
                  }
                }
              }
              else if ( param->isMemberParam() )
              {
                MemberParam const *memberParam = static_cast<MemberParam const *>(param);
                std::string const &memberName = memberParam->name();
                std::string const memberErrorPrefix = parameterErrorPrefix + "member '" + memberName + "': ";
                {
                  RC::ConstHandle<RT::Desc> memberDesc;
                  RC::ConstHandle<RT::VariableArrayDesc> memberArrayDesc;
                  RC::ConstHandle<RT::SlicedArrayDesc> memberSlicedArrayDesc;
                  RC::Handle<SharedSlicedArray> sharedSlicedArray;
                  try
                  {
                    container->getMemberDescs( memberName, memberDesc, memberArrayDesc, memberSlicedArrayDesc, sharedSlicedArray );
                  }
                  catch ( Exception e )
                  {
                    errors.push_back( memberErrorPrefix + std::string(e) );
                  }
                  if ( memberDesc && memberArrayDesc && memberSlicedArrayDesc )
                  {
                    RC::ConstHandle<RT::VariableArrayImpl> memberArrayImpl = memberArrayDesc->getImpl();
                    void *memberArrayData = container->getMemberArrayData( memberName );
                    if ( param->isElementParam() )
                    {
                      if ( arrayAccessSet.find( memberArrayData ) != arrayAccessSet.end() )
                        errors.push_back( memberErrorPrefix + "cannot access both per-slice and whole array data for the same member" );
                      else
                      {
                        elementAccessSet.insert( memberArrayData );
                        
                        RC::ConstHandle<RT::Impl> memberImpl = memberDesc->getImpl();
                        if ( astParamImpl != memberImpl )
                          errors.push_back( memberErrorPrefix + "parameter type mismatch: member element type is "+_(memberDesc->getName())+", operator parameter type is "+_(astParamDesc->getName()) );
                        if ( astParamExprType.getUsage() != CG::USAGE_LVALUE )
                          errors.push_back( memberErrorPrefix + "element parmeters must bind to operator io parameters" );
                        void *baseAddress;
                        if ( memberArrayImpl->getNumMembers( memberArrayData ) > 0 )
                          baseAddress = memberArrayImpl->getMemberData( memberArrayData, 0 );
                        else baseAddress = 0;
                        result->setBaseAddress( prefixCount+param->index(), baseAddress );
                        if ( container->getCount() != 1 )
                        {
                          if ( !haveAdjustmentIndex )
                          {
                            adjustmentIndex = result->addAdjustment( container->getCount(), std::max<size_t>( 1, container->getCount()/MT::getNumCores() ) );
                            haveAdjustmentIndex = true;
                          }
                          result->setAdjustmentOffset( adjustmentIndex, prefixCount+param->index(), memberImpl->getAllocSize() );
                        }
                      }
                    }
                    else
                    {
                      if ( elementAccessSet.find( memberArrayData ) != elementAccessSet.end() )
                        errors.push_back( memberErrorPrefix + "cannot access both per-slice and whole array data for the same member" );
                      else arrayAccessSet.insert( memberArrayData );
                      
                      RC::ConstHandle<RT::SlicedArrayImpl> slicedArrayImpl = memberSlicedArrayDesc->getImpl();
                      if ( astParamImpl != slicedArrayImpl )
                        errors.push_back( memberErrorPrefix + "parameter type mismatch: member array type is "+_(memberSlicedArrayDesc->getName())+", operator parameter type is "+_(astParamDesc->getName()) );
                      //if ( astParamExprType.getUsage() != CG::USAGE_LVALUE )
                      //  throw Exception( "array parmeters must bind to operator io parameters" );
                      
                      result->setBaseAddress( prefixCount+param->index(), sharedSlicedArray->getData() );
                    }
                  }
                }
              }
            }
          }
        }
      }
      return result;
    }
    
    std::vector<std::string> Prototype::desc() const
    {
      std::vector<std::string> result;
      result.resize( m_paramCount );
      for ( std::map< std::string, std::multimap< std::string, Param * > >::const_iterator it=m_params.begin(); it!=m_params.end(); ++it )
      {
        std::string const &nodeName = it->first;
        for ( std::multimap< std::string, Param * >::const_iterator jt=it->second.begin(); jt!=it->second.end(); ++jt )
        {
          Param const *param = jt->second;
          result[param->index()] = nodeName + "." + param->desc();
        }
      }
      return result;
    }
      
    RC::ConstHandle<JSON::Value> Prototype::jsonDesc() const
    {
      std::vector<std::string> items = desc();
      
      RC::Handle<JSON::Array> result = JSON::Array::Create();
      for ( size_t i=0; i<items.size(); ++i )
        result->push_back( JSON::String::Create( items[i] ) );
      return result;
    }
  };
};
