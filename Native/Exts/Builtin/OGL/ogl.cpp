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

unsigned int gBracket = 0;
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
    throw("Fabric::OGL::%s: %s",functionName,(const char*)gluErrorString(error));
}

FABRIC_EXT_EXPORT void glAccum_wrapper(
  KL::Size op,
  KL::Scalar value
){
  _clearError();
  glAccum( (GLenum)op, (GLfloat)value );
  _checkError("glAccum");
}

FABRIC_EXT_EXPORT void glAlphaFunc_wrapper(
  KL::Size func,
  KL::Scalar ref
){
  _clearError();
  glAlphaFunc( (GLenum)func, (GLclampf)ref );
  _checkError("glAlphaFunc");
}

FABRIC_EXT_EXPORT KL::Boolean glAreTexturesResident_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & textures,
  KL::VariableArray<KL::Boolean> & residences
){
  _clearError();
  GLboolean result = glAreTexturesResident( (GLsizei)n, (const GLuint*)&textures[0], (GLboolean*)&residences[0] );
  _checkError("glAreTexturesResident");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glArrayElement_wrapper(
  KL::Integer i
){
  _clearError();
  glArrayElement( (GLint)i );
  _checkError("glArrayElement");
}

FABRIC_EXT_EXPORT void glBegin_wrapper(
  KL::Size mode
){
  _clearError();
  _incBracket();
  glBegin( (GLenum)mode );
  _checkError("glBegin");
}

FABRIC_EXT_EXPORT void glBindTexture_wrapper(
  KL::Size target,
  KL::Size texture
){
  _clearError();
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
  glBitmap( (GLsizei)width, (GLsizei)height, (GLfloat)xorig, (GLfloat)yorig, (GLfloat)xmove, (GLfloat)ymove, (const GLubyte*)&bitmap[0] );
  _checkError("glBitmap");
}

FABRIC_EXT_EXPORT void glBlendFunc_wrapper(
  KL::Size sfactor,
  KL::Size dfactor
){
  _clearError();
  glBlendFunc( (GLenum)sfactor, (GLenum)dfactor );
  _checkError("glBlendFunc");
}

FABRIC_EXT_EXPORT void glCallList_wrapper(
  KL::Size list
){
  _clearError();
  glCallList( (GLuint)list );
  _checkError("glCallList");
}

FABRIC_EXT_EXPORT void glCallLists_wrapper(
  KL::Size n,
  KL::Size type,
  KL::Data lists
){
  _clearError();
  glCallLists( (GLsizei)n, (GLenum)type, lists );
  _checkError("glCallLists");
}

FABRIC_EXT_EXPORT void glClear_wrapper(
  KL::Size mask
){
  _clearError();
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
  glClearColor( (GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha );
  _checkError("glClearColor");
}

FABRIC_EXT_EXPORT void glClearDepth_wrapper(
  KL::Scalar depth
){
  _clearError();
  glClearDepth( (GLclampd)depth );
  _checkError("glClearDepth");
}

FABRIC_EXT_EXPORT void glClearIndex_wrapper(
  KL::Scalar c
){
  _clearError();
  glClearIndex( (GLfloat)c );
  _checkError("glClearIndex");
}

FABRIC_EXT_EXPORT void glClearStencil_wrapper(
  KL::Integer s
){
  _clearError();
  glClearStencil( (GLint)s );
  _checkError("glClearStencil");
}

FABRIC_EXT_EXPORT void glClipPlane_wrapper(
  KL::Size plane,
  const KL::VariableArray<KL::Scalar> & equation
){
  _clearError();
  glClipPlane( (GLenum)plane, (const GLdouble*)&equation[0] );
  _checkError("glClipPlane");
}

FABRIC_EXT_EXPORT void glColor3b_wrapper(
  KL::Byte red,
  KL::Byte green,
  KL::Byte blue
){
  _clearError();
  glColor3b( (GLbyte)red, (GLbyte)green, (GLbyte)blue );
  _checkError("glColor3b");
}

FABRIC_EXT_EXPORT void glColor3bv_wrapper(
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
  glColor3bv( (const GLbyte*)&v[0] );
  _checkError("glColor3bv");
}

FABRIC_EXT_EXPORT void glColor3d_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue
){
  _clearError();
  glColor3d( (GLdouble)red, (GLdouble)green, (GLdouble)blue );
  _checkError("glColor3d");
}

FABRIC_EXT_EXPORT void glColor3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glColor3dv( (const GLdouble*)&v[0] );
  _checkError("glColor3dv");
}

FABRIC_EXT_EXPORT void glColor3f_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue
){
  _clearError();
  glColor3f( (GLfloat)red, (GLfloat)green, (GLfloat)blue );
  _checkError("glColor3f");
}

FABRIC_EXT_EXPORT void glColor3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glColor3fv( (const GLfloat*)&v[0] );
  _checkError("glColor3fv");
}

FABRIC_EXT_EXPORT void glColor3i_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
  glColor3i( (GLint)red, (GLint)green, (GLint)blue );
  _checkError("glColor3i");
}

FABRIC_EXT_EXPORT void glColor3iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glColor3iv( (const GLint*)&v[0] );
  _checkError("glColor3iv");
}

FABRIC_EXT_EXPORT void glColor3s_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
  glColor3s( (GLshort)red, (GLshort)green, (GLshort)blue );
  _checkError("glColor3s");
}

FABRIC_EXT_EXPORT void glColor3sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glColor3sv( (const GLshort*)&v[0] );
  _checkError("glColor3sv");
}

FABRIC_EXT_EXPORT void glColor3ub_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue
){
  _clearError();
  glColor3ub( (GLubyte)red, (GLubyte)green, (GLubyte)blue );
  _checkError("glColor3ub");
}

FABRIC_EXT_EXPORT void glColor3ubv_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glColor3ubv( (const GLubyte*)&v[0] );
  _checkError("glColor3ubv");
}

FABRIC_EXT_EXPORT void glColor3ui_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue
){
  _clearError();
  glColor3ui( (GLuint)red, (GLuint)green, (GLuint)blue );
  _checkError("glColor3ui");
}

FABRIC_EXT_EXPORT void glColor3uiv_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glColor3uiv( (const GLuint*)&v[0] );
  _checkError("glColor3uiv");
}

FABRIC_EXT_EXPORT void glColor3us_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
  glColor3us( (GLushort)red, (GLushort)green, (GLushort)blue );
  _checkError("glColor3us");
}

FABRIC_EXT_EXPORT void glColor3usv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glColor4b( (GLbyte)red, (GLbyte)green, (GLbyte)blue, (GLbyte)alpha );
  _checkError("glColor4b");
}

FABRIC_EXT_EXPORT void glColor4bv_wrapper(
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
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
  glColor4d( (GLdouble)red, (GLdouble)green, (GLdouble)blue, (GLdouble)alpha );
  _checkError("glColor4d");
}

FABRIC_EXT_EXPORT void glColor4dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glColor4f( (GLfloat)red, (GLfloat)green, (GLfloat)blue, (GLfloat)alpha );
  _checkError("glColor4f");
}

FABRIC_EXT_EXPORT void glColor4fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glColor4i( (GLint)red, (GLint)green, (GLint)blue, (GLint)alpha );
  _checkError("glColor4i");
}

FABRIC_EXT_EXPORT void glColor4iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glColor4s( (GLshort)red, (GLshort)green, (GLshort)blue, (GLshort)alpha );
  _checkError("glColor4s");
}

FABRIC_EXT_EXPORT void glColor4sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glColor4ub( (GLubyte)red, (GLubyte)green, (GLubyte)blue, (GLubyte)alpha );
  _checkError("glColor4ub");
}

FABRIC_EXT_EXPORT void glColor4ubv_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
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
  glColor4ui( (GLuint)red, (GLuint)green, (GLuint)blue, (GLuint)alpha );
  _checkError("glColor4ui");
}

FABRIC_EXT_EXPORT void glColor4uiv_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
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
  glColor4us( (GLushort)red, (GLushort)green, (GLushort)blue, (GLushort)alpha );
  _checkError("glColor4us");
}

FABRIC_EXT_EXPORT void glColor4usv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glColorMask( (GLboolean)red, (GLboolean)green, (GLboolean)blue, (GLboolean)alpha );
  _checkError("glColorMask");
}

FABRIC_EXT_EXPORT void glColorMaterial_wrapper(
  KL::Size face,
  KL::Size mode
){
  _clearError();
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
  glCopyTexSubImage2D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyTexSubImage2D");
}

FABRIC_EXT_EXPORT void glCullFace_wrapper(
  KL::Size mode
){
  _clearError();
  glCullFace( (GLenum)mode );
  _checkError("glCullFace");
}

FABRIC_EXT_EXPORT void glDeleteLists_wrapper(
  KL::Size list,
  KL::Size range
){
  _clearError();
  glDeleteLists( (GLuint)list, (GLsizei)range );
  _checkError("glDeleteLists");
}

FABRIC_EXT_EXPORT void glDeleteTextures_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & textures
){
  _clearError();
  glDeleteTextures( (GLsizei)n, (const GLuint*)&textures[0] );
  _checkError("glDeleteTextures");
}

FABRIC_EXT_EXPORT void glDepthFunc_wrapper(
  KL::Size func
){
  _clearError();
  glDepthFunc( (GLenum)func );
  _checkError("glDepthFunc");
}

FABRIC_EXT_EXPORT void glDepthMask_wrapper(
  KL::Boolean flag
){
  _clearError();
  glDepthMask( (GLboolean)flag );
  _checkError("glDepthMask");
}

FABRIC_EXT_EXPORT void glDepthRange_wrapper(
  KL::Scalar zNear,
  KL::Scalar zFar
){
  _clearError();
  glDepthRange( (GLclampd)zNear, (GLclampd)zFar );
  _checkError("glDepthRange");
}

FABRIC_EXT_EXPORT void glDisable_wrapper(
  KL::Size cap
){
  _clearError();
  glDisable( (GLenum)cap );
  _checkError("glDisable");
}

FABRIC_EXT_EXPORT void glDisableClientState_wrapper(
  KL::Size array
){
  _clearError();
  glDisableClientState( (GLenum)array );
  _checkError("glDisableClientState");
}

FABRIC_EXT_EXPORT void glDrawArrays_wrapper(
  KL::Size mode,
  KL::Integer first,
  KL::Size count
){
  _clearError();
  glDrawArrays( (GLenum)mode, (GLint)first, (GLsizei)count );
  _checkError("glDrawArrays");
}

