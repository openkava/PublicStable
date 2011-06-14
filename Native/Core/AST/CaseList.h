/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_CASE_LIST_NODE_H
#define _FABRIC_AST_CASE_LIST_NODE_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Base/RC/Handle.h>

#include <vector>

namespace Fabric
{
  namespace AST
  {
    class Case;
    
    class CaseList : public Node
    {
    public:

      static RC::Handle<CaseList> Create( CG::Location const &location );
      static RC::Handle<CaseList> Create( CG::Location const &location, RC::ConstHandle<Case> const &case_ );
      static RC::Handle<CaseList> Create( CG::Location const &location, RC::ConstHandle<Case> const &case_, RC::ConstHandle<CaseList> const &caseList );
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;
      
      size_t getNumCases() const;
      RC::ConstHandle<Case> getCase( size_t index ) const;
      
    protected:
    
      CaseList( CG::Location const &location, std::vector< RC::ConstHandle<Case> > const &cases );
    
    private:
    
      std::vector< RC::ConstHandle<Case> > m_cases;
    };
  };
};

#endif //_FABRIC_AST_CASE_LIST_NODE_H
