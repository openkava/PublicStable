#ifndef _FABRIC_RT_STRUCT_IMPL_H
#define _FABRIC_RT_STRUCT_IMPL_H

#include <Fabric/Core/RT/Impl.h>
#include <Fabric/Core/RT/StructMemberInfo.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace RT
  {
    class StructImpl : public Impl
    {
      friend class Manager;
    
      typedef std::vector< size_t > MemberOffsetVector;
      typedef Util::UnorderedMap< std::string, size_t > NameToIndexMap;
      
    public:
      
      // Impl
      
      virtual void setData( void const *value, void *data ) const;
      virtual void disposeData( void *data ) const;
      virtual std::string descData( void const *data ) const;
      virtual void const *getDefaultData() const;
      
      virtual RC::Handle<JSON::Value> getJSONValue( void const *data ) const;
      virtual void setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &value, void *data ) const;
      
      virtual Util::Encoder &encode( Util::Encoder &encoder, void const *data ) const;
      virtual Util::Decoder &decode( Util::Decoder &decoder, void *data ) const;

      virtual bool isShallow() const;
      virtual bool isEquivalentTo( RC::ConstHandle< RT::Impl > const &desc ) const;
      
      // StructImpl
          
      size_t getNumMembers() const
      {
        return m_numMembers;
      }
      
      StructMemberInfo const &getMemberInfo( size_t index ) const
      {
        if ( index < 0 || index >= m_numMembers )
          throw Exception( "index out of range" );
        return m_memberInfos[index];
      }
      
      void const *getMemberData( void const *data, size_t index ) const
      {
        if ( index < 0 || index >= m_numMembers )
          throw Exception( "index out of range" );
        return getMemberData_NoCheck( data, index );
      }
      
      void *getMemberData( void *data, size_t index ) const
      {
        if ( index < 0 || index >= m_numMembers )
          throw Exception( "index out of range" );
        return getMemberData_NoCheck( data, index );
      }  
     
      bool hasMember( std::string const &name ) const
      {
        return m_nameToIndexMap.find( name ) != m_nameToIndexMap.end();
      }
      
      size_t getMemberIndex( std::string const &name ) const
      {
         NameToIndexMap::const_iterator it = m_nameToIndexMap.find( name );
        if ( it == m_nameToIndexMap.end() )
          throw Exception( "member not found" );
        return it->second;
      }

    protected:
    
      StructImpl( std::string const &codeName, StructMemberInfoVector const &memberInfos );
      ~StructImpl();
      
      void const *getMemberData_NoCheck( void const *data, size_t index ) const
      {
        return static_cast<uint8_t const *>(data) + m_memberOffsets[index];
      }
      
      void *getMemberData_NoCheck( void *data, size_t index ) const
      {
        return static_cast<uint8_t *>(data) + m_memberOffsets[index];
      }  

    private:
    
      std::string m_name;
      StructMemberInfoVector m_memberInfos;
      size_t m_numMembers;
      MemberOffsetVector m_memberOffsets;
      NameToIndexMap m_nameToIndexMap;
      void *m_defaultData;
      bool m_isShallow;
    };
  };
};

#endif //_FABRIC_RT_STRUCT_IMPL_H
