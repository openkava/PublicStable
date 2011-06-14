/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/CaseList.h>
#include <Fabric/Core/AST/Case.h>
#include <Fabric/Core/Util/Assert.h>

namespace Fabric
{
  namespace AST
  {
    RC::Handle<CaseList> CaseList::Create( CG::Location const &location )
    {
      std::vector< RC::ConstHandle<Case> > cases;
      return new CaseList( location, cases );
    }
    
    RC::Handle<CaseList> CaseList::Create( CG::Location const &location, RC::ConstHandle<Case> const &case_ )
    {
      std::vector< RC::ConstHandle<Case> > cases;
      cases.push_back( case_ );
      return new CaseList( location, cases );
    }
    
    RC::Handle<CaseList> CaseList::Create( CG::Location const &location, RC::ConstHandle<Case> const &case_, RC::ConstHandle<CaseList> const &caseList )
    {
      std::vector< RC::ConstHandle<Case> > cases;
      cases.push_back( case_ );
      cases.insert( cases.end(), caseList->m_cases.begin(), caseList->m_cases.end() );
      return new CaseList( location, cases );
    }
  
    CaseList::CaseList( CG::Location const &location, std::vector< RC::ConstHandle<Case> > const &cases )
      : Node( location )
      , m_cases( cases )
    {
    }
    
    std::string CaseList::localDesc() const
    {
      return "CaseList";
    }
    
    std::string CaseList::deepDesc( std::string const &indent ) const
    {
      std::string result = indent + localDesc() + "\n";
      std::string subIndent = indent + "  ";
      for ( size_t i=0; i<m_cases.size(); ++i )
        result += m_cases[i]->deepDesc( subIndent );
      return result;
    }
    
    size_t CaseList::getNumCases() const
    {
      return m_cases.size();
    }
    
    RC::ConstHandle<Case> CaseList::getCase( size_t index ) const
    {
      FABRIC_ASSERT( index < m_cases.size() );
      return m_cases[index];
    }
  };
};
