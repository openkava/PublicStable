/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_RT_MANAGER_H
#define _FABRIC_RT_MANAGER_H

#include <Fabric/Core/RT/StructMemberInfo.h>
#include <Fabric/Base/RC/Object.h>

#include <map>
#include <set>

namespace Fabric
{
  namespace MT
  {
    class LogCollector;
  };
  
  namespace JSON
  {
    class CommandChannel;
    class Value;
    class Object;
  };
  
  namespace RT
  {
    class Desc;
    class BooleanDesc;
    class ByteDesc;
    class ConstStringDesc;
    class IntegerDesc;
    class SizeDesc;
    class FloatDesc;
    class StringDesc;
    class StructDesc;
    class FixedArrayDesc;
    class VariableArrayDesc;
    class SlicedArrayDesc;
    class OpaqueDesc;
    class ModuleBuilder;
    class ModuleScope;
    
    class Manager : public RC::Object
    {
    public:
    
      class KLCompiler : public RC::Object
      {
      public:
      
        virtual RC::ConstHandle<RC::Object> compile( std::string const &klFilename, std::string const &klSource ) const = 0;
      };
    
      typedef std::map< std::string, RC::ConstHandle<RT::Desc> > Types;
      
      static RC::Handle<Manager> Create( RC::ConstHandle<KLCompiler> const &klCompiler );
      
      void setJSONCommandChannel( JSON::CommandChannel *jsonCommandChannel );
      
      RC::ConstHandle<BooleanDesc> getBooleanDesc() const;
      RC::ConstHandle<ByteDesc> getByteDesc() const;
      RC::ConstHandle<ConstStringDesc> getConstStringDesc() const;
      RC::ConstHandle<IntegerDesc> getIntegerDesc() const;
      RC::ConstHandle<SizeDesc> getSizeDesc() const;
      RC::ConstHandle<Desc> getIndexDesc() const;
      RC::ConstHandle<FloatDesc> getScalarDesc() const;
      RC::ConstHandle<FloatDesc> getFP64Desc() const;
      RC::ConstHandle<StringDesc> getStringDesc() const;
      RC::ConstHandle<OpaqueDesc> getDataDesc() const;
      
      RC::ConstHandle<StructDesc> registerStruct( std::string const &name, StructMemberInfoVector const &memberInfos );
      RC::ConstHandle<OpaqueDesc> registerOpaque( std::string const &name, size_t size );
      RC::ConstHandle<Desc> registerAlias( std::string const &name, RC::ConstHandle< Desc > const &desc );
            
      Types const &getTypes() const;
      
      RC::ConstHandle<Desc> maybeGetDesc( std::string const &name ) const;
      RC::ConstHandle<Desc> getDesc( std::string const &name ) const;
      
      RC::ConstHandle<VariableArrayDesc> getVariableArrayOf( RC::ConstHandle<Desc> const &memberDesc, size_t flags ) const;
      RC::ConstHandle<SlicedArrayDesc> getSlicedArrayOf( RC::ConstHandle<Desc> const &memberDesc ) const;
      RC::ConstHandle<FixedArrayDesc> getFixedArrayOf( RC::ConstHandle<Desc> const &memberDesc, size_t length ) const;
      
      RC::ConstHandle<JSON::Value> jsonRoute( std::vector<std::string> const &dst, size_t dstOffset, std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      RC::ConstHandle<JSON::Value> jsonExec( std::string const &cmd, RC::ConstHandle<JSON::Value> const &arg );
      void jsonExecRegisterType( RC::ConstHandle<JSON::Value> const &arg );
      RC::Handle<JSON::Object> jsonDesc() const;
      RC::Handle<JSON::Object> jsonDescRegisteredTypes() const;
      
      RC::ConstHandle<Desc> getStrongerTypeOrNone( RC::ConstHandle<Desc> const &lhsDesc, RC::ConstHandle<Desc> const &rhsDesc ) const;
      
      bool maybeGetASTForType( std::string const &typeName, RC::ConstHandle<RC::Object> &ast ) const;

    protected:
    
      Manager( RC::ConstHandle<KLCompiler> const &klCompiler );
      
      RC::ConstHandle<Desc> maybeGetBaseDesc( std::string const &baseName ) const;
      RC::ConstHandle<Desc> registerDesc( RC::ConstHandle< Desc > const &desc ) const;
      
    private:
    
      typedef std::map< size_t, RC::ConstHandle<ConstStringDesc> > ConstStringDescs;
      
      RC::ConstHandle<Desc> getComplexDesc( RC::ConstHandle<Desc> const &desc, char const *data, char const *dataEnd ) const;

      mutable Types m_types;
            
      RC::ConstHandle<BooleanDesc> m_booleanDesc;
      RC::ConstHandle<ByteDesc> m_byteDesc;
      RC::ConstHandle<IntegerDesc> m_integerDesc;
      RC::ConstHandle<SizeDesc> m_sizeDesc;
      RC::ConstHandle<Desc> m_indexDesc;
      RC::ConstHandle<FloatDesc> m_scalarDesc;
      RC::ConstHandle<FloatDesc> m_fp64Desc;
      RC::ConstHandle<StringDesc> m_stringDesc;
      RC::ConstHandle<OpaqueDesc> m_dataDesc;
      RC::ConstHandle<ConstStringDesc> m_constStringDesc;
      
      JSON::CommandChannel *m_jsonCommandChannel;
      
      RC::ConstHandle<KLCompiler> m_klCompiler;
    };
  };
};

#endif //_FABRIC_RT_MANAGER_H
