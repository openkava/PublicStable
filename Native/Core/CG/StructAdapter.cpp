#include "StructAdapter.h"
#include "BooleanAdapter.h"
#include "StringAdapter.h"
#include "SizeAdapter.h"
#include "OpaqueAdapter.h"
#include "Manager.h"
#include "ModuleBuilder.h"
#include "FunctionBuilder.h"
#include "BasicBlockBuilder.h"
#include "OverloadNames.h"

#include <Fabric/Core/RT/StructDesc.h>

#include <llvm/Module.h>
#include <llvm/Function.h>

namespace Fabric
{
  namespace CG
  {
    StructAdapter::StructAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::StructDesc> const &structDesc )
      : Adapter( manager, structDesc, FL_PASS_BY_REFERENCE )
      , m_structDesc( structDesc )
      , m_isShallow( structDesc->isShallow() )
    {
      size_t numMembers = structDesc->getNumMembers();
      m_memberAdapters.reserve( numMembers );
      for ( size_t i=0; i<numMembers; ++i )
      {
        RC::ConstHandle<Adapter> memberAdapter = manager->getAdapter( structDesc->getMemberInfo( i ).desc );
        m_memberAdapters.push_back( memberAdapter );
      }
      
      std::vector<llvm::Type const *> memberLLVMTypes;
      memberLLVMTypes.reserve( numMembers );
      for ( size_t i=0; i<numMembers; ++i )
        memberLLVMTypes.push_back( m_memberAdapters[i]->llvmRawType() );
      setLLVMType( llvm::StructType::get( manager->getLLVMContext(), memberLLVMTypes, true ) );
    }

    bool StructAdapter::hasMember( std::string const &memberName ) const
    {
      return m_structDesc->hasMember( memberName );
    }

    size_t StructAdapter::getMemberIndex( std::string const &memberName ) const
    {
      return m_structDesc->getMemberIndex( memberName );
    }
    
    RT::StructMemberInfo const &StructAdapter::getMemberInfo( size_t memberIndex ) const
    {
      return m_structDesc->getMemberInfo( memberIndex );
    }
    
    RC::ConstHandle<Adapter> StructAdapter::getMemberAdapter( size_t memberIndex ) const
    {
      FABRIC_ASSERT( memberIndex < m_memberAdapters.size() );
      return m_memberAdapters[memberIndex];
    }
    
    void StructAdapter::llvmInit( BasicBlockBuilder &basicBlockBuilder, llvm::Value *lValue ) const
    {
      if ( !m_structDesc->isShallow() )
      {
        for ( size_t i=0; i<getNumMembers(); ++i )
        {
          RC::ConstHandle<Adapter> const &memberAdapter = m_memberAdapters[i];
          llvm::Value *memberLValue = basicBlockBuilder->CreateConstGEP2_32( lValue, 0, i );
          memberAdapter->llvmInit( basicBlockBuilder, memberLValue );
        }
      }
    }
    
    void StructAdapter::llvmRetain( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      if ( !m_isShallow )
      {
        std::string name = "__" + getCodeName() + "__Retain";
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "rValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), name, ExprType(), params );
        basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
      }
    }
    
    void StructAdapter::llvmDefaultAssign( BasicBlockBuilder &basicBlockBuilder, llvm::Value *dstLValue, llvm::Value *srcRValue ) const
    {
      if ( !m_isShallow )
      {
        std::string name = "__" + getCodeName() + "__DefaultAssign";
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "dstLValue", this, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "srcRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), name, ExprType(), params );
        basicBlockBuilder->CreateCall2( functionBuilder.getLLVMFunction(), dstLValue, srcRValue );
      }
      else basicBlockBuilder->CreateStore( basicBlockBuilder->CreateLoad( srcRValue ), dstLValue );
    }
    
    void StructAdapter::llvmRelease( BasicBlockBuilder &basicBlockBuilder, llvm::Value *rValue ) const
    {
      if ( !m_isShallow )
      {
        std::string name = "__" + getCodeName() + "__Release";
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "rValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( basicBlockBuilder.getModuleBuilder(), name, ExprType(), params );
        basicBlockBuilder->CreateCall( functionBuilder.getLLVMFunction(), rValue );
      }
    }

