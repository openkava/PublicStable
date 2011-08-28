#include "Manager.h"
#include "BooleanDesc.h"
#include "BooleanImpl.h"
#include "ConstStringDesc.h"
#include "ConstStringImpl.h"
#include "FixedArrayDesc.h"
#include "FixedArrayImpl.h"
#include "FloatDesc.h"
#include "FloatImpl.h"
#include "IntegerDesc.h"
#include "IntegerImpl.h"
#include "StringDesc.h"
#include "StringImpl.h"
#include "VariableArrayDesc.h"
#include "VariableArrayImpl.h"
#include "StructDesc.h"
#include "StructImpl.h"
#include "OpaqueDesc.h"
#include "OpaqueImpl.h"

#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Core/JSON/CommandChannel.h>
#include <Fabric/Base/Config.h>
#include <Fabric/Core/Util/Format.h>
#include <Fabric/Core/Util/Debug.h>

namespace Fabric
{
  namespace RT
  {
    RC::Handle<Manager> Manager::Create( RC::ConstHandle<KLCompiler> const &klCompiler )
    {
#if defined( FABRIC_OS_WINDOWS )
      _set_output_format( _TWO_DIGIT_EXPONENT );
#endif

      return new Manager( klCompiler );
    }

    Manager::Manager( RC::ConstHandle<KLCompiler> const &klCompiler )
      : m_jsonCommandChannel( 0 )
      , m_klCompiler( klCompiler )
    {
      registerDesc( m_booleanDesc = new BooleanDesc( "Boolean", new BooleanImpl( "Boolean" ) ) );
      registerDesc( m_byteDesc = new ByteDesc( "Byte", new ByteImpl( "Byte" ) ) );
      registerDesc( m_integerDesc = new SI32Desc( "Integer", new SI32Impl( "Integer" ) ) );
      registerDesc( m_sizeDesc = new SizeDesc( "Size", new SizeImpl( "Size" ) ) );
      registerDesc( m_scalarDesc = new FP32Desc( "Scalar", new FP32Impl( "Scalar" ) ) );
      registerDesc( m_fp64Desc = new FloatDescT<double>( "Float64", new FloatImplT<double>( "Float64" ) ) );
      registerDesc( m_stringDesc = new StringDesc( "String", new StringImpl( "String" ) ) );
      registerDesc( m_dataDesc = new OpaqueDesc( "Data", new OpaqueImpl( "Data", sizeof(size_t) ) ) );
      registerDesc( m_constStringDesc = new ConstStringDesc( "ConstString", new ConstStringImpl( "ConstString" ) ) );
    }
    
    void Manager::setJSONCommandChannel( JSON::CommandChannel *jsonCommandChannel )
    {
      m_jsonCommandChannel = jsonCommandChannel;
    }
    
    RC::ConstHandle<VariableArrayDesc> Manager::getVariableArrayOf( RC::ConstHandle<RT::Desc> const &memberDesc ) const
    {
      std::string variableArrayName = memberDesc->getName() + "[]";
      RC::ConstHandle<VariableArrayImpl> variableArrayImpl = memberDesc->getImpl()->getVariableArrayImpl();
      RC::ConstHandle<VariableArrayDesc> variableArrayDesc = new VariableArrayDesc( variableArrayName, variableArrayImpl, memberDesc );
      return RC::ConstHandle<VariableArrayDesc>::StaticCast( registerDesc( variableArrayDesc ) );
    }

    RC::ConstHandle<FixedArrayDesc> Manager::getFixedArrayOf( RC::ConstHandle<RT::Desc> const &memberDesc, size_t length ) const
    {
      std::string fixedArrayName = memberDesc->getName() + "[" + _(length) + "]";
      RC::ConstHandle<FixedArrayImpl> fixedArrayImpl = memberDesc->getImpl()->getFixedArrayImpl( length );
      RC::ConstHandle<FixedArrayDesc> fixedArrayDesc = new FixedArrayDesc( fixedArrayName, fixedArrayImpl, memberDesc );
      return RC::ConstHandle<FixedArrayDesc>::StaticCast( registerDesc( fixedArrayDesc ) );
    }

