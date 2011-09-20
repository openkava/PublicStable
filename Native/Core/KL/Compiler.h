/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_CONTEXT_H
#define _FABRIC_KL_CONTEXT_H

#include <Fabric/Core/RT/Manager.h>

namespace Fabric
{
  namespace KL
  {
    class Compiler : public RT::Manager::KLCompiler
    {
    public:
    
      static RC::ConstHandle<Compiler> Create();
      
      virtual RC::ConstHandle<RC::Object> compile( std::string const &klFilename, std::string const &klSource ) const;

    protected:
    
      Compiler();
      
    private:
    
    };
  }
}

#endif //_FABRIC_KL_CONTEXT_H