FABRIC_EXT_EXPORT void glDrawBuffer_wrapper(
  KL::Size mode
){
  _clearError();
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
  glDrawPixels( (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  _checkError("glDrawPixels");
}

FABRIC_EXT_EXPORT void glEdgeFlag_wrapper(
  KL::Boolean flag
){
  _clearError();
  glEdgeFlag( (GLboolean)flag );
  _checkError("glEdgeFlag");
}

FABRIC_EXT_EXPORT void glEdgeFlagPointer_wrapper(
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
  glEdgeFlagPointer( (GLsizei)stride, pointer );
  _checkError("glEdgeFlagPointer");
}

FABRIC_EXT_EXPORT void glEdgeFlagv_wrapper(
  const KL::VariableArray<KL::Boolean> & flag
){
  _clearError();
  glEdgeFlagv( (const GLboolean*)&flag[0] );
  _checkError("glEdgeFlagv");
}

FABRIC_EXT_EXPORT void glEnable_wrapper(
  KL::Size cap
){
  _clearError();
  glEnable( (GLenum)cap );
  _checkError("glEnable");
}

FABRIC_EXT_EXPORT void glEnableClientState_wrapper(
  KL::Size array
){
  _clearError();
  glEnableClientState( (GLenum)array );
  _checkError("glEnableClientState");
}

FABRIC_EXT_EXPORT void glEnd_wrapper()
{
  _clearError();
  glEnd();
  _decBracket();
  _checkError("glEnd");
}

FABRIC_EXT_EXPORT void glEndList_wrapper()
{
  _clearError();
  glEndList();
  _checkError("glEndList");
}

FABRIC_EXT_EXPORT void glEvalCoord1d_wrapper(
  KL::Scalar u
){
  _clearError();
  glEvalCoord1d( (GLdouble)u );
  _checkError("glEvalCoord1d");
}

FABRIC_EXT_EXPORT void glEvalCoord1dv_wrapper(
  const KL::VariableArray<KL::Scalar> & u
){
  _clearError();
  glEvalCoord1dv( (const GLdouble*)&u[0] );
  _checkError("glEvalCoord1dv");
}

FABRIC_EXT_EXPORT void glEvalCoord1f_wrapper(
  KL::Scalar u
){
  _clearError();
  glEvalCoord1f( (GLfloat)u );
  _checkError("glEvalCoord1f");
}

FABRIC_EXT_EXPORT void glEvalCoord1fv_wrapper(
  const KL::VariableArray<KL::Scalar> & u
){
  _clearError();
  glEvalCoord1fv( (const GLfloat*)&u[0] );
  _checkError("glEvalCoord1fv");
}

FABRIC_EXT_EXPORT void glEvalCoord2d_wrapper(
  KL::Scalar u,
  KL::Scalar v
){
  _clearError();
  glEvalCoord2d( (GLdouble)u, (GLdouble)v );
  _checkError("glEvalCoord2d");
}

FABRIC_EXT_EXPORT void glEvalCoord2dv_wrapper(
  const KL::VariableArray<KL::Scalar> & u
){
  _clearError();
  glEvalCoord2dv( (const GLdouble*)&u[0] );
  _checkError("glEvalCoord2dv");
}

FABRIC_EXT_EXPORT void glEvalCoord2f_wrapper(
  KL::Scalar u,
  KL::Scalar v
){
  _clearError();
  glEvalCoord2f( (GLfloat)u, (GLfloat)v );
  _checkError("glEvalCoord2f");
}

FABRIC_EXT_EXPORT void glEvalCoord2fv_wrapper(
  const KL::VariableArray<KL::Scalar> & u
){
  _clearError();
  glEvalCoord2fv( (const GLfloat*)&u[0] );
  _checkError("glEvalCoord2fv");
}

FABRIC_EXT_EXPORT void glEvalMesh1_wrapper(
  KL::Size mode,
  KL::Integer i1,
  KL::Integer i2
){
  _clearError();
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
  glEvalMesh2( (GLenum)mode, (GLint)i1, (GLint)i2, (GLint)j1, (GLint)j2 );
  _checkError("glEvalMesh2");
}

FABRIC_EXT_EXPORT void glEvalPoint1_wrapper(
  KL::Integer i
){
  _clearError();
  glEvalPoint1( (GLint)i );
  _checkError("glEvalPoint1");
}

FABRIC_EXT_EXPORT void glEvalPoint2_wrapper(
  KL::Integer i,
  KL::Integer j
){
  _clearError();
  glEvalPoint2( (GLint)i, (GLint)j );
  _checkError("glEvalPoint2");
}

FABRIC_EXT_EXPORT void glFeedbackBuffer_wrapper(
  KL::Size size,
  KL::Size type,
  KL::VariableArray<KL::Scalar> & buffer
){
  _clearError();
  glFeedbackBuffer( (GLsizei)size, (GLenum)type, (GLfloat*)&buffer[0] );
  _checkError("glFeedbackBuffer");
}

FABRIC_EXT_EXPORT void glFinish_wrapper()
{
  _clearError();
  glFinish();
  _checkError("glFinish");
}

FABRIC_EXT_EXPORT void glFlush_wrapper()
{
  _clearError();
  glFlush();
  _checkError("glFlush");
}

FABRIC_EXT_EXPORT void glFogf_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glFogf( (GLenum)pname, (GLfloat)param );
  _checkError("glFogf");
}

FABRIC_EXT_EXPORT void glFogfv_wrapper(
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glFogfv( (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glFogfv");
}

FABRIC_EXT_EXPORT void glFogi_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glFogi( (GLenum)pname, (GLint)param );
  _checkError("glFogi");
}

FABRIC_EXT_EXPORT void glFogiv_wrapper(
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glFogiv( (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glFogiv");
}

FABRIC_EXT_EXPORT void glFrontFace_wrapper(
  KL::Size mode
){
  _clearError();
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
  glFrustum( (GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top, (GLdouble)zNear, (GLdouble)zFar );
  _checkError("glFrustum");
}

FABRIC_EXT_EXPORT KL::Size glGenLists_wrapper(
  KL::Size range
){
  _clearError();
  GLuint result = glGenLists( (GLsizei)range );
  _checkError("glGenLists");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glGenTextures_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & textures
){
  _clearError();
  glGenTextures( (GLsizei)n, (GLuint*)&textures[0] );
  _checkError("glGenTextures");
}

FABRIC_EXT_EXPORT void glGetBooleanv_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Boolean> & params
){
  _clearError();
  glGetBooleanv( (GLenum)pname, (GLboolean*)&params[0] );
  _checkError("glGetBooleanv");
}

FABRIC_EXT_EXPORT void glGetClipPlane_wrapper(
  KL::Size plane,
  KL::VariableArray<KL::Scalar> & equation
){
  _clearError();
  glGetClipPlane( (GLenum)plane, (GLdouble*)&equation[0] );
  _checkError("glGetClipPlane");
}

FABRIC_EXT_EXPORT void glGetDoublev_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetDoublev( (GLenum)pname, (GLdouble*)&params[0] );
  _checkError("glGetDoublev");
}

FABRIC_EXT_EXPORT KL::Size glGetError_wrapper()
{
  GLenum result = glGetError();
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glGetFloatv_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetFloatv( (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetFloatv");
}

FABRIC_EXT_EXPORT void glGetIntegerv_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetIntegerv( (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetIntegerv");
}

FABRIC_EXT_EXPORT void glGetLightfv_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetLightfv( (GLenum)light, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetLightfv");
}

FABRIC_EXT_EXPORT void glGetLightiv_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetLightiv( (GLenum)light, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetLightiv");
}

FABRIC_EXT_EXPORT void glGetMapdv_wrapper(
  KL::Size target,
  KL::Size query,
  KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glGetMapdv( (GLenum)target, (GLenum)query, (GLdouble*)&v[0] );
  _checkError("glGetMapdv");
}

FABRIC_EXT_EXPORT void glGetMapfv_wrapper(
  KL::Size target,
  KL::Size query,
  KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glGetMapfv( (GLenum)target, (GLenum)query, (GLfloat*)&v[0] );
  _checkError("glGetMapfv");
}

FABRIC_EXT_EXPORT void glGetMapiv_wrapper(
  KL::Size target,
  KL::Size query,
  KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glGetMapiv( (GLenum)target, (GLenum)query, (GLint*)&v[0] );
  _checkError("glGetMapiv");
}

FABRIC_EXT_EXPORT void glGetMaterialfv_wrapper(
  KL::Size face,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetMaterialfv( (GLenum)face, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetMaterialfv");
}

FABRIC_EXT_EXPORT void glGetMaterialiv_wrapper(
  KL::Size face,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetMaterialiv( (GLenum)face, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetMaterialiv");
}

FABRIC_EXT_EXPORT void glGetPixelMapfv_wrapper(
  KL::Size map,
  KL::VariableArray<KL::Scalar> & values
){
  _clearError();
  glGetPixelMapfv( (GLenum)map, (GLfloat*)&values[0] );
  _checkError("glGetPixelMapfv");
}

FABRIC_EXT_EXPORT void glGetPixelMapuiv_wrapper(
  KL::Size map,
  KL::VariableArray<KL::Size> & values
){
  _clearError();
  glGetPixelMapuiv( (GLenum)map, (GLuint*)&values[0] );
  _checkError("glGetPixelMapuiv");
}

FABRIC_EXT_EXPORT void glGetPixelMapusv_wrapper(
  KL::Size map,
  KL::VariableArray<KL::Integer> & values
){
  _clearError();
  glGetPixelMapusv( (GLenum)map, (GLushort*)&values[0] );
  _checkError("glGetPixelMapusv");
}

FABRIC_EXT_EXPORT void glGetPolygonStipple_wrapper(
  KL::VariableArray<KL::Size> & mask
){
  _clearError();
  glGetPolygonStipple( (GLubyte*)&mask[0] );
  _checkError("glGetPolygonStipple");
}

FABRIC_EXT_EXPORT KL::String glGetString_wrapper(
  KL::Size name
){
  _clearError();
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
  glGetTexEnvfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetTexEnvfv");
}

FABRIC_EXT_EXPORT void glGetTexEnviv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetTexEnviv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetTexEnviv");
}

FABRIC_EXT_EXPORT void glGetTexGendv_wrapper(
  KL::Size coord,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetTexGendv( (GLenum)coord, (GLenum)pname, (GLdouble*)&params[0] );
  _checkError("glGetTexGendv");
}

FABRIC_EXT_EXPORT void glGetTexGenfv_wrapper(
  KL::Size coord,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetTexGenfv( (GLenum)coord, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetTexGenfv");
}

FABRIC_EXT_EXPORT void glGetTexGeniv_wrapper(
  KL::Size coord,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glGetTexLevelParameteriv( (GLenum)target, (GLint)level, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetTexLevelParameteriv");
}

FABRIC_EXT_EXPORT void glGetTexParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetTexParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetTexParameterfv");
}

FABRIC_EXT_EXPORT void glGetTexParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetTexParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetTexParameteriv");
}

FABRIC_EXT_EXPORT void glHint_wrapper(
  KL::Size target,
  KL::Size mode
){
  _clearError();
  glHint( (GLenum)target, (GLenum)mode );
  _checkError("glHint");
}

FABRIC_EXT_EXPORT void glIndexMask_wrapper(
  KL::Size mask
){
  _clearError();
  glIndexMask( (GLuint)mask );
  _checkError("glIndexMask");
}

FABRIC_EXT_EXPORT void glIndexPointer_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
  glIndexPointer( (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glIndexPointer");
}

FABRIC_EXT_EXPORT void glIndexd_wrapper(
  KL::Scalar c
){
  _clearError();
  glIndexd( (GLdouble)c );
  _checkError("glIndexd");
}

FABRIC_EXT_EXPORT void glIndexdv_wrapper(
  const KL::VariableArray<KL::Scalar> & c
){
  _clearError();
  glIndexdv( (const GLdouble*)&c[0] );
  _checkError("glIndexdv");
}

FABRIC_EXT_EXPORT void glIndexf_wrapper(
  KL::Scalar c
){
  _clearError();
  glIndexf( (GLfloat)c );
  _checkError("glIndexf");
}

FABRIC_EXT_EXPORT void glIndexfv_wrapper(
  const KL::VariableArray<KL::Scalar> & c
){
  _clearError();
  glIndexfv( (const GLfloat*)&c[0] );
  _checkError("glIndexfv");
}

FABRIC_EXT_EXPORT void glIndexi_wrapper(
  KL::Integer c
){
  _clearError();
  glIndexi( (GLint)c );
  _checkError("glIndexi");
}

FABRIC_EXT_EXPORT void glIndexiv_wrapper(
  const KL::VariableArray<KL::Integer> & c
){
  _clearError();
  glIndexiv( (const GLint*)&c[0] );
  _checkError("glIndexiv");
}

FABRIC_EXT_EXPORT void glIndexs_wrapper(
  KL::Integer c
){
  _clearError();
  glIndexs( (GLshort)c );
  _checkError("glIndexs");
}

FABRIC_EXT_EXPORT void glIndexsv_wrapper(
  const KL::VariableArray<KL::Integer> & c
){
  _clearError();
  glIndexsv( (const GLshort*)&c[0] );
  _checkError("glIndexsv");
}

FABRIC_EXT_EXPORT void glIndexub_wrapper(
  KL::Size c
){
  _clearError();
  glIndexub( (GLubyte)c );
  _checkError("glIndexub");
}

FABRIC_EXT_EXPORT void glIndexubv_wrapper(
  const KL::VariableArray<KL::Size> & c
){
  _clearError();
  glIndexubv( (const GLubyte*)&c[0] );
  _checkError("glIndexubv");
}

FABRIC_EXT_EXPORT void glInitNames_wrapper()
{
  _clearError();
  glInitNames();
  _checkError("glInitNames");
}

FABRIC_EXT_EXPORT void glInterleavedArrays_wrapper(
  KL::Size format,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
  glInterleavedArrays( (GLenum)format, (GLsizei)stride, pointer );
  _checkError("glInterleavedArrays");
}

FABRIC_EXT_EXPORT KL::Boolean glIsEnabled_wrapper(
  KL::Size cap
){
  _clearError();
  GLboolean result = glIsEnabled( (GLenum)cap );
  _checkError("glIsEnabled");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glIsList_wrapper(
  KL::Size list
){
  _clearError();
  GLboolean result = glIsList( (GLuint)list );
  _checkError("glIsList");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glIsTexture_wrapper(
  KL::Size texture
){
  _clearError();
  GLboolean result = glIsTexture( (GLuint)texture );
  _checkError("glIsTexture");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glLightModelf_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glLightModelf( (GLenum)pname, (GLfloat)param );
  _checkError("glLightModelf");
}

FABRIC_EXT_EXPORT void glLightModelfv_wrapper(
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glLightModelfv( (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glLightModelfv");
}

FABRIC_EXT_EXPORT void glLightModeli_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glLightModeli( (GLenum)pname, (GLint)param );
  _checkError("glLightModeli");
}

FABRIC_EXT_EXPORT void glLightModeliv_wrapper(
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glLightModeliv( (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glLightModeliv");
}

FABRIC_EXT_EXPORT void glLightf_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glLightf( (GLenum)light, (GLenum)pname, (GLfloat)param );
  _checkError("glLightf");
}

FABRIC_EXT_EXPORT void glLightfv_wrapper(
  KL::Size light,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glLightfv( (GLenum)light, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glLightfv");
}

FABRIC_EXT_EXPORT void glLighti_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glLighti( (GLenum)light, (GLenum)pname, (GLint)param );
  _checkError("glLighti");
}

FABRIC_EXT_EXPORT void glLightiv_wrapper(
  KL::Size light,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glLightiv( (GLenum)light, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glLightiv");
}

FABRIC_EXT_EXPORT void glLineStipple_wrapper(
  KL::Integer factor,
  KL::Integer pattern
){
  _clearError();
  glLineStipple( (GLint)factor, (GLushort)pattern );
  _checkError("glLineStipple");
}

FABRIC_EXT_EXPORT void glLineWidth_wrapper(
  KL::Scalar width
){
  _clearError();
  glLineWidth( (GLfloat)width );
  _checkError("glLineWidth");
}

FABRIC_EXT_EXPORT void glListBase_wrapper(
  KL::Size base
){
  _clearError();
  glListBase( (GLuint)base );
  _checkError("glListBase");
}

FABRIC_EXT_EXPORT void glLoadIdentity_wrapper()
{
  _clearError();
  glLoadIdentity();
  _checkError("glLoadIdentity");
}

FABRIC_EXT_EXPORT void glLoadMatrixd_wrapper(
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
  glLoadMatrixd( (const GLdouble*)&m[0] );
  _checkError("glLoadMatrixd");
}

FABRIC_EXT_EXPORT void glLoadMatrixf_wrapper(
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
  glLoadMatrixf( (const GLfloat*)&m[0] );
  _checkError("glLoadMatrixf");
}

FABRIC_EXT_EXPORT void glLoadName_wrapper(
  KL::Size name
){
  _clearError();
  glLoadName( (GLuint)name );
  _checkError("glLoadName");
}

FABRIC_EXT_EXPORT void glLogicOp_wrapper(
  KL::Size opcode
){
  _clearError();
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
  glMap2f( (GLenum)target, (GLfloat)u1, (GLfloat)u2, (GLint)ustride, (GLint)uorder, (GLfloat)v1, (GLfloat)v2, (GLint)vstride, (GLint)vorder, (const GLfloat*)&points[0] );
  _checkError("glMap2f");
}

FABRIC_EXT_EXPORT void glMapGrid1d_wrapper(
  KL::Integer un,
  KL::Scalar u1,
  KL::Scalar u2
){
  _clearError();
  glMapGrid1d( (GLint)un, (GLdouble)u1, (GLdouble)u2 );
  _checkError("glMapGrid1d");
}

FABRIC_EXT_EXPORT void glMapGrid1f_wrapper(
  KL::Integer un,
  KL::Scalar u1,
  KL::Scalar u2
){
  _clearError();
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
  glMapGrid2f( (GLint)un, (GLfloat)u1, (GLfloat)u2, (GLint)vn, (GLfloat)v1, (GLfloat)v2 );
  _checkError("glMapGrid2f");
}

FABRIC_EXT_EXPORT void glMaterialf_wrapper(
  KL::Size face,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glMaterialf( (GLenum)face, (GLenum)pname, (GLfloat)param );
  _checkError("glMaterialf");
}

FABRIC_EXT_EXPORT void glMaterialfv_wrapper(
  KL::Size face,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glMaterialfv( (GLenum)face, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glMaterialfv");
}

FABRIC_EXT_EXPORT void glMateriali_wrapper(
  KL::Size face,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glMateriali( (GLenum)face, (GLenum)pname, (GLint)param );
  _checkError("glMateriali");
}

FABRIC_EXT_EXPORT void glMaterialiv_wrapper(
  KL::Size face,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glMaterialiv( (GLenum)face, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glMaterialiv");
}

FABRIC_EXT_EXPORT void glMatrixMode_wrapper(
  KL::Size mode
){
  _clearError();
  glMatrixMode( (GLenum)mode );
  _checkError("glMatrixMode");
}

FABRIC_EXT_EXPORT void glMultMatrixd_wrapper(
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
  glMultMatrixd( (const GLdouble*)&m[0] );
  _checkError("glMultMatrixd");
}

FABRIC_EXT_EXPORT void glMultMatrixf_wrapper(
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
  glMultMatrixf( (const GLfloat*)&m[0] );
  _checkError("glMultMatrixf");
}

FABRIC_EXT_EXPORT void glNewList_wrapper(
  KL::Size list,
  KL::Size mode
){
  _clearError();
  glNewList( (GLuint)list, (GLenum)mode );
  _checkError("glNewList");
}

FABRIC_EXT_EXPORT void glNormal3b_wrapper(
  KL::Byte nx,
  KL::Byte ny,
  KL::Byte nz
){
  _clearError();
  glNormal3b( (GLbyte)nx, (GLbyte)ny, (GLbyte)nz );
  _checkError("glNormal3b");
}

FABRIC_EXT_EXPORT void glNormal3bv_wrapper(
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
  glNormal3bv( (const GLbyte*)&v[0] );
  _checkError("glNormal3bv");
}

FABRIC_EXT_EXPORT void glNormal3d_wrapper(
  KL::Scalar nx,
  KL::Scalar ny,
  KL::Scalar nz
){
  _clearError();
  glNormal3d( (GLdouble)nx, (GLdouble)ny, (GLdouble)nz );
  _checkError("glNormal3d");
}

FABRIC_EXT_EXPORT void glNormal3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glNormal3dv( (const GLdouble*)&v[0] );
  _checkError("glNormal3dv");
}

FABRIC_EXT_EXPORT void glNormal3f_wrapper(
  KL::Scalar nx,
  KL::Scalar ny,
  KL::Scalar nz
){
  _clearError();
  glNormal3f( (GLfloat)nx, (GLfloat)ny, (GLfloat)nz );
  _checkError("glNormal3f");
}

FABRIC_EXT_EXPORT void glNormal3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glNormal3fv( (const GLfloat*)&v[0] );
  _checkError("glNormal3fv");
}

FABRIC_EXT_EXPORT void glNormal3i_wrapper(
  KL::Integer nx,
  KL::Integer ny,
  KL::Integer nz
){
  _clearError();
  glNormal3i( (GLint)nx, (GLint)ny, (GLint)nz );
  _checkError("glNormal3i");
}

FABRIC_EXT_EXPORT void glNormal3iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glNormal3iv( (const GLint*)&v[0] );
  _checkError("glNormal3iv");
}

FABRIC_EXT_EXPORT void glNormal3s_wrapper(
  KL::Integer nx,
  KL::Integer ny,
  KL::Integer nz
){
  _clearError();
  glNormal3s( (GLshort)nx, (GLshort)ny, (GLshort)nz );
  _checkError("glNormal3s");
}

FABRIC_EXT_EXPORT void glNormal3sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glNormal3sv( (const GLshort*)&v[0] );
  _checkError("glNormal3sv");
}

FABRIC_EXT_EXPORT void glNormalPointer_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
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
  glOrtho( (GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top, (GLdouble)zNear, (GLdouble)zFar );
  _checkError("glOrtho");
}

FABRIC_EXT_EXPORT void glPassThrough_wrapper(
  KL::Scalar token
){
  _clearError();
  glPassThrough( (GLfloat)token );
  _checkError("glPassThrough");
}

FABRIC_EXT_EXPORT void glPixelMapfv_wrapper(
  KL::Size map,
  KL::Size mapsize,
  const KL::VariableArray<KL::Scalar> & values
){
  _clearError();
  glPixelMapfv( (GLenum)map, (GLsizei)mapsize, (const GLfloat*)&values[0] );
  _checkError("glPixelMapfv");
}

FABRIC_EXT_EXPORT void glPixelMapuiv_wrapper(
  KL::Size map,
  KL::Size mapsize,
  const KL::VariableArray<KL::Size> & values
){
  _clearError();
  glPixelMapuiv( (GLenum)map, (GLsizei)mapsize, (const GLuint*)&values[0] );
  _checkError("glPixelMapuiv");
}

FABRIC_EXT_EXPORT void glPixelMapusv_wrapper(
  KL::Size map,
  KL::Size mapsize,
  const KL::VariableArray<KL::Integer> & values
){
  _clearError();
  glPixelMapusv( (GLenum)map, (GLsizei)mapsize, (const GLushort*)&values[0] );
  _checkError("glPixelMapusv");
}

FABRIC_EXT_EXPORT void glPixelStoref_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glPixelStoref( (GLenum)pname, (GLfloat)param );
  _checkError("glPixelStoref");
}

FABRIC_EXT_EXPORT void glPixelStorei_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glPixelStorei( (GLenum)pname, (GLint)param );
  _checkError("glPixelStorei");
}

FABRIC_EXT_EXPORT void glPixelTransferf_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glPixelTransferf( (GLenum)pname, (GLfloat)param );
  _checkError("glPixelTransferf");
}

FABRIC_EXT_EXPORT void glPixelTransferi_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glPixelTransferi( (GLenum)pname, (GLint)param );
  _checkError("glPixelTransferi");
}

FABRIC_EXT_EXPORT void glPixelZoom_wrapper(
  KL::Scalar xfactor,
  KL::Scalar yfactor
){
  _clearError();
  glPixelZoom( (GLfloat)xfactor, (GLfloat)yfactor );
  _checkError("glPixelZoom");
}

FABRIC_EXT_EXPORT void glPointSize_wrapper(
  KL::Scalar size
){
  _clearError();
  glPointSize( (GLfloat)size );
  _checkError("glPointSize");
}

FABRIC_EXT_EXPORT void glPolygonMode_wrapper(
  KL::Size face,
  KL::Size mode
){
  _clearError();
  glPolygonMode( (GLenum)face, (GLenum)mode );
  _checkError("glPolygonMode");
}

FABRIC_EXT_EXPORT void glPolygonOffset_wrapper(
  KL::Scalar factor,
  KL::Scalar units
){
  _clearError();
  glPolygonOffset( (GLfloat)factor, (GLfloat)units );
  _checkError("glPolygonOffset");
}

FABRIC_EXT_EXPORT void glPolygonStipple_wrapper(
  const KL::VariableArray<KL::Size> & mask
){
  _clearError();
  glPolygonStipple( (const GLubyte*)&mask[0] );
  _checkError("glPolygonStipple");
}

FABRIC_EXT_EXPORT void glPopAttrib_wrapper()
{
  _clearError();
  glPopAttrib();
  _checkError("glPopAttrib");
}

FABRIC_EXT_EXPORT void glPopClientAttrib_wrapper()
{
  _clearError();
  glPopClientAttrib();
  _checkError("glPopClientAttrib");
}

FABRIC_EXT_EXPORT void glPopMatrix_wrapper()
{
  _clearError();
  glPopMatrix();
  _checkError("glPopMatrix");
}

FABRIC_EXT_EXPORT void glPopName_wrapper()
{
  _clearError();
  glPopName();
  _checkError("glPopName");
}

FABRIC_EXT_EXPORT void glPrioritizeTextures_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & textures,
  const KL::VariableArray<KL::Scalar> & priorities
){
  _clearError();
  glPrioritizeTextures( (GLsizei)n, (const GLuint*)&textures[0], (const GLclampf*)&priorities[0] );
  _checkError("glPrioritizeTextures");
}

FABRIC_EXT_EXPORT void glPushAttrib_wrapper(
  KL::Size mask
){
  _clearError();
  glPushAttrib( (GLbitfield)mask );
  _checkError("glPushAttrib");
}

FABRIC_EXT_EXPORT void glPushClientAttrib_wrapper(
  KL::Size mask
){
  _clearError();
  glPushClientAttrib( (GLbitfield)mask );
  _checkError("glPushClientAttrib");
}

FABRIC_EXT_EXPORT void glPushMatrix_wrapper()
{
  _clearError();
  glPushMatrix();
  _checkError("glPushMatrix");
}

FABRIC_EXT_EXPORT void glPushName_wrapper(
  KL::Size name
){
  _clearError();
  glPushName( (GLuint)name );
  _checkError("glPushName");
}

FABRIC_EXT_EXPORT void glRasterPos2d_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
  glRasterPos2d( (GLdouble)x, (GLdouble)y );
  _checkError("glRasterPos2d");
}

FABRIC_EXT_EXPORT void glRasterPos2dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glRasterPos2dv( (const GLdouble*)&v[0] );
  _checkError("glRasterPos2dv");
}

FABRIC_EXT_EXPORT void glRasterPos2f_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
  glRasterPos2f( (GLfloat)x, (GLfloat)y );
  _checkError("glRasterPos2f");
}

FABRIC_EXT_EXPORT void glRasterPos2fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glRasterPos2fv( (const GLfloat*)&v[0] );
  _checkError("glRasterPos2fv");
}

FABRIC_EXT_EXPORT void glRasterPos2i_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
  glRasterPos2i( (GLint)x, (GLint)y );
  _checkError("glRasterPos2i");
}

FABRIC_EXT_EXPORT void glRasterPos2iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glRasterPos2iv( (const GLint*)&v[0] );
  _checkError("glRasterPos2iv");
}

FABRIC_EXT_EXPORT void glRasterPos2s_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
  glRasterPos2s( (GLshort)x, (GLshort)y );
  _checkError("glRasterPos2s");
}

FABRIC_EXT_EXPORT void glRasterPos2sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glRasterPos2sv( (const GLshort*)&v[0] );
  _checkError("glRasterPos2sv");
}

FABRIC_EXT_EXPORT void glRasterPos3d_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
  glRasterPos3d( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glRasterPos3d");
}

FABRIC_EXT_EXPORT void glRasterPos3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glRasterPos3dv( (const GLdouble*)&v[0] );
  _checkError("glRasterPos3dv");
}

FABRIC_EXT_EXPORT void glRasterPos3f_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
  glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glRasterPos3f");
}

FABRIC_EXT_EXPORT void glRasterPos3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glRasterPos3fv( (const GLfloat*)&v[0] );
  _checkError("glRasterPos3fv");
}

FABRIC_EXT_EXPORT void glRasterPos3i_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
  glRasterPos3i( (GLint)x, (GLint)y, (GLint)z );
  _checkError("glRasterPos3i");
}

FABRIC_EXT_EXPORT void glRasterPos3iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glRasterPos3iv( (const GLint*)&v[0] );
  _checkError("glRasterPos3iv");
}

FABRIC_EXT_EXPORT void glRasterPos3s_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
  glRasterPos3s( (GLshort)x, (GLshort)y, (GLshort)z );
  _checkError("glRasterPos3s");
}

