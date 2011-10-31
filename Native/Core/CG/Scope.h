/*
 *
 *  Created by Peter Zion on 10-12-04.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_CG_SCOPE_H
#define _FABRIC_CG_SCOPE_H

#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/ExprValue.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Location.h>
#include <Fabric/Core/Util/UnorderedMap.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace CG
  {
    class Symbol : public RC::Object
    {
    public:
    
      virtual bool isValue() const { return false; }
      virtual bool isFunction() const { return false; }
      
      virtual std::string desc() const = 0;
    };
    
    class ValueSymbol : public Symbol
    {
    public:
    
      virtual bool isValue() const { return true; }
      virtual bool isVariable() const { return false; }
      virtual bool isParameter() const { return false; }
      virtual bool isConstant() const { return false; }
      
      CG::ExprValue const &getExprValue() const
      {
        return m_exprValue;
      }
      
      RC::ConstHandle<CG::Adapter> getAdapter() const
      {
        return m_exprValue.getAdapter();
      }
      
    protected:
    
      ValueSymbol( CG::ExprValue const &exprValue )
        : m_exprValue( exprValue )
      {
      }
      
    private:
    
      CG::ExprValue m_exprValue;
    };
    
    class VariableSymbol : public ValueSymbol
    {
    public:
    
      static RC::ConstHandle<VariableSymbol> Create( CG::ExprValue const &exprValue )
      {
        return new VariableSymbol( exprValue );
      }
      
      virtual bool isVariable() const { return true; }
      
      virtual std::string desc() const
      {
        return "Variable(" + getExprValue().desc() + ")";
      }
      
    protected:
    
      VariableSymbol( CG::ExprValue const &exprValue )
        : ValueSymbol( exprValue )
      {
      }
    };
    
    class ParameterSymbol : public ValueSymbol
    {
    public:
    
      static RC::ConstHandle<ParameterSymbol> Create( CG::ExprValue const &exprValue )
      {
        return new ParameterSymbol( exprValue );
      }
      
      virtual bool isParameter() const { return true; }
      
      virtual std::string desc() const
      {
        return "Parameter(" + getExprValue().desc() + ")";
      }
      
    protected:
    
      ParameterSymbol( CG::ExprValue const &exprValue )
        : ValueSymbol( exprValue )
      {
      }
    };
    
    class ConstantSymbol : public ValueSymbol
    {
    public:
    
      static RC::ConstHandle<ConstantSymbol> Create( CG::ExprValue const &exprValue )
      {
        return new ConstantSymbol( exprValue );
      }
      
      virtual bool isConstant() const { return true; }
      
      virtual std::string desc() const
      {
        return "Constant(" + getExprValue().desc() + ")";
      }
      
    protected:
    
      ConstantSymbol( CG::ExprValue const &exprValue )
        : ValueSymbol( exprValue )
      {
      }
    };
    
    class FunctionParam
    {
    public:
    
      FunctionParam( std::string const &name, ExprType const &exprType )
        : m_name( name )
        , m_exprType( exprType )
      {
      }
    
      FunctionParam( std::string const &name, RC::ConstHandle<Adapter> const &adapter, Usage usage )
        : m_name( name )
        , m_exprType( adapter, usage )
      {
      }
      
      FunctionParam( FunctionParam const &that )
        : m_name( that.m_name )
        , m_exprType( that.m_exprType )
      {
      }
      
      FunctionParam &operator =( FunctionParam const &that )
      {
        m_name = that.m_name;
        m_exprType = that.m_exprType;
        return *this;
      }
    
      std::string const &getName() const
      {
        return m_name;
      }
      
      ExprType const &getExprType() const
      {
        return m_exprType;
      }
      
      std::string const &getTypeUserName() const
      {
        return getExprType().getUserName();
      }
      
      std::string const &getTypeCodeName() const
      {
        return getExprType().getCodeName();
      }
      
      RC::ConstHandle<Adapter> getAdapter() const
      {
        return getExprType().getAdapter();
      }
      
      Usage getUsage() const
      {
        return getExprType().getUsage();
      }
      
      llvm::Type const *getLLVMType( RC::Handle<Context> const &context ) const
      {
        switch ( getUsage() )
        {
          case USAGE_RVALUE:
            return getAdapter()->llvmRType( context );
          case USAGE_LVALUE:
            return getAdapter()->llvmLType( context );
          case USAGE_UNSPECIFIED:
            FABRIC_ASSERT( false );
            throw Exception( "usage unspecified" );
        }
        return 0;
      }
      
      std::string desc() const
      {
        return m_name + ":" + m_exprType.desc();
      }

    private:
    
      std::string m_name;
      ExprType m_exprType;
    };

    class ReturnInfo
    {
    public:
      ReturnInfo( ExprType const &exprType, llvm::Value *returnLValue = NULL )
        : m_exprType( exprType )
        , m_returnsStaticDataPtr( false )
        , m_returnLValue( returnLValue )
      {
      }

      ReturnInfo( ExprType const &exprType, bool returnsStaticDataPtr = false )
        : m_exprType( exprType )
        , m_returnsStaticDataPtr( returnsStaticDataPtr )
        , m_returnLValue( NULL )
      {
      }

      ReturnInfo( ReturnInfo const &that )
        : m_exprType( that.m_exprType )
        , m_returnsStaticDataPtr( that.m_returnsStaticDataPtr )
        , m_returnLValue( that.m_returnLValue )
      {
      }

      ReturnInfo &operator =( ReturnInfo const &that )
      {
        m_exprType = that.m_exprType;
        m_returnsStaticDataPtr = that.m_returnsStaticDataPtr;
        m_returnLValue = that.m_returnLValue;
        return( *this );
      }

      operator bool() const
      {
        return m_exprType.isValid();
      }
      
      bool operator !() const
      {
        return !m_exprType.isValid();
      }

      ExprType const &getExprType() const
      {
        return m_exprType;
      }

      RC::ConstHandle<Adapter> getAdapter() const
      {
        return m_exprType.getAdapter();
      }

      CG::Usage getUsage() const
      {
        return m_exprType.getUsage();
      }

      std::string desc() const
      {
        return m_exprType.desc();
      }

      bool usesReturnLValue() const
      {
        if ( m_exprType.isValid() && !m_returnsStaticDataPtr )
          return m_exprType.getAdapter()->usesReturnLValue();
        else return false;
      }

      llvm::Value *getReturnLValue( ) const
      {
        return( m_returnLValue );
      }

    private:

      ExprType  m_exprType;
      bool        m_returnsStaticDataPtr;
      llvm::Value *m_returnLValue;
    };

    class FunctionSymbol : public Symbol
    {
    public:
      
      static RC::ConstHandle<FunctionSymbol> Create( llvm::Function *llvmFunction, ReturnInfo const &returnInfo, std::vector< FunctionParam > const &params )
      {
        return new FunctionSymbol( llvmFunction, returnInfo, params );
      }
      
      virtual bool isFunction() const { return true; }
      
      llvm::Function *getLLVMFunction() const
      {
        return m_llvmFunction;
      }
      
      ReturnInfo const &getReturnInfo() const
      {
        return m_returnInfo;
      }
      
      std::vector< FunctionParam > const &getParams() const
      {
        return m_params;
      }

      virtual std::string desc() const
      {
        std::string result = "FunctionSymbol( " + m_returnInfo.desc();
        for ( size_t i=0; i<m_params.size(); ++i )
        {
          if ( i == 0 )
            result += "; ";
          else
            result += ", ";
          result += m_params[i].getName();
        }
        result += " )";
        return result;
      }
      
      ExprValue llvmCreateCall( BasicBlockBuilder &basicBlockBuilder, std::vector< ExprValue > &args ) const;
      
      ExprValue llvmCreateCall( BasicBlockBuilder &basicBlockBuilder, ExprValue const &arg ) const
      {
        std::vector< ExprValue > args;
        args.push_back( arg );
        return llvmCreateCall( basicBlockBuilder, args );
      }
      
      ExprValue llvmCreateCall( BasicBlockBuilder &basicBlockBuilder, ExprValue const &arg1, ExprValue const &arg2 ) const
      {
        std::vector< ExprValue > args;
        args.push_back( arg1 );
        args.push_back( arg2 );
        return llvmCreateCall( basicBlockBuilder, args );
      }
      
    protected:
      
      FunctionSymbol( llvm::Function *llvmFunction, ReturnInfo const &returnInfo, std::vector< FunctionParam > const &params )
        : m_llvmFunction( llvmFunction )
        , m_returnInfo( returnInfo )
        , m_params( params )
      {
      }
            
    private:
    
      llvm::Function *m_llvmFunction;
      ReturnInfo m_returnInfo;
      std::vector< FunctionParam > m_params;
    };
    
    class Scope
    {
    public:
    
      Scope( Scope const &parentScope )
        : m_parentScope( &parentScope )
      {
      }

      void put( RC::ConstHandle<Symbol> const &symbol )
      {
        m_symbols.push_back( symbol );
      }
      
      void put( std::string const &name, RC::ConstHandle<Symbol> const &symbol )
      {
        put( symbol );
        m_namedSymbols.insert( StringToSymbolMap::value_type( name, symbol ) );
      }
      
      bool has( std::string const &name ) const
      {
        StringToSymbolMap::const_iterator it = m_namedSymbols.find(name);
        return it != m_namedSymbols.end() || (m_parentScope && m_parentScope->has( name ));
      }
      
      RC::ConstHandle<Symbol> get( std::string const &name ) const
      {
        StringToSymbolMap::const_iterator it = m_namedSymbols.find(name);
        if ( it != m_namedSymbols.end() )
          return it->second;
        else if ( m_parentScope )
          return m_parentScope->get( name );
        else return 0;
      }
      
      std::string desc( std::string const &indent = "" ) const
      {
        std::string result = "";
        for ( StringToSymbolMap::const_iterator it=m_namedSymbols.begin(); it!=m_namedSymbols.end(); ++it )
          result += it->first + ": " + it->second->desc() + "\n";
        if ( m_parentScope )
          result += m_parentScope->desc( indent + "  " );
        return result;
      }
      
      void llvmUnwind( BasicBlockBuilder &bbb ) const
      {
        for ( SymbolVector::const_iterator it=m_symbols.begin(); it!=m_symbols.end(); ++it )
        {
          RC::ConstHandle<Symbol> const &symbol = *it;
          if ( symbol->isValue() )
          {
            RC::ConstHandle<ValueSymbol> valueSymbol = RC::ConstHandle<ValueSymbol>::StaticCast( symbol );
            if ( valueSymbol->isVariable() )
            {
              RC::ConstHandle<VariableSymbol> variableSymbol = RC::ConstHandle<VariableSymbol>::StaticCast( valueSymbol );
              ExprValue exprValue = variableSymbol->getExprValue();
              llvm::Value *rValue = 0, *lValue = 0;
              RC::ConstHandle<Adapter> adapter = exprValue.getAdapter();
              switch ( exprValue.getUsage() )
              {
                case USAGE_RVALUE:
                  rValue = exprValue.getValue();
                  lValue = adapter->llvmRValueToLValue( bbb, rValue );
                  break;
                case USAGE_LVALUE:
                  lValue = exprValue.getValue();
                  rValue = adapter->llvmLValueToRValue( bbb, lValue );
                  break;
                default:
                  FABRIC_ASSERT( false );
                  break;
              }
              exprValue.getAdapter()->llvmDispose( bbb, lValue );
            }
          }
        }
      }
            
      virtual void llvmBreak( BasicBlockBuilder &bbb, Location const &location ) const
      {
        llvmUnwind( bbb );
        if ( !m_parentScope )
          throw Error( location, "break outside of loop or switch statment" );
        m_parentScope->llvmBreak( bbb, location );
      }

      virtual void llvmContinue( BasicBlockBuilder &bbb, Location const &location ) const
      {
        llvmUnwind( bbb );
        if ( !m_parentScope )
          throw Error( location, "continue outside of loop" );
        m_parentScope->llvmContinue( bbb, location );
      }

      virtual ReturnInfo const &getReturnInfo() const
      {
        FABRIC_ASSERT( m_parentScope );
        return m_parentScope->getReturnInfo();
      }
            
      void llvmReturn( BasicBlockBuilder &bbb, ExprValue &exprValue ) const
      {
        ExprType const &returnExprType = getReturnInfo().getExprType();
        llvm::Value *returnValue = 0;
        switch ( returnExprType.getUsage() )
        {
          case USAGE_RVALUE:
            returnValue = returnExprType.getAdapter()->llvmCast( bbb, exprValue );
            break;
          case USAGE_LVALUE:
            if ( exprValue.getExprType() != returnExprType )
              throw Exception( "cannot return l-value through casting" );
            returnValue = exprValue.getValue();
            break;
          case USAGE_UNSPECIFIED:
            FABRIC_ASSERT( false );
            throw Exception( "unspecified usage" );
        }
        llvmReturn( bbb, returnValue );
      }
      
      virtual void llvmReturn( BasicBlockBuilder &bbb, llvm::Value *value ) const
      {
        llvmUnwind( bbb );
        FABRIC_ASSERT( m_parentScope );
        m_parentScope->llvmReturn( bbb, value );
      }
      
    protected:
    
      Scope()
        : m_parentScope( 0 )
      {
      }
      
    private:
    
      typedef std::vector< RC::ConstHandle<Symbol> > SymbolVector;
      typedef Util::UnorderedMap< std::string, RC::ConstHandle<Symbol> > StringToSymbolMap;
    
      SymbolVector m_symbols;
      StringToSymbolMap m_namedSymbols;
      Scope const *m_parentScope;
    };
    
    class LoopScope : public Scope
    {
    public:
    
      LoopScope( Scope const &parentScope, llvm::BasicBlock *doneBB, llvm::BasicBlock *nextBB )
        : Scope( parentScope )
        , m_doneBB( doneBB )
        , m_nextBB( nextBB )
      {
      }
      
      virtual void llvmBreak( BasicBlockBuilder &bbb, Location const &location ) const
      {
        bbb->CreateBr( m_doneBB );
      }
      
      virtual void llvmContinue( BasicBlockBuilder &bbb, Location const &location ) const
      {
        bbb->CreateBr( m_nextBB );
      }
      
    private:
    
      llvm::BasicBlock *m_doneBB;
      llvm::BasicBlock *m_nextBB;
    };
    
    class SwitchScope : public Scope
    {
    public:
    
      SwitchScope( Scope const &parentScope, llvm::BasicBlock *breakBB )
        : Scope( parentScope )
        , m_breakBB( breakBB )
      {
      }
      
      virtual void llvmBreak( BasicBlockBuilder &bbb, Location const &location ) const
      {
        bbb->CreateBr( m_breakBB );
      }
      
    private:
    
      llvm::BasicBlock *m_breakBB;
    };
    
    class ModuleScope : public Scope
    {
    public:
    
      ModuleScope()
        : Scope()
      {
      }
    };
    
    class FunctionScope : public Scope
    {
    public:
    
      FunctionScope( ModuleScope const &parentScope, ReturnInfo const &returnInfo )
        : Scope( parentScope )
        , m_returnInfo( returnInfo )
      {
      }
      
      virtual void llvmReturn( BasicBlockBuilder &bbb, llvm::Value *value ) const
      {
        llvmUnwind( bbb );

        if( m_returnInfo.usesReturnLValue() )
        {
          llvm::Value   *aggregateReturn = m_returnInfo.getReturnLValue();
          FABRIC_ASSERT( aggregateReturn );

          m_returnInfo.getAdapter()->llvmAssign( bbb, aggregateReturn, value );
          bbb->CreateRetVoid();
        }
        else
          bbb->CreateRet( value );
      }
      
      virtual ReturnInfo const &getReturnInfo() const
      {
        return m_returnInfo;
      }
      
    private:
    
      ReturnInfo m_returnInfo;
    };
  };
  
  inline std::string _( CG::ReturnInfo const &returnInfo )
  {
    return returnInfo.desc();
  }
};

#endif //_FABRIC_CG_SCOPE_H
