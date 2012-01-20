/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/Container.h>
#include <Fabric/Core/DG/Scope.h>
#include <Fabric/Core/DG/Context.h>
#include <Fabric/Core/DG/ExecutionEngine.h>
#include <Fabric/Core/DG/Binding.h>
#include <Fabric/Core/DG/FabricResource.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/IO/Manager.h>
#include <Fabric/Core/IO/FileHandleManager.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Core/RT/NumericDesc.h>
#include <Fabric/Core/RT/SlicedArrayDesc.h>
#include <Fabric/Core/RT/SlicedArrayImpl.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Base/JSON/Value.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Core/Util/Base64.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Base/JSON/Decode.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace DG
  {
    class Container::Member : public RC::Object
    {
    public:
    
      static RC::Handle<Member> Create( RC::Handle<RT::Manager> const &rtManager, RC::ConstHandle<RT::Desc> memberDesc, size_t count, void const *defaultMemberData )
      {
        return new Member( rtManager, memberDesc, count, defaultMemberData );
      }
      
      void const *getDefaultData() const
      {
        return m_defaultMemberData;
      }
      
      RC::ConstHandle<RT::Desc> getDesc() const
      {
        return m_memberDesc;
      }
      
      void getArrayDescAndData(
        RC::ConstHandle<RT::SlicedArrayDesc> &slicedArrayDesc,
        void *&slicedArrayData
        ) const
      {
        slicedArrayDesc = m_slicedArrayDesc;
        slicedArrayData = m_slicedArrayData;
      }
      
      void const *getImmutableElementData( size_t index ) const
      {
        return m_slicedArrayDesc->getMemberData( (void const *)m_slicedArrayData, index );
      }
      
      void *getMutableElementData( size_t index )
      {
        return m_slicedArrayDesc->getMemberData( m_slicedArrayData, index );
      }
      
      size_t size() const
      {
        return m_slicedArrayDesc->getNumMembers( m_slicedArrayData );
      }
      
      void resize( size_t newCount )
      {
        m_slicedArrayDesc->setNumMembers( m_slicedArrayData, newCount, m_defaultMemberData );
      }
      
      size_t getMemoryUsage() const
      {
        return m_slicedArrayDesc->getAllocSize() + m_slicedArrayDesc->getIndirectMemoryUsage( m_slicedArrayData );
      }

    protected:
      
      Member( RC::Handle<RT::Manager> const &rtManager, RC::ConstHandle<RT::Desc> memberDesc, size_t count, void const *defaultMemberData );
      ~Member();

    private:
    
      RC::ConstHandle<RT::Desc> m_memberDesc;
      void *m_defaultMemberData;
      RC::ConstHandle<RT::SlicedArrayDesc> m_slicedArrayDesc;
      void *m_slicedArrayData;
    };

    Container::Member::Member( RC::Handle<RT::Manager> const &rtManager, RC::ConstHandle<RT::Desc> memberDesc, size_t count, void const *defaultMemberData )
      : m_memberDesc( memberDesc )
    {
      if ( !defaultMemberData )
        defaultMemberData = memberDesc->getDefaultData();
      
      size_t memberSize = memberDesc->getAllocSize();
      m_defaultMemberData = malloc( memberSize );
      memset( m_defaultMemberData, 0, memberSize );
      memberDesc->setData( defaultMemberData, m_defaultMemberData );
      
      m_slicedArrayDesc = rtManager->getSlicedArrayOf( memberDesc );
      
      size_t arraySize = m_slicedArrayDesc->getAllocSize();
      m_slicedArrayData = malloc( arraySize );
      memset( m_slicedArrayData, 0, arraySize );
      m_slicedArrayDesc->setNumMembers( m_slicedArrayData, count, m_defaultMemberData );
    }
    
    Container::Member::~Member()
    {
      m_slicedArrayDesc->disposeData( m_slicedArrayData );
      free( m_slicedArrayData );
      
      m_memberDesc->disposeData( m_defaultMemberData );
      free( m_defaultMemberData );
    }

    Container::Container( std::string const &name, RC::Handle<Context> const &context )
      : NamedObject( name, context )
      , m_context( context.ptr() )
      , m_count( 1 )
    {
    }
    
    Container::~Container()
    {
    }
    
    Container::MemberDescs Container::getMemberDescs() const
    {
      MemberDescs result;
      for ( Members::const_iterator it=m_members.begin(); it!=m_members.end(); ++it )
      {
        MemberDesc &memberDesc = result[it->first];
        memberDesc.desc = it->second->getDesc();
        memberDesc.defaultData = it->second->getDefaultData();
      }
      return result;
    }

    void Container::addMember( std::string const &name, RC::ConstHandle<RT::Desc> const &desc, void const *defaultData )
    {
      if ( name.length() == 0 )
        throw Exception( "name must be non-empty" );
      
      RC::Handle<Member> member = Member::Create( m_context->getRTManager(), desc, m_count, defaultData );
      bool insertResult = m_members.insert( Members::value_type( name, member ) ).second;
      if ( !insertResult )
        throw Exception( "node already has a member named '"+name+"'" );
      
      markForRecompile();
      
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        jsonDescMembers( jg );
      }
      jsonNotifyMemberDelta( "members", 7, json );
    }
    
    void Container::removeMember( std::string const &name )
    {
      Members::iterator it = m_members.find( name );
      if ( it == m_members.end() )
        throw Exception( name + ": member not found" );
      m_members.erase( it );
      
      markForRecompile();
      
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        jsonDescMembers( jg );
      }
      jsonNotifyMemberDelta( "members", 7, json );
    }

    void Container::getMemberArrayDescAndData(
      std::string const &name,
      RC::ConstHandle<RT::SlicedArrayDesc> &slicedArrayDesc,
      void *&slicedArrayData
      )
    {
      Members::const_iterator it = m_members.find( name );
      if ( it == m_members.end() )
        throw Exception( _(name) + ": no such member" );
      it->second->getArrayDescAndData( slicedArrayDesc, slicedArrayData );
    }

    size_t Container::getCount() const
    {
      return m_count;
    }
    
    void Container::setCount( size_t count )
    {
      if ( count != m_count )
      {
        for ( Members::const_iterator it=m_members.begin(); it!=m_members.end(); ++it )
          it->second->resize( count );
        m_count = count;

        markForRefresh();
      
        Util::SimpleString json;
        {
          Util::JSONGenerator jg( &json );
          jsonDescCount( jg );
        }
        jsonNotifyMemberDelta( "count", 5, json );
      }
    }
    
    RC::ConstHandle<Container::Member> Container::getMember( std::string const &name ) const
    {
      Members::const_iterator it = m_members.find( name );
      if ( it == m_members.end() )
        throw Exception( _(name) + ": no such member" );
      return it->second;
    }

    RC::Handle<Container::Member> Container::getMember( std::string const &name )
    {
      Members::const_iterator it = m_members.find( name );
      if ( it == m_members.end() )
        throw Exception( _(name) + ": no such member" );
      setOutOfDate();
      return it->second;
    }

    RC::ConstHandle< RT::Desc > Container::getDesc( std::string const &name ) const
    {
      return getMember( name )->getDesc();
    }
    
    void const *Container::getConstData( std::string const &name, size_t index ) const
    {
      if ( index >= m_count )
        throw Exception( "index out of range" );
      return getMember( name )->getImmutableElementData( index );
    }

    void *Container::getMutableData( std::string const &name, size_t index )
    {
      if ( index >= m_count )
        throw Exception( "index out of range" );
        
      setOutOfDate();
      
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        Util::JSONObjectGenerator jog = jg.makeObject();
        
        {
          Util::JSONGenerator memberNameJG = jog.makeMember( "memberName", 10 );
          memberNameJG.makeString( name );
        }
        
        {
          Util::JSONGenerator memberNameJG = jog.makeMember( "sliceIndex", 10 );
          memberNameJG.makeInteger( index );
        }
      }
      jsonNotify( "dataChange", 10, &json );

      return getMember( name )->getMutableElementData( index );
    }
    
    void Container::getData( std::string const &name, size_t index, void *dstData ) const
    {
      if ( index >= m_count )
        throw Exception( "index out of range" );
      RC::ConstHandle<Member> member = getMember( name );
      return member->getDesc()->setData( member->getImmutableElementData( index ), dstData );
    }

    void Container::getDataJSON( std::string const &name, size_t index, Util::JSONGenerator &resultJG ) const
    {
      if ( index >= m_count )
        throw Exception( "index out of range" );
      RC::ConstHandle<Member> member = getMember( name );
      return member->getDesc()->generateJSON( member->getImmutableElementData( index ), resultJG );
    }
    
    void Container::setData( std::string const &name, size_t index, void const *data )
    {
      if ( index >= m_count )
        throw Exception( "index out of range" );
      
      RC::Handle<Member> member = getMember( name );
      setOutOfDate();
      member->getDesc()->setData( data, member->getMutableElementData( index ) );
      
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        Util::JSONObjectGenerator jog = jg.makeObject();
        
        {
          Util::JSONGenerator memberNameJG = jog.makeMember( "memberName", 10 );
          memberNameJG.makeString( name );
        }
        
        {
          Util::JSONGenerator memberNameJG = jog.makeMember( "sliceIndex", 10 );
          memberNameJG.makeInteger( index );
        }
      }
      jsonNotify( "dataChange", 10, &json );
    }
    
    void Container::generateJSON( Util::JSONGenerator &jsonGenerator ) const
    {
      Util::JSONObjectGenerator jsonObjectGenerator = jsonGenerator.makeObject();
      for ( Members::const_iterator it=m_members.begin(); it!=m_members.end(); ++it )
      {
        std::string const &name = it->first;
        RC::ConstHandle<Member> member = it->second;
        RC::ConstHandle<RT::Desc> memberDesc = member->getDesc();
        Util::JSONGenerator memberJG = jsonObjectGenerator.makeMember( name );
        Util::JSONArrayGenerator memberJAG = memberJG.makeArray();
        for ( size_t i=0; i<m_count; ++i )
        {
          Util::JSONGenerator elementJG = memberJAG.makeElement();
          memberDesc->generateJSON( member->getImmutableElementData(i), elementJG );
        }
      }
    }
    
    void Container::generateSliceJSON( size_t index, Util::JSONGenerator &jsonGenerator ) const
    {
      if ( index >= m_count )
        throw Exception( "index "+_(index)+" out of range (slice count is "+_(m_count)+")" );
        
      Util::JSONObjectGenerator jsonObjectGenerator = jsonGenerator.makeObject();
      for ( Members::const_iterator it=m_members.begin(); it!=m_members.end(); ++it )
      {
        std::string const &name = it->first;
        RC::ConstHandle<Member> member = it->second;
        RC::ConstHandle<RT::Desc> memberDesc = member->getDesc();
        Util::JSONGenerator memberJG = jsonObjectGenerator.makeMember(name);
        memberDesc->generateJSON( member->getImmutableElementData( index ), memberJG );
      }
    }
    
    void Container::generateMemberJSON( std::string const &memberName, Util::JSONGenerator &jsonGenerator ) const
    {
      Members::const_iterator it = m_members.find( memberName );
      if ( it == m_members.end() )
        throw Exception( "no such member "+_(memberName) );
      RC::ConstHandle<Member> member = it->second;
      RC::ConstHandle<RT::Desc> memberDesc = member->getDesc();
      
      size_t sliceCount = member->size();
      Util::JSONArrayGenerator jsonArrayGenerator = jsonGenerator.makeArray();
      for ( size_t sliceIndex=0; sliceIndex<sliceCount; ++sliceIndex )
      {
        Util::JSONGenerator elementJG = jsonArrayGenerator.makeElement();
        memberDesc->generateJSON( member->getImmutableElementData( sliceIndex ), elementJG );
      }
    }

    void Container::setJSON( RC::ConstHandle<JSON::Value> const &value )
    {
      if ( value->isArray() )
      {
        RC::ConstHandle<JSON::Array> const &arrayValue = RC::ConstHandle<JSON::Array>::StaticCast( value );
        setCount( arrayValue->size() );
        
        for ( size_t i=0; i<m_count; ++i )
          setSliceJSON( i, arrayValue->get(i) );
      }
      else if ( value->isObject() )
      {
        RC::ConstHandle<JSON::Object> objectValue = RC::ConstHandle<JSON::Object>::StaticCast( value );
        
        bool first = true;
        for ( JSON::Object::const_iterator it=objectValue->begin(); it!=objectValue->end(); ++it )
        {
          std::string const &name = it->first;
          Members::const_iterator jt = m_members.find( name );
          if ( jt == m_members.end() )
            throw Exception( "node does not have member named '" + name + "'" );
          RC::Handle<Member> member = jt->second;
          
          try
          {
            RC::ConstHandle<JSON::Array> const &arrayValue = it->second->toArray();
            if ( first )
            {
              setCount( arrayValue->size() );
              first = false;
            }
            else
            {
              if ( arrayValue->size() != m_count )
                throw Exception( "inconsistent array length" );
            }
          
            RC::ConstHandle<RT::Desc> memberDesc = member->getDesc();
            for ( size_t i=0; i<m_count; ++i )
              memberDesc->setDataFromJSONValue( arrayValue->get(i), member->getMutableElementData(i) );
            setOutOfDate();
          }
          catch ( Exception e )
          {
            throw _(name) + ": " + e;
          }
        }
      }
      else throw Exception( "JSON must be an object or an array" );
    }

    void Container::setSliceJSON( size_t index, RC::ConstHandle<JSON::Value> const &value )
    {
      if ( index >= m_count )
        throw Exception( "index "+_(index)+" out of range (slice count is "+_(m_count)+")" );
        
      if ( !value->isObject() )
        throw Exception( "malformed JSON" );
      RC::ConstHandle<JSON::Object> objectValue = RC::ConstHandle<JSON::Object>::StaticCast( value );
      
      for ( JSON::Object::const_iterator it=objectValue->begin(); it!=objectValue->end(); ++it )
      {
        std::string const &name = it->first;
        Members::const_iterator jt = m_members.find( name );
        if ( jt == m_members.end() )
          throw Exception( "node does not have member named " + _(name) );
        RC::Handle<Member> member = jt->second;
        
        try
        {
          member->getDesc()->setDataFromJSONValue( it->second, member->getMutableElementData(index) );
          setOutOfDate();
        }
        catch ( Exception e )
        {
          throw _(name) + ": " + e;
        }
      }
    }
      
    RC::Handle<MT::ParallelCall> Container::bind(
      std::vector<std::string> &errors,
      RC::ConstHandle<Binding> const &binding,
      Scope const &scope,
      size_t *newCount,
      unsigned prefixCount,
      void * const *prefixes
      )
    {
      SelfScope selfScope( this, &scope );

      return binding->bind( errors, selfScope, newCount, prefixCount, prefixes );
    }
    
    void Container::jsonDescMembers( Util::JSONGenerator &resultJG ) const
    {
      Util::JSONObjectGenerator resultJSONObject = resultJG.makeObject();
      for ( Members::const_iterator it=m_members.begin(); it!=m_members.end(); ++it )
      {
        std::string const &name = it->first;
        RC::ConstHandle<Member> member = it->second;
        RC::ConstHandle<RT::Desc> memberDesc = member->getDesc();
        Util::JSONGenerator memberJG = resultJSONObject.makeMember( name );
        Util::JSONObjectGenerator memberJOG = memberJG.makeObject();
        memberJOG.makeMember( "type", 4 ).makeString( memberDesc->getUserName() );
      }
    }
      
    void Container::jsonDescCount( Util::JSONGenerator &resultJG ) const
    {
      resultJG.makeInteger( getCount() );
    }
      
    void Container::jsonDesc( Util::JSONGenerator &resultJG ) const
    {
      NamedObject::jsonDesc( resultJG );
    }
      
    void Container::jsonDesc( Util::JSONObjectGenerator &resultJOG ) const
    {
      NamedObject::jsonDesc( resultJOG );
      
      {
        Util::JSONGenerator membersJG = resultJOG.makeMember( "members", 7 );
        jsonDescMembers( membersJG );
      }
      
      {
        Util::JSONGenerator countJG = resultJOG.makeMember( "count", 5 );
        jsonDescCount( countJG );
      }
    }
    
    void Container::jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      if ( cmd == "getData" )
        jsonExecGetData( arg, resultJAG );
      else if ( cmd == "getDataJSON" )
        jsonExecGetDataJSON( arg, resultJAG );
      else if ( cmd == "getDataJSON" )
        jsonExecGetDataJSON( arg, resultJAG );
      else if ( cmd == "getDataSize" )
        jsonExecGetDataSize( arg, resultJAG );
      else if ( cmd == "getDataElement" )
        jsonExecGetDataElement( arg, resultJAG );
      else if ( cmd == "putResourceToFile" )
        jsonExecPutResourceToFile( arg, resultJAG );
      else if ( cmd == "setData" )
        jsonExecSetData( arg, resultJAG );
      else if ( cmd == "getBulkData" )
        jsonExecGetBulkData( resultJAG );
      else if ( cmd == "setBulkData" )
        jsonExecSetBulkData( arg, resultJAG );
      else if ( cmd == "getBulkDataJSON" )
        jsonExecGetBulkDataJSON( resultJAG );
      else if ( cmd == "setBulkDataJSON" )
        jsonExecSetBulkDataJSON( arg, resultJAG );
      else if ( cmd == "getSlicesBulkData" )
        jsonExecGetSlicesBulkData( arg, resultJAG );
      else if ( cmd == "setSlicesBulkData" )
        jsonExecSetSlicesBulkData( arg, resultJAG );
      else if ( cmd == "getMembersBulkData" )
        jsonExecGetMembersBulkData( arg, resultJAG );
      else if ( cmd == "addMember" )
        jsonExecAddMember( arg, resultJAG );
      else if ( cmd == "removeMember" )
        jsonExecRemoveMember( arg, resultJAG );
      else if ( cmd == "setCount" )
        jsonSetCount( arg, resultJAG );
      else
        NamedObject::jsonExec( cmd, arg, resultJAG );
    }

    void Container::jsonExecAddMember( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
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
      
      RC::ConstHandle<RT::Desc> desc;
      try
      {
        std::string type = argJSONObject->get( "type" )->toString()->value();
        desc = m_context->getRTManager()->getDesc( type );
      }
      catch ( Exception e )
      {
        throw "'desc': " + e;
      }
      
      std::vector<uint8_t> defaultValue;
      try
      {
        RC::ConstHandle<JSON::Value> defaultValueJSONValue = argJSONObject->maybeGet( "defaultValue" );
        if ( defaultValueJSONValue )
        {
          defaultValue.resize( desc->getAllocSize() );
          desc->setDataFromJSONValue( defaultValueJSONValue, &defaultValue[0] );
        }
      }
      catch ( Exception e )
      {
        throw "'defaultValue': " + e;
      }
      
      addMember( name, desc, defaultValue.size()>0? &defaultValue[0]: 0 );

      if ( defaultValue.size() > 0 )
        desc->disposeData( &defaultValue[0] );
    }

    void Container::jsonExecRemoveMember( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      removeMember( arg->toString()->value() );
    }

    void Container::jsonSetCount( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      int32_t newCount = arg->toInteger()->value();
      if ( newCount < 0 )
        throw Exception( "count must be non-negative" );
      setCount( size_t( newCount ) );
    }
    
    void Container::jsonGenerateMemberSliceJSON( RC::ConstHandle<JSON::Value> const &arg, Util::JSONGenerator &resultJG )
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      
      RC::ConstHandle<Member> member;
      try
      {
        std::string memberName = argJSONObject->get( "memberName" )->toString()->value();
        member = getMember( memberName );
      }
      catch ( Exception e )
      {
        throw "'memberName': " + e;
      }

      size_t sliceIndex;
      try
      {
        sliceIndex = argJSONObject->get( "sliceIndex" )->toInteger()->value();
        if ( sliceIndex >= m_count )
          throw Exception( "out of range" );
      }
      catch ( Exception e )
      {
        throw "'sliceIndex': " + e;
      }

      member->getDesc()->generateJSON( member->getImmutableElementData( sliceIndex ), resultJG );
    }
      
    void Container::jsonExecGetData( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      Util::JSONGenerator resultJG = resultJAG.makeElement();
      jsonGenerateMemberSliceJSON( arg, resultJG );
    }
      
    void Container::jsonExecGetDataJSON( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        jsonGenerateMemberSliceJSON( arg, jg );
      }
      
      Util::JSONGenerator resultJG = resultJAG.makeElement();
      resultJG.makeString( json );
    }
      
    void Container::jsonExecGetDataSize( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      
      std::string memberName;
      try
      {
        memberName = argJSONObject->get( "memberName" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "'memberName': " + e;
      }
      
      RC::ConstHandle<RT::Desc> desc = getDesc( memberName );
      if ( !RT::isArray( desc->getType() ) )
        throw Exception( "member type is not an array" );
      RC::ConstHandle<RT::ArrayDesc> arrayDesc = RC::ConstHandle<RT::ArrayDesc>::StaticCast( desc );
      
      size_t sliceIndex;
      try
      {
        sliceIndex = argJSONObject->get( "sliceIndex" )->toInteger()->value();
      }
      catch ( Exception e )
      {
        throw "'sliceIndex': " + e;
      }
      
      void const *data = getConstData( memberName, sliceIndex );
      Util::JSONGenerator resultJG = resultJAG.makeElement();
      resultJG.makeInteger( arrayDesc->getNumMembers( data ) );
    }
      
    void Container::jsonExecGetDataElement( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      
      std::string memberName;
      try
      {
        memberName = argJSONObject->get( "memberName" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "'memberName': " + e;
      }
      
      RC::ConstHandle<RT::Desc> desc = getDesc( memberName );
      if ( !RT::isArray( desc->getType() ) )
        throw Exception( "member type is not an array" );
      RC::ConstHandle<RT::ArrayDesc> arrayDesc = RC::ConstHandle<RT::ArrayDesc>::StaticCast( desc );
      
      size_t sliceIndex;
      try
      {
        sliceIndex = argJSONObject->get( "sliceIndex" )->toInteger()->value();
      }
      catch ( Exception e )
      {
        throw "'sliceIndex': " + e;
      }
      
      void const *data = getConstData( memberName, sliceIndex );

      size_t elementIndex;
      try
      {
        elementIndex = argJSONObject->get( "elementIndex" )->toInteger()->value();
      }
      catch ( Exception e )
      {
        throw "'elementIndex': " + e;
      }
      
      void const *memberData = arrayDesc->getMemberData( data, elementIndex );
      Util::JSONGenerator resultJG = resultJAG.makeElement();
      arrayDesc->getMemberDesc()->generateJSON( memberData, resultJG );
    }
      
    void Container::jsonExecSetData( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      
      std::string memberName;
      try
      {
        memberName = argJSONObject->get( "memberName" )->toString()->value();
      }
      catch ( Exception e )
      {
        throw "'memberName': " + e;
      }
      
      size_t sliceIndex;
      try
      {
        sliceIndex = argJSONObject->get( "sliceIndex" )->toInteger()->value();
      }
      catch ( Exception e )
      {
        throw "'sliceIndex': " + e;
      }
      
      RC::ConstHandle<JSON::Value> dataJSONValue;
      try
      {
        dataJSONValue = argJSONObject->get( "data" );
      }
      catch ( Exception e )
      {
        throw "'data': " + e;
      }
      
      getDesc( memberName )->setDataFromJSONValue( dataJSONValue, getMutableData( memberName, sliceIndex ) );
    }
      
    void Container::jsonExecGetBulkData( Util::JSONArrayGenerator &resultJAG ) const
    {
      Util::JSONGenerator resultJG = resultJAG.makeElement();
      generateJSON( resultJG );
    }
      
    void Container::jsonExecGetBulkDataJSON( Util::JSONArrayGenerator &resultJAG ) const
    {
      Util::SimpleString json;
      {
        Util::JSONGenerator jg( &json );
        generateJSON( jg );
      }
      
      Util::JSONGenerator resultJG = resultJAG.makeElement();
      resultJG.makeString( json );
    }
      
    void Container::jsonExecSetBulkData( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      setJSON( arg );
    }
      
    void Container::jsonExecSetBulkDataJSON( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      std::string jsonString = arg->toString()->value();
      RC::ConstHandle<JSON::Value> jsonValue = JSON::decode( jsonString );
      setJSON( jsonValue );
    }
      
    void Container::jsonExecGetSlicesBulkData( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const
    {
      RC::ConstHandle<JSON::Array> argJSONArray = arg->toArray();
      size_t argJSONArraySize = argJSONArray->size();
      
      Util::JSONGenerator resultJG = resultJAG.makeElement();
      Util::JSONArrayGenerator slicesJAG = resultJG.makeArray();
      for ( size_t i=0; i<argJSONArraySize; ++i )
      {
        try
        {
          size_t index = argJSONArray->get( i )->toInteger()->value();
          Util::JSONGenerator sliceJG = slicesJAG.makeElement();
          generateSliceJSON( index, sliceJG );
        }
        catch ( Exception e )
        {
          throw "index " + _(i) + ": " + e;
        }
      }
    }
      
    void Container::jsonExecGetMembersBulkData( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const
    {
      RC::ConstHandle<JSON::Array> argJSONArray = arg->toArray();
      size_t argJSONArraySize = argJSONArray->size();
      
      Util::JSONGenerator resultJG = resultJAG.makeElement();
      Util::JSONObjectGenerator membersJOG = resultJG.makeObject();
      for ( size_t i=0; i<argJSONArraySize; ++i )
      {
        try
        {
          std::string memberName = argJSONArray->get(i)->toString()->value();
          Util::JSONGenerator memberJG = membersJOG.makeMember( memberName );
          generateMemberJSON( memberName, memberJG );
        }
        catch ( Exception e )
        {
          throw "index " + _(i) + ": " + e;
        }
      }
    }
      
    void Container::jsonExecSetSlicesBulkData( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      RC::ConstHandle<JSON::Array> argJSONArray = arg->toArray();
      size_t argJSONArraySize = argJSONArray->size();
      for ( size_t i=0; i<argJSONArraySize; ++i )
      {
        try
        {
          RC::ConstHandle<JSON::Object> argJSONObject = argJSONArray->get(i)->toObject();
          
          size_t sliceIndex;
          try
          {
            sliceIndex = argJSONObject->get( "sliceIndex" )->toInteger()->value();
          }
          catch ( Exception e )
          {
            throw "'sliceIndex': " + e;
          }
          
          RC::ConstHandle<JSON::Value> dataJSONValue;
          try
          {
            dataJSONValue = argJSONObject->get( "data" );
          }
          catch ( Exception e )
          {
            throw "'data': " + e;
          }
          
          setSliceJSON( sliceIndex, dataJSONValue );
        }
        catch ( Exception e )
        {
          throw "index " + _(i) + ": " + e;
        }
      }
    }

    void Container::jsonExecPutResourceToFile( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG )
    {
      RC::ConstHandle<JSON::Object> argJSONObject = arg->toObject();
      std::string memberName = argJSONObject->get( "memberName" )->toString( "memberName must be a String" )->value();
      std::string handle = argJSONObject->get( "file" )->toString( "file handle must be a String" )->value();

      RC::Handle<Container::Member> member = getMember( memberName );
      RC::ConstHandle<RT::Desc> desc = member->getDesc();
      if( desc->getUserName() != "FabricResource" )
        throw Exception( "member" + memberName + " is not of type FabricResource" );

      FabricResourceWrapper resource( m_context->getRTManager(), member->getMutableElementData( 0 ) );

      std::string dataExternalLocation = resource.getDataExternalLocation();
      if( dataExternalLocation.empty() )
        m_context->getIOManager()->getFileHandleManager()->putFile( handle, resource.getDataSize(), resource.getDataPtr(), false );
      else
        m_context->getIOManager()->getFileHandleManager()->copyFile( dataExternalLocation, handle );
    }

    void Container::jsonGetMemoryUsage( Util::JSONGenerator &jg ) const
    {
      Util::JSONObjectGenerator jog = jg.makeObject();
      for ( Members::const_iterator it = m_members.begin(); it != m_members.end(); ++it )
      {
        Util::JSONGenerator memberJG = jog.makeMember( it->first );
        RC::Handle<Member> const &member = it->second;
        memberJG.makeInteger( int32_t( member->getMemoryUsage() ) );
      }
    }
  };
};