FABRIC_EXT_EXPORT void glRasterPos3sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glRasterPos4d( (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  _checkError("glRasterPos4d");
}

FABRIC_EXT_EXPORT void glRasterPos4dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glRasterPos4f( (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  _checkError("glRasterPos4f");
}

FABRIC_EXT_EXPORT void glRasterPos4fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glRasterPos4i( (GLint)x, (GLint)y, (GLint)z, (GLint)w );
  _checkError("glRasterPos4i");
}

FABRIC_EXT_EXPORT void glRasterPos4iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glRasterPos4s( (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
  _checkError("glRasterPos4s");
}

FABRIC_EXT_EXPORT void glRasterPos4sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glRasterPos4sv( (const GLshort*)&v[0] );
  _checkError("glRasterPos4sv");
}

FABRIC_EXT_EXPORT void glReadBuffer_wrapper(
  KL::Size mode
){
  _clearError();
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
  glRectd( (GLdouble)x1, (GLdouble)y1, (GLdouble)x2, (GLdouble)y2 );
  _checkError("glRectd");
}

FABRIC_EXT_EXPORT void glRectdv_wrapper(
  const KL::VariableArray<KL::Scalar> & v1,
  const KL::VariableArray<KL::Scalar> & v2
){
  _clearError();
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
  glRectf( (GLfloat)x1, (GLfloat)y1, (GLfloat)x2, (GLfloat)y2 );
  _checkError("glRectf");
}

FABRIC_EXT_EXPORT void glRectfv_wrapper(
  const KL::VariableArray<KL::Scalar> & v1,
  const KL::VariableArray<KL::Scalar> & v2
){
  _clearError();
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
  glRecti( (GLint)x1, (GLint)y1, (GLint)x2, (GLint)y2 );
  _checkError("glRecti");
}

FABRIC_EXT_EXPORT void glRectiv_wrapper(
  const KL::VariableArray<KL::Integer> & v1,
  const KL::VariableArray<KL::Integer> & v2
){
  _clearError();
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
  glRects( (GLshort)x1, (GLshort)y1, (GLshort)x2, (GLshort)y2 );
  _checkError("glRects");
}

FABRIC_EXT_EXPORT void glRectsv_wrapper(
  const KL::VariableArray<KL::Integer> & v1,
  const KL::VariableArray<KL::Integer> & v2
){
  _clearError();
  glRectsv( (const GLshort*)&v1[0], (const GLshort*)&v2[0] );
  _checkError("glRectsv");
}

FABRIC_EXT_EXPORT KL::Integer glRenderMode_wrapper(
  KL::Size mode
){
  _clearError();
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
  glRotatef( (GLfloat)angle, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glRotatef");
}

FABRIC_EXT_EXPORT void glScaled_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
  glScaled( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glScaled");
}

FABRIC_EXT_EXPORT void glScalef_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
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
  glScissor( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glScissor");
}

FABRIC_EXT_EXPORT void glSelectBuffer_wrapper(
  KL::Size size,
  KL::VariableArray<KL::Size> & buffer
){
  _clearError();
  glSelectBuffer( (GLsizei)size, (GLuint*)&buffer[0] );
  _checkError("glSelectBuffer");
}

FABRIC_EXT_EXPORT void glShadeModel_wrapper(
  KL::Size mode
){
  _clearError();
  glShadeModel( (GLenum)mode );
  _checkError("glShadeModel");
}

FABRIC_EXT_EXPORT void glStencilFunc_wrapper(
  KL::Size func,
  KL::Integer ref,
  KL::Size mask
){
  _clearError();
  glStencilFunc( (GLenum)func, (GLint)ref, (GLuint)mask );
  _checkError("glStencilFunc");
}

FABRIC_EXT_EXPORT void glStencilMask_wrapper(
  KL::Size mask
){
  _clearError();
  glStencilMask( (GLuint)mask );
  _checkError("glStencilMask");
}

FABRIC_EXT_EXPORT void glStencilOp_wrapper(
  KL::Size fail,
  KL::Size zfail,
  KL::Size zpass
){
  _clearError();
  glStencilOp( (GLenum)fail, (GLenum)zfail, (GLenum)zpass );
  _checkError("glStencilOp");
}

FABRIC_EXT_EXPORT void glTexCoord1d_wrapper(
  KL::Scalar s
){
  _clearError();
  glTexCoord1d( (GLdouble)s );
  _checkError("glTexCoord1d");
}

FABRIC_EXT_EXPORT void glTexCoord1dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glTexCoord1dv( (const GLdouble*)&v[0] );
  _checkError("glTexCoord1dv");
}

FABRIC_EXT_EXPORT void glTexCoord1f_wrapper(
  KL::Scalar s
){
  _clearError();
  glTexCoord1f( (GLfloat)s );
  _checkError("glTexCoord1f");
}

FABRIC_EXT_EXPORT void glTexCoord1fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glTexCoord1fv( (const GLfloat*)&v[0] );
  _checkError("glTexCoord1fv");
}

FABRIC_EXT_EXPORT void glTexCoord1i_wrapper(
  KL::Integer s
){
  _clearError();
  glTexCoord1i( (GLint)s );
  _checkError("glTexCoord1i");
}

FABRIC_EXT_EXPORT void glTexCoord1iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glTexCoord1iv( (const GLint*)&v[0] );
  _checkError("glTexCoord1iv");
}

FABRIC_EXT_EXPORT void glTexCoord1s_wrapper(
  KL::Integer s
){
  _clearError();
  glTexCoord1s( (GLshort)s );
  _checkError("glTexCoord1s");
}

FABRIC_EXT_EXPORT void glTexCoord1sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glTexCoord1sv( (const GLshort*)&v[0] );
  _checkError("glTexCoord1sv");
}

FABRIC_EXT_EXPORT void glTexCoord2d_wrapper(
  KL::Scalar s,
  KL::Scalar t
){
  _clearError();
  glTexCoord2d( (GLdouble)s, (GLdouble)t );
  _checkError("glTexCoord2d");
}

FABRIC_EXT_EXPORT void glTexCoord2dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glTexCoord2dv( (const GLdouble*)&v[0] );
  _checkError("glTexCoord2dv");
}

FABRIC_EXT_EXPORT void glTexCoord2f_wrapper(
  KL::Scalar s,
  KL::Scalar t
){
  _clearError();
  glTexCoord2f( (GLfloat)s, (GLfloat)t );
  _checkError("glTexCoord2f");
}

FABRIC_EXT_EXPORT void glTexCoord2fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glTexCoord2fv( (const GLfloat*)&v[0] );
  _checkError("glTexCoord2fv");
}

FABRIC_EXT_EXPORT void glTexCoord2i_wrapper(
  KL::Integer s,
  KL::Integer t
){
  _clearError();
  glTexCoord2i( (GLint)s, (GLint)t );
  _checkError("glTexCoord2i");
}

FABRIC_EXT_EXPORT void glTexCoord2iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glTexCoord2iv( (const GLint*)&v[0] );
  _checkError("glTexCoord2iv");
}

FABRIC_EXT_EXPORT void glTexCoord2s_wrapper(
  KL::Integer s,
  KL::Integer t
){
  _clearError();
  glTexCoord2s( (GLshort)s, (GLshort)t );
  _checkError("glTexCoord2s");
}

FABRIC_EXT_EXPORT void glTexCoord2sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glTexCoord2sv( (const GLshort*)&v[0] );
  _checkError("glTexCoord2sv");
}

FABRIC_EXT_EXPORT void glTexCoord3d_wrapper(
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r
){
  _clearError();
  glTexCoord3d( (GLdouble)s, (GLdouble)t, (GLdouble)r );
  _checkError("glTexCoord3d");
}

FABRIC_EXT_EXPORT void glTexCoord3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glTexCoord3dv( (const GLdouble*)&v[0] );
  _checkError("glTexCoord3dv");
}

FABRIC_EXT_EXPORT void glTexCoord3f_wrapper(
  KL::Scalar s,
  KL::Scalar t,
  KL::Scalar r
){
  _clearError();
  glTexCoord3f( (GLfloat)s, (GLfloat)t, (GLfloat)r );
  _checkError("glTexCoord3f");
}

FABRIC_EXT_EXPORT void glTexCoord3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glTexCoord3fv( (const GLfloat*)&v[0] );
  _checkError("glTexCoord3fv");
}

FABRIC_EXT_EXPORT void glTexCoord3i_wrapper(
  KL::Integer s,
  KL::Integer t,
  KL::Integer r
){
  _clearError();
  glTexCoord3i( (GLint)s, (GLint)t, (GLint)r );
  _checkError("glTexCoord3i");
}

FABRIC_EXT_EXPORT void glTexCoord3iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glTexCoord3iv( (const GLint*)&v[0] );
  _checkError("glTexCoord3iv");
}

FABRIC_EXT_EXPORT void glTexCoord3s_wrapper(
  KL::Integer s,
  KL::Integer t,
  KL::Integer r
){
  _clearError();
  glTexCoord3s( (GLshort)s, (GLshort)t, (GLshort)r );
  _checkError("glTexCoord3s");
}

FABRIC_EXT_EXPORT void glTexCoord3sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glTexCoord4d( (GLdouble)s, (GLdouble)t, (GLdouble)r, (GLdouble)q );
  _checkError("glTexCoord4d");
}

FABRIC_EXT_EXPORT void glTexCoord4dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glTexCoord4f( (GLfloat)s, (GLfloat)t, (GLfloat)r, (GLfloat)q );
  _checkError("glTexCoord4f");
}

FABRIC_EXT_EXPORT void glTexCoord4fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glTexCoord4i( (GLint)s, (GLint)t, (GLint)r, (GLint)q );
  _checkError("glTexCoord4i");
}

FABRIC_EXT_EXPORT void glTexCoord4iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glTexCoord4s( (GLshort)s, (GLshort)t, (GLshort)r, (GLshort)q );
  _checkError("glTexCoord4s");
}

FABRIC_EXT_EXPORT void glTexCoord4sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glTexCoordPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glTexCoordPointer");
}

FABRIC_EXT_EXPORT void glTexEnvf_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glTexEnvf( (GLenum)target, (GLenum)pname, (GLfloat)param );
  _checkError("glTexEnvf");
}

FABRIC_EXT_EXPORT void glTexEnvfv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glTexEnvfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glTexEnvfv");
}

FABRIC_EXT_EXPORT void glTexEnvi_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glTexEnvi( (GLenum)target, (GLenum)pname, (GLint)param );
  _checkError("glTexEnvi");
}

FABRIC_EXT_EXPORT void glTexEnviv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glTexEnviv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glTexEnviv");
}

FABRIC_EXT_EXPORT void glTexGend_wrapper(
  KL::Size coord,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glTexGend( (GLenum)coord, (GLenum)pname, (GLdouble)param );
  _checkError("glTexGend");
}

FABRIC_EXT_EXPORT void glTexGendv_wrapper(
  KL::Size coord,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glTexGendv( (GLenum)coord, (GLenum)pname, (const GLdouble*)&params[0] );
  _checkError("glTexGendv");
}

FABRIC_EXT_EXPORT void glTexGenf_wrapper(
  KL::Size coord,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glTexGenf( (GLenum)coord, (GLenum)pname, (GLfloat)param );
  _checkError("glTexGenf");
}

FABRIC_EXT_EXPORT void glTexGenfv_wrapper(
  KL::Size coord,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glTexGenfv( (GLenum)coord, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glTexGenfv");
}

FABRIC_EXT_EXPORT void glTexGeni_wrapper(
  KL::Size coord,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glTexGeni( (GLenum)coord, (GLenum)pname, (GLint)param );
  _checkError("glTexGeni");
}

FABRIC_EXT_EXPORT void glTexGeniv_wrapper(
  KL::Size coord,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glTexImage2D( (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexImage2D");
}

FABRIC_EXT_EXPORT void glTexParameterf_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glTexParameterf( (GLenum)target, (GLenum)pname, (GLfloat)param );
  _checkError("glTexParameterf");
}

FABRIC_EXT_EXPORT void glTexParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glTexParameterfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glTexParameterfv");
}

FABRIC_EXT_EXPORT void glTexParameteri_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glTexParameteri( (GLenum)target, (GLenum)pname, (GLint)param );
  _checkError("glTexParameteri");
}

FABRIC_EXT_EXPORT void glTexParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glTexSubImage2D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexSubImage2D");
}

FABRIC_EXT_EXPORT void glTranslated_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
  glTranslated( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glTranslated");
}

FABRIC_EXT_EXPORT void glTranslatef_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
  glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glTranslatef");
}

FABRIC_EXT_EXPORT void glVertex2d_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
  glVertex2d( (GLdouble)x, (GLdouble)y );
  _checkError("glVertex2d");
}

FABRIC_EXT_EXPORT void glVertex2dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glVertex2dv( (const GLdouble*)&v[0] );
  _checkError("glVertex2dv");
}

FABRIC_EXT_EXPORT void glVertex2f_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
  glVertex2f( (GLfloat)x, (GLfloat)y );
  _checkError("glVertex2f");
}

FABRIC_EXT_EXPORT void glVertex2fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glVertex2fv( (const GLfloat*)&v[0] );
  _checkError("glVertex2fv");
}

FABRIC_EXT_EXPORT void glVertex2i_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
  glVertex2i( (GLint)x, (GLint)y );
  _checkError("glVertex2i");
}

FABRIC_EXT_EXPORT void glVertex2iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glVertex2iv( (const GLint*)&v[0] );
  _checkError("glVertex2iv");
}

FABRIC_EXT_EXPORT void glVertex2s_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
  glVertex2s( (GLshort)x, (GLshort)y );
  _checkError("glVertex2s");
}

FABRIC_EXT_EXPORT void glVertex2sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glVertex2sv( (const GLshort*)&v[0] );
  _checkError("glVertex2sv");
}

FABRIC_EXT_EXPORT void glVertex3d_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
  glVertex3d( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glVertex3d");
}

FABRIC_EXT_EXPORT void glVertex3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glVertex3dv( (const GLdouble*)&v[0] );
  _checkError("glVertex3dv");
}

FABRIC_EXT_EXPORT void glVertex3f_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
  glVertex3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glVertex3f");
}

FABRIC_EXT_EXPORT void glVertex3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glVertex3fv( (const GLfloat*)&v[0] );
  _checkError("glVertex3fv");
}

FABRIC_EXT_EXPORT void glVertex3i_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
  glVertex3i( (GLint)x, (GLint)y, (GLint)z );
  _checkError("glVertex3i");
}

FABRIC_EXT_EXPORT void glVertex3iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glVertex3iv( (const GLint*)&v[0] );
  _checkError("glVertex3iv");
}

FABRIC_EXT_EXPORT void glVertex3s_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
  glVertex3s( (GLshort)x, (GLshort)y, (GLshort)z );
  _checkError("glVertex3s");
}

FABRIC_EXT_EXPORT void glVertex3sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glVertex4d( (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  _checkError("glVertex4d");
}

FABRIC_EXT_EXPORT void glVertex4dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glVertex4f( (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  _checkError("glVertex4f");
}

FABRIC_EXT_EXPORT void glVertex4fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glVertex4i( (GLint)x, (GLint)y, (GLint)z, (GLint)w );
  _checkError("glVertex4i");
}

FABRIC_EXT_EXPORT void glVertex4iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glVertex4s( (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
  _checkError("glVertex4s");
}

FABRIC_EXT_EXPORT void glVertex4sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glTexSubImage3D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexSubImage3D");
}

FABRIC_EXT_EXPORT void glActiveTexture_wrapper(
  KL::Size texture
){
  _clearError();
  glActiveTexture( (GLenum)texture );
  _checkError("glActiveTexture");
}

FABRIC_EXT_EXPORT void glClientActiveTexture_wrapper(
  KL::Size texture
){
  _clearError();
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
  glCompressedTexSubImage3D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLsizei)imageSize, data );
  _checkError("glCompressedTexSubImage3D");
}

