/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/AST/UseVector.h>
#include <Fabric/Core/AST/Use.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    RC::ConstHandle<UseVector> UseVector::Create( RC::ConstHandle<Use> const &first, RC::ConstHandle<UseVector> const &remaining )
    {
      UseVector *result = new UseVector;
      if ( first )
        result->push_back( first );
      if ( remaining )
      {
        for ( UseVector::const_iterator it=remaining->begin(); it!=remaining->end(); ++it )
          result->push_back( *it );
      }
      return result;
    }
    
    UseVector::UseVector()
    {
    }
    
    void UseVector::appendJSON( Util::JSONGenerator const &jsonGenerator ) const
    {
      Util::JSONArrayGenerator jsonArrayGenerator = jsonGenerator.makeArray();
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->appendJSON( jsonArrayGenerator.makeElement() );
    }
    
    void UseVector::collectUses( UseNameToLocationMap &uses ) const
    {
      for ( const_iterator it=begin(); it!=end(); ++it )
        (*it)->collectUses( uses );
    }
  };
};