    Manager::Types const &Manager::getTypes() const
    {
      return m_types;
    }

    RC::ConstHandle<StructDesc> Manager::registerStruct( std::string const &name, StructMemberInfoVector const &memberInfos )
    {
      RC::ConstHandle<StructDesc> structDesc = new StructDesc( name, new StructImpl( name, memberInfos ) );
      return RC::ConstHandle<StructDesc>::StaticCast( registerDesc( structDesc ) );
    }

    RC::ConstHandle<OpaqueDesc> Manager::registerOpaque( std::string const &name, size_t size )
    {
      RC::ConstHandle<OpaqueDesc> result = new OpaqueDesc( name, new OpaqueImpl( name, size ) );
      return RC::ConstHandle<OpaqueDesc>::StaticCast( registerDesc( result ) );
    }

    RC::ConstHandle<Desc> Manager::registerAlias( std::string const &name, RC::ConstHandle<RT::Desc> const &desc )
    {
      RC::ConstHandle<Desc> aliasDesc;
      switch ( desc->getType() )
      {
        case DT_BOOLEAN:
          aliasDesc = new BooleanDesc( name, RC::ConstHandle<BooleanImpl>::StaticCast( desc->getImpl() ) );
          break;
        case DT_INTEGER:
          aliasDesc = new IntegerDesc( name, RC::ConstHandle<IntegerImpl>::StaticCast( desc->getImpl() ) );
          break;
        case DT_FLOAT:
          aliasDesc = new FloatDesc( name, RC::ConstHandle<FloatImpl>::StaticCast( desc->getImpl() ) );
          break;
        case DT_CONST_STRING:
          aliasDesc = new ConstStringDesc( name, RC::ConstHandle<ConstStringImpl>::StaticCast( desc->getImpl() ) );
          break;
        case DT_STRING:
          aliasDesc = new StringDesc( name, RC::ConstHandle<StringImpl>::StaticCast( desc->getImpl() ) );
          break;
        case DT_OPAQUE:
          aliasDesc = new OpaqueDesc( name, RC::ConstHandle<OpaqueImpl>::StaticCast( desc->getImpl() ) );
          break;
        case DT_STRUCT:
          aliasDesc = new StructDesc( name, RC::ConstHandle<StructImpl>::StaticCast( desc->getImpl() ) );
          break;
        case DT_VARIABLE_ARRAY:
          aliasDesc = new VariableArrayDesc( name, RC::ConstHandle<VariableArrayImpl>::StaticCast( desc->getImpl() ), RC::ConstHandle<VariableArrayDesc>::StaticCast( desc )->getMemberDesc() );
          break;
        case DT_FIXED_ARRAY:
          aliasDesc = new FixedArrayDesc( name, RC::ConstHandle<FixedArrayImpl>::StaticCast( desc->getImpl() ), RC::ConstHandle<FixedArrayDesc>::StaticCast( desc )->getMemberDesc() );
          break;
        default:
          FABRIC_ASSERT( false && "Unhandled type" );
          break;
      };
      return registerDesc( aliasDesc );
    }
    
    RC::ConstHandle<Desc> Manager::registerDesc( RC::ConstHandle<RT::Desc> const &desc ) const
    {
      std::string const &name = desc->getName();
      std::pair< Types::iterator, bool > insertResult = m_types.insert( Types::value_type( name, desc ) );
      if ( !insertResult.second )
      {
        RC::ConstHandle< RT::Desc > existingDesc = insertResult.first->second;
        if ( desc->isEquivalentTo( existingDesc ) )
          return existingDesc;
        throw Exception( "type '" + name + "' already registered with a different definition" );
      }
      
      if ( m_jsonCommandChannel )
      {
        std::vector<std::string> dst;
        dst.push_back( "RT" );
        dst.push_back( desc->getName() );
        
        m_jsonCommandChannel->jsonNotify( dst, "delta", desc->jsonDesc() );
      }
      
      return desc;
    }
    