FABRIC_EXT_EXPORT void glGetCompressedTexImage_wrapper(
  KL::Size target,
  KL::Integer lod,
  KL::Data img
){
  _clearError();
  glGetCompressedTexImage( (GLenum)target, (GLint)lod, img );
  _checkError("glGetCompressedTexImage");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1d_wrapper(
  KL::Size target,
  KL::Scalar s
){
  _clearError();
  glMultiTexCoord1d( (GLenum)target, (GLdouble)s );
  _checkError("glMultiTexCoord1d");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1dv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glMultiTexCoord1dv( (GLenum)target, (const GLdouble*)&v[0] );
  _checkError("glMultiTexCoord1dv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1f_wrapper(
  KL::Size target,
  KL::Scalar s
){
  _clearError();
  glMultiTexCoord1f( (GLenum)target, (GLfloat)s );
  _checkError("glMultiTexCoord1f");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1fv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glMultiTexCoord1fv( (GLenum)target, (const GLfloat*)&v[0] );
  _checkError("glMultiTexCoord1fv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1i_wrapper(
  KL::Size target,
  KL::Integer s
){
  _clearError();
  glMultiTexCoord1i( (GLenum)target, (GLint)s );
  _checkError("glMultiTexCoord1i");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1iv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glMultiTexCoord1iv( (GLenum)target, (const GLint*)&v[0] );
  _checkError("glMultiTexCoord1iv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1s_wrapper(
  KL::Size target,
  KL::Integer s
){
  _clearError();
  glMultiTexCoord1s( (GLenum)target, (GLshort)s );
  _checkError("glMultiTexCoord1s");
}

FABRIC_EXT_EXPORT void glMultiTexCoord1sv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glMultiTexCoord1sv( (GLenum)target, (const GLshort*)&v[0] );
  _checkError("glMultiTexCoord1sv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2d_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t
){
  _clearError();
  glMultiTexCoord2d( (GLenum)target, (GLdouble)s, (GLdouble)t );
  _checkError("glMultiTexCoord2d");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2dv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glMultiTexCoord2dv( (GLenum)target, (const GLdouble*)&v[0] );
  _checkError("glMultiTexCoord2dv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2f_wrapper(
  KL::Size target,
  KL::Scalar s,
  KL::Scalar t
){
  _clearError();
  glMultiTexCoord2f( (GLenum)target, (GLfloat)s, (GLfloat)t );
  _checkError("glMultiTexCoord2f");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2fv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glMultiTexCoord2fv( (GLenum)target, (const GLfloat*)&v[0] );
  _checkError("glMultiTexCoord2fv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2i_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t
){
  _clearError();
  glMultiTexCoord2i( (GLenum)target, (GLint)s, (GLint)t );
  _checkError("glMultiTexCoord2i");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2iv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glMultiTexCoord2iv( (GLenum)target, (const GLint*)&v[0] );
  _checkError("glMultiTexCoord2iv");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2s_wrapper(
  KL::Size target,
  KL::Integer s,
  KL::Integer t
){
  _clearError();
  glMultiTexCoord2s( (GLenum)target, (GLshort)s, (GLshort)t );
  _checkError("glMultiTexCoord2s");
}

FABRIC_EXT_EXPORT void glMultiTexCoord2sv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glMultiTexCoord3d( (GLenum)target, (GLdouble)s, (GLdouble)t, (GLdouble)r );
  _checkError("glMultiTexCoord3d");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3dv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glMultiTexCoord3f( (GLenum)target, (GLfloat)s, (GLfloat)t, (GLfloat)r );
  _checkError("glMultiTexCoord3f");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3fv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glMultiTexCoord3i( (GLenum)target, (GLint)s, (GLint)t, (GLint)r );
  _checkError("glMultiTexCoord3i");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3iv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glMultiTexCoord3s( (GLenum)target, (GLshort)s, (GLshort)t, (GLshort)r );
  _checkError("glMultiTexCoord3s");
}

FABRIC_EXT_EXPORT void glMultiTexCoord3sv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glMultiTexCoord4d( (GLenum)target, (GLdouble)s, (GLdouble)t, (GLdouble)r, (GLdouble)q );
  _checkError("glMultiTexCoord4d");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4dv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glMultiTexCoord4f( (GLenum)target, (GLfloat)s, (GLfloat)t, (GLfloat)r, (GLfloat)q );
  _checkError("glMultiTexCoord4f");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4fv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glMultiTexCoord4i( (GLenum)target, (GLint)s, (GLint)t, (GLint)r, (GLint)q );
  _checkError("glMultiTexCoord4i");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4iv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glMultiTexCoord4s( (GLenum)target, (GLshort)s, (GLshort)t, (GLshort)r, (GLshort)q );
  _checkError("glMultiTexCoord4s");
}

FABRIC_EXT_EXPORT void glMultiTexCoord4sv_wrapper(
  KL::Size target,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glMultiTexCoord4sv( (GLenum)target, (const GLshort*)&v[0] );
  _checkError("glMultiTexCoord4sv");
}

FABRIC_EXT_EXPORT void glSampleCoverage_wrapper(
  KL::Scalar value,
  KL::Boolean invert
){
  _clearError();
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
  glBlendColor( (GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha );
  _checkError("glBlendColor");
}

FABRIC_EXT_EXPORT void glBlendEquation_wrapper(
  KL::Size mode
){
  _clearError();
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
  glBlendFuncSeparate( (GLenum)sfactorRGB, (GLenum)dfactorRGB, (GLenum)sfactorAlpha, (GLenum)dfactorAlpha );
  _checkError("glBlendFuncSeparate");
}

FABRIC_EXT_EXPORT void glFogCoordPointer_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
  glFogCoordPointer( (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glFogCoordPointer");
}

FABRIC_EXT_EXPORT void glFogCoordd_wrapper(
  KL::Scalar coord
){
  _clearError();
  glFogCoordd( (GLdouble)coord );
  _checkError("glFogCoordd");
}

FABRIC_EXT_EXPORT void glFogCoorddv_wrapper(
  const KL::VariableArray<KL::Scalar> & coord
){
  _clearError();
  glFogCoorddv( (const GLdouble*)&coord[0] );
  _checkError("glFogCoorddv");
}

FABRIC_EXT_EXPORT void glFogCoordf_wrapper(
  KL::Scalar coord
){
  _clearError();
  glFogCoordf( (GLfloat)coord );
  _checkError("glFogCoordf");
}

FABRIC_EXT_EXPORT void glFogCoordfv_wrapper(
  const KL::VariableArray<KL::Scalar> & coord
){
  _clearError();
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
  glMultiDrawArrays( (GLenum)mode, (GLint*)&first[0], (GLsizei*)&count[0], (GLsizei)primcount );
  _checkError("glMultiDrawArrays");
}

FABRIC_EXT_EXPORT void glPointParameterf_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glPointParameterf( (GLenum)pname, (GLfloat)param );
  _checkError("glPointParameterf");
}

FABRIC_EXT_EXPORT void glPointParameterfv_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glPointParameterfv( (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glPointParameterfv");
}

FABRIC_EXT_EXPORT void glPointParameteri_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glPointParameteri( (GLenum)pname, (GLint)param );
  _checkError("glPointParameteri");
}

FABRIC_EXT_EXPORT void glPointParameteriv_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glPointParameteriv( (GLenum)pname, (GLint*)&params[0] );
  _checkError("glPointParameteriv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3b_wrapper(
  KL::Byte red,
  KL::Byte green,
  KL::Byte blue
){
  _clearError();
  glSecondaryColor3b( (GLbyte)red, (GLbyte)green, (GLbyte)blue );
  _checkError("glSecondaryColor3b");
}

FABRIC_EXT_EXPORT void glSecondaryColor3bv_wrapper(
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
  glSecondaryColor3bv( (const GLbyte*)&v[0] );
  _checkError("glSecondaryColor3bv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3d_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue
){
  _clearError();
  glSecondaryColor3d( (GLdouble)red, (GLdouble)green, (GLdouble)blue );
  _checkError("glSecondaryColor3d");
}

FABRIC_EXT_EXPORT void glSecondaryColor3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glSecondaryColor3dv( (const GLdouble*)&v[0] );
  _checkError("glSecondaryColor3dv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3f_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue
){
  _clearError();
  glSecondaryColor3f( (GLfloat)red, (GLfloat)green, (GLfloat)blue );
  _checkError("glSecondaryColor3f");
}

FABRIC_EXT_EXPORT void glSecondaryColor3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glSecondaryColor3fv( (const GLfloat*)&v[0] );
  _checkError("glSecondaryColor3fv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3i_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
  glSecondaryColor3i( (GLint)red, (GLint)green, (GLint)blue );
  _checkError("glSecondaryColor3i");
}

FABRIC_EXT_EXPORT void glSecondaryColor3iv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glSecondaryColor3iv( (const GLint*)&v[0] );
  _checkError("glSecondaryColor3iv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3s_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
  glSecondaryColor3s( (GLshort)red, (GLshort)green, (GLshort)blue );
  _checkError("glSecondaryColor3s");
}

FABRIC_EXT_EXPORT void glSecondaryColor3sv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glSecondaryColor3sv( (const GLshort*)&v[0] );
  _checkError("glSecondaryColor3sv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3ub_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue
){
  _clearError();
  glSecondaryColor3ub( (GLubyte)red, (GLubyte)green, (GLubyte)blue );
  _checkError("glSecondaryColor3ub");
}

FABRIC_EXT_EXPORT void glSecondaryColor3ubv_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glSecondaryColor3ubv( (const GLubyte*)&v[0] );
  _checkError("glSecondaryColor3ubv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3ui_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue
){
  _clearError();
  glSecondaryColor3ui( (GLuint)red, (GLuint)green, (GLuint)blue );
  _checkError("glSecondaryColor3ui");
}

FABRIC_EXT_EXPORT void glSecondaryColor3uiv_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glSecondaryColor3uiv( (const GLuint*)&v[0] );
  _checkError("glSecondaryColor3uiv");
}

FABRIC_EXT_EXPORT void glSecondaryColor3us_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
  glSecondaryColor3us( (GLushort)red, (GLushort)green, (GLushort)blue );
  _checkError("glSecondaryColor3us");
}

FABRIC_EXT_EXPORT void glSecondaryColor3usv_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glSecondaryColorPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glSecondaryColorPointer");
}

FABRIC_EXT_EXPORT void glWindowPos2d_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
  glWindowPos2d( (GLdouble)x, (GLdouble)y );
  _checkError("glWindowPos2d");
}

FABRIC_EXT_EXPORT void glWindowPos2dv_wrapper(
  const KL::VariableArray<KL::Scalar> & p
){
  _clearError();
  glWindowPos2dv( (const GLdouble*)&p[0] );
  _checkError("glWindowPos2dv");
}

FABRIC_EXT_EXPORT void glWindowPos2f_wrapper(
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
  glWindowPos2f( (GLfloat)x, (GLfloat)y );
  _checkError("glWindowPos2f");
}

FABRIC_EXT_EXPORT void glWindowPos2fv_wrapper(
  const KL::VariableArray<KL::Scalar> & p
){
  _clearError();
  glWindowPos2fv( (const GLfloat*)&p[0] );
  _checkError("glWindowPos2fv");
}

FABRIC_EXT_EXPORT void glWindowPos2i_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
  glWindowPos2i( (GLint)x, (GLint)y );
  _checkError("glWindowPos2i");
}

FABRIC_EXT_EXPORT void glWindowPos2iv_wrapper(
  const KL::VariableArray<KL::Integer> & p
){
  _clearError();
  glWindowPos2iv( (const GLint*)&p[0] );
  _checkError("glWindowPos2iv");
}

FABRIC_EXT_EXPORT void glWindowPos2s_wrapper(
  KL::Integer x,
  KL::Integer y
){
  _clearError();
  glWindowPos2s( (GLshort)x, (GLshort)y );
  _checkError("glWindowPos2s");
}

FABRIC_EXT_EXPORT void glWindowPos2sv_wrapper(
  const KL::VariableArray<KL::Integer> & p
){
  _clearError();
  glWindowPos2sv( (const GLshort*)&p[0] );
  _checkError("glWindowPos2sv");
}

FABRIC_EXT_EXPORT void glWindowPos3d_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
  glWindowPos3d( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glWindowPos3d");
}

FABRIC_EXT_EXPORT void glWindowPos3dv_wrapper(
  const KL::VariableArray<KL::Scalar> & p
){
  _clearError();
  glWindowPos3dv( (const GLdouble*)&p[0] );
  _checkError("glWindowPos3dv");
}

FABRIC_EXT_EXPORT void glWindowPos3f_wrapper(
  KL::Scalar x,
  KL::Scalar y,
  KL::Scalar z
){
  _clearError();
  glWindowPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glWindowPos3f");
}

FABRIC_EXT_EXPORT void glWindowPos3fv_wrapper(
  const KL::VariableArray<KL::Scalar> & p
){
  _clearError();
  glWindowPos3fv( (const GLfloat*)&p[0] );
  _checkError("glWindowPos3fv");
}

FABRIC_EXT_EXPORT void glWindowPos3i_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
  glWindowPos3i( (GLint)x, (GLint)y, (GLint)z );
  _checkError("glWindowPos3i");
}

FABRIC_EXT_EXPORT void glWindowPos3iv_wrapper(
  const KL::VariableArray<KL::Integer> & p
){
  _clearError();
  glWindowPos3iv( (const GLint*)&p[0] );
  _checkError("glWindowPos3iv");
}

FABRIC_EXT_EXPORT void glWindowPos3s_wrapper(
  KL::Integer x,
  KL::Integer y,
  KL::Integer z
){
  _clearError();
  glWindowPos3s( (GLshort)x, (GLshort)y, (GLshort)z );
  _checkError("glWindowPos3s");
}

FABRIC_EXT_EXPORT void glWindowPos3sv_wrapper(
  const KL::VariableArray<KL::Integer> & p
){
  _clearError();
  glWindowPos3sv( (const GLshort*)&p[0] );
  _checkError("glWindowPos3sv");
}

FABRIC_EXT_EXPORT void glBeginQuery_wrapper(
  KL::Size target,
  KL::Size id
){
  _clearError();
  glBeginQuery( (GLenum)target, (GLuint)id );
  _checkError("glBeginQuery");
}

FABRIC_EXT_EXPORT void glBindBuffer_wrapper(
  KL::Size target,
  KL::Size buffer
){
  _clearError();
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
  glBufferSubData( (GLenum)target, (GLintptr)offset, (GLsizeiptr)size, data );
  _checkError("glBufferSubData");
}

FABRIC_EXT_EXPORT void glDeleteBuffers_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & buffers
){
  _clearError();
  glDeleteBuffers( (GLsizei)n, (const GLuint*)&buffers[0] );
  _checkError("glDeleteBuffers");
}

FABRIC_EXT_EXPORT void glDeleteQueries_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & ids
){
  _clearError();
  glDeleteQueries( (GLsizei)n, (const GLuint*)&ids[0] );
  _checkError("glDeleteQueries");
}

FABRIC_EXT_EXPORT void glEndQuery_wrapper(
  KL::Size target
){
  _clearError();
  glEndQuery( (GLenum)target );
  _checkError("glEndQuery");
}

FABRIC_EXT_EXPORT void glGenBuffers_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & buffers
){
  _clearError();
  glGenBuffers( (GLsizei)n, (GLuint*)&buffers[0] );
  _checkError("glGenBuffers");
}

FABRIC_EXT_EXPORT void glGenQueries_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & ids
){
  _clearError();
  glGenQueries( (GLsizei)n, (GLuint*)&ids[0] );
  _checkError("glGenQueries");
}

FABRIC_EXT_EXPORT void glGetBufferParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glGetBufferSubData( (GLenum)target, (GLintptr)offset, (GLsizeiptr)size, data );
  _checkError("glGetBufferSubData");
}

FABRIC_EXT_EXPORT void glGetQueryObjectiv_wrapper(
  KL::Size id,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetQueryObjectiv( (GLuint)id, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetQueryObjectiv");
}

FABRIC_EXT_EXPORT void glGetQueryObjectuiv_wrapper(
  KL::Size id,
  KL::Size pname,
  KL::VariableArray<KL::Size> & params
){
  _clearError();
  glGetQueryObjectuiv( (GLuint)id, (GLenum)pname, (GLuint*)&params[0] );
  _checkError("glGetQueryObjectuiv");
}

FABRIC_EXT_EXPORT void glGetQueryiv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetQueryiv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetQueryiv");
}

FABRIC_EXT_EXPORT KL::Boolean glIsBuffer_wrapper(
  KL::Size buffer
){
  _clearError();
  GLboolean result = glIsBuffer( (GLuint)buffer );
  _checkError("glIsBuffer");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glIsQuery_wrapper(
  KL::Size id
){
  _clearError();
  GLboolean result = glIsQuery( (GLuint)id );
  _checkError("glIsQuery");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT GLvoid* glMapBuffer_wrapper(
  KL::Size target,
  KL::Size access
){
  _clearError();
  glMapBuffer( (GLenum)target, (GLenum)access );
  _checkError("glMapBuffer");
}

FABRIC_EXT_EXPORT KL::Boolean glUnmapBuffer_wrapper(
  KL::Size target
){
  _clearError();
  GLboolean result = glUnmapBuffer( (GLenum)target );
  _checkError("glUnmapBuffer");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glAttachShader_wrapper(
  KL::Size program,
  KL::Size shader
){
  _clearError();
  glAttachShader( (GLuint)program, (GLuint)shader );
  _checkError("glAttachShader");
}

FABRIC_EXT_EXPORT void glBindAttribLocation_wrapper(
  KL::Size program,
  KL::Size index,
  const KL::String &name
){
  _clearError();
  glBindAttribLocation( (GLuint)program, (GLuint)index, (const GLchar*)name.data() );
  _checkError("glBindAttribLocation");
}

FABRIC_EXT_EXPORT void glBlendEquationSeparate_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
  glBlendEquationSeparate( (GLenum)var0, (GLenum)var1 );
  _checkError("glBlendEquationSeparate");
}

FABRIC_EXT_EXPORT void glCompileShader_wrapper(
  KL::Size shader
){
  _clearError();
  glCompileShader( (GLuint)shader );
  _checkError("glCompileShader");
}

FABRIC_EXT_EXPORT KL::Size glCreateProgram_wrapper()
{
  _clearError();
  GLuint result = glCreateProgram();
  _checkError("glCreateProgram");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size glCreateShader_wrapper(
  KL::Size type
){
  _clearError();
  GLuint result = glCreateShader( (GLenum)type );
  _checkError("glCreateShader");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glDeleteProgram_wrapper(
  KL::Size program
){
  _clearError();
  glDeleteProgram( (GLuint)program );
  _checkError("glDeleteProgram");
}

FABRIC_EXT_EXPORT void glDeleteShader_wrapper(
  KL::Size shader
){
  _clearError();
  glDeleteShader( (GLuint)shader );
  _checkError("glDeleteShader");
}

FABRIC_EXT_EXPORT void glDetachShader_wrapper(
  KL::Size program,
  KL::Size shader
){
  _clearError();
  glDetachShader( (GLuint)program, (GLuint)shader );
  _checkError("glDetachShader");
}

FABRIC_EXT_EXPORT void glDisableVertexAttribArray_wrapper(
  KL::Size var0
){
  _clearError();
  glDisableVertexAttribArray( (GLuint)var0 );
  _checkError("glDisableVertexAttribArray");
}

FABRIC_EXT_EXPORT void glDrawBuffers_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & bufs
){
  _clearError();
  glDrawBuffers( (GLsizei)n, (const GLenum*)&bufs[0] );
  _checkError("glDrawBuffers");
}

FABRIC_EXT_EXPORT void glEnableVertexAttribArray_wrapper(
  KL::Size var0
){
  _clearError();
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
  glGetAttachedShaders( (GLuint)program, (GLsizei)maxCount, (GLsizei*)&count[0], (GLuint*)&shaders[0] );
  _checkError("glGetAttachedShaders");
}

FABRIC_EXT_EXPORT KL::Integer glGetAttribLocation_wrapper(
  KL::Size program,
  const KL::String &name
){
  _clearError();
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
  glGetShaderiv( (GLuint)shader, (GLenum)pname, (GLint*)&param[0] );
  _checkError("glGetShaderiv");
}

FABRIC_EXT_EXPORT KL::Integer glGetUniformLocation_wrapper(
  KL::Size program,
  const KL::String &name
){
  _clearError();
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
  glGetUniformfv( (GLuint)program, (GLint)location, (GLfloat*)&params[0] );
  _checkError("glGetUniformfv");
}

FABRIC_EXT_EXPORT void glGetUniformiv_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetUniformiv( (GLuint)program, (GLint)location, (GLint*)&params[0] );
  _checkError("glGetUniformiv");
}

FABRIC_EXT_EXPORT void glGetVertexAttribPointerv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::Data var2
){
  _clearError();
  glGetVertexAttribPointerv( (GLuint)var0, (GLenum)var1, var2 );
  _checkError("glGetVertexAttribPointerv");
}

FABRIC_EXT_EXPORT void glGetVertexAttribdv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Scalar> & var2
){
  _clearError();
  glGetVertexAttribdv( (GLuint)var0, (GLenum)var1, (GLdouble*)&var2[0] );
  _checkError("glGetVertexAttribdv");
}

FABRIC_EXT_EXPORT void glGetVertexAttribfv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Scalar> & var2
){
  _clearError();
  glGetVertexAttribfv( (GLuint)var0, (GLenum)var1, (GLfloat*)&var2[0] );
  _checkError("glGetVertexAttribfv");
}

FABRIC_EXT_EXPORT void glGetVertexAttribiv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Integer> & var2
){
  _clearError();
  glGetVertexAttribiv( (GLuint)var0, (GLenum)var1, (GLint*)&var2[0] );
  _checkError("glGetVertexAttribiv");
}

FABRIC_EXT_EXPORT KL::Boolean glIsProgram_wrapper(
  KL::Size program
){
  _clearError();
  GLboolean result = glIsProgram( (GLuint)program );
  _checkError("glIsProgram");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glIsShader_wrapper(
  KL::Size shader
){
  _clearError();
  GLboolean result = glIsShader( (GLuint)shader );
  _checkError("glIsShader");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glLinkProgram_wrapper(
  KL::Size program
){
  _clearError();
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
  glShaderSource( (GLuint)shader, (GLsizei)count, (const GLchar**)&strings[0], (const GLint*)&lengths[0] );
  _checkError("glShaderSource");
}

FABRIC_EXT_EXPORT void glStencilFuncSeparate_wrapper(
  KL::Size frontfunc,
  KL::Size backfunc,
  KL::Integer ref,
  KL::Size mask
){
  _clearError();
  glStencilFuncSeparate( (GLenum)frontfunc, (GLenum)backfunc, (GLint)ref, (GLuint)mask );
  _checkError("glStencilFuncSeparate");
}

FABRIC_EXT_EXPORT void glStencilMaskSeparate_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
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
  glStencilOpSeparate( (GLenum)face, (GLenum)sfail, (GLenum)dpfail, (GLenum)dppass );
  _checkError("glStencilOpSeparate");
}

FABRIC_EXT_EXPORT void glUniform1f_wrapper(
  KL::Integer location,
  KL::Scalar v0
){
  _clearError();
  glUniform1f( (GLint)location, (GLfloat)v0 );
  _checkError("glUniform1f");
}

FABRIC_EXT_EXPORT void glUniform1fv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glUniform1fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glUniform1fv");
}

FABRIC_EXT_EXPORT void glUniform1i_wrapper(
  KL::Integer location,
  KL::Integer v0
){
  _clearError();
  glUniform1i( (GLint)location, (GLint)v0 );
  _checkError("glUniform1i");
}

FABRIC_EXT_EXPORT void glUniform1iv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
  glUniform1iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glUniform1iv");
}

FABRIC_EXT_EXPORT void glUniform2f_wrapper(
  KL::Integer location,
  KL::Scalar v0,
  KL::Scalar v1
){
  _clearError();
  glUniform2f( (GLint)location, (GLfloat)v0, (GLfloat)v1 );
  _checkError("glUniform2f");
}

FABRIC_EXT_EXPORT void glUniform2fv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glUniform2fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glUniform2fv");
}

FABRIC_EXT_EXPORT void glUniform2i_wrapper(
  KL::Integer location,
  KL::Integer v0,
  KL::Integer v1
){
  _clearError();
  glUniform2i( (GLint)location, (GLint)v0, (GLint)v1 );
  _checkError("glUniform2i");
}

FABRIC_EXT_EXPORT void glUniform2iv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
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
  glUniform3f( (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2 );
  _checkError("glUniform3f");
}

FABRIC_EXT_EXPORT void glUniform3fv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
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
  glUniform3i( (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2 );
  _checkError("glUniform3i");
}

FABRIC_EXT_EXPORT void glUniform3iv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
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
  glUniform4f( (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2, (GLfloat)v3 );
  _checkError("glUniform4f");
}

FABRIC_EXT_EXPORT void glUniform4fv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
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
  glUniform4i( (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2, (GLint)v3 );
  _checkError("glUniform4i");
}

FABRIC_EXT_EXPORT void glUniform4iv_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Integer> & value
){
  _clearError();
  glUniform4iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glUniform4iv");
}

FABRIC_EXT_EXPORT void glUniformMatrix2fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glUniformMatrix2fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix2fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix3fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glUniformMatrix3fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix3fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix4fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glUniformMatrix4fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix4fv");
}

FABRIC_EXT_EXPORT void glUseProgram_wrapper(
  KL::Size program
){
  _clearError();
  glUseProgram( (GLuint)program );
  _checkError("glUseProgram");
}

FABRIC_EXT_EXPORT void glValidateProgram_wrapper(
  KL::Size program
){
  _clearError();
  glValidateProgram( (GLuint)program );
  _checkError("glValidateProgram");
}

FABRIC_EXT_EXPORT void glVertexAttrib1d_wrapper(
  KL::Size index,
  KL::Scalar x
){
  _clearError();
  glVertexAttrib1d( (GLuint)index, (GLdouble)x );
  _checkError("glVertexAttrib1d");
}

FABRIC_EXT_EXPORT void glVertexAttrib1dv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glVertexAttrib1dv( (GLuint)index, (const GLdouble*)&v[0] );
  _checkError("glVertexAttrib1dv");
}

FABRIC_EXT_EXPORT void glVertexAttrib1f_wrapper(
  KL::Size index,
  KL::Scalar x
){
  _clearError();
  glVertexAttrib1f( (GLuint)index, (GLfloat)x );
  _checkError("glVertexAttrib1f");
}

FABRIC_EXT_EXPORT void glVertexAttrib1fv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glVertexAttrib1fv( (GLuint)index, (const GLfloat*)&v[0] );
  _checkError("glVertexAttrib1fv");
}

FABRIC_EXT_EXPORT void glVertexAttrib1s_wrapper(
  KL::Size index,
  KL::Integer x
){
  _clearError();
  glVertexAttrib1s( (GLuint)index, (GLshort)x );
  _checkError("glVertexAttrib1s");
}

FABRIC_EXT_EXPORT void glVertexAttrib1sv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glVertexAttrib1sv( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib1sv");
}

FABRIC_EXT_EXPORT void glVertexAttrib2d_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
  glVertexAttrib2d( (GLuint)index, (GLdouble)x, (GLdouble)y );
  _checkError("glVertexAttrib2d");
}

FABRIC_EXT_EXPORT void glVertexAttrib2dv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glVertexAttrib2dv( (GLuint)index, (const GLdouble*)&v[0] );
  _checkError("glVertexAttrib2dv");
}

FABRIC_EXT_EXPORT void glVertexAttrib2f_wrapper(
  KL::Size index,
  KL::Scalar x,
  KL::Scalar y
){
  _clearError();
  glVertexAttrib2f( (GLuint)index, (GLfloat)x, (GLfloat)y );
  _checkError("glVertexAttrib2f");
}

FABRIC_EXT_EXPORT void glVertexAttrib2fv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glVertexAttrib2fv( (GLuint)index, (const GLfloat*)&v[0] );
  _checkError("glVertexAttrib2fv");
}

