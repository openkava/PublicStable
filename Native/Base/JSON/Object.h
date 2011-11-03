/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_JSON_OBJECT_H
#define _FABRIC_JSON_OBJECT_H

#include <Fabric/Base/JSON/Value.h>
#include <Fabric/Base/Exception.h>

#include <yajl_gen.h>
#include <map>

namespace Fabric
{
  namespace JSON
  {
    class Object : public Value
    {
      typedef std::map< std::string, RC::ConstHandle<Value> > Map;
      typedef std::vector<Map::const_iterator> MapOrder;
      
    public:
    
      static RC::Handle<Object> Create()
      {
        return new Object;
      }
      
      virtual bool isObject() const { return true; }
      
      bool has( std::string const &key ) const
      {
        return m_map.find( key ) != m_map.end();
      }
      
      RC::ConstHandle<Value> maybeGet( std::string const &key ) const
      {
        Map::const_iterator it = m_map.find( key );
        RC::ConstHandle<Value> result;
        if ( it != m_map.end() )
          result = it->second;
        return result;
      }
      
      RC::ConstHandle<Value> get( std::string const &key ) const
      {
        RC::ConstHandle<Value> result = maybeGet( key );
        if ( !result )
          throw Exception( "missing" );
        return result;
      }
      
      void set( std::string const &key, RC::ConstHandle<Value> const &value )
      {
        Map::iterator it = m_map.find( key );
        if ( it == m_map.end() )
          m_mapOrder.push_back( m_map.insert( Map::value_type( key, value ) ).first );
        else it->second = value;
      }
      
      class const_iterator
      {
      public:
      
        const_iterator( MapOrder::const_iterator const &it )
          : m_it( it )
        {
        }
      
        const_iterator( const_iterator const &that )
          : m_it( that.m_it )
        {
        }
        
        Map::const_iterator operator ->() const
        {
          return *m_it;
        }
        
        void operator ++()
        {
          ++m_it; 
        }
        
        bool operator !=( const_iterator const &that )
        {
          return m_it != that.m_it;
        }
      
      private:
      
        MapOrder::const_iterator m_it;
      };
      
      const_iterator begin() const
      {
        return m_mapOrder.begin();
      }
      const_iterator end() const
      {
        return m_mapOrder.end();
      }
      
      virtual void generate( yajl_gen yajlHandle ) const
      {
        yajl_gen_map_open( yajlHandle );
        for ( const_iterator it=begin(); it!=end(); ++it )
        {
          std::string const &key = it->first;
          yajl_gen_string( yajlHandle, (const unsigned char *)key.data(), key.length() );
          RC::ConstHandle<Value> const &value = it->second;
          value->generate( yajlHandle );
        }
        yajl_gen_map_close( yajlHandle );
      }
      
    protected:  
      
      Object()
      {
      }

    private:
    
      Map m_map;
      MapOrder m_mapOrder;
    };
  };
};

#endif //_FABRIC_UTIL_JSON_H
