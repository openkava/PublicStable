#include "Manager.h"
#include "BooleanAdapter.h"
#include "ByteAdapter.h"
#include "ConstStringAdapter.h"
#include "IntegerAdapter.h"
#include "SizeAdapter.h"
#include "FloatAdapter.h"
#include "StringAdapter.h"
#include "FixedArrayAdapter.h"
#include "VariableArrayAdapter.h"
#include "StructAdapter.h"
#include "OpaqueAdapter.h"
#include "ModuleBuilder.h"

#include <Fabric/Core/RT/BooleanDesc.h>
#include <Fabric/Core/RT/IntegerDesc.h>
#include <Fabric/Core/RT/FloatDesc.h>
#include <Fabric/Core/RT/OpaqueDesc.h>
#include <Fabric/Core/RT/ConstStringDesc.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/RT/FixedArrayDesc.h>
#include <Fabric/Core/RT/VariableArrayDesc.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/Util/Debug.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

namespace Fabric
{
  namespace CG
  {
    RC::Handle<Manager> Manager::Create( RC::Handle<RT::Manager> const &rtManager )
    {
      return new Manager( rtManager );
    }

    Manager::Manager( RC::Handle<RT::Manager> const &rtManager )
      : m_rtManager( rtManager )
    {
    }

    RC::Handle<RT::Manager> Manager::getRTManager() const
    {
      return m_rtManager;
    }
      
    RC::ConstHandle<Adapter> Manager::maybeGetAdapter( std::string const &userName ) const
    {
      RC::ConstHandle<Adapter> result;
      RC::ConstHandle<RT::Desc> rtDesc = m_rtManager->maybeGetDesc( userName );
      if ( rtDesc )
        result = getAdapter( rtDesc );
      return result;
    }
    
    RC::ConstHandle<Adapter> Manager::getAdapter( std::string const &userName ) const
    {
      RC::ConstHandle<Adapter> result = maybeGetAdapter( userName );
      if ( !result )
        throw Exception( _(userName) + ": type not registered" );
      return result;
    }

    std::string const &Manager::getCodeName( std::string const &userName ) const
    {
      return getAdapter( userName )->getCodeName();
    }

    RC::ConstHandle< CG::Adapter > Manager::getAdapter( RC::ConstHandle<RT::Desc> const &desc ) const
    {
      DescToAdapterMap::const_iterator it = m_descToAdapterMap.find( desc );
      if ( it == m_descToAdapterMap.end() )
      {
        RC::ConstHandle<Adapter> adapter;
        
        switch ( desc->getType() )
        {
          case RT::DT_BOOLEAN:
          {
            RC::ConstHandle<RT::BooleanDesc> booleanDesc = RC::ConstHandle<RT::BooleanDesc>::StaticCast( desc );
            adapter = new BooleanAdapter( this, booleanDesc );
          }
          break;
          
          case RT::DT_INTEGER:
          {
            RC::ConstHandle<RT::IntegerDesc> integerDesc = RC::ConstHandle<RT::IntegerDesc>::StaticCast( desc );
            if ( integerDesc->getSize() == 1 && !integerDesc->isSigned() )
              adapter = new ByteAdapter( this, integerDesc );
            else if ( integerDesc->getSize() == 4 && integerDesc->isSigned() )
              adapter = new IntegerAdapter( this, integerDesc );
            else adapter = new SizeAdapter( this, integerDesc );
          }
          break;
          
          case RT::DT_FLOAT:
          {
            RC::ConstHandle<RT::FloatDesc> floatDesc = RC::ConstHandle<RT::FloatDesc>::StaticCast( desc );
            adapter = new FloatAdapter( this, floatDesc );
          }
          break;
          
          case RT::DT_STRING:
          {
            RC::ConstHandle<RT::StringDesc> stringDesc = RC::ConstHandle<RT::StringDesc>::StaticCast( desc );
            adapter = new StringAdapter( this, stringDesc );
          }
          break;
          
          case RT::DT_CONST_STRING:
          {
            RC::ConstHandle<RT::ConstStringDesc> constStringDesc = RC::ConstHandle<RT::ConstStringDesc>::StaticCast( desc );
            adapter = new ConstStringAdapter( this, constStringDesc );
          }
          break;
          
          case RT::DT_STRUCT:
          {
            RC::ConstHandle<RT::StructDesc> structDesc = RC::ConstHandle<RT::StructDesc>::StaticCast( desc );
            adapter = new StructAdapter( this, structDesc );
          }
          break;
                    
          case RT::DT_FIXED_ARRAY:
          {
            RC::ConstHandle<RT::FixedArrayDesc> fixedArrayDesc = RC::ConstHandle<RT::FixedArrayDesc>::StaticCast( desc );
            adapter = new FixedArrayAdapter( this, fixedArrayDesc );
          }
          break;
          
          case RT::DT_VARIABLE_ARRAY:
          {
            RC::ConstHandle<RT::VariableArrayDesc> variableArrayDesc = RC::ConstHandle<RT::VariableArrayDesc>::StaticCast( desc );
            adapter = new VariableArrayAdapter( this, variableArrayDesc );
          }
          break;
          
          case RT::DT_OPAQUE:
          {
            RC::ConstHandle<RT::OpaqueDesc> opaqueDesc = RC::ConstHandle<RT::OpaqueDesc>::StaticCast( desc );
            adapter = new OpaqueAdapter( this, opaqueDesc );
          }
          break;
          
          default:
            FABRIC_ASSERT( false );
            break;
        }
        
        std::pair< DescToAdapterMap::iterator, bool > insertResult = m_descToAdapterMap.insert( DescToAdapterMap::value_type( desc, adapter ) );
        FABRIC_ASSERT( insertResult.second );
        it = insertResult.first;
      }
      return it->second;
    }

