/*
 *
 *  Created by Peter Zion on 10-08-19.
 *  Copyright 2010 Fabric 3D Inc.. All rights reserved.
 *
 */

#ifndef _FABRIC_OGL_H
#define _FABRIC_OGL_H

#include <Fabric/Base/Config.h>

#if defined(FABRIC_OS_LINUX)
# include <GL/glew.h>
# include <GL/gl.h>
# include <GL/glx.h>
# include <GL/glext.h>
# include <GL/glu.h>
# undef None
# undef True
# undef False
#elif defined(FABRIC_OS_MACOSX)
# include <OpenGL/OpenGL.h>
#elif defined(FABRIC_OS_NACL)
# include <GLES2/gl2.h>
#elif defined(FABRIC_OS_WINDOWS)
# include <GL/glew.h>
#endif

#include <Fabric/Base/RC/Handle.h>

namespace Fabric
{
  namespace RT
  {
    class Manager;
  };
  
  namespace CG
  {
    class ModuleBuilder;
  };
  
  namespace OGL
  {
    void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, RC::Handle<RT::Manager> const &rtManager );
    void *llvmResolveExternalFunction( std::string const &name );
  };
};

#endif //_FABRIC_OGL_H
