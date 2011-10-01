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

      llvm::Value *errorMsg0RValue = stringAdapter->llvmCast(
        basicBlockBuilder,
        ExprValue( constStringAdapter, USAGE_RVALUE, context,
          errorDescRValue? errorDescRValue: constStringAdapter->llvmConst( basicBlockBuilder, "KL" )
          )
        );
      
      std::string errorMsg1 = ": "+getUserName()+" "+item+" (";
      llvm::Value *_errorMsg1RValue = stringAdapter->llvmCast( basicBlockBuilder,
        ExprValue( constStringAdapter, USAGE_RVALUE, context, constStringAdapter->llvmConst( basicBlockBuilder, errorMsg1 ) )
        );
      
      llvm::Value *errorMsg1RValue = stringAdapter->llvmCallConcat(
        basicBlockBuilder,
        errorMsg0RValue,
        _errorMsg1RValue
        );
      stringAdapter->llvmRelease( basicBlockBuilder, errorMsg0RValue );
      stringAdapter->llvmRelease( basicBlockBuilder, _errorMsg1RValue );
      
      llvm::Value *indexStringRValue = stringAdapter->llvmCast( basicBlockBuilder,
        ExprValue( sizeAdapter, USAGE_RVALUE, context, indexRValue )
        );
        
      llvm::Value *errorMsg1PlusIndexRValue = stringAdapter->llvmCallConcat( basicBlockBuilder, errorMsg1RValue, indexStringRValue );
      stringAdapter->llvmRelease( basicBlockBuilder, errorMsg1RValue );
      stringAdapter->llvmRelease( basicBlockBuilder, indexStringRValue );
      
      std::string errorMsg2 = ") out of range (";
      llvm::Value *errorMsg2RValue = stringAdapter->llvmCast( basicBlockBuilder,
        ExprValue( constStringAdapter, USAGE_RVALUE, context, constStringAdapter->llvmConst( basicBlockBuilder, errorMsg2 ) )
        );
      llvm::Value *errorMsg1PlusIndexPlusErrorMsg2RValue = stringAdapter->llvmCallConcat( basicBlockBuilder, errorMsg1PlusIndexRValue, errorMsg2RValue );
      stringAdapter->llvmRelease( basicBlockBuilder, errorMsg1PlusIndexRValue );
      stringAdapter->llvmRelease( basicBlockBuilder, errorMsg2RValue );
      llvm::Value *sizeStringRValue = stringAdapter->llvmCast( basicBlockBuilder,
        ExprValue( sizeAdapter, USAGE_RVALUE, context, sizeRValue )
        );
      llvm::Value *errorMsg1PlusIndexPlusErrorMsg2PlusSizeRValue = stringAdapter->llvmCallConcat( basicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2RValue, sizeStringRValue );
      stringAdapter->llvmRelease( basicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2RValue );
      stringAdapter->llvmRelease( basicBlockBuilder, sizeStringRValue );
      std::string errorMsg3 = ")";
      llvm::Value *errorMsg3RValue = stringAdapter->llvmCast( basicBlockBuilder,
        ExprValue( constStringAdapter, USAGE_RVALUE, context, constStringAdapter->llvmConst( basicBlockBuilder, errorMsg3 ) )
        );
      llvm::Value *errorMsg1PlusIndexPlusErrorMsg2PlusSizePlusErrorMsg3RValue = stringAdapter->llvmCallConcat( basicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2PlusSizeRValue, errorMsg3RValue );
      stringAdapter->llvmRelease( basicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2PlusSizeRValue );
      stringAdapter->llvmRelease( basicBlockBuilder, errorMsg3RValue );
      llvmThrowException( basicBlockBuilder, errorMsg1PlusIndexPlusErrorMsg2PlusSizePlusErrorMsg3RValue );
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
