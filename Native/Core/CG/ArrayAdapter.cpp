/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ArrayAdapter.h"
#include "Manager.h"
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/ConstStringAdapter.h>
#include <Fabric/Core/CG/ExprValue.h>
#include <Fabric/Core/CG/Location.h>
#include <Fabric/Core/CG/SizeAdapter.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/StringAdapter.h>
#include <Fabric/Core/RT/ArrayDesc.h>

namespace Fabric
{
  namespace CG
  {
    ArrayAdapter::ArrayAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::ArrayDesc> const &arrayDesc, Flags flags )
      : Adapter( manager, arrayDesc, flags )
      , m_arrayDesc( arrayDesc )
      , m_memberAdapter( manager->getAdapter( arrayDesc->getMemberDesc() ) )
    {
    }
    
    std::string ArrayAdapter::toString( void const *data ) const
    {
      size_t numMembers = m_arrayDesc->getNumMembers( data );
      std::string result = "[";
      for ( size_t i=0; i<numMembers; ++i )
      {
        if ( i > 0 )
          result += ",";
        result += m_memberAdapter->toString( m_arrayDesc->getMemberData( data, i ) );
      }
      return result + "]";
    }

    void ArrayAdapter::llvmThrowOutOfRangeException(
      BasicBlockBuilder &basicBlockBuilder,
      std::string const &item,
      RC::ConstHandle<ConstStringAdapter> const &constStringAdapter,
      RC::ConstHandle<StringAdapter> const &stringAdapter,
      RC::ConstHandle<SizeAdapter> const &sizeAdapter,
      llvm::Value *indexRValue,
      llvm::Value *sizeRValue,
      llvm::Value *errorDescRValue
      ) const
    {
      RC::Handle<Context> context = basicBlockBuilder.getContext();
      
      llvm::Value *errorMsg1PlusIndexPlusErrorMsg2PlusSizePlusErrorMsg3LValue;
      {
        Scope subScope( basicBlockBuilder.getScope() );
        BasicBlockBuilder subBasicBlockBuilder( basicBlockBuilder, subScope );

        llvm::Value *errorMsg0RValue = stringAdapter->llvmCast(
          subBasicBlockBuilder,
          ExprValue( constStringAdapter, USAGE_RVALUE, context,
            errorDescRValue? errorDescRValue: constStringAdapter->llvmConst( subBasicBlockBuilder, "KL" )
            )
          );
        
        std::string errorMsg1 = ": "+getUserName()+" "+item+" (";
        llvm::Value *_errorMsg1RValue = stringAdapter->llvmCast( subBasicBlockBuilder,
          ExprValue( constStringAdapter, USAGE_RVALUE, context, constStringAdapter->llvmConst( subBasicBlockBuilder, errorMsg1 ) )
          );
        
        llvm::Value *errorMsg1LValue = stringAdapter->llvmAlloca( subBasicBlockBuilder, "errorMsg1" );
        stringAdapter->llvmInit( subBasicBlockBuilder, errorMsg1LValue );
        stringAdapter->llvmCallConcat(
          subBasicBlockBuilder,
          errorMsg0RValue,
          _errorMsg1RValue,
          errorMsg1LValue
          );
        
        llvm::Value *indexStringRValue = stringAdapter->llvmCast(
          subBasicBlockBuilder,
          ExprValue( sizeAdapter, USAGE_RVALUE, context, indexRValue )
          );
          
        llvm::Value *errorMsg1PlusIndexLValue = stringAdapter->llvmAlloca( subBasicBlockBuilder, "errorMsg1PlusIndex" );
        stringAdapter->llvmInit( subBasicBlockBuilder, errorMsg1PlusIndexLValue );
        stringAdapter->llvmCallConcat(
          subBasicBlockBuilder,
          stringAdapter->llvmLValueToRValue( subBasicBlockBuilder, errorMsg1LValue ),
          indexStringRValue,
          errorMsg1PlusIndexLValue
          );
        stringAdapter->llvmDispose( subBasicBlockBuilder, errorMsg1LValue );
        
        std::string errorMsg2 = ") out of range (";
        llvm::Value *errorMsg2RValue = stringAdapter->llvmCast(
          subBasicBlockBuilder,
          ExprValue( constStringAdapter, USAGE_RVALUE, context, constStringAdapter->llvmConst( subBasicBlockBuilder, errorMsg2 ) )
          );
          
        llvm::Value *errorMsg1PlusIndexPlusErrorMsg2LValue = stringAdapter->llvmAlloca( subBasicBlockBuilder, "errorMsg1PlusIndexPlusErrorMsg2" );
        stringAdapter->llvmInit( subBasicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2LValue );
        stringAdapter->llvmCallConcat(
          subBasicBlockBuilder,
          stringAdapter->llvmLValueToRValue( subBasicBlockBuilder, errorMsg1PlusIndexLValue ),
          errorMsg2RValue,
          errorMsg1PlusIndexPlusErrorMsg2LValue
          );
        stringAdapter->llvmDispose( subBasicBlockBuilder, errorMsg1PlusIndexLValue );

        llvm::Value *sizeStringRValue = stringAdapter->llvmCast(
          subBasicBlockBuilder,
          ExprValue( sizeAdapter, USAGE_RVALUE, context, sizeRValue )
          );
          
        llvm::Value *errorMsg1PlusIndexPlusErrorMsg2PlusSizeLValue = stringAdapter->llvmAlloca( subBasicBlockBuilder, "errorMsg1PlusIndexPlusErrorMsg2PlusSize" );
        stringAdapter->llvmInit( subBasicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2PlusSizeLValue );
        stringAdapter->llvmCallConcat(
          subBasicBlockBuilder,
          stringAdapter->llvmLValueToRValue( subBasicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2LValue ),
          sizeStringRValue,
          errorMsg1PlusIndexPlusErrorMsg2PlusSizeLValue
          );
        stringAdapter->llvmDispose( subBasicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2LValue );

        std::string errorMsg3 = ")";
        llvm::Value *errorMsg3RValue = stringAdapter->llvmCast(
          subBasicBlockBuilder,
          ExprValue( constStringAdapter, USAGE_RVALUE, context, constStringAdapter->llvmConst( subBasicBlockBuilder, errorMsg3 ) )
          );
          
        errorMsg1PlusIndexPlusErrorMsg2PlusSizePlusErrorMsg3LValue = stringAdapter->llvmAlloca( subBasicBlockBuilder, "errorMsg1PlusIndexPlusErrorMsg2PlusSizePlusErrorMsg3" );
        stringAdapter->llvmInit( subBasicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2PlusSizePlusErrorMsg3LValue );
        stringAdapter->llvmCallConcat(
          subBasicBlockBuilder,
          stringAdapter->llvmLValueToRValue( subBasicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2PlusSizeLValue ),
          errorMsg3RValue,
          errorMsg1PlusIndexPlusErrorMsg2PlusSizePlusErrorMsg3LValue
          );
        stringAdapter->llvmDispose( subBasicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2PlusSizeLValue );
        
        subScope.llvmUnwind( subBasicBlockBuilder );
      }
      
      llvmThrowException( basicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2PlusSizePlusErrorMsg3LValue );
    }

    llvm::Value *ArrayAdapter::llvmLocationConstStringRValue(
      BasicBlockBuilder &basicBlockBuilder,
      RC::ConstHandle<ConstStringAdapter> const &constStringAdapter,
      CG::Location const *location
      ) const
    {
      if ( location )
      {
        RC::ConstHandle<RC::String> filename = location->getFilename();
        std::string locationString = (filename? filename->stdString(): std::string("(unknown)")) + ":" + _(location->getLine()) + ":" + _(location->getColumn());
        return constStringAdapter->llvmConst( basicBlockBuilder, locationString );
      }
      else return constStringAdapter->llvmConst( basicBlockBuilder, "KL" );
    }
  };
};