    RC::ConstHandle<BooleanAdapter> Manager::getBooleanAdapter() const
    {
      if ( !m_booleanAdapter )
        m_booleanAdapter = RC::ConstHandle<BooleanAdapter>::StaticCast( getAdapter( m_rtManager->getBooleanDesc() ) );
      return m_booleanAdapter;
    }
    
    RC::ConstHandle<ByteAdapter> Manager::getByteAdapter() const
    {
      if ( !m_byteAdapter )
        m_byteAdapter = RC::ConstHandle<ByteAdapter>::StaticCast( getAdapter( m_rtManager->getByteDesc() ) );
      return m_byteAdapter;
    }
    
    RC::ConstHandle<IntegerAdapter> Manager::getIntegerAdapter() const
    {
      if ( !m_integerAdapter )
        m_integerAdapter = RC::ConstHandle<IntegerAdapter>::StaticCast( getAdapter( m_rtManager->getIntegerDesc() ) );
      return m_integerAdapter;
    }
    
    RC::ConstHandle<SizeAdapter> Manager::getSizeAdapter() const
    {
      if ( !m_sizeAdapter )
        m_sizeAdapter = RC::ConstHandle<SizeAdapter>::StaticCast( getAdapter( m_rtManager->getSizeDesc() ) );
      return m_sizeAdapter;
    }
    
    RC::ConstHandle<FloatAdapter> Manager::getScalarAdapter() const
    {
      if ( !m_scalarAdapter )
        m_scalarAdapter = RC::ConstHandle<FloatAdapter>::StaticCast( getAdapter( m_rtManager->getScalarDesc() ) );
      return m_scalarAdapter;
    }
    
    RC::ConstHandle<StringAdapter> Manager::getStringAdapter() const
    {
      if ( !m_stringAdapter )
        m_stringAdapter = RC::ConstHandle<StringAdapter>::StaticCast( getAdapter( m_rtManager->getStringDesc() ) );
      return m_stringAdapter;
    }
    
    RC::ConstHandle<OpaqueAdapter> Manager::getDataAdapter() const
    {
      if ( !m_dataAdapter )
        m_dataAdapter = RC::ConstHandle<OpaqueAdapter>::StaticCast( getAdapter( m_rtManager->getDataDesc() ) );
      return m_dataAdapter;
    }
    
