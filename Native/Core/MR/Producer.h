/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_MR_PRODUCER_H
#define _FABRIC_MR_PRODUCER_H

#include <Fabric/Core/MR/Object.h>

namespace Fabric
{
  namespace Util
  {
    class JSONGenerator;
    class JSONObjectGenerator;
  };
  
  namespace MR
  {
    class Producer : public MR::Object
    {
      FABRIC_MR_OBJECT_GET_CLASS_DECL()
      
      // Virtual functions: Object

      virtual void jsonExec(
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
      // Virtual functions: Producer
      
    protected:
    
      Producer( Object::Class const *myClass ); 
    
      virtual char const *getKind() const = 0;
      virtual void toJSONImpl( Util::JSONObjectGenerator &jog ) const = 0;
      
      // Non-virtual functions
    
    public:
    
      void toJSON( Util::JSONGenerator &jg ) const;
      
    private:

      void jsonExecGetJSONDesc(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    };
  };
};

#endif //_FABRIC_MR_PRODUCER_H
