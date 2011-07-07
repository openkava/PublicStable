/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

// include the EDK first, since it provides the OS based defines
#include <Fabric/EDK/EDK.h>
 
#include <GL/glew.h>
#if defined(FABRIC_OS_LINUX)
# include <GL/glxew.h>
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
#endif

using namespace Fabric::EDK;

// #define FABRIC_OGL_DEBUG 1

unsigned int gBracket = 0;
bool gGlewInitialized = false;
void _incBracket()
{
  gBracket++;
}
void _decBracket()
{
  if(gBracket > 0)
    gBracket--;
  else
    throw("Fabric::OGL::_decBracket: Unbalanced bracket.");
}
void _clearError()
{
  if(!gGlewInitialized){
#ifdef FABRIC_OGL_DEBUG
    printf("glewInit();\n");
#endif
    GLenum error = glewInit();
    if(error != GLEW_OK)
    {
      printf("[Fabric::OGL::glewInit] ERROR: %s\n",(const char*)gluErrorString(error));
      throw("[Fabric::OGL::glewInit] ERROR: %s",(const char*)gluErrorString(error));
    }
    gGlewInitialized = true;
  }

  // throw away last error
  if(gBracket > 0)
    return;
  glGetError();
}

void _checkError(const char * functionName)
{
  if(gBracket >0)
    return;
  GLenum error = glGetError();
  if(error != GL_NO_ERROR)
  {
    printf("[Fabric::OGL::%s] ERROR: %s\n",functionName,(const char*)gluErrorString(error));
    throw("[Fabric::OGL::%s] ERROR: %s",functionName,(const char*)gluErrorString(error));
  }
}

FABRIC_EXT_EXPORT void glAccum_wrapper(
  KL::Size op,
  KL::Scalar value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glAccum( %d, %f );\n", (int)op, (float)value);
#endif
  glAccum( (GLenum)op, (GLfloat)value );
  _checkError("glAccum");
}

FABRIC_EXT_EXPORT void glAlphaFunc_wrapper(
  KL::Size func,
  KL::Scalar ref
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glAlphaFunc( %d, %f );\n", (int)func, (float)ref);
#endif
  glAlphaFunc( (GLenum)func, (GLclampf)ref );
  _checkError("glAlphaFunc");
}

FABRIC_EXT_EXPORT KL::Boolean glAreTexturesResident_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & textures,
  KL::VariableArray<KL::Boolean> & residences
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glAreTexturesResident( %d, GLuint*, GLboolean* );\n", (int)n);
#endif
  GLboolean result = glAreTexturesResident( (GLsizei)n, (const GLuint*)&textures[0], (GLboolean*)&residences[0] );
  _checkError("glAreTexturesResident");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glArrayElement_wrapper(
  KL::Integer i
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glArrayElement( %d );\n", (int)i);
#endif
  glArrayElement( (GLint)i );
  _checkError("glArrayElement");
}

FABRIC_EXT_EXPORT void glBegin_wrapper(
  KL::Size mode
){
  _clearError();
  _incBracket();
#ifdef FABRIC_OGL_DEBUG
  printf("glBegin( %d );\n", (int)mode);
#endif
  glBegin( (GLenum)mode );
  _checkError("glBegin");
}

FABRIC_EXT_EXPORT void glBindTexture_wrapper(
  KL::Size target,
  KL::Size texture
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindTexture( %d, 0x%04X );\n", (int)target, (unsigned)texture);
#endif
  glBindTexture( (GLenum)target, (GLuint)texture );
  _checkError("glBindTexture");
}

FABRIC_EXT_EXPORT void glBitmap_wrapper(
  KL::Size width,
  KL::Size height,
  KL::Scalar xorig,
  KL::Scalar yorig,
  KL::Scalar xmove,
  KL::Scalar ymove,
  const KL::VariableArray<KL::Size> & bitmap
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBitmap( %d, %d, %f, %f, %f, %f, GLubyte* );\n", (int)width, (int)height, (float)xorig, (float)yorig, (float)xmove, (float)ymove);
#endif
  glBitmap( (GLsizei)width, (GLsizei)height, (GLfloat)xorig, (GLfloat)yorig, (GLfloat)xmove, (GLfloat)ymove, (const GLubyte*)&bitmap[0] );
  _checkError("glBitmap");
}

FABRIC_EXT_EXPORT void glBlendFunc_wrapper(
  KL::Size sfactor,
  KL::Size dfactor
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendFunc( %d, %d );\n", (int)sfactor, (int)dfactor);
#endif
  glBlendFunc( (GLenum)sfactor, (GLenum)dfactor );
  _checkError("glBlendFunc");
}

FABRIC_EXT_EXPORT void glCallList_wrapper(
  KL::Size list
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCallList( 0x%04X );\n", (unsigned)list);
#endif
  glCallList( (GLuint)list );
  _checkError("glCallList");
}

FABRIC_EXT_EXPORT void glCallLists_wrapper(
  KL::Size n,
  KL::Size type,
  KL::Data lists
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCallLists( %d, %d, GLvoid* );\n", (int)n, (int)type);
#endif
  glCallLists( (GLsizei)n, (GLenum)type, lists );
  _checkError("glCallLists");
}

FABRIC_EXT_EXPORT void glClear_wrapper(
  KL::Size mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClear( 0x%04X );\n", (unsigned)mask);
#endif
  glClear( (GLbitfield)mask );
  _checkError("glClear");
}

FABRIC_EXT_EXPORT void glClearAccum_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue,
  KL::Scalar alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClearAccum( %f, %f, %f, %f );\n", (float)red, (float)green, (float)blue, (float)alpha);
#endif
  glClearAccum( (GLfloat)red, (GLfloat)green, (GLfloat)blue, (GLfloat)alpha );
  _checkError("glClearAccum");
}

FABRIC_EXT_EXPORT void glClearColor_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue,
  KL::Scalar alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClearColor( %f, %f, %f, %f );\n", (float)red, (float)green, (float)blue, (float)alpha);
#endif
  glClearColor( (GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha );
  _checkError("glClearColor");
}

FABRIC_EXT_EXPORT void glClearDepth_wrapper(
  KL::Scalar depth
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClearDepth( %f );\n", (float)depth);
#endif
  glClearDepth( (GLclampd)depth );
  _checkError("glClearDepth");
}

FABRIC_EXT_EXPORT void glClearIndex_wrapper(
  KL::Scalar c
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClearIndex( %f );\n", (float)c);
#endif
  glClearIndex( (GLfloat)c );
  _checkError("glClearIndex");
}

FABRIC_EXT_EXPORT void glClearStencil_wrapper(
  KL::Integer s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClearStencil( %d );\n", (int)s);
#endif
  glClearStencil( (GLint)s );
  _checkError("glClearStencil");
}

FABRIC_EXT_EXPORT void glClipPlane_wrapper(
  KL::Size plane,
  const KL::VariableArray<KL::Scalar> & equation
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClipPlane( %d, GLdouble* );\n", (int)plane);
#endif
  glClipPlane( (GLenum)plane, (const GLdouble*)&equation[0] );
  _checkError("glClipPlane");
}

FABRIC_EXT_EXPORT void glColor3b_wrapper(
  KL::Byte red,
  KL::Byte green,
  KL::Byte blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3b( %d, %d, %d );\n", (int)red, (int)green, (int)blue);
#endif
  glColor3b( (GLbyte)red, (GLbyte)green, (GLbyte)blue );
  _checkError("glColor3b");
}

FABRIC_EXT_EXPORT void glColor3bv_wrapper(
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3bv( GLbyte* );\n");
#endif
  glColor3bv( (const GLbyte*)&v[0] );
  _checkError("glColor3bv");
}

FABRIC_EXT_EXPORT void glColor3d_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3d( %f, %f, %f );\n", (float)red, (float)green, (float)blue);
#endif
  glColor3d( (GLdouble)red, (GLdouble)green, (GLdouble)blue );
  _checkError("glColor3d");
}

FABRIC_EXT_EXPORT void glColor3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3dv( GLdouble* );\n");
#endif
  glColor3dv( (const GLdouble*)&v[0] );
  _checkError("glColor3dv");
}

FABRIC_EXT_EXPORT void glColor3f_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3f( %f, %f, %f );\n", (float)red, (float)green, (float)blue);
#endif
  glColor3f( (GLfloat)red, (GLfloat)green, (GLfloat)blue );
  _checkError("glColor3f");
}

FABRIC_EXT_EXPORT void glColor3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3fv( GLfloat* );\n");
#endif
  glColor3fv( (const GLfloat*)&v[0] );
  _checkError("glColor3fv");
}

FABRIC_EXT_EXPORT void glColor3i_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3i( %d, %d, %d );\n", (int)red, (int)green, (int)blue);
#endif
  glColor3i( (GLint)red, (GLint)green, (GLint)blue );
  _checkError("glColor3i");
}

FABRIC_EXT_EXPORT void glColor3iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3iv( GLint* );\n");
#endif
  glColor3iv( (const GLint*)&v[0] );
  _checkError("glColor3iv");
}

FABRIC_EXT_EXPORT void glColor3s_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3s( %d, %d, %d );\n", (int)red, (int)green, (int)blue);
#endif
  glColor3s( (GLshort)red, (GLshort)green, (GLshort)blue );
  _checkError("glColor3s");
}

FABRIC_EXT_EXPORT void glColor3sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3sv( GLshort* );\n");
#endif
  glColor3sv( (const GLshort*)&v[0] );
  _checkError("glColor3sv");
}

FABRIC_EXT_EXPORT void glColor3ub_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3ub( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue);
#endif
  glColor3ub( (GLubyte)red, (GLubyte)green, (GLubyte)blue );
  _checkError("glColor3ub");
}

FABRIC_EXT_EXPORT void glColor3ubv_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3ubv( GLubyte* );\n");
#endif
  glColor3ubv( (const GLubyte*)&v[0] );
  _checkError("glColor3ubv");
}

FABRIC_EXT_EXPORT void glColor3ui_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3ui( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue);
#endif
  glColor3ui( (GLuint)red, (GLuint)green, (GLuint)blue );
  _checkError("glColor3ui");
}

FABRIC_EXT_EXPORT void glColor3uiv_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3uiv( GLuint* );\n");
#endif
  glColor3uiv( (const GLuint*)&v[0] );
  _checkError("glColor3uiv");
}

FABRIC_EXT_EXPORT void glColor3us_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3us( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue);
#endif
  glColor3us( (GLushort)red, (GLushort)green, (GLushort)blue );
  _checkError("glColor3us");
}

FABRIC_EXT_EXPORT void glColor3usv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor3usv( GLushort* );\n");
#endif
  glColor3usv( (const GLushort*)&v[0] );
  _checkError("glColor3usv");
}

FABRIC_EXT_EXPORT void glColor4b_wrapper(
  KL::Byte red,
  KL::Byte green,
  KL::Byte blue,
  KL::Byte alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4b( %d, %d, %d, %d );\n", (int)red, (int)green, (int)blue, (int)alpha);
#endif
  glColor4b( (GLbyte)red, (GLbyte)green, (GLbyte)blue, (GLbyte)alpha );
  _checkError("glColor4b");
}

FABRIC_EXT_EXPORT void glColor4bv_wrapper(
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4bv( GLbyte* );\n");
#endif
  glColor4bv( (const GLbyte*)&v[0] );
  _checkError("glColor4bv");
}

FABRIC_EXT_EXPORT void glColor4d_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue,
  KL::Scalar alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4d( %f, %f, %f, %f );\n", (float)red, (float)green, (float)blue, (float)alpha);
#endif
  glColor4d( (GLdouble)red, (GLdouble)green, (GLdouble)blue, (GLdouble)alpha );
  _checkError("glColor4d");
}

FABRIC_EXT_EXPORT void glColor4dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4dv( GLdouble* );\n");
#endif
  glColor4dv( (const GLdouble*)&v[0] );
  _checkError("glColor4dv");
}

FABRIC_EXT_EXPORT void glColor4f_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue,
  KL::Scalar alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4f( %f, %f, %f, %f );\n", (float)red, (float)green, (float)blue, (float)alpha);
#endif
  glColor4f( (GLfloat)red, (GLfloat)green, (GLfloat)blue, (GLfloat)alpha );
  _checkError("glColor4f");
}

FABRIC_EXT_EXPORT void glColor4fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4fv( GLfloat* );\n");
#endif
  glColor4fv( (const GLfloat*)&v[0] );
  _checkError("glColor4fv");
}

FABRIC_EXT_EXPORT void glColor4i_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue,
  KL::Integer alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4i( %d, %d, %d, %d );\n", (int)red, (int)green, (int)blue, (int)alpha);
#endif
  glColor4i( (GLint)red, (GLint)green, (GLint)blue, (GLint)alpha );
  _checkError("glColor4i");
}

FABRIC_EXT_EXPORT void glColor4iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4iv( GLint* );\n");
#endif
  glColor4iv( (const GLint*)&v[0] );
  _checkError("glColor4iv");
}

FABRIC_EXT_EXPORT void glColor4s_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue,
  KL::Integer alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4s( %d, %d, %d, %d );\n", (int)red, (int)green, (int)blue, (int)alpha);
#endif
  glColor4s( (GLshort)red, (GLshort)green, (GLshort)blue, (GLshort)alpha );
  _checkError("glColor4s");
}

FABRIC_EXT_EXPORT void glColor4sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4sv( GLshort* );\n");
#endif
  glColor4sv( (const GLshort*)&v[0] );
  _checkError("glColor4sv");
}

FABRIC_EXT_EXPORT void glColor4ub_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue,
  KL::Size alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4ub( 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue, (unsigned)alpha);
#endif
  glColor4ub( (GLubyte)red, (GLubyte)green, (GLubyte)blue, (GLubyte)alpha );
  _checkError("glColor4ub");
}

FABRIC_EXT_EXPORT void glColor4ubv_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4ubv( GLubyte* );\n");
#endif
  glColor4ubv( (const GLubyte*)&v[0] );
  _checkError("glColor4ubv");
}

FABRIC_EXT_EXPORT void glColor4ui_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue,
  KL::Size alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4ui( 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue, (unsigned)alpha);
#endif
  glColor4ui( (GLuint)red, (GLuint)green, (GLuint)blue, (GLuint)alpha );
  _checkError("glColor4ui");
}

FABRIC_EXT_EXPORT void glColor4uiv_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4uiv( GLuint* );\n");
#endif
  glColor4uiv( (const GLuint*)&v[0] );
  _checkError("glColor4uiv");
}

FABRIC_EXT_EXPORT void glColor4us_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue,
  KL::Integer alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4us( 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue, (unsigned)alpha);
#endif
  glColor4us( (GLushort)red, (GLushort)green, (GLushort)blue, (GLushort)alpha );
  _checkError("glColor4us");
}

FABRIC_EXT_EXPORT void glColor4usv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColor4usv( GLushort* );\n");
#endif
  glColor4usv( (const GLushort*)&v[0] );
  _checkError("glColor4usv");
}

FABRIC_EXT_EXPORT void glColorMask_wrapper(
  KL::Boolean red,
  KL::Boolean green,
  KL::Boolean blue,
  KL::Boolean alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColorMask( %d, %d, %d, %d );\n", (int)red, (int)green, (int)blue, (int)alpha);
#endif
  glColorMask( (GLboolean)red, (GLboolean)green, (GLboolean)blue, (GLboolean)alpha );
  _checkError("glColorMask");
}

FABRIC_EXT_EXPORT void glColorMaterial_wrapper(
  KL::Size face,
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColorMaterial( %d, %d );\n", (int)face, (int)mode);
#endif
  glColorMaterial( (GLenum)face, (GLenum)mode );
  _checkError("glColorMaterial");
}

FABRIC_EXT_EXPORT void glColorPointer_wrapper(
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColorPointer( %d, %d, %d, GLvoid* );\n", (int)size, (int)type, (int)stride);
#endif
  glColorPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glColorPointer");
}

FABRIC_EXT_EXPORT void glCopyPixels_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height,
  KL::Size type
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyPixels( %d, %d, %d, %d, %d );\n", (int)x, (int)y, (int)width, (int)height, (int)type);
#endif
  glCopyPixels( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLenum)type );
  _checkError("glCopyPixels");
}

FABRIC_EXT_EXPORT void glCopyTexImage1D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size internalFormat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Integer border
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTexImage1D( %d, %d, %d, %d, %d, %d, %d );\n", (int)target, (int)level, (int)internalFormat, (int)x, (int)y, (int)width, (int)border);
#endif
  glCopyTexImage1D( (GLenum)target, (GLint)level, (GLenum)internalFormat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border );
  _checkError("glCopyTexImage1D");
}

FABRIC_EXT_EXPORT void glCopyTexImage2D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size internalFormat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height,
  KL::Integer border
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTexImage2D( %d, %d, %d, %d, %d, %d, %d, %d );\n", (int)target, (int)level, (int)internalFormat, (int)x, (int)y, (int)width, (int)height, (int)border);
#endif
  glCopyTexImage2D( (GLenum)target, (GLint)level, (GLenum)internalFormat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border );
  _checkError("glCopyTexImage2D");
}

FABRIC_EXT_EXPORT void glCopyTexSubImage1D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer x,
  KL::Integer y,
  KL::Size width
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTexSubImage1D( %d, %d, %d, %d, %d, %d );\n", (int)target, (int)level, (int)xoffset, (int)x, (int)y, (int)width);
#endif
  glCopyTexSubImage1D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width );
  _checkError("glCopyTexSubImage1D");
}

FABRIC_EXT_EXPORT void glCopyTexSubImage2D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTexSubImage2D( %d, %d, %d, %d, %d, %d, %d, %d );\n", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)x, (int)y, (int)width, (int)height);
#endif
  glCopyTexSubImage2D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyTexSubImage2D");
}

FABRIC_EXT_EXPORT void glCullFace_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCullFace( %d );\n", (int)mode);
#endif
  glCullFace( (GLenum)mode );
  _checkError("glCullFace");
}

FABRIC_EXT_EXPORT void glDeleteLists_wrapper(
  KL::Size list,
  KL::Size range
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteLists( 0x%04X, %d );\n", (unsigned)list, (int)range);
#endif
  glDeleteLists( (GLuint)list, (GLsizei)range );
  _checkError("glDeleteLists");
}

FABRIC_EXT_EXPORT void glDeleteTextures_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & textures
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteTextures( %d, GLuint* );\n", (int)n);
#endif
  glDeleteTextures( (GLsizei)n, (const GLuint*)&textures[0] );
  _checkError("glDeleteTextures");
}

FABRIC_EXT_EXPORT void glDepthFunc_wrapper(
  KL::Size func
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDepthFunc( %d );\n", (int)func);
#endif
  glDepthFunc( (GLenum)func );
  _checkError("glDepthFunc");
}

FABRIC_EXT_EXPORT void glDepthMask_wrapper(
  KL::Boolean flag
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDepthMask( %d );\n", (int)flag);
#endif
  glDepthMask( (GLboolean)flag );
  _checkError("glDepthMask");
}

FABRIC_EXT_EXPORT void glDepthRange_wrapper(
  KL::Scalar zNear,
  KL::Scalar zFar
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDepthRange( %f, %f );\n", (float)zNear, (float)zFar);
#endif
  glDepthRange( (GLclampd)zNear, (GLclampd)zFar );
  _checkError("glDepthRange");
}

FABRIC_EXT_EXPORT void glDisable_wrapper(
  KL::Size cap
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDisable( %d );\n", (int)cap);
#endif
  glDisable( (GLenum)cap );
  _checkError("glDisable");
}

FABRIC_EXT_EXPORT void glDisableClientState_wrapper(
  KL::Size array
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDisableClientState( %d );\n", (int)array);
#endif
  glDisableClientState( (GLenum)array );
  _checkError("glDisableClientState");
}

FABRIC_EXT_EXPORT void glDrawArrays_wrapper(
  KL::Size mode,
  KL::Integer first,
  KL::Size count
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawArrays( %d, %d, %d );\n", (int)mode, (int)first, (int)count);
#endif
  glDrawArrays( (GLenum)mode, (GLint)first, (GLsizei)count );
  _checkError("glDrawArrays");
}

FABRIC_EXT_EXPORT void glDrawBuffer_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawBuffer( %d );\n", (int)mode);
#endif
  glDrawBuffer( (GLenum)mode );
  _checkError("glDrawBuffer");
}

FABRIC_EXT_EXPORT void glDrawElements_wrapper(
  KL::Size mode,
  KL::Size count,
  KL::Size type,
  KL::Data indices
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawElements( %d, %d, %d, GLvoid* );\n", (int)mode, (int)count, (int)type);
#endif
  glDrawElements( (GLenum)mode, (GLsizei)count, (GLenum)type, indices );
  _checkError("glDrawElements");
}

FABRIC_EXT_EXPORT void glDrawPixels_wrapper(
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawPixels( %d, %d, %d, %d, GLvoid* );\n", (int)width, (int)height, (int)format, (int)type);
#endif
  glDrawPixels( (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  _checkError("glDrawPixels");
}

FABRIC_EXT_EXPORT void glEdgeFlag_wrapper(
  KL::Boolean flag
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEdgeFlag( %d );\n", (int)flag);
#endif
  glEdgeFlag( (GLboolean)flag );
  _checkError("glEdgeFlag");
}

FABRIC_EXT_EXPORT void glEdgeFlagPointer_wrapper(
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEdgeFlagPointer( %d, GLvoid* );\n", (int)stride);
#endif
  glEdgeFlagPointer( (GLsizei)stride, pointer );
  _checkError("glEdgeFlagPointer");
}

FABRIC_EXT_EXPORT void glEdgeFlagv_wrapper(
  const KL::VariableArray<KL::Boolean> & flag
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEdgeFlagv( GLboolean* );\n");
#endif
  glEdgeFlagv( (const GLboolean*)&flag[0] );
  _checkError("glEdgeFlagv");
}

FABRIC_EXT_EXPORT void glEnable_wrapper(
  KL::Size cap
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEnable( %d );\n", (int)cap);
#endif
  glEnable( (GLenum)cap );
  _checkError("glEnable");
}

FABRIC_EXT_EXPORT void glEnableClientState_wrapper(
  KL::Size array
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEnableClientState( %d );\n", (int)array);
#endif
  glEnableClientState( (GLenum)array );
  _checkError("glEnableClientState");
}

FABRIC_EXT_EXPORT void glEnd_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEnd(  );\n");
#endif
  glEnd();
  _decBracket();
  _checkError("glEnd");
}

FABRIC_EXT_EXPORT void glEndList_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEndList(  );\n");
#endif
  glEndList();
  _checkError("glEndList");
}

FABRIC_EXT_EXPORT void glEvalCoord1d_wrapper(
  KL::Scalar u
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEvalCoord1d( %f );\n", (float)u);
#endif
  glEvalCoord1d( (GLdouble)u );
  _checkError("glEvalCoord1d");
}

FABRIC_EXT_EXPORT void glEvalCoord1dv_wrapper(
  const KL::VariableArray<KL::Scalar> & u
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEvalCoord1dv( GLdouble* );\n");
#endif
  glEvalCoord1dv( (const GLdouble*)&u[0] );
  _checkError("glEvalCoord1dv");
}

FABRIC_EXT_EXPORT void glEvalCoord1f_wrapper(
  KL::Scalar u
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEvalCoord1f( %f );\n", (float)u);
#endif
  glEvalCoord1f( (GLfloat)u );
  _checkError("glEvalCoord1f");
}

FABRIC_EXT_EXPORT void glEvalCoord1fv_wrapper(
  const KL::VariableArray<KL::Scalar> & u
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEvalCoord1fv( GLfloat* );\n");
#endif
  glEvalCoord1fv( (const GLfloat*)&u[0] );
  _checkError("glEvalCoord1fv");
}

FABRIC_EXT_EXPORT void glEvalCoord2d_wrapper(
  KL::Scalar u,
  KL::Scalar v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEvalCoord2d( %f, %f );\n", (float)u, (float)v);
#endif
  glEvalCoord2d( (GLdouble)u, (GLdouble)v );
  _checkError("glEvalCoord2d");
}

FABRIC_EXT_EXPORT void glEvalCoord2dv_wrapper(
  const KL::VariableArray<KL::Scalar> & u
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEvalCoord2dv( GLdouble* );\n");
#endif
  glEvalCoord2dv( (const GLdouble*)&u[0] );
  _checkError("glEvalCoord2dv");
}

FABRIC_EXT_EXPORT void glEvalCoord2f_wrapper(
  KL::Scalar u,
  KL::Scalar v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEvalCoord2f( %f, %f );\n", (float)u, (float)v);
#endif
  glEvalCoord2f( (GLfloat)u, (GLfloat)v );
  _checkError("glEvalCoord2f");
}

FABRIC_EXT_EXPORT void glEvalCoord2fv_wrapper(
  const KL::VariableArray<KL::Scalar> & u
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEvalCoord2fv( GLfloat* );\n");
#endif
  glEvalCoord2fv( (const GLfloat*)&u[0] );
  _checkError("glEvalCoord2fv");
}

FABRIC_EXT_EXPORT void glEvalMesh1_wrapper(
  KL::Size mode,
  KL::Integer i1,
  KL::Integer i2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEvalMesh1( %d, %d, %d );\n", (int)mode, (int)i1, (int)i2);
#endif
  glEvalMesh1( (GLenum)mode, (GLint)i1, (GLint)i2 );
  _checkError("glEvalMesh1");
}

FABRIC_EXT_EXPORT void glEvalMesh2_wrapper(
  KL::Size mode,
  KL::Integer i1,
  KL::Integer i2,
  KL::Integer j1,
  KL::Integer j2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEvalMesh2( %d, %d, %d, %d, %d );\n", (int)mode, (int)i1, (int)i2, (int)j1, (int)j2);
#endif
  glEvalMesh2( (GLenum)mode, (GLint)i1, (GLint)i2, (GLint)j1, (GLint)j2 );
  _checkError("glEvalMesh2");
}

FABRIC_EXT_EXPORT void glEvalPoint1_wrapper(
  KL::Integer i
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEvalPoint1( %d );\n", (int)i);
#endif
  glEvalPoint1( (GLint)i );
  _checkError("glEvalPoint1");
}

FABRIC_EXT_EXPORT void glEvalPoint2_wrapper(
  KL::Integer i,
  KL::Integer j
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEvalPoint2( %d, %d );\n", (int)i, (int)j);
#endif
  glEvalPoint2( (GLint)i, (GLint)j );
  _checkError("glEvalPoint2");
}

FABRIC_EXT_EXPORT void glFeedbackBuffer_wrapper(
  KL::Size size,
  KL::Size type,
  KL::VariableArray<KL::Scalar> & buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFeedbackBuffer( %d, %d, GLfloat* );\n", (int)size, (int)type);
#endif
  glFeedbackBuffer( (GLsizei)size, (GLenum)type, (GLfloat*)&buffer[0] );
  _checkError("glFeedbackBuffer");
}

FABRIC_EXT_EXPORT void glFinish_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFinish(  );\n");
#endif
  glFinish();
  _checkError("glFinish");
}

FABRIC_EXT_EXPORT void glFlush_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFlush(  );\n");
#endif
  glFlush();
  _checkError("glFlush");
}

FABRIC_EXT_EXPORT void glFogf_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogf( %d, %f );\n", (int)pname, (float)param);
#endif
  glFogf( (GLenum)pname, (GLfloat)param );
  _checkError("glFogf");
}

FABRIC_EXT_EXPORT void glFogfv_wrapper(
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogfv( %d, GLfloat* );\n", (int)pname);
#endif
  glFogfv( (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glFogfv");
}

FABRIC_EXT_EXPORT void glFogi_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogi( %d, %d );\n", (int)pname, (int)param);
#endif
  glFogi( (GLenum)pname, (GLint)param );
  _checkError("glFogi");
}

FABRIC_EXT_EXPORT void glFogiv_wrapper(
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogiv( %d, GLint* );\n", (int)pname);
#endif
  glFogiv( (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glFogiv");
}

FABRIC_EXT_EXPORT void glFrontFace_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFrontFace( %d );\n", (int)mode);
#endif
  glFrontFace( (GLenum)mode );
  _checkError("glFrontFace");
}

FABRIC_EXT_EXPORT void glFrustum_wrapper(
  KL::Scalar left,
  KL::Scalar right,
  KL::Scalar bottom,
  KL::Scalar top,
  KL::Scalar zNear,
  KL::Scalar zFar
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFrustum( %f, %f, %f, %f, %f, %f );\n", (float)left, (float)right, (float)bottom, (float)top, (float)zNear, (float)zFar);
#endif
  glFrustum( (GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top, (GLdouble)zNear, (GLdouble)zFar );
  _checkError("glFrustum");
}

FABRIC_EXT_EXPORT KL::Size glGenLists_wrapper(
  KL::Size range
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenLists( %d );\n", (int)range);
#endif
  GLuint result = glGenLists( (GLsizei)range );
  _checkError("glGenLists");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glGenTextures_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & textures
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenTextures( %d, GLuint* );\n", (int)n);
#endif
  glGenTextures( (GLsizei)n, (GLuint*)&textures[0] );
  _checkError("glGenTextures");
}

FABRIC_EXT_EXPORT void glGetBooleanv_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Boolean> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetBooleanv( %d, GLboolean* );\n", (int)pname);
#endif
  glGetBooleanv( (GLenum)pname, (GLboolean*)&params[0] );
  _checkError("glGetBooleanv");
}

FABRIC_EXT_EXPORT void glGetClipPlane_wrapper(
  KL::Size plane,
  KL::VariableArray<KL::Scalar> & equation
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetClipPlane( %d, GLdouble* );\n", (int)plane);
#endif
  glGetClipPlane( (GLenum)plane, (GLdouble*)&equation[0] );
  _checkError("glGetClipPlane");
}

FABRIC_EXT_EXPORT void glGetDoublev_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetDoublev( %d, GLdouble* );\n", (int)pname);
#endif
  glGetDoublev( (GLenum)pname, (GLdouble*)&params[0] );
  _checkError("glGetDoublev");
}

FABRIC_EXT_EXPORT KL::Size glGetError_wrapper()
{
#ifdef FABRIC_OGL_DEBUG
  printf("glGetError(  );\n");
#endif
  GLenum result = glGetError();
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glGetFloatv_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetFloatv( %d, GLfloat* );\n", (int)pname);
#endif
  glGetFloatv( (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetFloatv");
}

FABRIC_EXT_EXPORT void glGetIntegerv_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetIntegerv( %d, GLint* );\n", (int)pname);
#endif
  glGetIntegerv( (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetIntegerv");
}

FABRIC_EXT_EXPORT void glGetLightfv_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetLightfv( %d, %d, GLfloat* );\n", (int)light, (int)pname);
#endif
  glGetLightfv( (GLenum)light, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetLightfv");
}

FABRIC_EXT_EXPORT void glGetLightiv_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetLightiv( %d, %d, GLint* );\n", (int)light, (int)pname);
#endif
  glGetLightiv( (GLenum)light, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetLightiv");
}

FABRIC_EXT_EXPORT void glGetMapdv_wrapper(
  KL::Size target,
  KL::Size query,
  KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMapdv( %d, %d, GLdouble* );\n", (int)target, (int)query);
#endif
  glGetMapdv( (GLenum)target, (GLenum)query, (GLdouble*)&v[0] );
  _checkError("glGetMapdv");
}

FABRIC_EXT_EXPORT void glGetMapfv_wrapper(
  KL::Size target,
  KL::Size query,
  KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMapfv( %d, %d, GLfloat* );\n", (int)target, (int)query);
#endif
  glGetMapfv( (GLenum)target, (GLenum)query, (GLfloat*)&v[0] );
  _checkError("glGetMapfv");
}

FABRIC_EXT_EXPORT void glGetMapiv_wrapper(
  KL::Size target,
  KL::Size query,
  KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMapiv( %d, %d, GLint* );\n", (int)target, (int)query);
#endif
  glGetMapiv( (GLenum)target, (GLenum)query, (GLint*)&v[0] );
  _checkError("glGetMapiv");
}

FABRIC_EXT_EXPORT void glGetMaterialfv_wrapper(
  KL::Size face,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMaterialfv( %d, %d, GLfloat* );\n", (int)face, (int)pname);
#endif
  glGetMaterialfv( (GLenum)face, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetMaterialfv");
}

FABRIC_EXT_EXPORT void glGetMaterialiv_wrapper(
  KL::Size face,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMaterialiv( %d, %d, GLint* );\n", (int)face, (int)pname);
#endif
  glGetMaterialiv( (GLenum)face, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetMaterialiv");
}

FABRIC_EXT_EXPORT void glGetPixelMapfv_wrapper(
  KL::Size map,
  KL::VariableArray<KL::Scalar> & values
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetPixelMapfv( %d, GLfloat* );\n", (int)map);
#endif
  glGetPixelMapfv( (GLenum)map, (GLfloat*)&values[0] );
  _checkError("glGetPixelMapfv");
}

FABRIC_EXT_EXPORT void glGetPixelMapuiv_wrapper(
  KL::Size map,
  KL::VariableArray<KL::Size> & values
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetPixelMapuiv( %d, GLuint* );\n", (int)map);
#endif
  glGetPixelMapuiv( (GLenum)map, (GLuint*)&values[0] );
  _checkError("glGetPixelMapuiv");
}

FABRIC_EXT_EXPORT void glGetPixelMapusv_wrapper(
  KL::Size map,
  KL::VariableArray<KL::Integer> & values
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetPixelMapusv( %d, GLushort* );\n", (int)map);
#endif
  glGetPixelMapusv( (GLenum)map, (GLushort*)&values[0] );
  _checkError("glGetPixelMapusv");
}

FABRIC_EXT_EXPORT void glGetPolygonStipple_wrapper(
  KL::VariableArray<KL::Size> & mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetPolygonStipple( GLubyte* );\n");
#endif
  glGetPolygonStipple( (GLubyte*)&mask[0] );
  _checkError("glGetPolygonStipple");
}

FABRIC_EXT_EXPORT KL::String glGetString_wrapper(
  KL::Size name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetString( %d );\n", (int)name);
#endif
  const GLubyte* result = glGetString( (GLenum)name );
  _checkError("glGetString");
  return (KL::String)(const char*)result;
}

FABRIC_EXT_EXPORT void glGetTexEnvfv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexEnvfv( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glGetTexEnvfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetTexEnvfv");
}

FABRIC_EXT_EXPORT void glGetTexEnviv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexEnviv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetTexEnviv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetTexEnviv");
}

FABRIC_EXT_EXPORT void glGetTexGendv_wrapper(
  KL::Size coord,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexGendv( %d, %d, GLdouble* );\n", (int)coord, (int)pname);
#endif
  glGetTexGendv( (GLenum)coord, (GLenum)pname, (GLdouble*)&params[0] );
  _checkError("glGetTexGendv");
}

FABRIC_EXT_EXPORT void glGetTexGenfv_wrapper(
  KL::Size coord,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexGenfv( %d, %d, GLfloat* );\n", (int)coord, (int)pname);
#endif
  glGetTexGenfv( (GLenum)coord, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetTexGenfv");
}

FABRIC_EXT_EXPORT void glGetTexGeniv_wrapper(
  KL::Size coord,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexGeniv( %d, %d, GLint* );\n", (int)coord, (int)pname);
#endif
  glGetTexGeniv( (GLenum)coord, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetTexGeniv");
}

FABRIC_EXT_EXPORT void glGetTexImage_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexImage( %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)format, (int)type);
#endif
  glGetTexImage( (GLenum)target, (GLint)level, (GLenum)format, (GLenum)type, pixels );
  _checkError("glGetTexImage");
}

FABRIC_EXT_EXPORT void glGetTexLevelParameterfv_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexLevelParameterfv( %d, %d, %d, GLfloat* );\n", (int)target, (int)level, (int)pname);
#endif
  glGetTexLevelParameterfv( (GLenum)target, (GLint)level, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetTexLevelParameterfv");
}

FABRIC_EXT_EXPORT void glGetTexLevelParameteriv_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexLevelParameteriv( %d, %d, %d, GLint* );\n", (int)target, (int)level, (int)pname);
#endif
  glGetTexLevelParameteriv( (GLenum)target, (GLint)level, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetTexLevelParameteriv");
}

FABRIC_EXT_EXPORT void glGetTexParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexParameterfv( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glGetTexParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetTexParameterfv");
}

FABRIC_EXT_EXPORT void glGetTexParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexParameteriv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetTexParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetTexParameteriv");
}

FABRIC_EXT_EXPORT void glHint_wrapper(
  KL::Size target,
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glHint( %d, %d );\n", (int)target, (int)mode);
#endif
  glHint( (GLenum)target, (GLenum)mode );
  _checkError("glHint");
}

FABRIC_EXT_EXPORT void glIndexMask_wrapper(
  KL::Size mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexMask( 0x%04X );\n", (unsigned)mask);
#endif
  glIndexMask( (GLuint)mask );
  _checkError("glIndexMask");
}

FABRIC_EXT_EXPORT void glIndexPointer_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexPointer( %d, %d, GLvoid* );\n", (int)type, (int)stride);
#endif
  glIndexPointer( (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glIndexPointer");
}

FABRIC_EXT_EXPORT void glIndexd_wrapper(
  KL::Scalar c
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexd( %f );\n", (float)c);
#endif
  glIndexd( (GLdouble)c );
  _checkError("glIndexd");
}

FABRIC_EXT_EXPORT void glIndexdv_wrapper(
  const KL::VariableArray<KL::Scalar> & c
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexdv( GLdouble* );\n");
#endif
  glIndexdv( (const GLdouble*)&c[0] );
  _checkError("glIndexdv");
}

FABRIC_EXT_EXPORT void glIndexf_wrapper(
  KL::Scalar c
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexf( %f );\n", (float)c);
#endif
  glIndexf( (GLfloat)c );
  _checkError("glIndexf");
}

FABRIC_EXT_EXPORT void glIndexfv_wrapper(
  const KL::VariableArray<KL::Scalar> & c
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexfv( GLfloat* );\n");
#endif
  glIndexfv( (const GLfloat*)&c[0] );
  _checkError("glIndexfv");
}

FABRIC_EXT_EXPORT void glIndexi_wrapper(
  KL::Integer c
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexi( %d );\n", (int)c);
#endif
  glIndexi( (GLint)c );
  _checkError("glIndexi");
}

FABRIC_EXT_EXPORT void glIndexiv_wrapper(
  const KL::VariableArray<KL::Integer> & c
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexiv( GLint* );\n");
#endif
  glIndexiv( (const GLint*)&c[0] );
  _checkError("glIndexiv");
}

FABRIC_EXT_EXPORT void glIndexs_wrapper(
  KL::Integer c
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexs( %d );\n", (int)c);
#endif
  glIndexs( (GLshort)c );
  _checkError("glIndexs");
}

FABRIC_EXT_EXPORT void glIndexsv_wrapper(
  const KL::VariableArray<KL::Integer> & c
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexsv( GLshort* );\n");
#endif
  glIndexsv( (const GLshort*)&c[0] );
  _checkError("glIndexsv");
}

FABRIC_EXT_EXPORT void glIndexub_wrapper(
  KL::Size c
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexub( 0x%04X );\n", (unsigned)c);
#endif
  glIndexub( (GLubyte)c );
  _checkError("glIndexub");
}

FABRIC_EXT_EXPORT void glIndexubv_wrapper(
  const KL::VariableArray<KL::Size> & c
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexubv( GLubyte* );\n");
#endif
  glIndexubv( (const GLubyte*)&c[0] );
  _checkError("glIndexubv");
}

FABRIC_EXT_EXPORT void glInitNames_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glInitNames(  );\n");
#endif
  glInitNames();
  _checkError("glInitNames");
}

FABRIC_EXT_EXPORT void glInterleavedArrays_wrapper(
  KL::Size format,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glInterleavedArrays( %d, %d, GLvoid* );\n", (int)format, (int)stride);
#endif
  glInterleavedArrays( (GLenum)format, (GLsizei)stride, pointer );
  _checkError("glInterleavedArrays");
}

FABRIC_EXT_EXPORT KL::Boolean glIsEnabled_wrapper(
  KL::Size cap
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsEnabled( %d );\n", (int)cap);
#endif
  GLboolean result = glIsEnabled( (GLenum)cap );
  _checkError("glIsEnabled");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glIsList_wrapper(
  KL::Size list
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsList( 0x%04X );\n", (unsigned)list);
#endif
  GLboolean result = glIsList( (GLuint)list );
  _checkError("glIsList");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glIsTexture_wrapper(
  KL::Size texture
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsTexture( 0x%04X );\n", (unsigned)texture);
#endif
  GLboolean result = glIsTexture( (GLuint)texture );
  _checkError("glIsTexture");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glLightModelf_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLightModelf( %d, %f );\n", (int)pname, (float)param);
#endif
  glLightModelf( (GLenum)pname, (GLfloat)param );
  _checkError("glLightModelf");
}

FABRIC_EXT_EXPORT void glLightModelfv_wrapper(
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLightModelfv( %d, GLfloat* );\n", (int)pname);
#endif
  glLightModelfv( (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glLightModelfv");
}

FABRIC_EXT_EXPORT void glLightModeli_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLightModeli( %d, %d );\n", (int)pname, (int)param);
#endif
  glLightModeli( (GLenum)pname, (GLint)param );
  _checkError("glLightModeli");
}

FABRIC_EXT_EXPORT void glLightModeliv_wrapper(
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLightModeliv( %d, GLint* );\n", (int)pname);
#endif
  glLightModeliv( (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glLightModeliv");
}

FABRIC_EXT_EXPORT void glLightf_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLightf( %d, %d, %f );\n", (int)light, (int)pname, (float)param);
#endif
  glLightf( (GLenum)light, (GLenum)pname, (GLfloat)param );
  _checkError("glLightf");
}

FABRIC_EXT_EXPORT void glLightfv_wrapper(
  KL::Size light,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLightfv( %d, %d, GLfloat* );\n", (int)light, (int)pname);
#endif
  glLightfv( (GLenum)light, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glLightfv");
}

FABRIC_EXT_EXPORT void glLighti_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLighti( %d, %d, %d );\n", (int)light, (int)pname, (int)param);
#endif
  glLighti( (GLenum)light, (GLenum)pname, (GLint)param );
  _checkError("glLighti");
}

FABRIC_EXT_EXPORT void glLightiv_wrapper(
  KL::Size light,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLightiv( %d, %d, GLint* );\n", (int)light, (int)pname);
#endif
  glLightiv( (GLenum)light, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glLightiv");
}

FABRIC_EXT_EXPORT void glLineStipple_wrapper(
  KL::Integer factor,
  KL::Integer pattern
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLineStipple( %d, 0x%04X );\n", (int)factor, (unsigned)pattern);
#endif
  glLineStipple( (GLint)factor, (GLushort)pattern );
  _checkError("glLineStipple");
}

FABRIC_EXT_EXPORT void glLineWidth_wrapper(
  KL::Scalar width
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLineWidth( %f );\n", (float)width);
#endif
  glLineWidth( (GLfloat)width );
  _checkError("glLineWidth");
}

FABRIC_EXT_EXPORT void glListBase_wrapper(
  KL::Size base
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glListBase( 0x%04X );\n", (unsigned)base);
#endif
  glListBase( (GLuint)base );
  _checkError("glListBase");
}

FABRIC_EXT_EXPORT void glLoadIdentity_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLoadIdentity(  );\n");
#endif
  glLoadIdentity();
  _checkError("glLoadIdentity");
}

FABRIC_EXT_EXPORT void glLoadMatrixd_wrapper(
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLoadMatrixd( GLdouble* );\n");
#endif
  glLoadMatrixd( (const GLdouble*)&m[0] );
  _checkError("glLoadMatrixd");
}

FABRIC_EXT_EXPORT void glLoadMatrixf_wrapper(
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLoadMatrixf( GLfloat* );\n");
#endif
  glLoadMatrixf( (const GLfloat*)&m[0] );
  _checkError("glLoadMatrixf");
}

FABRIC_EXT_EXPORT void glLoadName_wrapper(
  KL::Size name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLoadName( 0x%04X );\n", (unsigned)name);
#endif
  glLoadName( (GLuint)name );
  _checkError("glLoadName");
}

FABRIC_EXT_EXPORT void glLogicOp_wrapper(
  KL::Size opcode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLogicOp( %d );\n", (int)opcode);
#endif
  glLogicOp( (GLenum)opcode );
  _checkError("glLogicOp");
}

FABRIC_EXT_EXPORT void glMap1d_wrapper(
  KL::Size target,
  KL::Scalar u1,
  KL::Scalar u2,
  KL::Integer stride,
  KL::Integer order,
  const KL::VariableArray<KL::Scalar> & points
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMap1d( %d, %f, %f, %d, %d, GLdouble* );\n", (int)target, (float)u1, (float)u2, (int)stride, (int)order);
#endif
  glMap1d( (GLenum)target, (GLdouble)u1, (GLdouble)u2, (GLint)stride, (GLint)order, (const GLdouble*)&points[0] );
  _checkError("glMap1d");
}

FABRIC_EXT_EXPORT void glMap1f_wrapper(
  KL::Size target,
  KL::Scalar u1,
  KL::Scalar u2,
  KL::Integer stride,
  KL::Integer order,
  const KL::VariableArray<KL::Scalar> & points
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMap1f( %d, %f, %f, %d, %d, GLfloat* );\n", (int)target, (float)u1, (float)u2, (int)stride, (int)order);
#endif
  glMap1f( (GLenum)target, (GLfloat)u1, (GLfloat)u2, (GLint)stride, (GLint)order, (const GLfloat*)&points[0] );
  _checkError("glMap1f");
}

FABRIC_EXT_EXPORT void glMap2d_wrapper(
  KL::Size target,
  KL::Scalar u1,
  KL::Scalar u2,
  KL::Integer ustride,
  KL::Integer uorder,
  KL::Scalar v1,
  KL::Scalar v2,
  KL::Integer vstride,
  KL::Integer vorder,
  const KL::VariableArray<KL::Scalar> & points
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMap2d( %d, %f, %f, %d, %d, %f, %f, %d, %d, GLdouble* );\n", (int)target, (float)u1, (float)u2, (int)ustride, (int)uorder, (float)v1, (float)v2, (int)vstride, (int)vorder);
#endif
  glMap2d( (GLenum)target, (GLdouble)u1, (GLdouble)u2, (GLint)ustride, (GLint)uorder, (GLdouble)v1, (GLdouble)v2, (GLint)vstride, (GLint)vorder, (const GLdouble*)&points[0] );
  _checkError("glMap2d");
}

FABRIC_EXT_EXPORT void glMap2f_wrapper(
  KL::Size target,
  KL::Scalar u1,
  KL::Scalar u2,
  KL::Integer ustride,
  KL::Integer uorder,
  KL::Scalar v1,
  KL::Scalar v2,
  KL::Integer vstride,
  KL::Integer vorder,
  const KL::VariableArray<KL::Scalar> & points
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMap2f( %d, %f, %f, %d, %d, %f, %f, %d, %d, GLfloat* );\n", (int)target, (float)u1, (float)u2, (int)ustride, (int)uorder, (float)v1, (float)v2, (int)vstride, (int)vorder);
#endif
  glMap2f( (GLenum)target, (GLfloat)u1, (GLfloat)u2, (GLint)ustride, (GLint)uorder, (GLfloat)v1, (GLfloat)v2, (GLint)vstride, (GLint)vorder, (const GLfloat*)&points[0] );
  _checkError("glMap2f");
}

FABRIC_EXT_EXPORT void glMapGrid1d_wrapper(
  KL::Integer un,
  KL::Scalar u1,
  KL::Scalar u2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMapGrid1d( %d, %f, %f );\n", (int)un, (float)u1, (float)u2);
#endif
  glMapGrid1d( (GLint)un, (GLdouble)u1, (GLdouble)u2 );
  _checkError("glMapGrid1d");
}

FABRIC_EXT_EXPORT void glMapGrid1f_wrapper(
  KL::Integer un,
  KL::Scalar u1,
  KL::Scalar u2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMapGrid1f( %d, %f, %f );\n", (int)un, (float)u1, (float)u2);
#endif
  glMapGrid1f( (GLint)un, (GLfloat)u1, (GLfloat)u2 );
  _checkError("glMapGrid1f");
}

FABRIC_EXT_EXPORT void glMapGrid2d_wrapper(
  KL::Integer un,
  KL::Scalar u1,
  KL::Scalar u2,
  KL::Integer vn,
  KL::Scalar v1,
  KL::Scalar v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMapGrid2d( %d, %f, %f, %d, %f, %f );\n", (int)un, (float)u1, (float)u2, (int)vn, (float)v1, (float)v2);
#endif
  glMapGrid2d( (GLint)un, (GLdouble)u1, (GLdouble)u2, (GLint)vn, (GLdouble)v1, (GLdouble)v2 );
  _checkError("glMapGrid2d");
}

FABRIC_EXT_EXPORT void glMapGrid2f_wrapper(
  KL::Integer un,
  KL::Scalar u1,
  KL::Scalar u2,
  KL::Integer vn,
  KL::Scalar v1,
  KL::Scalar v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMapGrid2f( %d, %f, %f, %d, %f, %f );\n", (int)un, (float)u1, (float)u2, (int)vn, (float)v1, (float)v2);
#endif
  glMapGrid2f( (GLint)un, (GLfloat)u1, (GLfloat)u2, (GLint)vn, (GLfloat)v1, (GLfloat)v2 );
  _checkError("glMapGrid2f");
}

FABRIC_EXT_EXPORT void glMaterialf_wrapper(
  KL::Size face,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMaterialf( %d, %d, %f );\n", (int)face, (int)pname, (float)param);
#endif
  glMaterialf( (GLenum)face, (GLenum)pname, (GLfloat)param );
  _checkError("glMaterialf");
}

FABRIC_EXT_EXPORT void glMaterialfv_wrapper(
  KL::Size face,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMaterialfv( %d, %d, GLfloat* );\n", (int)face, (int)pname);
#endif
  glMaterialfv( (GLenum)face, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glMaterialfv");
}

FABRIC_EXT_EXPORT void glMateriali_wrapper(
  KL::Size face,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMateriali( %d, %d, %d );\n", (int)face, (int)pname, (int)param);
#endif
  glMateriali( (GLenum)face, (GLenum)pname, (GLint)param );
  _checkError("glMateriali");
}

FABRIC_EXT_EXPORT void glMaterialiv_wrapper(
  KL::Size face,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMaterialiv( %d, %d, GLint* );\n", (int)face, (int)pname);
#endif
  glMaterialiv( (GLenum)face, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glMaterialiv");
}

FABRIC_EXT_EXPORT void glMatrixMode_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixMode( %d );\n", (int)mode);
#endif
  glMatrixMode( (GLenum)mode );
  _checkError("glMatrixMode");
}

FABRIC_EXT_EXPORT void glMultMatrixd_wrapper(
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultMatrixd( GLdouble* );\n");
#endif
  glMultMatrixd( (const GLdouble*)&m[0] );
  _checkError("glMultMatrixd");
}

FABRIC_EXT_EXPORT void glMultMatrixf_wrapper(
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultMatrixf( GLfloat* );\n");
#endif
  glMultMatrixf( (const GLfloat*)&m[0] );
  _checkError("glMultMatrixf");
}

FABRIC_EXT_EXPORT void glNewList_wrapper(
  KL::Size list,
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNewList( 0x%04X, %d );\n", (unsigned)list, (int)mode);
#endif
  glNewList( (GLuint)list, (GLenum)mode );
  _checkError("glNewList");
}

FABRIC_EXT_EXPORT void glNormal3b_wrapper(
  KL::Byte nx,
  KL::Byte ny,
  KL::Byte nz
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNormal3b( %d, %d, %d );\n", (int)nx, (int)ny, (int)nz);
#endif
  glNormal3b( (GLbyte)nx, (GLbyte)ny, (GLbyte)nz );
  _checkError("glNormal3b");
}

FABRIC_EXT_EXPORT void glNormal3bv_wrapper(
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNormal3bv( GLbyte* );\n");
#endif
  glNormal3bv( (const GLbyte*)&v[0] );
  _checkError("glNormal3bv");
}

FABRIC_EXT_EXPORT void glNormal3d_wrapper(
  KL::Scalar nx,
  KL::Scalar ny,
  KL::Scalar nz
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNormal3d( %f, %f, %f );\n", (float)nx, (float)ny, (float)nz);
#endif
  glNormal3d( (GLdouble)nx, (GLdouble)ny, (GLdouble)nz );
  _checkError("glNormal3d");
}

FABRIC_EXT_EXPORT void glNormal3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNormal3dv( GLdouble* );\n");
#endif
  glNormal3dv( (const GLdouble*)&v[0] );
  _checkError("glNormal3dv");
}

FABRIC_EXT_EXPORT void glNormal3f_wrapper(
  KL::Scalar nx,
  KL::Scalar ny,
  KL::Scalar nz
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNormal3f( %f, %f, %f );\n", (float)nx, (float)ny, (float)nz);
#endif
  glNormal3f( (GLfloat)nx, (GLfloat)ny, (GLfloat)nz );
  _checkError("glNormal3f");
}

FABRIC_EXT_EXPORT void glNormal3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNormal3fv( GLfloat* );\n");
#endif
  glNormal3fv( (const GLfloat*)&v[0] );
  _checkError("glNormal3fv");
}

FABRIC_EXT_EXPORT void glNormal3i_wrapper(
  KL::Integer nx,
  KL::Integer ny,
  KL::Integer nz
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNormal3i( %d, %d, %d );\n", (int)nx, (int)ny, (int)nz);
#endif
  glNormal3i( (GLint)nx, (GLint)ny, (GLint)nz );
  _checkError("glNormal3i");
}

FABRIC_EXT_EXPORT void glNormal3iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNormal3iv( GLint* );\n");
#endif
  glNormal3iv( (const GLint*)&v[0] );
  _checkError("glNormal3iv");
}

FABRIC_EXT_EXPORT void glNormal3s_wrapper(
  KL::Integer nx,
  KL::Integer ny,
  KL::Integer nz
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNormal3s( %d, %d, %d );\n", (int)nx, (int)ny, (int)nz);
#endif
  glNormal3s( (GLshort)nx, (GLshort)ny, (GLshort)nz );
  _checkError("glNormal3s");
}

FABRIC_EXT_EXPORT void glNormal3sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNormal3sv( GLshort* );\n");
#endif
  glNormal3sv( (const GLshort*)&v[0] );
  _checkError("glNormal3sv");
}

FABRIC_EXT_EXPORT void glNormalPointer_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNormalPointer( %d, %d, GLvoid* );\n", (int)type, (int)stride);
#endif
  glNormalPointer( (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glNormalPointer");
}

FABRIC_EXT_EXPORT void glOrtho_wrapper(
  KL::Scalar left,
  KL::Scalar right,
  KL::Scalar bottom,
  KL::Scalar top,
  KL::Scalar zNear,
  KL::Scalar zFar
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glOrtho( %f, %f, %f, %f, %f, %f );\n", (float)left, (float)right, (float)bottom, (float)top, (float)zNear, (float)zFar);
#endif
  glOrtho( (GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top, (GLdouble)zNear, (GLdouble)zFar );
  _checkError("glOrtho");
}

FABRIC_EXT_EXPORT void glPassThrough_wrapper(
  KL::Scalar token
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPassThrough( %f );\n", (float)token);
#endif
  glPassThrough( (GLfloat)token );
  _checkError("glPassThrough");
}

FABRIC_EXT_EXPORT void glPixelMapfv_wrapper(
  KL::Size map,
  KL::Size mapsize,
  const KL::VariableArray<KL::Scalar> & values
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPixelMapfv( %d, %d, GLfloat* );\n", (int)map, (int)mapsize);
#endif
  glPixelMapfv( (GLenum)map, (GLsizei)mapsize, (const GLfloat*)&values[0] );
  _checkError("glPixelMapfv");
}

FABRIC_EXT_EXPORT void glPixelMapuiv_wrapper(
  KL::Size map,
  KL::Size mapsize,
  const KL::VariableArray<KL::Size> & values
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPixelMapuiv( %d, %d, GLuint* );\n", (int)map, (int)mapsize);
#endif
  glPixelMapuiv( (GLenum)map, (GLsizei)mapsize, (const GLuint*)&values[0] );
  _checkError("glPixelMapuiv");
}

FABRIC_EXT_EXPORT void glPixelMapusv_wrapper(
  KL::Size map,
  KL::Size mapsize,
  const KL::VariableArray<KL::Integer> & values
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPixelMapusv( %d, %d, GLushort* );\n", (int)map, (int)mapsize);
#endif
  glPixelMapusv( (GLenum)map, (GLsizei)mapsize, (const GLushort*)&values[0] );
  _checkError("glPixelMapusv");
}

FABRIC_EXT_EXPORT void glPixelStoref_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPixelStoref( %d, %f );\n", (int)pname, (float)param);
#endif
  glPixelStoref( (GLenum)pname, (GLfloat)param );
  _checkError("glPixelStoref");
}

FABRIC_EXT_EXPORT void glPixelStorei_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPixelStorei( %d, %d );\n", (int)pname, (int)param);
#endif
  glPixelStorei( (GLenum)pname, (GLint)param );
  _checkError("glPixelStorei");
}

FABRIC_EXT_EXPORT void glPixelTransferf_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPixelTransferf( %d, %f );\n", (int)pname, (float)param);
#endif
  glPixelTransferf( (GLenum)pname, (GLfloat)param );
  _checkError("glPixelTransferf");
}

FABRIC_EXT_EXPORT void glPixelTransferi_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPixelTransferi( %d, %d );\n", (int)pname, (int)param);
#endif
  glPixelTransferi( (GLenum)pname, (GLint)param );
  _checkError("glPixelTransferi");
}

FABRIC_EXT_EXPORT void glPixelZoom_wrapper(
  KL::Scalar xfactor,
  KL::Scalar yfactor
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPixelZoom( %f, %f );\n", (float)xfactor, (float)yfactor);
#endif
  glPixelZoom( (GLfloat)xfactor, (GLfloat)yfactor );
  _checkError("glPixelZoom");
}

FABRIC_EXT_EXPORT void glPointSize_wrapper(
  KL::Scalar size
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPointSize( %f );\n", (float)size);
#endif
  glPointSize( (GLfloat)size );
  _checkError("glPointSize");
}

FABRIC_EXT_EXPORT void glPolygonMode_wrapper(
  KL::Size face,
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPolygonMode( %d, %d );\n", (int)face, (int)mode);
#endif
  glPolygonMode( (GLenum)face, (GLenum)mode );
  _checkError("glPolygonMode");
}

FABRIC_EXT_EXPORT void glPolygonOffset_wrapper(
  KL::Scalar factor,
  KL::Scalar units
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPolygonOffset( %f, %f );\n", (float)factor, (float)units);
#endif
  glPolygonOffset( (GLfloat)factor, (GLfloat)units );
  _checkError("glPolygonOffset");
}

FABRIC_EXT_EXPORT void glPolygonStipple_wrapper(
  const KL::VariableArray<KL::Size> & mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPolygonStipple( GLubyte* );\n");
#endif
  glPolygonStipple( (const GLubyte*)&mask[0] );
  _checkError("glPolygonStipple");
}

FABRIC_EXT_EXPORT void glPopAttrib_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPopAttrib(  );\n");
#endif
  glPopAttrib();
  _checkError("glPopAttrib");
}

FABRIC_EXT_EXPORT void glPopClientAttrib_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPopClientAttrib(  );\n");
#endif
  glPopClientAttrib();
  _checkError("glPopClientAttrib");
}

FABRIC_EXT_EXPORT void glPopMatrix_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPopMatrix(  );\n");
#endif
  glPopMatrix();
  _checkError("glPopMatrix");
}

FABRIC_EXT_EXPORT void glPopName_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPopName(  );\n");
#endif
  glPopName();
  _checkError("glPopName");
}

FABRIC_EXT_EXPORT void glPrioritizeTextures_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & textures,
  const KL::VariableArray<KL::Scalar> & priorities
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPrioritizeTextures( %d, GLuint*, GLclampf* );\n", (int)n);
#endif
  glPrioritizeTextures( (GLsizei)n, (const GLuint*)&textures[0], (const GLclampf*)&priorities[0] );
  _checkError("glPrioritizeTextures");
}

FABRIC_EXT_EXPORT void glPushAttrib_wrapper(
  KL::Size mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPushAttrib( 0x%04X );\n", (unsigned)mask);
#endif
  glPushAttrib( (GLbitfield)mask );
  _checkError("glPushAttrib");
}

FABRIC_EXT_EXPORT void glPushClientAttrib_wrapper(
  KL::Size mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPushClientAttrib( 0x%04X );\n", (unsigned)mask);
#endif
  glPushClientAttrib( (GLbitfield)mask );
  _checkError("glPushClientAttrib");
}

FABRIC_EXT_EXPORT void glPushMatrix_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPushMatrix(  );\n");
#endif
  glPushMatrix();
  _checkError("glPushMatrix");
}

FABRIC_EXT_EXPORT void glPushName_wrapper(
  KL::Size name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPushName( 0x%04X );\n", (unsigned)name);
#endif
  glPushName( (GLuint)name );
  _checkError("glPushName");
}

FABRIC_EXT_EXPORT void glRasterPos2d_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos2d( %f, %f );\n", (float)x, (float)y);
#endif
  glRasterPos2d( (GLdouble)x, (GLdouble)y );
  _checkError("glRasterPos2d");
}

FABRIC_EXT_EXPORT void glRasterPos2dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos2dv( GLdouble* );\n");
#endif
  glRasterPos2dv( (const GLdouble*)&v[0] );
  _checkError("glRasterPos2dv");
}

FABRIC_EXT_EXPORT void glRasterPos2f_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos2f( %f, %f );\n", (float)x, (float)y);
#endif
  glRasterPos2f( (GLfloat)x, (GLfloat)y );
  _checkError("glRasterPos2f");
}

FABRIC_EXT_EXPORT void glRasterPos2fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos2fv( GLfloat* );\n");
#endif
  glRasterPos2fv( (const GLfloat*)&v[0] );
  _checkError("glRasterPos2fv");
}

FABRIC_EXT_EXPORT void glRasterPos2i_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos2i( %d, %d );\n", (int)x, (int)y);
#endif
  glRasterPos2i( (GLint)x, (GLint)y );
  _checkError("glRasterPos2i");
}

FABRIC_EXT_EXPORT void glRasterPos2iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos2iv( GLint* );\n");
#endif
  glRasterPos2iv( (const GLint*)&v[0] );
  _checkError("glRasterPos2iv");
}

FABRIC_EXT_EXPORT void glRasterPos2s_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos2s( %d, %d );\n", (int)x, (int)y);
#endif
  glRasterPos2s( (GLshort)x, (GLshort)y );
  _checkError("glRasterPos2s");
}

FABRIC_EXT_EXPORT void glRasterPos2sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos2sv( GLshort* );\n");
#endif
  glRasterPos2sv( (const GLshort*)&v[0] );
  _checkError("glRasterPos2sv");
}

FABRIC_EXT_EXPORT void glRasterPos3d_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos3d( %f, %f, %f );\n", (float)x, (float)y, (float)z);
#endif
  glRasterPos3d( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glRasterPos3d");
}

FABRIC_EXT_EXPORT void glRasterPos3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos3dv( GLdouble* );\n");
#endif
  glRasterPos3dv( (const GLdouble*)&v[0] );
  _checkError("glRasterPos3dv");
}

FABRIC_EXT_EXPORT void glRasterPos3f_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos3f( %f, %f, %f );\n", (float)x, (float)y, (float)z);
#endif
  glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glRasterPos3f");
}

FABRIC_EXT_EXPORT void glRasterPos3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos3fv( GLfloat* );\n");
#endif
  glRasterPos3fv( (const GLfloat*)&v[0] );
  _checkError("glRasterPos3fv");
}

FABRIC_EXT_EXPORT void glRasterPos3i_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos3i( %d, %d, %d );\n", (int)x, (int)y, (int)z);
#endif
  glRasterPos3i( (GLint)x, (GLint)y, (GLint)z );
  _checkError("glRasterPos3i");
}

FABRIC_EXT_EXPORT void glRasterPos3iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos3iv( GLint* );\n");
#endif
  glRasterPos3iv( (const GLint*)&v[0] );
  _checkError("glRasterPos3iv");
}

FABRIC_EXT_EXPORT void glRasterPos3s_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos3s( %d, %d, %d );\n", (int)x, (int)y, (int)z);
#endif
  glRasterPos3s( (GLshort)x, (GLshort)y, (GLshort)z );
  _checkError("glRasterPos3s");
}

FABRIC_EXT_EXPORT void glRasterPos3sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos3sv( GLshort* );\n");
#endif
  glRasterPos3sv( (const GLshort*)&v[0] );
  _checkError("glRasterPos3sv");
}

FABRIC_EXT_EXPORT void glRasterPos4d_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos4d( %f, %f, %f, %f );\n", (float)x, (float)y, (float)z, (float)w);
#endif
  glRasterPos4d( (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  _checkError("glRasterPos4d");
}

FABRIC_EXT_EXPORT void glRasterPos4dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos4dv( GLdouble* );\n");
#endif
  glRasterPos4dv( (const GLdouble*)&v[0] );
  _checkError("glRasterPos4dv");
}

FABRIC_EXT_EXPORT void glRasterPos4f_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos4f( %f, %f, %f, %f );\n", (float)x, (float)y, (float)z, (float)w);
#endif
  glRasterPos4f( (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  _checkError("glRasterPos4f");
}

FABRIC_EXT_EXPORT void glRasterPos4fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos4fv( GLfloat* );\n");
#endif
  glRasterPos4fv( (const GLfloat*)&v[0] );
  _checkError("glRasterPos4fv");
}

FABRIC_EXT_EXPORT void glRasterPos4i_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z,
  KL::Integer w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos4i( %d, %d, %d, %d );\n", (int)x, (int)y, (int)z, (int)w);
#endif
  glRasterPos4i( (GLint)x, (GLint)y, (GLint)z, (GLint)w );
  _checkError("glRasterPos4i");
}

FABRIC_EXT_EXPORT void glRasterPos4iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos4iv( GLint* );\n");
#endif
  glRasterPos4iv( (const GLint*)&v[0] );
  _checkError("glRasterPos4iv");
}

FABRIC_EXT_EXPORT void glRasterPos4s_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z,
  KL::Integer w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos4s( %d, %d, %d, %d );\n", (int)x, (int)y, (int)z, (int)w);
#endif
  glRasterPos4s( (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
  _checkError("glRasterPos4s");
}

FABRIC_EXT_EXPORT void glRasterPos4sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRasterPos4sv( GLshort* );\n");
#endif
  glRasterPos4sv( (const GLshort*)&v[0] );
  _checkError("glRasterPos4sv");
}

FABRIC_EXT_EXPORT void glReadBuffer_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glReadBuffer( %d );\n", (int)mode);
#endif
  glReadBuffer( (GLenum)mode );
  _checkError("glReadBuffer");
}

FABRIC_EXT_EXPORT void glReadPixels_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glReadPixels( %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)x, (int)y, (int)width, (int)height, (int)format, (int)type);
#endif
  glReadPixels( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  _checkError("glReadPixels");
}

FABRIC_EXT_EXPORT void glRectd_wrapper(
  KL::Scalar x1,
  KL::Scalar y1,
  KL::Scalar x2,
  KL::Scalar y2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRectd( %f, %f, %f, %f );\n", (float)x1, (float)y1, (float)x2, (float)y2);
#endif
  glRectd( (GLdouble)x1, (GLdouble)y1, (GLdouble)x2, (GLdouble)y2 );
  _checkError("glRectd");
}

FABRIC_EXT_EXPORT void glRectdv_wrapper(
  const KL::VariableArray<KL::Scalar> & v1,
  const KL::VariableArray<KL::Scalar> & v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRectdv( GLdouble*, GLdouble* );\n");
#endif
  glRectdv( (const GLdouble*)&v1[0], (const GLdouble*)&v2[0] );
  _checkError("glRectdv");
}

FABRIC_EXT_EXPORT void glRectf_wrapper(
  KL::Scalar x1,
  KL::Scalar y1,
  KL::Scalar x2,
  KL::Scalar y2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRectf( %f, %f, %f, %f );\n", (float)x1, (float)y1, (float)x2, (float)y2);
#endif
  glRectf( (GLfloat)x1, (GLfloat)y1, (GLfloat)x2, (GLfloat)y2 );
  _checkError("glRectf");
}

FABRIC_EXT_EXPORT void glRectfv_wrapper(
  const KL::VariableArray<KL::Scalar> & v1,
  const KL::VariableArray<KL::Scalar> & v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRectfv( GLfloat*, GLfloat* );\n");
#endif
  glRectfv( (const GLfloat*)&v1[0], (const GLfloat*)&v2[0] );
  _checkError("glRectfv");
}

FABRIC_EXT_EXPORT void glRecti_wrapper(
  KL::Integer x1,
  KL::Integer y1,
  KL::Integer x2,
  KL::Integer y2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRecti( %d, %d, %d, %d );\n", (int)x1, (int)y1, (int)x2, (int)y2);
#endif
  glRecti( (GLint)x1, (GLint)y1, (GLint)x2, (GLint)y2 );
  _checkError("glRecti");
}

FABRIC_EXT_EXPORT void glRectiv_wrapper(
  const KL::VariableArray<KL::Integer> & v1,
  const KL::VariableArray<KL::Integer> & v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRectiv( GLint*, GLint* );\n");
#endif
  glRectiv( (const GLint*)&v1[0], (const GLint*)&v2[0] );
  _checkError("glRectiv");
}

FABRIC_EXT_EXPORT void glRects_wrapper(
  KL::Integer x1,
  KL::Integer y1,
  KL::Integer x2,
  KL::Integer y2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRects( %d, %d, %d, %d );\n", (int)x1, (int)y1, (int)x2, (int)y2);
#endif
  glRects( (GLshort)x1, (GLshort)y1, (GLshort)x2, (GLshort)y2 );
  _checkError("glRects");
}

FABRIC_EXT_EXPORT void glRectsv_wrapper(
  const KL::VariableArray<KL::Integer> & v1,
  const KL::VariableArray<KL::Integer> & v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRectsv( GLshort*, GLshort* );\n");
#endif
  glRectsv( (const GLshort*)&v1[0], (const GLshort*)&v2[0] );
  _checkError("glRectsv");
}

FABRIC_EXT_EXPORT KL::Integer glRenderMode_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRenderMode( %d );\n", (int)mode);
#endif
  GLint result = glRenderMode( (GLenum)mode );
  _checkError("glRenderMode");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT void glRotated_wrapper(
  KL::Scalar angle,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRotated( %f, %f, %f, %f );\n", (float)angle, (float)x, (float)y, (float)z);
#endif
  glRotated( (GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glRotated");
}

FABRIC_EXT_EXPORT void glRotatef_wrapper(
  KL::Scalar angle,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRotatef( %f, %f, %f, %f );\n", (float)angle, (float)x, (float)y, (float)z);
#endif
  glRotatef( (GLfloat)angle, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glRotatef");
}

FABRIC_EXT_EXPORT void glScaled_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glScaled( %f, %f, %f );\n", (float)x, (float)y, (float)z);
#endif
  glScaled( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glScaled");
}

FABRIC_EXT_EXPORT void glScalef_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glScalef( %f, %f, %f );\n", (float)x, (float)y, (float)z);
#endif
  glScalef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glScalef");
}

FABRIC_EXT_EXPORT void glScissor_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glScissor( %d, %d, %d, %d );\n", (int)x, (int)y, (int)width, (int)height);
#endif
  glScissor( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glScissor");
}

FABRIC_EXT_EXPORT void glSelectBuffer_wrapper(
  KL::Size size,
  KL::VariableArray<KL::Size> & buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSelectBuffer( %d, GLuint* );\n", (int)size);
#endif
  glSelectBuffer( (GLsizei)size, (GLuint*)&buffer[0] );
  _checkError("glSelectBuffer");
}

FABRIC_EXT_EXPORT void glShadeModel_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glShadeModel( %d );\n", (int)mode);
#endif
  glShadeModel( (GLenum)mode );
  _checkError("glShadeModel");
}

FABRIC_EXT_EXPORT void glStencilFunc_wrapper(
  KL::Size func,
  KL::Integer ref,
  KL::Size mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glStencilFunc( %d, %d, 0x%04X );\n", (int)func, (int)ref, (unsigned)mask);
#endif
  glStencilFunc( (GLenum)func, (GLint)ref, (GLuint)mask );
  _checkError("glStencilFunc");
}

FABRIC_EXT_EXPORT void glStencilMask_wrapper(
  KL::Size mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glStencilMask( 0x%04X );\n", (unsigned)mask);
#endif
  glStencilMask( (GLuint)mask );
  _checkError("glStencilMask");
}

FABRIC_EXT_EXPORT void glStencilOp_wrapper(
  KL::Size fail,
  KL::Size zfail,
  KL::Size zpass
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glStencilOp( %d, %d, %d );\n", (int)fail, (int)zfail, (int)zpass);
#endif
  glStencilOp( (GLenum)fail, (GLenum)zfail, (GLenum)zpass );
  _checkError("glStencilOp");
}

FABRIC_EXT_EXPORT void glTexCoord1d_wrapper(
  KL::Scalar s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord1d( %f );\n", (float)s);
#endif
  glTexCoord1d( (GLdouble)s );
  _checkError("glTexCoord1d");
}

FABRIC_EXT_EXPORT void glTexCoord1dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord1dv( GLdouble* );\n");
#endif
  glTexCoord1dv( (const GLdouble*)&v[0] );
  _checkError("glTexCoord1dv");
}

FABRIC_EXT_EXPORT void glTexCoord1f_wrapper(
  KL::Scalar s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord1f( %f );\n", (float)s);
#endif
  glTexCoord1f( (GLfloat)s );
  _checkError("glTexCoord1f");
}

FABRIC_EXT_EXPORT void glTexCoord1fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord1fv( GLfloat* );\n");
#endif
  glTexCoord1fv( (const GLfloat*)&v[0] );
  _checkError("glTexCoord1fv");
}

FABRIC_EXT_EXPORT void glTexCoord1i_wrapper(
  KL::Integer s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord1i( %d );\n", (int)s);
#endif
  glTexCoord1i( (GLint)s );
  _checkError("glTexCoord1i");
}

FABRIC_EXT_EXPORT void glTexCoord1iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord1iv( GLint* );\n");
#endif
  glTexCoord1iv( (const GLint*)&v[0] );
  _checkError("glTexCoord1iv");
}

FABRIC_EXT_EXPORT void glTexCoord1s_wrapper(
  KL::Integer s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord1s( %d );\n", (int)s);
#endif
  glTexCoord1s( (GLshort)s );
  _checkError("glTexCoord1s");
}

FABRIC_EXT_EXPORT void glTexCoord1sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord1sv( GLshort* );\n");
#endif
  glTexCoord1sv( (const GLshort*)&v[0] );
  _checkError("glTexCoord1sv");
}

FABRIC_EXT_EXPORT void glTexCoord2d_wrapper(
  KL::Scalar s,
  KL::Scalar t
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord2d( %f, %f );\n", (float)s, (float)t);
#endif
  glTexCoord2d( (GLdouble)s, (GLdouble)t );
  _checkError("glTexCoord2d");
}

FABRIC_EXT_EXPORT void glTexCoord2dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord2dv( GLdouble* );\n");
#endif
  glTexCoord2dv( (const GLdouble*)&v[0] );
  _checkError("glTexCoord2dv");
}

FABRIC_EXT_EXPORT void glTexCoord2f_wrapper(
  KL::Scalar s,
  KL::Scalar t
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord2f( %f, %f );\n", (float)s, (float)t);
#endif
  glTexCoord2f( (GLfloat)s, (GLfloat)t );
  _checkError("glTexCoord2f");
}

FABRIC_EXT_EXPORT void glTexCoord2fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord2fv( GLfloat* );\n");
#endif
  glTexCoord2fv( (const GLfloat*)&v[0] );
  _checkError("glTexCoord2fv");
}

FABRIC_EXT_EXPORT void glTexCoord2i_wrapper(
  KL::Integer s,
  KL::Integer t
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord2i( %d, %d );\n", (int)s, (int)t);
#endif
  glTexCoord2i( (GLint)s, (GLint)t );
  _checkError("glTexCoord2i");
}

FABRIC_EXT_EXPORT void glTexCoord2iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord2iv( GLint* );\n");
#endif
  glTexCoord2iv( (const GLint*)&v[0] );
  _checkError("glTexCoord2iv");
}

FABRIC_EXT_EXPORT void glTexCoord2s_wrapper(
  KL::Integer s,
  KL::Integer t
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord2s( %d, %d );\n", (int)s, (int)t);
#endif
  glTexCoord2s( (GLshort)s, (GLshort)t );
  _checkError("glTexCoord2s");
}

FABRIC_EXT_EXPORT void glTexCoord2sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord2sv( GLshort* );\n");
#endif
  glTexCoord2sv( (const GLshort*)&v[0] );
  _checkError("glTexCoord2sv");
}

FABRIC_EXT_EXPORT void glTexCoord3d_wrapper(
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord3d( %f, %f, %f );\n", (float)s, (float)t, (float)r);
#endif
  glTexCoord3d( (GLdouble)s, (GLdouble)t, (GLdouble)r );
  _checkError("glTexCoord3d");
}

FABRIC_EXT_EXPORT void glTexCoord3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord3dv( GLdouble* );\n");
#endif
  glTexCoord3dv( (const GLdouble*)&v[0] );
  _checkError("glTexCoord3dv");
}

FABRIC_EXT_EXPORT void glTexCoord3f_wrapper(
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord3f( %f, %f, %f );\n", (float)s, (float)t, (float)r);
#endif
  glTexCoord3f( (GLfloat)s, (GLfloat)t, (GLfloat)r );
  _checkError("glTexCoord3f");
}

FABRIC_EXT_EXPORT void glTexCoord3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord3fv( GLfloat* );\n");
#endif
  glTexCoord3fv( (const GLfloat*)&v[0] );
  _checkError("glTexCoord3fv");
}

FABRIC_EXT_EXPORT void glTexCoord3i_wrapper(
  KL::Integer s,
  KL::Integer t,
  KL::Integer r
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord3i( %d, %d, %d );\n", (int)s, (int)t, (int)r);
#endif
  glTexCoord3i( (GLint)s, (GLint)t, (GLint)r );
  _checkError("glTexCoord3i");
}

FABRIC_EXT_EXPORT void glTexCoord3iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord3iv( GLint* );\n");
#endif
  glTexCoord3iv( (const GLint*)&v[0] );
  _checkError("glTexCoord3iv");
}

FABRIC_EXT_EXPORT void glTexCoord3s_wrapper(
  KL::Integer s,
  KL::Integer t,
  KL::Integer r
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord3s( %d, %d, %d );\n", (int)s, (int)t, (int)r);
#endif
  glTexCoord3s( (GLshort)s, (GLshort)t, (GLshort)r );
  _checkError("glTexCoord3s");
}

FABRIC_EXT_EXPORT void glTexCoord3sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord3sv( GLshort* );\n");
#endif
  glTexCoord3sv( (const GLshort*)&v[0] );
  _checkError("glTexCoord3sv");
}

FABRIC_EXT_EXPORT void glTexCoord4d_wrapper(
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r,
  KL::Scalar q
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord4d( %f, %f, %f, %f );\n", (float)s, (float)t, (float)r, (float)q);
#endif
  glTexCoord4d( (GLdouble)s, (GLdouble)t, (GLdouble)r, (GLdouble)q );
  _checkError("glTexCoord4d");
}

FABRIC_EXT_EXPORT void glTexCoord4dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord4dv( GLdouble* );\n");
#endif
  glTexCoord4dv( (const GLdouble*)&v[0] );
  _checkError("glTexCoord4dv");
}

FABRIC_EXT_EXPORT void glTexCoord4f_wrapper(
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r,
  KL::Scalar q
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord4f( %f, %f, %f, %f );\n", (float)s, (float)t, (float)r, (float)q);
#endif
  glTexCoord4f( (GLfloat)s, (GLfloat)t, (GLfloat)r, (GLfloat)q );
  _checkError("glTexCoord4f");
}

FABRIC_EXT_EXPORT void glTexCoord4fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord4fv( GLfloat* );\n");
#endif
  glTexCoord4fv( (const GLfloat*)&v[0] );
  _checkError("glTexCoord4fv");
}

FABRIC_EXT_EXPORT void glTexCoord4i_wrapper(
  KL::Integer s,
  KL::Integer t,
  KL::Integer r,
  KL::Integer q
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord4i( %d, %d, %d, %d );\n", (int)s, (int)t, (int)r, (int)q);
#endif
  glTexCoord4i( (GLint)s, (GLint)t, (GLint)r, (GLint)q );
  _checkError("glTexCoord4i");
}

FABRIC_EXT_EXPORT void glTexCoord4iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord4iv( GLint* );\n");
#endif
  glTexCoord4iv( (const GLint*)&v[0] );
  _checkError("glTexCoord4iv");
}

FABRIC_EXT_EXPORT void glTexCoord4s_wrapper(
  KL::Integer s,
  KL::Integer t,
  KL::Integer r,
  KL::Integer q
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord4s( %d, %d, %d, %d );\n", (int)s, (int)t, (int)r, (int)q);
#endif
  glTexCoord4s( (GLshort)s, (GLshort)t, (GLshort)r, (GLshort)q );
  _checkError("glTexCoord4s");
}

FABRIC_EXT_EXPORT void glTexCoord4sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoord4sv( GLshort* );\n");
#endif
  glTexCoord4sv( (const GLshort*)&v[0] );
  _checkError("glTexCoord4sv");
}

FABRIC_EXT_EXPORT void glTexCoordPointer_wrapper(
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoordPointer( %d, %d, %d, GLvoid* );\n", (int)size, (int)type, (int)stride);
#endif
  glTexCoordPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glTexCoordPointer");
}

FABRIC_EXT_EXPORT void glTexEnvf_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexEnvf( %d, %d, %f );\n", (int)target, (int)pname, (float)param);
#endif
  glTexEnvf( (GLenum)target, (GLenum)pname, (GLfloat)param );
  _checkError("glTexEnvf");
}

FABRIC_EXT_EXPORT void glTexEnvfv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexEnvfv( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glTexEnvfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glTexEnvfv");
}

FABRIC_EXT_EXPORT void glTexEnvi_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexEnvi( %d, %d, %d );\n", (int)target, (int)pname, (int)param);
#endif
  glTexEnvi( (GLenum)target, (GLenum)pname, (GLint)param );
  _checkError("glTexEnvi");
}

FABRIC_EXT_EXPORT void glTexEnviv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexEnviv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glTexEnviv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glTexEnviv");
}

FABRIC_EXT_EXPORT void glTexGend_wrapper(
  KL::Size coord,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexGend( %d, %d, %f );\n", (int)coord, (int)pname, (float)param);
#endif
  glTexGend( (GLenum)coord, (GLenum)pname, (GLdouble)param );
  _checkError("glTexGend");
}

FABRIC_EXT_EXPORT void glTexGendv_wrapper(
  KL::Size coord,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexGendv( %d, %d, GLdouble* );\n", (int)coord, (int)pname);
#endif
  glTexGendv( (GLenum)coord, (GLenum)pname, (const GLdouble*)&params[0] );
  _checkError("glTexGendv");
}

FABRIC_EXT_EXPORT void glTexGenf_wrapper(
  KL::Size coord,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexGenf( %d, %d, %f );\n", (int)coord, (int)pname, (float)param);
#endif
  glTexGenf( (GLenum)coord, (GLenum)pname, (GLfloat)param );
  _checkError("glTexGenf");
}

FABRIC_EXT_EXPORT void glTexGenfv_wrapper(
  KL::Size coord,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexGenfv( %d, %d, GLfloat* );\n", (int)coord, (int)pname);
#endif
  glTexGenfv( (GLenum)coord, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glTexGenfv");
}

FABRIC_EXT_EXPORT void glTexGeni_wrapper(
  KL::Size coord,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexGeni( %d, %d, %d );\n", (int)coord, (int)pname, (int)param);
#endif
  glTexGeni( (GLenum)coord, (GLenum)pname, (GLint)param );
  _checkError("glTexGeni");
}

FABRIC_EXT_EXPORT void glTexGeniv_wrapper(
  KL::Size coord,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexGeniv( %d, %d, GLint* );\n", (int)coord, (int)pname);
#endif
  glTexGeniv( (GLenum)coord, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glTexGeniv");
}

FABRIC_EXT_EXPORT void glTexImage1D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer internalformat,
  KL::Size width,
  KL::Integer border,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexImage1D( %d, %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)format, (int)type);
#endif
  glTexImage1D( (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexImage1D");
}

FABRIC_EXT_EXPORT void glTexImage2D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer internalformat,
  KL::Size width,
  KL::Size height,
  KL::Integer border,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexImage2D( %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)format, (int)type);
#endif
  glTexImage2D( (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexImage2D");
}

FABRIC_EXT_EXPORT void glTexParameterf_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexParameterf( %d, %d, %f );\n", (int)target, (int)pname, (float)param);
#endif
  glTexParameterf( (GLenum)target, (GLenum)pname, (GLfloat)param );
  _checkError("glTexParameterf");
}

FABRIC_EXT_EXPORT void glTexParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexParameterfv( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glTexParameterfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glTexParameterfv");
}

FABRIC_EXT_EXPORT void glTexParameteri_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexParameteri( %d, %d, %d );\n", (int)target, (int)pname, (int)param);
#endif
  glTexParameteri( (GLenum)target, (GLenum)pname, (GLint)param );
  _checkError("glTexParameteri");
}

FABRIC_EXT_EXPORT void glTexParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexParameteriv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glTexParameteriv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glTexParameteriv");
}

FABRIC_EXT_EXPORT void glTexSubImage1D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Size width,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexSubImage1D( %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)type);
#endif
  glTexSubImage1D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexSubImage1D");
}

FABRIC_EXT_EXPORT void glTexSubImage2D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexSubImage2D( %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)type);
#endif
  glTexSubImage2D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexSubImage2D");
}

FABRIC_EXT_EXPORT void glTranslated_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTranslated( %f, %f, %f );\n", (float)x, (float)y, (float)z);
#endif
  glTranslated( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glTranslated");
}

FABRIC_EXT_EXPORT void glTranslatef_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTranslatef( %f, %f, %f );\n", (float)x, (float)y, (float)z);
#endif
  glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glTranslatef");
}

FABRIC_EXT_EXPORT void glVertex2d_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex2d( %f, %f );\n", (float)x, (float)y);
#endif
  glVertex2d( (GLdouble)x, (GLdouble)y );
  _checkError("glVertex2d");
}

FABRIC_EXT_EXPORT void glVertex2dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex2dv( GLdouble* );\n");
#endif
  glVertex2dv( (const GLdouble*)&v[0] );
  _checkError("glVertex2dv");
}

FABRIC_EXT_EXPORT void glVertex2f_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex2f( %f, %f );\n", (float)x, (float)y);
#endif
  glVertex2f( (GLfloat)x, (GLfloat)y );
  _checkError("glVertex2f");
}

FABRIC_EXT_EXPORT void glVertex2fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex2fv( GLfloat* );\n");
#endif
  glVertex2fv( (const GLfloat*)&v[0] );
  _checkError("glVertex2fv");
}

FABRIC_EXT_EXPORT void glVertex2i_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex2i( %d, %d );\n", (int)x, (int)y);
#endif
  glVertex2i( (GLint)x, (GLint)y );
  _checkError("glVertex2i");
}

FABRIC_EXT_EXPORT void glVertex2iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex2iv( GLint* );\n");
#endif
  glVertex2iv( (const GLint*)&v[0] );
  _checkError("glVertex2iv");
}

FABRIC_EXT_EXPORT void glVertex2s_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex2s( %d, %d );\n", (int)x, (int)y);
#endif
  glVertex2s( (GLshort)x, (GLshort)y );
  _checkError("glVertex2s");
}

FABRIC_EXT_EXPORT void glVertex2sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex2sv( GLshort* );\n");
#endif
  glVertex2sv( (const GLshort*)&v[0] );
  _checkError("glVertex2sv");
}

FABRIC_EXT_EXPORT void glVertex3d_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex3d( %f, %f, %f );\n", (float)x, (float)y, (float)z);
#endif
  glVertex3d( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glVertex3d");
}

FABRIC_EXT_EXPORT void glVertex3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex3dv( GLdouble* );\n");
#endif
  glVertex3dv( (const GLdouble*)&v[0] );
  _checkError("glVertex3dv");
}

FABRIC_EXT_EXPORT void glVertex3f_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex3f( %f, %f, %f );\n", (float)x, (float)y, (float)z);
#endif
  glVertex3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glVertex3f");
}

FABRIC_EXT_EXPORT void glVertex3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex3fv( GLfloat* );\n");
#endif
  glVertex3fv( (const GLfloat*)&v[0] );
  _checkError("glVertex3fv");
}

FABRIC_EXT_EXPORT void glVertex3i_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex3i( %d, %d, %d );\n", (int)x, (int)y, (int)z);
#endif
  glVertex3i( (GLint)x, (GLint)y, (GLint)z );
  _checkError("glVertex3i");
}

FABRIC_EXT_EXPORT void glVertex3iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex3iv( GLint* );\n");
#endif
  glVertex3iv( (const GLint*)&v[0] );
  _checkError("glVertex3iv");
}

FABRIC_EXT_EXPORT void glVertex3s_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex3s( %d, %d, %d );\n", (int)x, (int)y, (int)z);
#endif
  glVertex3s( (GLshort)x, (GLshort)y, (GLshort)z );
  _checkError("glVertex3s");
}

FABRIC_EXT_EXPORT void glVertex3sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex3sv( GLshort* );\n");
#endif
  glVertex3sv( (const GLshort*)&v[0] );
  _checkError("glVertex3sv");
}

FABRIC_EXT_EXPORT void glVertex4d_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex4d( %f, %f, %f, %f );\n", (float)x, (float)y, (float)z, (float)w);
#endif
  glVertex4d( (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  _checkError("glVertex4d");
}

FABRIC_EXT_EXPORT void glVertex4dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex4dv( GLdouble* );\n");
#endif
  glVertex4dv( (const GLdouble*)&v[0] );
  _checkError("glVertex4dv");
}

FABRIC_EXT_EXPORT void glVertex4f_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex4f( %f, %f, %f, %f );\n", (float)x, (float)y, (float)z, (float)w);
#endif
  glVertex4f( (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  _checkError("glVertex4f");
}

FABRIC_EXT_EXPORT void glVertex4fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex4fv( GLfloat* );\n");
#endif
  glVertex4fv( (const GLfloat*)&v[0] );
  _checkError("glVertex4fv");
}

FABRIC_EXT_EXPORT void glVertex4i_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z,
  KL::Integer w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex4i( %d, %d, %d, %d );\n", (int)x, (int)y, (int)z, (int)w);
#endif
  glVertex4i( (GLint)x, (GLint)y, (GLint)z, (GLint)w );
  _checkError("glVertex4i");
}

FABRIC_EXT_EXPORT void glVertex4iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex4iv( GLint* );\n");
#endif
  glVertex4iv( (const GLint*)&v[0] );
  _checkError("glVertex4iv");
}

FABRIC_EXT_EXPORT void glVertex4s_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z,
  KL::Integer w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex4s( %d, %d, %d, %d );\n", (int)x, (int)y, (int)z, (int)w);
#endif
  glVertex4s( (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
  _checkError("glVertex4s");
}

FABRIC_EXT_EXPORT void glVertex4sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertex4sv( GLshort* );\n");
#endif
  glVertex4sv( (const GLshort*)&v[0] );
  _checkError("glVertex4sv");
}

FABRIC_EXT_EXPORT void glVertexPointer_wrapper(
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexPointer( %d, %d, %d, GLvoid* );\n", (int)size, (int)type, (int)stride);
#endif
  glVertexPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glVertexPointer");
}

FABRIC_EXT_EXPORT void glViewport_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glViewport( %d, %d, %d, %d );\n", (int)x, (int)y, (int)width, (int)height);
#endif
  glViewport( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glViewport");
}

FABRIC_EXT_EXPORT void glCopyTexSubImage3D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer zoffset,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTexSubImage3D( %d, %d, %d, %d, %d, %d, %d, %d, %d );\n", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)x, (int)y, (int)width, (int)height);
#endif
  glCopyTexSubImage3D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyTexSubImage3D");
}

FABRIC_EXT_EXPORT void glDrawRangeElements_wrapper(
  KL::Size mode,
  KL::Size start,
  KL::Size end,
  KL::Size count,
  KL::Size type,
  KL::Data indices
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawRangeElements( %d, 0x%04X, 0x%04X, %d, %d, GLvoid* );\n", (int)mode, (unsigned)start, (unsigned)end, (int)count, (int)type);
#endif
  glDrawRangeElements( (GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, indices );
  _checkError("glDrawRangeElements");
}

FABRIC_EXT_EXPORT void glTexImage3D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer internalFormat,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Integer border,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexImage3D( %d, %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)internalFormat, (int)width, (int)height, (int)depth, (int)border, (int)format, (int)type);
#endif
  glTexImage3D( (GLenum)target, (GLint)level, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexImage3D");
}

FABRIC_EXT_EXPORT void glTexSubImage3D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer zoffset,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexSubImage3D( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)type);
#endif
  glTexSubImage3D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexSubImage3D");
}

FABRIC_EXT_EXPORT void glActiveTexture_wrapper(
  KL::Size texture
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glActiveTexture( %d );\n", (int)texture);
#endif
  glActiveTexture( (GLenum)texture );
  _checkError("glActiveTexture");
}

FABRIC_EXT_EXPORT void glClientActiveTexture_wrapper(
  KL::Size texture
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClientActiveTexture( %d );\n", (int)texture);
#endif
  glClientActiveTexture( (GLenum)texture );
  _checkError("glClientActiveTexture");
}

FABRIC_EXT_EXPORT void glCompressedTexImage1D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Integer border,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTexImage1D( %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)imageSize);
#endif
  glCompressedTexImage1D( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLint)border, (GLsizei)imageSize, data );
  _checkError("glCompressedTexImage1D");
}

FABRIC_EXT_EXPORT void glCompressedTexImage2D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Integer border,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTexImage2D( %d, %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)imageSize);
#endif
  glCompressedTexImage2D( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLsizei)imageSize, data );
  _checkError("glCompressedTexImage2D");
}

FABRIC_EXT_EXPORT void glCompressedTexImage3D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Integer border,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTexImage3D( %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)imageSize);
#endif
  glCompressedTexImage3D( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLsizei)imageSize, data );
  _checkError("glCompressedTexImage3D");
}

FABRIC_EXT_EXPORT void glCompressedTexSubImage1D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Size width,
  KL::Size format,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTexSubImage1D( %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)imageSize);
#endif
  glCompressedTexSubImage1D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedTexSubImage1D");
}

FABRIC_EXT_EXPORT void glCompressedTexSubImage2D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTexSubImage2D( %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)imageSize);
#endif
  glCompressedTexSubImage2D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedTexSubImage2D");
}

FABRIC_EXT_EXPORT void glCompressedTexSubImage3D_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer zoffset,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Size format,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTexSubImage3D( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)imageSize);
#endif
  glCompressedTexSubImage3D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedTexSubImage3D");
}

FABRIC_EXT_EXPORT void glGetCompressedTexImage_wrapper(
  KL::Size target,
  KL::Integer lod,
  KL::Data img
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetCompressedTexImage( %d, %d, GLvoid* );\n", (int)target, (int)lod);
#endif
  glGetCompressedTexImage( (GLenum)target, (GLint)lod, img );
  _checkError("glGetCompressedTexImage");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1d_wrapper(
  KL::Size target,
  KL::Scalar s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1d( %d, %f );\n", (int)target, (float)s);
#endif
  glMultiTexCoord1d( (GLenum)target, (GLdouble)s );
  _checkError("glMultiTexCoord1d");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1dv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1dv( %d, GLdouble* );\n", (int)target);
#endif
  glMultiTexCoord1dv( (GLenum)target, (const GLdouble*)&v[0] );
  _checkError("glMultiTexCoord1dv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1f_wrapper(
  KL::Size target,
  KL::Scalar s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1f( %d, %f );\n", (int)target, (float)s);
#endif
  glMultiTexCoord1f( (GLenum)target, (GLfloat)s );
  _checkError("glMultiTexCoord1f");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1fv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1fv( %d, GLfloat* );\n", (int)target);
#endif
  glMultiTexCoord1fv( (GLenum)target, (const GLfloat*)&v[0] );
  _checkError("glMultiTexCoord1fv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1i_wrapper(
  KL::Size target,
  KL::Integer s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1i( %d, %d );\n", (int)target, (int)s);
#endif
  glMultiTexCoord1i( (GLenum)target, (GLint)s );
  _checkError("glMultiTexCoord1i");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1iv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1iv( %d, GLint* );\n", (int)target);
#endif
  glMultiTexCoord1iv( (GLenum)target, (const GLint*)&v[0] );
  _checkError("glMultiTexCoord1iv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1s_wrapper(
  KL::Size target,
  KL::Integer s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1s( %d, %d );\n", (int)target, (int)s);
#endif
  glMultiTexCoord1s( (GLenum)target, (GLshort)s );
  _checkError("glMultiTexCoord1s");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1sv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1sv( %d, GLshort* );\n", (int)target);
#endif
  glMultiTexCoord1sv( (GLenum)target, (const GLshort*)&v[0] );
  _checkError("glMultiTexCoord1sv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2d_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2d( %d, %f, %f );\n", (int)target, (float)s, (float)t);
#endif
  glMultiTexCoord2d( (GLenum)target, (GLdouble)s, (GLdouble)t );
  _checkError("glMultiTexCoord2d");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2dv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2dv( %d, GLdouble* );\n", (int)target);
#endif
  glMultiTexCoord2dv( (GLenum)target, (const GLdouble*)&v[0] );
  _checkError("glMultiTexCoord2dv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2f_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2f( %d, %f, %f );\n", (int)target, (float)s, (float)t);
#endif
  glMultiTexCoord2f( (GLenum)target, (GLfloat)s, (GLfloat)t );
  _checkError("glMultiTexCoord2f");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2fv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2fv( %d, GLfloat* );\n", (int)target);
#endif
  glMultiTexCoord2fv( (GLenum)target, (const GLfloat*)&v[0] );
  _checkError("glMultiTexCoord2fv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2i_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2i( %d, %d, %d );\n", (int)target, (int)s, (int)t);
#endif
  glMultiTexCoord2i( (GLenum)target, (GLint)s, (GLint)t );
  _checkError("glMultiTexCoord2i");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2iv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2iv( %d, GLint* );\n", (int)target);
#endif
  glMultiTexCoord2iv( (GLenum)target, (const GLint*)&v[0] );
  _checkError("glMultiTexCoord2iv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2s_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2s( %d, %d, %d );\n", (int)target, (int)s, (int)t);
#endif
  glMultiTexCoord2s( (GLenum)target, (GLshort)s, (GLshort)t );
  _checkError("glMultiTexCoord2s");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2sv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2sv( %d, GLshort* );\n", (int)target);
#endif
  glMultiTexCoord2sv( (GLenum)target, (const GLshort*)&v[0] );
  _checkError("glMultiTexCoord2sv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3d_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3d( %d, %f, %f, %f );\n", (int)target, (float)s, (float)t, (float)r);
#endif
  glMultiTexCoord3d( (GLenum)target, (GLdouble)s, (GLdouble)t, (GLdouble)r );
  _checkError("glMultiTexCoord3d");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3dv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3dv( %d, GLdouble* );\n", (int)target);
#endif
  glMultiTexCoord3dv( (GLenum)target, (const GLdouble*)&v[0] );
  _checkError("glMultiTexCoord3dv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3f_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3f( %d, %f, %f, %f );\n", (int)target, (float)s, (float)t, (float)r);
#endif
  glMultiTexCoord3f( (GLenum)target, (GLfloat)s, (GLfloat)t, (GLfloat)r );
  _checkError("glMultiTexCoord3f");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3fv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3fv( %d, GLfloat* );\n", (int)target);
#endif
  glMultiTexCoord3fv( (GLenum)target, (const GLfloat*)&v[0] );
  _checkError("glMultiTexCoord3fv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3i_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t,
  KL::Integer r
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3i( %d, %d, %d, %d );\n", (int)target, (int)s, (int)t, (int)r);
#endif
  glMultiTexCoord3i( (GLenum)target, (GLint)s, (GLint)t, (GLint)r );
  _checkError("glMultiTexCoord3i");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3iv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3iv( %d, GLint* );\n", (int)target);
#endif
  glMultiTexCoord3iv( (GLenum)target, (const GLint*)&v[0] );
  _checkError("glMultiTexCoord3iv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3s_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t,
  KL::Integer r
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3s( %d, %d, %d, %d );\n", (int)target, (int)s, (int)t, (int)r);
#endif
  glMultiTexCoord3s( (GLenum)target, (GLshort)s, (GLshort)t, (GLshort)r );
  _checkError("glMultiTexCoord3s");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3sv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3sv( %d, GLshort* );\n", (int)target);
#endif
  glMultiTexCoord3sv( (GLenum)target, (const GLshort*)&v[0] );
  _checkError("glMultiTexCoord3sv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4d_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r,
  KL::Scalar q
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4d( %d, %f, %f, %f, %f );\n", (int)target, (float)s, (float)t, (float)r, (float)q);
#endif
  glMultiTexCoord4d( (GLenum)target, (GLdouble)s, (GLdouble)t, (GLdouble)r, (GLdouble)q );
  _checkError("glMultiTexCoord4d");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4dv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4dv( %d, GLdouble* );\n", (int)target);
#endif
  glMultiTexCoord4dv( (GLenum)target, (const GLdouble*)&v[0] );
  _checkError("glMultiTexCoord4dv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4f_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r,
  KL::Scalar q
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4f( %d, %f, %f, %f, %f );\n", (int)target, (float)s, (float)t, (float)r, (float)q);
#endif
  glMultiTexCoord4f( (GLenum)target, (GLfloat)s, (GLfloat)t, (GLfloat)r, (GLfloat)q );
  _checkError("glMultiTexCoord4f");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4fv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4fv( %d, GLfloat* );\n", (int)target);
#endif
  glMultiTexCoord4fv( (GLenum)target, (const GLfloat*)&v[0] );
  _checkError("glMultiTexCoord4fv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4i_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t,
  KL::Integer r,
  KL::Integer q
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4i( %d, %d, %d, %d, %d );\n", (int)target, (int)s, (int)t, (int)r, (int)q);
#endif
  glMultiTexCoord4i( (GLenum)target, (GLint)s, (GLint)t, (GLint)r, (GLint)q );
  _checkError("glMultiTexCoord4i");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4iv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4iv( %d, GLint* );\n", (int)target);
#endif
  glMultiTexCoord4iv( (GLenum)target, (const GLint*)&v[0] );
  _checkError("glMultiTexCoord4iv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4s_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t,
  KL::Integer r,
  KL::Integer q
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4s( %d, %d, %d, %d, %d );\n", (int)target, (int)s, (int)t, (int)r, (int)q);
#endif
  glMultiTexCoord4s( (GLenum)target, (GLshort)s, (GLshort)t, (GLshort)r, (GLshort)q );
  _checkError("glMultiTexCoord4s");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4sv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4sv( %d, GLshort* );\n", (int)target);
#endif
  glMultiTexCoord4sv( (GLenum)target, (const GLshort*)&v[0] );
  _checkError("glMultiTexCoord4sv");
}

FABRIC_EXT_EXPORT void glSampleCoverage_wrapper(
  KL::Scalar value,
  KL::Boolean invert
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSampleCoverage( %f, %d );\n", (float)value, (int)invert);
#endif
  glSampleCoverage( (GLclampf)value, (GLboolean)invert );
  _checkError("glSampleCoverage");
}

FABRIC_EXT_EXPORT void glBlendColor_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue,
  KL::Scalar alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendColor( %f, %f, %f, %f );\n", (float)red, (float)green, (float)blue, (float)alpha);
#endif
  glBlendColor( (GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha );
  _checkError("glBlendColor");
}

FABRIC_EXT_EXPORT void glBlendEquation_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendEquation( %d );\n", (int)mode);
#endif
  glBlendEquation( (GLenum)mode );
  _checkError("glBlendEquation");
}

FABRIC_EXT_EXPORT void glBlendFuncSeparate_wrapper(
  KL::Size sfactorRGB,
  KL::Size dfactorRGB,
  KL::Size sfactorAlpha,
  KL::Size dfactorAlpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendFuncSeparate( %d, %d, %d, %d );\n", (int)sfactorRGB, (int)dfactorRGB, (int)sfactorAlpha, (int)dfactorAlpha);
#endif
  glBlendFuncSeparate( (GLenum)sfactorRGB, (GLenum)dfactorRGB, (GLenum)sfactorAlpha, (GLenum)dfactorAlpha );
  _checkError("glBlendFuncSeparate");
}

FABRIC_EXT_EXPORT void glFogCoordPointer_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogCoordPointer( %d, %d, GLvoid* );\n", (int)type, (int)stride);
#endif
  glFogCoordPointer( (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glFogCoordPointer");
}

FABRIC_EXT_EXPORT void glFogCoordd_wrapper(
  KL::Scalar coord
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogCoordd( %f );\n", (float)coord);
#endif
  glFogCoordd( (GLdouble)coord );
  _checkError("glFogCoordd");
}

FABRIC_EXT_EXPORT void glFogCoorddv_wrapper(
  const KL::VariableArray<KL::Scalar> & coord
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogCoorddv( GLdouble* );\n");
#endif
  glFogCoorddv( (const GLdouble*)&coord[0] );
  _checkError("glFogCoorddv");
}

FABRIC_EXT_EXPORT void glFogCoordf_wrapper(
  KL::Scalar coord
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogCoordf( %f );\n", (float)coord);
#endif
  glFogCoordf( (GLfloat)coord );
  _checkError("glFogCoordf");
}

FABRIC_EXT_EXPORT void glFogCoordfv_wrapper(
  const KL::VariableArray<KL::Scalar> & coord
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogCoordfv( GLfloat* );\n");
#endif
  glFogCoordfv( (const GLfloat*)&coord[0] );
  _checkError("glFogCoordfv");
}

FABRIC_EXT_EXPORT void glMultiDrawArrays_wrapper(
  KL::Size mode,
  KL::VariableArray<KL::Integer> & first,
  KL::VariableArray<KL::Size> & count,
  KL::Size primcount
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiDrawArrays( %d, GLint*, GLsizei*, %d );\n", (int)mode, (int)primcount);
#endif
  glMultiDrawArrays( (GLenum)mode, (GLint*)&first[0], (GLsizei*)&count[0], (GLsizei)primcount );
  _checkError("glMultiDrawArrays");
}

FABRIC_EXT_EXPORT void glPointParameterf_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPointParameterf( %d, %f );\n", (int)pname, (float)param);
#endif
  glPointParameterf( (GLenum)pname, (GLfloat)param );
  _checkError("glPointParameterf");
}

FABRIC_EXT_EXPORT void glPointParameterfv_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPointParameterfv( %d, GLfloat* );\n", (int)pname);
#endif
  glPointParameterfv( (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glPointParameterfv");
}

FABRIC_EXT_EXPORT void glPointParameteri_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPointParameteri( %d, %d );\n", (int)pname, (int)param);
#endif
  glPointParameteri( (GLenum)pname, (GLint)param );
  _checkError("glPointParameteri");
}

FABRIC_EXT_EXPORT void glPointParameteriv_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPointParameteriv( %d, GLint* );\n", (int)pname);
#endif
  glPointParameteriv( (GLenum)pname, (GLint*)&params[0] );
  _checkError("glPointParameteriv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3b_wrapper(
  KL::Byte red,
  KL::Byte green,
  KL::Byte blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3b( %d, %d, %d );\n", (int)red, (int)green, (int)blue);
#endif
  glSecondaryColor3b( (GLbyte)red, (GLbyte)green, (GLbyte)blue );
  _checkError("glSecondaryColor3b");
}

FABRIC_EXT_EXPORT void glSecondaryColor3bv_wrapper(
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3bv( GLbyte* );\n");
#endif
  glSecondaryColor3bv( (const GLbyte*)&v[0] );
  _checkError("glSecondaryColor3bv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3d_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3d( %f, %f, %f );\n", (float)red, (float)green, (float)blue);
#endif
  glSecondaryColor3d( (GLdouble)red, (GLdouble)green, (GLdouble)blue );
  _checkError("glSecondaryColor3d");
}

FABRIC_EXT_EXPORT void glSecondaryColor3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3dv( GLdouble* );\n");
#endif
  glSecondaryColor3dv( (const GLdouble*)&v[0] );
  _checkError("glSecondaryColor3dv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3f_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3f( %f, %f, %f );\n", (float)red, (float)green, (float)blue);
#endif
  glSecondaryColor3f( (GLfloat)red, (GLfloat)green, (GLfloat)blue );
  _checkError("glSecondaryColor3f");
}

FABRIC_EXT_EXPORT void glSecondaryColor3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3fv( GLfloat* );\n");
#endif
  glSecondaryColor3fv( (const GLfloat*)&v[0] );
  _checkError("glSecondaryColor3fv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3i_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3i( %d, %d, %d );\n", (int)red, (int)green, (int)blue);
#endif
  glSecondaryColor3i( (GLint)red, (GLint)green, (GLint)blue );
  _checkError("glSecondaryColor3i");
}

FABRIC_EXT_EXPORT void glSecondaryColor3iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3iv( GLint* );\n");
#endif
  glSecondaryColor3iv( (const GLint*)&v[0] );
  _checkError("glSecondaryColor3iv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3s_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3s( %d, %d, %d );\n", (int)red, (int)green, (int)blue);
#endif
  glSecondaryColor3s( (GLshort)red, (GLshort)green, (GLshort)blue );
  _checkError("glSecondaryColor3s");
}

FABRIC_EXT_EXPORT void glSecondaryColor3sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3sv( GLshort* );\n");
#endif
  glSecondaryColor3sv( (const GLshort*)&v[0] );
  _checkError("glSecondaryColor3sv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3ub_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3ub( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue);
#endif
  glSecondaryColor3ub( (GLubyte)red, (GLubyte)green, (GLubyte)blue );
  _checkError("glSecondaryColor3ub");
}

FABRIC_EXT_EXPORT void glSecondaryColor3ubv_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3ubv( GLubyte* );\n");
#endif
  glSecondaryColor3ubv( (const GLubyte*)&v[0] );
  _checkError("glSecondaryColor3ubv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3ui_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3ui( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue);
#endif
  glSecondaryColor3ui( (GLuint)red, (GLuint)green, (GLuint)blue );
  _checkError("glSecondaryColor3ui");
}

FABRIC_EXT_EXPORT void glSecondaryColor3uiv_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3uiv( GLuint* );\n");
#endif
  glSecondaryColor3uiv( (const GLuint*)&v[0] );
  _checkError("glSecondaryColor3uiv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3us_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3us( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue);
#endif
  glSecondaryColor3us( (GLushort)red, (GLushort)green, (GLushort)blue );
  _checkError("glSecondaryColor3us");
}

FABRIC_EXT_EXPORT void glSecondaryColor3usv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3usv( GLushort* );\n");
#endif
  glSecondaryColor3usv( (const GLushort*)&v[0] );
  _checkError("glSecondaryColor3usv");
}

FABRIC_EXT_EXPORT void glSecondaryColorPointer_wrapper(
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColorPointer( %d, %d, %d, GLvoid* );\n", (int)size, (int)type, (int)stride);
#endif
  glSecondaryColorPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glSecondaryColorPointer");
}

FABRIC_EXT_EXPORT void glWindowPos2d_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2d( %f, %f );\n", (float)x, (float)y);
#endif
  glWindowPos2d( (GLdouble)x, (GLdouble)y );
  _checkError("glWindowPos2d");
}

FABRIC_EXT_EXPORT void glWindowPos2dv_wrapper(
  const KL::VariableArray<KL::Scalar> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2dv( GLdouble* );\n");
#endif
  glWindowPos2dv( (const GLdouble*)&p[0] );
  _checkError("glWindowPos2dv");
}

FABRIC_EXT_EXPORT void glWindowPos2f_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2f( %f, %f );\n", (float)x, (float)y);
#endif
  glWindowPos2f( (GLfloat)x, (GLfloat)y );
  _checkError("glWindowPos2f");
}

FABRIC_EXT_EXPORT void glWindowPos2fv_wrapper(
  const KL::VariableArray<KL::Scalar> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2fv( GLfloat* );\n");
#endif
  glWindowPos2fv( (const GLfloat*)&p[0] );
  _checkError("glWindowPos2fv");
}

FABRIC_EXT_EXPORT void glWindowPos2i_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2i( %d, %d );\n", (int)x, (int)y);
#endif
  glWindowPos2i( (GLint)x, (GLint)y );
  _checkError("glWindowPos2i");
}

FABRIC_EXT_EXPORT void glWindowPos2iv_wrapper(
  const KL::VariableArray<KL::Integer> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2iv( GLint* );\n");
#endif
  glWindowPos2iv( (const GLint*)&p[0] );
  _checkError("glWindowPos2iv");
}

FABRIC_EXT_EXPORT void glWindowPos2s_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2s( %d, %d );\n", (int)x, (int)y);
#endif
  glWindowPos2s( (GLshort)x, (GLshort)y );
  _checkError("glWindowPos2s");
}

FABRIC_EXT_EXPORT void glWindowPos2sv_wrapper(
  const KL::VariableArray<KL::Integer> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2sv( GLshort* );\n");
#endif
  glWindowPos2sv( (const GLshort*)&p[0] );
  _checkError("glWindowPos2sv");
}

FABRIC_EXT_EXPORT void glWindowPos3d_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3d( %f, %f, %f );\n", (float)x, (float)y, (float)z);
#endif
  glWindowPos3d( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glWindowPos3d");
}

FABRIC_EXT_EXPORT void glWindowPos3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3dv( GLdouble* );\n");
#endif
  glWindowPos3dv( (const GLdouble*)&p[0] );
  _checkError("glWindowPos3dv");
}

FABRIC_EXT_EXPORT void glWindowPos3f_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3f( %f, %f, %f );\n", (float)x, (float)y, (float)z);
#endif
  glWindowPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glWindowPos3f");
}

FABRIC_EXT_EXPORT void glWindowPos3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3fv( GLfloat* );\n");
#endif
  glWindowPos3fv( (const GLfloat*)&p[0] );
  _checkError("glWindowPos3fv");
}

FABRIC_EXT_EXPORT void glWindowPos3i_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3i( %d, %d, %d );\n", (int)x, (int)y, (int)z);
#endif
  glWindowPos3i( (GLint)x, (GLint)y, (GLint)z );
  _checkError("glWindowPos3i");
}

FABRIC_EXT_EXPORT void glWindowPos3iv_wrapper(
  const KL::VariableArray<KL::Integer> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3iv( GLint* );\n");
#endif
  glWindowPos3iv( (const GLint*)&p[0] );
  _checkError("glWindowPos3iv");
}

FABRIC_EXT_EXPORT void glWindowPos3s_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3s( %d, %d, %d );\n", (int)x, (int)y, (int)z);
#endif
  glWindowPos3s( (GLshort)x, (GLshort)y, (GLshort)z );
  _checkError("glWindowPos3s");
}

FABRIC_EXT_EXPORT void glWindowPos3sv_wrapper(
  const KL::VariableArray<KL::Integer> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3sv( GLshort* );\n");
#endif
  glWindowPos3sv( (const GLshort*)&p[0] );
  _checkError("glWindowPos3sv");
}

FABRIC_EXT_EXPORT void glBeginQuery_wrapper(
  KL::Size target,
  KL::Size id
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBeginQuery( %d, 0x%04X );\n", (int)target, (unsigned)id);
#endif
  glBeginQuery( (GLenum)target, (GLuint)id );
  _checkError("glBeginQuery");
}

FABRIC_EXT_EXPORT void glBindBuffer_wrapper(
  KL::Size target,
  KL::Size buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindBuffer( %d, 0x%04X );\n", (int)target, (unsigned)buffer);
#endif
  glBindBuffer( (GLenum)target, (GLuint)buffer );
  _checkError("glBindBuffer");
}

FABRIC_EXT_EXPORT void glBufferData_wrapper(
  KL::Size target,
  KL::Size size,
  KL::Data data,
  KL::Size usage
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBufferData( %d, %d, GLvoid*, %d );\n", (int)target, (int)size, (int)usage);
#endif
  glBufferData( (GLenum)target, (GLsizeiptr)size, data, (GLenum)usage );
  _checkError("glBufferData");
}

FABRIC_EXT_EXPORT void glBufferSubData_wrapper(
  KL::Size target,
  KL::Integer offset,
  KL::Size size,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBufferSubData( %d, %d, %d, GLvoid* );\n", (int)target, (int)offset, (int)size);
#endif
  glBufferSubData( (GLenum)target, (GLintptr)offset, (GLsizeiptr)size, data );
  _checkError("glBufferSubData");
}

FABRIC_EXT_EXPORT void glDeleteBuffers_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & buffers
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteBuffers( %d, GLuint* );\n", (int)n);
#endif
  glDeleteBuffers( (GLsizei)n, (const GLuint*)&buffers[0] );
  _checkError("glDeleteBuffers");
}

FABRIC_EXT_EXPORT void glDeleteQueries_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & ids
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteQueries( %d, GLuint* );\n", (int)n);
#endif
  glDeleteQueries( (GLsizei)n, (const GLuint*)&ids[0] );
  _checkError("glDeleteQueries");
}

FABRIC_EXT_EXPORT void glEndQuery_wrapper(
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEndQuery( %d );\n", (int)target);
#endif
  glEndQuery( (GLenum)target );
  _checkError("glEndQuery");
}

FABRIC_EXT_EXPORT void glGenBuffers_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & buffers
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenBuffers( %d, GLuint* );\n", (int)n);
#endif
  glGenBuffers( (GLsizei)n, (GLuint*)&buffers[0] );
  _checkError("glGenBuffers");
}

FABRIC_EXT_EXPORT void glGenQueries_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & ids
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenQueries( %d, GLuint* );\n", (int)n);
#endif
  glGenQueries( (GLsizei)n, (GLuint*)&ids[0] );
  _checkError("glGenQueries");
}

FABRIC_EXT_EXPORT void glGetBufferParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetBufferParameteriv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetBufferParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetBufferParameteriv");
}

FABRIC_EXT_EXPORT void glGetBufferSubData_wrapper(
  KL::Size target,
  KL::Integer offset,
  KL::Size size,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetBufferSubData( %d, %d, %d, GLvoid* );\n", (int)target, (int)offset, (int)size);
#endif
  glGetBufferSubData( (GLenum)target, (GLintptr)offset, (GLsizeiptr)size, data );
  _checkError("glGetBufferSubData");
}

FABRIC_EXT_EXPORT void glGetQueryObjectiv_wrapper(
  KL::Size id,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetQueryObjectiv( 0x%04X, %d, GLint* );\n", (unsigned)id, (int)pname);
#endif
  glGetQueryObjectiv( (GLuint)id, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetQueryObjectiv");
}

FABRIC_EXT_EXPORT void glGetQueryObjectuiv_wrapper(
  KL::Size id,
  KL::Size pname,
  KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetQueryObjectuiv( 0x%04X, %d, GLuint* );\n", (unsigned)id, (int)pname);
#endif
  glGetQueryObjectuiv( (GLuint)id, (GLenum)pname, (GLuint*)&params[0] );
  _checkError("glGetQueryObjectuiv");
}

FABRIC_EXT_EXPORT void glGetQueryiv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetQueryiv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetQueryiv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetQueryiv");
}

FABRIC_EXT_EXPORT KL::Boolean glIsBuffer_wrapper(
  KL::Size buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsBuffer( 0x%04X );\n", (unsigned)buffer);
#endif
  GLboolean result = glIsBuffer( (GLuint)buffer );
  _checkError("glIsBuffer");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glIsQuery_wrapper(
  KL::Size id
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsQuery( 0x%04X );\n", (unsigned)id);
#endif
  GLboolean result = glIsQuery( (GLuint)id );
  _checkError("glIsQuery");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT GLvoid* glMapBuffer_wrapper(
  KL::Size target,
  KL::Size access
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMapBuffer( %d, %d );\n", (int)target, (int)access);
#endif
  glMapBuffer( (GLenum)target, (GLenum)access );
  _checkError("glMapBuffer");
}

FABRIC_EXT_EXPORT KL::Boolean glUnmapBuffer_wrapper(
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUnmapBuffer( %d );\n", (int)target);
#endif
  GLboolean result = glUnmapBuffer( (GLenum)target );
  _checkError("glUnmapBuffer");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glAttachShader_wrapper(
  KL::Size program,
  KL::Size shader
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glAttachShader( 0x%04X, 0x%04X );\n", (unsigned)program, (unsigned)shader);
#endif
  glAttachShader( (GLuint)program, (GLuint)shader );
  _checkError("glAttachShader");
}

FABRIC_EXT_EXPORT void glBindAttribLocation_wrapper(
  KL::Size program,
  KL::Size index,
  const KL::String &name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindAttribLocation( 0x%04X, 0x%04X, char* );\n", (unsigned)program, (unsigned)index);
#endif
  glBindAttribLocation( (GLuint)program, (GLuint)index, (const GLchar*)name.data() );
  _checkError("glBindAttribLocation");
}

FABRIC_EXT_EXPORT void glBlendEquationSeparate_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendEquationSeparate( %d, %d );\n", (int)var0, (int)var1);
#endif
  glBlendEquationSeparate( (GLenum)var0, (GLenum)var1 );
  _checkError("glBlendEquationSeparate");
}

FABRIC_EXT_EXPORT void glCompileShader_wrapper(
  KL::Size shader
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompileShader( 0x%04X );\n", (unsigned)shader);
#endif
  glCompileShader( (GLuint)shader );
  _checkError("glCompileShader");
}

FABRIC_EXT_EXPORT KL::Size glCreateProgram_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCreateProgram(  );\n");
#endif
  GLuint result = glCreateProgram();
  _checkError("glCreateProgram");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size glCreateShader_wrapper(
  KL::Size type
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCreateShader( %d );\n", (int)type);
#endif
  GLuint result = glCreateShader( (GLenum)type );
  _checkError("glCreateShader");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glDeleteProgram_wrapper(
  KL::Size program
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteProgram( 0x%04X );\n", (unsigned)program);
#endif
  glDeleteProgram( (GLuint)program );
  _checkError("glDeleteProgram");
}

FABRIC_EXT_EXPORT void glDeleteShader_wrapper(
  KL::Size shader
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteShader( 0x%04X );\n", (unsigned)shader);
#endif
  glDeleteShader( (GLuint)shader );
  _checkError("glDeleteShader");
}

FABRIC_EXT_EXPORT void glDetachShader_wrapper(
  KL::Size program,
  KL::Size shader
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDetachShader( 0x%04X, 0x%04X );\n", (unsigned)program, (unsigned)shader);
#endif
  glDetachShader( (GLuint)program, (GLuint)shader );
  _checkError("glDetachShader");
}

FABRIC_EXT_EXPORT void glDisableVertexAttribArray_wrapper(
  KL::Size var0
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDisableVertexAttribArray( 0x%04X );\n", (unsigned)var0);
#endif
  glDisableVertexAttribArray( (GLuint)var0 );
  _checkError("glDisableVertexAttribArray");
}

FABRIC_EXT_EXPORT void glDrawBuffers_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & bufs
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawBuffers( %d, GLenum* );\n", (int)n);
#endif
  glDrawBuffers( (GLsizei)n, (const GLenum*)&bufs[0] );
  _checkError("glDrawBuffers");
}

FABRIC_EXT_EXPORT void glEnableVertexAttribArray_wrapper(
  KL::Size var0
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEnableVertexAttribArray( 0x%04X );\n", (unsigned)var0);
#endif
  glEnableVertexAttribArray( (GLuint)var0 );
  _checkError("glEnableVertexAttribArray");
}

FABRIC_EXT_EXPORT void glGetActiveAttrib_wrapper(
  KL::Size program,
  KL::Size index,
  KL::Size maxLength,
  KL::VariableArray<KL::Size> & length,
  KL::VariableArray<KL::Integer> & size,
  KL::VariableArray<KL::Size> & type,
  KL::String & name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetActiveAttrib( 0x%04X, 0x%04X, %d, GLsizei*, GLint*, GLenum*, char* );\n", (unsigned)program, (unsigned)index, (int)maxLength);
#endif
  char * nameStr = new char[1024];
  glGetActiveAttrib( (GLuint)program, (GLuint)index, (GLsizei)maxLength, (GLsizei*)&length[0], (GLint*)&size[0], (GLenum*)&type[0], (GLchar*)nameStr );
  _checkError("glGetActiveAttrib");
  name = KL::String((const char*)nameStr);
  delete( nameStr );
}

FABRIC_EXT_EXPORT void glGetActiveUniform_wrapper(
  KL::Size program,
  KL::Size index,
  KL::Size maxLength,
  KL::VariableArray<KL::Size> & length,
  KL::VariableArray<KL::Integer> & size,
  KL::VariableArray<KL::Size> & type,
  KL::String & name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetActiveUniform( 0x%04X, 0x%04X, %d, GLsizei*, GLint*, GLenum*, char* );\n", (unsigned)program, (unsigned)index, (int)maxLength);
#endif
  char * nameStr = new char[1024];
  glGetActiveUniform( (GLuint)program, (GLuint)index, (GLsizei)maxLength, (GLsizei*)&length[0], (GLint*)&size[0], (GLenum*)&type[0], (GLchar*)nameStr );
  _checkError("glGetActiveUniform");
  name = KL::String((const char*)nameStr);
  delete( nameStr );
}

FABRIC_EXT_EXPORT void glGetAttachedShaders_wrapper(
  KL::Size program,
  KL::Size maxCount,
  KL::VariableArray<KL::Size> & count,
  KL::VariableArray<KL::Size> & shaders
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetAttachedShaders( 0x%04X, %d, GLsizei*, GLuint* );\n", (unsigned)program, (int)maxCount);
#endif
  glGetAttachedShaders( (GLuint)program, (GLsizei)maxCount, (GLsizei*)&count[0], (GLuint*)&shaders[0] );
  _checkError("glGetAttachedShaders");
}

FABRIC_EXT_EXPORT KL::Integer glGetAttribLocation_wrapper(
  KL::Size program,
  const KL::String &name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetAttribLocation( 0x%04X, char* );\n", (unsigned)program);
#endif
  GLint result = glGetAttribLocation( (GLuint)program, (const GLchar*)name.data() );
  _checkError("glGetAttribLocation");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT void glGetProgramInfoLog_wrapper(
  KL::Size program,
  KL::Size bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::String & infoLog
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetProgramInfoLog( 0x%04X, %d, GLsizei*, char* );\n", (unsigned)program, (int)bufSize);
#endif
  char * infoLogStr = new char[1024];
  glGetProgramInfoLog( (GLuint)program, (GLsizei)bufSize, (GLsizei*)&length[0], (GLchar*)infoLogStr );
  _checkError("glGetProgramInfoLog");
  infoLog = KL::String((const char*)infoLogStr);
  delete( infoLogStr );
}

FABRIC_EXT_EXPORT void glGetProgramiv_wrapper(
  KL::Size program,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetProgramiv( 0x%04X, %d, GLint* );\n", (unsigned)program, (int)pname);
#endif
  glGetProgramiv( (GLuint)program, (GLenum)pname, (GLint*)&param[0] );
  _checkError("glGetProgramiv");
}

FABRIC_EXT_EXPORT void glGetShaderInfoLog_wrapper(
  KL::Size shader,
  KL::Size bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::String & infoLog
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetShaderInfoLog( 0x%04X, %d, GLsizei*, char* );\n", (unsigned)shader, (int)bufSize);
#endif
  char * infoLogStr = new char[1024];
  glGetShaderInfoLog( (GLuint)shader, (GLsizei)bufSize, (GLsizei*)&length[0], (GLchar*)infoLogStr );
  _checkError("glGetShaderInfoLog");
  infoLog = KL::String((const char*)infoLogStr);
  delete( infoLogStr );
}

FABRIC_EXT_EXPORT void glGetShaderSource_wrapper(
  KL::Integer obj,
  KL::Size maxLength,
  KL::VariableArray<KL::Size> & length,
  KL::String & source
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetShaderSource( %d, %d, GLsizei*, char* );\n", (int)obj, (int)maxLength);
#endif
  char * sourceStr = new char[1024];
  glGetShaderSource( (GLint)obj, (GLsizei)maxLength, (GLsizei*)&length[0], (GLchar*)sourceStr );
  _checkError("glGetShaderSource");
  source = KL::String((const char*)sourceStr);
  delete( sourceStr );
}

FABRIC_EXT_EXPORT void glGetShaderiv_wrapper(
  KL::Size shader,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetShaderiv( 0x%04X, %d, GLint* );\n", (unsigned)shader, (int)pname);
#endif
  glGetShaderiv( (GLuint)shader, (GLenum)pname, (GLint*)&param[0] );
  _checkError("glGetShaderiv");
}

FABRIC_EXT_EXPORT KL::Integer glGetUniformLocation_wrapper(
  KL::Size program,
  const KL::String &name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetUniformLocation( 0x%04X, char* );\n", (unsigned)program);
#endif
  GLint result = glGetUniformLocation( (GLuint)program, (const GLchar*)name.data() );
  _checkError("glGetUniformLocation");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT void glGetUniformfv_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetUniformfv( 0x%04X, %d, GLfloat* );\n", (unsigned)program, (int)location);
#endif
  glGetUniformfv( (GLuint)program, (GLint)location, (GLfloat*)&params[0] );
  _checkError("glGetUniformfv");
}

FABRIC_EXT_EXPORT void glGetUniformiv_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetUniformiv( 0x%04X, %d, GLint* );\n", (unsigned)program, (int)location);
#endif
  glGetUniformiv( (GLuint)program, (GLint)location, (GLint*)&params[0] );
  _checkError("glGetUniformiv");
}

FABRIC_EXT_EXPORT void glGetVertexAttribPointerv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::Data var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexAttribPointerv( 0x%04X, %d, GLvoid* );\n", (unsigned)var0, (int)var1);
#endif
  glGetVertexAttribPointerv( (GLuint)var0, (GLenum)var1, var2 );
  _checkError("glGetVertexAttribPointerv");
}

FABRIC_EXT_EXPORT void glGetVertexAttribdv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Scalar> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexAttribdv( 0x%04X, %d, GLdouble* );\n", (unsigned)var0, (int)var1);
#endif
  glGetVertexAttribdv( (GLuint)var0, (GLenum)var1, (GLdouble*)&var2[0] );
  _checkError("glGetVertexAttribdv");
}

FABRIC_EXT_EXPORT void glGetVertexAttribfv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Scalar> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexAttribfv( 0x%04X, %d, GLfloat* );\n", (unsigned)var0, (int)var1);
#endif
  glGetVertexAttribfv( (GLuint)var0, (GLenum)var1, (GLfloat*)&var2[0] );
  _checkError("glGetVertexAttribfv");
}

FABRIC_EXT_EXPORT void glGetVertexAttribiv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Integer> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexAttribiv( 0x%04X, %d, GLint* );\n", (unsigned)var0, (int)var1);
#endif
  glGetVertexAttribiv( (GLuint)var0, (GLenum)var1, (GLint*)&var2[0] );
  _checkError("glGetVertexAttribiv");
}

FABRIC_EXT_EXPORT KL::Boolean glIsProgram_wrapper(
  KL::Size program
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsProgram( 0x%04X );\n", (unsigned)program);
#endif
  GLboolean result = glIsProgram( (GLuint)program );
  _checkError("glIsProgram");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glIsShader_wrapper(
  KL::Size shader
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsShader( 0x%04X );\n", (unsigned)shader);
#endif
  GLboolean result = glIsShader( (GLuint)shader );
  _checkError("glIsShader");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glLinkProgram_wrapper(
  KL::Size program
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLinkProgram( 0x%04X );\n", (unsigned)program);
#endif
  glLinkProgram( (GLuint)program );
  _checkError("glLinkProgram");
}

FABRIC_EXT_EXPORT void glShaderSource_wrapper(
  KL::Size shader,
  KL::Size count,
  const KL::VariableArray<KL::String> &strings,
  const KL::VariableArray<KL::Integer> & lengths
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glShaderSource( 0x%04X, %d, char*, GLint* );\n", (unsigned)shader, (int)count);
#endif
  KL::VariableArray<KL::Data> stringsData;
  stringsData.resize(strings.size());
  for(KL::Size stringsCounter = 0;stringsCounter<strings.size();stringsCounter++)
    stringsData[stringsCounter] = (KL::Data)strings[stringsCounter].data();
  glShaderSource( (GLuint)shader, (GLsizei)count, (const GLchar**)&stringsData[0], (const GLint*)&lengths[0] );
  _checkError("glShaderSource");
}

FABRIC_EXT_EXPORT void glStencilFuncSeparate_wrapper(
  KL::Size frontfunc,
  KL::Size backfunc,
  KL::Integer ref,
  KL::Size mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glStencilFuncSeparate( %d, %d, %d, 0x%04X );\n", (int)frontfunc, (int)backfunc, (int)ref, (unsigned)mask);
#endif
  glStencilFuncSeparate( (GLenum)frontfunc, (GLenum)backfunc, (GLint)ref, (GLuint)mask );
  _checkError("glStencilFuncSeparate");
}

FABRIC_EXT_EXPORT void glStencilMaskSeparate_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glStencilMaskSeparate( %d, 0x%04X );\n", (int)var0, (unsigned)var1);
#endif
  glStencilMaskSeparate( (GLenum)var0, (GLuint)var1 );
  _checkError("glStencilMaskSeparate");
}

FABRIC_EXT_EXPORT void glStencilOpSeparate_wrapper(
  KL::Size face,
  KL::Size sfail,
  KL::Size dpfail,
  KL::Size dppass
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glStencilOpSeparate( %d, %d, %d, %d );\n", (int)face, (int)sfail, (int)dpfail, (int)dppass);
#endif
  glStencilOpSeparate( (GLenum)face, (GLenum)sfail, (GLenum)dpfail, (GLenum)dppass );
  _checkError("glStencilOpSeparate");
}

FABRIC_EXT_EXPORT void glUniform1f_wrapper(
  KL::Integer location,
  KL::Scalar v0
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform1f( %d, %f );\n", (int)location, (float)v0);
#endif
  glUniform1f( (GLint)location, (GLfloat)v0 );
  _checkError("glUniform1f");
}

FABRIC_EXT_EXPORT void glUniform1fv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform1fv( %d, %d, GLfloat* );\n", (int)location, (int)count);
#endif
  glUniform1fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glUniform1fv");
}

FABRIC_EXT_EXPORT void glUniform1i_wrapper(
  KL::Integer location,
  KL::Integer v0
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform1i( %d, %d );\n", (int)location, (int)v0);
#endif
  glUniform1i( (GLint)location, (GLint)v0 );
  _checkError("glUniform1i");
}

FABRIC_EXT_EXPORT void glUniform1iv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform1iv( %d, %d, GLint* );\n", (int)location, (int)count);
#endif
  glUniform1iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glUniform1iv");
}

FABRIC_EXT_EXPORT void glUniform2f_wrapper(
  KL::Integer location,
  KL::Scalar v0,
  KL::Scalar v1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform2f( %d, %f, %f );\n", (int)location, (float)v0, (float)v1);
#endif
  glUniform2f( (GLint)location, (GLfloat)v0, (GLfloat)v1 );
  _checkError("glUniform2f");
}

FABRIC_EXT_EXPORT void glUniform2fv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform2fv( %d, %d, GLfloat* );\n", (int)location, (int)count);
#endif
  glUniform2fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glUniform2fv");
}

FABRIC_EXT_EXPORT void glUniform2i_wrapper(
  KL::Integer location,
  KL::Integer v0,
  KL::Integer v1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform2i( %d, %d, %d );\n", (int)location, (int)v0, (int)v1);
#endif
  glUniform2i( (GLint)location, (GLint)v0, (GLint)v1 );
  _checkError("glUniform2i");
}

FABRIC_EXT_EXPORT void glUniform2iv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform2iv( %d, %d, GLint* );\n", (int)location, (int)count);
#endif
  glUniform2iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glUniform2iv");
}

FABRIC_EXT_EXPORT void glUniform3f_wrapper(
  KL::Integer location,
  KL::Scalar v0,
  KL::Scalar v1,
  KL::Scalar v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform3f( %d, %f, %f, %f );\n", (int)location, (float)v0, (float)v1, (float)v2);
#endif
  glUniform3f( (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2 );
  _checkError("glUniform3f");
}

FABRIC_EXT_EXPORT void glUniform3fv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform3fv( %d, %d, GLfloat* );\n", (int)location, (int)count);
#endif
  glUniform3fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glUniform3fv");
}

FABRIC_EXT_EXPORT void glUniform3i_wrapper(
  KL::Integer location,
  KL::Integer v0,
  KL::Integer v1,
  KL::Integer v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform3i( %d, %d, %d, %d );\n", (int)location, (int)v0, (int)v1, (int)v2);
#endif
  glUniform3i( (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2 );
  _checkError("glUniform3i");
}

FABRIC_EXT_EXPORT void glUniform3iv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform3iv( %d, %d, GLint* );\n", (int)location, (int)count);
#endif
  glUniform3iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glUniform3iv");
}

FABRIC_EXT_EXPORT void glUniform4f_wrapper(
  KL::Integer location,
  KL::Scalar v0,
  KL::Scalar v1,
  KL::Scalar v2,
  KL::Scalar v3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform4f( %d, %f, %f, %f, %f );\n", (int)location, (float)v0, (float)v1, (float)v2, (float)v3);
#endif
  glUniform4f( (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2, (GLfloat)v3 );
  _checkError("glUniform4f");
}

FABRIC_EXT_EXPORT void glUniform4fv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform4fv( %d, %d, GLfloat* );\n", (int)location, (int)count);
#endif
  glUniform4fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glUniform4fv");
}

FABRIC_EXT_EXPORT void glUniform4i_wrapper(
  KL::Integer location,
  KL::Integer v0,
  KL::Integer v1,
  KL::Integer v2,
  KL::Integer v3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform4i( %d, %d, %d, %d, %d );\n", (int)location, (int)v0, (int)v1, (int)v2, (int)v3);
#endif
  glUniform4i( (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2, (GLint)v3 );
  _checkError("glUniform4i");
}

FABRIC_EXT_EXPORT void glUniform4iv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform4iv( %d, %d, GLint* );\n", (int)location, (int)count);
#endif
  glUniform4iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glUniform4iv");
}

FABRIC_EXT_EXPORT void glUniformMatrix2fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat22> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformMatrix2fv( %d, %d, %d, GLfloat* );\n", (int)location, (int)count, (int)transpose);
#endif
  glUniformMatrix2fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix2fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix3fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat33> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformMatrix3fv( %d, %d, %d, GLfloat* );\n", (int)location, (int)count, (int)transpose);
#endif
  glUniformMatrix3fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix3fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix4fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat44> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformMatrix4fv( %d, %d, %d, GLfloat* );\n", (int)location, (int)count, (int)transpose);
#endif
  glUniformMatrix4fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix4fv");
}

FABRIC_EXT_EXPORT void glUseProgram_wrapper(
  KL::Size program
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUseProgram( 0x%04X );\n", (unsigned)program);
#endif
  glUseProgram( (GLuint)program );
  _checkError("glUseProgram");
}

FABRIC_EXT_EXPORT void glValidateProgram_wrapper(
  KL::Size program
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glValidateProgram( 0x%04X );\n", (unsigned)program);
#endif
  glValidateProgram( (GLuint)program );
  _checkError("glValidateProgram");
}

FABRIC_EXT_EXPORT void glVertexAttrib1d_wrapper(
  KL::Size index,
  KL::Scalar x
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib1d( 0x%04X, %f );\n", (unsigned)index, (float)x);
#endif
  glVertexAttrib1d( (GLuint)index, (GLdouble)x );
  _checkError("glVertexAttrib1d");
}

FABRIC_EXT_EXPORT void glVertexAttrib1dv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib1dv( 0x%04X, GLdouble* );\n", (unsigned)index);
#endif
  glVertexAttrib1dv( (GLuint)index, (const GLdouble*)&v[0] );
  _checkError("glVertexAttrib1dv");
}

FABRIC_EXT_EXPORT void glVertexAttrib1f_wrapper(
  KL::Size index,
  KL::Scalar x
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib1f( 0x%04X, %f );\n", (unsigned)index, (float)x);
#endif
  glVertexAttrib1f( (GLuint)index, (GLfloat)x );
  _checkError("glVertexAttrib1f");
}

FABRIC_EXT_EXPORT void glVertexAttrib1fv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib1fv( 0x%04X, GLfloat* );\n", (unsigned)index);
#endif
  glVertexAttrib1fv( (GLuint)index, (const GLfloat*)&v[0] );
  _checkError("glVertexAttrib1fv");
}

FABRIC_EXT_EXPORT void glVertexAttrib1s_wrapper(
  KL::Size index,
  KL::Integer x
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib1s( 0x%04X, %d );\n", (unsigned)index, (int)x);
#endif
  glVertexAttrib1s( (GLuint)index, (GLshort)x );
  _checkError("glVertexAttrib1s");
}

FABRIC_EXT_EXPORT void glVertexAttrib1sv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib1sv( 0x%04X, GLshort* );\n", (unsigned)index);
#endif
  glVertexAttrib1sv( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib1sv");
}

FABRIC_EXT_EXPORT void glVertexAttrib2d_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib2d( 0x%04X, %f, %f );\n", (unsigned)index, (float)x, (float)y);
#endif
  glVertexAttrib2d( (GLuint)index, (GLdouble)x, (GLdouble)y );
  _checkError("glVertexAttrib2d");
}

FABRIC_EXT_EXPORT void glVertexAttrib2dv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib2dv( 0x%04X, GLdouble* );\n", (unsigned)index);
#endif
  glVertexAttrib2dv( (GLuint)index, (const GLdouble*)&v[0] );
  _checkError("glVertexAttrib2dv");
}

FABRIC_EXT_EXPORT void glVertexAttrib2f_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib2f( 0x%04X, %f, %f );\n", (unsigned)index, (float)x, (float)y);
#endif
  glVertexAttrib2f( (GLuint)index, (GLfloat)x, (GLfloat)y );
  _checkError("glVertexAttrib2f");
}

FABRIC_EXT_EXPORT void glVertexAttrib2fv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib2fv( 0x%04X, GLfloat* );\n", (unsigned)index);
#endif
  glVertexAttrib2fv( (GLuint)index, (const GLfloat*)&v[0] );
  _checkError("glVertexAttrib2fv");
}

FABRIC_EXT_EXPORT void glVertexAttrib2s_wrapper(
  KL::Size index,
  KL::Integer x,
  KL::Integer y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib2s( 0x%04X, %d, %d );\n", (unsigned)index, (int)x, (int)y);
#endif
  glVertexAttrib2s( (GLuint)index, (GLshort)x, (GLshort)y );
  _checkError("glVertexAttrib2s");
}

FABRIC_EXT_EXPORT void glVertexAttrib2sv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib2sv( 0x%04X, GLshort* );\n", (unsigned)index);
#endif
  glVertexAttrib2sv( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib2sv");
}

FABRIC_EXT_EXPORT void glVertexAttrib3d_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib3d( 0x%04X, %f, %f, %f );\n", (unsigned)index, (float)x, (float)y, (float)z);
#endif
  glVertexAttrib3d( (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glVertexAttrib3d");
}

FABRIC_EXT_EXPORT void glVertexAttrib3dv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib3dv( 0x%04X, GLdouble* );\n", (unsigned)index);
#endif
  glVertexAttrib3dv( (GLuint)index, (const GLdouble*)&v[0] );
  _checkError("glVertexAttrib3dv");
}

FABRIC_EXT_EXPORT void glVertexAttrib3f_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib3f( 0x%04X, %f, %f, %f );\n", (unsigned)index, (float)x, (float)y, (float)z);
#endif
  glVertexAttrib3f( (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glVertexAttrib3f");
}

FABRIC_EXT_EXPORT void glVertexAttrib3fv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib3fv( 0x%04X, GLfloat* );\n", (unsigned)index);
#endif
  glVertexAttrib3fv( (GLuint)index, (const GLfloat*)&v[0] );
  _checkError("glVertexAttrib3fv");
}

FABRIC_EXT_EXPORT void glVertexAttrib3s_wrapper(
  KL::Size index,
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib3s( 0x%04X, %d, %d, %d );\n", (unsigned)index, (int)x, (int)y, (int)z);
#endif
  glVertexAttrib3s( (GLuint)index, (GLshort)x, (GLshort)y, (GLshort)z );
  _checkError("glVertexAttrib3s");
}

FABRIC_EXT_EXPORT void glVertexAttrib3sv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib3sv( 0x%04X, GLshort* );\n", (unsigned)index);
#endif
  glVertexAttrib3sv( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib3sv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Nbv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4Nbv( 0x%04X, GLbyte* );\n", (unsigned)index);
#endif
  glVertexAttrib4Nbv( (GLuint)index, (const GLbyte*)&v[0] );
  _checkError("glVertexAttrib4Nbv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Niv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4Niv( 0x%04X, GLint* );\n", (unsigned)index);
#endif
  glVertexAttrib4Niv( (GLuint)index, (const GLint*)&v[0] );
  _checkError("glVertexAttrib4Niv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Nsv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4Nsv( 0x%04X, GLshort* );\n", (unsigned)index);
#endif
  glVertexAttrib4Nsv( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib4Nsv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Nub_wrapper(
  KL::Size index,
  KL::Size x,
  KL::Size y,
  KL::Size z,
  KL::Size w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4Nub( 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)index, (unsigned)x, (unsigned)y, (unsigned)z, (unsigned)w);
#endif
  glVertexAttrib4Nub( (GLuint)index, (GLubyte)x, (GLubyte)y, (GLubyte)z, (GLubyte)w );
  _checkError("glVertexAttrib4Nub");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Nubv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4Nubv( 0x%04X, GLubyte* );\n", (unsigned)index);
#endif
  glVertexAttrib4Nubv( (GLuint)index, (const GLubyte*)&v[0] );
  _checkError("glVertexAttrib4Nubv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Nuiv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4Nuiv( 0x%04X, GLuint* );\n", (unsigned)index);
#endif
  glVertexAttrib4Nuiv( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttrib4Nuiv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Nusv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4Nusv( 0x%04X, GLushort* );\n", (unsigned)index);
#endif
  glVertexAttrib4Nusv( (GLuint)index, (const GLushort*)&v[0] );
  _checkError("glVertexAttrib4Nusv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4bv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4bv( 0x%04X, GLbyte* );\n", (unsigned)index);
#endif
  glVertexAttrib4bv( (GLuint)index, (const GLbyte*)&v[0] );
  _checkError("glVertexAttrib4bv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4d_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4d( 0x%04X, %f, %f, %f, %f );\n", (unsigned)index, (float)x, (float)y, (float)z, (float)w);
#endif
  glVertexAttrib4d( (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  _checkError("glVertexAttrib4d");
}

FABRIC_EXT_EXPORT void glVertexAttrib4dv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4dv( 0x%04X, GLdouble* );\n", (unsigned)index);
#endif
  glVertexAttrib4dv( (GLuint)index, (const GLdouble*)&v[0] );
  _checkError("glVertexAttrib4dv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4f_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4f( 0x%04X, %f, %f, %f, %f );\n", (unsigned)index, (float)x, (float)y, (float)z, (float)w);
#endif
  glVertexAttrib4f( (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  _checkError("glVertexAttrib4f");
}

FABRIC_EXT_EXPORT void glVertexAttrib4fv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4fv( 0x%04X, GLfloat* );\n", (unsigned)index);
#endif
  glVertexAttrib4fv( (GLuint)index, (const GLfloat*)&v[0] );
  _checkError("glVertexAttrib4fv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4iv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4iv( 0x%04X, GLint* );\n", (unsigned)index);
#endif
  glVertexAttrib4iv( (GLuint)index, (const GLint*)&v[0] );
  _checkError("glVertexAttrib4iv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4s_wrapper(
  KL::Size index,
  KL::Integer x,
  KL::Integer y,
  KL::Integer z,
  KL::Integer w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4s( 0x%04X, %d, %d, %d, %d );\n", (unsigned)index, (int)x, (int)y, (int)z, (int)w);
#endif
  glVertexAttrib4s( (GLuint)index, (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
  _checkError("glVertexAttrib4s");
}

FABRIC_EXT_EXPORT void glVertexAttrib4sv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4sv( 0x%04X, GLshort* );\n", (unsigned)index);
#endif
  glVertexAttrib4sv( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib4sv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4ubv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4ubv( 0x%04X, GLubyte* );\n", (unsigned)index);
#endif
  glVertexAttrib4ubv( (GLuint)index, (const GLubyte*)&v[0] );
  _checkError("glVertexAttrib4ubv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4uiv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4uiv( 0x%04X, GLuint* );\n", (unsigned)index);
#endif
  glVertexAttrib4uiv( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttrib4uiv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4usv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4usv( 0x%04X, GLushort* );\n", (unsigned)index);
#endif
  glVertexAttrib4usv( (GLuint)index, (const GLushort*)&v[0] );
  _checkError("glVertexAttrib4usv");
}

FABRIC_EXT_EXPORT void glVertexAttribPointer_wrapper(
  KL::Size index,
  KL::Integer size,
  KL::Size type,
  KL::Boolean normalized,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribPointer( 0x%04X, %d, %d, %d, %d, GLvoid* );\n", (unsigned)index, (int)size, (int)type, (int)normalized, (int)stride);
#endif
  glVertexAttribPointer( (GLuint)index, (GLint)size, (GLenum)type, (GLboolean)normalized, (GLsizei)stride, pointer );
  _checkError("glVertexAttribPointer");
}

FABRIC_EXT_EXPORT void glUniformMatrix2x3fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat33> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformMatrix2x3fv( %d, %d, %d, GLfloat* );\n", (int)location, (int)count, (int)transpose);
#endif
  glUniformMatrix2x3fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix2x3fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix2x4fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat44> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformMatrix2x4fv( %d, %d, %d, GLfloat* );\n", (int)location, (int)count, (int)transpose);
#endif
  glUniformMatrix2x4fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix2x4fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix3x2fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat22> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformMatrix3x2fv( %d, %d, %d, GLfloat* );\n", (int)location, (int)count, (int)transpose);
#endif
  glUniformMatrix3x2fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix3x2fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix3x4fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat44> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformMatrix3x4fv( %d, %d, %d, GLfloat* );\n", (int)location, (int)count, (int)transpose);
#endif
  glUniformMatrix3x4fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix3x4fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix4x2fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat22> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformMatrix4x2fv( %d, %d, %d, GLfloat* );\n", (int)location, (int)count, (int)transpose);
#endif
  glUniformMatrix4x2fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix4x2fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix4x3fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat33> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformMatrix4x3fv( %d, %d, %d, GLfloat* );\n", (int)location, (int)count, (int)transpose);
#endif
  glUniformMatrix4x3fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix4x3fv");
}

FABRIC_EXT_EXPORT void glBeginConditionalRender_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBeginConditionalRender( 0x%04X, %d );\n", (unsigned)var0, (int)var1);
#endif
  glBeginConditionalRender( (GLuint)var0, (GLenum)var1 );
  _checkError("glBeginConditionalRender");
}

FABRIC_EXT_EXPORT void glBeginTransformFeedback_wrapper(
  KL::Size var0
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBeginTransformFeedback( %d );\n", (int)var0);
#endif
  glBeginTransformFeedback( (GLenum)var0 );
  _checkError("glBeginTransformFeedback");
}

FABRIC_EXT_EXPORT void glBindFragDataLocation_wrapper(
  KL::Size var0,
  KL::Size var1,
  const KL::String &var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindFragDataLocation( 0x%04X, 0x%04X, char* );\n", (unsigned)var0, (unsigned)var1);
#endif
  glBindFragDataLocation( (GLuint)var0, (GLuint)var1, (const GLchar*)var2.data() );
  _checkError("glBindFragDataLocation");
}

FABRIC_EXT_EXPORT void glClampColor_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClampColor( %d, %d );\n", (int)var0, (int)var1);
#endif
  glClampColor( (GLenum)var0, (GLenum)var1 );
  _checkError("glClampColor");
}

FABRIC_EXT_EXPORT void glClearBufferfi_wrapper(
  KL::Size var0,
  KL::Integer var1,
  KL::Scalar var2,
  KL::Integer var3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClearBufferfi( %d, %d, %f, %d );\n", (int)var0, (int)var1, (float)var2, (int)var3);
#endif
  glClearBufferfi( (GLenum)var0, (GLint)var1, (GLfloat)var2, (GLint)var3 );
  _checkError("glClearBufferfi");
}

FABRIC_EXT_EXPORT void glClearBufferfv_wrapper(
  KL::Size var0,
  KL::Integer var1,
  const KL::VariableArray<KL::Scalar> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClearBufferfv( %d, %d, GLfloat* );\n", (int)var0, (int)var1);
#endif
  glClearBufferfv( (GLenum)var0, (GLint)var1, (const GLfloat*)&var2[0] );
  _checkError("glClearBufferfv");
}

FABRIC_EXT_EXPORT void glClearBufferiv_wrapper(
  KL::Size var0,
  KL::Integer var1,
  const KL::VariableArray<KL::Integer> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClearBufferiv( %d, %d, GLint* );\n", (int)var0, (int)var1);
#endif
  glClearBufferiv( (GLenum)var0, (GLint)var1, (const GLint*)&var2[0] );
  _checkError("glClearBufferiv");
}

FABRIC_EXT_EXPORT void glClearBufferuiv_wrapper(
  KL::Size var0,
  KL::Integer var1,
  const KL::VariableArray<KL::Size> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClearBufferuiv( %d, %d, GLuint* );\n", (int)var0, (int)var1);
#endif
  glClearBufferuiv( (GLenum)var0, (GLint)var1, (const GLuint*)&var2[0] );
  _checkError("glClearBufferuiv");
}

FABRIC_EXT_EXPORT void glColorMaski_wrapper(
  KL::Size var0,
  KL::Boolean var1,
  KL::Boolean var2,
  KL::Boolean var3,
  KL::Boolean var4
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColorMaski( 0x%04X, %d, %d, %d, %d );\n", (unsigned)var0, (int)var1, (int)var2, (int)var3, (int)var4);
#endif
  glColorMaski( (GLuint)var0, (GLboolean)var1, (GLboolean)var2, (GLboolean)var3, (GLboolean)var4 );
  _checkError("glColorMaski");
}

FABRIC_EXT_EXPORT void glDisablei_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDisablei( %d, 0x%04X );\n", (int)var0, (unsigned)var1);
#endif
  glDisablei( (GLenum)var0, (GLuint)var1 );
  _checkError("glDisablei");
}

FABRIC_EXT_EXPORT void glEnablei_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEnablei( %d, 0x%04X );\n", (int)var0, (unsigned)var1);
#endif
  glEnablei( (GLenum)var0, (GLuint)var1 );
  _checkError("glEnablei");
}

FABRIC_EXT_EXPORT void glEndConditionalRender_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEndConditionalRender(  );\n");
#endif
  glEndConditionalRender();
  _checkError("glEndConditionalRender");
}

FABRIC_EXT_EXPORT void glEndTransformFeedback_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEndTransformFeedback(  );\n");
#endif
  glEndTransformFeedback();
  _checkError("glEndTransformFeedback");
}

FABRIC_EXT_EXPORT void glGetBooleani_v_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Boolean> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetBooleani_v( %d, 0x%04X, GLboolean* );\n", (int)var0, (unsigned)var1);
#endif
  glGetBooleani_v( (GLenum)var0, (GLuint)var1, (GLboolean*)&var2[0] );
  _checkError("glGetBooleani_v");
}

FABRIC_EXT_EXPORT KL::Integer glGetFragDataLocation_wrapper(
  KL::Size var0,
  const KL::String &var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetFragDataLocation( 0x%04X, char* );\n", (unsigned)var0);
#endif
  GLint result = glGetFragDataLocation( (GLuint)var0, (const GLchar*)var1.data() );
  _checkError("glGetFragDataLocation");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT void glGetTexParameterIiv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Integer> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexParameterIiv( %d, %d, GLint* );\n", (int)var0, (int)var1);
#endif
  glGetTexParameterIiv( (GLenum)var0, (GLenum)var1, (GLint*)&var2[0] );
  _checkError("glGetTexParameterIiv");
}

FABRIC_EXT_EXPORT void glGetTexParameterIuiv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Size> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexParameterIuiv( %d, %d, GLuint* );\n", (int)var0, (int)var1);
#endif
  glGetTexParameterIuiv( (GLenum)var0, (GLenum)var1, (GLuint*)&var2[0] );
  _checkError("glGetTexParameterIuiv");
}

FABRIC_EXT_EXPORT void glGetTransformFeedbackVarying_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Integer> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTransformFeedbackVarying( 0x%04X, 0x%04X, GLint* );\n", (unsigned)var0, (unsigned)var1);
#endif
  glGetTransformFeedbackVarying( (GLuint)var0, (GLuint)var1, (GLint*)&var2[0] );
  _checkError("glGetTransformFeedbackVarying");
}

FABRIC_EXT_EXPORT void glGetUniformuiv_wrapper(
  KL::Size var0,
  KL::Integer var1,
  KL::VariableArray<KL::Size> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetUniformuiv( 0x%04X, %d, GLuint* );\n", (unsigned)var0, (int)var1);
#endif
  glGetUniformuiv( (GLuint)var0, (GLint)var1, (GLuint*)&var2[0] );
  _checkError("glGetUniformuiv");
}

FABRIC_EXT_EXPORT void glGetVertexAttribIiv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Integer> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexAttribIiv( 0x%04X, %d, GLint* );\n", (unsigned)var0, (int)var1);
#endif
  glGetVertexAttribIiv( (GLuint)var0, (GLenum)var1, (GLint*)&var2[0] );
  _checkError("glGetVertexAttribIiv");
}

FABRIC_EXT_EXPORT void glGetVertexAttribIuiv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Size> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexAttribIuiv( 0x%04X, %d, GLuint* );\n", (unsigned)var0, (int)var1);
#endif
  glGetVertexAttribIuiv( (GLuint)var0, (GLenum)var1, (GLuint*)&var2[0] );
  _checkError("glGetVertexAttribIuiv");
}

FABRIC_EXT_EXPORT KL::Boolean glIsEnabledi_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsEnabledi( %d, 0x%04X );\n", (int)var0, (unsigned)var1);
#endif
  GLboolean result = glIsEnabledi( (GLenum)var0, (GLuint)var1 );
  _checkError("glIsEnabledi");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glTexParameterIiv_wrapper(
  KL::Size var0,
  KL::Size var1,
  const KL::VariableArray<KL::Integer> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexParameterIiv( %d, %d, GLint* );\n", (int)var0, (int)var1);
#endif
  glTexParameterIiv( (GLenum)var0, (GLenum)var1, (const GLint*)&var2[0] );
  _checkError("glTexParameterIiv");
}

FABRIC_EXT_EXPORT void glTexParameterIuiv_wrapper(
  KL::Size var0,
  KL::Size var1,
  const KL::VariableArray<KL::Size> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexParameterIuiv( %d, %d, GLuint* );\n", (int)var0, (int)var1);
#endif
  glTexParameterIuiv( (GLenum)var0, (GLenum)var1, (const GLuint*)&var2[0] );
  _checkError("glTexParameterIuiv");
}

FABRIC_EXT_EXPORT void glTransformFeedbackVaryings_wrapper(
  KL::Size var0,
  KL::Size var1,
  const KL::VariableArray<KL::String> &var2,
  KL::Size var3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTransformFeedbackVaryings( 0x%04X, %d, char*, %d );\n", (unsigned)var0, (int)var1, (int)var3);
#endif
  KL::VariableArray<KL::Data> var2Data;
  var2Data.resize(var2.size());
  for(KL::Size var2Counter = 0;var2Counter<var2.size();var2Counter++)
    var2Data[var2Counter] = (KL::Data)var2[var2Counter].data();
  glTransformFeedbackVaryings( (GLuint)var0, (GLsizei)var1, (const GLchar**)&var2Data[0], (GLenum)var3 );
  _checkError("glTransformFeedbackVaryings");
}

FABRIC_EXT_EXPORT void glUniform1ui_wrapper(
  KL::Integer var0,
  KL::Size var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform1ui( %d, 0x%04X );\n", (int)var0, (unsigned)var1);
#endif
  glUniform1ui( (GLint)var0, (GLuint)var1 );
  _checkError("glUniform1ui");
}

FABRIC_EXT_EXPORT void glUniform1uiv_wrapper(
  KL::Integer var0,
  KL::Size var1,
  const KL::VariableArray<KL::Size> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform1uiv( %d, %d, GLuint* );\n", (int)var0, (int)var1);
#endif
  glUniform1uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
  _checkError("glUniform1uiv");
}

FABRIC_EXT_EXPORT void glUniform2ui_wrapper(
  KL::Integer var0,
  KL::Size var1,
  KL::Size var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform2ui( %d, 0x%04X, 0x%04X );\n", (int)var0, (unsigned)var1, (unsigned)var2);
#endif
  glUniform2ui( (GLint)var0, (GLuint)var1, (GLuint)var2 );
  _checkError("glUniform2ui");
}

FABRIC_EXT_EXPORT void glUniform2uiv_wrapper(
  KL::Integer var0,
  KL::Size var1,
  const KL::VariableArray<KL::Size> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform2uiv( %d, %d, GLuint* );\n", (int)var0, (int)var1);
#endif
  glUniform2uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
  _checkError("glUniform2uiv");
}

FABRIC_EXT_EXPORT void glUniform3ui_wrapper(
  KL::Integer var0,
  KL::Size var1,
  KL::Size var2,
  KL::Size var3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform3ui( %d, 0x%04X, 0x%04X, 0x%04X );\n", (int)var0, (unsigned)var1, (unsigned)var2, (unsigned)var3);
#endif
  glUniform3ui( (GLint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3 );
  _checkError("glUniform3ui");
}

FABRIC_EXT_EXPORT void glUniform3uiv_wrapper(
  KL::Integer var0,
  KL::Size var1,
  const KL::VariableArray<KL::Size> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform3uiv( %d, %d, GLuint* );\n", (int)var0, (int)var1);
#endif
  glUniform3uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
  _checkError("glUniform3uiv");
}

FABRIC_EXT_EXPORT void glUniform4ui_wrapper(
  KL::Integer var0,
  KL::Size var1,
  KL::Size var2,
  KL::Size var3,
  KL::Size var4
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform4ui( %d, 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (int)var0, (unsigned)var1, (unsigned)var2, (unsigned)var3, (unsigned)var4);
#endif
  glUniform4ui( (GLint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3, (GLuint)var4 );
  _checkError("glUniform4ui");
}

FABRIC_EXT_EXPORT void glUniform4uiv_wrapper(
  KL::Integer var0,
  KL::Size var1,
  const KL::VariableArray<KL::Size> & var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform4uiv( %d, %d, GLuint* );\n", (int)var0, (int)var1);
#endif
  glUniform4uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
  _checkError("glUniform4uiv");
}

FABRIC_EXT_EXPORT void glVertexAttribI1i_wrapper(
  KL::Size var0,
  KL::Integer var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI1i( 0x%04X, %d );\n", (unsigned)var0, (int)var1);
#endif
  glVertexAttribI1i( (GLuint)var0, (GLint)var1 );
  _checkError("glVertexAttribI1i");
}

FABRIC_EXT_EXPORT void glVertexAttribI1iv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Integer> & var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI1iv( 0x%04X, GLint* );\n", (unsigned)var0);
#endif
  glVertexAttribI1iv( (GLuint)var0, (const GLint*)&var1[0] );
  _checkError("glVertexAttribI1iv");
}

FABRIC_EXT_EXPORT void glVertexAttribI1ui_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI1ui( 0x%04X, 0x%04X );\n", (unsigned)var0, (unsigned)var1);
#endif
  glVertexAttribI1ui( (GLuint)var0, (GLuint)var1 );
  _checkError("glVertexAttribI1ui");
}

FABRIC_EXT_EXPORT void glVertexAttribI1uiv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Size> & var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI1uiv( 0x%04X, GLuint* );\n", (unsigned)var0);
#endif
  glVertexAttribI1uiv( (GLuint)var0, (const GLuint*)&var1[0] );
  _checkError("glVertexAttribI1uiv");
}

FABRIC_EXT_EXPORT void glVertexAttribI2i_wrapper(
  KL::Size var0,
  KL::Integer var1,
  KL::Integer var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI2i( 0x%04X, %d, %d );\n", (unsigned)var0, (int)var1, (int)var2);
#endif
  glVertexAttribI2i( (GLuint)var0, (GLint)var1, (GLint)var2 );
  _checkError("glVertexAttribI2i");
}

FABRIC_EXT_EXPORT void glVertexAttribI2iv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Integer> & var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI2iv( 0x%04X, GLint* );\n", (unsigned)var0);
#endif
  glVertexAttribI2iv( (GLuint)var0, (const GLint*)&var1[0] );
  _checkError("glVertexAttribI2iv");
}

FABRIC_EXT_EXPORT void glVertexAttribI2ui_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::Size var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI2ui( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)var0, (unsigned)var1, (unsigned)var2);
#endif
  glVertexAttribI2ui( (GLuint)var0, (GLuint)var1, (GLuint)var2 );
  _checkError("glVertexAttribI2ui");
}

FABRIC_EXT_EXPORT void glVertexAttribI2uiv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Size> & var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI2uiv( 0x%04X, GLuint* );\n", (unsigned)var0);
#endif
  glVertexAttribI2uiv( (GLuint)var0, (const GLuint*)&var1[0] );
  _checkError("glVertexAttribI2uiv");
}

FABRIC_EXT_EXPORT void glVertexAttribI3i_wrapper(
  KL::Size var0,
  KL::Integer var1,
  KL::Integer var2,
  KL::Integer var3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI3i( 0x%04X, %d, %d, %d );\n", (unsigned)var0, (int)var1, (int)var2, (int)var3);
#endif
  glVertexAttribI3i( (GLuint)var0, (GLint)var1, (GLint)var2, (GLint)var3 );
  _checkError("glVertexAttribI3i");
}

FABRIC_EXT_EXPORT void glVertexAttribI3iv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Integer> & var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI3iv( 0x%04X, GLint* );\n", (unsigned)var0);
#endif
  glVertexAttribI3iv( (GLuint)var0, (const GLint*)&var1[0] );
  _checkError("glVertexAttribI3iv");
}

FABRIC_EXT_EXPORT void glVertexAttribI3ui_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::Size var2,
  KL::Size var3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI3ui( 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)var0, (unsigned)var1, (unsigned)var2, (unsigned)var3);
#endif
  glVertexAttribI3ui( (GLuint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3 );
  _checkError("glVertexAttribI3ui");
}

FABRIC_EXT_EXPORT void glVertexAttribI3uiv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Size> & var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI3uiv( 0x%04X, GLuint* );\n", (unsigned)var0);
#endif
  glVertexAttribI3uiv( (GLuint)var0, (const GLuint*)&var1[0] );
  _checkError("glVertexAttribI3uiv");
}

FABRIC_EXT_EXPORT void glVertexAttribI4bv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Byte> & var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4bv( 0x%04X, GLbyte* );\n", (unsigned)var0);
#endif
  glVertexAttribI4bv( (GLuint)var0, (const GLbyte*)&var1[0] );
  _checkError("glVertexAttribI4bv");
}

FABRIC_EXT_EXPORT void glVertexAttribI4i_wrapper(
  KL::Size var0,
  KL::Integer var1,
  KL::Integer var2,
  KL::Integer var3,
  KL::Integer var4
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4i( 0x%04X, %d, %d, %d, %d );\n", (unsigned)var0, (int)var1, (int)var2, (int)var3, (int)var4);
#endif
  glVertexAttribI4i( (GLuint)var0, (GLint)var1, (GLint)var2, (GLint)var3, (GLint)var4 );
  _checkError("glVertexAttribI4i");
}

FABRIC_EXT_EXPORT void glVertexAttribI4iv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Integer> & var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4iv( 0x%04X, GLint* );\n", (unsigned)var0);
#endif
  glVertexAttribI4iv( (GLuint)var0, (const GLint*)&var1[0] );
  _checkError("glVertexAttribI4iv");
}

FABRIC_EXT_EXPORT void glVertexAttribI4sv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Integer> & var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4sv( 0x%04X, GLshort* );\n", (unsigned)var0);
#endif
  glVertexAttribI4sv( (GLuint)var0, (const GLshort*)&var1[0] );
  _checkError("glVertexAttribI4sv");
}

FABRIC_EXT_EXPORT void glVertexAttribI4ubv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Size> & var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4ubv( 0x%04X, GLubyte* );\n", (unsigned)var0);
#endif
  glVertexAttribI4ubv( (GLuint)var0, (const GLubyte*)&var1[0] );
  _checkError("glVertexAttribI4ubv");
}

FABRIC_EXT_EXPORT void glVertexAttribI4ui_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::Size var2,
  KL::Size var3,
  KL::Size var4
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4ui( 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)var0, (unsigned)var1, (unsigned)var2, (unsigned)var3, (unsigned)var4);
#endif
  glVertexAttribI4ui( (GLuint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3, (GLuint)var4 );
  _checkError("glVertexAttribI4ui");
}

FABRIC_EXT_EXPORT void glVertexAttribI4uiv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Size> & var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4uiv( 0x%04X, GLuint* );\n", (unsigned)var0);
#endif
  glVertexAttribI4uiv( (GLuint)var0, (const GLuint*)&var1[0] );
  _checkError("glVertexAttribI4uiv");
}

FABRIC_EXT_EXPORT void glVertexAttribI4usv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Integer> & var1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4usv( 0x%04X, GLushort* );\n", (unsigned)var0);
#endif
  glVertexAttribI4usv( (GLuint)var0, (const GLushort*)&var1[0] );
  _checkError("glVertexAttribI4usv");
}

FABRIC_EXT_EXPORT void glVertexAttribIPointer_wrapper(
  KL::Size var0,
  KL::Integer var1,
  KL::Size var2,
  KL::Size var3,
  KL::Data var4
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribIPointer( 0x%04X, %d, %d, %d, GLvoid* );\n", (unsigned)var0, (int)var1, (int)var2, (int)var3);
#endif
  glVertexAttribIPointer( (GLuint)var0, (GLint)var1, (GLenum)var2, (GLsizei)var3, var4 );
  _checkError("glVertexAttribIPointer");
}

FABRIC_EXT_EXPORT void glDrawArraysInstanced_wrapper(
  KL::Size var0,
  KL::Integer var1,
  KL::Size var2,
  KL::Size var3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawArraysInstanced( %d, %d, %d, %d );\n", (int)var0, (int)var1, (int)var2, (int)var3);
#endif
  glDrawArraysInstanced( (GLenum)var0, (GLint)var1, (GLsizei)var2, (GLsizei)var3 );
  _checkError("glDrawArraysInstanced");
}

FABRIC_EXT_EXPORT void glDrawElementsInstanced_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::Size var2,
  KL::Data var3,
  KL::Size var4
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawElementsInstanced( %d, %d, %d, GLvoid*, %d );\n", (int)var0, (int)var1, (int)var2, (int)var4);
#endif
  glDrawElementsInstanced( (GLenum)var0, (GLsizei)var1, (GLenum)var2, var3, (GLsizei)var4 );
  _checkError("glDrawElementsInstanced");
}

FABRIC_EXT_EXPORT void glPrimitiveRestartIndex_wrapper(
  KL::Size var0
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPrimitiveRestartIndex( 0x%04X );\n", (unsigned)var0);
#endif
  glPrimitiveRestartIndex( (GLuint)var0 );
  _checkError("glPrimitiveRestartIndex");
}

FABRIC_EXT_EXPORT void glTexBuffer_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::Size var2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexBuffer( %d, %d, 0x%04X );\n", (int)var0, (int)var1, (unsigned)var2);
#endif
  glTexBuffer( (GLenum)var0, (GLenum)var1, (GLuint)var2 );
  _checkError("glTexBuffer");
}

FABRIC_EXT_EXPORT void glFramebufferTexture_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::Size var2,
  KL::Integer var3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTexture( %d, %d, 0x%04X, %d );\n", (int)var0, (int)var1, (unsigned)var2, (int)var3);
#endif
  glFramebufferTexture( (GLenum)var0, (GLenum)var1, (GLuint)var2, (GLint)var3 );
  _checkError("glFramebufferTexture");
}

FABRIC_EXT_EXPORT void glTbufferMask3DFX_wrapper(
  KL::Size mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTbufferMask3DFX( 0x%04X );\n", (unsigned)mask);
#endif
  glTbufferMask3DFX( (GLuint)mask );
  _checkError("glTbufferMask3DFX");
}

FABRIC_EXT_EXPORT void glClampColorARB_wrapper(
  KL::Size target,
  KL::Size clamp
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClampColorARB( %d, %d );\n", (int)target, (int)clamp);
#endif
  glClampColorARB( (GLenum)target, (GLenum)clamp );
  _checkError("glClampColorARB");
}

FABRIC_EXT_EXPORT void glCopyBufferSubData_wrapper(
  KL::Size readtarget,
  KL::Size writetarget,
  KL::Integer readoffset,
  KL::Integer writeoffset,
  KL::Size size
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyBufferSubData( %d, %d, %d, %d, %d );\n", (int)readtarget, (int)writetarget, (int)readoffset, (int)writeoffset, (int)size);
#endif
  glCopyBufferSubData( (GLenum)readtarget, (GLenum)writetarget, (GLintptr)readoffset, (GLintptr)writeoffset, (GLsizeiptr)size );
  _checkError("glCopyBufferSubData");
}

FABRIC_EXT_EXPORT void glDrawBuffersARB_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & bufs
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawBuffersARB( %d, GLenum* );\n", (int)n);
#endif
  glDrawBuffersARB( (GLsizei)n, (const GLenum*)&bufs[0] );
  _checkError("glDrawBuffersARB");
}

FABRIC_EXT_EXPORT void glBlendEquationSeparateiARB_wrapper(
  KL::Size buf,
  KL::Size modeRGB,
  KL::Size modeAlpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendEquationSeparateiARB( 0x%04X, %d, %d );\n", (unsigned)buf, (int)modeRGB, (int)modeAlpha);
#endif
  glBlendEquationSeparateiARB( (GLuint)buf, (GLenum)modeRGB, (GLenum)modeAlpha );
  _checkError("glBlendEquationSeparateiARB");
}

FABRIC_EXT_EXPORT void glBlendEquationiARB_wrapper(
  KL::Size buf,
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendEquationiARB( 0x%04X, %d );\n", (unsigned)buf, (int)mode);
#endif
  glBlendEquationiARB( (GLuint)buf, (GLenum)mode );
  _checkError("glBlendEquationiARB");
}

FABRIC_EXT_EXPORT void glBlendFuncSeparateiARB_wrapper(
  KL::Size buf,
  KL::Size srcRGB,
  KL::Size dstRGB,
  KL::Size srcAlpha,
  KL::Size dstAlpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendFuncSeparateiARB( 0x%04X, %d, %d, %d, %d );\n", (unsigned)buf, (int)srcRGB, (int)dstRGB, (int)srcAlpha, (int)dstAlpha);
#endif
  glBlendFuncSeparateiARB( (GLuint)buf, (GLenum)srcRGB, (GLenum)dstRGB, (GLenum)srcAlpha, (GLenum)dstAlpha );
  _checkError("glBlendFuncSeparateiARB");
}

FABRIC_EXT_EXPORT void glBlendFunciARB_wrapper(
  KL::Size buf,
  KL::Size src,
  KL::Size dst
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendFunciARB( 0x%04X, %d, %d );\n", (unsigned)buf, (int)src, (int)dst);
#endif
  glBlendFunciARB( (GLuint)buf, (GLenum)src, (GLenum)dst );
  _checkError("glBlendFunciARB");
}

FABRIC_EXT_EXPORT void glDrawElementsBaseVertex_wrapper(
  KL::Size mode,
  KL::Size count,
  KL::Size type,
  KL::Data indices,
  KL::Integer basevertex
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawElementsBaseVertex( %d, %d, %d, void*, %d );\n", (int)mode, (int)count, (int)type, (int)basevertex);
#endif
  glDrawElementsBaseVertex( (GLenum)mode, (GLsizei)count, (GLenum)type, indices, (GLint)basevertex );
  _checkError("glDrawElementsBaseVertex");
}

FABRIC_EXT_EXPORT void glDrawElementsInstancedBaseVertex_wrapper(
  KL::Size mode,
  KL::Size count,
  KL::Size type,
  KL::Data indices,
  KL::Size primcount,
  KL::Integer basevertex
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawElementsInstancedBaseVertex( %d, %d, %d, void*, %d, %d );\n", (int)mode, (int)count, (int)type, (int)primcount, (int)basevertex);
#endif
  glDrawElementsInstancedBaseVertex( (GLenum)mode, (GLsizei)count, (GLenum)type, indices, (GLsizei)primcount, (GLint)basevertex );
  _checkError("glDrawElementsInstancedBaseVertex");
}

FABRIC_EXT_EXPORT void glDrawRangeElementsBaseVertex_wrapper(
  KL::Size mode,
  KL::Size start,
  KL::Size end,
  KL::Size count,
  KL::Size type,
  KL::Data indices,
  KL::Integer basevertex
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawRangeElementsBaseVertex( %d, 0x%04X, 0x%04X, %d, %d, void*, %d );\n", (int)mode, (unsigned)start, (unsigned)end, (int)count, (int)type, (int)basevertex);
#endif
  glDrawRangeElementsBaseVertex( (GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, indices, (GLint)basevertex );
  _checkError("glDrawRangeElementsBaseVertex");
}

FABRIC_EXT_EXPORT void glDrawArraysInstancedARB_wrapper(
  KL::Size mode,
  KL::Integer first,
  KL::Size count,
  KL::Size primcount
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawArraysInstancedARB( %d, %d, %d, %d );\n", (int)mode, (int)first, (int)count, (int)primcount);
#endif
  glDrawArraysInstancedARB( (GLenum)mode, (GLint)first, (GLsizei)count, (GLsizei)primcount );
  _checkError("glDrawArraysInstancedARB");
}

FABRIC_EXT_EXPORT void glDrawElementsInstancedARB_wrapper(
  KL::Size mode,
  KL::Size count,
  KL::Size type,
  KL::Data indices,
  KL::Size primcount
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawElementsInstancedARB( %d, %d, %d, void*, %d );\n", (int)mode, (int)count, (int)type, (int)primcount);
#endif
  glDrawElementsInstancedARB( (GLenum)mode, (GLsizei)count, (GLenum)type, indices, (GLsizei)primcount );
  _checkError("glDrawElementsInstancedARB");
}

FABRIC_EXT_EXPORT void glBindFramebuffer_wrapper(
  KL::Size target,
  KL::Size framebuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindFramebuffer( %d, 0x%04X );\n", (int)target, (unsigned)framebuffer);
#endif
  glBindFramebuffer( (GLenum)target, (GLuint)framebuffer );
  _checkError("glBindFramebuffer");
}

FABRIC_EXT_EXPORT void glBindRenderbuffer_wrapper(
  KL::Size target,
  KL::Size renderbuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindRenderbuffer( %d, 0x%04X );\n", (int)target, (unsigned)renderbuffer);
#endif
  glBindRenderbuffer( (GLenum)target, (GLuint)renderbuffer );
  _checkError("glBindRenderbuffer");
}

FABRIC_EXT_EXPORT void glBlitFramebuffer_wrapper(
  KL::Integer srcX0,
  KL::Integer srcY0,
  KL::Integer srcX1,
  KL::Integer srcY1,
  KL::Integer dstX0,
  KL::Integer dstY0,
  KL::Integer dstX1,
  KL::Integer dstY1,
  KL::Size mask,
  KL::Size filter
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlitFramebuffer( %d, %d, %d, %d, %d, %d, %d, %d, 0x%04X, %d );\n", (int)srcX0, (int)srcY0, (int)srcX1, (int)srcY1, (int)dstX0, (int)dstY0, (int)dstX1, (int)dstY1, (unsigned)mask, (int)filter);
#endif
  glBlitFramebuffer( (GLint)srcX0, (GLint)srcY0, (GLint)srcX1, (GLint)srcY1, (GLint)dstX0, (GLint)dstY0, (GLint)dstX1, (GLint)dstY1, (GLbitfield)mask, (GLenum)filter );
  _checkError("glBlitFramebuffer");
}

FABRIC_EXT_EXPORT KL::Size glCheckFramebufferStatus_wrapper(
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCheckFramebufferStatus( %d );\n", (int)target);
#endif
  GLenum result = glCheckFramebufferStatus( (GLenum)target );
  _checkError("glCheckFramebufferStatus");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glDeleteFramebuffers_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & framebuffers
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteFramebuffers( %d, GLuint* );\n", (int)n);
#endif
  glDeleteFramebuffers( (GLsizei)n, (const GLuint*)&framebuffers[0] );
  _checkError("glDeleteFramebuffers");
}

FABRIC_EXT_EXPORT void glDeleteRenderbuffers_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & renderbuffers
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteRenderbuffers( %d, GLuint* );\n", (int)n);
#endif
  glDeleteRenderbuffers( (GLsizei)n, (const GLuint*)&renderbuffers[0] );
  _checkError("glDeleteRenderbuffers");
}

FABRIC_EXT_EXPORT void glFramebufferRenderbuffer_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size renderbuffertarget,
  KL::Size renderbuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferRenderbuffer( %d, %d, %d, 0x%04X );\n", (int)target, (int)attachment, (int)renderbuffertarget, (unsigned)renderbuffer);
#endif
  glFramebufferRenderbuffer( (GLenum)target, (GLenum)attachment, (GLenum)renderbuffertarget, (GLuint)renderbuffer );
  _checkError("glFramebufferRenderbuffer");
}

FABRIC_EXT_EXPORT void glFramebufferTexture1D_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size textarget,
  KL::Size texture,
  KL::Integer level
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTexture1D( %d, %d, %d, 0x%04X, %d );\n", (int)target, (int)attachment, (int)textarget, (unsigned)texture, (int)level);
#endif
  glFramebufferTexture1D( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
  _checkError("glFramebufferTexture1D");
}

FABRIC_EXT_EXPORT void glFramebufferTexture2D_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size textarget,
  KL::Size texture,
  KL::Integer level
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTexture2D( %d, %d, %d, 0x%04X, %d );\n", (int)target, (int)attachment, (int)textarget, (unsigned)texture, (int)level);
#endif
  glFramebufferTexture2D( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
  _checkError("glFramebufferTexture2D");
}

FABRIC_EXT_EXPORT void glFramebufferTexture3D_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size textarget,
  KL::Size texture,
  KL::Integer level,
  KL::Integer layer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTexture3D( %d, %d, %d, 0x%04X, %d, %d );\n", (int)target, (int)attachment, (int)textarget, (unsigned)texture, (int)level, (int)layer);
#endif
  glFramebufferTexture3D( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level, (GLint)layer );
  _checkError("glFramebufferTexture3D");
}

FABRIC_EXT_EXPORT void glFramebufferTextureLayer_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size texture,
  KL::Integer level,
  KL::Integer layer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTextureLayer( %d, %d, 0x%04X, %d, %d );\n", (int)target, (int)attachment, (unsigned)texture, (int)level, (int)layer);
#endif
  glFramebufferTextureLayer( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer );
  _checkError("glFramebufferTextureLayer");
}

FABRIC_EXT_EXPORT void glGenFramebuffers_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & framebuffers
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenFramebuffers( %d, GLuint* );\n", (int)n);
#endif
  glGenFramebuffers( (GLsizei)n, (GLuint*)&framebuffers[0] );
  _checkError("glGenFramebuffers");
}

FABRIC_EXT_EXPORT void glGenRenderbuffers_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & renderbuffers
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenRenderbuffers( %d, GLuint* );\n", (int)n);
#endif
  glGenRenderbuffers( (GLsizei)n, (GLuint*)&renderbuffers[0] );
  _checkError("glGenRenderbuffers");
}

FABRIC_EXT_EXPORT void glGenerateMipmap_wrapper(
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenerateMipmap( %d );\n", (int)target);
#endif
  glGenerateMipmap( (GLenum)target );
  _checkError("glGenerateMipmap");
}

FABRIC_EXT_EXPORT void glGetFramebufferAttachmentParameteriv_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetFramebufferAttachmentParameteriv( %d, %d, %d, GLint* );\n", (int)target, (int)attachment, (int)pname);
#endif
  glGetFramebufferAttachmentParameteriv( (GLenum)target, (GLenum)attachment, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetFramebufferAttachmentParameteriv");
}

FABRIC_EXT_EXPORT void glGetRenderbufferParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetRenderbufferParameteriv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetRenderbufferParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetRenderbufferParameteriv");
}

FABRIC_EXT_EXPORT KL::Boolean glIsFramebuffer_wrapper(
  KL::Size framebuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsFramebuffer( 0x%04X );\n", (unsigned)framebuffer);
#endif
  GLboolean result = glIsFramebuffer( (GLuint)framebuffer );
  _checkError("glIsFramebuffer");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glIsRenderbuffer_wrapper(
  KL::Size renderbuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsRenderbuffer( 0x%04X );\n", (unsigned)renderbuffer);
#endif
  GLboolean result = glIsRenderbuffer( (GLuint)renderbuffer );
  _checkError("glIsRenderbuffer");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glRenderbufferStorage_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRenderbufferStorage( %d, %d, %d, %d );\n", (int)target, (int)internalformat, (int)width, (int)height);
#endif
  glRenderbufferStorage( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  _checkError("glRenderbufferStorage");
}

FABRIC_EXT_EXPORT void glRenderbufferStorageMultisample_wrapper(
  KL::Size target,
  KL::Size samples,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRenderbufferStorageMultisample( %d, %d, %d, %d, %d );\n", (int)target, (int)samples, (int)internalformat, (int)width, (int)height);
#endif
  glRenderbufferStorageMultisample( (GLenum)target, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  _checkError("glRenderbufferStorageMultisample");
}

FABRIC_EXT_EXPORT void glFramebufferTextureARB_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size texture,
  KL::Integer level
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTextureARB( %d, %d, 0x%04X, %d );\n", (int)target, (int)attachment, (unsigned)texture, (int)level);
#endif
  glFramebufferTextureARB( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level );
  _checkError("glFramebufferTextureARB");
}

FABRIC_EXT_EXPORT void glFramebufferTextureFaceARB_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size texture,
  KL::Integer level,
  KL::Size face
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTextureFaceARB( %d, %d, 0x%04X, %d, %d );\n", (int)target, (int)attachment, (unsigned)texture, (int)level, (int)face);
#endif
  glFramebufferTextureFaceARB( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLenum)face );
  _checkError("glFramebufferTextureFaceARB");
}

FABRIC_EXT_EXPORT void glFramebufferTextureLayerARB_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size texture,
  KL::Integer level,
  KL::Integer layer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTextureLayerARB( %d, %d, 0x%04X, %d, %d );\n", (int)target, (int)attachment, (unsigned)texture, (int)level, (int)layer);
#endif
  glFramebufferTextureLayerARB( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer );
  _checkError("glFramebufferTextureLayerARB");
}

FABRIC_EXT_EXPORT void glProgramParameteriARB_wrapper(
  KL::Size program,
  KL::Size pname,
  KL::Integer value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramParameteriARB( 0x%04X, %d, %d );\n", (unsigned)program, (int)pname, (int)value);
#endif
  glProgramParameteriARB( (GLuint)program, (GLenum)pname, (GLint)value );
  _checkError("glProgramParameteriARB");
}

FABRIC_EXT_EXPORT void glColorSubTable_wrapper(
  KL::Size target,
  KL::Size start,
  KL::Size count,
  KL::Size format,
  KL::Size type,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColorSubTable( %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)start, (int)count, (int)format, (int)type);
#endif
  glColorSubTable( (GLenum)target, (GLsizei)start, (GLsizei)count, (GLenum)format, (GLenum)type, data );
  _checkError("glColorSubTable");
}

FABRIC_EXT_EXPORT void glColorTable_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Size width,
  KL::Size format,
  KL::Size type,
  KL::Data table
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColorTable( %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)internalformat, (int)width, (int)format, (int)type);
#endif
  glColorTable( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLenum)format, (GLenum)type, table );
  _checkError("glColorTable");
}

FABRIC_EXT_EXPORT void glColorTableParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColorTableParameterfv( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glColorTableParameterfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glColorTableParameterfv");
}

FABRIC_EXT_EXPORT void glColorTableParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColorTableParameteriv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glColorTableParameteriv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glColorTableParameteriv");
}

FABRIC_EXT_EXPORT void glConvolutionFilter1D_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Size width,
  KL::Size format,
  KL::Size type,
  KL::Data image
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glConvolutionFilter1D( %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)internalformat, (int)width, (int)format, (int)type);
#endif
  glConvolutionFilter1D( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLenum)format, (GLenum)type, image );
  _checkError("glConvolutionFilter1D");
}

FABRIC_EXT_EXPORT void glConvolutionFilter2D_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size type,
  KL::Data image
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glConvolutionFilter2D( %d, %d, %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)internalformat, (int)width, (int)height, (int)format, (int)type);
#endif
  glConvolutionFilter2D( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, image );
  _checkError("glConvolutionFilter2D");
}

FABRIC_EXT_EXPORT void glConvolutionParameterf_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Scalar params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glConvolutionParameterf( %d, %d, %f );\n", (int)target, (int)pname, (float)params);
#endif
  glConvolutionParameterf( (GLenum)target, (GLenum)pname, (GLfloat)params );
  _checkError("glConvolutionParameterf");
}

FABRIC_EXT_EXPORT void glConvolutionParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glConvolutionParameterfv( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glConvolutionParameterfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glConvolutionParameterfv");
}

FABRIC_EXT_EXPORT void glConvolutionParameteri_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Integer params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glConvolutionParameteri( %d, %d, %d );\n", (int)target, (int)pname, (int)params);
#endif
  glConvolutionParameteri( (GLenum)target, (GLenum)pname, (GLint)params );
  _checkError("glConvolutionParameteri");
}

FABRIC_EXT_EXPORT void glConvolutionParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glConvolutionParameteriv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glConvolutionParameteriv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glConvolutionParameteriv");
}

FABRIC_EXT_EXPORT void glCopyColorSubTable_wrapper(
  KL::Size target,
  KL::Size start,
  KL::Integer x,
  KL::Integer y,
  KL::Size width
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyColorSubTable( %d, %d, %d, %d, %d );\n", (int)target, (int)start, (int)x, (int)y, (int)width);
#endif
  glCopyColorSubTable( (GLenum)target, (GLsizei)start, (GLint)x, (GLint)y, (GLsizei)width );
  _checkError("glCopyColorSubTable");
}

FABRIC_EXT_EXPORT void glCopyColorTable_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyColorTable( %d, %d, %d, %d, %d );\n", (int)target, (int)internalformat, (int)x, (int)y, (int)width);
#endif
  glCopyColorTable( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width );
  _checkError("glCopyColorTable");
}

FABRIC_EXT_EXPORT void glCopyConvolutionFilter1D_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyConvolutionFilter1D( %d, %d, %d, %d, %d );\n", (int)target, (int)internalformat, (int)x, (int)y, (int)width);
#endif
  glCopyConvolutionFilter1D( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width );
  _checkError("glCopyConvolutionFilter1D");
}

FABRIC_EXT_EXPORT void glCopyConvolutionFilter2D_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyConvolutionFilter2D( %d, %d, %d, %d, %d, %d );\n", (int)target, (int)internalformat, (int)x, (int)y, (int)width, (int)height);
#endif
  glCopyConvolutionFilter2D( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyConvolutionFilter2D");
}

FABRIC_EXT_EXPORT void glGetColorTable_wrapper(
  KL::Size target,
  KL::Size format,
  KL::Size type,
  KL::Data table
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetColorTable( %d, %d, %d, GLvoid* );\n", (int)target, (int)format, (int)type);
#endif
  glGetColorTable( (GLenum)target, (GLenum)format, (GLenum)type, table );
  _checkError("glGetColorTable");
}

FABRIC_EXT_EXPORT void glGetColorTableParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetColorTableParameterfv( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glGetColorTableParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetColorTableParameterfv");
}

FABRIC_EXT_EXPORT void glGetColorTableParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetColorTableParameteriv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetColorTableParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetColorTableParameteriv");
}

FABRIC_EXT_EXPORT void glGetConvolutionFilter_wrapper(
  KL::Size target,
  KL::Size format,
  KL::Size type,
  KL::Data image
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetConvolutionFilter( %d, %d, %d, GLvoid* );\n", (int)target, (int)format, (int)type);
#endif
  glGetConvolutionFilter( (GLenum)target, (GLenum)format, (GLenum)type, image );
  _checkError("glGetConvolutionFilter");
}

FABRIC_EXT_EXPORT void glGetConvolutionParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetConvolutionParameterfv( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glGetConvolutionParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetConvolutionParameterfv");
}

FABRIC_EXT_EXPORT void glGetConvolutionParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetConvolutionParameteriv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetConvolutionParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetConvolutionParameteriv");
}

FABRIC_EXT_EXPORT void glGetHistogram_wrapper(
  KL::Size target,
  KL::Boolean reset,
  KL::Size format,
  KL::Size type,
  KL::Data values
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetHistogram( %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)reset, (int)format, (int)type);
#endif
  glGetHistogram( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, values );
  _checkError("glGetHistogram");
}

FABRIC_EXT_EXPORT void glGetHistogramParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetHistogramParameterfv( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glGetHistogramParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetHistogramParameterfv");
}

FABRIC_EXT_EXPORT void glGetHistogramParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetHistogramParameteriv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetHistogramParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetHistogramParameteriv");
}

FABRIC_EXT_EXPORT void glGetMinmax_wrapper(
  KL::Size target,
  KL::Boolean reset,
  KL::Size format,
  KL::Size types,
  KL::Data values
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMinmax( %d, %d, %d, %d, GLvoid* );\n", (int)target, (int)reset, (int)format, (int)types);
#endif
  glGetMinmax( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)types, values );
  _checkError("glGetMinmax");
}

FABRIC_EXT_EXPORT void glGetMinmaxParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMinmaxParameterfv( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glGetMinmaxParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetMinmaxParameterfv");
}

FABRIC_EXT_EXPORT void glGetMinmaxParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMinmaxParameteriv( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetMinmaxParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetMinmaxParameteriv");
}

FABRIC_EXT_EXPORT void glGetSeparableFilter_wrapper(
  KL::Size target,
  KL::Size format,
  KL::Size type,
  KL::Data row,
  KL::Data column,
  KL::Data span
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetSeparableFilter( %d, %d, %d, GLvoid*, GLvoid*, GLvoid* );\n", (int)target, (int)format, (int)type);
#endif
  glGetSeparableFilter( (GLenum)target, (GLenum)format, (GLenum)type, row, column, span );
  _checkError("glGetSeparableFilter");
}

FABRIC_EXT_EXPORT void glHistogram_wrapper(
  KL::Size target,
  KL::Size width,
  KL::Size internalformat,
  KL::Boolean sink
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glHistogram( %d, %d, %d, %d );\n", (int)target, (int)width, (int)internalformat, (int)sink);
#endif
  glHistogram( (GLenum)target, (GLsizei)width, (GLenum)internalformat, (GLboolean)sink );
  _checkError("glHistogram");
}

FABRIC_EXT_EXPORT void glMinmax_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Boolean sink
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMinmax( %d, %d, %d );\n", (int)target, (int)internalformat, (int)sink);
#endif
  glMinmax( (GLenum)target, (GLenum)internalformat, (GLboolean)sink );
  _checkError("glMinmax");
}

FABRIC_EXT_EXPORT void glResetHistogram_wrapper(
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glResetHistogram( %d );\n", (int)target);
#endif
  glResetHistogram( (GLenum)target );
  _checkError("glResetHistogram");
}

FABRIC_EXT_EXPORT void glResetMinmax_wrapper(
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glResetMinmax( %d );\n", (int)target);
#endif
  glResetMinmax( (GLenum)target );
  _checkError("glResetMinmax");
}

FABRIC_EXT_EXPORT void glSeparableFilter2D_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size type,
  KL::Data row,
  KL::Data column
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSeparableFilter2D( %d, %d, %d, %d, %d, %d, GLvoid*, GLvoid* );\n", (int)target, (int)internalformat, (int)width, (int)height, (int)format, (int)type);
#endif
  glSeparableFilter2D( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, row, column );
  _checkError("glSeparableFilter2D");
}

FABRIC_EXT_EXPORT void glVertexAttribDivisorARB_wrapper(
  KL::Size index,
  KL::Size divisor
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribDivisorARB( 0x%04X, 0x%04X );\n", (unsigned)index, (unsigned)divisor);
#endif
  glVertexAttribDivisorARB( (GLuint)index, (GLuint)divisor );
  _checkError("glVertexAttribDivisorARB");
}

FABRIC_EXT_EXPORT void glFlushMappedBufferRange_wrapper(
  KL::Size target,
  KL::Integer offset,
  KL::Size length
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFlushMappedBufferRange( %d, %d, %d );\n", (int)target, (int)offset, (int)length);
#endif
  glFlushMappedBufferRange( (GLenum)target, (GLintptr)offset, (GLsizeiptr)length );
  _checkError("glFlushMappedBufferRange");
}

FABRIC_EXT_EXPORT GLvoid glMapBufferRange_wrapper(
  KL::Size target,
  KL::Integer offset,
  KL::Size length,
  KL::Size access
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMapBufferRange( %d, %d, %d, 0x%04X );\n", (int)target, (int)offset, (int)length, (unsigned)access);
#endif
  glMapBufferRange( (GLenum)target, (GLintptr)offset, (GLsizeiptr)length, (GLbitfield)access );
  _checkError("glMapBufferRange");
}

FABRIC_EXT_EXPORT void glCurrentPaletteMatrixARB_wrapper(
  KL::Integer index
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCurrentPaletteMatrixARB( %d );\n", (int)index);
#endif
  glCurrentPaletteMatrixARB( (GLint)index );
  _checkError("glCurrentPaletteMatrixARB");
}

FABRIC_EXT_EXPORT void glMatrixIndexPointerARB_wrapper(
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixIndexPointerARB( %d, %d, %d, GLvoid* );\n", (int)size, (int)type, (int)stride);
#endif
  glMatrixIndexPointerARB( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glMatrixIndexPointerARB");
}

FABRIC_EXT_EXPORT void glMatrixIndexubvARB_wrapper(
  KL::Integer size,
  KL::VariableArray<KL::Size> & indices
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixIndexubvARB( %d, GLubyte* );\n", (int)size);
#endif
  glMatrixIndexubvARB( (GLint)size, (GLubyte*)&indices[0] );
  _checkError("glMatrixIndexubvARB");
}

FABRIC_EXT_EXPORT void glMatrixIndexuivARB_wrapper(
  KL::Integer size,
  KL::VariableArray<KL::Size> & indices
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixIndexuivARB( %d, GLuint* );\n", (int)size);
#endif
  glMatrixIndexuivARB( (GLint)size, (GLuint*)&indices[0] );
  _checkError("glMatrixIndexuivARB");
}

FABRIC_EXT_EXPORT void glMatrixIndexusvARB_wrapper(
  KL::Integer size,
  KL::VariableArray<KL::Integer> & indices
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixIndexusvARB( %d, GLushort* );\n", (int)size);
#endif
  glMatrixIndexusvARB( (GLint)size, (GLushort*)&indices[0] );
  _checkError("glMatrixIndexusvARB");
}

FABRIC_EXT_EXPORT void glSampleCoverageARB_wrapper(
  KL::Scalar value,
  KL::Boolean invert
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSampleCoverageARB( %f, %d );\n", (float)value, (int)invert);
#endif
  glSampleCoverageARB( (GLclampf)value, (GLboolean)invert );
  _checkError("glSampleCoverageARB");
}

FABRIC_EXT_EXPORT void glActiveTextureARB_wrapper(
  KL::Size texture
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glActiveTextureARB( %d );\n", (int)texture);
#endif
  glActiveTextureARB( (GLenum)texture );
  _checkError("glActiveTextureARB");
}

FABRIC_EXT_EXPORT void glClientActiveTextureARB_wrapper(
  KL::Size texture
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClientActiveTextureARB( %d );\n", (int)texture);
#endif
  glClientActiveTextureARB( (GLenum)texture );
  _checkError("glClientActiveTextureARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1dARB_wrapper(
  KL::Size target,
  KL::Scalar s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1dARB( %d, %f );\n", (int)target, (float)s);
#endif
  glMultiTexCoord1dARB( (GLenum)target, (GLdouble)s );
  _checkError("glMultiTexCoord1dARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1dvARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1dvARB( %d, GLdouble* );\n", (int)target);
#endif
  glMultiTexCoord1dvARB( (GLenum)target, (const GLdouble*)&v[0] );
  _checkError("glMultiTexCoord1dvARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1fARB_wrapper(
  KL::Size target,
  KL::Scalar s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1fARB( %d, %f );\n", (int)target, (float)s);
#endif
  glMultiTexCoord1fARB( (GLenum)target, (GLfloat)s );
  _checkError("glMultiTexCoord1fARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1fvARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1fvARB( %d, GLfloat* );\n", (int)target);
#endif
  glMultiTexCoord1fvARB( (GLenum)target, (const GLfloat*)&v[0] );
  _checkError("glMultiTexCoord1fvARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1iARB_wrapper(
  KL::Size target,
  KL::Integer s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1iARB( %d, %d );\n", (int)target, (int)s);
#endif
  glMultiTexCoord1iARB( (GLenum)target, (GLint)s );
  _checkError("glMultiTexCoord1iARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1ivARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1ivARB( %d, GLint* );\n", (int)target);
#endif
  glMultiTexCoord1ivARB( (GLenum)target, (const GLint*)&v[0] );
  _checkError("glMultiTexCoord1ivARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1sARB_wrapper(
  KL::Size target,
  KL::Integer s
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1sARB( %d, %d );\n", (int)target, (int)s);
#endif
  glMultiTexCoord1sARB( (GLenum)target, (GLshort)s );
  _checkError("glMultiTexCoord1sARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1svARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord1svARB( %d, GLshort* );\n", (int)target);
#endif
  glMultiTexCoord1svARB( (GLenum)target, (const GLshort*)&v[0] );
  _checkError("glMultiTexCoord1svARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2dARB_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2dARB( %d, %f, %f );\n", (int)target, (float)s, (float)t);
#endif
  glMultiTexCoord2dARB( (GLenum)target, (GLdouble)s, (GLdouble)t );
  _checkError("glMultiTexCoord2dARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2dvARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2dvARB( %d, GLdouble* );\n", (int)target);
#endif
  glMultiTexCoord2dvARB( (GLenum)target, (const GLdouble*)&v[0] );
  _checkError("glMultiTexCoord2dvARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2fARB_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2fARB( %d, %f, %f );\n", (int)target, (float)s, (float)t);
#endif
  glMultiTexCoord2fARB( (GLenum)target, (GLfloat)s, (GLfloat)t );
  _checkError("glMultiTexCoord2fARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2fvARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2fvARB( %d, GLfloat* );\n", (int)target);
#endif
  glMultiTexCoord2fvARB( (GLenum)target, (const GLfloat*)&v[0] );
  _checkError("glMultiTexCoord2fvARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2iARB_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2iARB( %d, %d, %d );\n", (int)target, (int)s, (int)t);
#endif
  glMultiTexCoord2iARB( (GLenum)target, (GLint)s, (GLint)t );
  _checkError("glMultiTexCoord2iARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2ivARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2ivARB( %d, GLint* );\n", (int)target);
#endif
  glMultiTexCoord2ivARB( (GLenum)target, (const GLint*)&v[0] );
  _checkError("glMultiTexCoord2ivARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2sARB_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2sARB( %d, %d, %d );\n", (int)target, (int)s, (int)t);
#endif
  glMultiTexCoord2sARB( (GLenum)target, (GLshort)s, (GLshort)t );
  _checkError("glMultiTexCoord2sARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2svARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord2svARB( %d, GLshort* );\n", (int)target);
#endif
  glMultiTexCoord2svARB( (GLenum)target, (const GLshort*)&v[0] );
  _checkError("glMultiTexCoord2svARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3dARB_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3dARB( %d, %f, %f, %f );\n", (int)target, (float)s, (float)t, (float)r);
#endif
  glMultiTexCoord3dARB( (GLenum)target, (GLdouble)s, (GLdouble)t, (GLdouble)r );
  _checkError("glMultiTexCoord3dARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3dvARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3dvARB( %d, GLdouble* );\n", (int)target);
#endif
  glMultiTexCoord3dvARB( (GLenum)target, (const GLdouble*)&v[0] );
  _checkError("glMultiTexCoord3dvARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3fARB_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3fARB( %d, %f, %f, %f );\n", (int)target, (float)s, (float)t, (float)r);
#endif
  glMultiTexCoord3fARB( (GLenum)target, (GLfloat)s, (GLfloat)t, (GLfloat)r );
  _checkError("glMultiTexCoord3fARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3fvARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3fvARB( %d, GLfloat* );\n", (int)target);
#endif
  glMultiTexCoord3fvARB( (GLenum)target, (const GLfloat*)&v[0] );
  _checkError("glMultiTexCoord3fvARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3iARB_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t,
  KL::Integer r
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3iARB( %d, %d, %d, %d );\n", (int)target, (int)s, (int)t, (int)r);
#endif
  glMultiTexCoord3iARB( (GLenum)target, (GLint)s, (GLint)t, (GLint)r );
  _checkError("glMultiTexCoord3iARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3ivARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3ivARB( %d, GLint* );\n", (int)target);
#endif
  glMultiTexCoord3ivARB( (GLenum)target, (const GLint*)&v[0] );
  _checkError("glMultiTexCoord3ivARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3sARB_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t,
  KL::Integer r
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3sARB( %d, %d, %d, %d );\n", (int)target, (int)s, (int)t, (int)r);
#endif
  glMultiTexCoord3sARB( (GLenum)target, (GLshort)s, (GLshort)t, (GLshort)r );
  _checkError("glMultiTexCoord3sARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3svARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord3svARB( %d, GLshort* );\n", (int)target);
#endif
  glMultiTexCoord3svARB( (GLenum)target, (const GLshort*)&v[0] );
  _checkError("glMultiTexCoord3svARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4dARB_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r,
  KL::Scalar q
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4dARB( %d, %f, %f, %f, %f );\n", (int)target, (float)s, (float)t, (float)r, (float)q);
#endif
  glMultiTexCoord4dARB( (GLenum)target, (GLdouble)s, (GLdouble)t, (GLdouble)r, (GLdouble)q );
  _checkError("glMultiTexCoord4dARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4dvARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4dvARB( %d, GLdouble* );\n", (int)target);
#endif
  glMultiTexCoord4dvARB( (GLenum)target, (const GLdouble*)&v[0] );
  _checkError("glMultiTexCoord4dvARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4fARB_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r,
  KL::Scalar q
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4fARB( %d, %f, %f, %f, %f );\n", (int)target, (float)s, (float)t, (float)r, (float)q);
#endif
  glMultiTexCoord4fARB( (GLenum)target, (GLfloat)s, (GLfloat)t, (GLfloat)r, (GLfloat)q );
  _checkError("glMultiTexCoord4fARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4fvARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4fvARB( %d, GLfloat* );\n", (int)target);
#endif
  glMultiTexCoord4fvARB( (GLenum)target, (const GLfloat*)&v[0] );
  _checkError("glMultiTexCoord4fvARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4iARB_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t,
  KL::Integer r,
  KL::Integer q
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4iARB( %d, %d, %d, %d, %d );\n", (int)target, (int)s, (int)t, (int)r, (int)q);
#endif
  glMultiTexCoord4iARB( (GLenum)target, (GLint)s, (GLint)t, (GLint)r, (GLint)q );
  _checkError("glMultiTexCoord4iARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4ivARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4ivARB( %d, GLint* );\n", (int)target);
#endif
  glMultiTexCoord4ivARB( (GLenum)target, (const GLint*)&v[0] );
  _checkError("glMultiTexCoord4ivARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4sARB_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t,
  KL::Integer r,
  KL::Integer q
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4sARB( %d, %d, %d, %d, %d );\n", (int)target, (int)s, (int)t, (int)r, (int)q);
#endif
  glMultiTexCoord4sARB( (GLenum)target, (GLshort)s, (GLshort)t, (GLshort)r, (GLshort)q );
  _checkError("glMultiTexCoord4sARB");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4svARB_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoord4svARB( %d, GLshort* );\n", (int)target);
#endif
  glMultiTexCoord4svARB( (GLenum)target, (const GLshort*)&v[0] );
  _checkError("glMultiTexCoord4svARB");
}

FABRIC_EXT_EXPORT void glBeginQueryARB_wrapper(
  KL::Size target,
  KL::Size id
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBeginQueryARB( %d, 0x%04X );\n", (int)target, (unsigned)id);
#endif
  glBeginQueryARB( (GLenum)target, (GLuint)id );
  _checkError("glBeginQueryARB");
}

FABRIC_EXT_EXPORT void glDeleteQueriesARB_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & ids
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteQueriesARB( %d, GLuint* );\n", (int)n);
#endif
  glDeleteQueriesARB( (GLsizei)n, (const GLuint*)&ids[0] );
  _checkError("glDeleteQueriesARB");
}

FABRIC_EXT_EXPORT void glEndQueryARB_wrapper(
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEndQueryARB( %d );\n", (int)target);
#endif
  glEndQueryARB( (GLenum)target );
  _checkError("glEndQueryARB");
}

FABRIC_EXT_EXPORT void glGenQueriesARB_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & ids
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenQueriesARB( %d, GLuint* );\n", (int)n);
#endif
  glGenQueriesARB( (GLsizei)n, (GLuint*)&ids[0] );
  _checkError("glGenQueriesARB");
}

FABRIC_EXT_EXPORT void glGetQueryObjectivARB_wrapper(
  KL::Size id,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetQueryObjectivARB( 0x%04X, %d, GLint* );\n", (unsigned)id, (int)pname);
#endif
  glGetQueryObjectivARB( (GLuint)id, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetQueryObjectivARB");
}

FABRIC_EXT_EXPORT void glGetQueryObjectuivARB_wrapper(
  KL::Size id,
  KL::Size pname,
  KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetQueryObjectuivARB( 0x%04X, %d, GLuint* );\n", (unsigned)id, (int)pname);
#endif
  glGetQueryObjectuivARB( (GLuint)id, (GLenum)pname, (GLuint*)&params[0] );
  _checkError("glGetQueryObjectuivARB");
}

FABRIC_EXT_EXPORT void glGetQueryivARB_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetQueryivARB( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetQueryivARB( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetQueryivARB");
}

FABRIC_EXT_EXPORT KL::Boolean glIsQueryARB_wrapper(
  KL::Size id
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsQueryARB( 0x%04X );\n", (unsigned)id);
#endif
  GLboolean result = glIsQueryARB( (GLuint)id );
  _checkError("glIsQueryARB");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glPointParameterfARB_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPointParameterfARB( %d, %f );\n", (int)pname, (float)param);
#endif
  glPointParameterfARB( (GLenum)pname, (GLfloat)param );
  _checkError("glPointParameterfARB");
}

FABRIC_EXT_EXPORT void glPointParameterfvARB_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPointParameterfvARB( %d, GLfloat* );\n", (int)pname);
#endif
  glPointParameterfvARB( (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glPointParameterfvARB");
}

FABRIC_EXT_EXPORT void glProvokingVertex_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProvokingVertex( %d );\n", (int)mode);
#endif
  glProvokingVertex( (GLenum)mode );
  _checkError("glProvokingVertex");
}

FABRIC_EXT_EXPORT void glMinSampleShadingARB_wrapper(
  KL::Scalar value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMinSampleShadingARB( %f );\n", (float)value);
#endif
  glMinSampleShadingARB( (GLclampf)value );
  _checkError("glMinSampleShadingARB");
}

FABRIC_EXT_EXPORT void glUniform1fARB_wrapper(
  KL::Integer location,
  KL::Scalar v0
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform1fARB( %d, %f );\n", (int)location, (float)v0);
#endif
  glUniform1fARB( (GLint)location, (GLfloat)v0 );
  _checkError("glUniform1fARB");
}

FABRIC_EXT_EXPORT void glUniform1fvARB_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform1fvARB( %d, %d, GLfloat* );\n", (int)location, (int)count);
#endif
  glUniform1fvARB( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glUniform1fvARB");
}

FABRIC_EXT_EXPORT void glUniform1iARB_wrapper(
  KL::Integer location,
  KL::Integer v0
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform1iARB( %d, %d );\n", (int)location, (int)v0);
#endif
  glUniform1iARB( (GLint)location, (GLint)v0 );
  _checkError("glUniform1iARB");
}

FABRIC_EXT_EXPORT void glUniform1ivARB_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform1ivARB( %d, %d, GLint* );\n", (int)location, (int)count);
#endif
  glUniform1ivARB( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glUniform1ivARB");
}

FABRIC_EXT_EXPORT void glUniform2fARB_wrapper(
  KL::Integer location,
  KL::Scalar v0,
  KL::Scalar v1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform2fARB( %d, %f, %f );\n", (int)location, (float)v0, (float)v1);
#endif
  glUniform2fARB( (GLint)location, (GLfloat)v0, (GLfloat)v1 );
  _checkError("glUniform2fARB");
}

FABRIC_EXT_EXPORT void glUniform2fvARB_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform2fvARB( %d, %d, GLfloat* );\n", (int)location, (int)count);
#endif
  glUniform2fvARB( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glUniform2fvARB");
}

FABRIC_EXT_EXPORT void glUniform2iARB_wrapper(
  KL::Integer location,
  KL::Integer v0,
  KL::Integer v1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform2iARB( %d, %d, %d );\n", (int)location, (int)v0, (int)v1);
#endif
  glUniform2iARB( (GLint)location, (GLint)v0, (GLint)v1 );
  _checkError("glUniform2iARB");
}

FABRIC_EXT_EXPORT void glUniform2ivARB_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform2ivARB( %d, %d, GLint* );\n", (int)location, (int)count);
#endif
  glUniform2ivARB( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glUniform2ivARB");
}

FABRIC_EXT_EXPORT void glUniform3fARB_wrapper(
  KL::Integer location,
  KL::Scalar v0,
  KL::Scalar v1,
  KL::Scalar v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform3fARB( %d, %f, %f, %f );\n", (int)location, (float)v0, (float)v1, (float)v2);
#endif
  glUniform3fARB( (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2 );
  _checkError("glUniform3fARB");
}

FABRIC_EXT_EXPORT void glUniform3fvARB_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform3fvARB( %d, %d, GLfloat* );\n", (int)location, (int)count);
#endif
  glUniform3fvARB( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glUniform3fvARB");
}

FABRIC_EXT_EXPORT void glUniform3iARB_wrapper(
  KL::Integer location,
  KL::Integer v0,
  KL::Integer v1,
  KL::Integer v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform3iARB( %d, %d, %d, %d );\n", (int)location, (int)v0, (int)v1, (int)v2);
#endif
  glUniform3iARB( (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2 );
  _checkError("glUniform3iARB");
}

FABRIC_EXT_EXPORT void glUniform3ivARB_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform3ivARB( %d, %d, GLint* );\n", (int)location, (int)count);
#endif
  glUniform3ivARB( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glUniform3ivARB");
}

FABRIC_EXT_EXPORT void glUniform4fARB_wrapper(
  KL::Integer location,
  KL::Scalar v0,
  KL::Scalar v1,
  KL::Scalar v2,
  KL::Scalar v3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform4fARB( %d, %f, %f, %f, %f );\n", (int)location, (float)v0, (float)v1, (float)v2, (float)v3);
#endif
  glUniform4fARB( (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2, (GLfloat)v3 );
  _checkError("glUniform4fARB");
}

FABRIC_EXT_EXPORT void glUniform4fvARB_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform4fvARB( %d, %d, GLfloat* );\n", (int)location, (int)count);
#endif
  glUniform4fvARB( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glUniform4fvARB");
}

FABRIC_EXT_EXPORT void glUniform4iARB_wrapper(
  KL::Integer location,
  KL::Integer v0,
  KL::Integer v1,
  KL::Integer v2,
  KL::Integer v3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform4iARB( %d, %d, %d, %d, %d );\n", (int)location, (int)v0, (int)v1, (int)v2, (int)v3);
#endif
  glUniform4iARB( (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2, (GLint)v3 );
  _checkError("glUniform4iARB");
}

FABRIC_EXT_EXPORT void glUniform4ivARB_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform4ivARB( %d, %d, GLint* );\n", (int)location, (int)count);
#endif
  glUniform4ivARB( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glUniform4ivARB");
}

FABRIC_EXT_EXPORT void glUniformMatrix2fvARB_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat22> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformMatrix2fvARB( %d, %d, %d, GLfloat* );\n", (int)location, (int)count, (int)transpose);
#endif
  glUniformMatrix2fvARB( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix2fvARB");
}

FABRIC_EXT_EXPORT void glUniformMatrix3fvARB_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat33> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformMatrix3fvARB( %d, %d, %d, GLfloat* );\n", (int)location, (int)count, (int)transpose);
#endif
  glUniformMatrix3fvARB( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix3fvARB");
}

FABRIC_EXT_EXPORT void glUniformMatrix4fvARB_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat44> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformMatrix4fvARB( %d, %d, %d, GLfloat* );\n", (int)location, (int)count, (int)transpose);
#endif
  glUniformMatrix4fvARB( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix4fvARB");
}

FABRIC_EXT_EXPORT void glTexBufferARB_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Size buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexBufferARB( %d, %d, 0x%04X );\n", (int)target, (int)internalformat, (unsigned)buffer);
#endif
  glTexBufferARB( (GLenum)target, (GLenum)internalformat, (GLuint)buffer );
  _checkError("glTexBufferARB");
}

FABRIC_EXT_EXPORT void glCompressedTexImage1DARB_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Integer border,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTexImage1DARB( %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)imageSize);
#endif
  glCompressedTexImage1DARB( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLint)border, (GLsizei)imageSize, data );
  _checkError("glCompressedTexImage1DARB");
}

FABRIC_EXT_EXPORT void glCompressedTexImage2DARB_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Integer border,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTexImage2DARB( %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)imageSize);
#endif
  glCompressedTexImage2DARB( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLsizei)imageSize, data );
  _checkError("glCompressedTexImage2DARB");
}

FABRIC_EXT_EXPORT void glCompressedTexImage3DARB_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Integer border,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTexImage3DARB( %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)imageSize);
#endif
  glCompressedTexImage3DARB( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLsizei)imageSize, data );
  _checkError("glCompressedTexImage3DARB");
}

FABRIC_EXT_EXPORT void glCompressedTexSubImage1DARB_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Size width,
  KL::Size format,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTexSubImage1DARB( %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)imageSize);
#endif
  glCompressedTexSubImage1DARB( (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedTexSubImage1DARB");
}

FABRIC_EXT_EXPORT void glCompressedTexSubImage2DARB_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTexSubImage2DARB( %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)imageSize);
#endif
  glCompressedTexSubImage2DARB( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedTexSubImage2DARB");
}

FABRIC_EXT_EXPORT void glCompressedTexSubImage3DARB_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer zoffset,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Size format,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTexSubImage3DARB( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)imageSize);
#endif
  glCompressedTexSubImage3DARB( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedTexSubImage3DARB");
}

FABRIC_EXT_EXPORT void glGetCompressedTexImageARB_wrapper(
  KL::Size target,
  KL::Integer lod,
  KL::Data img
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetCompressedTexImageARB( %d, %d, void* );\n", (int)target, (int)lod);
#endif
  glGetCompressedTexImageARB( (GLenum)target, (GLint)lod, img );
  _checkError("glGetCompressedTexImageARB");
}

FABRIC_EXT_EXPORT void glGetMultisamplefv_wrapper(
  KL::Size pname,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & val
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultisamplefv( %d, 0x%04X, GLfloat* );\n", (int)pname, (unsigned)index);
#endif
  glGetMultisamplefv( (GLenum)pname, (GLuint)index, (GLfloat*)&val[0] );
  _checkError("glGetMultisamplefv");
}

FABRIC_EXT_EXPORT void glSampleMaski_wrapper(
  KL::Size index,
  KL::Size mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSampleMaski( 0x%04X, 0x%04X );\n", (unsigned)index, (unsigned)mask);
#endif
  glSampleMaski( (GLuint)index, (GLbitfield)mask );
  _checkError("glSampleMaski");
}

FABRIC_EXT_EXPORT void glTexImage2DMultisample_wrapper(
  KL::Size target,
  KL::Size samples,
  KL::Integer internalformat,
  KL::Size width,
  KL::Size height,
  KL::Boolean fixedsamplelocations
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexImage2DMultisample( %d, %d, %d, %d, %d, %d );\n", (int)target, (int)samples, (int)internalformat, (int)width, (int)height, (int)fixedsamplelocations);
#endif
  glTexImage2DMultisample( (GLenum)target, (GLsizei)samples, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLboolean)fixedsamplelocations );
  _checkError("glTexImage2DMultisample");
}

FABRIC_EXT_EXPORT void glTexImage3DMultisample_wrapper(
  KL::Size target,
  KL::Size samples,
  KL::Integer internalformat,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Boolean fixedsamplelocations
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexImage3DMultisample( %d, %d, %d, %d, %d, %d, %d );\n", (int)target, (int)samples, (int)internalformat, (int)width, (int)height, (int)depth, (int)fixedsamplelocations);
#endif
  glTexImage3DMultisample( (GLenum)target, (GLsizei)samples, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLboolean)fixedsamplelocations );
  _checkError("glTexImage3DMultisample");
}

FABRIC_EXT_EXPORT void glBindBufferBase_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Size buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindBufferBase( %d, 0x%04X, 0x%04X );\n", (int)target, (unsigned)index, (unsigned)buffer);
#endif
  glBindBufferBase( (GLenum)target, (GLuint)index, (GLuint)buffer );
  _checkError("glBindBufferBase");
}

FABRIC_EXT_EXPORT void glBindBufferRange_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Size buffer,
  KL::Integer offset,
  KL::Size size
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindBufferRange( %d, 0x%04X, 0x%04X, %d, %d );\n", (int)target, (unsigned)index, (unsigned)buffer, (int)offset, (int)size);
#endif
  glBindBufferRange( (GLenum)target, (GLuint)index, (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size );
  _checkError("glBindBufferRange");
}

FABRIC_EXT_EXPORT void glGetActiveUniformBlockName_wrapper(
  KL::Size program,
  KL::Size uniformBlockIndex,
  KL::Size bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::String & uniformBlockName
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetActiveUniformBlockName( 0x%04X, 0x%04X, %d, GLsizei*, char* );\n", (unsigned)program, (unsigned)uniformBlockIndex, (int)bufSize);
#endif
  char * uniformBlockNameStr = new char[1024];
  glGetActiveUniformBlockName( (GLuint)program, (GLuint)uniformBlockIndex, (GLsizei)bufSize, (GLsizei*)&length[0], (char*)uniformBlockNameStr );
  _checkError("glGetActiveUniformBlockName");
  uniformBlockName = KL::String((const char*)uniformBlockNameStr);
  delete( uniformBlockNameStr );
}

FABRIC_EXT_EXPORT void glGetActiveUniformBlockiv_wrapper(
  KL::Size program,
  KL::Size uniformBlockIndex,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetActiveUniformBlockiv( 0x%04X, 0x%04X, %d, GLint* );\n", (unsigned)program, (unsigned)uniformBlockIndex, (int)pname);
#endif
  glGetActiveUniformBlockiv( (GLuint)program, (GLuint)uniformBlockIndex, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetActiveUniformBlockiv");
}

FABRIC_EXT_EXPORT void glGetActiveUniformName_wrapper(
  KL::Size program,
  KL::Size uniformIndex,
  KL::Size bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::String & uniformName
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetActiveUniformName( 0x%04X, 0x%04X, %d, GLsizei*, char* );\n", (unsigned)program, (unsigned)uniformIndex, (int)bufSize);
#endif
  char * uniformNameStr = new char[1024];
  glGetActiveUniformName( (GLuint)program, (GLuint)uniformIndex, (GLsizei)bufSize, (GLsizei*)&length[0], (char*)uniformNameStr );
  _checkError("glGetActiveUniformName");
  uniformName = KL::String((const char*)uniformNameStr);
  delete( uniformNameStr );
}

FABRIC_EXT_EXPORT void glGetActiveUniformsiv_wrapper(
  KL::Size program,
  KL::Size uniformCount,
  const KL::VariableArray<KL::Size> & uniformIndices,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetActiveUniformsiv( 0x%04X, %d, GLuint*, %d, GLint* );\n", (unsigned)program, (int)uniformCount, (int)pname);
#endif
  glGetActiveUniformsiv( (GLuint)program, (GLsizei)uniformCount, (const GLuint*)&uniformIndices[0], (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetActiveUniformsiv");
}

FABRIC_EXT_EXPORT void glGetIntegeri_v_wrapper(
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Integer> & data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetIntegeri_v( %d, 0x%04X, GLint* );\n", (int)target, (unsigned)index);
#endif
  glGetIntegeri_v( (GLenum)target, (GLuint)index, (GLint*)&data[0] );
  _checkError("glGetIntegeri_v");
}

FABRIC_EXT_EXPORT KL::Size glGetUniformBlockIndex_wrapper(
  KL::Size program,
  const KL::String &uniformBlockName
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetUniformBlockIndex( 0x%04X, char* );\n", (unsigned)program);
#endif
  GLuint result = glGetUniformBlockIndex( (GLuint)program, (const char*)uniformBlockName.data() );
  _checkError("glGetUniformBlockIndex");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glGetUniformIndices_wrapper(
  KL::Size program,
  KL::Size uniformCount,
  const KL::VariableArray<KL::String> &uniformNames,
  KL::VariableArray<KL::Size> & uniformIndices
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetUniformIndices( 0x%04X, %d, char*, GLuint* );\n", (unsigned)program, (int)uniformCount);
#endif
  KL::VariableArray<KL::Data> uniformNamesData;
  uniformNamesData.resize(uniformNames.size());
  for(KL::Size uniformNamesCounter = 0;uniformNamesCounter<uniformNames.size();uniformNamesCounter++)
    uniformNamesData[uniformNamesCounter] = (KL::Data)uniformNames[uniformNamesCounter].data();
  glGetUniformIndices( (GLuint)program, (GLsizei)uniformCount, (const char**)&uniformNamesData[0], (GLuint*)&uniformIndices[0] );
  _checkError("glGetUniformIndices");
}

FABRIC_EXT_EXPORT void glUniformBlockBinding_wrapper(
  KL::Size program,
  KL::Size uniformBlockIndex,
  KL::Size uniformBlockBinding
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformBlockBinding( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)program, (unsigned)uniformBlockIndex, (unsigned)uniformBlockBinding);
#endif
  glUniformBlockBinding( (GLuint)program, (GLuint)uniformBlockIndex, (GLuint)uniformBlockBinding );
  _checkError("glUniformBlockBinding");
}

FABRIC_EXT_EXPORT void glBindVertexArray_wrapper(
  KL::Size array
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindVertexArray( 0x%04X );\n", (unsigned)array);
#endif
  glBindVertexArray( (GLuint)array );
  _checkError("glBindVertexArray");
}

FABRIC_EXT_EXPORT void glDeleteVertexArrays_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & arrays
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteVertexArrays( %d, GLuint* );\n", (int)n);
#endif
  glDeleteVertexArrays( (GLsizei)n, (const GLuint*)&arrays[0] );
  _checkError("glDeleteVertexArrays");
}

FABRIC_EXT_EXPORT void glGenVertexArrays_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & arrays
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenVertexArrays( %d, GLuint* );\n", (int)n);
#endif
  glGenVertexArrays( (GLsizei)n, (GLuint*)&arrays[0] );
  _checkError("glGenVertexArrays");
}

FABRIC_EXT_EXPORT KL::Boolean glIsVertexArray_wrapper(
  KL::Size array
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsVertexArray( 0x%04X );\n", (unsigned)array);
#endif
  GLboolean result = glIsVertexArray( (GLuint)array );
  _checkError("glIsVertexArray");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glVertexBlendARB_wrapper(
  KL::Integer count
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexBlendARB( %d );\n", (int)count);
#endif
  glVertexBlendARB( (GLint)count );
  _checkError("glVertexBlendARB");
}

FABRIC_EXT_EXPORT void glWeightPointerARB_wrapper(
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWeightPointerARB( %d, %d, %d, GLvoid* );\n", (int)size, (int)type, (int)stride);
#endif
  glWeightPointerARB( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glWeightPointerARB");
}

FABRIC_EXT_EXPORT void glWeightbvARB_wrapper(
  KL::Integer size,
  KL::VariableArray<KL::Byte> & weights
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWeightbvARB( %d, GLbyte* );\n", (int)size);
#endif
  glWeightbvARB( (GLint)size, (GLbyte*)&weights[0] );
  _checkError("glWeightbvARB");
}

FABRIC_EXT_EXPORT void glWeightdvARB_wrapper(
  KL::Integer size,
  KL::VariableArray<KL::Scalar> & weights
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWeightdvARB( %d, GLdouble* );\n", (int)size);
#endif
  glWeightdvARB( (GLint)size, (GLdouble*)&weights[0] );
  _checkError("glWeightdvARB");
}

FABRIC_EXT_EXPORT void glWeightfvARB_wrapper(
  KL::Integer size,
  KL::VariableArray<KL::Scalar> & weights
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWeightfvARB( %d, GLfloat* );\n", (int)size);
#endif
  glWeightfvARB( (GLint)size, (GLfloat*)&weights[0] );
  _checkError("glWeightfvARB");
}

FABRIC_EXT_EXPORT void glWeightivARB_wrapper(
  KL::Integer size,
  KL::VariableArray<KL::Integer> & weights
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWeightivARB( %d, GLint* );\n", (int)size);
#endif
  glWeightivARB( (GLint)size, (GLint*)&weights[0] );
  _checkError("glWeightivARB");
}

FABRIC_EXT_EXPORT void glWeightsvARB_wrapper(
  KL::Integer size,
  KL::VariableArray<KL::Integer> & weights
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWeightsvARB( %d, GLshort* );\n", (int)size);
#endif
  glWeightsvARB( (GLint)size, (GLshort*)&weights[0] );
  _checkError("glWeightsvARB");
}

FABRIC_EXT_EXPORT void glWeightubvARB_wrapper(
  KL::Integer size,
  KL::VariableArray<KL::Size> & weights
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWeightubvARB( %d, GLubyte* );\n", (int)size);
#endif
  glWeightubvARB( (GLint)size, (GLubyte*)&weights[0] );
  _checkError("glWeightubvARB");
}

FABRIC_EXT_EXPORT void glWeightuivARB_wrapper(
  KL::Integer size,
  KL::VariableArray<KL::Size> & weights
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWeightuivARB( %d, GLuint* );\n", (int)size);
#endif
  glWeightuivARB( (GLint)size, (GLuint*)&weights[0] );
  _checkError("glWeightuivARB");
}

FABRIC_EXT_EXPORT void glWeightusvARB_wrapper(
  KL::Integer size,
  KL::VariableArray<KL::Integer> & weights
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWeightusvARB( %d, GLushort* );\n", (int)size);
#endif
  glWeightusvARB( (GLint)size, (GLushort*)&weights[0] );
  _checkError("glWeightusvARB");
}

FABRIC_EXT_EXPORT void glBindBufferARB_wrapper(
  KL::Size target,
  KL::Size buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindBufferARB( %d, 0x%04X );\n", (int)target, (unsigned)buffer);
#endif
  glBindBufferARB( (GLenum)target, (GLuint)buffer );
  _checkError("glBindBufferARB");
}

FABRIC_EXT_EXPORT void glDeleteBuffersARB_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & buffers
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteBuffersARB( %d, GLuint* );\n", (int)n);
#endif
  glDeleteBuffersARB( (GLsizei)n, (const GLuint*)&buffers[0] );
  _checkError("glDeleteBuffersARB");
}

FABRIC_EXT_EXPORT void glGenBuffersARB_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & buffers
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenBuffersARB( %d, GLuint* );\n", (int)n);
#endif
  glGenBuffersARB( (GLsizei)n, (GLuint*)&buffers[0] );
  _checkError("glGenBuffersARB");
}

FABRIC_EXT_EXPORT void glGetBufferParameterivARB_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetBufferParameterivARB( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetBufferParameterivARB( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetBufferParameterivARB");
}

FABRIC_EXT_EXPORT KL::Boolean glIsBufferARB_wrapper(
  KL::Size buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsBufferARB( 0x%04X );\n", (unsigned)buffer);
#endif
  GLboolean result = glIsBufferARB( (GLuint)buffer );
  _checkError("glIsBufferARB");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT GLvoid glMapBufferARB_wrapper(
  KL::Size target,
  KL::Size access
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMapBufferARB( %d, %d );\n", (int)target, (int)access);
#endif
  glMapBufferARB( (GLenum)target, (GLenum)access );
  _checkError("glMapBufferARB");
}

FABRIC_EXT_EXPORT KL::Boolean glUnmapBufferARB_wrapper(
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUnmapBufferARB( %d );\n", (int)target);
#endif
  GLboolean result = glUnmapBufferARB( (GLenum)target );
  _checkError("glUnmapBufferARB");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glBindProgramARB_wrapper(
  KL::Size target,
  KL::Size program
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindProgramARB( %d, 0x%04X );\n", (int)target, (unsigned)program);
#endif
  glBindProgramARB( (GLenum)target, (GLuint)program );
  _checkError("glBindProgramARB");
}

FABRIC_EXT_EXPORT void glDeleteProgramsARB_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & programs
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteProgramsARB( %d, GLuint* );\n", (int)n);
#endif
  glDeleteProgramsARB( (GLsizei)n, (const GLuint*)&programs[0] );
  _checkError("glDeleteProgramsARB");
}

FABRIC_EXT_EXPORT void glDisableVertexAttribArrayARB_wrapper(
  KL::Size index
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDisableVertexAttribArrayARB( 0x%04X );\n", (unsigned)index);
#endif
  glDisableVertexAttribArrayARB( (GLuint)index );
  _checkError("glDisableVertexAttribArrayARB");
}

FABRIC_EXT_EXPORT void glEnableVertexAttribArrayARB_wrapper(
  KL::Size index
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEnableVertexAttribArrayARB( 0x%04X );\n", (unsigned)index);
#endif
  glEnableVertexAttribArrayARB( (GLuint)index );
  _checkError("glEnableVertexAttribArrayARB");
}

FABRIC_EXT_EXPORT void glGenProgramsARB_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & programs
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenProgramsARB( %d, GLuint* );\n", (int)n);
#endif
  glGenProgramsARB( (GLsizei)n, (GLuint*)&programs[0] );
  _checkError("glGenProgramsARB");
}

FABRIC_EXT_EXPORT void glGetProgramEnvParameterdvARB_wrapper(
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetProgramEnvParameterdvARB( %d, 0x%04X, GLdouble* );\n", (int)target, (unsigned)index);
#endif
  glGetProgramEnvParameterdvARB( (GLenum)target, (GLuint)index, (GLdouble*)&params[0] );
  _checkError("glGetProgramEnvParameterdvARB");
}

FABRIC_EXT_EXPORT void glGetProgramEnvParameterfvARB_wrapper(
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetProgramEnvParameterfvARB( %d, 0x%04X, GLfloat* );\n", (int)target, (unsigned)index);
#endif
  glGetProgramEnvParameterfvARB( (GLenum)target, (GLuint)index, (GLfloat*)&params[0] );
  _checkError("glGetProgramEnvParameterfvARB");
}

FABRIC_EXT_EXPORT void glGetProgramLocalParameterdvARB_wrapper(
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetProgramLocalParameterdvARB( %d, 0x%04X, GLdouble* );\n", (int)target, (unsigned)index);
#endif
  glGetProgramLocalParameterdvARB( (GLenum)target, (GLuint)index, (GLdouble*)&params[0] );
  _checkError("glGetProgramLocalParameterdvARB");
}

FABRIC_EXT_EXPORT void glGetProgramLocalParameterfvARB_wrapper(
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetProgramLocalParameterfvARB( %d, 0x%04X, GLfloat* );\n", (int)target, (unsigned)index);
#endif
  glGetProgramLocalParameterfvARB( (GLenum)target, (GLuint)index, (GLfloat*)&params[0] );
  _checkError("glGetProgramLocalParameterfvARB");
}

FABRIC_EXT_EXPORT void glGetProgramStringARB_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Data string
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetProgramStringARB( %d, %d, void* );\n", (int)target, (int)pname);
#endif
  glGetProgramStringARB( (GLenum)target, (GLenum)pname, string );
  _checkError("glGetProgramStringARB");
}

FABRIC_EXT_EXPORT void glGetProgramivARB_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetProgramivARB( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetProgramivARB( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetProgramivARB");
}

FABRIC_EXT_EXPORT void glGetVertexAttribdvARB_wrapper(
  KL::Size index,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexAttribdvARB( 0x%04X, %d, GLdouble* );\n", (unsigned)index, (int)pname);
#endif
  glGetVertexAttribdvARB( (GLuint)index, (GLenum)pname, (GLdouble*)&params[0] );
  _checkError("glGetVertexAttribdvARB");
}

FABRIC_EXT_EXPORT void glGetVertexAttribfvARB_wrapper(
  KL::Size index,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexAttribfvARB( 0x%04X, %d, GLfloat* );\n", (unsigned)index, (int)pname);
#endif
  glGetVertexAttribfvARB( (GLuint)index, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetVertexAttribfvARB");
}

FABRIC_EXT_EXPORT void glGetVertexAttribivARB_wrapper(
  KL::Size index,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexAttribivARB( 0x%04X, %d, GLint* );\n", (unsigned)index, (int)pname);
#endif
  glGetVertexAttribivARB( (GLuint)index, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetVertexAttribivARB");
}

FABRIC_EXT_EXPORT KL::Boolean glIsProgramARB_wrapper(
  KL::Size program
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsProgramARB( 0x%04X );\n", (unsigned)program);
#endif
  GLboolean result = glIsProgramARB( (GLuint)program );
  _checkError("glIsProgramARB");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glProgramEnvParameter4dARB_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramEnvParameter4dARB( %d, 0x%04X, %f, %f, %f, %f );\n", (int)target, (unsigned)index, (float)x, (float)y, (float)z, (float)w);
#endif
  glProgramEnvParameter4dARB( (GLenum)target, (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  _checkError("glProgramEnvParameter4dARB");
}

FABRIC_EXT_EXPORT void glProgramEnvParameter4dvARB_wrapper(
  KL::Size target,
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramEnvParameter4dvARB( %d, 0x%04X, GLdouble* );\n", (int)target, (unsigned)index);
#endif
  glProgramEnvParameter4dvARB( (GLenum)target, (GLuint)index, (const GLdouble*)&params[0] );
  _checkError("glProgramEnvParameter4dvARB");
}

FABRIC_EXT_EXPORT void glProgramEnvParameter4fARB_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramEnvParameter4fARB( %d, 0x%04X, %f, %f, %f, %f );\n", (int)target, (unsigned)index, (float)x, (float)y, (float)z, (float)w);
#endif
  glProgramEnvParameter4fARB( (GLenum)target, (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  _checkError("glProgramEnvParameter4fARB");
}

FABRIC_EXT_EXPORT void glProgramEnvParameter4fvARB_wrapper(
  KL::Size target,
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramEnvParameter4fvARB( %d, 0x%04X, GLfloat* );\n", (int)target, (unsigned)index);
#endif
  glProgramEnvParameter4fvARB( (GLenum)target, (GLuint)index, (const GLfloat*)&params[0] );
  _checkError("glProgramEnvParameter4fvARB");
}

FABRIC_EXT_EXPORT void glProgramLocalParameter4dARB_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramLocalParameter4dARB( %d, 0x%04X, %f, %f, %f, %f );\n", (int)target, (unsigned)index, (float)x, (float)y, (float)z, (float)w);
#endif
  glProgramLocalParameter4dARB( (GLenum)target, (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  _checkError("glProgramLocalParameter4dARB");
}

FABRIC_EXT_EXPORT void glProgramLocalParameter4dvARB_wrapper(
  KL::Size target,
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramLocalParameter4dvARB( %d, 0x%04X, GLdouble* );\n", (int)target, (unsigned)index);
#endif
  glProgramLocalParameter4dvARB( (GLenum)target, (GLuint)index, (const GLdouble*)&params[0] );
  _checkError("glProgramLocalParameter4dvARB");
}

FABRIC_EXT_EXPORT void glProgramLocalParameter4fARB_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramLocalParameter4fARB( %d, 0x%04X, %f, %f, %f, %f );\n", (int)target, (unsigned)index, (float)x, (float)y, (float)z, (float)w);
#endif
  glProgramLocalParameter4fARB( (GLenum)target, (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  _checkError("glProgramLocalParameter4fARB");
}

FABRIC_EXT_EXPORT void glProgramLocalParameter4fvARB_wrapper(
  KL::Size target,
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramLocalParameter4fvARB( %d, 0x%04X, GLfloat* );\n", (int)target, (unsigned)index);
#endif
  glProgramLocalParameter4fvARB( (GLenum)target, (GLuint)index, (const GLfloat*)&params[0] );
  _checkError("glProgramLocalParameter4fvARB");
}

FABRIC_EXT_EXPORT void glProgramStringARB_wrapper(
  KL::Size target,
  KL::Size format,
  KL::Size len,
  KL::Data string
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramStringARB( %d, %d, %d, void* );\n", (int)target, (int)format, (int)len);
#endif
  glProgramStringARB( (GLenum)target, (GLenum)format, (GLsizei)len, string );
  _checkError("glProgramStringARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib1dARB_wrapper(
  KL::Size index,
  KL::Scalar x
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib1dARB( 0x%04X, %f );\n", (unsigned)index, (float)x);
#endif
  glVertexAttrib1dARB( (GLuint)index, (GLdouble)x );
  _checkError("glVertexAttrib1dARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib1dvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib1dvARB( 0x%04X, GLdouble* );\n", (unsigned)index);
#endif
  glVertexAttrib1dvARB( (GLuint)index, (const GLdouble*)&v[0] );
  _checkError("glVertexAttrib1dvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib1fARB_wrapper(
  KL::Size index,
  KL::Scalar x
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib1fARB( 0x%04X, %f );\n", (unsigned)index, (float)x);
#endif
  glVertexAttrib1fARB( (GLuint)index, (GLfloat)x );
  _checkError("glVertexAttrib1fARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib1fvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib1fvARB( 0x%04X, GLfloat* );\n", (unsigned)index);
#endif
  glVertexAttrib1fvARB( (GLuint)index, (const GLfloat*)&v[0] );
  _checkError("glVertexAttrib1fvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib1sARB_wrapper(
  KL::Size index,
  KL::Integer x
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib1sARB( 0x%04X, %d );\n", (unsigned)index, (int)x);
#endif
  glVertexAttrib1sARB( (GLuint)index, (GLshort)x );
  _checkError("glVertexAttrib1sARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib1svARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib1svARB( 0x%04X, GLshort* );\n", (unsigned)index);
#endif
  glVertexAttrib1svARB( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib1svARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib2dARB_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib2dARB( 0x%04X, %f, %f );\n", (unsigned)index, (float)x, (float)y);
#endif
  glVertexAttrib2dARB( (GLuint)index, (GLdouble)x, (GLdouble)y );
  _checkError("glVertexAttrib2dARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib2dvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib2dvARB( 0x%04X, GLdouble* );\n", (unsigned)index);
#endif
  glVertexAttrib2dvARB( (GLuint)index, (const GLdouble*)&v[0] );
  _checkError("glVertexAttrib2dvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib2fARB_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib2fARB( 0x%04X, %f, %f );\n", (unsigned)index, (float)x, (float)y);
#endif
  glVertexAttrib2fARB( (GLuint)index, (GLfloat)x, (GLfloat)y );
  _checkError("glVertexAttrib2fARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib2fvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib2fvARB( 0x%04X, GLfloat* );\n", (unsigned)index);
#endif
  glVertexAttrib2fvARB( (GLuint)index, (const GLfloat*)&v[0] );
  _checkError("glVertexAttrib2fvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib2sARB_wrapper(
  KL::Size index,
  KL::Integer x,
  KL::Integer y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib2sARB( 0x%04X, %d, %d );\n", (unsigned)index, (int)x, (int)y);
#endif
  glVertexAttrib2sARB( (GLuint)index, (GLshort)x, (GLshort)y );
  _checkError("glVertexAttrib2sARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib2svARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib2svARB( 0x%04X, GLshort* );\n", (unsigned)index);
#endif
  glVertexAttrib2svARB( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib2svARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib3dARB_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib3dARB( 0x%04X, %f, %f, %f );\n", (unsigned)index, (float)x, (float)y, (float)z);
#endif
  glVertexAttrib3dARB( (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glVertexAttrib3dARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib3dvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib3dvARB( 0x%04X, GLdouble* );\n", (unsigned)index);
#endif
  glVertexAttrib3dvARB( (GLuint)index, (const GLdouble*)&v[0] );
  _checkError("glVertexAttrib3dvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib3fARB_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib3fARB( 0x%04X, %f, %f, %f );\n", (unsigned)index, (float)x, (float)y, (float)z);
#endif
  glVertexAttrib3fARB( (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glVertexAttrib3fARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib3fvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib3fvARB( 0x%04X, GLfloat* );\n", (unsigned)index);
#endif
  glVertexAttrib3fvARB( (GLuint)index, (const GLfloat*)&v[0] );
  _checkError("glVertexAttrib3fvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib3sARB_wrapper(
  KL::Size index,
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib3sARB( 0x%04X, %d, %d, %d );\n", (unsigned)index, (int)x, (int)y, (int)z);
#endif
  glVertexAttrib3sARB( (GLuint)index, (GLshort)x, (GLshort)y, (GLshort)z );
  _checkError("glVertexAttrib3sARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib3svARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib3svARB( 0x%04X, GLshort* );\n", (unsigned)index);
#endif
  glVertexAttrib3svARB( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib3svARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4NbvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4NbvARB( 0x%04X, GLbyte* );\n", (unsigned)index);
#endif
  glVertexAttrib4NbvARB( (GLuint)index, (const GLbyte*)&v[0] );
  _checkError("glVertexAttrib4NbvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4NivARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4NivARB( 0x%04X, GLint* );\n", (unsigned)index);
#endif
  glVertexAttrib4NivARB( (GLuint)index, (const GLint*)&v[0] );
  _checkError("glVertexAttrib4NivARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4NsvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4NsvARB( 0x%04X, GLshort* );\n", (unsigned)index);
#endif
  glVertexAttrib4NsvARB( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib4NsvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4NubARB_wrapper(
  KL::Size index,
  KL::Size x,
  KL::Size y,
  KL::Size z,
  KL::Size w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4NubARB( 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)index, (unsigned)x, (unsigned)y, (unsigned)z, (unsigned)w);
#endif
  glVertexAttrib4NubARB( (GLuint)index, (GLubyte)x, (GLubyte)y, (GLubyte)z, (GLubyte)w );
  _checkError("glVertexAttrib4NubARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4NubvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4NubvARB( 0x%04X, GLubyte* );\n", (unsigned)index);
#endif
  glVertexAttrib4NubvARB( (GLuint)index, (const GLubyte*)&v[0] );
  _checkError("glVertexAttrib4NubvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4NuivARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4NuivARB( 0x%04X, GLuint* );\n", (unsigned)index);
#endif
  glVertexAttrib4NuivARB( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttrib4NuivARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4NusvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4NusvARB( 0x%04X, GLushort* );\n", (unsigned)index);
#endif
  glVertexAttrib4NusvARB( (GLuint)index, (const GLushort*)&v[0] );
  _checkError("glVertexAttrib4NusvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4bvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4bvARB( 0x%04X, GLbyte* );\n", (unsigned)index);
#endif
  glVertexAttrib4bvARB( (GLuint)index, (const GLbyte*)&v[0] );
  _checkError("glVertexAttrib4bvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4dARB_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4dARB( 0x%04X, %f, %f, %f, %f );\n", (unsigned)index, (float)x, (float)y, (float)z, (float)w);
#endif
  glVertexAttrib4dARB( (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  _checkError("glVertexAttrib4dARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4dvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4dvARB( 0x%04X, GLdouble* );\n", (unsigned)index);
#endif
  glVertexAttrib4dvARB( (GLuint)index, (const GLdouble*)&v[0] );
  _checkError("glVertexAttrib4dvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4fARB_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4fARB( 0x%04X, %f, %f, %f, %f );\n", (unsigned)index, (float)x, (float)y, (float)z, (float)w);
#endif
  glVertexAttrib4fARB( (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  _checkError("glVertexAttrib4fARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4fvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4fvARB( 0x%04X, GLfloat* );\n", (unsigned)index);
#endif
  glVertexAttrib4fvARB( (GLuint)index, (const GLfloat*)&v[0] );
  _checkError("glVertexAttrib4fvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4ivARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4ivARB( 0x%04X, GLint* );\n", (unsigned)index);
#endif
  glVertexAttrib4ivARB( (GLuint)index, (const GLint*)&v[0] );
  _checkError("glVertexAttrib4ivARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4sARB_wrapper(
  KL::Size index,
  KL::Integer x,
  KL::Integer y,
  KL::Integer z,
  KL::Integer w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4sARB( 0x%04X, %d, %d, %d, %d );\n", (unsigned)index, (int)x, (int)y, (int)z, (int)w);
#endif
  glVertexAttrib4sARB( (GLuint)index, (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
  _checkError("glVertexAttrib4sARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4svARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4svARB( 0x%04X, GLshort* );\n", (unsigned)index);
#endif
  glVertexAttrib4svARB( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib4svARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4ubvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4ubvARB( 0x%04X, GLubyte* );\n", (unsigned)index);
#endif
  glVertexAttrib4ubvARB( (GLuint)index, (const GLubyte*)&v[0] );
  _checkError("glVertexAttrib4ubvARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4uivARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4uivARB( 0x%04X, GLuint* );\n", (unsigned)index);
#endif
  glVertexAttrib4uivARB( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttrib4uivARB");
}

FABRIC_EXT_EXPORT void glVertexAttrib4usvARB_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttrib4usvARB( 0x%04X, GLushort* );\n", (unsigned)index);
#endif
  glVertexAttrib4usvARB( (GLuint)index, (const GLushort*)&v[0] );
  _checkError("glVertexAttrib4usvARB");
}

FABRIC_EXT_EXPORT void glVertexAttribPointerARB_wrapper(
  KL::Size index,
  KL::Integer size,
  KL::Size type,
  KL::Boolean normalized,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribPointerARB( 0x%04X, %d, %d, %d, %d, void* );\n", (unsigned)index, (int)size, (int)type, (int)normalized, (int)stride);
#endif
  glVertexAttribPointerARB( (GLuint)index, (GLint)size, (GLenum)type, (GLboolean)normalized, (GLsizei)stride, pointer );
  _checkError("glVertexAttribPointerARB");
}

FABRIC_EXT_EXPORT void glWindowPos2dARB_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2dARB( %f, %f );\n", (float)x, (float)y);
#endif
  glWindowPos2dARB( (GLdouble)x, (GLdouble)y );
  _checkError("glWindowPos2dARB");
}

FABRIC_EXT_EXPORT void glWindowPos2dvARB_wrapper(
  const KL::VariableArray<KL::Scalar> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2dvARB( GLdouble* );\n");
#endif
  glWindowPos2dvARB( (const GLdouble*)&p[0] );
  _checkError("glWindowPos2dvARB");
}

FABRIC_EXT_EXPORT void glWindowPos2fARB_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2fARB( %f, %f );\n", (float)x, (float)y);
#endif
  glWindowPos2fARB( (GLfloat)x, (GLfloat)y );
  _checkError("glWindowPos2fARB");
}

FABRIC_EXT_EXPORT void glWindowPos2fvARB_wrapper(
  const KL::VariableArray<KL::Scalar> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2fvARB( GLfloat* );\n");
#endif
  glWindowPos2fvARB( (const GLfloat*)&p[0] );
  _checkError("glWindowPos2fvARB");
}

FABRIC_EXT_EXPORT void glWindowPos2iARB_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2iARB( %d, %d );\n", (int)x, (int)y);
#endif
  glWindowPos2iARB( (GLint)x, (GLint)y );
  _checkError("glWindowPos2iARB");
}

FABRIC_EXT_EXPORT void glWindowPos2ivARB_wrapper(
  const KL::VariableArray<KL::Integer> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2ivARB( GLint* );\n");
#endif
  glWindowPos2ivARB( (const GLint*)&p[0] );
  _checkError("glWindowPos2ivARB");
}

FABRIC_EXT_EXPORT void glWindowPos2sARB_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2sARB( %d, %d );\n", (int)x, (int)y);
#endif
  glWindowPos2sARB( (GLshort)x, (GLshort)y );
  _checkError("glWindowPos2sARB");
}

FABRIC_EXT_EXPORT void glWindowPos2svARB_wrapper(
  const KL::VariableArray<KL::Integer> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos2svARB( GLshort* );\n");
#endif
  glWindowPos2svARB( (const GLshort*)&p[0] );
  _checkError("glWindowPos2svARB");
}

FABRIC_EXT_EXPORT void glWindowPos3dARB_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3dARB( %f, %f, %f );\n", (float)x, (float)y, (float)z);
#endif
  glWindowPos3dARB( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glWindowPos3dARB");
}

FABRIC_EXT_EXPORT void glWindowPos3dvARB_wrapper(
  const KL::VariableArray<KL::Scalar> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3dvARB( GLdouble* );\n");
#endif
  glWindowPos3dvARB( (const GLdouble*)&p[0] );
  _checkError("glWindowPos3dvARB");
}

FABRIC_EXT_EXPORT void glWindowPos3fARB_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3fARB( %f, %f, %f );\n", (float)x, (float)y, (float)z);
#endif
  glWindowPos3fARB( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glWindowPos3fARB");
}

FABRIC_EXT_EXPORT void glWindowPos3fvARB_wrapper(
  const KL::VariableArray<KL::Scalar> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3fvARB( GLfloat* );\n");
#endif
  glWindowPos3fvARB( (const GLfloat*)&p[0] );
  _checkError("glWindowPos3fvARB");
}

FABRIC_EXT_EXPORT void glWindowPos3iARB_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3iARB( %d, %d, %d );\n", (int)x, (int)y, (int)z);
#endif
  glWindowPos3iARB( (GLint)x, (GLint)y, (GLint)z );
  _checkError("glWindowPos3iARB");
}

FABRIC_EXT_EXPORT void glWindowPos3ivARB_wrapper(
  const KL::VariableArray<KL::Integer> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3ivARB( GLint* );\n");
#endif
  glWindowPos3ivARB( (const GLint*)&p[0] );
  _checkError("glWindowPos3ivARB");
}

FABRIC_EXT_EXPORT void glWindowPos3sARB_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3sARB( %d, %d, %d );\n", (int)x, (int)y, (int)z);
#endif
  glWindowPos3sARB( (GLshort)x, (GLshort)y, (GLshort)z );
  _checkError("glWindowPos3sARB");
}

FABRIC_EXT_EXPORT void glWindowPos3svARB_wrapper(
  const KL::VariableArray<KL::Integer> & p
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWindowPos3svARB( GLshort* );\n");
#endif
  glWindowPos3svARB( (const GLshort*)&p[0] );
  _checkError("glWindowPos3svARB");
}

FABRIC_EXT_EXPORT KL::Integer glGetUniformBufferSizeEXT_wrapper(
  KL::Size program,
  KL::Integer location
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetUniformBufferSizeEXT( 0x%04X, %d );\n", (unsigned)program, (int)location);
#endif
  GLint result = glGetUniformBufferSizeEXT( (GLuint)program, (GLint)location );
  _checkError("glGetUniformBufferSizeEXT");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer glGetUniformOffsetEXT_wrapper(
  KL::Size program,
  KL::Integer location
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetUniformOffsetEXT( 0x%04X, %d );\n", (unsigned)program, (int)location);
#endif
  GLintptr result = glGetUniformOffsetEXT( (GLuint)program, (GLint)location );
  _checkError("glGetUniformOffsetEXT");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT void glUniformBufferEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniformBufferEXT( 0x%04X, %d, 0x%04X );\n", (unsigned)program, (int)location, (unsigned)buffer);
#endif
  glUniformBufferEXT( (GLuint)program, (GLint)location, (GLuint)buffer );
  _checkError("glUniformBufferEXT");
}

FABRIC_EXT_EXPORT void glBlendColorEXT_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue,
  KL::Scalar alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendColorEXT( %f, %f, %f, %f );\n", (float)red, (float)green, (float)blue, (float)alpha);
#endif
  glBlendColorEXT( (GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha );
  _checkError("glBlendColorEXT");
}

FABRIC_EXT_EXPORT void glBlendEquationSeparateEXT_wrapper(
  KL::Size modeRGB,
  KL::Size modeAlpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendEquationSeparateEXT( %d, %d );\n", (int)modeRGB, (int)modeAlpha);
#endif
  glBlendEquationSeparateEXT( (GLenum)modeRGB, (GLenum)modeAlpha );
  _checkError("glBlendEquationSeparateEXT");
}

FABRIC_EXT_EXPORT void glBlendFuncSeparateEXT_wrapper(
  KL::Size sfactorRGB,
  KL::Size dfactorRGB,
  KL::Size sfactorAlpha,
  KL::Size dfactorAlpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendFuncSeparateEXT( %d, %d, %d, %d );\n", (int)sfactorRGB, (int)dfactorRGB, (int)sfactorAlpha, (int)dfactorAlpha);
#endif
  glBlendFuncSeparateEXT( (GLenum)sfactorRGB, (GLenum)dfactorRGB, (GLenum)sfactorAlpha, (GLenum)dfactorAlpha );
  _checkError("glBlendFuncSeparateEXT");
}

FABRIC_EXT_EXPORT void glBlendEquationEXT_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlendEquationEXT( %d );\n", (int)mode);
#endif
  glBlendEquationEXT( (GLenum)mode );
  _checkError("glBlendEquationEXT");
}

FABRIC_EXT_EXPORT void glColorSubTableEXT_wrapper(
  KL::Size target,
  KL::Size start,
  KL::Size count,
  KL::Size format,
  KL::Size type,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColorSubTableEXT( %d, %d, %d, %d, %d, void* );\n", (int)target, (int)start, (int)count, (int)format, (int)type);
#endif
  glColorSubTableEXT( (GLenum)target, (GLsizei)start, (GLsizei)count, (GLenum)format, (GLenum)type, data );
  _checkError("glColorSubTableEXT");
}

FABRIC_EXT_EXPORT void glCopyColorSubTableEXT_wrapper(
  KL::Size target,
  KL::Size start,
  KL::Integer x,
  KL::Integer y,
  KL::Size width
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyColorSubTableEXT( %d, %d, %d, %d, %d );\n", (int)target, (int)start, (int)x, (int)y, (int)width);
#endif
  glCopyColorSubTableEXT( (GLenum)target, (GLsizei)start, (GLint)x, (GLint)y, (GLsizei)width );
  _checkError("glCopyColorSubTableEXT");
}

FABRIC_EXT_EXPORT void glLockArraysEXT_wrapper(
  KL::Integer first,
  KL::Size count
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLockArraysEXT( %d, %d );\n", (int)first, (int)count);
#endif
  glLockArraysEXT( (GLint)first, (GLsizei)count );
  _checkError("glLockArraysEXT");
}

FABRIC_EXT_EXPORT void glUnlockArraysEXT_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUnlockArraysEXT(  );\n");
#endif
  glUnlockArraysEXT();
  _checkError("glUnlockArraysEXT");
}

FABRIC_EXT_EXPORT void glConvolutionFilter1DEXT_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Size width,
  KL::Size format,
  KL::Size type,
  KL::Data image
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glConvolutionFilter1DEXT( %d, %d, %d, %d, %d, void* );\n", (int)target, (int)internalformat, (int)width, (int)format, (int)type);
#endif
  glConvolutionFilter1DEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLenum)format, (GLenum)type, image );
  _checkError("glConvolutionFilter1DEXT");
}

FABRIC_EXT_EXPORT void glConvolutionFilter2DEXT_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size type,
  KL::Data image
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glConvolutionFilter2DEXT( %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)internalformat, (int)width, (int)height, (int)format, (int)type);
#endif
  glConvolutionFilter2DEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, image );
  _checkError("glConvolutionFilter2DEXT");
}

FABRIC_EXT_EXPORT void glConvolutionParameterfEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glConvolutionParameterfEXT( %d, %d, %f );\n", (int)target, (int)pname, (float)param);
#endif
  glConvolutionParameterfEXT( (GLenum)target, (GLenum)pname, (GLfloat)param );
  _checkError("glConvolutionParameterfEXT");
}

FABRIC_EXT_EXPORT void glConvolutionParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glConvolutionParameterfvEXT( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glConvolutionParameterfvEXT( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glConvolutionParameterfvEXT");
}

FABRIC_EXT_EXPORT void glConvolutionParameteriEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glConvolutionParameteriEXT( %d, %d, %d );\n", (int)target, (int)pname, (int)param);
#endif
  glConvolutionParameteriEXT( (GLenum)target, (GLenum)pname, (GLint)param );
  _checkError("glConvolutionParameteriEXT");
}

FABRIC_EXT_EXPORT void glConvolutionParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glConvolutionParameterivEXT( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glConvolutionParameterivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glConvolutionParameterivEXT");
}

FABRIC_EXT_EXPORT void glCopyConvolutionFilter1DEXT_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyConvolutionFilter1DEXT( %d, %d, %d, %d, %d );\n", (int)target, (int)internalformat, (int)x, (int)y, (int)width);
#endif
  glCopyConvolutionFilter1DEXT( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width );
  _checkError("glCopyConvolutionFilter1DEXT");
}

FABRIC_EXT_EXPORT void glCopyConvolutionFilter2DEXT_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyConvolutionFilter2DEXT( %d, %d, %d, %d, %d, %d );\n", (int)target, (int)internalformat, (int)x, (int)y, (int)width, (int)height);
#endif
  glCopyConvolutionFilter2DEXT( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyConvolutionFilter2DEXT");
}

FABRIC_EXT_EXPORT void glGetConvolutionFilterEXT_wrapper(
  KL::Size target,
  KL::Size format,
  KL::Size type,
  KL::Data image
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetConvolutionFilterEXT( %d, %d, %d, void* );\n", (int)target, (int)format, (int)type);
#endif
  glGetConvolutionFilterEXT( (GLenum)target, (GLenum)format, (GLenum)type, image );
  _checkError("glGetConvolutionFilterEXT");
}

FABRIC_EXT_EXPORT void glGetConvolutionParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetConvolutionParameterfvEXT( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glGetConvolutionParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetConvolutionParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetConvolutionParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetConvolutionParameterivEXT( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetConvolutionParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetConvolutionParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetSeparableFilterEXT_wrapper(
  KL::Size target,
  KL::Size format,
  KL::Size type,
  KL::Data row,
  KL::Data column,
  KL::Data span
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetSeparableFilterEXT( %d, %d, %d, void*, void*, void* );\n", (int)target, (int)format, (int)type);
#endif
  glGetSeparableFilterEXT( (GLenum)target, (GLenum)format, (GLenum)type, row, column, span );
  _checkError("glGetSeparableFilterEXT");
}

FABRIC_EXT_EXPORT void glSeparableFilter2DEXT_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size type,
  KL::Data row,
  KL::Data column
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSeparableFilter2DEXT( %d, %d, %d, %d, %d, %d, void*, void* );\n", (int)target, (int)internalformat, (int)width, (int)height, (int)format, (int)type);
#endif
  glSeparableFilter2DEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, row, column );
  _checkError("glSeparableFilter2DEXT");
}

FABRIC_EXT_EXPORT void glBinormalPointerEXT_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBinormalPointerEXT( %d, %d, void* );\n", (int)type, (int)stride);
#endif
  glBinormalPointerEXT( (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glBinormalPointerEXT");
}

FABRIC_EXT_EXPORT void glTangentPointerEXT_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTangentPointerEXT( %d, %d, void* );\n", (int)type, (int)stride);
#endif
  glTangentPointerEXT( (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glTangentPointerEXT");
}

FABRIC_EXT_EXPORT void glCopyTexImage1DEXT_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Integer border
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTexImage1DEXT( %d, %d, %d, %d, %d, %d, %d );\n", (int)target, (int)level, (int)internalformat, (int)x, (int)y, (int)width, (int)border);
#endif
  glCopyTexImage1DEXT( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border );
  _checkError("glCopyTexImage1DEXT");
}

FABRIC_EXT_EXPORT void glCopyTexImage2DEXT_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height,
  KL::Integer border
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTexImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d );\n", (int)target, (int)level, (int)internalformat, (int)x, (int)y, (int)width, (int)height, (int)border);
#endif
  glCopyTexImage2DEXT( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border );
  _checkError("glCopyTexImage2DEXT");
}

FABRIC_EXT_EXPORT void glCopyTexSubImage1DEXT_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer x,
  KL::Integer y,
  KL::Size width
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTexSubImage1DEXT( %d, %d, %d, %d, %d, %d );\n", (int)target, (int)level, (int)xoffset, (int)x, (int)y, (int)width);
#endif
  glCopyTexSubImage1DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width );
  _checkError("glCopyTexSubImage1DEXT");
}

FABRIC_EXT_EXPORT void glCopyTexSubImage2DEXT_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTexSubImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d );\n", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)x, (int)y, (int)width, (int)height);
#endif
  glCopyTexSubImage2DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyTexSubImage2DEXT");
}

FABRIC_EXT_EXPORT void glCopyTexSubImage3DEXT_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer zoffset,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTexSubImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d );\n", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)x, (int)y, (int)width, (int)height);
#endif
  glCopyTexSubImage3DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyTexSubImage3DEXT");
}

FABRIC_EXT_EXPORT void glCullParameterdvEXT_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCullParameterdvEXT( %d, GLdouble* );\n", (int)pname);
#endif
  glCullParameterdvEXT( (GLenum)pname, (GLdouble*)&params[0] );
  _checkError("glCullParameterdvEXT");
}

FABRIC_EXT_EXPORT void glCullParameterfvEXT_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCullParameterfvEXT( %d, GLfloat* );\n", (int)pname);
#endif
  glCullParameterfvEXT( (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glCullParameterfvEXT");
}

FABRIC_EXT_EXPORT void glDepthBoundsEXT_wrapper(
  KL::Scalar zmin,
  KL::Scalar zmax
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDepthBoundsEXT( %f, %f );\n", (float)zmin, (float)zmax);
#endif
  glDepthBoundsEXT( (GLclampd)zmin, (GLclampd)zmax );
  _checkError("glDepthBoundsEXT");
}

FABRIC_EXT_EXPORT void glBindMultiTextureEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size texture
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindMultiTextureEXT( %d, %d, 0x%04X );\n", (int)texunit, (int)target, (unsigned)texture);
#endif
  glBindMultiTextureEXT( (GLenum)texunit, (GLenum)target, (GLuint)texture );
  _checkError("glBindMultiTextureEXT");
}

FABRIC_EXT_EXPORT KL::Size glCheckNamedFramebufferStatusEXT_wrapper(
  KL::Size framebuffer,
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCheckNamedFramebufferStatusEXT( 0x%04X, %d );\n", (unsigned)framebuffer, (int)target);
#endif
  GLenum result = glCheckNamedFramebufferStatusEXT( (GLuint)framebuffer, (GLenum)target );
  _checkError("glCheckNamedFramebufferStatusEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glClientAttribDefaultEXT_wrapper(
  KL::Size mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClientAttribDefaultEXT( 0x%04X );\n", (unsigned)mask);
#endif
  glClientAttribDefaultEXT( (GLbitfield)mask );
  _checkError("glClientAttribDefaultEXT");
}

FABRIC_EXT_EXPORT void glCompressedMultiTexImage1DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Integer border,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedMultiTexImage1DEXT( %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)imageSize);
#endif
  glCompressedMultiTexImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLint)border, (GLsizei)imageSize, data );
  _checkError("glCompressedMultiTexImage1DEXT");
}

FABRIC_EXT_EXPORT void glCompressedMultiTexImage2DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Integer border,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedMultiTexImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)imageSize);
#endif
  glCompressedMultiTexImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLsizei)imageSize, data );
  _checkError("glCompressedMultiTexImage2DEXT");
}

FABRIC_EXT_EXPORT void glCompressedMultiTexImage3DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Integer border,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedMultiTexImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)imageSize);
#endif
  glCompressedMultiTexImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLsizei)imageSize, data );
  _checkError("glCompressedMultiTexImage3DEXT");
}

FABRIC_EXT_EXPORT void glCompressedMultiTexSubImage1DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Size width,
  KL::Size format,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedMultiTexSubImage1DEXT( %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)imageSize);
#endif
  glCompressedMultiTexSubImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedMultiTexSubImage1DEXT");
}

FABRIC_EXT_EXPORT void glCompressedMultiTexSubImage2DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedMultiTexSubImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)imageSize);
#endif
  glCompressedMultiTexSubImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedMultiTexSubImage2DEXT");
}

FABRIC_EXT_EXPORT void glCompressedMultiTexSubImage3DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer zoffset,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Size format,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedMultiTexSubImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)imageSize);
#endif
  glCompressedMultiTexSubImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedMultiTexSubImage3DEXT");
}

FABRIC_EXT_EXPORT void glCompressedTextureImage1DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Integer border,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTextureImage1DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)imageSize);
#endif
  glCompressedTextureImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLint)border, (GLsizei)imageSize, data );
  _checkError("glCompressedTextureImage1DEXT");
}

FABRIC_EXT_EXPORT void glCompressedTextureImage2DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Integer border,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTextureImage2DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)imageSize);
#endif
  glCompressedTextureImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLsizei)imageSize, data );
  _checkError("glCompressedTextureImage2DEXT");
}

FABRIC_EXT_EXPORT void glCompressedTextureImage3DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Integer border,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTextureImage3DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)imageSize);
#endif
  glCompressedTextureImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLsizei)imageSize, data );
  _checkError("glCompressedTextureImage3DEXT");
}

FABRIC_EXT_EXPORT void glCompressedTextureSubImage1DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Size width,
  KL::Size format,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTextureSubImage1DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)imageSize);
#endif
  glCompressedTextureSubImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedTextureSubImage1DEXT");
}

FABRIC_EXT_EXPORT void glCompressedTextureSubImage2DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTextureSubImage2DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)imageSize);
#endif
  glCompressedTextureSubImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedTextureSubImage2DEXT");
}

FABRIC_EXT_EXPORT void glCompressedTextureSubImage3DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer zoffset,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Size format,
  KL::Size imageSize,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCompressedTextureSubImage3DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)imageSize);
#endif
  glCompressedTextureSubImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedTextureSubImage3DEXT");
}

FABRIC_EXT_EXPORT void glCopyMultiTexImage1DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Integer border
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyMultiTexImage1DEXT( %d, %d, %d, %d, %d, %d, %d, %d );\n", (int)texunit, (int)target, (int)level, (int)internalformat, (int)x, (int)y, (int)width, (int)border);
#endif
  glCopyMultiTexImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border );
  _checkError("glCopyMultiTexImage1DEXT");
}

FABRIC_EXT_EXPORT void glCopyMultiTexImage2DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height,
  KL::Integer border
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyMultiTexImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d );\n", (int)texunit, (int)target, (int)level, (int)internalformat, (int)x, (int)y, (int)width, (int)height, (int)border);
#endif
  glCopyMultiTexImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border );
  _checkError("glCopyMultiTexImage2DEXT");
}

FABRIC_EXT_EXPORT void glCopyMultiTexSubImage1DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer x,
  KL::Integer y,
  KL::Size width
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyMultiTexSubImage1DEXT( %d, %d, %d, %d, %d, %d, %d );\n", (int)texunit, (int)target, (int)level, (int)xoffset, (int)x, (int)y, (int)width);
#endif
  glCopyMultiTexSubImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width );
  _checkError("glCopyMultiTexSubImage1DEXT");
}

FABRIC_EXT_EXPORT void glCopyMultiTexSubImage2DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyMultiTexSubImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d );\n", (int)texunit, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)x, (int)y, (int)width, (int)height);
#endif
  glCopyMultiTexSubImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyMultiTexSubImage2DEXT");
}

FABRIC_EXT_EXPORT void glCopyMultiTexSubImage3DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer zoffset,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyMultiTexSubImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d );\n", (int)texunit, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)x, (int)y, (int)width, (int)height);
#endif
  glCopyMultiTexSubImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyMultiTexSubImage3DEXT");
}

FABRIC_EXT_EXPORT void glCopyTextureImage1DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Integer border
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTextureImage1DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d );\n", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)x, (int)y, (int)width, (int)border);
#endif
  glCopyTextureImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border );
  _checkError("glCopyTextureImage1DEXT");
}

FABRIC_EXT_EXPORT void glCopyTextureImage2DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height,
  KL::Integer border
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTextureImage2DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d );\n", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)x, (int)y, (int)width, (int)height, (int)border);
#endif
  glCopyTextureImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border );
  _checkError("glCopyTextureImage2DEXT");
}

FABRIC_EXT_EXPORT void glCopyTextureSubImage1DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer x,
  KL::Integer y,
  KL::Size width
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTextureSubImage1DEXT( 0x%04X, %d, %d, %d, %d, %d, %d );\n", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)x, (int)y, (int)width);
#endif
  glCopyTextureSubImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width );
  _checkError("glCopyTextureSubImage1DEXT");
}

FABRIC_EXT_EXPORT void glCopyTextureSubImage2DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTextureSubImage2DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d );\n", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)x, (int)y, (int)width, (int)height);
#endif
  glCopyTextureSubImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyTextureSubImage2DEXT");
}

FABRIC_EXT_EXPORT void glCopyTextureSubImage3DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer zoffset,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCopyTextureSubImage3DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, %d );\n", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)x, (int)y, (int)width, (int)height);
#endif
  glCopyTextureSubImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyTextureSubImage3DEXT");
}

FABRIC_EXT_EXPORT void glDisableClientStateIndexedEXT_wrapper(
  KL::Size array,
  KL::Size index
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDisableClientStateIndexedEXT( %d, 0x%04X );\n", (int)array, (unsigned)index);
#endif
  glDisableClientStateIndexedEXT( (GLenum)array, (GLuint)index );
  _checkError("glDisableClientStateIndexedEXT");
}

FABRIC_EXT_EXPORT void glDisableClientStateiEXT_wrapper(
  KL::Size array,
  KL::Size index
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDisableClientStateiEXT( %d, 0x%04X );\n", (int)array, (unsigned)index);
#endif
  glDisableClientStateiEXT( (GLenum)array, (GLuint)index );
  _checkError("glDisableClientStateiEXT");
}

FABRIC_EXT_EXPORT void glDisableVertexArrayAttribEXT_wrapper(
  KL::Size vaobj,
  KL::Size index
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDisableVertexArrayAttribEXT( 0x%04X, 0x%04X );\n", (unsigned)vaobj, (unsigned)index);
#endif
  glDisableVertexArrayAttribEXT( (GLuint)vaobj, (GLuint)index );
  _checkError("glDisableVertexArrayAttribEXT");
}

FABRIC_EXT_EXPORT void glDisableVertexArrayEXT_wrapper(
  KL::Size vaobj,
  KL::Size array
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDisableVertexArrayEXT( 0x%04X, %d );\n", (unsigned)vaobj, (int)array);
#endif
  glDisableVertexArrayEXT( (GLuint)vaobj, (GLenum)array );
  _checkError("glDisableVertexArrayEXT");
}

FABRIC_EXT_EXPORT void glEnableClientStateIndexedEXT_wrapper(
  KL::Size array,
  KL::Size index
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEnableClientStateIndexedEXT( %d, 0x%04X );\n", (int)array, (unsigned)index);
#endif
  glEnableClientStateIndexedEXT( (GLenum)array, (GLuint)index );
  _checkError("glEnableClientStateIndexedEXT");
}

FABRIC_EXT_EXPORT void glEnableClientStateiEXT_wrapper(
  KL::Size array,
  KL::Size index
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEnableClientStateiEXT( %d, 0x%04X );\n", (int)array, (unsigned)index);
#endif
  glEnableClientStateiEXT( (GLenum)array, (GLuint)index );
  _checkError("glEnableClientStateiEXT");
}

FABRIC_EXT_EXPORT void glEnableVertexArrayAttribEXT_wrapper(
  KL::Size vaobj,
  KL::Size index
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEnableVertexArrayAttribEXT( 0x%04X, 0x%04X );\n", (unsigned)vaobj, (unsigned)index);
#endif
  glEnableVertexArrayAttribEXT( (GLuint)vaobj, (GLuint)index );
  _checkError("glEnableVertexArrayAttribEXT");
}

FABRIC_EXT_EXPORT void glEnableVertexArrayEXT_wrapper(
  KL::Size vaobj,
  KL::Size array
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEnableVertexArrayEXT( 0x%04X, %d );\n", (unsigned)vaobj, (int)array);
#endif
  glEnableVertexArrayEXT( (GLuint)vaobj, (GLenum)array );
  _checkError("glEnableVertexArrayEXT");
}

FABRIC_EXT_EXPORT void glFlushMappedNamedBufferRangeEXT_wrapper(
  KL::Size buffer,
  KL::Integer offset,
  KL::Size length
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFlushMappedNamedBufferRangeEXT( 0x%04X, %d, %d );\n", (unsigned)buffer, (int)offset, (int)length);
#endif
  glFlushMappedNamedBufferRangeEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)length );
  _checkError("glFlushMappedNamedBufferRangeEXT");
}

FABRIC_EXT_EXPORT void glFramebufferDrawBufferEXT_wrapper(
  KL::Size framebuffer,
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferDrawBufferEXT( 0x%04X, %d );\n", (unsigned)framebuffer, (int)mode);
#endif
  glFramebufferDrawBufferEXT( (GLuint)framebuffer, (GLenum)mode );
  _checkError("glFramebufferDrawBufferEXT");
}

FABRIC_EXT_EXPORT void glFramebufferDrawBuffersEXT_wrapper(
  KL::Size framebuffer,
  KL::Size n,
  const KL::VariableArray<KL::Size> & bufs
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferDrawBuffersEXT( 0x%04X, %d, GLenum* );\n", (unsigned)framebuffer, (int)n);
#endif
  glFramebufferDrawBuffersEXT( (GLuint)framebuffer, (GLsizei)n, (const GLenum*)&bufs[0] );
  _checkError("glFramebufferDrawBuffersEXT");
}

FABRIC_EXT_EXPORT void glFramebufferReadBufferEXT_wrapper(
  KL::Size framebuffer,
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferReadBufferEXT( 0x%04X, %d );\n", (unsigned)framebuffer, (int)mode);
#endif
  glFramebufferReadBufferEXT( (GLuint)framebuffer, (GLenum)mode );
  _checkError("glFramebufferReadBufferEXT");
}

FABRIC_EXT_EXPORT void glGenerateMultiTexMipmapEXT_wrapper(
  KL::Size texunit,
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenerateMultiTexMipmapEXT( %d, %d );\n", (int)texunit, (int)target);
#endif
  glGenerateMultiTexMipmapEXT( (GLenum)texunit, (GLenum)target );
  _checkError("glGenerateMultiTexMipmapEXT");
}

FABRIC_EXT_EXPORT void glGenerateTextureMipmapEXT_wrapper(
  KL::Size texture,
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenerateTextureMipmapEXT( 0x%04X, %d );\n", (unsigned)texture, (int)target);
#endif
  glGenerateTextureMipmapEXT( (GLuint)texture, (GLenum)target );
  _checkError("glGenerateTextureMipmapEXT");
}

FABRIC_EXT_EXPORT void glGetCompressedMultiTexImageEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Data img
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetCompressedMultiTexImageEXT( %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level);
#endif
  glGetCompressedMultiTexImageEXT( (GLenum)texunit, (GLenum)target, (GLint)level, img );
  _checkError("glGetCompressedMultiTexImageEXT");
}

FABRIC_EXT_EXPORT void glGetCompressedTextureImageEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Data img
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetCompressedTextureImageEXT( 0x%04X, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level);
#endif
  glGetCompressedTextureImageEXT( (GLuint)texture, (GLenum)target, (GLint)level, img );
  _checkError("glGetCompressedTextureImageEXT");
}

FABRIC_EXT_EXPORT void glGetDoubleIndexedvEXT_wrapper(
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetDoubleIndexedvEXT( %d, 0x%04X, GLdouble* );\n", (int)target, (unsigned)index);
#endif
  glGetDoubleIndexedvEXT( (GLenum)target, (GLuint)index, (GLdouble*)&params[0] );
  _checkError("glGetDoubleIndexedvEXT");
}

FABRIC_EXT_EXPORT void glGetDoublei_vEXT_wrapper(
  KL::Size pname,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetDoublei_vEXT( %d, 0x%04X, GLdouble* );\n", (int)pname, (unsigned)index);
#endif
  glGetDoublei_vEXT( (GLenum)pname, (GLuint)index, (GLdouble*)&params[0] );
  _checkError("glGetDoublei_vEXT");
}

FABRIC_EXT_EXPORT void glGetFloatIndexedvEXT_wrapper(
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetFloatIndexedvEXT( %d, 0x%04X, GLfloat* );\n", (int)target, (unsigned)index);
#endif
  glGetFloatIndexedvEXT( (GLenum)target, (GLuint)index, (GLfloat*)&params[0] );
  _checkError("glGetFloatIndexedvEXT");
}

FABRIC_EXT_EXPORT void glGetFloati_vEXT_wrapper(
  KL::Size pname,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetFloati_vEXT( %d, 0x%04X, GLfloat* );\n", (int)pname, (unsigned)index);
#endif
  glGetFloati_vEXT( (GLenum)pname, (GLuint)index, (GLfloat*)&params[0] );
  _checkError("glGetFloati_vEXT");
}

FABRIC_EXT_EXPORT void glGetFramebufferParameterivEXT_wrapper(
  KL::Size framebuffer,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetFramebufferParameterivEXT( 0x%04X, %d, GLint* );\n", (unsigned)framebuffer, (int)pname);
#endif
  glGetFramebufferParameterivEXT( (GLuint)framebuffer, (GLenum)pname, (GLint*)&param[0] );
  _checkError("glGetFramebufferParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetMultiTexEnvfvEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultiTexEnvfvEXT( %d, %d, %d, GLfloat* );\n", (int)texunit, (int)target, (int)pname);
#endif
  glGetMultiTexEnvfvEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetMultiTexEnvfvEXT");
}

FABRIC_EXT_EXPORT void glGetMultiTexEnvivEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultiTexEnvivEXT( %d, %d, %d, GLint* );\n", (int)texunit, (int)target, (int)pname);
#endif
  glGetMultiTexEnvivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetMultiTexEnvivEXT");
}

FABRIC_EXT_EXPORT void glGetMultiTexGendvEXT_wrapper(
  KL::Size texunit,
  KL::Size coord,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultiTexGendvEXT( %d, %d, %d, GLdouble* );\n", (int)texunit, (int)coord, (int)pname);
#endif
  glGetMultiTexGendvEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLdouble*)&params[0] );
  _checkError("glGetMultiTexGendvEXT");
}

FABRIC_EXT_EXPORT void glGetMultiTexGenfvEXT_wrapper(
  KL::Size texunit,
  KL::Size coord,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultiTexGenfvEXT( %d, %d, %d, GLfloat* );\n", (int)texunit, (int)coord, (int)pname);
#endif
  glGetMultiTexGenfvEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetMultiTexGenfvEXT");
}

FABRIC_EXT_EXPORT void glGetMultiTexGenivEXT_wrapper(
  KL::Size texunit,
  KL::Size coord,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultiTexGenivEXT( %d, %d, %d, GLint* );\n", (int)texunit, (int)coord, (int)pname);
#endif
  glGetMultiTexGenivEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetMultiTexGenivEXT");
}

FABRIC_EXT_EXPORT void glGetMultiTexImageEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultiTexImageEXT( %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)format, (int)type);
#endif
  glGetMultiTexImageEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)format, (GLenum)type, pixels );
  _checkError("glGetMultiTexImageEXT");
}

FABRIC_EXT_EXPORT void glGetMultiTexLevelParameterfvEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultiTexLevelParameterfvEXT( %d, %d, %d, %d, GLfloat* );\n", (int)texunit, (int)target, (int)level, (int)pname);
#endif
  glGetMultiTexLevelParameterfvEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetMultiTexLevelParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetMultiTexLevelParameterivEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultiTexLevelParameterivEXT( %d, %d, %d, %d, GLint* );\n", (int)texunit, (int)target, (int)level, (int)pname);
#endif
  glGetMultiTexLevelParameterivEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetMultiTexLevelParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetMultiTexParameterIivEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultiTexParameterIivEXT( %d, %d, %d, GLint* );\n", (int)texunit, (int)target, (int)pname);
#endif
  glGetMultiTexParameterIivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetMultiTexParameterIivEXT");
}

FABRIC_EXT_EXPORT void glGetMultiTexParameterIuivEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultiTexParameterIuivEXT( %d, %d, %d, GLuint* );\n", (int)texunit, (int)target, (int)pname);
#endif
  glGetMultiTexParameterIuivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLuint*)&params[0] );
  _checkError("glGetMultiTexParameterIuivEXT");
}

FABRIC_EXT_EXPORT void glGetMultiTexParameterfvEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultiTexParameterfvEXT( %d, %d, %d, GLfloat* );\n", (int)texunit, (int)target, (int)pname);
#endif
  glGetMultiTexParameterfvEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetMultiTexParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetMultiTexParameterivEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMultiTexParameterivEXT( %d, %d, %d, GLint* );\n", (int)texunit, (int)target, (int)pname);
#endif
  glGetMultiTexParameterivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetMultiTexParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetNamedBufferParameterivEXT_wrapper(
  KL::Size buffer,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetNamedBufferParameterivEXT( 0x%04X, %d, GLint* );\n", (unsigned)buffer, (int)pname);
#endif
  glGetNamedBufferParameterivEXT( (GLuint)buffer, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetNamedBufferParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetNamedBufferSubDataEXT_wrapper(
  KL::Size buffer,
  KL::Integer offset,
  KL::Size size,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetNamedBufferSubDataEXT( 0x%04X, %d, %d, void* );\n", (unsigned)buffer, (int)offset, (int)size);
#endif
  glGetNamedBufferSubDataEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size, data );
  _checkError("glGetNamedBufferSubDataEXT");
}

FABRIC_EXT_EXPORT void glGetNamedFramebufferAttachmentParameterivEXT_wrapper(
  KL::Size framebuffer,
  KL::Size attachment,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetNamedFramebufferAttachmentParameterivEXT( 0x%04X, %d, %d, GLint* );\n", (unsigned)framebuffer, (int)attachment, (int)pname);
#endif
  glGetNamedFramebufferAttachmentParameterivEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetNamedFramebufferAttachmentParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetNamedProgramLocalParameterIivEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetNamedProgramLocalParameterIivEXT( 0x%04X, %d, 0x%04X, GLint* );\n", (unsigned)program, (int)target, (unsigned)index);
#endif
  glGetNamedProgramLocalParameterIivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLint*)&params[0] );
  _checkError("glGetNamedProgramLocalParameterIivEXT");
}

FABRIC_EXT_EXPORT void glGetNamedProgramLocalParameterIuivEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetNamedProgramLocalParameterIuivEXT( 0x%04X, %d, 0x%04X, GLuint* );\n", (unsigned)program, (int)target, (unsigned)index);
#endif
  glGetNamedProgramLocalParameterIuivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLuint*)&params[0] );
  _checkError("glGetNamedProgramLocalParameterIuivEXT");
}

FABRIC_EXT_EXPORT void glGetNamedProgramLocalParameterdvEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetNamedProgramLocalParameterdvEXT( 0x%04X, %d, 0x%04X, GLdouble* );\n", (unsigned)program, (int)target, (unsigned)index);
#endif
  glGetNamedProgramLocalParameterdvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLdouble*)&params[0] );
  _checkError("glGetNamedProgramLocalParameterdvEXT");
}

FABRIC_EXT_EXPORT void glGetNamedProgramLocalParameterfvEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetNamedProgramLocalParameterfvEXT( 0x%04X, %d, 0x%04X, GLfloat* );\n", (unsigned)program, (int)target, (unsigned)index);
#endif
  glGetNamedProgramLocalParameterfvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLfloat*)&params[0] );
  _checkError("glGetNamedProgramLocalParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetNamedProgramStringEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size pname,
  KL::Data string
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetNamedProgramStringEXT( 0x%04X, %d, %d, void* );\n", (unsigned)program, (int)target, (int)pname);
#endif
  glGetNamedProgramStringEXT( (GLuint)program, (GLenum)target, (GLenum)pname, string );
  _checkError("glGetNamedProgramStringEXT");
}

FABRIC_EXT_EXPORT void glGetNamedProgramivEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetNamedProgramivEXT( 0x%04X, %d, %d, GLint* );\n", (unsigned)program, (int)target, (int)pname);
#endif
  glGetNamedProgramivEXT( (GLuint)program, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetNamedProgramivEXT");
}

FABRIC_EXT_EXPORT void glGetNamedRenderbufferParameterivEXT_wrapper(
  KL::Size renderbuffer,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetNamedRenderbufferParameterivEXT( 0x%04X, %d, GLint* );\n", (unsigned)renderbuffer, (int)pname);
#endif
  glGetNamedRenderbufferParameterivEXT( (GLuint)renderbuffer, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetNamedRenderbufferParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetTextureImageEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTextureImageEXT( 0x%04X, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)format, (int)type);
#endif
  glGetTextureImageEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)format, (GLenum)type, pixels );
  _checkError("glGetTextureImageEXT");
}

FABRIC_EXT_EXPORT void glGetTextureLevelParameterfvEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTextureLevelParameterfvEXT( 0x%04X, %d, %d, %d, GLfloat* );\n", (unsigned)texture, (int)target, (int)level, (int)pname);
#endif
  glGetTextureLevelParameterfvEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetTextureLevelParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetTextureLevelParameterivEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTextureLevelParameterivEXT( 0x%04X, %d, %d, %d, GLint* );\n", (unsigned)texture, (int)target, (int)level, (int)pname);
#endif
  glGetTextureLevelParameterivEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetTextureLevelParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetTextureParameterIivEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTextureParameterIivEXT( 0x%04X, %d, %d, GLint* );\n", (unsigned)texture, (int)target, (int)pname);
#endif
  glGetTextureParameterIivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetTextureParameterIivEXT");
}

FABRIC_EXT_EXPORT void glGetTextureParameterIuivEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTextureParameterIuivEXT( 0x%04X, %d, %d, GLuint* );\n", (unsigned)texture, (int)target, (int)pname);
#endif
  glGetTextureParameterIuivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLuint*)&params[0] );
  _checkError("glGetTextureParameterIuivEXT");
}

FABRIC_EXT_EXPORT void glGetTextureParameterfvEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTextureParameterfvEXT( 0x%04X, %d, %d, GLfloat* );\n", (unsigned)texture, (int)target, (int)pname);
#endif
  glGetTextureParameterfvEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetTextureParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetTextureParameterivEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTextureParameterivEXT( 0x%04X, %d, %d, GLint* );\n", (unsigned)texture, (int)target, (int)pname);
#endif
  glGetTextureParameterivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetTextureParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetVertexArrayIntegeri_vEXT_wrapper(
  KL::Size vaobj,
  KL::Size index,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexArrayIntegeri_vEXT( 0x%04X, 0x%04X, %d, GLint* );\n", (unsigned)vaobj, (unsigned)index, (int)pname);
#endif
  glGetVertexArrayIntegeri_vEXT( (GLuint)vaobj, (GLuint)index, (GLenum)pname, (GLint*)&param[0] );
  _checkError("glGetVertexArrayIntegeri_vEXT");
}

FABRIC_EXT_EXPORT void glGetVertexArrayIntegervEXT_wrapper(
  KL::Size vaobj,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexArrayIntegervEXT( 0x%04X, %d, GLint* );\n", (unsigned)vaobj, (int)pname);
#endif
  glGetVertexArrayIntegervEXT( (GLuint)vaobj, (GLenum)pname, (GLint*)&param[0] );
  _checkError("glGetVertexArrayIntegervEXT");
}

FABRIC_EXT_EXPORT GLvoid glMapNamedBufferEXT_wrapper(
  KL::Size buffer,
  KL::Size access
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMapNamedBufferEXT( 0x%04X, %d );\n", (unsigned)buffer, (int)access);
#endif
  glMapNamedBufferEXT( (GLuint)buffer, (GLenum)access );
  _checkError("glMapNamedBufferEXT");
}

FABRIC_EXT_EXPORT GLvoid glMapNamedBufferRangeEXT_wrapper(
  KL::Size buffer,
  KL::Integer offset,
  KL::Size length,
  KL::Size access
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMapNamedBufferRangeEXT( 0x%04X, %d, %d, 0x%04X );\n", (unsigned)buffer, (int)offset, (int)length, (unsigned)access);
#endif
  glMapNamedBufferRangeEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)length, (GLbitfield)access );
  _checkError("glMapNamedBufferRangeEXT");
}

FABRIC_EXT_EXPORT void glMatrixFrustumEXT_wrapper(
  KL::Size matrixMode,
  KL::Scalar l,
  KL::Scalar r,
  KL::Scalar b,
  KL::Scalar t,
  KL::Scalar n,
  KL::Scalar f
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixFrustumEXT( %d, %f, %f, %f, %f, %f, %f );\n", (int)matrixMode, (float)l, (float)r, (float)b, (float)t, (float)n, (float)f);
#endif
  glMatrixFrustumEXT( (GLenum)matrixMode, (GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t, (GLdouble)n, (GLdouble)f );
  _checkError("glMatrixFrustumEXT");
}

FABRIC_EXT_EXPORT void glMatrixLoadIdentityEXT_wrapper(
  KL::Size matrixMode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixLoadIdentityEXT( %d );\n", (int)matrixMode);
#endif
  glMatrixLoadIdentityEXT( (GLenum)matrixMode );
  _checkError("glMatrixLoadIdentityEXT");
}

FABRIC_EXT_EXPORT void glMatrixLoadTransposedEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixLoadTransposedEXT( %d, GLdouble* );\n", (int)matrixMode);
#endif
  glMatrixLoadTransposedEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
  _checkError("glMatrixLoadTransposedEXT");
}

FABRIC_EXT_EXPORT void glMatrixLoadTransposefEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixLoadTransposefEXT( %d, GLfloat* );\n", (int)matrixMode);
#endif
  glMatrixLoadTransposefEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
  _checkError("glMatrixLoadTransposefEXT");
}

FABRIC_EXT_EXPORT void glMatrixLoaddEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixLoaddEXT( %d, GLdouble* );\n", (int)matrixMode);
#endif
  glMatrixLoaddEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
  _checkError("glMatrixLoaddEXT");
}

FABRIC_EXT_EXPORT void glMatrixLoadfEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixLoadfEXT( %d, GLfloat* );\n", (int)matrixMode);
#endif
  glMatrixLoadfEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
  _checkError("glMatrixLoadfEXT");
}

FABRIC_EXT_EXPORT void glMatrixMultTransposedEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixMultTransposedEXT( %d, GLdouble* );\n", (int)matrixMode);
#endif
  glMatrixMultTransposedEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
  _checkError("glMatrixMultTransposedEXT");
}

FABRIC_EXT_EXPORT void glMatrixMultTransposefEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixMultTransposefEXT( %d, GLfloat* );\n", (int)matrixMode);
#endif
  glMatrixMultTransposefEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
  _checkError("glMatrixMultTransposefEXT");
}

FABRIC_EXT_EXPORT void glMatrixMultdEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixMultdEXT( %d, GLdouble* );\n", (int)matrixMode);
#endif
  glMatrixMultdEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
  _checkError("glMatrixMultdEXT");
}

FABRIC_EXT_EXPORT void glMatrixMultfEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixMultfEXT( %d, GLfloat* );\n", (int)matrixMode);
#endif
  glMatrixMultfEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
  _checkError("glMatrixMultfEXT");
}

FABRIC_EXT_EXPORT void glMatrixOrthoEXT_wrapper(
  KL::Size matrixMode,
  KL::Scalar l,
  KL::Scalar r,
  KL::Scalar b,
  KL::Scalar t,
  KL::Scalar n,
  KL::Scalar f
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixOrthoEXT( %d, %f, %f, %f, %f, %f, %f );\n", (int)matrixMode, (float)l, (float)r, (float)b, (float)t, (float)n, (float)f);
#endif
  glMatrixOrthoEXT( (GLenum)matrixMode, (GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t, (GLdouble)n, (GLdouble)f );
  _checkError("glMatrixOrthoEXT");
}

FABRIC_EXT_EXPORT void glMatrixPopEXT_wrapper(
  KL::Size matrixMode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixPopEXT( %d );\n", (int)matrixMode);
#endif
  glMatrixPopEXT( (GLenum)matrixMode );
  _checkError("glMatrixPopEXT");
}

FABRIC_EXT_EXPORT void glMatrixPushEXT_wrapper(
  KL::Size matrixMode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixPushEXT( %d );\n", (int)matrixMode);
#endif
  glMatrixPushEXT( (GLenum)matrixMode );
  _checkError("glMatrixPushEXT");
}

FABRIC_EXT_EXPORT void glMatrixRotatedEXT_wrapper(
  KL::Size matrixMode,
  KL::Scalar angle,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixRotatedEXT( %d, %f, %f, %f, %f );\n", (int)matrixMode, (float)angle, (float)x, (float)y, (float)z);
#endif
  glMatrixRotatedEXT( (GLenum)matrixMode, (GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glMatrixRotatedEXT");
}

FABRIC_EXT_EXPORT void glMatrixRotatefEXT_wrapper(
  KL::Size matrixMode,
  KL::Scalar angle,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixRotatefEXT( %d, %f, %f, %f, %f );\n", (int)matrixMode, (float)angle, (float)x, (float)y, (float)z);
#endif
  glMatrixRotatefEXT( (GLenum)matrixMode, (GLfloat)angle, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glMatrixRotatefEXT");
}

FABRIC_EXT_EXPORT void glMatrixScaledEXT_wrapper(
  KL::Size matrixMode,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixScaledEXT( %d, %f, %f, %f );\n", (int)matrixMode, (float)x, (float)y, (float)z);
#endif
  glMatrixScaledEXT( (GLenum)matrixMode, (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glMatrixScaledEXT");
}

FABRIC_EXT_EXPORT void glMatrixScalefEXT_wrapper(
  KL::Size matrixMode,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixScalefEXT( %d, %f, %f, %f );\n", (int)matrixMode, (float)x, (float)y, (float)z);
#endif
  glMatrixScalefEXT( (GLenum)matrixMode, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glMatrixScalefEXT");
}

FABRIC_EXT_EXPORT void glMatrixTranslatedEXT_wrapper(
  KL::Size matrixMode,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixTranslatedEXT( %d, %f, %f, %f );\n", (int)matrixMode, (float)x, (float)y, (float)z);
#endif
  glMatrixTranslatedEXT( (GLenum)matrixMode, (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glMatrixTranslatedEXT");
}

FABRIC_EXT_EXPORT void glMatrixTranslatefEXT_wrapper(
  KL::Size matrixMode,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMatrixTranslatefEXT( %d, %f, %f, %f );\n", (int)matrixMode, (float)x, (float)y, (float)z);
#endif
  glMatrixTranslatefEXT( (GLenum)matrixMode, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glMatrixTranslatefEXT");
}

FABRIC_EXT_EXPORT void glMultiTexBufferEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size internalformat,
  KL::Size buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexBufferEXT( %d, %d, %d, 0x%04X );\n", (int)texunit, (int)target, (int)internalformat, (unsigned)buffer);
#endif
  glMultiTexBufferEXT( (GLenum)texunit, (GLenum)target, (GLenum)internalformat, (GLuint)buffer );
  _checkError("glMultiTexBufferEXT");
}

FABRIC_EXT_EXPORT void glMultiTexCoordPointerEXT_wrapper(
  KL::Size texunit,
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexCoordPointerEXT( %d, %d, %d, %d, void* );\n", (int)texunit, (int)size, (int)type, (int)stride);
#endif
  glMultiTexCoordPointerEXT( (GLenum)texunit, (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glMultiTexCoordPointerEXT");
}

FABRIC_EXT_EXPORT void glMultiTexEnvfEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexEnvfEXT( %d, %d, %d, %f );\n", (int)texunit, (int)target, (int)pname, (float)param);
#endif
  glMultiTexEnvfEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLfloat)param );
  _checkError("glMultiTexEnvfEXT");
}

FABRIC_EXT_EXPORT void glMultiTexEnvfvEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexEnvfvEXT( %d, %d, %d, GLfloat* );\n", (int)texunit, (int)target, (int)pname);
#endif
  glMultiTexEnvfvEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glMultiTexEnvfvEXT");
}

FABRIC_EXT_EXPORT void glMultiTexEnviEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexEnviEXT( %d, %d, %d, %d );\n", (int)texunit, (int)target, (int)pname, (int)param);
#endif
  glMultiTexEnviEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint)param );
  _checkError("glMultiTexEnviEXT");
}

FABRIC_EXT_EXPORT void glMultiTexEnvivEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexEnvivEXT( %d, %d, %d, GLint* );\n", (int)texunit, (int)target, (int)pname);
#endif
  glMultiTexEnvivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glMultiTexEnvivEXT");
}

FABRIC_EXT_EXPORT void glMultiTexGendEXT_wrapper(
  KL::Size texunit,
  KL::Size coord,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexGendEXT( %d, %d, %d, %f );\n", (int)texunit, (int)coord, (int)pname, (float)param);
#endif
  glMultiTexGendEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLdouble)param );
  _checkError("glMultiTexGendEXT");
}

FABRIC_EXT_EXPORT void glMultiTexGendvEXT_wrapper(
  KL::Size texunit,
  KL::Size coord,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexGendvEXT( %d, %d, %d, GLdouble* );\n", (int)texunit, (int)coord, (int)pname);
#endif
  glMultiTexGendvEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (const GLdouble*)&params[0] );
  _checkError("glMultiTexGendvEXT");
}

FABRIC_EXT_EXPORT void glMultiTexGenfEXT_wrapper(
  KL::Size texunit,
  KL::Size coord,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexGenfEXT( %d, %d, %d, %f );\n", (int)texunit, (int)coord, (int)pname, (float)param);
#endif
  glMultiTexGenfEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLfloat)param );
  _checkError("glMultiTexGenfEXT");
}

FABRIC_EXT_EXPORT void glMultiTexGenfvEXT_wrapper(
  KL::Size texunit,
  KL::Size coord,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexGenfvEXT( %d, %d, %d, GLfloat* );\n", (int)texunit, (int)coord, (int)pname);
#endif
  glMultiTexGenfvEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glMultiTexGenfvEXT");
}

FABRIC_EXT_EXPORT void glMultiTexGeniEXT_wrapper(
  KL::Size texunit,
  KL::Size coord,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexGeniEXT( %d, %d, %d, %d );\n", (int)texunit, (int)coord, (int)pname, (int)param);
#endif
  glMultiTexGeniEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLint)param );
  _checkError("glMultiTexGeniEXT");
}

FABRIC_EXT_EXPORT void glMultiTexGenivEXT_wrapper(
  KL::Size texunit,
  KL::Size coord,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexGenivEXT( %d, %d, %d, GLint* );\n", (int)texunit, (int)coord, (int)pname);
#endif
  glMultiTexGenivEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glMultiTexGenivEXT");
}

FABRIC_EXT_EXPORT void glMultiTexImage1DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Integer internalformat,
  KL::Size width,
  KL::Integer border,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexImage1DEXT( %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)format, (int)type);
#endif
  glMultiTexImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  _checkError("glMultiTexImage1DEXT");
}

FABRIC_EXT_EXPORT void glMultiTexImage2DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Integer internalformat,
  KL::Size width,
  KL::Size height,
  KL::Integer border,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)format, (int)type);
#endif
  glMultiTexImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  _checkError("glMultiTexImage2DEXT");
}

FABRIC_EXT_EXPORT void glMultiTexImage3DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Integer internalformat,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Integer border,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)format, (int)type);
#endif
  glMultiTexImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  _checkError("glMultiTexImage3DEXT");
}

FABRIC_EXT_EXPORT void glMultiTexParameterIivEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexParameterIivEXT( %d, %d, %d, GLint* );\n", (int)texunit, (int)target, (int)pname);
#endif
  glMultiTexParameterIivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glMultiTexParameterIivEXT");
}

FABRIC_EXT_EXPORT void glMultiTexParameterIuivEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexParameterIuivEXT( %d, %d, %d, GLuint* );\n", (int)texunit, (int)target, (int)pname);
#endif
  glMultiTexParameterIuivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLuint*)&params[0] );
  _checkError("glMultiTexParameterIuivEXT");
}

FABRIC_EXT_EXPORT void glMultiTexParameterfEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexParameterfEXT( %d, %d, %d, %f );\n", (int)texunit, (int)target, (int)pname, (float)param);
#endif
  glMultiTexParameterfEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLfloat)param );
  _checkError("glMultiTexParameterfEXT");
}

FABRIC_EXT_EXPORT void glMultiTexParameterfvEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexParameterfvEXT( %d, %d, %d, GLfloat* );\n", (int)texunit, (int)target, (int)pname);
#endif
  glMultiTexParameterfvEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLfloat*)&param[0] );
  _checkError("glMultiTexParameterfvEXT");
}

FABRIC_EXT_EXPORT void glMultiTexParameteriEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexParameteriEXT( %d, %d, %d, %d );\n", (int)texunit, (int)target, (int)pname, (int)param);
#endif
  glMultiTexParameteriEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint)param );
  _checkError("glMultiTexParameteriEXT");
}

FABRIC_EXT_EXPORT void glMultiTexParameterivEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexParameterivEXT( %d, %d, %d, GLint* );\n", (int)texunit, (int)target, (int)pname);
#endif
  glMultiTexParameterivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLint*)&param[0] );
  _checkError("glMultiTexParameterivEXT");
}

FABRIC_EXT_EXPORT void glMultiTexRenderbufferEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size renderbuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexRenderbufferEXT( %d, %d, 0x%04X );\n", (int)texunit, (int)target, (unsigned)renderbuffer);
#endif
  glMultiTexRenderbufferEXT( (GLenum)texunit, (GLenum)target, (GLuint)renderbuffer );
  _checkError("glMultiTexRenderbufferEXT");
}

FABRIC_EXT_EXPORT void glMultiTexSubImage1DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Size width,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexSubImage1DEXT( %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)type);
#endif
  glMultiTexSubImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, pixels );
  _checkError("glMultiTexSubImage1DEXT");
}

FABRIC_EXT_EXPORT void glMultiTexSubImage2DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexSubImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)type);
#endif
  glMultiTexSubImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  _checkError("glMultiTexSubImage2DEXT");
}

FABRIC_EXT_EXPORT void glMultiTexSubImage3DEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer zoffset,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiTexSubImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)texunit, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)type);
#endif
  glMultiTexSubImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
  _checkError("glMultiTexSubImage3DEXT");
}

FABRIC_EXT_EXPORT void glNamedBufferDataEXT_wrapper(
  KL::Size buffer,
  KL::Size size,
  KL::Data data,
  KL::Size usage
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedBufferDataEXT( 0x%04X, %d, void*, %d );\n", (unsigned)buffer, (int)size, (int)usage);
#endif
  glNamedBufferDataEXT( (GLuint)buffer, (GLsizeiptr)size, data, (GLenum)usage );
  _checkError("glNamedBufferDataEXT");
}

FABRIC_EXT_EXPORT void glNamedBufferSubDataEXT_wrapper(
  KL::Size buffer,
  KL::Integer offset,
  KL::Size size,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedBufferSubDataEXT( 0x%04X, %d, %d, void* );\n", (unsigned)buffer, (int)offset, (int)size);
#endif
  glNamedBufferSubDataEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size, data );
  _checkError("glNamedBufferSubDataEXT");
}

FABRIC_EXT_EXPORT void glNamedCopyBufferSubDataEXT_wrapper(
  KL::Size readBuffer,
  KL::Size writeBuffer,
  KL::Integer readOffset,
  KL::Integer writeOffset,
  KL::Size size
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedCopyBufferSubDataEXT( 0x%04X, 0x%04X, %d, %d, %d );\n", (unsigned)readBuffer, (unsigned)writeBuffer, (int)readOffset, (int)writeOffset, (int)size);
#endif
  glNamedCopyBufferSubDataEXT( (GLuint)readBuffer, (GLuint)writeBuffer, (GLintptr)readOffset, (GLintptr)writeOffset, (GLsizeiptr)size );
  _checkError("glNamedCopyBufferSubDataEXT");
}

FABRIC_EXT_EXPORT void glNamedFramebufferRenderbufferEXT_wrapper(
  KL::Size framebuffer,
  KL::Size attachment,
  KL::Size renderbuffertarget,
  KL::Size renderbuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedFramebufferRenderbufferEXT( 0x%04X, %d, %d, 0x%04X );\n", (unsigned)framebuffer, (int)attachment, (int)renderbuffertarget, (unsigned)renderbuffer);
#endif
  glNamedFramebufferRenderbufferEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)renderbuffertarget, (GLuint)renderbuffer );
  _checkError("glNamedFramebufferRenderbufferEXT");
}

FABRIC_EXT_EXPORT void glNamedFramebufferTexture1DEXT_wrapper(
  KL::Size framebuffer,
  KL::Size attachment,
  KL::Size textarget,
  KL::Size texture,
  KL::Integer level
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedFramebufferTexture1DEXT( 0x%04X, %d, %d, 0x%04X, %d );\n", (unsigned)framebuffer, (int)attachment, (int)textarget, (unsigned)texture, (int)level);
#endif
  glNamedFramebufferTexture1DEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
  _checkError("glNamedFramebufferTexture1DEXT");
}

FABRIC_EXT_EXPORT void glNamedFramebufferTexture2DEXT_wrapper(
  KL::Size framebuffer,
  KL::Size attachment,
  KL::Size textarget,
  KL::Size texture,
  KL::Integer level
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedFramebufferTexture2DEXT( 0x%04X, %d, %d, 0x%04X, %d );\n", (unsigned)framebuffer, (int)attachment, (int)textarget, (unsigned)texture, (int)level);
#endif
  glNamedFramebufferTexture2DEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
  _checkError("glNamedFramebufferTexture2DEXT");
}

FABRIC_EXT_EXPORT void glNamedFramebufferTexture3DEXT_wrapper(
  KL::Size framebuffer,
  KL::Size attachment,
  KL::Size textarget,
  KL::Size texture,
  KL::Integer level,
  KL::Integer zoffset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedFramebufferTexture3DEXT( 0x%04X, %d, %d, 0x%04X, %d, %d );\n", (unsigned)framebuffer, (int)attachment, (int)textarget, (unsigned)texture, (int)level, (int)zoffset);
#endif
  glNamedFramebufferTexture3DEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level, (GLint)zoffset );
  _checkError("glNamedFramebufferTexture3DEXT");
}

FABRIC_EXT_EXPORT void glNamedFramebufferTextureEXT_wrapper(
  KL::Size framebuffer,
  KL::Size attachment,
  KL::Size texture,
  KL::Integer level
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedFramebufferTextureEXT( 0x%04X, %d, 0x%04X, %d );\n", (unsigned)framebuffer, (int)attachment, (unsigned)texture, (int)level);
#endif
  glNamedFramebufferTextureEXT( (GLuint)framebuffer, (GLenum)attachment, (GLuint)texture, (GLint)level );
  _checkError("glNamedFramebufferTextureEXT");
}

FABRIC_EXT_EXPORT void glNamedFramebufferTextureFaceEXT_wrapper(
  KL::Size framebuffer,
  KL::Size attachment,
  KL::Size texture,
  KL::Integer level,
  KL::Size face
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedFramebufferTextureFaceEXT( 0x%04X, %d, 0x%04X, %d, %d );\n", (unsigned)framebuffer, (int)attachment, (unsigned)texture, (int)level, (int)face);
#endif
  glNamedFramebufferTextureFaceEXT( (GLuint)framebuffer, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLenum)face );
  _checkError("glNamedFramebufferTextureFaceEXT");
}

FABRIC_EXT_EXPORT void glNamedFramebufferTextureLayerEXT_wrapper(
  KL::Size framebuffer,
  KL::Size attachment,
  KL::Size texture,
  KL::Integer level,
  KL::Integer layer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedFramebufferTextureLayerEXT( 0x%04X, %d, 0x%04X, %d, %d );\n", (unsigned)framebuffer, (int)attachment, (unsigned)texture, (int)level, (int)layer);
#endif
  glNamedFramebufferTextureLayerEXT( (GLuint)framebuffer, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer );
  _checkError("glNamedFramebufferTextureLayerEXT");
}

FABRIC_EXT_EXPORT void glNamedProgramLocalParameter4dEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedProgramLocalParameter4dEXT( 0x%04X, %d, 0x%04X, %f, %f, %f, %f );\n", (unsigned)program, (int)target, (unsigned)index, (float)x, (float)y, (float)z, (float)w);
#endif
  glNamedProgramLocalParameter4dEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  _checkError("glNamedProgramLocalParameter4dEXT");
}

FABRIC_EXT_EXPORT void glNamedProgramLocalParameter4dvEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedProgramLocalParameter4dvEXT( 0x%04X, %d, 0x%04X, GLdouble* );\n", (unsigned)program, (int)target, (unsigned)index);
#endif
  glNamedProgramLocalParameter4dvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (const GLdouble*)&params[0] );
  _checkError("glNamedProgramLocalParameter4dvEXT");
}

FABRIC_EXT_EXPORT void glNamedProgramLocalParameter4fEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z,
  KL::Scalar w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedProgramLocalParameter4fEXT( 0x%04X, %d, 0x%04X, %f, %f, %f, %f );\n", (unsigned)program, (int)target, (unsigned)index, (float)x, (float)y, (float)z, (float)w);
#endif
  glNamedProgramLocalParameter4fEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  _checkError("glNamedProgramLocalParameter4fEXT");
}

FABRIC_EXT_EXPORT void glNamedProgramLocalParameter4fvEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedProgramLocalParameter4fvEXT( 0x%04X, %d, 0x%04X, GLfloat* );\n", (unsigned)program, (int)target, (unsigned)index);
#endif
  glNamedProgramLocalParameter4fvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (const GLfloat*)&params[0] );
  _checkError("glNamedProgramLocalParameter4fvEXT");
}

FABRIC_EXT_EXPORT void glNamedProgramLocalParameterI4iEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  KL::Integer x,
  KL::Integer y,
  KL::Integer z,
  KL::Integer w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedProgramLocalParameterI4iEXT( 0x%04X, %d, 0x%04X, %d, %d, %d, %d );\n", (unsigned)program, (int)target, (unsigned)index, (int)x, (int)y, (int)z, (int)w);
#endif
  glNamedProgramLocalParameterI4iEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLint)x, (GLint)y, (GLint)z, (GLint)w );
  _checkError("glNamedProgramLocalParameterI4iEXT");
}

FABRIC_EXT_EXPORT void glNamedProgramLocalParameterI4ivEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedProgramLocalParameterI4ivEXT( 0x%04X, %d, 0x%04X, GLint* );\n", (unsigned)program, (int)target, (unsigned)index);
#endif
  glNamedProgramLocalParameterI4ivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (const GLint*)&params[0] );
  _checkError("glNamedProgramLocalParameterI4ivEXT");
}

FABRIC_EXT_EXPORT void glNamedProgramLocalParameterI4uiEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  KL::Size x,
  KL::Size y,
  KL::Size z,
  KL::Size w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedProgramLocalParameterI4uiEXT( 0x%04X, %d, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)program, (int)target, (unsigned)index, (unsigned)x, (unsigned)y, (unsigned)z, (unsigned)w);
#endif
  glNamedProgramLocalParameterI4uiEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z, (GLuint)w );
  _checkError("glNamedProgramLocalParameterI4uiEXT");
}

FABRIC_EXT_EXPORT void glNamedProgramLocalParameterI4uivEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  const KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedProgramLocalParameterI4uivEXT( 0x%04X, %d, 0x%04X, GLuint* );\n", (unsigned)program, (int)target, (unsigned)index);
#endif
  glNamedProgramLocalParameterI4uivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (const GLuint*)&params[0] );
  _checkError("glNamedProgramLocalParameterI4uivEXT");
}

FABRIC_EXT_EXPORT void glNamedProgramLocalParameters4fvEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedProgramLocalParameters4fvEXT( 0x%04X, %d, 0x%04X, %d, GLfloat* );\n", (unsigned)program, (int)target, (unsigned)index, (int)count);
#endif
  glNamedProgramLocalParameters4fvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLsizei)count, (const GLfloat*)&params[0] );
  _checkError("glNamedProgramLocalParameters4fvEXT");
}

FABRIC_EXT_EXPORT void glNamedProgramLocalParametersI4ivEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedProgramLocalParametersI4ivEXT( 0x%04X, %d, 0x%04X, %d, GLint* );\n", (unsigned)program, (int)target, (unsigned)index, (int)count);
#endif
  glNamedProgramLocalParametersI4ivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLsizei)count, (const GLint*)&params[0] );
  _checkError("glNamedProgramLocalParametersI4ivEXT");
}

FABRIC_EXT_EXPORT void glNamedProgramLocalParametersI4uivEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size index,
  KL::Size count,
  const KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedProgramLocalParametersI4uivEXT( 0x%04X, %d, 0x%04X, %d, GLuint* );\n", (unsigned)program, (int)target, (unsigned)index, (int)count);
#endif
  glNamedProgramLocalParametersI4uivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLsizei)count, (const GLuint*)&params[0] );
  _checkError("glNamedProgramLocalParametersI4uivEXT");
}

FABRIC_EXT_EXPORT void glNamedProgramStringEXT_wrapper(
  KL::Size program,
  KL::Size target,
  KL::Size format,
  KL::Size len,
  KL::Data string
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedProgramStringEXT( 0x%04X, %d, %d, %d, void* );\n", (unsigned)program, (int)target, (int)format, (int)len);
#endif
  glNamedProgramStringEXT( (GLuint)program, (GLenum)target, (GLenum)format, (GLsizei)len, string );
  _checkError("glNamedProgramStringEXT");
}

FABRIC_EXT_EXPORT void glNamedRenderbufferStorageEXT_wrapper(
  KL::Size renderbuffer,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedRenderbufferStorageEXT( 0x%04X, %d, %d, %d );\n", (unsigned)renderbuffer, (int)internalformat, (int)width, (int)height);
#endif
  glNamedRenderbufferStorageEXT( (GLuint)renderbuffer, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  _checkError("glNamedRenderbufferStorageEXT");
}

FABRIC_EXT_EXPORT void glNamedRenderbufferStorageMultisampleCoverageEXT_wrapper(
  KL::Size renderbuffer,
  KL::Size coverageSamples,
  KL::Size colorSamples,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedRenderbufferStorageMultisampleCoverageEXT( 0x%04X, %d, %d, %d, %d, %d );\n", (unsigned)renderbuffer, (int)coverageSamples, (int)colorSamples, (int)internalformat, (int)width, (int)height);
#endif
  glNamedRenderbufferStorageMultisampleCoverageEXT( (GLuint)renderbuffer, (GLsizei)coverageSamples, (GLsizei)colorSamples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  _checkError("glNamedRenderbufferStorageMultisampleCoverageEXT");
}

FABRIC_EXT_EXPORT void glNamedRenderbufferStorageMultisampleEXT_wrapper(
  KL::Size renderbuffer,
  KL::Size samples,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNamedRenderbufferStorageMultisampleEXT( 0x%04X, %d, %d, %d, %d );\n", (unsigned)renderbuffer, (int)samples, (int)internalformat, (int)width, (int)height);
#endif
  glNamedRenderbufferStorageMultisampleEXT( (GLuint)renderbuffer, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  _checkError("glNamedRenderbufferStorageMultisampleEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform1fEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Scalar v0
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform1fEXT( 0x%04X, %d, %f );\n", (unsigned)program, (int)location, (float)v0);
#endif
  glProgramUniform1fEXT( (GLuint)program, (GLint)location, (GLfloat)v0 );
  _checkError("glProgramUniform1fEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform1fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform1fvEXT( 0x%04X, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count);
#endif
  glProgramUniform1fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glProgramUniform1fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform1iEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Integer v0
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform1iEXT( 0x%04X, %d, %d );\n", (unsigned)program, (int)location, (int)v0);
#endif
  glProgramUniform1iEXT( (GLuint)program, (GLint)location, (GLint)v0 );
  _checkError("glProgramUniform1iEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform1ivEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform1ivEXT( 0x%04X, %d, %d, GLint* );\n", (unsigned)program, (int)location, (int)count);
#endif
  glProgramUniform1ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glProgramUniform1ivEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform1uiEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size v0
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform1uiEXT( 0x%04X, %d, 0x%04X );\n", (unsigned)program, (int)location, (unsigned)v0);
#endif
  glProgramUniform1uiEXT( (GLuint)program, (GLint)location, (GLuint)v0 );
  _checkError("glProgramUniform1uiEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform1uivEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Size> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform1uivEXT( 0x%04X, %d, %d, GLuint* );\n", (unsigned)program, (int)location, (int)count);
#endif
  glProgramUniform1uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  _checkError("glProgramUniform1uivEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform2fEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Scalar v0,
  KL::Scalar v1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform2fEXT( 0x%04X, %d, %f, %f );\n", (unsigned)program, (int)location, (float)v0, (float)v1);
#endif
  glProgramUniform2fEXT( (GLuint)program, (GLint)location, (GLfloat)v0, (GLfloat)v1 );
  _checkError("glProgramUniform2fEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform2fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform2fvEXT( 0x%04X, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count);
#endif
  glProgramUniform2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glProgramUniform2fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform2iEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Integer v0,
  KL::Integer v1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform2iEXT( 0x%04X, %d, %d, %d );\n", (unsigned)program, (int)location, (int)v0, (int)v1);
#endif
  glProgramUniform2iEXT( (GLuint)program, (GLint)location, (GLint)v0, (GLint)v1 );
  _checkError("glProgramUniform2iEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform2ivEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform2ivEXT( 0x%04X, %d, %d, GLint* );\n", (unsigned)program, (int)location, (int)count);
#endif
  glProgramUniform2ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glProgramUniform2ivEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform2uiEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size v0,
  KL::Size v1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform2uiEXT( 0x%04X, %d, 0x%04X, 0x%04X );\n", (unsigned)program, (int)location, (unsigned)v0, (unsigned)v1);
#endif
  glProgramUniform2uiEXT( (GLuint)program, (GLint)location, (GLuint)v0, (GLuint)v1 );
  _checkError("glProgramUniform2uiEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform2uivEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Size> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform2uivEXT( 0x%04X, %d, %d, GLuint* );\n", (unsigned)program, (int)location, (int)count);
#endif
  glProgramUniform2uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  _checkError("glProgramUniform2uivEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform3fEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Scalar v0,
  KL::Scalar v1,
  KL::Scalar v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform3fEXT( 0x%04X, %d, %f, %f, %f );\n", (unsigned)program, (int)location, (float)v0, (float)v1, (float)v2);
#endif
  glProgramUniform3fEXT( (GLuint)program, (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2 );
  _checkError("glProgramUniform3fEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform3fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform3fvEXT( 0x%04X, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count);
#endif
  glProgramUniform3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glProgramUniform3fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform3iEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Integer v0,
  KL::Integer v1,
  KL::Integer v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform3iEXT( 0x%04X, %d, %d, %d, %d );\n", (unsigned)program, (int)location, (int)v0, (int)v1, (int)v2);
#endif
  glProgramUniform3iEXT( (GLuint)program, (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2 );
  _checkError("glProgramUniform3iEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform3ivEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform3ivEXT( 0x%04X, %d, %d, GLint* );\n", (unsigned)program, (int)location, (int)count);
#endif
  glProgramUniform3ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glProgramUniform3ivEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform3uiEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size v0,
  KL::Size v1,
  KL::Size v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform3uiEXT( 0x%04X, %d, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)program, (int)location, (unsigned)v0, (unsigned)v1, (unsigned)v2);
#endif
  glProgramUniform3uiEXT( (GLuint)program, (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2 );
  _checkError("glProgramUniform3uiEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform3uivEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Size> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform3uivEXT( 0x%04X, %d, %d, GLuint* );\n", (unsigned)program, (int)location, (int)count);
#endif
  glProgramUniform3uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  _checkError("glProgramUniform3uivEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform4fEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Scalar v0,
  KL::Scalar v1,
  KL::Scalar v2,
  KL::Scalar v3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform4fEXT( 0x%04X, %d, %f, %f, %f, %f );\n", (unsigned)program, (int)location, (float)v0, (float)v1, (float)v2, (float)v3);
#endif
  glProgramUniform4fEXT( (GLuint)program, (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2, (GLfloat)v3 );
  _checkError("glProgramUniform4fEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform4fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform4fvEXT( 0x%04X, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count);
#endif
  glProgramUniform4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glProgramUniform4fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform4iEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Integer v0,
  KL::Integer v1,
  KL::Integer v2,
  KL::Integer v3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform4iEXT( 0x%04X, %d, %d, %d, %d, %d );\n", (unsigned)program, (int)location, (int)v0, (int)v1, (int)v2, (int)v3);
#endif
  glProgramUniform4iEXT( (GLuint)program, (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2, (GLint)v3 );
  _checkError("glProgramUniform4iEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform4ivEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform4ivEXT( 0x%04X, %d, %d, GLint* );\n", (unsigned)program, (int)location, (int)count);
#endif
  glProgramUniform4ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glProgramUniform4ivEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform4uiEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size v0,
  KL::Size v1,
  KL::Size v2,
  KL::Size v3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform4uiEXT( 0x%04X, %d, 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)program, (int)location, (unsigned)v0, (unsigned)v1, (unsigned)v2, (unsigned)v3);
#endif
  glProgramUniform4uiEXT( (GLuint)program, (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2, (GLuint)v3 );
  _checkError("glProgramUniform4uiEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform4uivEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Size> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniform4uivEXT( 0x%04X, %d, %d, GLuint* );\n", (unsigned)program, (int)location, (int)count);
#endif
  glProgramUniform4uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  _checkError("glProgramUniform4uivEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix2fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat22> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniformMatrix2fvEXT( 0x%04X, %d, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count, (int)transpose);
#endif
  glProgramUniformMatrix2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix2fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix2x3fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat33> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniformMatrix2x3fvEXT( 0x%04X, %d, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count, (int)transpose);
#endif
  glProgramUniformMatrix2x3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix2x3fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix2x4fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat44> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniformMatrix2x4fvEXT( 0x%04X, %d, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count, (int)transpose);
#endif
  glProgramUniformMatrix2x4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix2x4fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix3fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat33> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniformMatrix3fvEXT( 0x%04X, %d, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count, (int)transpose);
#endif
  glProgramUniformMatrix3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix3fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix3x2fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat22> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniformMatrix3x2fvEXT( 0x%04X, %d, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count, (int)transpose);
#endif
  glProgramUniformMatrix3x2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix3x2fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix3x4fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat44> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniformMatrix3x4fvEXT( 0x%04X, %d, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count, (int)transpose);
#endif
  glProgramUniformMatrix3x4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix3x4fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix4fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat44> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniformMatrix4fvEXT( 0x%04X, %d, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count, (int)transpose);
#endif
  glProgramUniformMatrix4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix4fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix4x2fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat22> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniformMatrix4x2fvEXT( 0x%04X, %d, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count, (int)transpose);
#endif
  glProgramUniformMatrix4x2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix4x2fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix4x3fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  KL::VariableArray<KL::Mat33> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramUniformMatrix4x3fvEXT( 0x%04X, %d, %d, %d, GLfloat* );\n", (unsigned)program, (int)location, (int)count, (int)transpose);
#endif
  glProgramUniformMatrix4x3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix4x3fvEXT");
}

FABRIC_EXT_EXPORT void glPushClientAttribDefaultEXT_wrapper(
  KL::Size mask
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPushClientAttribDefaultEXT( 0x%04X );\n", (unsigned)mask);
#endif
  glPushClientAttribDefaultEXT( (GLbitfield)mask );
  _checkError("glPushClientAttribDefaultEXT");
}

FABRIC_EXT_EXPORT void glTextureBufferEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size internalformat,
  KL::Size buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureBufferEXT( 0x%04X, %d, %d, 0x%04X );\n", (unsigned)texture, (int)target, (int)internalformat, (unsigned)buffer);
#endif
  glTextureBufferEXT( (GLuint)texture, (GLenum)target, (GLenum)internalformat, (GLuint)buffer );
  _checkError("glTextureBufferEXT");
}

FABRIC_EXT_EXPORT void glTextureImage1DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Integer internalformat,
  KL::Size width,
  KL::Integer border,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureImage1DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)format, (int)type);
#endif
  glTextureImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTextureImage1DEXT");
}

FABRIC_EXT_EXPORT void glTextureImage2DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Integer internalformat,
  KL::Size width,
  KL::Size height,
  KL::Integer border,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureImage2DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)format, (int)type);
#endif
  glTextureImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTextureImage2DEXT");
}

FABRIC_EXT_EXPORT void glTextureImage3DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Integer internalformat,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Integer border,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureImage3DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)format, (int)type);
#endif
  glTextureImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTextureImage3DEXT");
}

FABRIC_EXT_EXPORT void glTextureParameterIivEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureParameterIivEXT( 0x%04X, %d, %d, GLint* );\n", (unsigned)texture, (int)target, (int)pname);
#endif
  glTextureParameterIivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glTextureParameterIivEXT");
}

FABRIC_EXT_EXPORT void glTextureParameterIuivEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureParameterIuivEXT( 0x%04X, %d, %d, GLuint* );\n", (unsigned)texture, (int)target, (int)pname);
#endif
  glTextureParameterIuivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLuint*)&params[0] );
  _checkError("glTextureParameterIuivEXT");
}

FABRIC_EXT_EXPORT void glTextureParameterfEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureParameterfEXT( 0x%04X, %d, %d, %f );\n", (unsigned)texture, (int)target, (int)pname, (float)param);
#endif
  glTextureParameterfEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLfloat)param );
  _checkError("glTextureParameterfEXT");
}

FABRIC_EXT_EXPORT void glTextureParameterfvEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureParameterfvEXT( 0x%04X, %d, %d, GLfloat* );\n", (unsigned)texture, (int)target, (int)pname);
#endif
  glTextureParameterfvEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLfloat*)&param[0] );
  _checkError("glTextureParameterfvEXT");
}

FABRIC_EXT_EXPORT void glTextureParameteriEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureParameteriEXT( 0x%04X, %d, %d, %d );\n", (unsigned)texture, (int)target, (int)pname, (int)param);
#endif
  glTextureParameteriEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLint)param );
  _checkError("glTextureParameteriEXT");
}

FABRIC_EXT_EXPORT void glTextureParameterivEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureParameterivEXT( 0x%04X, %d, %d, GLint* );\n", (unsigned)texture, (int)target, (int)pname);
#endif
  glTextureParameterivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLint*)&param[0] );
  _checkError("glTextureParameterivEXT");
}

FABRIC_EXT_EXPORT void glTextureRenderbufferEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size renderbuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureRenderbufferEXT( 0x%04X, %d, 0x%04X );\n", (unsigned)texture, (int)target, (unsigned)renderbuffer);
#endif
  glTextureRenderbufferEXT( (GLuint)texture, (GLenum)target, (GLuint)renderbuffer );
  _checkError("glTextureRenderbufferEXT");
}

FABRIC_EXT_EXPORT void glTextureSubImage1DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Size width,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureSubImage1DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)type);
#endif
  glTextureSubImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTextureSubImage1DEXT");
}

FABRIC_EXT_EXPORT void glTextureSubImage2DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureSubImage2DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)type);
#endif
  glTextureSubImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTextureSubImage2DEXT");
}

FABRIC_EXT_EXPORT void glTextureSubImage3DEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer zoffset,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureSubImage3DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)type);
#endif
  glTextureSubImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTextureSubImage3DEXT");
}

FABRIC_EXT_EXPORT KL::Boolean glUnmapNamedBufferEXT_wrapper(
  KL::Size buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUnmapNamedBufferEXT( 0x%04X );\n", (unsigned)buffer);
#endif
  GLboolean result = glUnmapNamedBufferEXT( (GLuint)buffer );
  _checkError("glUnmapNamedBufferEXT");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glVertexArrayColorOffsetEXT_wrapper(
  KL::Size vaobj,
  KL::Size buffer,
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Integer offset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexArrayColorOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d, %d );\n", (unsigned)vaobj, (unsigned)buffer, (int)size, (int)type, (int)stride, (int)offset);
#endif
  glVertexArrayColorOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  _checkError("glVertexArrayColorOffsetEXT");
}

FABRIC_EXT_EXPORT void glVertexArrayEdgeFlagOffsetEXT_wrapper(
  KL::Size vaobj,
  KL::Size buffer,
  KL::Size stride,
  KL::Integer offset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexArrayEdgeFlagOffsetEXT( 0x%04X, 0x%04X, %d, %d );\n", (unsigned)vaobj, (unsigned)buffer, (int)stride, (int)offset);
#endif
  glVertexArrayEdgeFlagOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLsizei)stride, (GLintptr)offset );
  _checkError("glVertexArrayEdgeFlagOffsetEXT");
}

FABRIC_EXT_EXPORT void glVertexArrayFogCoordOffsetEXT_wrapper(
  KL::Size vaobj,
  KL::Size buffer,
  KL::Size type,
  KL::Size stride,
  KL::Integer offset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexArrayFogCoordOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d );\n", (unsigned)vaobj, (unsigned)buffer, (int)type, (int)stride, (int)offset);
#endif
  glVertexArrayFogCoordOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  _checkError("glVertexArrayFogCoordOffsetEXT");
}

FABRIC_EXT_EXPORT void glVertexArrayIndexOffsetEXT_wrapper(
  KL::Size vaobj,
  KL::Size buffer,
  KL::Size type,
  KL::Size stride,
  KL::Integer offset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexArrayIndexOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d );\n", (unsigned)vaobj, (unsigned)buffer, (int)type, (int)stride, (int)offset);
#endif
  glVertexArrayIndexOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  _checkError("glVertexArrayIndexOffsetEXT");
}

FABRIC_EXT_EXPORT void glVertexArrayMultiTexCoordOffsetEXT_wrapper(
  KL::Size vaobj,
  KL::Size buffer,
  KL::Size texunit,
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Integer offset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexArrayMultiTexCoordOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d, %d, %d );\n", (unsigned)vaobj, (unsigned)buffer, (int)texunit, (int)size, (int)type, (int)stride, (int)offset);
#endif
  glVertexArrayMultiTexCoordOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLenum)texunit, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  _checkError("glVertexArrayMultiTexCoordOffsetEXT");
}

FABRIC_EXT_EXPORT void glVertexArrayNormalOffsetEXT_wrapper(
  KL::Size vaobj,
  KL::Size buffer,
  KL::Size type,
  KL::Size stride,
  KL::Integer offset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexArrayNormalOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d );\n", (unsigned)vaobj, (unsigned)buffer, (int)type, (int)stride, (int)offset);
#endif
  glVertexArrayNormalOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  _checkError("glVertexArrayNormalOffsetEXT");
}

FABRIC_EXT_EXPORT void glVertexArraySecondaryColorOffsetEXT_wrapper(
  KL::Size vaobj,
  KL::Size buffer,
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Integer offset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexArraySecondaryColorOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d, %d );\n", (unsigned)vaobj, (unsigned)buffer, (int)size, (int)type, (int)stride, (int)offset);
#endif
  glVertexArraySecondaryColorOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  _checkError("glVertexArraySecondaryColorOffsetEXT");
}

FABRIC_EXT_EXPORT void glVertexArrayTexCoordOffsetEXT_wrapper(
  KL::Size vaobj,
  KL::Size buffer,
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Integer offset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexArrayTexCoordOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d, %d );\n", (unsigned)vaobj, (unsigned)buffer, (int)size, (int)type, (int)stride, (int)offset);
#endif
  glVertexArrayTexCoordOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  _checkError("glVertexArrayTexCoordOffsetEXT");
}

FABRIC_EXT_EXPORT void glVertexArrayVertexAttribIOffsetEXT_wrapper(
  KL::Size vaobj,
  KL::Size buffer,
  KL::Size index,
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Integer offset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexArrayVertexAttribIOffsetEXT( 0x%04X, 0x%04X, 0x%04X, %d, %d, %d, %d );\n", (unsigned)vaobj, (unsigned)buffer, (unsigned)index, (int)size, (int)type, (int)stride, (int)offset);
#endif
  glVertexArrayVertexAttribIOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLuint)index, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  _checkError("glVertexArrayVertexAttribIOffsetEXT");
}

FABRIC_EXT_EXPORT void glVertexArrayVertexAttribOffsetEXT_wrapper(
  KL::Size vaobj,
  KL::Size buffer,
  KL::Size index,
  KL::Integer size,
  KL::Size type,
  KL::Boolean normalized,
  KL::Size stride,
  KL::Integer offset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexArrayVertexAttribOffsetEXT( 0x%04X, 0x%04X, 0x%04X, %d, %d, %d, %d, %d );\n", (unsigned)vaobj, (unsigned)buffer, (unsigned)index, (int)size, (int)type, (int)normalized, (int)stride, (int)offset);
#endif
  glVertexArrayVertexAttribOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLuint)index, (GLint)size, (GLenum)type, (GLboolean)normalized, (GLsizei)stride, (GLintptr)offset );
  _checkError("glVertexArrayVertexAttribOffsetEXT");
}

FABRIC_EXT_EXPORT void glVertexArrayVertexOffsetEXT_wrapper(
  KL::Size vaobj,
  KL::Size buffer,
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Integer offset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexArrayVertexOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d, %d );\n", (unsigned)vaobj, (unsigned)buffer, (int)size, (int)type, (int)stride, (int)offset);
#endif
  glVertexArrayVertexOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  _checkError("glVertexArrayVertexOffsetEXT");
}

FABRIC_EXT_EXPORT void glColorMaskIndexedEXT_wrapper(
  KL::Size buf,
  KL::Boolean r,
  KL::Boolean g,
  KL::Boolean b,
  KL::Boolean a
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColorMaskIndexedEXT( 0x%04X, %d, %d, %d, %d );\n", (unsigned)buf, (int)r, (int)g, (int)b, (int)a);
#endif
  glColorMaskIndexedEXT( (GLuint)buf, (GLboolean)r, (GLboolean)g, (GLboolean)b, (GLboolean)a );
  _checkError("glColorMaskIndexedEXT");
}

FABRIC_EXT_EXPORT void glDisableIndexedEXT_wrapper(
  KL::Size target,
  KL::Size index
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDisableIndexedEXT( %d, 0x%04X );\n", (int)target, (unsigned)index);
#endif
  glDisableIndexedEXT( (GLenum)target, (GLuint)index );
  _checkError("glDisableIndexedEXT");
}

FABRIC_EXT_EXPORT void glEnableIndexedEXT_wrapper(
  KL::Size target,
  KL::Size index
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEnableIndexedEXT( %d, 0x%04X );\n", (int)target, (unsigned)index);
#endif
  glEnableIndexedEXT( (GLenum)target, (GLuint)index );
  _checkError("glEnableIndexedEXT");
}

FABRIC_EXT_EXPORT void glGetBooleanIndexedvEXT_wrapper(
  KL::Size value,
  KL::Size index,
  KL::VariableArray<KL::Boolean> & data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetBooleanIndexedvEXT( %d, 0x%04X, GLboolean* );\n", (int)value, (unsigned)index);
#endif
  glGetBooleanIndexedvEXT( (GLenum)value, (GLuint)index, (GLboolean*)&data[0] );
  _checkError("glGetBooleanIndexedvEXT");
}

FABRIC_EXT_EXPORT void glGetIntegerIndexedvEXT_wrapper(
  KL::Size value,
  KL::Size index,
  KL::VariableArray<KL::Integer> & data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetIntegerIndexedvEXT( %d, 0x%04X, GLint* );\n", (int)value, (unsigned)index);
#endif
  glGetIntegerIndexedvEXT( (GLenum)value, (GLuint)index, (GLint*)&data[0] );
  _checkError("glGetIntegerIndexedvEXT");
}

FABRIC_EXT_EXPORT KL::Boolean glIsEnabledIndexedEXT_wrapper(
  KL::Size target,
  KL::Size index
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsEnabledIndexedEXT( %d, 0x%04X );\n", (int)target, (unsigned)index);
#endif
  GLboolean result = glIsEnabledIndexedEXT( (GLenum)target, (GLuint)index );
  _checkError("glIsEnabledIndexedEXT");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glDrawArraysInstancedEXT_wrapper(
  KL::Size mode,
  KL::Integer start,
  KL::Size count,
  KL::Size primcount
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawArraysInstancedEXT( %d, %d, %d, %d );\n", (int)mode, (int)start, (int)count, (int)primcount);
#endif
  glDrawArraysInstancedEXT( (GLenum)mode, (GLint)start, (GLsizei)count, (GLsizei)primcount );
  _checkError("glDrawArraysInstancedEXT");
}

FABRIC_EXT_EXPORT void glDrawElementsInstancedEXT_wrapper(
  KL::Size mode,
  KL::Size count,
  KL::Size type,
  KL::Data indices,
  KL::Size primcount
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawElementsInstancedEXT( %d, %d, %d, GLvoid*, %d );\n", (int)mode, (int)count, (int)type, (int)primcount);
#endif
  glDrawElementsInstancedEXT( (GLenum)mode, (GLsizei)count, (GLenum)type, indices, (GLsizei)primcount );
  _checkError("glDrawElementsInstancedEXT");
}

FABRIC_EXT_EXPORT void glDrawRangeElementsEXT_wrapper(
  KL::Size mode,
  KL::Size start,
  KL::Size end,
  KL::Size count,
  KL::Size type,
  KL::Data indices
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawRangeElementsEXT( %d, 0x%04X, 0x%04X, %d, %d, GLvoid* );\n", (int)mode, (unsigned)start, (unsigned)end, (int)count, (int)type);
#endif
  glDrawRangeElementsEXT( (GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, indices );
  _checkError("glDrawRangeElementsEXT");
}

FABRIC_EXT_EXPORT void glFogCoordPointerEXT_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogCoordPointerEXT( %d, %d, GLvoid* );\n", (int)type, (int)stride);
#endif
  glFogCoordPointerEXT( (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glFogCoordPointerEXT");
}

FABRIC_EXT_EXPORT void glFogCoorddEXT_wrapper(
  KL::Scalar coord
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogCoorddEXT( %f );\n", (float)coord);
#endif
  glFogCoorddEXT( (GLdouble)coord );
  _checkError("glFogCoorddEXT");
}

FABRIC_EXT_EXPORT void glFogCoorddvEXT_wrapper(
  const KL::VariableArray<KL::Scalar> & coord
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogCoorddvEXT( GLdouble* );\n");
#endif
  glFogCoorddvEXT( (const GLdouble*)&coord[0] );
  _checkError("glFogCoorddvEXT");
}

FABRIC_EXT_EXPORT void glFogCoordfEXT_wrapper(
  KL::Scalar coord
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogCoordfEXT( %f );\n", (float)coord);
#endif
  glFogCoordfEXT( (GLfloat)coord );
  _checkError("glFogCoordfEXT");
}

FABRIC_EXT_EXPORT void glFogCoordfvEXT_wrapper(
  const KL::VariableArray<KL::Scalar> & coord
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFogCoordfvEXT( GLfloat* );\n");
#endif
  glFogCoordfvEXT( (const GLfloat*)&coord[0] );
  _checkError("glFogCoordfvEXT");
}

FABRIC_EXT_EXPORT void glFragmentColorMaterialEXT_wrapper(
  KL::Size face,
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentColorMaterialEXT( %d, %d );\n", (int)face, (int)mode);
#endif
  glFragmentColorMaterialEXT( (GLenum)face, (GLenum)mode );
  _checkError("glFragmentColorMaterialEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightModelfEXT_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentLightModelfEXT( %d, %f );\n", (int)pname, (float)param);
#endif
  glFragmentLightModelfEXT( (GLenum)pname, (GLfloat)param );
  _checkError("glFragmentLightModelfEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightModelfvEXT_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentLightModelfvEXT( %d, GLfloat* );\n", (int)pname);
#endif
  glFragmentLightModelfvEXT( (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glFragmentLightModelfvEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightModeliEXT_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentLightModeliEXT( %d, %d );\n", (int)pname, (int)param);
#endif
  glFragmentLightModeliEXT( (GLenum)pname, (GLint)param );
  _checkError("glFragmentLightModeliEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightModelivEXT_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentLightModelivEXT( %d, GLint* );\n", (int)pname);
#endif
  glFragmentLightModelivEXT( (GLenum)pname, (GLint*)&params[0] );
  _checkError("glFragmentLightModelivEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightfEXT_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentLightfEXT( %d, %d, %f );\n", (int)light, (int)pname, (float)param);
#endif
  glFragmentLightfEXT( (GLenum)light, (GLenum)pname, (GLfloat)param );
  _checkError("glFragmentLightfEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightfvEXT_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentLightfvEXT( %d, %d, GLfloat* );\n", (int)light, (int)pname);
#endif
  glFragmentLightfvEXT( (GLenum)light, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glFragmentLightfvEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightiEXT_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentLightiEXT( %d, %d, %d );\n", (int)light, (int)pname, (int)param);
#endif
  glFragmentLightiEXT( (GLenum)light, (GLenum)pname, (GLint)param );
  _checkError("glFragmentLightiEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightivEXT_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentLightivEXT( %d, %d, GLint* );\n", (int)light, (int)pname);
#endif
  glFragmentLightivEXT( (GLenum)light, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glFragmentLightivEXT");
}

FABRIC_EXT_EXPORT void glFragmentMaterialfEXT_wrapper(
  KL::Size face,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentMaterialfEXT( %d, %d, %f );\n", (int)face, (int)pname, (float)param);
#endif
  glFragmentMaterialfEXT( (GLenum)face, (GLenum)pname, (const GLfloat)param );
  _checkError("glFragmentMaterialfEXT");
}

FABRIC_EXT_EXPORT void glFragmentMaterialfvEXT_wrapper(
  KL::Size face,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentMaterialfvEXT( %d, %d, GLfloat* );\n", (int)face, (int)pname);
#endif
  glFragmentMaterialfvEXT( (GLenum)face, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glFragmentMaterialfvEXT");
}

FABRIC_EXT_EXPORT void glFragmentMaterialiEXT_wrapper(
  KL::Size face,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentMaterialiEXT( %d, %d, %d );\n", (int)face, (int)pname, (int)param);
#endif
  glFragmentMaterialiEXT( (GLenum)face, (GLenum)pname, (const GLint)param );
  _checkError("glFragmentMaterialiEXT");
}

FABRIC_EXT_EXPORT void glFragmentMaterialivEXT_wrapper(
  KL::Size face,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFragmentMaterialivEXT( %d, %d, GLint* );\n", (int)face, (int)pname);
#endif
  glFragmentMaterialivEXT( (GLenum)face, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glFragmentMaterialivEXT");
}

FABRIC_EXT_EXPORT void glGetFragmentLightfvEXT_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetFragmentLightfvEXT( %d, %d, GLfloat* );\n", (int)light, (int)pname);
#endif
  glGetFragmentLightfvEXT( (GLenum)light, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetFragmentLightfvEXT");
}

FABRIC_EXT_EXPORT void glGetFragmentLightivEXT_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetFragmentLightivEXT( %d, %d, GLint* );\n", (int)light, (int)pname);
#endif
  glGetFragmentLightivEXT( (GLenum)light, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetFragmentLightivEXT");
}

FABRIC_EXT_EXPORT void glGetFragmentMaterialfvEXT_wrapper(
  KL::Size face,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetFragmentMaterialfvEXT( %d, %d, GLfloat* );\n", (int)face, (int)pname);
#endif
  glGetFragmentMaterialfvEXT( (GLenum)face, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glGetFragmentMaterialfvEXT");
}

FABRIC_EXT_EXPORT void glGetFragmentMaterialivEXT_wrapper(
  KL::Size face,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetFragmentMaterialivEXT( %d, %d, GLint* );\n", (int)face, (int)pname);
#endif
  glGetFragmentMaterialivEXT( (GLenum)face, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glGetFragmentMaterialivEXT");
}

FABRIC_EXT_EXPORT void glLightEnviEXT_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glLightEnviEXT( %d, %d );\n", (int)pname, (int)param);
#endif
  glLightEnviEXT( (GLenum)pname, (GLint)param );
  _checkError("glLightEnviEXT");
}

FABRIC_EXT_EXPORT void glBlitFramebufferEXT_wrapper(
  KL::Integer srcX0,
  KL::Integer srcY0,
  KL::Integer srcX1,
  KL::Integer srcY1,
  KL::Integer dstX0,
  KL::Integer dstY0,
  KL::Integer dstX1,
  KL::Integer dstY1,
  KL::Size mask,
  KL::Size filter
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBlitFramebufferEXT( %d, %d, %d, %d, %d, %d, %d, %d, 0x%04X, %d );\n", (int)srcX0, (int)srcY0, (int)srcX1, (int)srcY1, (int)dstX0, (int)dstY0, (int)dstX1, (int)dstY1, (unsigned)mask, (int)filter);
#endif
  glBlitFramebufferEXT( (GLint)srcX0, (GLint)srcY0, (GLint)srcX1, (GLint)srcY1, (GLint)dstX0, (GLint)dstY0, (GLint)dstX1, (GLint)dstY1, (GLbitfield)mask, (GLenum)filter );
  _checkError("glBlitFramebufferEXT");
}

FABRIC_EXT_EXPORT void glRenderbufferStorageMultisampleEXT_wrapper(
  KL::Size target,
  KL::Size samples,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRenderbufferStorageMultisampleEXT( %d, %d, %d, %d, %d );\n", (int)target, (int)samples, (int)internalformat, (int)width, (int)height);
#endif
  glRenderbufferStorageMultisampleEXT( (GLenum)target, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  _checkError("glRenderbufferStorageMultisampleEXT");
}

FABRIC_EXT_EXPORT void glBindFramebufferEXT_wrapper(
  KL::Size target,
  KL::Size framebuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindFramebufferEXT( %d, 0x%04X );\n", (int)target, (unsigned)framebuffer);
#endif
  glBindFramebufferEXT( (GLenum)target, (GLuint)framebuffer );
  _checkError("glBindFramebufferEXT");
}

FABRIC_EXT_EXPORT void glBindRenderbufferEXT_wrapper(
  KL::Size target,
  KL::Size renderbuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindRenderbufferEXT( %d, 0x%04X );\n", (int)target, (unsigned)renderbuffer);
#endif
  glBindRenderbufferEXT( (GLenum)target, (GLuint)renderbuffer );
  _checkError("glBindRenderbufferEXT");
}

FABRIC_EXT_EXPORT KL::Size glCheckFramebufferStatusEXT_wrapper(
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCheckFramebufferStatusEXT( %d );\n", (int)target);
#endif
  GLenum result = glCheckFramebufferStatusEXT( (GLenum)target );
  _checkError("glCheckFramebufferStatusEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glDeleteFramebuffersEXT_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & framebuffers
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteFramebuffersEXT( %d, GLuint* );\n", (int)n);
#endif
  glDeleteFramebuffersEXT( (GLsizei)n, (const GLuint*)&framebuffers[0] );
  _checkError("glDeleteFramebuffersEXT");
}

FABRIC_EXT_EXPORT void glDeleteRenderbuffersEXT_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & renderbuffers
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteRenderbuffersEXT( %d, GLuint* );\n", (int)n);
#endif
  glDeleteRenderbuffersEXT( (GLsizei)n, (const GLuint*)&renderbuffers[0] );
  _checkError("glDeleteRenderbuffersEXT");
}

FABRIC_EXT_EXPORT void glFramebufferRenderbufferEXT_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size renderbuffertarget,
  KL::Size renderbuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferRenderbufferEXT( %d, %d, %d, 0x%04X );\n", (int)target, (int)attachment, (int)renderbuffertarget, (unsigned)renderbuffer);
#endif
  glFramebufferRenderbufferEXT( (GLenum)target, (GLenum)attachment, (GLenum)renderbuffertarget, (GLuint)renderbuffer );
  _checkError("glFramebufferRenderbufferEXT");
}

FABRIC_EXT_EXPORT void glFramebufferTexture1DEXT_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size textarget,
  KL::Size texture,
  KL::Integer level
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTexture1DEXT( %d, %d, %d, 0x%04X, %d );\n", (int)target, (int)attachment, (int)textarget, (unsigned)texture, (int)level);
#endif
  glFramebufferTexture1DEXT( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
  _checkError("glFramebufferTexture1DEXT");
}

FABRIC_EXT_EXPORT void glFramebufferTexture2DEXT_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size textarget,
  KL::Size texture,
  KL::Integer level
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTexture2DEXT( %d, %d, %d, 0x%04X, %d );\n", (int)target, (int)attachment, (int)textarget, (unsigned)texture, (int)level);
#endif
  glFramebufferTexture2DEXT( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
  _checkError("glFramebufferTexture2DEXT");
}

FABRIC_EXT_EXPORT void glFramebufferTexture3DEXT_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size textarget,
  KL::Size texture,
  KL::Integer level,
  KL::Integer zoffset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTexture3DEXT( %d, %d, %d, 0x%04X, %d, %d );\n", (int)target, (int)attachment, (int)textarget, (unsigned)texture, (int)level, (int)zoffset);
#endif
  glFramebufferTexture3DEXT( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level, (GLint)zoffset );
  _checkError("glFramebufferTexture3DEXT");
}

FABRIC_EXT_EXPORT void glGenFramebuffersEXT_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & framebuffers
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenFramebuffersEXT( %d, GLuint* );\n", (int)n);
#endif
  glGenFramebuffersEXT( (GLsizei)n, (GLuint*)&framebuffers[0] );
  _checkError("glGenFramebuffersEXT");
}

FABRIC_EXT_EXPORT void glGenRenderbuffersEXT_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & renderbuffers
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenRenderbuffersEXT( %d, GLuint* );\n", (int)n);
#endif
  glGenRenderbuffersEXT( (GLsizei)n, (GLuint*)&renderbuffers[0] );
  _checkError("glGenRenderbuffersEXT");
}

FABRIC_EXT_EXPORT void glGenerateMipmapEXT_wrapper(
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenerateMipmapEXT( %d );\n", (int)target);
#endif
  glGenerateMipmapEXT( (GLenum)target );
  _checkError("glGenerateMipmapEXT");
}

FABRIC_EXT_EXPORT void glGetFramebufferAttachmentParameterivEXT_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetFramebufferAttachmentParameterivEXT( %d, %d, %d, GLint* );\n", (int)target, (int)attachment, (int)pname);
#endif
  glGetFramebufferAttachmentParameterivEXT( (GLenum)target, (GLenum)attachment, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetFramebufferAttachmentParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetRenderbufferParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetRenderbufferParameterivEXT( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetRenderbufferParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetRenderbufferParameterivEXT");
}

FABRIC_EXT_EXPORT KL::Boolean glIsFramebufferEXT_wrapper(
  KL::Size framebuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsFramebufferEXT( 0x%04X );\n", (unsigned)framebuffer);
#endif
  GLboolean result = glIsFramebufferEXT( (GLuint)framebuffer );
  _checkError("glIsFramebufferEXT");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glIsRenderbufferEXT_wrapper(
  KL::Size renderbuffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsRenderbufferEXT( 0x%04X );\n", (unsigned)renderbuffer);
#endif
  GLboolean result = glIsRenderbufferEXT( (GLuint)renderbuffer );
  _checkError("glIsRenderbufferEXT");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glRenderbufferStorageEXT_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glRenderbufferStorageEXT( %d, %d, %d, %d );\n", (int)target, (int)internalformat, (int)width, (int)height);
#endif
  glRenderbufferStorageEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  _checkError("glRenderbufferStorageEXT");
}

FABRIC_EXT_EXPORT void glFramebufferTextureEXT_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size texture,
  KL::Integer level
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTextureEXT( %d, %d, 0x%04X, %d );\n", (int)target, (int)attachment, (unsigned)texture, (int)level);
#endif
  glFramebufferTextureEXT( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level );
  _checkError("glFramebufferTextureEXT");
}

FABRIC_EXT_EXPORT void glFramebufferTextureFaceEXT_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size texture,
  KL::Integer level,
  KL::Size face
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTextureFaceEXT( %d, %d, 0x%04X, %d, %d );\n", (int)target, (int)attachment, (unsigned)texture, (int)level, (int)face);
#endif
  glFramebufferTextureFaceEXT( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLenum)face );
  _checkError("glFramebufferTextureFaceEXT");
}

FABRIC_EXT_EXPORT void glFramebufferTextureLayerEXT_wrapper(
  KL::Size target,
  KL::Size attachment,
  KL::Size texture,
  KL::Integer level,
  KL::Integer layer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glFramebufferTextureLayerEXT( %d, %d, 0x%04X, %d, %d );\n", (int)target, (int)attachment, (unsigned)texture, (int)level, (int)layer);
#endif
  glFramebufferTextureLayerEXT( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer );
  _checkError("glFramebufferTextureLayerEXT");
}

FABRIC_EXT_EXPORT void glProgramParameteriEXT_wrapper(
  KL::Size program,
  KL::Size pname,
  KL::Integer value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramParameteriEXT( 0x%04X, %d, %d );\n", (unsigned)program, (int)pname, (int)value);
#endif
  glProgramParameteriEXT( (GLuint)program, (GLenum)pname, (GLint)value );
  _checkError("glProgramParameteriEXT");
}

FABRIC_EXT_EXPORT void glProgramEnvParameters4fvEXT_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramEnvParameters4fvEXT( %d, 0x%04X, %d, GLfloat* );\n", (int)target, (unsigned)index, (int)count);
#endif
  glProgramEnvParameters4fvEXT( (GLenum)target, (GLuint)index, (GLsizei)count, (const GLfloat*)&params[0] );
  _checkError("glProgramEnvParameters4fvEXT");
}

FABRIC_EXT_EXPORT void glProgramLocalParameters4fvEXT_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProgramLocalParameters4fvEXT( %d, 0x%04X, %d, GLfloat* );\n", (int)target, (unsigned)index, (int)count);
#endif
  glProgramLocalParameters4fvEXT( (GLenum)target, (GLuint)index, (GLsizei)count, (const GLfloat*)&params[0] );
  _checkError("glProgramLocalParameters4fvEXT");
}

FABRIC_EXT_EXPORT void glBindFragDataLocationEXT_wrapper(
  KL::Size program,
  KL::Size color,
  const KL::String &name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindFragDataLocationEXT( 0x%04X, 0x%04X, char* );\n", (unsigned)program, (unsigned)color);
#endif
  glBindFragDataLocationEXT( (GLuint)program, (GLuint)color, (const GLchar*)name.data() );
  _checkError("glBindFragDataLocationEXT");
}

FABRIC_EXT_EXPORT KL::Integer glGetFragDataLocationEXT_wrapper(
  KL::Size program,
  const KL::String &name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetFragDataLocationEXT( 0x%04X, char* );\n", (unsigned)program);
#endif
  GLint result = glGetFragDataLocationEXT( (GLuint)program, (const GLchar*)name.data() );
  _checkError("glGetFragDataLocationEXT");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT void glGetUniformuivEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetUniformuivEXT( 0x%04X, %d, GLuint* );\n", (unsigned)program, (int)location);
#endif
  glGetUniformuivEXT( (GLuint)program, (GLint)location, (GLuint*)&params[0] );
  _checkError("glGetUniformuivEXT");
}

FABRIC_EXT_EXPORT void glGetVertexAttribIivEXT_wrapper(
  KL::Size index,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexAttribIivEXT( 0x%04X, %d, GLint* );\n", (unsigned)index, (int)pname);
#endif
  glGetVertexAttribIivEXT( (GLuint)index, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetVertexAttribIivEXT");
}

FABRIC_EXT_EXPORT void glGetVertexAttribIuivEXT_wrapper(
  KL::Size index,
  KL::Size pname,
  KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVertexAttribIuivEXT( 0x%04X, %d, GLuint* );\n", (unsigned)index, (int)pname);
#endif
  glGetVertexAttribIuivEXT( (GLuint)index, (GLenum)pname, (GLuint*)&params[0] );
  _checkError("glGetVertexAttribIuivEXT");
}

FABRIC_EXT_EXPORT void glUniform1uiEXT_wrapper(
  KL::Integer location,
  KL::Size v0
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform1uiEXT( %d, 0x%04X );\n", (int)location, (unsigned)v0);
#endif
  glUniform1uiEXT( (GLint)location, (GLuint)v0 );
  _checkError("glUniform1uiEXT");
}

FABRIC_EXT_EXPORT void glUniform1uivEXT_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Size> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform1uivEXT( %d, %d, GLuint* );\n", (int)location, (int)count);
#endif
  glUniform1uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  _checkError("glUniform1uivEXT");
}

FABRIC_EXT_EXPORT void glUniform2uiEXT_wrapper(
  KL::Integer location,
  KL::Size v0,
  KL::Size v1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform2uiEXT( %d, 0x%04X, 0x%04X );\n", (int)location, (unsigned)v0, (unsigned)v1);
#endif
  glUniform2uiEXT( (GLint)location, (GLuint)v0, (GLuint)v1 );
  _checkError("glUniform2uiEXT");
}

FABRIC_EXT_EXPORT void glUniform2uivEXT_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Size> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform2uivEXT( %d, %d, GLuint* );\n", (int)location, (int)count);
#endif
  glUniform2uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  _checkError("glUniform2uivEXT");
}

FABRIC_EXT_EXPORT void glUniform3uiEXT_wrapper(
  KL::Integer location,
  KL::Size v0,
  KL::Size v1,
  KL::Size v2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform3uiEXT( %d, 0x%04X, 0x%04X, 0x%04X );\n", (int)location, (unsigned)v0, (unsigned)v1, (unsigned)v2);
#endif
  glUniform3uiEXT( (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2 );
  _checkError("glUniform3uiEXT");
}

FABRIC_EXT_EXPORT void glUniform3uivEXT_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Size> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform3uivEXT( %d, %d, GLuint* );\n", (int)location, (int)count);
#endif
  glUniform3uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  _checkError("glUniform3uivEXT");
}

FABRIC_EXT_EXPORT void glUniform4uiEXT_wrapper(
  KL::Integer location,
  KL::Size v0,
  KL::Size v1,
  KL::Size v2,
  KL::Size v3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform4uiEXT( %d, 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (int)location, (unsigned)v0, (unsigned)v1, (unsigned)v2, (unsigned)v3);
#endif
  glUniform4uiEXT( (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2, (GLuint)v3 );
  _checkError("glUniform4uiEXT");
}

FABRIC_EXT_EXPORT void glUniform4uivEXT_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Size> & value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUniform4uivEXT( %d, %d, GLuint* );\n", (int)location, (int)count);
#endif
  glUniform4uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  _checkError("glUniform4uivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI1iEXT_wrapper(
  KL::Size index,
  KL::Integer x
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI1iEXT( 0x%04X, %d );\n", (unsigned)index, (int)x);
#endif
  glVertexAttribI1iEXT( (GLuint)index, (GLint)x );
  _checkError("glVertexAttribI1iEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI1ivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI1ivEXT( 0x%04X, GLint* );\n", (unsigned)index);
#endif
  glVertexAttribI1ivEXT( (GLuint)index, (const GLint*)&v[0] );
  _checkError("glVertexAttribI1ivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI1uiEXT_wrapper(
  KL::Size index,
  KL::Size x
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI1uiEXT( 0x%04X, 0x%04X );\n", (unsigned)index, (unsigned)x);
#endif
  glVertexAttribI1uiEXT( (GLuint)index, (GLuint)x );
  _checkError("glVertexAttribI1uiEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI1uivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI1uivEXT( 0x%04X, GLuint* );\n", (unsigned)index);
#endif
  glVertexAttribI1uivEXT( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttribI1uivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI2iEXT_wrapper(
  KL::Size index,
  KL::Integer x,
  KL::Integer y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI2iEXT( 0x%04X, %d, %d );\n", (unsigned)index, (int)x, (int)y);
#endif
  glVertexAttribI2iEXT( (GLuint)index, (GLint)x, (GLint)y );
  _checkError("glVertexAttribI2iEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI2ivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI2ivEXT( 0x%04X, GLint* );\n", (unsigned)index);
#endif
  glVertexAttribI2ivEXT( (GLuint)index, (const GLint*)&v[0] );
  _checkError("glVertexAttribI2ivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI2uiEXT_wrapper(
  KL::Size index,
  KL::Size x,
  KL::Size y
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI2uiEXT( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)index, (unsigned)x, (unsigned)y);
#endif
  glVertexAttribI2uiEXT( (GLuint)index, (GLuint)x, (GLuint)y );
  _checkError("glVertexAttribI2uiEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI2uivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI2uivEXT( 0x%04X, GLuint* );\n", (unsigned)index);
#endif
  glVertexAttribI2uivEXT( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttribI2uivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI3iEXT_wrapper(
  KL::Size index,
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI3iEXT( 0x%04X, %d, %d, %d );\n", (unsigned)index, (int)x, (int)y, (int)z);
#endif
  glVertexAttribI3iEXT( (GLuint)index, (GLint)x, (GLint)y, (GLint)z );
  _checkError("glVertexAttribI3iEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI3ivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI3ivEXT( 0x%04X, GLint* );\n", (unsigned)index);
#endif
  glVertexAttribI3ivEXT( (GLuint)index, (const GLint*)&v[0] );
  _checkError("glVertexAttribI3ivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI3uiEXT_wrapper(
  KL::Size index,
  KL::Size x,
  KL::Size y,
  KL::Size z
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI3uiEXT( 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)index, (unsigned)x, (unsigned)y, (unsigned)z);
#endif
  glVertexAttribI3uiEXT( (GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z );
  _checkError("glVertexAttribI3uiEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI3uivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI3uivEXT( 0x%04X, GLuint* );\n", (unsigned)index);
#endif
  glVertexAttribI3uivEXT( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttribI3uivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4bvEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4bvEXT( 0x%04X, GLbyte* );\n", (unsigned)index);
#endif
  glVertexAttribI4bvEXT( (GLuint)index, (const GLbyte*)&v[0] );
  _checkError("glVertexAttribI4bvEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4iEXT_wrapper(
  KL::Size index,
  KL::Integer x,
  KL::Integer y,
  KL::Integer z,
  KL::Integer w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4iEXT( 0x%04X, %d, %d, %d, %d );\n", (unsigned)index, (int)x, (int)y, (int)z, (int)w);
#endif
  glVertexAttribI4iEXT( (GLuint)index, (GLint)x, (GLint)y, (GLint)z, (GLint)w );
  _checkError("glVertexAttribI4iEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4ivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4ivEXT( 0x%04X, GLint* );\n", (unsigned)index);
#endif
  glVertexAttribI4ivEXT( (GLuint)index, (const GLint*)&v[0] );
  _checkError("glVertexAttribI4ivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4svEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4svEXT( 0x%04X, GLshort* );\n", (unsigned)index);
#endif
  glVertexAttribI4svEXT( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttribI4svEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4ubvEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4ubvEXT( 0x%04X, GLubyte* );\n", (unsigned)index);
#endif
  glVertexAttribI4ubvEXT( (GLuint)index, (const GLubyte*)&v[0] );
  _checkError("glVertexAttribI4ubvEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4uiEXT_wrapper(
  KL::Size index,
  KL::Size x,
  KL::Size y,
  KL::Size z,
  KL::Size w
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4uiEXT( 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)index, (unsigned)x, (unsigned)y, (unsigned)z, (unsigned)w);
#endif
  glVertexAttribI4uiEXT( (GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z, (GLuint)w );
  _checkError("glVertexAttribI4uiEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4uivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4uivEXT( 0x%04X, GLuint* );\n", (unsigned)index);
#endif
  glVertexAttribI4uivEXT( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttribI4uivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4usvEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribI4usvEXT( 0x%04X, GLushort* );\n", (unsigned)index);
#endif
  glVertexAttribI4usvEXT( (GLuint)index, (const GLushort*)&v[0] );
  _checkError("glVertexAttribI4usvEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribIPointerEXT_wrapper(
  KL::Size index,
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexAttribIPointerEXT( 0x%04X, %d, %d, %d, GLvoid* );\n", (unsigned)index, (int)size, (int)type, (int)stride);
#endif
  glVertexAttribIPointerEXT( (GLuint)index, (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glVertexAttribIPointerEXT");
}

FABRIC_EXT_EXPORT void glGetHistogramEXT_wrapper(
  KL::Size target,
  KL::Boolean reset,
  KL::Size format,
  KL::Size type,
  KL::Data values
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetHistogramEXT( %d, %d, %d, %d, void* );\n", (int)target, (int)reset, (int)format, (int)type);
#endif
  glGetHistogramEXT( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, values );
  _checkError("glGetHistogramEXT");
}

FABRIC_EXT_EXPORT void glGetHistogramParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetHistogramParameterfvEXT( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glGetHistogramParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetHistogramParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetHistogramParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetHistogramParameterivEXT( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetHistogramParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetHistogramParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetMinmaxEXT_wrapper(
  KL::Size target,
  KL::Boolean reset,
  KL::Size format,
  KL::Size type,
  KL::Data values
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMinmaxEXT( %d, %d, %d, %d, void* );\n", (int)target, (int)reset, (int)format, (int)type);
#endif
  glGetMinmaxEXT( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, values );
  _checkError("glGetMinmaxEXT");
}

FABRIC_EXT_EXPORT void glGetMinmaxParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMinmaxParameterfvEXT( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glGetMinmaxParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetMinmaxParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetMinmaxParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetMinmaxParameterivEXT( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetMinmaxParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetMinmaxParameterivEXT");
}

FABRIC_EXT_EXPORT void glHistogramEXT_wrapper(
  KL::Size target,
  KL::Size width,
  KL::Size internalformat,
  KL::Boolean sink
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glHistogramEXT( %d, %d, %d, %d );\n", (int)target, (int)width, (int)internalformat, (int)sink);
#endif
  glHistogramEXT( (GLenum)target, (GLsizei)width, (GLenum)internalformat, (GLboolean)sink );
  _checkError("glHistogramEXT");
}

FABRIC_EXT_EXPORT void glMinmaxEXT_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Boolean sink
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMinmaxEXT( %d, %d, %d );\n", (int)target, (int)internalformat, (int)sink);
#endif
  glMinmaxEXT( (GLenum)target, (GLenum)internalformat, (GLboolean)sink );
  _checkError("glMinmaxEXT");
}

FABRIC_EXT_EXPORT void glResetHistogramEXT_wrapper(
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glResetHistogramEXT( %d );\n", (int)target);
#endif
  glResetHistogramEXT( (GLenum)target );
  _checkError("glResetHistogramEXT");
}

FABRIC_EXT_EXPORT void glResetMinmaxEXT_wrapper(
  KL::Size target
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glResetMinmaxEXT( %d );\n", (int)target);
#endif
  glResetMinmaxEXT( (GLenum)target );
  _checkError("glResetMinmaxEXT");
}

FABRIC_EXT_EXPORT void glIndexFuncEXT_wrapper(
  KL::Size func,
  KL::Scalar ref
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexFuncEXT( %d, %f );\n", (int)func, (float)ref);
#endif
  glIndexFuncEXT( (GLenum)func, (GLfloat)ref );
  _checkError("glIndexFuncEXT");
}

FABRIC_EXT_EXPORT void glIndexMaterialEXT_wrapper(
  KL::Size face,
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexMaterialEXT( %d, %d );\n", (int)face, (int)mode);
#endif
  glIndexMaterialEXT( (GLenum)face, (GLenum)mode );
  _checkError("glIndexMaterialEXT");
}

FABRIC_EXT_EXPORT void glApplyTextureEXT_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glApplyTextureEXT( %d );\n", (int)mode);
#endif
  glApplyTextureEXT( (GLenum)mode );
  _checkError("glApplyTextureEXT");
}

FABRIC_EXT_EXPORT void glTextureLightEXT_wrapper(
  KL::Size pname
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureLightEXT( %d );\n", (int)pname);
#endif
  glTextureLightEXT( (GLenum)pname );
  _checkError("glTextureLightEXT");
}

FABRIC_EXT_EXPORT void glTextureMaterialEXT_wrapper(
  KL::Size face,
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureMaterialEXT( %d, %d );\n", (int)face, (int)mode);
#endif
  glTextureMaterialEXT( (GLenum)face, (GLenum)mode );
  _checkError("glTextureMaterialEXT");
}

FABRIC_EXT_EXPORT void glMultiDrawArraysEXT_wrapper(
  KL::Size mode,
  KL::VariableArray<KL::Integer> & first,
  KL::VariableArray<KL::Size> & count,
  KL::Size primcount
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glMultiDrawArraysEXT( %d, GLint*, GLsizei*, %d );\n", (int)mode, (int)primcount);
#endif
  glMultiDrawArraysEXT( (GLenum)mode, (GLint*)&first[0], (GLsizei*)&count[0], (GLsizei)primcount );
  _checkError("glMultiDrawArraysEXT");
}

FABRIC_EXT_EXPORT void glSampleMaskEXT_wrapper(
  KL::Scalar value,
  KL::Boolean invert
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSampleMaskEXT( %f, %d );\n", (float)value, (int)invert);
#endif
  glSampleMaskEXT( (GLclampf)value, (GLboolean)invert );
  _checkError("glSampleMaskEXT");
}

FABRIC_EXT_EXPORT void glSamplePatternEXT_wrapper(
  KL::Size pattern
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSamplePatternEXT( %d );\n", (int)pattern);
#endif
  glSamplePatternEXT( (GLenum)pattern );
  _checkError("glSamplePatternEXT");
}

FABRIC_EXT_EXPORT void glColorTableEXT_wrapper(
  KL::Size target,
  KL::Size internalFormat,
  KL::Size width,
  KL::Size format,
  KL::Size type,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColorTableEXT( %d, %d, %d, %d, %d, void* );\n", (int)target, (int)internalFormat, (int)width, (int)format, (int)type);
#endif
  glColorTableEXT( (GLenum)target, (GLenum)internalFormat, (GLsizei)width, (GLenum)format, (GLenum)type, data );
  _checkError("glColorTableEXT");
}

FABRIC_EXT_EXPORT void glGetColorTableEXT_wrapper(
  KL::Size target,
  KL::Size format,
  KL::Size type,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetColorTableEXT( %d, %d, %d, void* );\n", (int)target, (int)format, (int)type);
#endif
  glGetColorTableEXT( (GLenum)target, (GLenum)format, (GLenum)type, data );
  _checkError("glGetColorTableEXT");
}

FABRIC_EXT_EXPORT void glGetColorTableParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetColorTableParameterfvEXT( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glGetColorTableParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetColorTableParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetColorTableParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetColorTableParameterivEXT( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetColorTableParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetColorTableParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetPixelTransformParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetPixelTransformParameterfvEXT( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glGetPixelTransformParameterfvEXT( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glGetPixelTransformParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetPixelTransformParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetPixelTransformParameterivEXT( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetPixelTransformParameterivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glGetPixelTransformParameterivEXT");
}

FABRIC_EXT_EXPORT void glPixelTransformParameterfEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPixelTransformParameterfEXT( %d, %d, %f );\n", (int)target, (int)pname, (float)param);
#endif
  glPixelTransformParameterfEXT( (GLenum)target, (GLenum)pname, (const GLfloat)param );
  _checkError("glPixelTransformParameterfEXT");
}

FABRIC_EXT_EXPORT void glPixelTransformParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPixelTransformParameterfvEXT( %d, %d, GLfloat* );\n", (int)target, (int)pname);
#endif
  glPixelTransformParameterfvEXT( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glPixelTransformParameterfvEXT");
}

FABRIC_EXT_EXPORT void glPixelTransformParameteriEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPixelTransformParameteriEXT( %d, %d, %d );\n", (int)target, (int)pname, (int)param);
#endif
  glPixelTransformParameteriEXT( (GLenum)target, (GLenum)pname, (const GLint)param );
  _checkError("glPixelTransformParameteriEXT");
}

FABRIC_EXT_EXPORT void glPixelTransformParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPixelTransformParameterivEXT( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glPixelTransformParameterivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glPixelTransformParameterivEXT");
}

FABRIC_EXT_EXPORT void glPointParameterfEXT_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPointParameterfEXT( %d, %f );\n", (int)pname, (float)param);
#endif
  glPointParameterfEXT( (GLenum)pname, (GLfloat)param );
  _checkError("glPointParameterfEXT");
}

FABRIC_EXT_EXPORT void glPointParameterfvEXT_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPointParameterfvEXT( %d, GLfloat* );\n", (int)pname);
#endif
  glPointParameterfvEXT( (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glPointParameterfvEXT");
}

FABRIC_EXT_EXPORT void glPolygonOffsetEXT_wrapper(
  KL::Scalar factor,
  KL::Scalar bias
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPolygonOffsetEXT( %f, %f );\n", (float)factor, (float)bias);
#endif
  glPolygonOffsetEXT( (GLfloat)factor, (GLfloat)bias );
  _checkError("glPolygonOffsetEXT");
}

FABRIC_EXT_EXPORT void glProvokingVertexEXT_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glProvokingVertexEXT( %d );\n", (int)mode);
#endif
  glProvokingVertexEXT( (GLenum)mode );
  _checkError("glProvokingVertexEXT");
}

FABRIC_EXT_EXPORT void glBeginSceneEXT_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBeginSceneEXT(  );\n");
#endif
  glBeginSceneEXT();
  _checkError("glBeginSceneEXT");
}

FABRIC_EXT_EXPORT void glEndSceneEXT_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEndSceneEXT(  );\n");
#endif
  glEndSceneEXT();
  _checkError("glEndSceneEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3bEXT_wrapper(
  KL::Byte red,
  KL::Byte green,
  KL::Byte blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3bEXT( %d, %d, %d );\n", (int)red, (int)green, (int)blue);
#endif
  glSecondaryColor3bEXT( (GLbyte)red, (GLbyte)green, (GLbyte)blue );
  _checkError("glSecondaryColor3bEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3bvEXT_wrapper(
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3bvEXT( GLbyte* );\n");
#endif
  glSecondaryColor3bvEXT( (const GLbyte*)&v[0] );
  _checkError("glSecondaryColor3bvEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3dEXT_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3dEXT( %f, %f, %f );\n", (float)red, (float)green, (float)blue);
#endif
  glSecondaryColor3dEXT( (GLdouble)red, (GLdouble)green, (GLdouble)blue );
  _checkError("glSecondaryColor3dEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3dvEXT_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3dvEXT( GLdouble* );\n");
#endif
  glSecondaryColor3dvEXT( (const GLdouble*)&v[0] );
  _checkError("glSecondaryColor3dvEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3fEXT_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3fEXT( %f, %f, %f );\n", (float)red, (float)green, (float)blue);
#endif
  glSecondaryColor3fEXT( (GLfloat)red, (GLfloat)green, (GLfloat)blue );
  _checkError("glSecondaryColor3fEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3fvEXT_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3fvEXT( GLfloat* );\n");
#endif
  glSecondaryColor3fvEXT( (const GLfloat*)&v[0] );
  _checkError("glSecondaryColor3fvEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3iEXT_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3iEXT( %d, %d, %d );\n", (int)red, (int)green, (int)blue);
#endif
  glSecondaryColor3iEXT( (GLint)red, (GLint)green, (GLint)blue );
  _checkError("glSecondaryColor3iEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3ivEXT_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3ivEXT( GLint* );\n");
#endif
  glSecondaryColor3ivEXT( (const GLint*)&v[0] );
  _checkError("glSecondaryColor3ivEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3sEXT_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3sEXT( %d, %d, %d );\n", (int)red, (int)green, (int)blue);
#endif
  glSecondaryColor3sEXT( (GLshort)red, (GLshort)green, (GLshort)blue );
  _checkError("glSecondaryColor3sEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3svEXT_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3svEXT( GLshort* );\n");
#endif
  glSecondaryColor3svEXT( (const GLshort*)&v[0] );
  _checkError("glSecondaryColor3svEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3ubEXT_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3ubEXT( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue);
#endif
  glSecondaryColor3ubEXT( (GLubyte)red, (GLubyte)green, (GLubyte)blue );
  _checkError("glSecondaryColor3ubEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3ubvEXT_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3ubvEXT( GLubyte* );\n");
#endif
  glSecondaryColor3ubvEXT( (const GLubyte*)&v[0] );
  _checkError("glSecondaryColor3ubvEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3uiEXT_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3uiEXT( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue);
#endif
  glSecondaryColor3uiEXT( (GLuint)red, (GLuint)green, (GLuint)blue );
  _checkError("glSecondaryColor3uiEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3uivEXT_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3uivEXT( GLuint* );\n");
#endif
  glSecondaryColor3uivEXT( (const GLuint*)&v[0] );
  _checkError("glSecondaryColor3uivEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3usEXT_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3usEXT( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue);
#endif
  glSecondaryColor3usEXT( (GLushort)red, (GLushort)green, (GLushort)blue );
  _checkError("glSecondaryColor3usEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3usvEXT_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColor3usvEXT( GLushort* );\n");
#endif
  glSecondaryColor3usvEXT( (const GLushort*)&v[0] );
  _checkError("glSecondaryColor3usvEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColorPointerEXT_wrapper(
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSecondaryColorPointerEXT( %d, %d, %d, GLvoid* );\n", (int)size, (int)type, (int)stride);
#endif
  glSecondaryColorPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glSecondaryColorPointerEXT");
}

FABRIC_EXT_EXPORT void glActiveProgramEXT_wrapper(
  KL::Size program
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glActiveProgramEXT( 0x%04X );\n", (unsigned)program);
#endif
  glActiveProgramEXT( (GLuint)program );
  _checkError("glActiveProgramEXT");
}

FABRIC_EXT_EXPORT KL::Size glCreateShaderProgramEXT_wrapper(
  KL::Size type,
  const KL::String &string
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glCreateShaderProgramEXT( %d, char* );\n", (int)type);
#endif
  GLuint result = glCreateShaderProgramEXT( (GLenum)type, (const char*)string.data() );
  _checkError("glCreateShaderProgramEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glUseShaderProgramEXT_wrapper(
  KL::Size type,
  KL::Size program
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glUseShaderProgramEXT( %d, 0x%04X );\n", (int)type, (unsigned)program);
#endif
  glUseShaderProgramEXT( (GLenum)type, (GLuint)program );
  _checkError("glUseShaderProgramEXT");
}

FABRIC_EXT_EXPORT void glActiveStencilFaceEXT_wrapper(
  KL::Size face
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glActiveStencilFaceEXT( %d );\n", (int)face);
#endif
  glActiveStencilFaceEXT( (GLenum)face );
  _checkError("glActiveStencilFaceEXT");
}

FABRIC_EXT_EXPORT void glTexSubImage1DEXT_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Size width,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexSubImage1DEXT( %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)type);
#endif
  glTexSubImage1DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexSubImage1DEXT");
}

FABRIC_EXT_EXPORT void glTexSubImage2DEXT_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexSubImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)type);
#endif
  glTexSubImage2DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexSubImage2DEXT");
}

FABRIC_EXT_EXPORT void glTexSubImage3DEXT_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Integer xoffset,
  KL::Integer yoffset,
  KL::Integer zoffset,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexSubImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)type);
#endif
  glTexSubImage3DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexSubImage3DEXT");
}

FABRIC_EXT_EXPORT void glTexImage3DEXT_wrapper(
  KL::Size target,
  KL::Integer level,
  KL::Size internalformat,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Integer border,
  KL::Size format,
  KL::Size type,
  KL::Data pixels
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)format, (int)type);
#endif
  glTexImage3DEXT( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexImage3DEXT");
}

FABRIC_EXT_EXPORT void glTexBufferEXT_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Size buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexBufferEXT( %d, %d, 0x%04X );\n", (int)target, (int)internalformat, (unsigned)buffer);
#endif
  glTexBufferEXT( (GLenum)target, (GLenum)internalformat, (GLuint)buffer );
  _checkError("glTexBufferEXT");
}

FABRIC_EXT_EXPORT void glClearColorIiEXT_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue,
  KL::Integer alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClearColorIiEXT( %d, %d, %d, %d );\n", (int)red, (int)green, (int)blue, (int)alpha);
#endif
  glClearColorIiEXT( (GLint)red, (GLint)green, (GLint)blue, (GLint)alpha );
  _checkError("glClearColorIiEXT");
}

FABRIC_EXT_EXPORT void glClearColorIuiEXT_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue,
  KL::Size alpha
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glClearColorIuiEXT( 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)red, (unsigned)green, (unsigned)blue, (unsigned)alpha);
#endif
  glClearColorIuiEXT( (GLuint)red, (GLuint)green, (GLuint)blue, (GLuint)alpha );
  _checkError("glClearColorIuiEXT");
}

FABRIC_EXT_EXPORT void glGetTexParameterIivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexParameterIivEXT( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glGetTexParameterIivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetTexParameterIivEXT");
}

FABRIC_EXT_EXPORT void glGetTexParameterIuivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTexParameterIuivEXT( %d, %d, GLuint* );\n", (int)target, (int)pname);
#endif
  glGetTexParameterIuivEXT( (GLenum)target, (GLenum)pname, (GLuint*)&params[0] );
  _checkError("glGetTexParameterIuivEXT");
}

FABRIC_EXT_EXPORT void glTexParameterIivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexParameterIivEXT( %d, %d, GLint* );\n", (int)target, (int)pname);
#endif
  glTexParameterIivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glTexParameterIivEXT");
}

FABRIC_EXT_EXPORT void glTexParameterIuivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Size> & params
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexParameterIuivEXT( %d, %d, GLuint* );\n", (int)target, (int)pname);
#endif
  glTexParameterIuivEXT( (GLenum)target, (GLenum)pname, (const GLuint*)&params[0] );
  _checkError("glTexParameterIuivEXT");
}

FABRIC_EXT_EXPORT KL::Boolean glAreTexturesResidentEXT_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & textures,
  KL::VariableArray<KL::Boolean> & residences
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glAreTexturesResidentEXT( %d, GLuint*, GLboolean* );\n", (int)n);
#endif
  GLboolean result = glAreTexturesResidentEXT( (GLsizei)n, (const GLuint*)&textures[0], (GLboolean*)&residences[0] );
  _checkError("glAreTexturesResidentEXT");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glBindTextureEXT_wrapper(
  KL::Size target,
  KL::Size texture
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindTextureEXT( %d, 0x%04X );\n", (int)target, (unsigned)texture);
#endif
  glBindTextureEXT( (GLenum)target, (GLuint)texture );
  _checkError("glBindTextureEXT");
}

FABRIC_EXT_EXPORT void glDeleteTexturesEXT_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & textures
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteTexturesEXT( %d, GLuint* );\n", (int)n);
#endif
  glDeleteTexturesEXT( (GLsizei)n, (const GLuint*)&textures[0] );
  _checkError("glDeleteTexturesEXT");
}

FABRIC_EXT_EXPORT void glGenTexturesEXT_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & textures
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenTexturesEXT( %d, GLuint* );\n", (int)n);
#endif
  glGenTexturesEXT( (GLsizei)n, (GLuint*)&textures[0] );
  _checkError("glGenTexturesEXT");
}

FABRIC_EXT_EXPORT KL::Boolean glIsTextureEXT_wrapper(
  KL::Size texture
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsTextureEXT( 0x%04X );\n", (unsigned)texture);
#endif
  GLboolean result = glIsTextureEXT( (GLuint)texture );
  _checkError("glIsTextureEXT");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glPrioritizeTexturesEXT_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & textures,
  const KL::VariableArray<KL::Scalar> & priorities
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glPrioritizeTexturesEXT( %d, GLuint*, GLclampf* );\n", (int)n);
#endif
  glPrioritizeTexturesEXT( (GLsizei)n, (const GLuint*)&textures[0], (const GLclampf*)&priorities[0] );
  _checkError("glPrioritizeTexturesEXT");
}

FABRIC_EXT_EXPORT void glTextureNormalEXT_wrapper(
  KL::Size mode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTextureNormalEXT( %d );\n", (int)mode);
#endif
  glTextureNormalEXT( (GLenum)mode );
  _checkError("glTextureNormalEXT");
}

FABRIC_EXT_EXPORT void glBeginTransformFeedbackEXT_wrapper(
  KL::Size primitiveMode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBeginTransformFeedbackEXT( %d );\n", (int)primitiveMode);
#endif
  glBeginTransformFeedbackEXT( (GLenum)primitiveMode );
  _checkError("glBeginTransformFeedbackEXT");
}

FABRIC_EXT_EXPORT void glBindBufferBaseEXT_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Size buffer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindBufferBaseEXT( %d, 0x%04X, 0x%04X );\n", (int)target, (unsigned)index, (unsigned)buffer);
#endif
  glBindBufferBaseEXT( (GLenum)target, (GLuint)index, (GLuint)buffer );
  _checkError("glBindBufferBaseEXT");
}

FABRIC_EXT_EXPORT void glBindBufferOffsetEXT_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Size buffer,
  KL::Integer offset
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindBufferOffsetEXT( %d, 0x%04X, 0x%04X, %d );\n", (int)target, (unsigned)index, (unsigned)buffer, (int)offset);
#endif
  glBindBufferOffsetEXT( (GLenum)target, (GLuint)index, (GLuint)buffer, (GLintptr)offset );
  _checkError("glBindBufferOffsetEXT");
}

FABRIC_EXT_EXPORT void glBindBufferRangeEXT_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Size buffer,
  KL::Integer offset,
  KL::Size size
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindBufferRangeEXT( %d, 0x%04X, 0x%04X, %d, %d );\n", (int)target, (unsigned)index, (unsigned)buffer, (int)offset, (int)size);
#endif
  glBindBufferRangeEXT( (GLenum)target, (GLuint)index, (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size );
  _checkError("glBindBufferRangeEXT");
}

FABRIC_EXT_EXPORT void glEndTransformFeedbackEXT_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEndTransformFeedbackEXT(  );\n");
#endif
  glEndTransformFeedbackEXT();
  _checkError("glEndTransformFeedbackEXT");
}

FABRIC_EXT_EXPORT void glGetTransformFeedbackVaryingEXT_wrapper(
  KL::Size program,
  KL::Size index,
  KL::Size bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::VariableArray<KL::Size> & size,
  KL::VariableArray<KL::Size> & type,
  KL::String & name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetTransformFeedbackVaryingEXT( 0x%04X, 0x%04X, %d, GLsizei*, GLsizei*, GLenum*, char* );\n", (unsigned)program, (unsigned)index, (int)bufSize);
#endif
  char * nameStr = new char[1024];
  glGetTransformFeedbackVaryingEXT( (GLuint)program, (GLuint)index, (GLsizei)bufSize, (GLsizei*)&length[0], (GLsizei*)&size[0], (GLenum*)&type[0], (char*)nameStr );
  _checkError("glGetTransformFeedbackVaryingEXT");
  name = KL::String((const char*)nameStr);
  delete( nameStr );
}

FABRIC_EXT_EXPORT void glTransformFeedbackVaryingsEXT_wrapper(
  KL::Size program,
  KL::Size count,
  const KL::VariableArray<KL::String> &varyings,
  KL::Size bufferMode
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTransformFeedbackVaryingsEXT( 0x%04X, %d, char*, %d );\n", (unsigned)program, (int)count, (int)bufferMode);
#endif
  KL::VariableArray<KL::Data> varyingsData;
  varyingsData.resize(varyings.size());
  for(KL::Size varyingsCounter = 0;varyingsCounter<varyings.size();varyingsCounter++)
    varyingsData[varyingsCounter] = (KL::Data)varyings[varyingsCounter].data();
  glTransformFeedbackVaryingsEXT( (GLuint)program, (GLsizei)count, (const char**)&varyingsData[0], (GLenum)bufferMode );
  _checkError("glTransformFeedbackVaryingsEXT");
}

FABRIC_EXT_EXPORT void glArrayElementEXT_wrapper(
  KL::Integer i
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glArrayElementEXT( %d );\n", (int)i);
#endif
  glArrayElementEXT( (GLint)i );
  _checkError("glArrayElementEXT");
}

FABRIC_EXT_EXPORT void glColorPointerEXT_wrapper(
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Size count,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glColorPointerEXT( %d, %d, %d, %d, void* );\n", (int)size, (int)type, (int)stride, (int)count);
#endif
  glColorPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
  _checkError("glColorPointerEXT");
}

FABRIC_EXT_EXPORT void glDrawArraysEXT_wrapper(
  KL::Size mode,
  KL::Integer first,
  KL::Size count
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawArraysEXT( %d, %d, %d );\n", (int)mode, (int)first, (int)count);
#endif
  glDrawArraysEXT( (GLenum)mode, (GLint)first, (GLsizei)count );
  _checkError("glDrawArraysEXT");
}

FABRIC_EXT_EXPORT void glEdgeFlagPointerEXT_wrapper(
  KL::Size stride,
  KL::Size count,
  const KL::VariableArray<KL::Boolean> & pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEdgeFlagPointerEXT( %d, %d, GLboolean* );\n", (int)stride, (int)count);
#endif
  glEdgeFlagPointerEXT( (GLsizei)stride, (GLsizei)count, (const GLboolean*)&pointer[0] );
  _checkError("glEdgeFlagPointerEXT");
}

FABRIC_EXT_EXPORT void glIndexPointerEXT_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Size count,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIndexPointerEXT( %d, %d, %d, void* );\n", (int)type, (int)stride, (int)count);
#endif
  glIndexPointerEXT( (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
  _checkError("glIndexPointerEXT");
}

FABRIC_EXT_EXPORT void glNormalPointerEXT_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Size count,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNormalPointerEXT( %d, %d, %d, void* );\n", (int)type, (int)stride, (int)count);
#endif
  glNormalPointerEXT( (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
  _checkError("glNormalPointerEXT");
}

FABRIC_EXT_EXPORT void glTexCoordPointerEXT_wrapper(
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Size count,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glTexCoordPointerEXT( %d, %d, %d, %d, void* );\n", (int)size, (int)type, (int)stride, (int)count);
#endif
  glTexCoordPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
  _checkError("glTexCoordPointerEXT");
}

FABRIC_EXT_EXPORT void glVertexPointerEXT_wrapper(
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Size count,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexPointerEXT( %d, %d, %d, %d, void* );\n", (int)size, (int)type, (int)stride, (int)count);
#endif
  glVertexPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
  _checkError("glVertexPointerEXT");
}

FABRIC_EXT_EXPORT void glBeginVertexShaderEXT_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBeginVertexShaderEXT(  );\n");
#endif
  glBeginVertexShaderEXT();
  _checkError("glBeginVertexShaderEXT");
}

FABRIC_EXT_EXPORT KL::Size glBindLightParameterEXT_wrapper(
  KL::Size light,
  KL::Size value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindLightParameterEXT( %d, %d );\n", (int)light, (int)value);
#endif
  GLuint result = glBindLightParameterEXT( (GLenum)light, (GLenum)value );
  _checkError("glBindLightParameterEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size glBindMaterialParameterEXT_wrapper(
  KL::Size face,
  KL::Size value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindMaterialParameterEXT( %d, %d );\n", (int)face, (int)value);
#endif
  GLuint result = glBindMaterialParameterEXT( (GLenum)face, (GLenum)value );
  _checkError("glBindMaterialParameterEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size glBindParameterEXT_wrapper(
  KL::Size value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindParameterEXT( %d );\n", (int)value);
#endif
  GLuint result = glBindParameterEXT( (GLenum)value );
  _checkError("glBindParameterEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size glBindTexGenParameterEXT_wrapper(
  KL::Size unit,
  KL::Size coord,
  KL::Size value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindTexGenParameterEXT( %d, %d, %d );\n", (int)unit, (int)coord, (int)value);
#endif
  GLuint result = glBindTexGenParameterEXT( (GLenum)unit, (GLenum)coord, (GLenum)value );
  _checkError("glBindTexGenParameterEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size glBindTextureUnitParameterEXT_wrapper(
  KL::Size unit,
  KL::Size value
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindTextureUnitParameterEXT( %d, %d );\n", (int)unit, (int)value);
#endif
  GLuint result = glBindTextureUnitParameterEXT( (GLenum)unit, (GLenum)value );
  _checkError("glBindTextureUnitParameterEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glBindVertexShaderEXT_wrapper(
  KL::Size id
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBindVertexShaderEXT( 0x%04X );\n", (unsigned)id);
#endif
  glBindVertexShaderEXT( (GLuint)id );
  _checkError("glBindVertexShaderEXT");
}

FABRIC_EXT_EXPORT void glDeleteVertexShaderEXT_wrapper(
  KL::Size id
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteVertexShaderEXT( 0x%04X );\n", (unsigned)id);
#endif
  glDeleteVertexShaderEXT( (GLuint)id );
  _checkError("glDeleteVertexShaderEXT");
}

FABRIC_EXT_EXPORT void glDisableVariantClientStateEXT_wrapper(
  KL::Size id
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDisableVariantClientStateEXT( 0x%04X );\n", (unsigned)id);
#endif
  glDisableVariantClientStateEXT( (GLuint)id );
  _checkError("glDisableVariantClientStateEXT");
}

FABRIC_EXT_EXPORT void glEnableVariantClientStateEXT_wrapper(
  KL::Size id
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEnableVariantClientStateEXT( 0x%04X );\n", (unsigned)id);
#endif
  glEnableVariantClientStateEXT( (GLuint)id );
  _checkError("glEnableVariantClientStateEXT");
}

FABRIC_EXT_EXPORT void glEndVertexShaderEXT_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glEndVertexShaderEXT(  );\n");
#endif
  glEndVertexShaderEXT();
  _checkError("glEndVertexShaderEXT");
}

FABRIC_EXT_EXPORT void glExtractComponentEXT_wrapper(
  KL::Size res,
  KL::Size src,
  KL::Size num
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glExtractComponentEXT( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)res, (unsigned)src, (unsigned)num);
#endif
  glExtractComponentEXT( (GLuint)res, (GLuint)src, (GLuint)num );
  _checkError("glExtractComponentEXT");
}

FABRIC_EXT_EXPORT KL::Size glGenSymbolsEXT_wrapper(
  KL::Size dataType,
  KL::Size storageType,
  KL::Size range,
  KL::Size components
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenSymbolsEXT( %d, %d, %d, 0x%04X );\n", (int)dataType, (int)storageType, (int)range, (unsigned)components);
#endif
  GLuint result = glGenSymbolsEXT( (GLenum)dataType, (GLenum)storageType, (GLenum)range, (GLuint)components );
  _checkError("glGenSymbolsEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size glGenVertexShadersEXT_wrapper(
  KL::Size range
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGenVertexShadersEXT( 0x%04X );\n", (unsigned)range);
#endif
  GLuint result = glGenVertexShadersEXT( (GLuint)range );
  _checkError("glGenVertexShadersEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glGetInvariantBooleanvEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Boolean> & data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetInvariantBooleanvEXT( 0x%04X, %d, GLboolean* );\n", (unsigned)id, (int)value);
#endif
  glGetInvariantBooleanvEXT( (GLuint)id, (GLenum)value, (GLboolean*)&data[0] );
  _checkError("glGetInvariantBooleanvEXT");
}

FABRIC_EXT_EXPORT void glGetInvariantFloatvEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Scalar> & data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetInvariantFloatvEXT( 0x%04X, %d, GLfloat* );\n", (unsigned)id, (int)value);
#endif
  glGetInvariantFloatvEXT( (GLuint)id, (GLenum)value, (GLfloat*)&data[0] );
  _checkError("glGetInvariantFloatvEXT");
}

FABRIC_EXT_EXPORT void glGetInvariantIntegervEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Integer> & data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetInvariantIntegervEXT( 0x%04X, %d, GLint* );\n", (unsigned)id, (int)value);
#endif
  glGetInvariantIntegervEXT( (GLuint)id, (GLenum)value, (GLint*)&data[0] );
  _checkError("glGetInvariantIntegervEXT");
}

FABRIC_EXT_EXPORT void glGetLocalConstantBooleanvEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Boolean> & data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetLocalConstantBooleanvEXT( 0x%04X, %d, GLboolean* );\n", (unsigned)id, (int)value);
#endif
  glGetLocalConstantBooleanvEXT( (GLuint)id, (GLenum)value, (GLboolean*)&data[0] );
  _checkError("glGetLocalConstantBooleanvEXT");
}

FABRIC_EXT_EXPORT void glGetLocalConstantFloatvEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Scalar> & data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetLocalConstantFloatvEXT( 0x%04X, %d, GLfloat* );\n", (unsigned)id, (int)value);
#endif
  glGetLocalConstantFloatvEXT( (GLuint)id, (GLenum)value, (GLfloat*)&data[0] );
  _checkError("glGetLocalConstantFloatvEXT");
}

FABRIC_EXT_EXPORT void glGetLocalConstantIntegervEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Integer> & data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetLocalConstantIntegervEXT( 0x%04X, %d, GLint* );\n", (unsigned)id, (int)value);
#endif
  glGetLocalConstantIntegervEXT( (GLuint)id, (GLenum)value, (GLint*)&data[0] );
  _checkError("glGetLocalConstantIntegervEXT");
}

FABRIC_EXT_EXPORT void glGetVariantBooleanvEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Boolean> & data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVariantBooleanvEXT( 0x%04X, %d, GLboolean* );\n", (unsigned)id, (int)value);
#endif
  glGetVariantBooleanvEXT( (GLuint)id, (GLenum)value, (GLboolean*)&data[0] );
  _checkError("glGetVariantBooleanvEXT");
}

FABRIC_EXT_EXPORT void glGetVariantFloatvEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Scalar> & data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVariantFloatvEXT( 0x%04X, %d, GLfloat* );\n", (unsigned)id, (int)value);
#endif
  glGetVariantFloatvEXT( (GLuint)id, (GLenum)value, (GLfloat*)&data[0] );
  _checkError("glGetVariantFloatvEXT");
}

FABRIC_EXT_EXPORT void glGetVariantIntegervEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Integer> & data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glGetVariantIntegervEXT( 0x%04X, %d, GLint* );\n", (unsigned)id, (int)value);
#endif
  glGetVariantIntegervEXT( (GLuint)id, (GLenum)value, (GLint*)&data[0] );
  _checkError("glGetVariantIntegervEXT");
}

FABRIC_EXT_EXPORT void glInsertComponentEXT_wrapper(
  KL::Size res,
  KL::Size src,
  KL::Size num
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glInsertComponentEXT( 0x%04X, 0x%04X, 0x%04X );\n", (unsigned)res, (unsigned)src, (unsigned)num);
#endif
  glInsertComponentEXT( (GLuint)res, (GLuint)src, (GLuint)num );
  _checkError("glInsertComponentEXT");
}

FABRIC_EXT_EXPORT KL::Boolean glIsVariantEnabledEXT_wrapper(
  KL::Size id,
  KL::Size cap
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glIsVariantEnabledEXT( 0x%04X, %d );\n", (unsigned)id, (int)cap);
#endif
  GLboolean result = glIsVariantEnabledEXT( (GLuint)id, (GLenum)cap );
  _checkError("glIsVariantEnabledEXT");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glSetInvariantEXT_wrapper(
  KL::Size id,
  KL::Size type,
  KL::Data addr
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSetInvariantEXT( 0x%04X, %d, GLvoid* );\n", (unsigned)id, (int)type);
#endif
  glSetInvariantEXT( (GLuint)id, (GLenum)type, addr );
  _checkError("glSetInvariantEXT");
}

FABRIC_EXT_EXPORT void glSetLocalConstantEXT_wrapper(
  KL::Size id,
  KL::Size type,
  KL::Data addr
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSetLocalConstantEXT( 0x%04X, %d, GLvoid* );\n", (unsigned)id, (int)type);
#endif
  glSetLocalConstantEXT( (GLuint)id, (GLenum)type, addr );
  _checkError("glSetLocalConstantEXT");
}

FABRIC_EXT_EXPORT void glShaderOp1EXT_wrapper(
  KL::Size op,
  KL::Size res,
  KL::Size arg1
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glShaderOp1EXT( %d, 0x%04X, 0x%04X );\n", (int)op, (unsigned)res, (unsigned)arg1);
#endif
  glShaderOp1EXT( (GLenum)op, (GLuint)res, (GLuint)arg1 );
  _checkError("glShaderOp1EXT");
}

FABRIC_EXT_EXPORT void glShaderOp2EXT_wrapper(
  KL::Size op,
  KL::Size res,
  KL::Size arg1,
  KL::Size arg2
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glShaderOp2EXT( %d, 0x%04X, 0x%04X, 0x%04X );\n", (int)op, (unsigned)res, (unsigned)arg1, (unsigned)arg2);
#endif
  glShaderOp2EXT( (GLenum)op, (GLuint)res, (GLuint)arg1, (GLuint)arg2 );
  _checkError("glShaderOp2EXT");
}

FABRIC_EXT_EXPORT void glShaderOp3EXT_wrapper(
  KL::Size op,
  KL::Size res,
  KL::Size arg1,
  KL::Size arg2,
  KL::Size arg3
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glShaderOp3EXT( %d, 0x%04X, 0x%04X, 0x%04X, 0x%04X );\n", (int)op, (unsigned)res, (unsigned)arg1, (unsigned)arg2, (unsigned)arg3);
#endif
  glShaderOp3EXT( (GLenum)op, (GLuint)res, (GLuint)arg1, (GLuint)arg2, (GLuint)arg3 );
  _checkError("glShaderOp3EXT");
}

FABRIC_EXT_EXPORT void glSwizzleEXT_wrapper(
  KL::Size res,
  KL::Size in,
  KL::Size outX,
  KL::Size outY,
  KL::Size outZ,
  KL::Size outW
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glSwizzleEXT( 0x%04X, 0x%04X, %d, %d, %d, %d );\n", (unsigned)res, (unsigned)in, (int)outX, (int)outY, (int)outZ, (int)outW);
#endif
  glSwizzleEXT( (GLuint)res, (GLuint)in, (GLenum)outX, (GLenum)outY, (GLenum)outZ, (GLenum)outW );
  _checkError("glSwizzleEXT");
}

FABRIC_EXT_EXPORT void glVariantPointerEXT_wrapper(
  KL::Size id,
  KL::Size type,
  KL::Size stride,
  KL::Data addr
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVariantPointerEXT( 0x%04X, %d, 0x%04X, GLvoid* );\n", (unsigned)id, (int)type, (unsigned)stride);
#endif
  glVariantPointerEXT( (GLuint)id, (GLenum)type, (GLuint)stride, addr );
  _checkError("glVariantPointerEXT");
}

FABRIC_EXT_EXPORT void glVariantbvEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Byte> & addr
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVariantbvEXT( 0x%04X, GLbyte* );\n", (unsigned)id);
#endif
  glVariantbvEXT( (GLuint)id, (GLbyte*)&addr[0] );
  _checkError("glVariantbvEXT");
}

FABRIC_EXT_EXPORT void glVariantdvEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Scalar> & addr
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVariantdvEXT( 0x%04X, GLdouble* );\n", (unsigned)id);
#endif
  glVariantdvEXT( (GLuint)id, (GLdouble*)&addr[0] );
  _checkError("glVariantdvEXT");
}

FABRIC_EXT_EXPORT void glVariantfvEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Scalar> & addr
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVariantfvEXT( 0x%04X, GLfloat* );\n", (unsigned)id);
#endif
  glVariantfvEXT( (GLuint)id, (GLfloat*)&addr[0] );
  _checkError("glVariantfvEXT");
}

FABRIC_EXT_EXPORT void glVariantivEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Integer> & addr
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVariantivEXT( 0x%04X, GLint* );\n", (unsigned)id);
#endif
  glVariantivEXT( (GLuint)id, (GLint*)&addr[0] );
  _checkError("glVariantivEXT");
}

FABRIC_EXT_EXPORT void glVariantsvEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Integer> & addr
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVariantsvEXT( 0x%04X, GLshort* );\n", (unsigned)id);
#endif
  glVariantsvEXT( (GLuint)id, (GLshort*)&addr[0] );
  _checkError("glVariantsvEXT");
}

FABRIC_EXT_EXPORT void glVariantubvEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Size> & addr
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVariantubvEXT( 0x%04X, GLubyte* );\n", (unsigned)id);
#endif
  glVariantubvEXT( (GLuint)id, (GLubyte*)&addr[0] );
  _checkError("glVariantubvEXT");
}

FABRIC_EXT_EXPORT void glVariantuivEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Size> & addr
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVariantuivEXT( 0x%04X, GLuint* );\n", (unsigned)id);
#endif
  glVariantuivEXT( (GLuint)id, (GLuint*)&addr[0] );
  _checkError("glVariantuivEXT");
}

FABRIC_EXT_EXPORT void glVariantusvEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Integer> & addr
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVariantusvEXT( 0x%04X, GLushort* );\n", (unsigned)id);
#endif
  glVariantusvEXT( (GLuint)id, (GLushort*)&addr[0] );
  _checkError("glVariantusvEXT");
}

FABRIC_EXT_EXPORT void glWriteMaskEXT_wrapper(
  KL::Size res,
  KL::Size in,
  KL::Size outX,
  KL::Size outY,
  KL::Size outZ,
  KL::Size outW
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glWriteMaskEXT( 0x%04X, 0x%04X, %d, %d, %d, %d );\n", (unsigned)res, (unsigned)in, (int)outX, (int)outY, (int)outZ, (int)outW);
#endif
  glWriteMaskEXT( (GLuint)res, (GLuint)in, (GLenum)outX, (GLenum)outY, (GLenum)outZ, (GLenum)outW );
  _checkError("glWriteMaskEXT");
}

FABRIC_EXT_EXPORT void glVertexWeightPointerEXT_wrapper(
  KL::Integer size,
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexWeightPointerEXT( %d, %d, %d, void* );\n", (int)size, (int)type, (int)stride);
#endif
  glVertexWeightPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glVertexWeightPointerEXT");
}

FABRIC_EXT_EXPORT void glVertexWeightfEXT_wrapper(
  KL::Scalar weight
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexWeightfEXT( %f );\n", (float)weight);
#endif
  glVertexWeightfEXT( (GLfloat)weight );
  _checkError("glVertexWeightfEXT");
}

FABRIC_EXT_EXPORT void glVertexWeightfvEXT_wrapper(
  KL::VariableArray<KL::Scalar> & weight
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glVertexWeightfvEXT( GLfloat* );\n");
#endif
  glVertexWeightfvEXT( (GLfloat*)&weight[0] );
  _checkError("glVertexWeightfvEXT");
}

FABRIC_EXT_EXPORT KL::Size glBufferRegionEnabledEXT_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glBufferRegionEnabledEXT(  );\n");
#endif
  GLuint result = glBufferRegionEnabledEXT();
  _checkError("glBufferRegionEnabledEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glDeleteBufferRegionEXT_wrapper(
  KL::Size region
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDeleteBufferRegionEXT( %d );\n", (int)region);
#endif
  glDeleteBufferRegionEXT( (GLenum)region );
  _checkError("glDeleteBufferRegionEXT");
}

FABRIC_EXT_EXPORT void glDrawBufferRegionEXT_wrapper(
  KL::Size region,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height,
  KL::Integer xDest,
  KL::Integer yDest
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glDrawBufferRegionEXT( 0x%04X, %d, %d, %d, %d, %d, %d );\n", (unsigned)region, (int)x, (int)y, (int)width, (int)height, (int)xDest, (int)yDest);
#endif
  glDrawBufferRegionEXT( (GLuint)region, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)xDest, (GLint)yDest );
  _checkError("glDrawBufferRegionEXT");
}

FABRIC_EXT_EXPORT KL::Size glNewBufferRegionEXT_wrapper(
  KL::Size region
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glNewBufferRegionEXT( %d );\n", (int)region);
#endif
  GLuint result = glNewBufferRegionEXT( (GLenum)region );
  _checkError("glNewBufferRegionEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glReadBufferRegionEXT_wrapper(
  KL::Size region,
  KL::Integer x,
  KL::Integer y,
  KL::Size width,
  KL::Size height
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glReadBufferRegionEXT( 0x%04X, %d, %d, %d, %d );\n", (unsigned)region, (int)x, (int)y, (int)width, (int)height);
#endif
  glReadBufferRegionEXT( (GLuint)region, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glReadBufferRegionEXT");
}

FABRIC_EXT_EXPORT void glXWaitGL_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glXWaitGL(  );\n");
#endif
  glXWaitGL();
  _checkError("glXWaitGL");
}

FABRIC_EXT_EXPORT void glXWaitX_wrapper()
{
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glXWaitX(  );\n");
#endif
  glXWaitX();
  _checkError("glXWaitX");
}

FABRIC_EXT_EXPORT KL::Boolean glxewIsSupported_wrapper(
  const KL::String &name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glxewIsSupported( char* );\n");
#endif
  GLboolean result = glxewIsSupported( (const char*)name.data() );
  _checkError("glxewIsSupported");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glxewGetExtension_wrapper(
  const KL::String &name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("glxewGetExtension( char* );\n");
#endif
  GLboolean result = glxewGetExtension( (const char*)name.data() );
  _checkError("glxewGetExtension");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Integer gluBuild1DMipmapLevels_wrapper(
  KL::Size target,
  KL::Integer internalFormat,
  KL::Size width,
  KL::Size format,
  KL::Size type,
  KL::Integer level,
  KL::Integer base,
  KL::Integer max,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluBuild1DMipmapLevels( %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)internalFormat, (int)width, (int)format, (int)type, (int)level, (int)base, (int)max);
#endif
  GLint result = gluBuild1DMipmapLevels( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLenum)format, (GLenum)type, (GLint)level, (GLint)base, (GLint)max, data );
  _checkError("gluBuild1DMipmapLevels");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer gluBuild1DMipmaps_wrapper(
  KL::Size target,
  KL::Integer internalFormat,
  KL::Size width,
  KL::Size format,
  KL::Size type,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluBuild1DMipmaps( %d, %d, %d, %d, %d, void* );\n", (int)target, (int)internalFormat, (int)width, (int)format, (int)type);
#endif
  GLint result = gluBuild1DMipmaps( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLenum)format, (GLenum)type, data );
  _checkError("gluBuild1DMipmaps");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer gluBuild2DMipmapLevels_wrapper(
  KL::Size target,
  KL::Integer internalFormat,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size type,
  KL::Integer level,
  KL::Integer base,
  KL::Integer max,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluBuild2DMipmapLevels( %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)internalFormat, (int)width, (int)height, (int)format, (int)type, (int)level, (int)base, (int)max);
#endif
  GLint result = gluBuild2DMipmapLevels( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, (GLint)level, (GLint)base, (GLint)max, data );
  _checkError("gluBuild2DMipmapLevels");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer gluBuild2DMipmaps_wrapper(
  KL::Size target,
  KL::Integer internalFormat,
  KL::Size width,
  KL::Size height,
  KL::Size format,
  KL::Size type,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluBuild2DMipmaps( %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)internalFormat, (int)width, (int)height, (int)format, (int)type);
#endif
  GLint result = gluBuild2DMipmaps( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, data );
  _checkError("gluBuild2DMipmaps");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer gluBuild3DMipmapLevels_wrapper(
  KL::Size target,
  KL::Integer internalFormat,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Size format,
  KL::Size type,
  KL::Integer level,
  KL::Integer base,
  KL::Integer max,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluBuild3DMipmapLevels( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)internalFormat, (int)width, (int)height, (int)depth, (int)format, (int)type, (int)level, (int)base, (int)max);
#endif
  GLint result = gluBuild3DMipmapLevels( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, (GLint)level, (GLint)base, (GLint)max, data );
  _checkError("gluBuild3DMipmapLevels");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer gluBuild3DMipmaps_wrapper(
  KL::Size target,
  KL::Integer internalFormat,
  KL::Size width,
  KL::Size height,
  KL::Size depth,
  KL::Size format,
  KL::Size type,
  KL::Data data
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluBuild3DMipmaps( %d, %d, %d, %d, %d, %d, %d, void* );\n", (int)target, (int)internalFormat, (int)width, (int)height, (int)depth, (int)format, (int)type);
#endif
  GLint result = gluBuild3DMipmaps( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, data );
  _checkError("gluBuild3DMipmaps");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Boolean gluCheckExtension_wrapper(
  const KL::VariableArray<KL::Size> & extName,
  const KL::VariableArray<KL::Size> & extString
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluCheckExtension( GLubyte*, GLubyte* );\n");
#endif
  GLboolean result = gluCheckExtension( (const GLubyte*)&extName[0], (const GLubyte*)&extString[0] );
  _checkError("gluCheckExtension");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::String gluErrorString_wrapper(
  KL::Size error
){
#ifdef FABRIC_OGL_DEBUG
  printf("gluErrorString( %d );\n", (int)error);
#endif
  const GLubyte* result = gluErrorString( (GLenum)error );
  return (KL::String)(const char*)result;
}

FABRIC_EXT_EXPORT KL::String gluGetString_wrapper(
  KL::Size name
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluGetString( %d );\n", (int)name);
#endif
  const GLubyte* result = gluGetString( (GLenum)name );
  _checkError("gluGetString");
  return (KL::String)(const char*)result;
}

FABRIC_EXT_EXPORT void gluLookAt_wrapper(
  KL::Scalar eyeX,
  KL::Scalar eyeY,
  KL::Scalar eyeZ,
  KL::Scalar centerX,
  KL::Scalar centerY,
  KL::Scalar centerZ,
  KL::Scalar upX,
  KL::Scalar upY,
  KL::Scalar upZ
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluLookAt( %f, %f, %f, %f, %f, %f, %f, %f, %f );\n", (float)eyeX, (float)eyeY, (float)eyeZ, (float)centerX, (float)centerY, (float)centerZ, (float)upX, (float)upY, (float)upZ);
#endif
  gluLookAt( (GLdouble)eyeX, (GLdouble)eyeY, (GLdouble)eyeZ, (GLdouble)centerX, (GLdouble)centerY, (GLdouble)centerZ, (GLdouble)upX, (GLdouble)upY, (GLdouble)upZ );
  _checkError("gluLookAt");
}

FABRIC_EXT_EXPORT void gluOrtho2D_wrapper(
  KL::Scalar left,
  KL::Scalar right,
  KL::Scalar bottom,
  KL::Scalar top
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluOrtho2D( %f, %f, %f, %f );\n", (float)left, (float)right, (float)bottom, (float)top);
#endif
  gluOrtho2D( (GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top );
  _checkError("gluOrtho2D");
}

FABRIC_EXT_EXPORT void gluPerspective_wrapper(
  KL::Scalar fovy,
  KL::Scalar aspect,
  KL::Scalar zNear,
  KL::Scalar zFar
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluPerspective( %f, %f, %f, %f );\n", (float)fovy, (float)aspect, (float)zNear, (float)zFar);
#endif
  gluPerspective( (GLdouble)fovy, (GLdouble)aspect, (GLdouble)zNear, (GLdouble)zFar );
  _checkError("gluPerspective");
}

FABRIC_EXT_EXPORT void gluPickMatrix_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar delX,
  KL::Scalar delY,
  KL::VariableArray<KL::Integer> & viewport
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluPickMatrix( %f, %f, %f, %f, GLint* );\n", (float)x, (float)y, (float)delX, (float)delY);
#endif
  gluPickMatrix( (GLdouble)x, (GLdouble)y, (GLdouble)delX, (GLdouble)delY, (GLint*)&viewport[0] );
  _checkError("gluPickMatrix");
}

FABRIC_EXT_EXPORT KL::Integer gluProject_wrapper(
  KL::Scalar objX,
  KL::Scalar objY,
  KL::Scalar objZ,
  const KL::VariableArray<KL::Scalar> & model,
  const KL::VariableArray<KL::Scalar> & proj,
  const KL::VariableArray<KL::Integer> & view,
  KL::VariableArray<KL::Scalar> & winX,
  KL::VariableArray<KL::Scalar> & winY,
  KL::VariableArray<KL::Scalar> & winZ
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluProject( %f, %f, %f, GLdouble*, GLdouble*, GLint*, GLdouble*, GLdouble*, GLdouble* );\n", (float)objX, (float)objY, (float)objZ);
#endif
  GLint result = gluProject( (GLdouble)objX, (GLdouble)objY, (GLdouble)objZ, (const GLdouble*)&model[0], (const GLdouble*)&proj[0], (const GLint*)&view[0], (GLdouble*)&winX[0], (GLdouble*)&winY[0], (GLdouble*)&winZ[0] );
  _checkError("gluProject");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer gluScaleImage_wrapper(
  KL::Size format,
  KL::Size wIn,
  KL::Size hIn,
  KL::Size typeIn,
  KL::Data dataIn,
  KL::Size wOut,
  KL::Size hOut,
  KL::Size typeOut,
  KL::Data dataOut
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluScaleImage( %d, %d, %d, %d, void*, %d, %d, %d, GLvoid* );\n", (int)format, (int)wIn, (int)hIn, (int)typeIn, (int)wOut, (int)hOut, (int)typeOut);
#endif
  GLint result = gluScaleImage( (GLenum)format, (GLsizei)wIn, (GLsizei)hIn, (GLenum)typeIn, dataIn, (GLsizei)wOut, (GLsizei)hOut, (GLenum)typeOut, dataOut );
  _checkError("gluScaleImage");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer gluUnProject_wrapper(
  KL::Scalar winX,
  KL::Scalar winY,
  KL::Scalar winZ,
  const KL::VariableArray<KL::Scalar> & model,
  const KL::VariableArray<KL::Scalar> & proj,
  const KL::VariableArray<KL::Integer> & view,
  KL::VariableArray<KL::Scalar> & objX,
  KL::VariableArray<KL::Scalar> & objY,
  KL::VariableArray<KL::Scalar> & objZ
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluUnProject( %f, %f, %f, GLdouble*, GLdouble*, GLint*, GLdouble*, GLdouble*, GLdouble* );\n", (float)winX, (float)winY, (float)winZ);
#endif
  GLint result = gluUnProject( (GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, (const GLdouble*)&model[0], (const GLdouble*)&proj[0], (const GLint*)&view[0], (GLdouble*)&objX[0], (GLdouble*)&objY[0], (GLdouble*)&objZ[0] );
  _checkError("gluUnProject");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer gluUnProject4_wrapper(
  KL::Scalar winX,
  KL::Scalar winY,
  KL::Scalar winZ,
  KL::Scalar clipW,
  const KL::VariableArray<KL::Scalar> & model,
  const KL::VariableArray<KL::Scalar> & proj,
  const KL::VariableArray<KL::Integer> & view,
  KL::Scalar nearVal,
  KL::Scalar farVal,
  KL::VariableArray<KL::Scalar> & objX,
  KL::VariableArray<KL::Scalar> & objY,
  KL::VariableArray<KL::Scalar> & objZ,
  KL::VariableArray<KL::Scalar> & objW
){
  _clearError();
#ifdef FABRIC_OGL_DEBUG
  printf("gluUnProject4( %f, %f, %f, %f, GLdouble*, GLdouble*, GLint*, %f, %f, GLdouble*, GLdouble*, GLdouble*, GLdouble* );\n", (float)winX, (float)winY, (float)winZ, (float)clipW, (float)nearVal, (float)farVal);
#endif
  GLint result = gluUnProject4( (GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, (GLdouble)clipW, (const GLdouble*)&model[0], (const GLdouble*)&proj[0], (const GLint*)&view[0], (GLdouble)nearVal, (GLdouble)farVal, (GLdouble*)&objX[0], (GLdouble*)&objY[0], (GLdouble*)&objZ[0], (GLdouble*)&objW[0] );
  _checkError("gluUnProject4");
  return (KL::Integer)result;
}