FABRIC_EXT_EXPORT void glVertexAttrib2s_wrapper(
  KL::Size index,
  KL::Integer x,
  KL::Integer y
){
  _clearError();
  glVertexAttrib2s( (GLuint)index, (GLshort)x, (GLshort)y );
  _checkError("glVertexAttrib2s");
}

FABRIC_EXT_EXPORT void glVertexAttrib2sv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glVertexAttrib3d( (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z );
  _checkError("glVertexAttrib3d");
}

FABRIC_EXT_EXPORT void glVertexAttrib3dv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glVertexAttrib3f( (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  _checkError("glVertexAttrib3f");
}

FABRIC_EXT_EXPORT void glVertexAttrib3fv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glVertexAttrib3s( (GLuint)index, (GLshort)x, (GLshort)y, (GLshort)z );
  _checkError("glVertexAttrib3s");
}

FABRIC_EXT_EXPORT void glVertexAttrib3sv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glVertexAttrib3sv( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib3sv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Nbv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
  glVertexAttrib4Nbv( (GLuint)index, (const GLbyte*)&v[0] );
  _checkError("glVertexAttrib4Nbv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Niv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glVertexAttrib4Niv( (GLuint)index, (const GLint*)&v[0] );
  _checkError("glVertexAttrib4Niv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Nsv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glVertexAttrib4Nub( (GLuint)index, (GLubyte)x, (GLubyte)y, (GLubyte)z, (GLubyte)w );
  _checkError("glVertexAttrib4Nub");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Nubv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glVertexAttrib4Nubv( (GLuint)index, (const GLubyte*)&v[0] );
  _checkError("glVertexAttrib4Nubv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Nuiv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glVertexAttrib4Nuiv( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttrib4Nuiv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4Nusv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glVertexAttrib4Nusv( (GLuint)index, (const GLushort*)&v[0] );
  _checkError("glVertexAttrib4Nusv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4bv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
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
  glVertexAttrib4d( (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  _checkError("glVertexAttrib4d");
}

FABRIC_EXT_EXPORT void glVertexAttrib4dv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
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
  glVertexAttrib4f( (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  _checkError("glVertexAttrib4f");
}

FABRIC_EXT_EXPORT void glVertexAttrib4fv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glVertexAttrib4fv( (GLuint)index, (const GLfloat*)&v[0] );
  _checkError("glVertexAttrib4fv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4iv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glVertexAttrib4s( (GLuint)index, (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
  _checkError("glVertexAttrib4s");
}

FABRIC_EXT_EXPORT void glVertexAttrib4sv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glVertexAttrib4sv( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttrib4sv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4ubv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glVertexAttrib4ubv( (GLuint)index, (const GLubyte*)&v[0] );
  _checkError("glVertexAttrib4ubv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4uiv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glVertexAttrib4uiv( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttrib4uiv");
}

FABRIC_EXT_EXPORT void glVertexAttrib4usv_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glVertexAttribPointer( (GLuint)index, (GLint)size, (GLenum)type, (GLboolean)normalized, (GLsizei)stride, pointer );
  _checkError("glVertexAttribPointer");
}

FABRIC_EXT_EXPORT void glUniformMatrix2x3fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glUniformMatrix2x3fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix2x3fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix2x4fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glUniformMatrix2x4fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix2x4fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix3x2fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glUniformMatrix3x2fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix3x2fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix3x4fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glUniformMatrix3x4fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix3x4fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix4x2fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glUniformMatrix4x2fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix4x2fv");
}

FABRIC_EXT_EXPORT void glUniformMatrix4x3fv_wrapper(
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glUniformMatrix4x3fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glUniformMatrix4x3fv");
}

FABRIC_EXT_EXPORT void glBeginConditionalRender_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
  glBeginConditionalRender( (GLuint)var0, (GLenum)var1 );
  _checkError("glBeginConditionalRender");
}

FABRIC_EXT_EXPORT void glBeginTransformFeedback_wrapper(
  KL::Size var0
){
  _clearError();
  glBeginTransformFeedback( (GLenum)var0 );
  _checkError("glBeginTransformFeedback");
}

FABRIC_EXT_EXPORT void glBindFragDataLocation_wrapper(
  KL::Size var0,
  KL::Size var1,
  const KL::String &var2
){
  _clearError();
  glBindFragDataLocation( (GLuint)var0, (GLuint)var1, (const GLchar*)var2.data() );
  _checkError("glBindFragDataLocation");
}

FABRIC_EXT_EXPORT void glClampColor_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
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
  glClearBufferfi( (GLenum)var0, (GLint)var1, (GLfloat)var2, (GLint)var3 );
  _checkError("glClearBufferfi");
}

FABRIC_EXT_EXPORT void glClearBufferfv_wrapper(
  KL::Size var0,
  KL::Integer var1,
  const KL::VariableArray<KL::Scalar> & var2
){
  _clearError();
  glClearBufferfv( (GLenum)var0, (GLint)var1, (const GLfloat*)&var2[0] );
  _checkError("glClearBufferfv");
}

FABRIC_EXT_EXPORT void glClearBufferiv_wrapper(
  KL::Size var0,
  KL::Integer var1,
  const KL::VariableArray<KL::Integer> & var2
){
  _clearError();
  glClearBufferiv( (GLenum)var0, (GLint)var1, (const GLint*)&var2[0] );
  _checkError("glClearBufferiv");
}

FABRIC_EXT_EXPORT void glClearBufferuiv_wrapper(
  KL::Size var0,
  KL::Integer var1,
  const KL::VariableArray<KL::Size> & var2
){
  _clearError();
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
  glColorMaski( (GLuint)var0, (GLboolean)var1, (GLboolean)var2, (GLboolean)var3, (GLboolean)var4 );
  _checkError("glColorMaski");
}

FABRIC_EXT_EXPORT void glDisablei_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
  glDisablei( (GLenum)var0, (GLuint)var1 );
  _checkError("glDisablei");
}

FABRIC_EXT_EXPORT void glEnablei_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
  glEnablei( (GLenum)var0, (GLuint)var1 );
  _checkError("glEnablei");
}

FABRIC_EXT_EXPORT void glEndConditionalRender_wrapper()
{
  _clearError();
  glEndConditionalRender();
  _checkError("glEndConditionalRender");
}

FABRIC_EXT_EXPORT void glEndTransformFeedback_wrapper()
{
  _clearError();
  glEndTransformFeedback();
  _checkError("glEndTransformFeedback");
}

FABRIC_EXT_EXPORT void glGetBooleani_v_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Boolean> & var2
){
  _clearError();
  glGetBooleani_v( (GLenum)var0, (GLuint)var1, (GLboolean*)&var2[0] );
  _checkError("glGetBooleani_v");
}

FABRIC_EXT_EXPORT KL::Integer glGetFragDataLocation_wrapper(
  KL::Size var0,
  const KL::String &var1
){
  _clearError();
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
  glGetTexParameterIiv( (GLenum)var0, (GLenum)var1, (GLint*)&var2[0] );
  _checkError("glGetTexParameterIiv");
}

FABRIC_EXT_EXPORT void glGetTexParameterIuiv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Size> & var2
){
  _clearError();
  glGetTexParameterIuiv( (GLenum)var0, (GLenum)var1, (GLuint*)&var2[0] );
  _checkError("glGetTexParameterIuiv");
}

FABRIC_EXT_EXPORT void glGetTransformFeedbackVarying_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Integer> & var2
){
  _clearError();
  glGetTransformFeedbackVarying( (GLuint)var0, (GLuint)var1, (GLint*)&var2[0] );
  _checkError("glGetTransformFeedbackVarying");
}

FABRIC_EXT_EXPORT void glGetUniformuiv_wrapper(
  KL::Size var0,
  KL::Integer var1,
  KL::VariableArray<KL::Size> & var2
){
  _clearError();
  glGetUniformuiv( (GLuint)var0, (GLint)var1, (GLuint*)&var2[0] );
  _checkError("glGetUniformuiv");
}

FABRIC_EXT_EXPORT void glGetVertexAttribIiv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Integer> & var2
){
  _clearError();
  glGetVertexAttribIiv( (GLuint)var0, (GLenum)var1, (GLint*)&var2[0] );
  _checkError("glGetVertexAttribIiv");
}

FABRIC_EXT_EXPORT void glGetVertexAttribIuiv_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::VariableArray<KL::Size> & var2
){
  _clearError();
  glGetVertexAttribIuiv( (GLuint)var0, (GLenum)var1, (GLuint*)&var2[0] );
  _checkError("glGetVertexAttribIuiv");
}

FABRIC_EXT_EXPORT KL::Boolean glIsEnabledi_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
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
  glTexParameterIiv( (GLenum)var0, (GLenum)var1, (const GLint*)&var2[0] );
  _checkError("glTexParameterIiv");
}

FABRIC_EXT_EXPORT void glTexParameterIuiv_wrapper(
  KL::Size var0,
  KL::Size var1,
  const KL::VariableArray<KL::Size> & var2
){
  _clearError();
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
  glTransformFeedbackVaryings( (GLuint)var0, (GLsizei)var1, (const GLchar**)&var2[0], (GLenum)var3 );
  _checkError("glTransformFeedbackVaryings");
}

FABRIC_EXT_EXPORT void glUniform1ui_wrapper(
  KL::Integer var0,
  KL::Size var1
){
  _clearError();
  glUniform1ui( (GLint)var0, (GLuint)var1 );
  _checkError("glUniform1ui");
}

FABRIC_EXT_EXPORT void glUniform1uiv_wrapper(
  KL::Integer var0,
  KL::Size var1,
  const KL::VariableArray<KL::Size> & var2
){
  _clearError();
  glUniform1uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
  _checkError("glUniform1uiv");
}

FABRIC_EXT_EXPORT void glUniform2ui_wrapper(
  KL::Integer var0,
  KL::Size var1,
  KL::Size var2
){
  _clearError();
  glUniform2ui( (GLint)var0, (GLuint)var1, (GLuint)var2 );
  _checkError("glUniform2ui");
}

FABRIC_EXT_EXPORT void glUniform2uiv_wrapper(
  KL::Integer var0,
  KL::Size var1,
  const KL::VariableArray<KL::Size> & var2
){
  _clearError();
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
  glUniform3ui( (GLint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3 );
  _checkError("glUniform3ui");
}

FABRIC_EXT_EXPORT void glUniform3uiv_wrapper(
  KL::Integer var0,
  KL::Size var1,
  const KL::VariableArray<KL::Size> & var2
){
  _clearError();
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
  glUniform4ui( (GLint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3, (GLuint)var4 );
  _checkError("glUniform4ui");
}

FABRIC_EXT_EXPORT void glUniform4uiv_wrapper(
  KL::Integer var0,
  KL::Size var1,
  const KL::VariableArray<KL::Size> & var2
){
  _clearError();
  glUniform4uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
  _checkError("glUniform4uiv");
}

FABRIC_EXT_EXPORT void glVertexAttribI1i_wrapper(
  KL::Size var0,
  KL::Integer var1
){
  _clearError();
  glVertexAttribI1i( (GLuint)var0, (GLint)var1 );
  _checkError("glVertexAttribI1i");
}

FABRIC_EXT_EXPORT void glVertexAttribI1iv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Integer> & var1
){
  _clearError();
  glVertexAttribI1iv( (GLuint)var0, (const GLint*)&var1[0] );
  _checkError("glVertexAttribI1iv");
}

FABRIC_EXT_EXPORT void glVertexAttribI1ui_wrapper(
  KL::Size var0,
  KL::Size var1
){
  _clearError();
  glVertexAttribI1ui( (GLuint)var0, (GLuint)var1 );
  _checkError("glVertexAttribI1ui");
}

FABRIC_EXT_EXPORT void glVertexAttribI1uiv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Size> & var1
){
  _clearError();
  glVertexAttribI1uiv( (GLuint)var0, (const GLuint*)&var1[0] );
  _checkError("glVertexAttribI1uiv");
}

FABRIC_EXT_EXPORT void glVertexAttribI2i_wrapper(
  KL::Size var0,
  KL::Integer var1,
  KL::Integer var2
){
  _clearError();
  glVertexAttribI2i( (GLuint)var0, (GLint)var1, (GLint)var2 );
  _checkError("glVertexAttribI2i");
}

FABRIC_EXT_EXPORT void glVertexAttribI2iv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Integer> & var1
){
  _clearError();
  glVertexAttribI2iv( (GLuint)var0, (const GLint*)&var1[0] );
  _checkError("glVertexAttribI2iv");
}

FABRIC_EXT_EXPORT void glVertexAttribI2ui_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::Size var2
){
  _clearError();
  glVertexAttribI2ui( (GLuint)var0, (GLuint)var1, (GLuint)var2 );
  _checkError("glVertexAttribI2ui");
}

FABRIC_EXT_EXPORT void glVertexAttribI2uiv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Size> & var1
){
  _clearError();
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
  glVertexAttribI3i( (GLuint)var0, (GLint)var1, (GLint)var2, (GLint)var3 );
  _checkError("glVertexAttribI3i");
}

FABRIC_EXT_EXPORT void glVertexAttribI3iv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Integer> & var1
){
  _clearError();
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
  glVertexAttribI3ui( (GLuint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3 );
  _checkError("glVertexAttribI3ui");
}

FABRIC_EXT_EXPORT void glVertexAttribI3uiv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Size> & var1
){
  _clearError();
  glVertexAttribI3uiv( (GLuint)var0, (const GLuint*)&var1[0] );
  _checkError("glVertexAttribI3uiv");
}

FABRIC_EXT_EXPORT void glVertexAttribI4bv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Byte> & var1
){
  _clearError();
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
  glVertexAttribI4i( (GLuint)var0, (GLint)var1, (GLint)var2, (GLint)var3, (GLint)var4 );
  _checkError("glVertexAttribI4i");
}

FABRIC_EXT_EXPORT void glVertexAttribI4iv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Integer> & var1
){
  _clearError();
  glVertexAttribI4iv( (GLuint)var0, (const GLint*)&var1[0] );
  _checkError("glVertexAttribI4iv");
}

FABRIC_EXT_EXPORT void glVertexAttribI4sv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Integer> & var1
){
  _clearError();
  glVertexAttribI4sv( (GLuint)var0, (const GLshort*)&var1[0] );
  _checkError("glVertexAttribI4sv");
}

FABRIC_EXT_EXPORT void glVertexAttribI4ubv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Size> & var1
){
  _clearError();
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
  glVertexAttribI4ui( (GLuint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3, (GLuint)var4 );
  _checkError("glVertexAttribI4ui");
}

FABRIC_EXT_EXPORT void glVertexAttribI4uiv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Size> & var1
){
  _clearError();
  glVertexAttribI4uiv( (GLuint)var0, (const GLuint*)&var1[0] );
  _checkError("glVertexAttribI4uiv");
}

FABRIC_EXT_EXPORT void glVertexAttribI4usv_wrapper(
  KL::Size var0,
  const KL::VariableArray<KL::Integer> & var1
){
  _clearError();
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
  glDrawElementsInstanced( (GLenum)var0, (GLsizei)var1, (GLenum)var2, var3, (GLsizei)var4 );
  _checkError("glDrawElementsInstanced");
}

FABRIC_EXT_EXPORT void glPrimitiveRestartIndex_wrapper(
  KL::Size var0
){
  _clearError();
  glPrimitiveRestartIndex( (GLuint)var0 );
  _checkError("glPrimitiveRestartIndex");
}

FABRIC_EXT_EXPORT void glTexBuffer_wrapper(
  KL::Size var0,
  KL::Size var1,
  KL::Size var2
){
  _clearError();
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
  glFramebufferTexture( (GLenum)var0, (GLenum)var1, (GLuint)var2, (GLint)var3 );
  _checkError("glFramebufferTexture");
}

FABRIC_EXT_EXPORT void glTbufferMask3DFX_wrapper(
  KL::Size mask
){
  _clearError();
  glTbufferMask3DFX( (GLuint)mask );
  _checkError("glTbufferMask3DFX");
}

FABRIC_EXT_EXPORT void glCopyBufferSubData_wrapper(
  KL::Size readtarget,
  KL::Size writetarget,
  KL::Integer readoffset,
  KL::Integer writeoffset,
  KL::Size size
){
  _clearError();
  glCopyBufferSubData( (GLenum)readtarget, (GLenum)writetarget, (GLintptr)readoffset, (GLintptr)writeoffset, (GLsizeiptr)size );
  _checkError("glCopyBufferSubData");
}

FABRIC_EXT_EXPORT void glDrawElementsBaseVertex_wrapper(
  KL::Size mode,
  KL::Size count,
  KL::Size type,
  KL::Data indices,
  KL::Integer basevertex
){
  _clearError();
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
  glDrawRangeElementsBaseVertex( (GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, indices, (GLint)basevertex );
  _checkError("glDrawRangeElementsBaseVertex");
}

FABRIC_EXT_EXPORT void glBindFramebuffer_wrapper(
  KL::Size target,
  KL::Size framebuffer
){
  _clearError();
  glBindFramebuffer( (GLenum)target, (GLuint)framebuffer );
  _checkError("glBindFramebuffer");
}

FABRIC_EXT_EXPORT void glBindRenderbuffer_wrapper(
  KL::Size target,
  KL::Size renderbuffer
){
  _clearError();
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
  glBlitFramebuffer( (GLint)srcX0, (GLint)srcY0, (GLint)srcX1, (GLint)srcY1, (GLint)dstX0, (GLint)dstY0, (GLint)dstX1, (GLint)dstY1, (GLbitfield)mask, (GLenum)filter );
  _checkError("glBlitFramebuffer");
}

FABRIC_EXT_EXPORT KL::Size glCheckFramebufferStatus_wrapper(
  KL::Size target
){
  _clearError();
  GLenum result = glCheckFramebufferStatus( (GLenum)target );
  _checkError("glCheckFramebufferStatus");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glDeleteFramebuffers_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & framebuffers
){
  _clearError();
  glDeleteFramebuffers( (GLsizei)n, (const GLuint*)&framebuffers[0] );
  _checkError("glDeleteFramebuffers");
}

FABRIC_EXT_EXPORT void glDeleteRenderbuffers_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & renderbuffers
){
  _clearError();
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
  glFramebufferTextureLayer( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer );
  _checkError("glFramebufferTextureLayer");
}

FABRIC_EXT_EXPORT void glGenFramebuffers_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & framebuffers
){
  _clearError();
  glGenFramebuffers( (GLsizei)n, (GLuint*)&framebuffers[0] );
  _checkError("glGenFramebuffers");
}

FABRIC_EXT_EXPORT void glGenRenderbuffers_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & renderbuffers
){
  _clearError();
  glGenRenderbuffers( (GLsizei)n, (GLuint*)&renderbuffers[0] );
  _checkError("glGenRenderbuffers");
}

FABRIC_EXT_EXPORT void glGenerateMipmap_wrapper(
  KL::Size target
){
  _clearError();
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
  glGetFramebufferAttachmentParameteriv( (GLenum)target, (GLenum)attachment, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetFramebufferAttachmentParameteriv");
}

FABRIC_EXT_EXPORT void glGetRenderbufferParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetRenderbufferParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetRenderbufferParameteriv");
}

FABRIC_EXT_EXPORT KL::Boolean glIsFramebuffer_wrapper(
  KL::Size framebuffer
){
  _clearError();
  GLboolean result = glIsFramebuffer( (GLuint)framebuffer );
  _checkError("glIsFramebuffer");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glIsRenderbuffer_wrapper(
  KL::Size renderbuffer
){
  _clearError();
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
  glRenderbufferStorageMultisample( (GLenum)target, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  _checkError("glRenderbufferStorageMultisample");
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
  glColorTable( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLenum)format, (GLenum)type, table );
  _checkError("glColorTable");
}

FABRIC_EXT_EXPORT void glColorTableParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glColorTableParameterfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glColorTableParameterfv");
}

FABRIC_EXT_EXPORT void glColorTableParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glConvolutionFilter2D( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, image );
  _checkError("glConvolutionFilter2D");
}

FABRIC_EXT_EXPORT void glConvolutionParameterf_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Scalar params
){
  _clearError();
  glConvolutionParameterf( (GLenum)target, (GLenum)pname, (GLfloat)params );
  _checkError("glConvolutionParameterf");
}

FABRIC_EXT_EXPORT void glConvolutionParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glConvolutionParameterfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glConvolutionParameterfv");
}

FABRIC_EXT_EXPORT void glConvolutionParameteri_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Integer params
){
  _clearError();
  glConvolutionParameteri( (GLenum)target, (GLenum)pname, (GLint)params );
  _checkError("glConvolutionParameteri");
}

FABRIC_EXT_EXPORT void glConvolutionParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glGetColorTable( (GLenum)target, (GLenum)format, (GLenum)type, table );
  _checkError("glGetColorTable");
}

FABRIC_EXT_EXPORT void glGetColorTableParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetColorTableParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetColorTableParameterfv");
}

