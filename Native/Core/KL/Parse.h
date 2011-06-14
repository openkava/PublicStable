// Parse.h
// *****************************************************************************
// (C) 2011 Fabric Technologies, Inc.

#ifndef __FABRIC_PARSE_H__
#define __FABRIC_PARSE_H__

#include <Fabric/Core/KL/Source.h>
#include <Fabric/Core/AST/GlobalList.h>
#include <Fabric/Core/CG/Diagnostics.h>
#include <Fabric/Core/CG/Manager.h>

namespace Fabric
{
  namespace KL
  {
    void Parse( 
      Source &source, 
      RC::Handle<CG::Manager> const &cgManager, 
      CG::Diagnostics &diagnostics,
      Fabric::RC::Handle<Fabric::AST::GlobalList> *result,
      bool debugParse = false
      );
  }
}

#endif //__FABRIC_PARSE_H__
