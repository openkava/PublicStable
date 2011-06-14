/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/DG/CodeManager.h>
#include <Fabric/Core/Util/Assert.h>
#include <Fabric/Core/DG/IRCache.h>

namespace Fabric
{
  namespace DG
  {
    CodeManager::CodeManager()
      : m_irCache( IRCache::Create() )
    {
    }
    
    RC::ConstHandle<Code> CodeManager::compileSourceCode( RC::ConstHandle<Context> const &context, std::string const &sourceCode )
    {
      RC::ConstHandle<Code> result;
      
      SourceCodeToCodeMap::const_iterator it = m_sourceCodeToCodeMap.find( sourceCode );
      if ( it != m_sourceCodeToCodeMap.end() )
        result = it->second.makeStrong();
        
      if ( !result )
      {
        //FABRIC_DEBUG_LOG( "No compiled code in cache; compiling" );
        result = Code::Create( context, sourceCode, m_irCache );
        it = m_sourceCodeToCodeMap.insert( SourceCodeToCodeMap::value_type( sourceCode, result ) ).first;
        FABRIC_ASSERT( it != m_sourceCodeToCodeMap.end() );
      }
      //else FABRIC_DEBUG_LOG( "Retrieved compiled code from cache" );
        
      return result;
    }
  };
};