FABRIC_EXT_EXPORT void glGetColorTableParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glGetConvolutionFilter( (GLenum)target, (GLenum)format, (GLenum)type, image );
  _checkError("glGetConvolutionFilter");
}

FABRIC_EXT_EXPORT void glGetConvolutionParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetConvolutionParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetConvolutionParameterfv");
}

FABRIC_EXT_EXPORT void glGetConvolutionParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glGetHistogram( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, values );
  _checkError("glGetHistogram");
}

FABRIC_EXT_EXPORT void glGetHistogramParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetHistogramParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetHistogramParameterfv");
}

FABRIC_EXT_EXPORT void glGetHistogramParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glGetMinmax( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)types, values );
  _checkError("glGetMinmax");
}

FABRIC_EXT_EXPORT void glGetMinmaxParameterfv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetMinmaxParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetMinmaxParameterfv");
}

FABRIC_EXT_EXPORT void glGetMinmaxParameteriv_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glHistogram( (GLenum)target, (GLsizei)width, (GLenum)internalformat, (GLboolean)sink );
  _checkError("glHistogram");
}

FABRIC_EXT_EXPORT void glMinmax_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Boolean sink
){
  _clearError();
  glMinmax( (GLenum)target, (GLenum)internalformat, (GLboolean)sink );
  _checkError("glMinmax");
}

FABRIC_EXT_EXPORT void glResetHistogram_wrapper(
  KL::Size target
){
  _clearError();
  glResetHistogram( (GLenum)target );
  _checkError("glResetHistogram");
}

FABRIC_EXT_EXPORT void glResetMinmax_wrapper(
  KL::Size target
){
  _clearError();
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
  glSeparableFilter2D( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, row, column );
  _checkError("glSeparableFilter2D");
}

FABRIC_EXT_EXPORT void glFlushMappedBufferRange_wrapper(
  KL::Size target,
  KL::Integer offset,
  KL::Size length
){
  _clearError();
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
  glMapBufferRange( (GLenum)target, (GLintptr)offset, (GLsizeiptr)length, (GLbitfield)access );
  _checkError("glMapBufferRange");
}

FABRIC_EXT_EXPORT void glProvokingVertex_wrapper(
  KL::Size mode
){
  _clearError();
  glProvokingVertex( (GLenum)mode );
  _checkError("glProvokingVertex");
}

FABRIC_EXT_EXPORT void glGetMultisamplefv_wrapper(
  KL::Size pname,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & val
){
  _clearError();
  glGetMultisamplefv( (GLenum)pname, (GLuint)index, (GLfloat*)&val[0] );
  _checkError("glGetMultisamplefv");
}

FABRIC_EXT_EXPORT void glSampleMaski_wrapper(
  KL::Size index,
  KL::Size mask
){
  _clearError();
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
  glTexImage3DMultisample( (GLenum)target, (GLsizei)samples, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLboolean)fixedsamplelocations );
  _checkError("glTexImage3DMultisample");
}

FABRIC_EXT_EXPORT void glBindBufferBase_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Size buffer
){
  _clearError();
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
  glGetActiveUniformsiv( (GLuint)program, (GLsizei)uniformCount, (const GLuint*)&uniformIndices[0], (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetActiveUniformsiv");
}

FABRIC_EXT_EXPORT void glGetIntegeri_v_wrapper(
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Integer> & data
){
  _clearError();
  glGetIntegeri_v( (GLenum)target, (GLuint)index, (GLint*)&data[0] );
  _checkError("glGetIntegeri_v");
}

FABRIC_EXT_EXPORT KL::Size glGetUniformBlockIndex_wrapper(
  KL::Size program,
  const KL::String &uniformBlockName
){
  _clearError();
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
  glGetUniformIndices( (GLuint)program, (GLsizei)uniformCount, (const char**)&uniformNames[0], (GLuint*)&uniformIndices[0] );
  _checkError("glGetUniformIndices");
}

FABRIC_EXT_EXPORT void glUniformBlockBinding_wrapper(
  KL::Size program,
  KL::Size uniformBlockIndex,
  KL::Size uniformBlockBinding
){
  _clearError();
  glUniformBlockBinding( (GLuint)program, (GLuint)uniformBlockIndex, (GLuint)uniformBlockBinding );
  _checkError("glUniformBlockBinding");
}

FABRIC_EXT_EXPORT void glBindVertexArray_wrapper(
  KL::Size array
){
  _clearError();
  glBindVertexArray( (GLuint)array );
  _checkError("glBindVertexArray");
}

FABRIC_EXT_EXPORT void glDeleteVertexArrays_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & arrays
){
  _clearError();
  glDeleteVertexArrays( (GLsizei)n, (const GLuint*)&arrays[0] );
  _checkError("glDeleteVertexArrays");
}

FABRIC_EXT_EXPORT void glGenVertexArrays_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & arrays
){
  _clearError();
  glGenVertexArrays( (GLsizei)n, (GLuint*)&arrays[0] );
  _checkError("glGenVertexArrays");
}

FABRIC_EXT_EXPORT KL::Boolean glIsVertexArray_wrapper(
  KL::Size array
){
  _clearError();
  GLboolean result = glIsVertexArray( (GLuint)array );
  _checkError("glIsVertexArray");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Integer glGetUniformBufferSizeEXT_wrapper(
  KL::Size program,
  KL::Integer location
){
  _clearError();
  GLint result = glGetUniformBufferSizeEXT( (GLuint)program, (GLint)location );
  _checkError("glGetUniformBufferSizeEXT");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer glGetUniformOffsetEXT_wrapper(
  KL::Size program,
  KL::Integer location
){
  _clearError();
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
  glBlendColorEXT( (GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha );
  _checkError("glBlendColorEXT");
}

FABRIC_EXT_EXPORT void glBlendEquationSeparateEXT_wrapper(
  KL::Size modeRGB,
  KL::Size modeAlpha
){
  _clearError();
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
  glBlendFuncSeparateEXT( (GLenum)sfactorRGB, (GLenum)dfactorRGB, (GLenum)sfactorAlpha, (GLenum)dfactorAlpha );
  _checkError("glBlendFuncSeparateEXT");
}

FABRIC_EXT_EXPORT void glBlendEquationEXT_wrapper(
  KL::Size mode
){
  _clearError();
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
  glCopyColorSubTableEXT( (GLenum)target, (GLsizei)start, (GLint)x, (GLint)y, (GLsizei)width );
  _checkError("glCopyColorSubTableEXT");
}

FABRIC_EXT_EXPORT void glLockArraysEXT_wrapper(
  KL::Integer first,
  KL::Size count
){
  _clearError();
  glLockArraysEXT( (GLint)first, (GLsizei)count );
  _checkError("glLockArraysEXT");
}

FABRIC_EXT_EXPORT void glUnlockArraysEXT_wrapper()
{
  _clearError();
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
  glConvolutionFilter2DEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, image );
  _checkError("glConvolutionFilter2DEXT");
}

FABRIC_EXT_EXPORT void glConvolutionParameterfEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glConvolutionParameterfEXT( (GLenum)target, (GLenum)pname, (GLfloat)param );
  _checkError("glConvolutionParameterfEXT");
}

FABRIC_EXT_EXPORT void glConvolutionParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glConvolutionParameterfvEXT( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glConvolutionParameterfvEXT");
}

FABRIC_EXT_EXPORT void glConvolutionParameteriEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glConvolutionParameteriEXT( (GLenum)target, (GLenum)pname, (GLint)param );
  _checkError("glConvolutionParameteriEXT");
}

FABRIC_EXT_EXPORT void glConvolutionParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glGetConvolutionFilterEXT( (GLenum)target, (GLenum)format, (GLenum)type, image );
  _checkError("glGetConvolutionFilterEXT");
}

FABRIC_EXT_EXPORT void glGetConvolutionParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetConvolutionParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetConvolutionParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetConvolutionParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glSeparableFilter2DEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, row, column );
  _checkError("glSeparableFilter2DEXT");
}

FABRIC_EXT_EXPORT void glBinormalPointerEXT_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
  glBinormalPointerEXT( (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glBinormalPointerEXT");
}

FABRIC_EXT_EXPORT void glTangentPointerEXT_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
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
  glCopyTexSubImage3DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyTexSubImage3DEXT");
}

FABRIC_EXT_EXPORT void glCullParameterdvEXT_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glCullParameterdvEXT( (GLenum)pname, (GLdouble*)&params[0] );
  _checkError("glCullParameterdvEXT");
}

FABRIC_EXT_EXPORT void glCullParameterfvEXT_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glCullParameterfvEXT( (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glCullParameterfvEXT");
}

FABRIC_EXT_EXPORT void glDepthBoundsEXT_wrapper(
  KL::Scalar zmin,
  KL::Scalar zmax
){
  _clearError();
  glDepthBoundsEXT( (GLclampd)zmin, (GLclampd)zmax );
  _checkError("glDepthBoundsEXT");
}

FABRIC_EXT_EXPORT void glBindMultiTextureEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size texture
){
  _clearError();
  glBindMultiTextureEXT( (GLenum)texunit, (GLenum)target, (GLuint)texture );
  _checkError("glBindMultiTextureEXT");
}

FABRIC_EXT_EXPORT KL::Size glCheckNamedFramebufferStatusEXT_wrapper(
  KL::Size framebuffer,
  KL::Size target
){
  _clearError();
  GLenum result = glCheckNamedFramebufferStatusEXT( (GLuint)framebuffer, (GLenum)target );
  _checkError("glCheckNamedFramebufferStatusEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glClientAttribDefaultEXT_wrapper(
  KL::Size mask
){
  _clearError();
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
  glCopyTextureSubImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glCopyTextureSubImage3DEXT");
}

FABRIC_EXT_EXPORT void glDisableClientStateIndexedEXT_wrapper(
  KL::Size array,
  KL::Size index
){
  _clearError();
  glDisableClientStateIndexedEXT( (GLenum)array, (GLuint)index );
  _checkError("glDisableClientStateIndexedEXT");
}

FABRIC_EXT_EXPORT void glDisableClientStateiEXT_wrapper(
  KL::Size array,
  KL::Size index
){
  _clearError();
  glDisableClientStateiEXT( (GLenum)array, (GLuint)index );
  _checkError("glDisableClientStateiEXT");
}

FABRIC_EXT_EXPORT void glDisableVertexArrayAttribEXT_wrapper(
  KL::Size vaobj,
  KL::Size index
){
  _clearError();
  glDisableVertexArrayAttribEXT( (GLuint)vaobj, (GLuint)index );
  _checkError("glDisableVertexArrayAttribEXT");
}

FABRIC_EXT_EXPORT void glDisableVertexArrayEXT_wrapper(
  KL::Size vaobj,
  KL::Size array
){
  _clearError();
  glDisableVertexArrayEXT( (GLuint)vaobj, (GLenum)array );
  _checkError("glDisableVertexArrayEXT");
}

FABRIC_EXT_EXPORT void glEnableClientStateIndexedEXT_wrapper(
  KL::Size array,
  KL::Size index
){
  _clearError();
  glEnableClientStateIndexedEXT( (GLenum)array, (GLuint)index );
  _checkError("glEnableClientStateIndexedEXT");
}

FABRIC_EXT_EXPORT void glEnableClientStateiEXT_wrapper(
  KL::Size array,
  KL::Size index
){
  _clearError();
  glEnableClientStateiEXT( (GLenum)array, (GLuint)index );
  _checkError("glEnableClientStateiEXT");
}

FABRIC_EXT_EXPORT void glEnableVertexArrayAttribEXT_wrapper(
  KL::Size vaobj,
  KL::Size index
){
  _clearError();
  glEnableVertexArrayAttribEXT( (GLuint)vaobj, (GLuint)index );
  _checkError("glEnableVertexArrayAttribEXT");
}

FABRIC_EXT_EXPORT void glEnableVertexArrayEXT_wrapper(
  KL::Size vaobj,
  KL::Size array
){
  _clearError();
  glEnableVertexArrayEXT( (GLuint)vaobj, (GLenum)array );
  _checkError("glEnableVertexArrayEXT");
}

FABRIC_EXT_EXPORT void glFlushMappedNamedBufferRangeEXT_wrapper(
  KL::Size buffer,
  KL::Integer offset,
  KL::Size length
){
  _clearError();
  glFlushMappedNamedBufferRangeEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)length );
  _checkError("glFlushMappedNamedBufferRangeEXT");
}

FABRIC_EXT_EXPORT void glFramebufferDrawBufferEXT_wrapper(
  KL::Size framebuffer,
  KL::Size mode
){
  _clearError();
  glFramebufferDrawBufferEXT( (GLuint)framebuffer, (GLenum)mode );
  _checkError("glFramebufferDrawBufferEXT");
}

FABRIC_EXT_EXPORT void glFramebufferDrawBuffersEXT_wrapper(
  KL::Size framebuffer,
  KL::Size n,
  const KL::VariableArray<KL::Size> & bufs
){
  _clearError();
  glFramebufferDrawBuffersEXT( (GLuint)framebuffer, (GLsizei)n, (const GLenum*)&bufs[0] );
  _checkError("glFramebufferDrawBuffersEXT");
}

FABRIC_EXT_EXPORT void glFramebufferReadBufferEXT_wrapper(
  KL::Size framebuffer,
  KL::Size mode
){
  _clearError();
  glFramebufferReadBufferEXT( (GLuint)framebuffer, (GLenum)mode );
  _checkError("glFramebufferReadBufferEXT");
}

FABRIC_EXT_EXPORT void glGenerateMultiTexMipmapEXT_wrapper(
  KL::Size texunit,
  KL::Size target
){
  _clearError();
  glGenerateMultiTexMipmapEXT( (GLenum)texunit, (GLenum)target );
  _checkError("glGenerateMultiTexMipmapEXT");
}

FABRIC_EXT_EXPORT void glGenerateTextureMipmapEXT_wrapper(
  KL::Size texture,
  KL::Size target
){
  _clearError();
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
  glGetCompressedTextureImageEXT( (GLuint)texture, (GLenum)target, (GLint)level, img );
  _checkError("glGetCompressedTextureImageEXT");
}

FABRIC_EXT_EXPORT void glGetDoubleIndexedvEXT_wrapper(
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetDoubleIndexedvEXT( (GLenum)target, (GLuint)index, (GLdouble*)&params[0] );
  _checkError("glGetDoubleIndexedvEXT");
}

FABRIC_EXT_EXPORT void glGetDoublei_vEXT_wrapper(
  KL::Size pname,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetDoublei_vEXT( (GLenum)pname, (GLuint)index, (GLdouble*)&params[0] );
  _checkError("glGetDoublei_vEXT");
}

FABRIC_EXT_EXPORT void glGetFloatIndexedvEXT_wrapper(
  KL::Size target,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetFloatIndexedvEXT( (GLenum)target, (GLuint)index, (GLfloat*)&params[0] );
  _checkError("glGetFloatIndexedvEXT");
}

FABRIC_EXT_EXPORT void glGetFloati_vEXT_wrapper(
  KL::Size pname,
  KL::Size index,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetFloati_vEXT( (GLenum)pname, (GLuint)index, (GLfloat*)&params[0] );
  _checkError("glGetFloati_vEXT");
}

FABRIC_EXT_EXPORT void glGetFramebufferParameterivEXT_wrapper(
  KL::Size framebuffer,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & param
){
  _clearError();
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
  glGetMultiTexParameterivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetMultiTexParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetNamedBufferParameterivEXT_wrapper(
  KL::Size buffer,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glGetNamedProgramivEXT( (GLuint)program, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetNamedProgramivEXT");
}

FABRIC_EXT_EXPORT void glGetNamedRenderbufferParameterivEXT_wrapper(
  KL::Size renderbuffer,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glGetVertexArrayIntegeri_vEXT( (GLuint)vaobj, (GLuint)index, (GLenum)pname, (GLint*)&param[0] );
  _checkError("glGetVertexArrayIntegeri_vEXT");
}

FABRIC_EXT_EXPORT void glGetVertexArrayIntegervEXT_wrapper(
  KL::Size vaobj,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & param
){
  _clearError();
  glGetVertexArrayIntegervEXT( (GLuint)vaobj, (GLenum)pname, (GLint*)&param[0] );
  _checkError("glGetVertexArrayIntegervEXT");
}

FABRIC_EXT_EXPORT GLvoid glMapNamedBufferEXT_wrapper(
  KL::Size buffer,
  KL::Size access
){
  _clearError();
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
  glMatrixFrustumEXT( (GLenum)matrixMode, (GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t, (GLdouble)n, (GLdouble)f );
  _checkError("glMatrixFrustumEXT");
}

FABRIC_EXT_EXPORT void glMatrixLoadIdentityEXT_wrapper(
  KL::Size matrixMode
){
  _clearError();
  glMatrixLoadIdentityEXT( (GLenum)matrixMode );
  _checkError("glMatrixLoadIdentityEXT");
}

FABRIC_EXT_EXPORT void glMatrixLoadTransposedEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
  glMatrixLoadTransposedEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
  _checkError("glMatrixLoadTransposedEXT");
}

FABRIC_EXT_EXPORT void glMatrixLoadTransposefEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
  glMatrixLoadTransposefEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
  _checkError("glMatrixLoadTransposefEXT");
}

FABRIC_EXT_EXPORT void glMatrixLoaddEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
  glMatrixLoaddEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
  _checkError("glMatrixLoaddEXT");
}

FABRIC_EXT_EXPORT void glMatrixLoadfEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
  glMatrixLoadfEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
  _checkError("glMatrixLoadfEXT");
}

FABRIC_EXT_EXPORT void glMatrixMultTransposedEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
  glMatrixMultTransposedEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
  _checkError("glMatrixMultTransposedEXT");
}

FABRIC_EXT_EXPORT void glMatrixMultTransposefEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
  glMatrixMultTransposefEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
  _checkError("glMatrixMultTransposefEXT");
}

FABRIC_EXT_EXPORT void glMatrixMultdEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
  glMatrixMultdEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
  _checkError("glMatrixMultdEXT");
}

FABRIC_EXT_EXPORT void glMatrixMultfEXT_wrapper(
  KL::Size matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  _clearError();
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
  glMatrixOrthoEXT( (GLenum)matrixMode, (GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t, (GLdouble)n, (GLdouble)f );
  _checkError("glMatrixOrthoEXT");
}

FABRIC_EXT_EXPORT void glMatrixPopEXT_wrapper(
  KL::Size matrixMode
){
  _clearError();
  glMatrixPopEXT( (GLenum)matrixMode );
  _checkError("glMatrixPopEXT");
}

FABRIC_EXT_EXPORT void glMatrixPushEXT_wrapper(
  KL::Size matrixMode
){
  _clearError();
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
  glMultiTexParameterivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLint*)&param[0] );
  _checkError("glMultiTexParameterivEXT");
}

FABRIC_EXT_EXPORT void glMultiTexRenderbufferEXT_wrapper(
  KL::Size texunit,
  KL::Size target,
  KL::Size renderbuffer
){
  _clearError();
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
  glNamedRenderbufferStorageMultisampleEXT( (GLuint)renderbuffer, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  _checkError("glNamedRenderbufferStorageMultisampleEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform1fEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Scalar v0
){
  _clearError();
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
  glProgramUniform1fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  _checkError("glProgramUniform1fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform1iEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Integer v0
){
  _clearError();
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
  glProgramUniform1ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  _checkError("glProgramUniform1ivEXT");
}

FABRIC_EXT_EXPORT void glProgramUniform1uiEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size v0
){
  _clearError();
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
  glProgramUniform4uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  _checkError("glProgramUniform4uivEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix2fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glProgramUniformMatrix2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix2fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix2x3fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glProgramUniformMatrix2x3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix2x3fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix2x4fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glProgramUniformMatrix2x4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix2x4fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix3fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glProgramUniformMatrix3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix3fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix3x2fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glProgramUniformMatrix3x2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix3x2fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix3x4fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glProgramUniformMatrix3x4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix3x4fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix4fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glProgramUniformMatrix4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix4fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix4x2fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glProgramUniformMatrix4x2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix4x2fvEXT");
}

FABRIC_EXT_EXPORT void glProgramUniformMatrix4x3fvEXT_wrapper(
  KL::Size program,
  KL::Integer location,
  KL::Size count,
  KL::Boolean transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  _clearError();
  glProgramUniformMatrix4x3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  _checkError("glProgramUniformMatrix4x3fvEXT");
}

FABRIC_EXT_EXPORT void glPushClientAttribDefaultEXT_wrapper(
  KL::Size mask
){
  _clearError();
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
  glTextureParameterivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLint*)&param[0] );
  _checkError("glTextureParameterivEXT");
}

FABRIC_EXT_EXPORT void glTextureRenderbufferEXT_wrapper(
  KL::Size texture,
  KL::Size target,
  KL::Size renderbuffer
){
  _clearError();
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
  glTextureSubImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTextureSubImage3DEXT");
}