    RC::ConstHandle<ConstStringAdapter> Manager::getConstStringAdapter( size_t length ) const
    {
      ConstStringAdapters::const_iterator it = m_constStringAdapters.find( length );
      if ( it == m_constStringAdapters.end() )
        it = m_constStringAdapters.insert( ConstStringAdapters::value_type( length, RC::ConstHandle<ConstStringAdapter>::StaticCast( getAdapter( m_rtManager->getConstStringDesc( length ) ) ) ) ).first;
      return it->second;
    }
      
    RC::ConstHandle<VariableArrayAdapter> Manager::getVariableArrayOf( RC::ConstHandle<Adapter> const &adapter ) const
    {
      RC::ConstHandle<RT::Desc> variableArrayDesc = m_rtManager->getVariableArrayOf( adapter->getDesc() );
      return RC::ConstHandle<VariableArrayAdapter>::StaticCast( getAdapter( variableArrayDesc ) );
    }
    
    RC::ConstHandle<FixedArrayAdapter> Manager::getFixedArrayOf( RC::ConstHandle<Adapter> const &adapter, size_t length ) const
    {
      RC::ConstHandle<RT::Desc> fixedArrayDesc = m_rtManager->getFixedArrayOf( adapter->getDesc(), length );
      return RC::ConstHandle<FixedArrayAdapter>::StaticCast( getAdapter( fixedArrayDesc ) );
    }
    
    RC::ConstHandle<StructAdapter> Manager::registerStruct( std::string const &name, RT::StructMemberInfoVector const &structMemberInfoVector )
    {
      return RC::ConstHandle<StructAdapter>::StaticCast( getAdapter( m_rtManager->registerStruct( name, structMemberInfoVector ) ) );
    }
    
    RC::ConstHandle<Adapter> Manager::registerAlias( std::string const &name, RC::ConstHandle<Adapter> const &adapter )
    {
      return getAdapter( m_rtManager->registerAlias( name, adapter->getDesc() ) );
    }
    
    void *Manager::llvmResolveExternalFunction( std::string const &functionName ) const
    {
      void *result = 0;
      for ( DescToAdapterMap::const_iterator it=m_descToAdapterMap.begin(); it!=m_descToAdapterMap.end(); ++it )
      {
        RC::ConstHandle<Adapter> const &adapter = it->second;
        result = adapter->llvmResolveExternalFunction( functionName );
        if ( result )
          break;
      }
      return result;
    }
    
    void Manager::llvmAddGlobalMappingsToExecutionEngine( llvm::ExecutionEngine *executionEngine, llvm::Module &module ) const
    {
      for ( DescToAdapterMap::const_iterator it=m_descToAdapterMap.begin(); it!=m_descToAdapterMap.end(); ++it )
      {
        RC::ConstHandle<Adapter> const &adapter = it->second;
        std::string adapterName = "__" + adapter->getCodeName() + "__Adapter";
        llvm::GlobalValue *llvmAdapterGlobalValue = adapter->llvmAdapterGlobalValue( module );
        
        void *adapterPtr = executionEngine->getPointerToGlobalIfAvailable( llvmAdapterGlobalValue );
        if( !adapterPtr )
          executionEngine->addGlobalMapping( llvmAdapterGlobalValue, (void *)adapter.ptr() );
      }
    }

    void Manager::llvmPrepareModule( ModuleBuilder &moduleBuilder ) const
    {
      // [pzion 20110224] Since adapteres are created lazily, we actually
      // iterates the types.
      RT::Manager::Types const &rtTypes = m_rtManager->getTypes();
      for ( RT::Manager::Types::const_iterator it=rtTypes.begin(); it!=rtTypes.end(); ++it )
      {
        RC::ConstHandle<Adapter> adapter = getAdapter( it->second );
        adapter->llvmPrepareModule( moduleBuilder, true );
      }
    }

    llvm::LLVMContext &Manager::getLLVMContext() const
    {
      return m_llvmContext;
    }

    RC::ConstHandle<RT::Desc> Manager::getStrongerTypeOrNone( RC::ConstHandle<RT::Desc> const &lhsDesc, RC::ConstHandle<RT::Desc> const &rhsDesc ) const
    {
      return m_rtManager->getStrongerTypeOrNone( lhsDesc, rhsDesc );
    }
  };
};