    static inline bool isFirstTypeNameChar( char ch )
    {
      return isalpha( ch ) || ch == '_';
    }
    
    static inline bool isRemainingTypeNameChar( char ch )
    {
      return isFirstTypeNameChar( ch ) || isdigit( ch );
    }

    RC::ConstHandle<RT::Desc> Manager::maybeGetBaseDesc( std::string const &baseName ) const
    {
      RC::ConstHandle<RT::Desc> baseDesc;
      Types::const_iterator it = m_types.find( baseName );
      if ( it != m_types.end() )
        baseDesc = it->second;
      return baseDesc;
    }

    RC::ConstHandle<RT::Desc> Manager::maybeGetDesc( std::string const &name ) const
    {
      char const *data = name.data();
      char const *dataEnd = data + name.length();
      
      if ( data == dataEnd || !isFirstTypeNameChar( *data ) )
        throw Exception( "malformed type expression" );
      char const *baseNameStart = data++;
      
      while ( data != dataEnd && isRemainingTypeNameChar( *data ) )
        ++data;
      char const *baseNameEnd = data;
      
      std::string baseName( baseNameStart, baseNameEnd - baseNameStart );
      RC::ConstHandle<RT::Desc> desc = maybeGetBaseDesc( baseName );
      if ( desc )
        desc = getComplexDesc( desc, data, dataEnd );
      return desc;
    }

    RC::ConstHandle<RT::Desc> Manager::getDesc( std::string const &name ) const
    {
      RC::ConstHandle<RT::Desc> result = maybeGetDesc( name );
      if ( !result )
        throw Exception( _(name) + ": type not registered" );
      return result;
    }
    
    RC::ConstHandle<RT::Desc> Manager::getComplexDesc( RC::ConstHandle<RT::Desc> const &desc, char const *data, char const *dataEnd ) const
    {
      if ( data != dataEnd && *data == '[' )
      {
        ++data;
        
        if ( data != dataEnd && *data == ']' )
        {
          ++data;
          return getVariableArrayOf( getComplexDesc( desc, data, dataEnd ) );
        }
        else if ( data != dataEnd && isdigit( *data ) )
        {
          size_t length = 0;
          
          while ( data != dataEnd && isdigit( *data ) )
          {
            length = 10 * length + (*data - '0');
            ++data;
          }
          
          if ( data == dataEnd || *data != ']' )
            throw Exception( "malformed type expression" );
          ++data;
          
          return getFixedArrayOf( getComplexDesc( desc, data, dataEnd ), length );
        }
        else throw Exception( "malformed type expression" );
      }
      else return desc;
    }

    RC::ConstHandle<BooleanDesc> Manager::getBooleanDesc() const
    {
      return m_booleanDesc;
    }
    
    RC::ConstHandle<ByteDesc> Manager::getByteDesc() const
    {
      return m_byteDesc;
    }
    
    RC::ConstHandle<IntegerDesc> Manager::getIntegerDesc() const
    {
      return m_integerDesc;
    }
    
    RC::ConstHandle<SizeDesc> Manager::getSizeDesc() const
    {
      return m_sizeDesc;
    }
    
    RC::ConstHandle<FloatDesc> Manager::getScalarDesc() const
    {
      return m_scalarDesc;
    }
    
    RC::ConstHandle<FloatDesc> Manager::getFP64Desc() const
    {
      return m_fp64Desc;
    }
    
    RC::ConstHandle<StringDesc> Manager::getStringDesc() const
    {
      return m_stringDesc;
    }
    
    RC::ConstHandle<OpaqueDesc> Manager::getDataDesc() const
    {
      return m_dataDesc;
    }
    
    RC::ConstHandle<ConstStringDesc> Manager::getConstStringDesc() const
    {
      return m_constStringDesc;
    }
      
