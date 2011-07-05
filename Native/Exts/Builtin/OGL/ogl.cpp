/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

using namespace Fabric::EDK;

#include <GL/glew.h>
#include <GL/glxew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <GL/glu.h>

FABRIC_EXT_EXPORT KL::String klGetString(
  KL::Size name
){
  const GLubyte* result = glGetString( (GLenum)name );
  GLenum error = glGetError();
  if ( error != GL_NO_ERROR )
    throw Fabric::Exception( "Fabric::OGL::glGetString( %d )", (int)name);
  return (KL::String)(const char*)result;
}

FABRIC_EXT_EXPORT void klGetNamedProgramStringEXT(
  KL::Size program,
  KL::Size target,
  KL::Size pname,
  KL::Data string
){
  glGetNamedProgramStringEXT( (GLuint)program, (GLenum)target, (GLenum)pname, string );
  GLenum error = glGetError();
  if ( error != GL_NO_ERROR )
    throw Fabric::Exception( "Fabric::OGL::glGetNamedProgramStringEXT( 0x%04X, %d, %d, void* )", (unsigned)program, (int)target, (int)pname);
}

FABRIC_EXT_EXPORT void klNamedProgramStringEXT(
  KL::Size program,
  KL::Size target,
  KL::Size format,
  KL::Size len,
  KL::Data string
){
  glNamedProgramStringEXT( (GLuint)program, (GLenum)target, (GLenum)format, (GLsizei)len, string );
  GLenum error = glGetError();
  if ( error != GL_NO_ERROR )
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramStringEXT( 0x%04X, %d, %d, %d, void* )", (unsigned)program, (int)target, (int)format, (int)len);
}

FABRIC_EXT_EXPORT KL::String kluErrorString(
  KL::Size error
){
  const GLubyte* result = gluErrorString( (GLenum)error );
  return (KL::String)(const char*)result;
}

FABRIC_EXT_EXPORT KL::String kluGetString(
  KL::Size name
){
  const GLubyte* result = gluGetString( (GLenum)name );
  GLenum error = glGetError();
  if ( error != GL_NO_ERROR )
    throw Fabric::Exception( "Fabric::OGL::gluGetString( %d )", (int)name);
  return (KL::String)(const char*)result;
}