FABRIC_EXT_EXPORT KL::Boolean glUnmapNamedBufferEXT_wrapper(
  KL::Size buffer
){
  _clearError();
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
  glColorMaskIndexedEXT( (GLuint)buf, (GLboolean)r, (GLboolean)g, (GLboolean)b, (GLboolean)a );
  _checkError("glColorMaskIndexedEXT");
}

FABRIC_EXT_EXPORT void glDisableIndexedEXT_wrapper(
  KL::Size target,
  KL::Size index
){
  _clearError();
  glDisableIndexedEXT( (GLenum)target, (GLuint)index );
  _checkError("glDisableIndexedEXT");
}

FABRIC_EXT_EXPORT void glEnableIndexedEXT_wrapper(
  KL::Size target,
  KL::Size index
){
  _clearError();
  glEnableIndexedEXT( (GLenum)target, (GLuint)index );
  _checkError("glEnableIndexedEXT");
}

FABRIC_EXT_EXPORT void glGetBooleanIndexedvEXT_wrapper(
  KL::Size value,
  KL::Size index,
  KL::VariableArray<KL::Boolean> & data
){
  _clearError();
  glGetBooleanIndexedvEXT( (GLenum)value, (GLuint)index, (GLboolean*)&data[0] );
  _checkError("glGetBooleanIndexedvEXT");
}

FABRIC_EXT_EXPORT void glGetIntegerIndexedvEXT_wrapper(
  KL::Size value,
  KL::Size index,
  KL::VariableArray<KL::Integer> & data
){
  _clearError();
  glGetIntegerIndexedvEXT( (GLenum)value, (GLuint)index, (GLint*)&data[0] );
  _checkError("glGetIntegerIndexedvEXT");
}