    RC::Handle<JSON::Object> Manager::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = JSON::Object::Create();
      result->set( "registeredTypes", jsonDescRegisteredTypes() );
      return result;
    }
      
    RC::Handle<JSON::Object> Manager::jsonDescRegisteredTypes() const
    {
      RC::Handle<JSON::Object> registeredTypes = JSON::Object::Create();
      for ( Types::const_iterator it=m_types.begin(); it!=m_types.end(); ++it )
        registeredTypes->set( it->first, it->second->jsonDesc() );
      return registeredTypes;
    }

    RC::ConstHandle<JSON::Value> Manager::jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;

      if ( dst.size() - dstOffset == 0 )
      {
        try
        {
          result = jsonExec( cmd, arg );
        }
        catch ( Exception e )
        {
          throw "command " + _(cmd) + ": " + e;
        }
      }
      else throw Exception( "unroutable" );
      
      return result;
    }

    RC::ConstHandle<JSON::Value> Manager::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Value> result;
      if ( cmd == "registerType" )
        jsonExecRegisterType( arg );
      else throw Exception( "unknown command" );
      return result;
    }
    
    void Manager::jsonExecRegisterType( RC::ConstHandle<JSON::Value> const &arg )
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      
      std::string name;
      try
      {
        name = argJSONObject->get( "name" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "'name': " + e;
      }
      
      try
      {
        RC::ConstHandle<JSON::Object> defaultValue;
        try
        {
          defaultValue = argJSONObject->get( "defaultValue" )->toObject();
        }
        catch ( Exception e )
        {
          throw "'defaultValue': " + e;
        }
        
        RT::StructMemberInfoVector memberInfos;
        try
        {
          RC::ConstHandle<JSON::Array> membersArray = argJSONObject->get( "members" )->toArray();
          size_t membersArraySize = membersArray->size();
          for ( size_t i=0; i<membersArraySize; ++i )
          {
            try
            {
              RC::ConstHandle<JSON::Object> memberObject = membersArray->get(i)->toObject();
              RT::StructMemberInfo memberInfo;

              try
              {
                memberInfo.name = memberObject->get( "name" )->toString()->value();
              }
              catch ( Exception e )
              {
                throw "'name': " + e;
              }
              
              try
              {
                std::string typeName = memberObject->get( "type" )->toString()->value();
                if ( typeName.empty() )
                  throw Exception( "must be non-empty" );
                memberInfo.desc = getDesc( typeName );
                if ( !memberInfo.desc )
                  throw Exception( "type " + _(typeName) + " not registered" );
              }
              catch ( Exception e )
              {
                throw "'type': " + e;
              }
              
              memberInfo.defaultData.resize( memberInfo.desc->getSize() );
              try
              {
                memberInfo.desc->setDataFromJSONValue( defaultValue->get( memberInfo.name ), &memberInfo.defaultData[0] );
              }
              catch ( Exception e )
              {
                throw _(memberInfo.name) + " default value: " + e;
              }

              memberInfos.push_back( memberInfo );
            }
            catch ( Exception e )
            {
              throw "index " + _(i) + ": " + e;
            }
          }
        }
        catch ( Exception e )
        {
          throw "members: " + e;
        }
        
        RC::ConstHandle<RC::Object> klBindingsAST;
        try
        {
          RC::ConstHandle<JSON::Value> klBindingsJSONValue = argJSONObject->maybeGet( "kBindings" );
          if ( klBindingsJSONValue )
          {
            std::string klBindings = klBindingsJSONValue->toString()->value();
            klBindingsAST = m_klCompiler->compile( klBindings );
          }
        }
        catch ( Exception e )
        {
          throw "'kBindings': " + e;
        }
        
        RC::ConstHandle< RT::StructDesc > structDesc = registerStruct( name, memberInfos );

        for ( size_t i=0; i<memberInfos.size(); ++i )
        {
          RT::StructMemberInfo &memberInfo = memberInfos[i];
          memberInfo.desc->disposeData( &memberInfo.defaultData[0] );
        }

        if ( klBindingsAST )
          structDesc->setKLBindingsAST( klBindingsAST );
      }
      catch ( Exception e )
      {
        throw "name " + _(name) + ": " + e;
      }
    }
    
    RC::ConstHandle<Desc> Manager::getStrongerTypeOrNone( RC::ConstHandle<Desc> const &lhsDesc, RC::ConstHandle<Desc> const &rhsDesc ) const
    {
      ImplType lhsType = lhsDesc->getType(), rhsType = rhsDesc->getType();
      if ( isNumeric( lhsType ) && isNumeric( rhsType ) )
      {
        RC::ConstHandle<NumericDesc> lhsNumericDesc = RC::ConstHandle<NumericDesc>::StaticCast( lhsDesc );
        RC::ConstHandle<NumericDesc> rhsNumericDesc = RC::ConstHandle<NumericDesc>::StaticCast( rhsDesc );
        if ( lhsNumericDesc->isFloat() )
        {
          if ( rhsNumericDesc->isFloat() && rhsNumericDesc->getSize() > lhsNumericDesc->getSize() )
            return rhsNumericDesc;
          else return lhsNumericDesc;
        }
        else if ( rhsNumericDesc->isFloat() )
          return rhsNumericDesc;
        else
        {
          if ( rhsNumericDesc->getSize() > lhsNumericDesc->getSize()
            || ( rhsNumericDesc->getSize() == lhsNumericDesc->getSize() && RC::ConstHandle<IntegerDesc>::StaticCast(rhsNumericDesc)->isSigned() ) )
            return rhsNumericDesc;
          else return lhsNumericDesc;
        }
      }
      else if( lhsType == rhsType )
      {
        return lhsDesc;
      }
      else if ( isConstString( lhsType ) || isConstString( rhsType ) )
      {
        return m_stringDesc;
      }
      else if ( isString( lhsType ) )
      {
        return lhsDesc;
      }
      else if ( isString( rhsType ) )
      {
        return rhsDesc;
      }
      else return 0;
    }

    void Manager::buildTopoSortedDescs( RC::ConstHandle<Desc> const &desc, std::set< RC::ConstHandle<Desc> > &doneDescs, std::vector< RC::ConstHandle<Desc> > &result ) const
    {
      if ( doneDescs.find( desc ) == doneDescs.end() )
      {
        doneDescs.insert( desc );
        
        if ( isStruct( desc->getType() ) )
        {
          RC::ConstHandle<StructDesc> structDesc = RC::ConstHandle<StructDesc>::StaticCast( desc );
          size_t numMembers = structDesc->getNumMembers();
          for ( size_t i=0; i<numMembers; ++i )
            buildTopoSortedDescs( structDesc->getMemberInfo(i).desc, doneDescs, result );
        }

        if ( isArray( desc->getType() ) )
        {
          RC::ConstHandle<ArrayDesc> arrayDesc = RC::ConstHandle<ArrayDesc>::StaticCast( desc );
          buildTopoSortedDescs( arrayDesc->getMemberDesc(), doneDescs, result );
        }

        result.push_back( desc );
      }
    }

    std::vector< RC::ConstHandle<Desc> > Manager::getTopoSortedDescs() const
    {
      std::vector< RC::ConstHandle<Desc> > result;
      std::set< RC::ConstHandle<Desc> > descsForTopoSort;
      for ( Types::const_iterator it=m_types.begin(); it!=m_types.end(); ++it )
        buildTopoSortedDescs( it->second, descsForTopoSort, result );
      return result;
    }

    bool Manager::maybeGetASTForType( std::string const &typeName, RC::ConstHandle<RC::Object> &ast ) const
    {
      Types::const_iterator it = m_types.find( typeName );
      if ( it != m_types.end() )
      {
        ast = it->second->getKLBindingsAST();
        return true;
      }
      else return false;
    }
  };
};
