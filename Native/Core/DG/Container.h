/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_CONTAINER_H
#define _FABRIC_DG_CONTAINER_H

#include <Fabric/Core/DG/NamedObject.h>
#include <Fabric/Core/DG/Prototype.h>
#include <Fabric/Core/Util/UnorderedMap.h>

#include <map>
#include <string>
#include <stdint.h>

namespace Fabric
{
  namespace Util
  {
    class SimpleString;
    class Encoder;
    class Decoder;
  };
  
  namespace JSON
  {
    class Value;
    class Integer;
    class Object;
  };
  
  namespace MT
  {
    class LogCollector;
    class ParallelCall;
  };
  
  namespace RT
  {
    class Manager;
    class Desc;
    class Impl;
    class SlicedArrayDesc;
  };
  
  namespace DG
  {
    class Scope;
    class Context;
    class Binding;
    class SharedSlicedArray;
    
    class Container : public NamedObject
    {
      friend class Prototype;
      
    public:
    
      virtual bool isContainer() const { return true; }
      virtual bool isNode() const { return false; }
      virtual bool isEvent() const { return false; }
      virtual bool isEventHandler() const { return false; }

      struct MemberDesc
      {
        RC::ConstHandle<RT::Desc> desc;
        void const *defaultData;
      };
      typedef Util::UnorderedMap< std::string, MemberDesc > MemberDescs;
      MemberDescs getMemberDescs() const;
      void addMember( std::string const &name, RC::ConstHandle<RT::Desc> const &desc, void const *defaultData );
      void removeMember( std::string const &name );
      void getMemberArrayDescAndData(
        std::string const &name,
        RC::ConstHandle<RT::SlicedArrayDesc> &slicedArrayDesc,
        void *&slicedArrayData
        );
    
      size_t getCount() const;
      void setCount( size_t count );

      RC::ConstHandle<RT::Desc> getDesc( std::string const &name ) const;
      void const *getConstData( std::string const &name, size_t index ) const;
      void *getMutableData( std::string const &name, size_t index );
      void getData( std::string const &name, size_t index, void *dstData ) const;
      void getDataJSON( std::string const &name, size_t index, Util::JSONGenerator &resultJG ) const;
      void setData( std::string const &name, size_t index, void const *data );

      void generateJSON( Util::JSONGenerator &jsonGenerator ) const;
      void generateSliceJSON( size_t index, Util::JSONGenerator &jsonGenerator ) const;
      void generateMemberJSON( std::string const &member, Util::JSONGenerator &jsonGenerator ) const;

      void setJSON( RC::ConstHandle<JSON::Value> const &value );
      void setSliceJSON( size_t index, RC::ConstHandle<JSON::Value> const &value );

      virtual void jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecAddMember( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecRemoveMember( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonSetCount( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecGetData( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecGetDataJSON( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecGetDataSize( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecGetDataElement( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecSetData( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecGetBulkData( Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecSetBulkData( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecGetSlicesBulkData( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecSetSlicesBulkData( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG );
      void jsonExecGetMembersBulkData( RC::ConstHandle<JSON::Value> const &arg, Util::JSONArrayGenerator &resultJAG ) const;
      void jsonExecPutResourceToFile( RC::ConstHandle<JSON::Value> const &arg, bool userFile, Util::JSONArrayGenerator &resultJAG );
      void jsonExecGetResourceFromFile( RC::ConstHandle<JSON::Value> const &arg, bool userFile, Util::JSONArrayGenerator &resultJAG );
      void jsonDesc( Util::JSONGenerator &resultJG ) const;
      virtual void jsonDesc( Util::JSONObjectGenerator &resultJOG ) const;
      void jsonDescMembers( Util::JSONGenerator &resultJG ) const;
      void jsonDescCount( Util::JSONGenerator &resultJG ) const;
      
    protected:
    
      Container( std::string const &name, RC::Handle<Context> const &context );
      ~Container();
      
      virtual void setOutOfDate() = 0;
      
      virtual RC::Handle<MT::ParallelCall> bind(
        std::vector<std::string> &errors,
        RC::ConstHandle<Binding> const &binding,
        Scope const &scope,
        size_t *newCount,
        unsigned prefixCount = 0,
        void * const *prefixes = 0
        );
      
      class Member;
      typedef std::map< std::string, RC::Handle<Member> > Members;
      
      RC::ConstHandle<Member> getMember( std::string const &name ) const;
      RC::Handle<Member> getMember( std::string const &name );

      void* jsonGetResourceMember( RC::ConstHandle<JSON::Value> const &arg, std::string& memberName );

    private:
    
      Context *m_context;

      size_t m_count;
      Members m_members;
    };
  };
};

#endif //_FABRIC_DG_CONTAINER_H