FABRIC_EXT_EXPORT KL::Boolean glIsEnabledIndexedEXT_wrapper(
  KL::Size target,
  KL::Size index
){
  _clearError();
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
  glDrawRangeElementsEXT( (GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, indices );
  _checkError("glDrawRangeElementsEXT");
}

FABRIC_EXT_EXPORT void glFogCoordPointerEXT_wrapper(
  KL::Size type,
  KL::Size stride,
  KL::Data pointer
){
  _clearError();
  glFogCoordPointerEXT( (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glFogCoordPointerEXT");
}

FABRIC_EXT_EXPORT void glFogCoorddEXT_wrapper(
  KL::Scalar coord
){
  _clearError();
  glFogCoorddEXT( (GLdouble)coord );
  _checkError("glFogCoorddEXT");
}

FABRIC_EXT_EXPORT void glFogCoorddvEXT_wrapper(
  const KL::VariableArray<KL::Scalar> & coord
){
  _clearError();
  glFogCoorddvEXT( (const GLdouble*)&coord[0] );
  _checkError("glFogCoorddvEXT");
}

FABRIC_EXT_EXPORT void glFogCoordfEXT_wrapper(
  KL::Scalar coord
){
  _clearError();
  glFogCoordfEXT( (GLfloat)coord );
  _checkError("glFogCoordfEXT");
}

FABRIC_EXT_EXPORT void glFogCoordfvEXT_wrapper(
  const KL::VariableArray<KL::Scalar> & coord
){
  _clearError();
  glFogCoordfvEXT( (const GLfloat*)&coord[0] );
  _checkError("glFogCoordfvEXT");
}

FABRIC_EXT_EXPORT void glFragmentColorMaterialEXT_wrapper(
  KL::Size face,
  KL::Size mode
){
  _clearError();
  glFragmentColorMaterialEXT( (GLenum)face, (GLenum)mode );
  _checkError("glFragmentColorMaterialEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightModelfEXT_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glFragmentLightModelfEXT( (GLenum)pname, (GLfloat)param );
  _checkError("glFragmentLightModelfEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightModelfvEXT_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glFragmentLightModelfvEXT( (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glFragmentLightModelfvEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightModeliEXT_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glFragmentLightModeliEXT( (GLenum)pname, (GLint)param );
  _checkError("glFragmentLightModeliEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightModelivEXT_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glFragmentLightModelivEXT( (GLenum)pname, (GLint*)&params[0] );
  _checkError("glFragmentLightModelivEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightfEXT_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glFragmentLightfEXT( (GLenum)light, (GLenum)pname, (GLfloat)param );
  _checkError("glFragmentLightfEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightfvEXT_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glFragmentLightfvEXT( (GLenum)light, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glFragmentLightfvEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightiEXT_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glFragmentLightiEXT( (GLenum)light, (GLenum)pname, (GLint)param );
  _checkError("glFragmentLightiEXT");
}

FABRIC_EXT_EXPORT void glFragmentLightivEXT_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glFragmentLightivEXT( (GLenum)light, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glFragmentLightivEXT");
}

FABRIC_EXT_EXPORT void glFragmentMaterialfEXT_wrapper(
  KL::Size face,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glFragmentMaterialfEXT( (GLenum)face, (GLenum)pname, (const GLfloat)param );
  _checkError("glFragmentMaterialfEXT");
}

FABRIC_EXT_EXPORT void glFragmentMaterialfvEXT_wrapper(
  KL::Size face,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glFragmentMaterialfvEXT( (GLenum)face, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glFragmentMaterialfvEXT");
}

FABRIC_EXT_EXPORT void glFragmentMaterialiEXT_wrapper(
  KL::Size face,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glFragmentMaterialiEXT( (GLenum)face, (GLenum)pname, (const GLint)param );
  _checkError("glFragmentMaterialiEXT");
}

FABRIC_EXT_EXPORT void glFragmentMaterialivEXT_wrapper(
  KL::Size face,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glFragmentMaterialivEXT( (GLenum)face, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glFragmentMaterialivEXT");
}

FABRIC_EXT_EXPORT void glGetFragmentLightfvEXT_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetFragmentLightfvEXT( (GLenum)light, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetFragmentLightfvEXT");
}

FABRIC_EXT_EXPORT void glGetFragmentLightivEXT_wrapper(
  KL::Size light,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetFragmentLightivEXT( (GLenum)light, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetFragmentLightivEXT");
}

FABRIC_EXT_EXPORT void glGetFragmentMaterialfvEXT_wrapper(
  KL::Size face,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetFragmentMaterialfvEXT( (GLenum)face, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glGetFragmentMaterialfvEXT");
}

FABRIC_EXT_EXPORT void glGetFragmentMaterialivEXT_wrapper(
  KL::Size face,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetFragmentMaterialivEXT( (GLenum)face, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glGetFragmentMaterialivEXT");
}

FABRIC_EXT_EXPORT void glLightEnviEXT_wrapper(
  KL::Size pname,
  KL::Integer param
){
  _clearError();
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
  glRenderbufferStorageMultisampleEXT( (GLenum)target, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  _checkError("glRenderbufferStorageMultisampleEXT");
}

FABRIC_EXT_EXPORT void glBindFramebufferEXT_wrapper(
  KL::Size target,
  KL::Size framebuffer
){
  _clearError();
  glBindFramebufferEXT( (GLenum)target, (GLuint)framebuffer );
  _checkError("glBindFramebufferEXT");
}

FABRIC_EXT_EXPORT void glBindRenderbufferEXT_wrapper(
  KL::Size target,
  KL::Size renderbuffer
){
  _clearError();
  glBindRenderbufferEXT( (GLenum)target, (GLuint)renderbuffer );
  _checkError("glBindRenderbufferEXT");
}

FABRIC_EXT_EXPORT KL::Size glCheckFramebufferStatusEXT_wrapper(
  KL::Size target
){
  _clearError();
  GLenum result = glCheckFramebufferStatusEXT( (GLenum)target );
  _checkError("glCheckFramebufferStatusEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glDeleteFramebuffersEXT_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & framebuffers
){
  _clearError();
  glDeleteFramebuffersEXT( (GLsizei)n, (const GLuint*)&framebuffers[0] );
  _checkError("glDeleteFramebuffersEXT");
}

FABRIC_EXT_EXPORT void glDeleteRenderbuffersEXT_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & renderbuffers
){
  _clearError();
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
  glFramebufferTexture3DEXT( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level, (GLint)zoffset );
  _checkError("glFramebufferTexture3DEXT");
}

FABRIC_EXT_EXPORT void glGenFramebuffersEXT_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & framebuffers
){
  _clearError();
  glGenFramebuffersEXT( (GLsizei)n, (GLuint*)&framebuffers[0] );
  _checkError("glGenFramebuffersEXT");
}

FABRIC_EXT_EXPORT void glGenRenderbuffersEXT_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & renderbuffers
){
  _clearError();
  glGenRenderbuffersEXT( (GLsizei)n, (GLuint*)&renderbuffers[0] );
  _checkError("glGenRenderbuffersEXT");
}

FABRIC_EXT_EXPORT void glGenerateMipmapEXT_wrapper(
  KL::Size target
){
  _clearError();
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
  glGetFramebufferAttachmentParameterivEXT( (GLenum)target, (GLenum)attachment, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetFramebufferAttachmentParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetRenderbufferParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetRenderbufferParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetRenderbufferParameterivEXT");
}

FABRIC_EXT_EXPORT KL::Boolean glIsFramebufferEXT_wrapper(
  KL::Size framebuffer
){
  _clearError();
  GLboolean result = glIsFramebufferEXT( (GLuint)framebuffer );
  _checkError("glIsFramebufferEXT");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glIsRenderbufferEXT_wrapper(
  KL::Size renderbuffer
){
  _clearError();
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
  glFramebufferTextureLayerEXT( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer );
  _checkError("glFramebufferTextureLayerEXT");
}

FABRIC_EXT_EXPORT void glProgramParameteriEXT_wrapper(
  KL::Size program,
  KL::Size pname,
  KL::Integer value
){
  _clearError();
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
  glProgramLocalParameters4fvEXT( (GLenum)target, (GLuint)index, (GLsizei)count, (const GLfloat*)&params[0] );
  _checkError("glProgramLocalParameters4fvEXT");
}

FABRIC_EXT_EXPORT void glBindFragDataLocationEXT_wrapper(
  KL::Size program,
  KL::Size color,
  const KL::String &name
){
  _clearError();
  glBindFragDataLocationEXT( (GLuint)program, (GLuint)color, (const GLchar*)name.data() );
  _checkError("glBindFragDataLocationEXT");
}

FABRIC_EXT_EXPORT KL::Integer glGetFragDataLocationEXT_wrapper(
  KL::Size program,
  const KL::String &name
){
  _clearError();
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
  glGetUniformuivEXT( (GLuint)program, (GLint)location, (GLuint*)&params[0] );
  _checkError("glGetUniformuivEXT");
}

FABRIC_EXT_EXPORT void glGetVertexAttribIivEXT_wrapper(
  KL::Size index,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetVertexAttribIivEXT( (GLuint)index, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetVertexAttribIivEXT");
}

FABRIC_EXT_EXPORT void glGetVertexAttribIuivEXT_wrapper(
  KL::Size index,
  KL::Size pname,
  KL::VariableArray<KL::Size> & params
){
  _clearError();
  glGetVertexAttribIuivEXT( (GLuint)index, (GLenum)pname, (GLuint*)&params[0] );
  _checkError("glGetVertexAttribIuivEXT");
}

FABRIC_EXT_EXPORT void glUniform1uiEXT_wrapper(
  KL::Integer location,
  KL::Size v0
){
  _clearError();
  glUniform1uiEXT( (GLint)location, (GLuint)v0 );
  _checkError("glUniform1uiEXT");
}

FABRIC_EXT_EXPORT void glUniform1uivEXT_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Size> & value
){
  _clearError();
  glUniform1uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  _checkError("glUniform1uivEXT");
}

FABRIC_EXT_EXPORT void glUniform2uiEXT_wrapper(
  KL::Integer location,
  KL::Size v0,
  KL::Size v1
){
  _clearError();
  glUniform2uiEXT( (GLint)location, (GLuint)v0, (GLuint)v1 );
  _checkError("glUniform2uiEXT");
}

FABRIC_EXT_EXPORT void glUniform2uivEXT_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Size> & value
){
  _clearError();
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
  glUniform3uiEXT( (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2 );
  _checkError("glUniform3uiEXT");
}

FABRIC_EXT_EXPORT void glUniform3uivEXT_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Size> & value
){
  _clearError();
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
  glUniform4uiEXT( (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2, (GLuint)v3 );
  _checkError("glUniform4uiEXT");
}

FABRIC_EXT_EXPORT void glUniform4uivEXT_wrapper(
  KL::Integer location,
  KL::Size count,
  const KL::VariableArray<KL::Size> & value
){
  _clearError();
  glUniform4uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  _checkError("glUniform4uivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI1iEXT_wrapper(
  KL::Size index,
  KL::Integer x
){
  _clearError();
  glVertexAttribI1iEXT( (GLuint)index, (GLint)x );
  _checkError("glVertexAttribI1iEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI1ivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glVertexAttribI1ivEXT( (GLuint)index, (const GLint*)&v[0] );
  _checkError("glVertexAttribI1ivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI1uiEXT_wrapper(
  KL::Size index,
  KL::Size x
){
  _clearError();
  glVertexAttribI1uiEXT( (GLuint)index, (GLuint)x );
  _checkError("glVertexAttribI1uiEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI1uivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glVertexAttribI1uivEXT( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttribI1uivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI2iEXT_wrapper(
  KL::Size index,
  KL::Integer x,
  KL::Integer y
){
  _clearError();
  glVertexAttribI2iEXT( (GLuint)index, (GLint)x, (GLint)y );
  _checkError("glVertexAttribI2iEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI2ivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glVertexAttribI2ivEXT( (GLuint)index, (const GLint*)&v[0] );
  _checkError("glVertexAttribI2ivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI2uiEXT_wrapper(
  KL::Size index,
  KL::Size x,
  KL::Size y
){
  _clearError();
  glVertexAttribI2uiEXT( (GLuint)index, (GLuint)x, (GLuint)y );
  _checkError("glVertexAttribI2uiEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI2uivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
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
  glVertexAttribI3iEXT( (GLuint)index, (GLint)x, (GLint)y, (GLint)z );
  _checkError("glVertexAttribI3iEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI3ivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glVertexAttribI3uiEXT( (GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z );
  _checkError("glVertexAttribI3uiEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI3uivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glVertexAttribI3uivEXT( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttribI3uivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4bvEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
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
  glVertexAttribI4iEXT( (GLuint)index, (GLint)x, (GLint)y, (GLint)z, (GLint)w );
  _checkError("glVertexAttribI4iEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4ivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glVertexAttribI4ivEXT( (GLuint)index, (const GLint*)&v[0] );
  _checkError("glVertexAttribI4ivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4svEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glVertexAttribI4svEXT( (GLuint)index, (const GLshort*)&v[0] );
  _checkError("glVertexAttribI4svEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4ubvEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
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
  glVertexAttribI4uiEXT( (GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z, (GLuint)w );
  _checkError("glVertexAttribI4uiEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4uivEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glVertexAttribI4uivEXT( (GLuint)index, (const GLuint*)&v[0] );
  _checkError("glVertexAttribI4uivEXT");
}

FABRIC_EXT_EXPORT void glVertexAttribI4usvEXT_wrapper(
  KL::Size index,
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glGetHistogramEXT( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, values );
  _checkError("glGetHistogramEXT");
}

FABRIC_EXT_EXPORT void glGetHistogramParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetHistogramParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetHistogramParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetHistogramParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glGetMinmaxEXT( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, values );
  _checkError("glGetMinmaxEXT");
}

FABRIC_EXT_EXPORT void glGetMinmaxParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetMinmaxParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetMinmaxParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetMinmaxParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
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
  glHistogramEXT( (GLenum)target, (GLsizei)width, (GLenum)internalformat, (GLboolean)sink );
  _checkError("glHistogramEXT");
}

FABRIC_EXT_EXPORT void glMinmaxEXT_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Boolean sink
){
  _clearError();
  glMinmaxEXT( (GLenum)target, (GLenum)internalformat, (GLboolean)sink );
  _checkError("glMinmaxEXT");
}

FABRIC_EXT_EXPORT void glResetHistogramEXT_wrapper(
  KL::Size target
){
  _clearError();
  glResetHistogramEXT( (GLenum)target );
  _checkError("glResetHistogramEXT");
}

FABRIC_EXT_EXPORT void glResetMinmaxEXT_wrapper(
  KL::Size target
){
  _clearError();
  glResetMinmaxEXT( (GLenum)target );
  _checkError("glResetMinmaxEXT");
}

FABRIC_EXT_EXPORT void glIndexFuncEXT_wrapper(
  KL::Size func,
  KL::Scalar ref
){
  _clearError();
  glIndexFuncEXT( (GLenum)func, (GLfloat)ref );
  _checkError("glIndexFuncEXT");
}

FABRIC_EXT_EXPORT void glIndexMaterialEXT_wrapper(
  KL::Size face,
  KL::Size mode
){
  _clearError();
  glIndexMaterialEXT( (GLenum)face, (GLenum)mode );
  _checkError("glIndexMaterialEXT");
}

FABRIC_EXT_EXPORT void glApplyTextureEXT_wrapper(
  KL::Size mode
){
  _clearError();
  glApplyTextureEXT( (GLenum)mode );
  _checkError("glApplyTextureEXT");
}

FABRIC_EXT_EXPORT void glTextureLightEXT_wrapper(
  KL::Size pname
){
  _clearError();
  glTextureLightEXT( (GLenum)pname );
  _checkError("glTextureLightEXT");
}

FABRIC_EXT_EXPORT void glTextureMaterialEXT_wrapper(
  KL::Size face,
  KL::Size mode
){
  _clearError();
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
  glMultiDrawArraysEXT( (GLenum)mode, (GLint*)&first[0], (GLsizei*)&count[0], (GLsizei)primcount );
  _checkError("glMultiDrawArraysEXT");
}

FABRIC_EXT_EXPORT void glSampleMaskEXT_wrapper(
  KL::Scalar value,
  KL::Boolean invert
){
  _clearError();
  glSampleMaskEXT( (GLclampf)value, (GLboolean)invert );
  _checkError("glSampleMaskEXT");
}

FABRIC_EXT_EXPORT void glSamplePatternEXT_wrapper(
  KL::Size pattern
){
  _clearError();
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
  glGetColorTableEXT( (GLenum)target, (GLenum)format, (GLenum)type, data );
  _checkError("glGetColorTableEXT");
}

FABRIC_EXT_EXPORT void glGetColorTableParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetColorTableParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glGetColorTableParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetColorTableParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetColorTableParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetColorTableParameterivEXT");
}

FABRIC_EXT_EXPORT void glGetPixelTransformParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glGetPixelTransformParameterfvEXT( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glGetPixelTransformParameterfvEXT");
}

FABRIC_EXT_EXPORT void glGetPixelTransformParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetPixelTransformParameterivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glGetPixelTransformParameterivEXT");
}

FABRIC_EXT_EXPORT void glPixelTransformParameterfEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glPixelTransformParameterfEXT( (GLenum)target, (GLenum)pname, (const GLfloat)param );
  _checkError("glPixelTransformParameterfEXT");
}

FABRIC_EXT_EXPORT void glPixelTransformParameterfvEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glPixelTransformParameterfvEXT( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  _checkError("glPixelTransformParameterfvEXT");
}

FABRIC_EXT_EXPORT void glPixelTransformParameteriEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::Integer param
){
  _clearError();
  glPixelTransformParameteriEXT( (GLenum)target, (GLenum)pname, (const GLint)param );
  _checkError("glPixelTransformParameteriEXT");
}

FABRIC_EXT_EXPORT void glPixelTransformParameterivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glPixelTransformParameterivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glPixelTransformParameterivEXT");
}

FABRIC_EXT_EXPORT void glPointParameterfEXT_wrapper(
  KL::Size pname,
  KL::Scalar param
){
  _clearError();
  glPointParameterfEXT( (GLenum)pname, (GLfloat)param );
  _checkError("glPointParameterfEXT");
}

FABRIC_EXT_EXPORT void glPointParameterfvEXT_wrapper(
  KL::Size pname,
  KL::VariableArray<KL::Scalar> & params
){
  _clearError();
  glPointParameterfvEXT( (GLenum)pname, (GLfloat*)&params[0] );
  _checkError("glPointParameterfvEXT");
}

FABRIC_EXT_EXPORT void glPolygonOffsetEXT_wrapper(
  KL::Scalar factor,
  KL::Scalar bias
){
  _clearError();
  glPolygonOffsetEXT( (GLfloat)factor, (GLfloat)bias );
  _checkError("glPolygonOffsetEXT");
}

FABRIC_EXT_EXPORT void glProvokingVertexEXT_wrapper(
  KL::Size mode
){
  _clearError();
  glProvokingVertexEXT( (GLenum)mode );
  _checkError("glProvokingVertexEXT");
}

FABRIC_EXT_EXPORT void glBeginSceneEXT_wrapper()
{
  _clearError();
  glBeginSceneEXT();
  _checkError("glBeginSceneEXT");
}

FABRIC_EXT_EXPORT void glEndSceneEXT_wrapper()
{
  _clearError();
  glEndSceneEXT();
  _checkError("glEndSceneEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3bEXT_wrapper(
  KL::Byte red,
  KL::Byte green,
  KL::Byte blue
){
  _clearError();
  glSecondaryColor3bEXT( (GLbyte)red, (GLbyte)green, (GLbyte)blue );
  _checkError("glSecondaryColor3bEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3bvEXT_wrapper(
  const KL::VariableArray<KL::Byte> & v
){
  _clearError();
  glSecondaryColor3bvEXT( (const GLbyte*)&v[0] );
  _checkError("glSecondaryColor3bvEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3dEXT_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue
){
  _clearError();
  glSecondaryColor3dEXT( (GLdouble)red, (GLdouble)green, (GLdouble)blue );
  _checkError("glSecondaryColor3dEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3dvEXT_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glSecondaryColor3dvEXT( (const GLdouble*)&v[0] );
  _checkError("glSecondaryColor3dvEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3fEXT_wrapper(
  KL::Scalar red,
  KL::Scalar green,
  KL::Scalar blue
){
  _clearError();
  glSecondaryColor3fEXT( (GLfloat)red, (GLfloat)green, (GLfloat)blue );
  _checkError("glSecondaryColor3fEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3fvEXT_wrapper(
  const KL::VariableArray<KL::Scalar> & v
){
  _clearError();
  glSecondaryColor3fvEXT( (const GLfloat*)&v[0] );
  _checkError("glSecondaryColor3fvEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3iEXT_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
  glSecondaryColor3iEXT( (GLint)red, (GLint)green, (GLint)blue );
  _checkError("glSecondaryColor3iEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3ivEXT_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glSecondaryColor3ivEXT( (const GLint*)&v[0] );
  _checkError("glSecondaryColor3ivEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3sEXT_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
  glSecondaryColor3sEXT( (GLshort)red, (GLshort)green, (GLshort)blue );
  _checkError("glSecondaryColor3sEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3svEXT_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
  glSecondaryColor3svEXT( (const GLshort*)&v[0] );
  _checkError("glSecondaryColor3svEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3ubEXT_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue
){
  _clearError();
  glSecondaryColor3ubEXT( (GLubyte)red, (GLubyte)green, (GLubyte)blue );
  _checkError("glSecondaryColor3ubEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3ubvEXT_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glSecondaryColor3ubvEXT( (const GLubyte*)&v[0] );
  _checkError("glSecondaryColor3ubvEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3uiEXT_wrapper(
  KL::Size red,
  KL::Size green,
  KL::Size blue
){
  _clearError();
  glSecondaryColor3uiEXT( (GLuint)red, (GLuint)green, (GLuint)blue );
  _checkError("glSecondaryColor3uiEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3uivEXT_wrapper(
  const KL::VariableArray<KL::Size> & v
){
  _clearError();
  glSecondaryColor3uivEXT( (const GLuint*)&v[0] );
  _checkError("glSecondaryColor3uivEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3usEXT_wrapper(
  KL::Integer red,
  KL::Integer green,
  KL::Integer blue
){
  _clearError();
  glSecondaryColor3usEXT( (GLushort)red, (GLushort)green, (GLushort)blue );
  _checkError("glSecondaryColor3usEXT");
}

FABRIC_EXT_EXPORT void glSecondaryColor3usvEXT_wrapper(
  const KL::VariableArray<KL::Integer> & v
){
  _clearError();
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
  glSecondaryColorPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glSecondaryColorPointerEXT");
}

FABRIC_EXT_EXPORT void glActiveProgramEXT_wrapper(
  KL::Size program
){
  _clearError();
  glActiveProgramEXT( (GLuint)program );
  _checkError("glActiveProgramEXT");
}

FABRIC_EXT_EXPORT KL::Size glCreateShaderProgramEXT_wrapper(
  KL::Size type,
  const KL::String &string
){
  _clearError();
  GLuint result = glCreateShaderProgramEXT( (GLenum)type, (const char*)string.data() );
  _checkError("glCreateShaderProgramEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glUseShaderProgramEXT_wrapper(
  KL::Size type,
  KL::Size program
){
  _clearError();
  glUseShaderProgramEXT( (GLenum)type, (GLuint)program );
  _checkError("glUseShaderProgramEXT");
}

FABRIC_EXT_EXPORT void glActiveStencilFaceEXT_wrapper(
  KL::Size face
){
  _clearError();
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
  glTexImage3DEXT( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  _checkError("glTexImage3DEXT");
}

FABRIC_EXT_EXPORT void glTexBufferEXT_wrapper(
  KL::Size target,
  KL::Size internalformat,
  KL::Size buffer
){
  _clearError();
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
  glClearColorIuiEXT( (GLuint)red, (GLuint)green, (GLuint)blue, (GLuint)alpha );
  _checkError("glClearColorIuiEXT");
}

FABRIC_EXT_EXPORT void glGetTexParameterIivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glGetTexParameterIivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  _checkError("glGetTexParameterIivEXT");
}

FABRIC_EXT_EXPORT void glGetTexParameterIuivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  KL::VariableArray<KL::Size> & params
){
  _clearError();
  glGetTexParameterIuivEXT( (GLenum)target, (GLenum)pname, (GLuint*)&params[0] );
  _checkError("glGetTexParameterIuivEXT");
}

FABRIC_EXT_EXPORT void glTexParameterIivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Integer> & params
){
  _clearError();
  glTexParameterIivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  _checkError("glTexParameterIivEXT");
}

FABRIC_EXT_EXPORT void glTexParameterIuivEXT_wrapper(
  KL::Size target,
  KL::Size pname,
  const KL::VariableArray<KL::Size> & params
){
  _clearError();
  glTexParameterIuivEXT( (GLenum)target, (GLenum)pname, (const GLuint*)&params[0] );
  _checkError("glTexParameterIuivEXT");
}

FABRIC_EXT_EXPORT KL::Boolean glAreTexturesResidentEXT_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & textures,
  KL::VariableArray<KL::Boolean> & residences
){
  _clearError();
  GLboolean result = glAreTexturesResidentEXT( (GLsizei)n, (const GLuint*)&textures[0], (GLboolean*)&residences[0] );
  _checkError("glAreTexturesResidentEXT");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void glBindTextureEXT_wrapper(
  KL::Size target,
  KL::Size texture
){
  _clearError();
  glBindTextureEXT( (GLenum)target, (GLuint)texture );
  _checkError("glBindTextureEXT");
}

FABRIC_EXT_EXPORT void glDeleteTexturesEXT_wrapper(
  KL::Size n,
  const KL::VariableArray<KL::Size> & textures
){
  _clearError();
  glDeleteTexturesEXT( (GLsizei)n, (const GLuint*)&textures[0] );
  _checkError("glDeleteTexturesEXT");
}

FABRIC_EXT_EXPORT void glGenTexturesEXT_wrapper(
  KL::Size n,
  KL::VariableArray<KL::Size> & textures
){
  _clearError();
  glGenTexturesEXT( (GLsizei)n, (GLuint*)&textures[0] );
  _checkError("glGenTexturesEXT");
}

FABRIC_EXT_EXPORT KL::Boolean glIsTextureEXT_wrapper(
  KL::Size texture
){
  _clearError();
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
  glPrioritizeTexturesEXT( (GLsizei)n, (const GLuint*)&textures[0], (const GLclampf*)&priorities[0] );
  _checkError("glPrioritizeTexturesEXT");
}

FABRIC_EXT_EXPORT void glTextureNormalEXT_wrapper(
  KL::Size mode
){
  _clearError();
  glTextureNormalEXT( (GLenum)mode );
  _checkError("glTextureNormalEXT");
}

FABRIC_EXT_EXPORT void glBeginTransformFeedbackEXT_wrapper(
  KL::Size primitiveMode
){
  _clearError();
  glBeginTransformFeedbackEXT( (GLenum)primitiveMode );
  _checkError("glBeginTransformFeedbackEXT");
}

FABRIC_EXT_EXPORT void glBindBufferBaseEXT_wrapper(
  KL::Size target,
  KL::Size index,
  KL::Size buffer
){
  _clearError();
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
  glBindBufferRangeEXT( (GLenum)target, (GLuint)index, (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size );
  _checkError("glBindBufferRangeEXT");
}

FABRIC_EXT_EXPORT void glEndTransformFeedbackEXT_wrapper()
{
  _clearError();
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
  glTransformFeedbackVaryingsEXT( (GLuint)program, (GLsizei)count, (const char**)&varyings[0], (GLenum)bufferMode );
  _checkError("glTransformFeedbackVaryingsEXT");
}

FABRIC_EXT_EXPORT void glArrayElementEXT_wrapper(
  KL::Integer i
){
  _clearError();
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
  glColorPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
  _checkError("glColorPointerEXT");
}

FABRIC_EXT_EXPORT void glDrawArraysEXT_wrapper(
  KL::Size mode,
  KL::Integer first,
  KL::Size count
){
  _clearError();
  glDrawArraysEXT( (GLenum)mode, (GLint)first, (GLsizei)count );
  _checkError("glDrawArraysEXT");
}

FABRIC_EXT_EXPORT void glEdgeFlagPointerEXT_wrapper(
  KL::Size stride,
  KL::Size count,
  const KL::VariableArray<KL::Boolean> & pointer
){
  _clearError();
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
  glVertexPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
  _checkError("glVertexPointerEXT");
}

FABRIC_EXT_EXPORT void glBeginVertexShaderEXT_wrapper()
{
  _clearError();
  glBeginVertexShaderEXT();
  _checkError("glBeginVertexShaderEXT");
}

FABRIC_EXT_EXPORT KL::Size glBindLightParameterEXT_wrapper(
  KL::Size light,
  KL::Size value
){
  _clearError();
  GLuint result = glBindLightParameterEXT( (GLenum)light, (GLenum)value );
  _checkError("glBindLightParameterEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size glBindMaterialParameterEXT_wrapper(
  KL::Size face,
  KL::Size value
){
  _clearError();
  GLuint result = glBindMaterialParameterEXT( (GLenum)face, (GLenum)value );
  _checkError("glBindMaterialParameterEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size glBindParameterEXT_wrapper(
  KL::Size value
){
  _clearError();
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
  GLuint result = glBindTexGenParameterEXT( (GLenum)unit, (GLenum)coord, (GLenum)value );
  _checkError("glBindTexGenParameterEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size glBindTextureUnitParameterEXT_wrapper(
  KL::Size unit,
  KL::Size value
){
  _clearError();
  GLuint result = glBindTextureUnitParameterEXT( (GLenum)unit, (GLenum)value );
  _checkError("glBindTextureUnitParameterEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glBindVertexShaderEXT_wrapper(
  KL::Size id
){
  _clearError();
  glBindVertexShaderEXT( (GLuint)id );
  _checkError("glBindVertexShaderEXT");
}

FABRIC_EXT_EXPORT void glDeleteVertexShaderEXT_wrapper(
  KL::Size id
){
  _clearError();
  glDeleteVertexShaderEXT( (GLuint)id );
  _checkError("glDeleteVertexShaderEXT");
}

FABRIC_EXT_EXPORT void glDisableVariantClientStateEXT_wrapper(
  KL::Size id
){
  _clearError();
  glDisableVariantClientStateEXT( (GLuint)id );
  _checkError("glDisableVariantClientStateEXT");
}

FABRIC_EXT_EXPORT void glEnableVariantClientStateEXT_wrapper(
  KL::Size id
){
  _clearError();
  glEnableVariantClientStateEXT( (GLuint)id );
  _checkError("glEnableVariantClientStateEXT");
}

FABRIC_EXT_EXPORT void glEndVertexShaderEXT_wrapper()
{
  _clearError();
  glEndVertexShaderEXT();
  _checkError("glEndVertexShaderEXT");
}

FABRIC_EXT_EXPORT void glExtractComponentEXT_wrapper(
  KL::Size res,
  KL::Size src,
  KL::Size num
){
  _clearError();
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
  GLuint result = glGenSymbolsEXT( (GLenum)dataType, (GLenum)storageType, (GLenum)range, (GLuint)components );
  _checkError("glGenSymbolsEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size glGenVertexShadersEXT_wrapper(
  KL::Size range
){
  _clearError();
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
  glGetInvariantBooleanvEXT( (GLuint)id, (GLenum)value, (GLboolean*)&data[0] );
  _checkError("glGetInvariantBooleanvEXT");
}

FABRIC_EXT_EXPORT void glGetInvariantFloatvEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Scalar> & data
){
  _clearError();
  glGetInvariantFloatvEXT( (GLuint)id, (GLenum)value, (GLfloat*)&data[0] );
  _checkError("glGetInvariantFloatvEXT");
}

FABRIC_EXT_EXPORT void glGetInvariantIntegervEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Integer> & data
){
  _clearError();
  glGetInvariantIntegervEXT( (GLuint)id, (GLenum)value, (GLint*)&data[0] );
  _checkError("glGetInvariantIntegervEXT");
}

FABRIC_EXT_EXPORT void glGetLocalConstantBooleanvEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Boolean> & data
){
  _clearError();
  glGetLocalConstantBooleanvEXT( (GLuint)id, (GLenum)value, (GLboolean*)&data[0] );
  _checkError("glGetLocalConstantBooleanvEXT");
}

FABRIC_EXT_EXPORT void glGetLocalConstantFloatvEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Scalar> & data
){
  _clearError();
  glGetLocalConstantFloatvEXT( (GLuint)id, (GLenum)value, (GLfloat*)&data[0] );
  _checkError("glGetLocalConstantFloatvEXT");
}

FABRIC_EXT_EXPORT void glGetLocalConstantIntegervEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Integer> & data
){
  _clearError();
  glGetLocalConstantIntegervEXT( (GLuint)id, (GLenum)value, (GLint*)&data[0] );
  _checkError("glGetLocalConstantIntegervEXT");
}

FABRIC_EXT_EXPORT void glGetVariantBooleanvEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Boolean> & data
){
  _clearError();
  glGetVariantBooleanvEXT( (GLuint)id, (GLenum)value, (GLboolean*)&data[0] );
  _checkError("glGetVariantBooleanvEXT");
}

FABRIC_EXT_EXPORT void glGetVariantFloatvEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Scalar> & data
){
  _clearError();
  glGetVariantFloatvEXT( (GLuint)id, (GLenum)value, (GLfloat*)&data[0] );
  _checkError("glGetVariantFloatvEXT");
}

FABRIC_EXT_EXPORT void glGetVariantIntegervEXT_wrapper(
  KL::Size id,
  KL::Size value,
  KL::VariableArray<KL::Integer> & data
){
  _clearError();
  glGetVariantIntegervEXT( (GLuint)id, (GLenum)value, (GLint*)&data[0] );
  _checkError("glGetVariantIntegervEXT");
}

FABRIC_EXT_EXPORT void glInsertComponentEXT_wrapper(
  KL::Size res,
  KL::Size src,
  KL::Size num
){
  _clearError();
  glInsertComponentEXT( (GLuint)res, (GLuint)src, (GLuint)num );
  _checkError("glInsertComponentEXT");
}

FABRIC_EXT_EXPORT KL::Boolean glIsVariantEnabledEXT_wrapper(
  KL::Size id,
  KL::Size cap
){
  _clearError();
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
  glSetInvariantEXT( (GLuint)id, (GLenum)type, addr );
  _checkError("glSetInvariantEXT");
}

FABRIC_EXT_EXPORT void glSetLocalConstantEXT_wrapper(
  KL::Size id,
  KL::Size type,
  KL::Data addr
){
  _clearError();
  glSetLocalConstantEXT( (GLuint)id, (GLenum)type, addr );
  _checkError("glSetLocalConstantEXT");
}

FABRIC_EXT_EXPORT void glShaderOp1EXT_wrapper(
  KL::Size op,
  KL::Size res,
  KL::Size arg1
){
  _clearError();
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
  glVariantPointerEXT( (GLuint)id, (GLenum)type, (GLuint)stride, addr );
  _checkError("glVariantPointerEXT");
}

FABRIC_EXT_EXPORT void glVariantbvEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Byte> & addr
){
  _clearError();
  glVariantbvEXT( (GLuint)id, (GLbyte*)&addr[0] );
  _checkError("glVariantbvEXT");
}

FABRIC_EXT_EXPORT void glVariantdvEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Scalar> & addr
){
  _clearError();
  glVariantdvEXT( (GLuint)id, (GLdouble*)&addr[0] );
  _checkError("glVariantdvEXT");
}

FABRIC_EXT_EXPORT void glVariantfvEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Scalar> & addr
){
  _clearError();
  glVariantfvEXT( (GLuint)id, (GLfloat*)&addr[0] );
  _checkError("glVariantfvEXT");
}

FABRIC_EXT_EXPORT void glVariantivEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Integer> & addr
){
  _clearError();
  glVariantivEXT( (GLuint)id, (GLint*)&addr[0] );
  _checkError("glVariantivEXT");
}

FABRIC_EXT_EXPORT void glVariantsvEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Integer> & addr
){
  _clearError();
  glVariantsvEXT( (GLuint)id, (GLshort*)&addr[0] );
  _checkError("glVariantsvEXT");
}

FABRIC_EXT_EXPORT void glVariantubvEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Size> & addr
){
  _clearError();
  glVariantubvEXT( (GLuint)id, (GLubyte*)&addr[0] );
  _checkError("glVariantubvEXT");
}

FABRIC_EXT_EXPORT void glVariantuivEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Size> & addr
){
  _clearError();
  glVariantuivEXT( (GLuint)id, (GLuint*)&addr[0] );
  _checkError("glVariantuivEXT");
}

FABRIC_EXT_EXPORT void glVariantusvEXT_wrapper(
  KL::Size id,
  KL::VariableArray<KL::Integer> & addr
){
  _clearError();
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
  glVertexWeightPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  _checkError("glVertexWeightPointerEXT");
}

FABRIC_EXT_EXPORT void glVertexWeightfEXT_wrapper(
  KL::Scalar weight
){
  _clearError();
  glVertexWeightfEXT( (GLfloat)weight );
  _checkError("glVertexWeightfEXT");
}

FABRIC_EXT_EXPORT void glVertexWeightfvEXT_wrapper(
  KL::VariableArray<KL::Scalar> & weight
){
  _clearError();
  glVertexWeightfvEXT( (GLfloat*)&weight[0] );
  _checkError("glVertexWeightfvEXT");
}

FABRIC_EXT_EXPORT KL::Size glBufferRegionEnabledEXT_wrapper()
{
  _clearError();
  GLuint result = glBufferRegionEnabledEXT();
  _checkError("glBufferRegionEnabledEXT");
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void glDeleteBufferRegionEXT_wrapper(
  KL::Size region
){
  _clearError();
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
  glDrawBufferRegionEXT( (GLuint)region, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)xDest, (GLint)yDest );
  _checkError("glDrawBufferRegionEXT");
}

FABRIC_EXT_EXPORT KL::Size glNewBufferRegionEXT_wrapper(
  KL::Size region
){
  _clearError();
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
  glReadBufferRegionEXT( (GLuint)region, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  _checkError("glReadBufferRegionEXT");
}

FABRIC_EXT_EXPORT void glXWaitGL_wrapper()
{
  _clearError();
  glXWaitGL();
  _checkError("glXWaitGL");
}

FABRIC_EXT_EXPORT void glXWaitX_wrapper()
{
  _clearError();
  glXWaitX();
  _checkError("glXWaitX");
}

FABRIC_EXT_EXPORT KL::Boolean glxewIsSupported_wrapper(
  const KL::String &name
){
  _clearError();
  GLboolean result = glxewIsSupported( (const char*)name.data() );
  _checkError("glxewIsSupported");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean glxewGetExtension_wrapper(
  const KL::String &name
){
  _clearError();
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
  GLint result = gluBuild3DMipmaps( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, data );
  _checkError("gluBuild3DMipmaps");
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Boolean gluCheckExtension_wrapper(
  const KL::VariableArray<KL::Size> & extName,
  const KL::VariableArray<KL::Size> & extString
){
  _clearError();
  GLboolean result = gluCheckExtension( (const GLubyte*)&extName[0], (const GLubyte*)&extString[0] );
  _checkError("gluCheckExtension");
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::String gluErrorString_wrapper(
  KL::Size error
){
  const GLubyte* result = gluErrorString( (GLenum)error );
  return (KL::String)(const char*)result;
}

FABRIC_EXT_EXPORT KL::String gluGetString_wrapper(
  KL::Size name
){
  _clearError();
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
  GLint result = gluUnProject4( (GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, (GLdouble)clipW, (const GLdouble*)&model[0], (const GLdouble*)&proj[0], (const GLint*)&view[0], (GLdouble)nearVal, (GLdouble)farVal, (GLdouble*)&objX[0], (GLdouble*)&objY[0], (GLdouble*)&objZ[0], (GLdouble*)&objW[0] );
  _checkError("gluUnProject4");
  return (KL::Integer)result;
}