    void StructAdapter::llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const
    {
      if ( moduleBuilder.contains( getCodeName() ) )
        return;
      
      moduleBuilder->addTypeName( getCodeName(), llvmRawType() );

      RC::ConstHandle<StringAdapter> stringAdapter = getManager()->getStringAdapter();
      RC::ConstHandle<SizeAdapter> sizeAdapter = getManager()->getSizeAdapter();
      RC::ConstHandle<OpaqueAdapter> dataAdapter = getManager()->getDataAdapter();
      
      if ( !m_isShallow )
      {
        {
          std::vector< FunctionParam > params;
          params.push_back( FunctionParam( "rValue", this, USAGE_RVALUE ) );
          FunctionBuilder functionBuilder( moduleBuilder, "__" + getCodeName() + "__Retain", ExprType(), params );
          if ( buildFunctions )
          {
            llvm::Value *rValue = functionBuilder[0];
            BasicBlockBuilder basicBlockBuilder( functionBuilder );
            basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
            for ( size_t i=0; i<getNumMembers(); ++i )
            {
              RC::ConstHandle<Adapter> const &memberAdapter = m_memberAdapters[i];
              llvm::Value *memberLValue = basicBlockBuilder->CreateConstGEP2_32( rValue, 0, i );
              llvm::Value *memberRValue = memberAdapter->llvmLValueToRValue( basicBlockBuilder, memberLValue );
              memberAdapter->llvmRetain( basicBlockBuilder, memberRValue );
            }
            basicBlockBuilder->CreateRetVoid();
          }
        }

        {
          std::vector< FunctionParam > params;
          params.push_back( FunctionParam( "dstLValue", this, USAGE_LVALUE ) );
          params.push_back( FunctionParam( "srcRValue", this, USAGE_RVALUE ) );
          FunctionBuilder functionBuilder( moduleBuilder, "__" + getCodeName() + "__DefaultAssign", ExprType(), params );
          if ( buildFunctions )
          {
            llvm::Value *dstLValue = functionBuilder[0];
            llvm::Value *srcRValue = functionBuilder[1];
            BasicBlockBuilder basicBlockBuilder( functionBuilder );
            basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
            for ( size_t i=0; i<getNumMembers(); ++i )
            {
              RC::ConstHandle<Adapter> const &memberAdapter = m_memberAdapters[i];
              llvm::Value *dstMemberLValue = basicBlockBuilder->CreateConstGEP2_32( dstLValue, 0, i );
              llvm::Value *srcMemberLValue = basicBlockBuilder->CreateConstGEP2_32( srcRValue, 0, i );
              llvm::Value *srcMemberRValue = memberAdapter->llvmLValueToRValue( basicBlockBuilder, srcMemberLValue );
              memberAdapter->llvmAssign( basicBlockBuilder, dstMemberLValue, srcMemberRValue );
            }
            basicBlockBuilder->CreateRetVoid();
          }
        }

        {
          std::vector< FunctionParam > params;
          params.push_back( FunctionParam( "rValue", this, USAGE_RVALUE ) );
          FunctionBuilder functionBuilder( moduleBuilder, "__" + getCodeName() + "__Release", ExprType(), params );
          if ( buildFunctions )
          {
            llvm::Value *rValue = functionBuilder[0];
            BasicBlockBuilder basicBlockBuilder( functionBuilder );
            basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
            for ( size_t i=0; i<getNumMembers(); ++i )
            {
              RC::ConstHandle<Adapter> const &memberAdapter = m_memberAdapters[i];
              llvm::Value *memberLValue = basicBlockBuilder->CreateConstGEP2_32( rValue, 0, i );
              llvm::Value *memberRValue = memberAdapter->llvmLValueToRValue( basicBlockBuilder, memberLValue );
              memberAdapter->llvmRelease( basicBlockBuilder, memberRValue );
            }
            basicBlockBuilder->CreateRetVoid();
          }
        }
      }
      
      {
        std::string name = constructOverloadName( stringAdapter, this );
        std::vector< FunctionParam > params;
        params.push_back( FunctionParam( "stringLValue", stringAdapter, USAGE_LVALUE ) );
        params.push_back( FunctionParam( "structRValue", this, USAGE_RVALUE ) );
        FunctionBuilder functionBuilder( moduleBuilder, name, ExprType(), params );
        if ( buildFunctions )
        {
          llvm::Value *stringLValue = functionBuilder[0];
          llvm::Value *structRValue = functionBuilder[1];
          BasicBlockBuilder basicBlockBuilder( functionBuilder );
          basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
          llvm::Value *structLValue = llvmRValueToLValue( basicBlockBuilder, structRValue );
          llvm::Value *stringRValue = stringAdapter->llvmCallCast( basicBlockBuilder, this, structLValue );
          stringAdapter->llvmAssign( basicBlockBuilder, stringLValue, stringRValue );
          basicBlockBuilder->CreateRetVoid();
        }
      }
      
      if ( getDesc()->isShallow() )
      {
        {
          std::string name = methodOverloadName( "dataSize", this );
          std::vector< FunctionParam > params;
          params.push_back( FunctionParam( "selfRValue", this, USAGE_RVALUE ) );
          FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( sizeAdapter, USAGE_RVALUE ), params );
          if ( buildFunctions )
          {
            llvm::Value *selfRValue = functionBuilder[0];
            BasicBlockBuilder basicBlockBuilder( functionBuilder );
            basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
            llvm::Value *dataSizeRValue = llvm::ConstantInt::get( sizeAdapter->llvmRType(), getDesc()->getSize() );
            basicBlockBuilder->CreateRet( dataSizeRValue );
          }
        }
        
        {
          std::string name = methodOverloadName( "data", this );
          std::vector< FunctionParam > params;
          params.push_back( FunctionParam( "selfLValue", this, USAGE_LVALUE ) );
          FunctionBuilder functionBuilder( moduleBuilder, name, ExprType( dataAdapter, USAGE_RVALUE ), params );
          if ( buildFunctions )
          {
            llvm::Value *selfLValue = functionBuilder[0];
            BasicBlockBuilder basicBlockBuilder( functionBuilder );
            basicBlockBuilder->SetInsertPoint( functionBuilder.createBasicBlock( "entry" ) );
            basicBlockBuilder->CreateRet( basicBlockBuilder->CreatePointerCast( selfLValue, dataAdapter->llvmRType() ) );
          }
        }
      }
    }
    
    void *StructAdapter::llvmResolveExternalFunction( std::string const &functionName ) const
    {
      return 0;
    }
   
    std::string StructAdapter::toString( void const *data ) const
    {
      size_t numMembers = m_memberAdapters.size();
      std::string result = "{";
      for ( size_t i=0; i<numMembers; ++i )
      {
        if ( i > 0 )
          result += ",";
        result += m_structDesc->getMemberInfo(i).name + ":" + m_memberAdapters[i]->toString( m_structDesc->getMemberData( data, i ) );
      }
      return result + "}";
    }
    
    llvm::Constant *StructAdapter::llvmDefaultValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      std::vector<llvm::Constant *> memberDefaultRValues;
      for ( size_t i=0; i<m_memberAdapters.size(); ++i )
      {
        RC::ConstHandle<Adapter> const &memberAdapter = m_memberAdapters[i];
        memberDefaultRValues.push_back( memberAdapter->llvmDefaultValue( basicBlockBuilder ) );
      }
      return llvm::ConstantStruct::get( getLLVMContext(), memberDefaultRValues, true );
    }
      
    llvm::Constant *StructAdapter::llvmDefaultRValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      return llvmDefaultLValue( basicBlockBuilder );
    }

    llvm::Constant *StructAdapter::llvmDefaultLValue( BasicBlockBuilder &basicBlockBuilder ) const
    {
      llvm::Constant *defaultValue = llvmDefaultValue( basicBlockBuilder );
      return new llvm::GlobalVariable(
        *basicBlockBuilder.getModuleBuilder(),
        defaultValue->getType(),
        true,
        llvm::GlobalValue::InternalLinkage,
        defaultValue,
        "__" + getCodeName() + "__DefaultValue"
        );
    }
  };
};
