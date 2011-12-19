/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_MAP_H
#define _FABRIC_MR_MAP_H

#include <Fabric/Core/MR/ArrayProducer.h>

namespace Fabric
{
  namespace KLC
  {
    class MapOperator;
  };
  
  namespace MR
  {
    class ValueProducer;
    
    class Map : public ArrayProducer
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
    
    public:
    
      static RC::Handle<Map> Create(
        RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
        RC::ConstHandle<KLC::MapOperator> const &mapOperator,
        RC::ConstHandle<ValueProducer> const &sharedValueProducer
        );
      
      // Virtual functions: ArrayProducer
    
    public:
    
      virtual size_t count() const;
      virtual void produce( size_t index, void *data ) const;
            
    protected:
    
      Map(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<ArrayProducer> const &inputArrayProducer,
        RC::ConstHandle<KLC::MapOperator> const &mapOperator,
        RC::ConstHandle<ValueProducer> const &sharedValueProducer
        );
      ~Map();
    
      virtual char const *getKind() const;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const;
    
    private:
    
      RC::ConstHandle<ArrayProducer> m_inputArrayProducer;
      RC::ConstHandle<KLC::MapOperator> m_mapOperator;
      RC::ConstHandle<ValueProducer> m_sharedValueProducer;
    };
  };
};

#endif //_FABRIC_MR_MAP_H
