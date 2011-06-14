#ifndef _FABRIC_RT_MANAGER_H
#define _FABRIC_RT_MANAGER_H

#include <Fabric/Core/RT/StructMemberInfo.h>
#include <Fabric/Base/RC/Object.h>
#include <Fabric/Core/Util/UnorderedMap.h>

#include <map>

namespace Fabric
{
  namespace MT
  {
    class LogCollector;
  };
  
  namespace JSON
  {
    class Value;
    class CommandChannel;
  };
  
  namespace RT
  {
    class Desc;
    class BooleanDesc;
    class ByteDesc;
    class IntegerDesc;
    class SizeDesc;
    class ScalarDesc;
    class StringDesc;
    class StructDesc;
    class FixedArrayDesc;
    class VariableArrayDesc;
    class OpaqueDesc;
    class ModuleBuilder;
    class ModuleScope;
    
    class Manager : public RC::Object
    {
    public:
    
      typedef std::map< std::string, RC::ConstHandle<RT::Desc> > Types;
      
      static RC::Handle<Manager> Create();
      
      void setJSONCommandChannel( JSON::CommandChannel *jsonCommandChannel );
      
      RC::ConstHandle<BooleanDesc> getBooleanDesc() const;
      RC::ConstHandle<ByteDesc> getByteDesc() const;
      RC::ConstHandle<IntegerDesc> getIntegerDesc() const;
      RC::ConstHandle<SizeDesc> getSizeDesc() const;
      RC::ConstHandle<ScalarDesc> getScalarDesc() const;
      RC::ConstHandle<StringDesc> getStringDesc() const;
      RC::ConstHandle<OpaqueDesc> getDataDesc() const;
      
      RC::ConstHandle<StructDesc> registerStruct( std::string const &name, StructMemberInfoVector const &memberInfos );
      RC::ConstHandle<OpaqueDesc> registerOpaque( std::string const &name, size_t size );
      RC::ConstHandle<Desc> registerAlias( std::string const &name, RC::ConstHandle< Desc > const &desc );
            
      Types const &getTypes() const;
      
      RC::ConstHandle<Desc> maybeGetBaseDesc( std::string const &baseName ) const;
      RC::ConstHandle<Desc> getDesc( std::string const &name ) const;
      
      RC::ConstHandle<VariableArrayDesc> getVariableArrayOf( RC::ConstHandle<Desc> const &memberDesc ) const;
      RC::ConstHandle<FixedArrayDesc> getFixedArrayOf( RC::ConstHandle<Desc> const &memberDesc, size_t length ) const;
      
      std::string kBindings() const;
      
      RC::ConstHandle<JSON::Value> jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      RC::ConstHandle<JSON::Value> jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      void jsonExecRegisterType( RC::ConstHandle<JSON::Value> const &arg );
      RC::Handle<JSON::Object> jsonDesc() const;
      RC::Handle<JSON::Object> jsonDescRegisteredTypes() const;
      
    protected:
    
      Manager();
      
      RC::ConstHandle<Desc> registerDesc( RC::ConstHandle< Desc > const &desc ) const;

    private:
      
      RC::ConstHandle<Desc> getComplexDesc( RC::ConstHandle<Desc> const &desc, char const *data, char const *dataEnd ) const;

      typedef std::map< std::string, std::string > KBindings;
      std::string buildTopoSortedKBindings( KBindings::iterator const &it, KBindings &kBindings) const;

      mutable Types m_types;
            
      RC::ConstHandle<BooleanDesc> m_booleanDesc;
      RC::ConstHandle<ByteDesc> m_byteDesc;
      RC::ConstHandle<IntegerDesc> m_integerDesc;
      RC::ConstHandle<SizeDesc> m_sizeDesc;
      RC::ConstHandle<ScalarDesc> m_scalarDesc;
      RC::ConstHandle<StringDesc> m_stringDesc;
      RC::ConstHandle<OpaqueDesc> m_dataDesc;
      
      JSON::CommandChannel *m_jsonCommandChannel;
    };
  };
};

#endif //_FABRIC_RT_MANAGER_H
