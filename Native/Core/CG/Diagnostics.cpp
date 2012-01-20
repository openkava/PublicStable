/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/CG/Diagnostics.h>

namespace Fabric
{
  namespace CG
  {
    Diagnostics::Diagnostics( Diagnostics const &that )
      : m_containsError( false )
    {
      append( that );
    }
    
    void Diagnostics::append( Diagnostics const &that )
    {
      m_containsError = m_containsError || that.m_containsError;
      for ( Impl::const_iterator it = that.m_impl.begin(); it != that.m_impl.end(); ++it )
        m_impl.push_back( *it );
    }
    
    void Diagnostics::generateJSON( Util::JSONGenerator &jg ) const
    {
      Util::JSONArrayGenerator resultJAG = jg.makeArray();
      for ( CG::Diagnostics::const_iterator it=begin(); it!=end(); ++it )
      {
        CG::Location const &location = it->first;
        CG::Diagnostic const &diagnostic = it->second;
        
        Util::JSONObjectGenerator resultJOG = resultJAG.makeElement().makeObject();
        {
          Util::JSONGenerator memberJG = resultJOG.makeMember( "filename", 8 );
          memberJG.makeString( location.getFilename() );
        }
        {
          Util::JSONGenerator memberJG = resultJOG.makeMember( "line", 4 );
          memberJG.makeInteger( location.getLine() );
        }
        {
          Util::JSONGenerator memberJG = resultJOG.makeMember( "column", 6 );
          memberJG.makeInteger( location.getColumn() );
        }
        {
          Util::JSONGenerator memberJG = resultJOG.makeMember( "level", 5 );
          memberJG.makeString( diagnostic.getLevelDesc() );
        }
        {
          Util::JSONGenerator memberJG = resultJOG.makeMember( "desc", 4 );
          memberJG.makeString( diagnostic.getDesc().getData(), diagnostic.getDesc().getLength() );
        }
      }
    }
  }
}
