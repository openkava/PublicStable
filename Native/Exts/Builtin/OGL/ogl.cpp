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

FABRIC_EXT_EXPORT void klAccum(
  const KL::Size & op,
  const KL::Scalar & value
){
  glAccum( (GLenum)op, (GLfloat)value );
}

FABRIC_EXT_EXPORT void klAlphaFunc(
  const KL::Size & func,
  const KL::Scalar & ref
){
  glAlphaFunc( (GLenum)func, (GLclampf)ref );
}

FABRIC_EXT_EXPORT KL::Boolean klAreTexturesResident(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & textures,
  KL::VariableArray<KL::Boolean> & residences
){
  GLboolean result = glAreTexturesResident( (GLsizei)n, (const GLuint*)&textures[0], (GLboolean*)&residences[0] );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void klArrayElement(
  const KL::Integer & i
){
  glArrayElement( (GLint)i );
}

FABRIC_EXT_EXPORT void klBegin(
  const KL::Size & mode
){
  glBegin( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klBindTexture(
  const KL::Size & target,
  const KL::Size & texture
){
  glBindTexture( (GLenum)target, (GLuint)texture );
}

FABRIC_EXT_EXPORT void klBitmap(
  const KL::Size & width,
  const KL::Size & height,
  const KL::Scalar & xorig,
  const KL::Scalar & yorig,
  const KL::Scalar & xmove,
  const KL::Scalar & ymove,
  const KL::VariableArray<KL::Size> & bitmap
){
  glBitmap( (GLsizei)width, (GLsizei)height, (GLfloat)xorig, (GLfloat)yorig, (GLfloat)xmove, (GLfloat)ymove, (const GLubyte*)&bitmap[0] );
}

FABRIC_EXT_EXPORT void klBlendFunc(
  const KL::Size & sfactor,
  const KL::Size & dfactor
){
  glBlendFunc( (GLenum)sfactor, (GLenum)dfactor );
}

FABRIC_EXT_EXPORT void klCallList(
  const KL::Size & list
){
  glCallList( (GLuint)list );
}

FABRIC_EXT_EXPORT void klCallLists(
  const KL::Size & n,
  const KL::Size & type,
  KL::Data lists
){
  glCallLists( (GLsizei)n, (GLenum)type, lists );
}

FABRIC_EXT_EXPORT void klClear(
  const KL::Integer & mask
){
  glClear( (GLbitfield)mask );
}

FABRIC_EXT_EXPORT void klClearAccum(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue,
  const KL::Scalar & alpha
){
  glClearAccum( (GLfloat)red, (GLfloat)green, (GLfloat)blue, (GLfloat)alpha );
}

FABRIC_EXT_EXPORT void klClearColor(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue,
  const KL::Scalar & alpha
){
  glClearColor( (GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha );
}

FABRIC_EXT_EXPORT void klClearDepth(
  const KL::Scalar & depth
){
  glClearDepth( (GLclampd)depth );
}

FABRIC_EXT_EXPORT void klClearIndex(
  const KL::Scalar & c
){
  glClearIndex( (GLfloat)c );
}

FABRIC_EXT_EXPORT void klClearStencil(
  const KL::Integer & s
){
  glClearStencil( (GLint)s );
}

FABRIC_EXT_EXPORT void klClipPlane(
  const KL::Size & plane,
  const KL::VariableArray<KL::Scalar> & equation
){
  glClipPlane( (GLenum)plane, (const GLdouble*)&equation[0] );
}

FABRIC_EXT_EXPORT void klColor3b(
  const KL::Byte & red,
  const KL::Byte & green,
  const KL::Byte & blue
){
  glColor3b( (GLbyte)red, (GLbyte)green, (GLbyte)blue );
}

FABRIC_EXT_EXPORT void klColor3bv(
  const KL::VariableArray<KL::Byte> & v
){
  glColor3bv( (const GLbyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor3d(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue
){
  glColor3d( (GLdouble)red, (GLdouble)green, (GLdouble)blue );
}

FABRIC_EXT_EXPORT void klColor3dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glColor3dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor3f(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue
){
  glColor3f( (GLfloat)red, (GLfloat)green, (GLfloat)blue );
}

FABRIC_EXT_EXPORT void klColor3fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glColor3fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor3i(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  glColor3i( (GLint)red, (GLint)green, (GLint)blue );
}

FABRIC_EXT_EXPORT void klColor3iv(
  const KL::VariableArray<KL::Integer> & v
){
  glColor3iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor3s(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  glColor3s( (GLshort)red, (GLshort)green, (GLshort)blue );
}

FABRIC_EXT_EXPORT void klColor3sv(
  const KL::VariableArray<KL::Integer> & v
){
  glColor3sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor3ub(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue
){
  glColor3ub( (GLubyte)red, (GLubyte)green, (GLubyte)blue );
}

FABRIC_EXT_EXPORT void klColor3ubv(
  const KL::VariableArray<KL::Size> & v
){
  glColor3ubv( (const GLubyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor3ui(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue
){
  glColor3ui( (GLuint)red, (GLuint)green, (GLuint)blue );
}

FABRIC_EXT_EXPORT void klColor3uiv(
  const KL::VariableArray<KL::Size> & v
){
  glColor3uiv( (const GLuint*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor3us(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  glColor3us( (GLushort)red, (GLushort)green, (GLushort)blue );
}

FABRIC_EXT_EXPORT void klColor3usv(
  const KL::VariableArray<KL::Integer> & v
){
  glColor3usv( (const GLushort*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor4b(
  const KL::Byte & red,
  const KL::Byte & green,
  const KL::Byte & blue,
  const KL::Byte & alpha
){
  glColor4b( (GLbyte)red, (GLbyte)green, (GLbyte)blue, (GLbyte)alpha );
}

FABRIC_EXT_EXPORT void klColor4bv(
  const KL::VariableArray<KL::Byte> & v
){
  glColor4bv( (const GLbyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor4d(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue,
  const KL::Scalar & alpha
){
  glColor4d( (GLdouble)red, (GLdouble)green, (GLdouble)blue, (GLdouble)alpha );
}

FABRIC_EXT_EXPORT void klColor4dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glColor4dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor4f(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue,
  const KL::Scalar & alpha
){
  glColor4f( (GLfloat)red, (GLfloat)green, (GLfloat)blue, (GLfloat)alpha );
}

FABRIC_EXT_EXPORT void klColor4fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glColor4fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor4i(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue,
  const KL::Integer & alpha
){
  glColor4i( (GLint)red, (GLint)green, (GLint)blue, (GLint)alpha );
}

FABRIC_EXT_EXPORT void klColor4iv(
  const KL::VariableArray<KL::Integer> & v
){
  glColor4iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor4s(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue,
  const KL::Integer & alpha
){
  glColor4s( (GLshort)red, (GLshort)green, (GLshort)blue, (GLshort)alpha );
}

FABRIC_EXT_EXPORT void klColor4sv(
  const KL::VariableArray<KL::Integer> & v
){
  glColor4sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor4ub(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue,
  const KL::Size & alpha
){
  glColor4ub( (GLubyte)red, (GLubyte)green, (GLubyte)blue, (GLubyte)alpha );
}

FABRIC_EXT_EXPORT void klColor4ubv(
  const KL::VariableArray<KL::Size> & v
){
  glColor4ubv( (const GLubyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor4ui(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue,
  const KL::Size & alpha
){
  glColor4ui( (GLuint)red, (GLuint)green, (GLuint)blue, (GLuint)alpha );
}

FABRIC_EXT_EXPORT void klColor4uiv(
  const KL::VariableArray<KL::Size> & v
){
  glColor4uiv( (const GLuint*)&v[0] );
}

FABRIC_EXT_EXPORT void klColor4us(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue,
  const KL::Integer & alpha
){
  glColor4us( (GLushort)red, (GLushort)green, (GLushort)blue, (GLushort)alpha );
}

FABRIC_EXT_EXPORT void klColor4usv(
  const KL::VariableArray<KL::Integer> & v
){
  glColor4usv( (const GLushort*)&v[0] );
}

FABRIC_EXT_EXPORT void klColorMask(
  const KL::Boolean & red,
  const KL::Boolean & green,
  const KL::Boolean & blue,
  const KL::Boolean & alpha
){
  glColorMask( (GLboolean)red, (GLboolean)green, (GLboolean)blue, (GLboolean)alpha );
}

FABRIC_EXT_EXPORT void klColorMaterial(
  const KL::Size & face,
  const KL::Size & mode
){
  glColorMaterial( (GLenum)face, (GLenum)mode );
}

FABRIC_EXT_EXPORT void klColorPointer(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glColorPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klCopyPixels(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & type
){
  glCopyPixels( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLenum)type );
}

FABRIC_EXT_EXPORT void klCopyTexImage1D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalFormat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Integer & border
){
  glCopyTexImage1D( (GLenum)target, (GLint)level, (GLenum)internalFormat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border );
}

FABRIC_EXT_EXPORT void klCopyTexImage2D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalFormat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Integer & border
){
  glCopyTexImage2D( (GLenum)target, (GLint)level, (GLenum)internalFormat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border );
}

FABRIC_EXT_EXPORT void klCopyTexSubImage1D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  glCopyTexSubImage1D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width );
}

FABRIC_EXT_EXPORT void klCopyTexSubImage2D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glCopyTexSubImage2D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klCullFace(
  const KL::Size & mode
){
  glCullFace( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klDeleteLists(
  const KL::Size & list,
  const KL::Size & range
){
  glDeleteLists( (GLuint)list, (GLsizei)range );
}

FABRIC_EXT_EXPORT void klDeleteTextures(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & textures
){
  glDeleteTextures( (GLsizei)n, (const GLuint*)&textures[0] );
}

FABRIC_EXT_EXPORT void klDepthFunc(
  const KL::Size & func
){
  glDepthFunc( (GLenum)func );
}

FABRIC_EXT_EXPORT void klDepthMask(
  const KL::Boolean & flag
){
  glDepthMask( (GLboolean)flag );
}

FABRIC_EXT_EXPORT void klDepthRange(
  const KL::Scalar & zNear,
  const KL::Scalar & zFar
){
  glDepthRange( (GLclampd)zNear, (GLclampd)zFar );
}

FABRIC_EXT_EXPORT void klDisable(
  const KL::Size & cap
){
  glDisable( (GLenum)cap );
}

FABRIC_EXT_EXPORT void klDisableClientState(
  const KL::Size & array
){
  glDisableClientState( (GLenum)array );
}

FABRIC_EXT_EXPORT void klDrawArrays(
  const KL::Size & mode,
  const KL::Integer & first,
  const KL::Size & count
){
  glDrawArrays( (GLenum)mode, (GLint)first, (GLsizei)count );
}

FABRIC_EXT_EXPORT void klDrawBuffer(
  const KL::Size & mode
){
  glDrawBuffer( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klDrawElements(
  const KL::Size & mode,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices
){
  glDrawElements( (GLenum)mode, (GLsizei)count, (GLenum)type, indices );
}

FABRIC_EXT_EXPORT void klDrawPixels(
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glDrawPixels( (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klEdgeFlag(
  const KL::Boolean & flag
){
  glEdgeFlag( (GLboolean)flag );
}

FABRIC_EXT_EXPORT void klEdgeFlagPointer(
  const KL::Size & stride,
  KL::Data pointer
){
  glEdgeFlagPointer( (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klEdgeFlagv(
  const KL::VariableArray<KL::Boolean> & flag
){
  glEdgeFlagv( (const GLboolean*)&flag[0] );
}

FABRIC_EXT_EXPORT void klEnable(
  const KL::Size & cap
){
  glEnable( (GLenum)cap );
}

FABRIC_EXT_EXPORT void klEnableClientState(
  const KL::Size & array
){
  glEnableClientState( (GLenum)array );
}

FABRIC_EXT_EXPORT void klEnd()
{
  glEnd();
}

FABRIC_EXT_EXPORT void klEndList()
{
  glEndList();
}

FABRIC_EXT_EXPORT void klEvalCoord1d(
  const KL::Scalar & u
){
  glEvalCoord1d( (GLdouble)u );
}

FABRIC_EXT_EXPORT void klEvalCoord1dv(
  const KL::VariableArray<KL::Scalar> & u
){
  glEvalCoord1dv( (const GLdouble*)&u[0] );
}

FABRIC_EXT_EXPORT void klEvalCoord1f(
  const KL::Scalar & u
){
  glEvalCoord1f( (GLfloat)u );
}

FABRIC_EXT_EXPORT void klEvalCoord1fv(
  const KL::VariableArray<KL::Scalar> & u
){
  glEvalCoord1fv( (const GLfloat*)&u[0] );
}

FABRIC_EXT_EXPORT void klEvalCoord2d(
  const KL::Scalar & u,
  const KL::Scalar & v
){
  glEvalCoord2d( (GLdouble)u, (GLdouble)v );
}

FABRIC_EXT_EXPORT void klEvalCoord2dv(
  const KL::VariableArray<KL::Scalar> & u
){
  glEvalCoord2dv( (const GLdouble*)&u[0] );
}

FABRIC_EXT_EXPORT void klEvalCoord2f(
  const KL::Scalar & u,
  const KL::Scalar & v
){
  glEvalCoord2f( (GLfloat)u, (GLfloat)v );
}

FABRIC_EXT_EXPORT void klEvalCoord2fv(
  const KL::VariableArray<KL::Scalar> & u
){
  glEvalCoord2fv( (const GLfloat*)&u[0] );
}

FABRIC_EXT_EXPORT void klEvalMesh1(
  const KL::Size & mode,
  const KL::Integer & i1,
  const KL::Integer & i2
){
  glEvalMesh1( (GLenum)mode, (GLint)i1, (GLint)i2 );
}

FABRIC_EXT_EXPORT void klEvalMesh2(
  const KL::Size & mode,
  const KL::Integer & i1,
  const KL::Integer & i2,
  const KL::Integer & j1,
  const KL::Integer & j2
){
  glEvalMesh2( (GLenum)mode, (GLint)i1, (GLint)i2, (GLint)j1, (GLint)j2 );
}

FABRIC_EXT_EXPORT void klEvalPoint1(
  const KL::Integer & i
){
  glEvalPoint1( (GLint)i );
}

FABRIC_EXT_EXPORT void klEvalPoint2(
  const KL::Integer & i,
  const KL::Integer & j
){
  glEvalPoint2( (GLint)i, (GLint)j );
}

FABRIC_EXT_EXPORT void klFeedbackBuffer(
  const KL::Size & size,
  const KL::Size & type,
  KL::VariableArray<KL::Scalar> & buffer
){
  glFeedbackBuffer( (GLsizei)size, (GLenum)type, (GLfloat*)&buffer[0] );
}

FABRIC_EXT_EXPORT void klFinish()
{
  glFinish();
}

FABRIC_EXT_EXPORT void klFlush()
{
  glFlush();
}

FABRIC_EXT_EXPORT void klFogf(
  const KL::Size & pname,
  const KL::Scalar & param
){
  glFogf( (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klFogfv(
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glFogfv( (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klFogi(
  const KL::Size & pname,
  const KL::Integer & param
){
  glFogi( (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klFogiv(
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glFogiv( (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klFrontFace(
  const KL::Size & mode
){
  glFrontFace( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klFrustum(
  const KL::Scalar & left,
  const KL::Scalar & right,
  const KL::Scalar & bottom,
  const KL::Scalar & top,
  const KL::Scalar & zNear,
  const KL::Scalar & zFar
){
  glFrustum( (GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top, (GLdouble)zNear, (GLdouble)zFar );
}

FABRIC_EXT_EXPORT KL::Size klGenLists(
  const KL::Size & range
){
  GLuint result = glGenLists( (GLsizei)range );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klGenTextures(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & textures
){
  glGenTextures( (GLsizei)n, (GLuint*)&textures[0] );
}

FABRIC_EXT_EXPORT void klGetBooleanv(
  const KL::Size & pname,
  KL::VariableArray<KL::Boolean> & params
){
  glGetBooleanv( (GLenum)pname, (GLboolean*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetClipPlane(
  const KL::Size & plane,
  KL::VariableArray<KL::Scalar> & equation
){
  glGetClipPlane( (GLenum)plane, (GLdouble*)&equation[0] );
}

FABRIC_EXT_EXPORT void klGetDoublev(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetDoublev( (GLenum)pname, (GLdouble*)&params[0] );
}

FABRIC_EXT_EXPORT KL::Size klGetError()
{
  GLenum result = glGetError();
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klGetFloatv(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetFloatv( (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetIntegerv(
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetIntegerv( (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetLightfv(
  const KL::Size & light,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetLightfv( (GLenum)light, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetLightiv(
  const KL::Size & light,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetLightiv( (GLenum)light, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMapdv(
  const KL::Size & target,
  const KL::Size & query,
  KL::VariableArray<KL::Scalar> & v
){
  glGetMapdv( (GLenum)target, (GLenum)query, (GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klGetMapfv(
  const KL::Size & target,
  const KL::Size & query,
  KL::VariableArray<KL::Scalar> & v
){
  glGetMapfv( (GLenum)target, (GLenum)query, (GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klGetMapiv(
  const KL::Size & target,
  const KL::Size & query,
  KL::VariableArray<KL::Integer> & v
){
  glGetMapiv( (GLenum)target, (GLenum)query, (GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klGetMaterialfv(
  const KL::Size & face,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetMaterialfv( (GLenum)face, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMaterialiv(
  const KL::Size & face,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetMaterialiv( (GLenum)face, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetPixelMapfv(
  const KL::Size & map,
  KL::VariableArray<KL::Scalar> & values
){
  glGetPixelMapfv( (GLenum)map, (GLfloat*)&values[0] );
}

FABRIC_EXT_EXPORT void klGetPixelMapuiv(
  const KL::Size & map,
  KL::VariableArray<KL::Size> & values
){
  glGetPixelMapuiv( (GLenum)map, (GLuint*)&values[0] );
}

FABRIC_EXT_EXPORT void klGetPixelMapusv(
  const KL::Size & map,
  KL::VariableArray<KL::Integer> & values
){
  glGetPixelMapusv( (GLenum)map, (GLushort*)&values[0] );
}

FABRIC_EXT_EXPORT void klGetPolygonStipple(
  KL::VariableArray<KL::Size> & mask
){
  glGetPolygonStipple( (GLubyte*)&mask[0] );
}

FABRIC_EXT_EXPORT KL::Size klGetString(
  const KL::Size & name
){
  const GLubyte* result = glGetString( (GLenum)name );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klGetTexEnvfv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetTexEnvfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTexEnviv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetTexEnviv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTexGendv(
  const KL::Size & coord,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetTexGendv( (GLenum)coord, (GLenum)pname, (GLdouble*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTexGenfv(
  const KL::Size & coord,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetTexGenfv( (GLenum)coord, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTexGeniv(
  const KL::Size & coord,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetTexGeniv( (GLenum)coord, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTexImage(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glGetTexImage( (GLenum)target, (GLint)level, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klGetTexLevelParameterfv(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetTexLevelParameterfv( (GLenum)target, (GLint)level, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTexLevelParameteriv(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetTexLevelParameteriv( (GLenum)target, (GLint)level, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTexParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetTexParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTexParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetTexParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klHint(
  const KL::Size & target,
  const KL::Size & mode
){
  glHint( (GLenum)target, (GLenum)mode );
}

FABRIC_EXT_EXPORT void klIndexMask(
  const KL::Size & mask
){
  glIndexMask( (GLuint)mask );
}

FABRIC_EXT_EXPORT void klIndexPointer(
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glIndexPointer( (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klIndexd(
  const KL::Scalar & c
){
  glIndexd( (GLdouble)c );
}

FABRIC_EXT_EXPORT void klIndexdv(
  const KL::VariableArray<KL::Scalar> & c
){
  glIndexdv( (const GLdouble*)&c[0] );
}

FABRIC_EXT_EXPORT void klIndexf(
  const KL::Scalar & c
){
  glIndexf( (GLfloat)c );
}

FABRIC_EXT_EXPORT void klIndexfv(
  const KL::VariableArray<KL::Scalar> & c
){
  glIndexfv( (const GLfloat*)&c[0] );
}

FABRIC_EXT_EXPORT void klIndexi(
  const KL::Integer & c
){
  glIndexi( (GLint)c );
}

FABRIC_EXT_EXPORT void klIndexiv(
  const KL::VariableArray<KL::Integer> & c
){
  glIndexiv( (const GLint*)&c[0] );
}

FABRIC_EXT_EXPORT void klIndexs(
  const KL::Integer & c
){
  glIndexs( (GLshort)c );
}

FABRIC_EXT_EXPORT void klIndexsv(
  const KL::VariableArray<KL::Integer> & c
){
  glIndexsv( (const GLshort*)&c[0] );
}

FABRIC_EXT_EXPORT void klIndexub(
  const KL::Size & c
){
  glIndexub( (GLubyte)c );
}

FABRIC_EXT_EXPORT void klIndexubv(
  const KL::VariableArray<KL::Size> & c
){
  glIndexubv( (const GLubyte*)&c[0] );
}

FABRIC_EXT_EXPORT void klInitNames()
{
  glInitNames();
}

FABRIC_EXT_EXPORT void klInterleavedArrays(
  const KL::Size & format,
  const KL::Size & stride,
  KL::Data pointer
){
  glInterleavedArrays( (GLenum)format, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT KL::Boolean klIsEnabled(
  const KL::Size & cap
){
  GLboolean result = glIsEnabled( (GLenum)cap );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean klIsList(
  const KL::Size & list
){
  GLboolean result = glIsList( (GLuint)list );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean klIsTexture(
  const KL::Size & texture
){
  GLboolean result = glIsTexture( (GLuint)texture );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void klLightModelf(
  const KL::Size & pname,
  const KL::Scalar & param
){
  glLightModelf( (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klLightModelfv(
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glLightModelfv( (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klLightModeli(
  const KL::Size & pname,
  const KL::Integer & param
){
  glLightModeli( (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klLightModeliv(
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glLightModeliv( (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klLightf(
  const KL::Size & light,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glLightf( (GLenum)light, (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klLightfv(
  const KL::Size & light,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glLightfv( (GLenum)light, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klLighti(
  const KL::Size & light,
  const KL::Size & pname,
  const KL::Integer & param
){
  glLighti( (GLenum)light, (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klLightiv(
  const KL::Size & light,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glLightiv( (GLenum)light, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klLineStipple(
  const KL::Integer & factor,
  const KL::Integer & pattern
){
  glLineStipple( (GLint)factor, (GLushort)pattern );
}

FABRIC_EXT_EXPORT void klLineWidth(
  const KL::Scalar & width
){
  glLineWidth( (GLfloat)width );
}

FABRIC_EXT_EXPORT void klListBase(
  const KL::Size & base
){
  glListBase( (GLuint)base );
}

FABRIC_EXT_EXPORT void klLoadIdentity()
{
  glLoadIdentity();
}

FABRIC_EXT_EXPORT void klLoadMatrixd(
  const KL::VariableArray<KL::Scalar> & m
){
  glLoadMatrixd( (const GLdouble*)&m[0] );
}

FABRIC_EXT_EXPORT void klLoadMatrixf(
  const KL::VariableArray<KL::Scalar> & m
){
  glLoadMatrixf( (const GLfloat*)&m[0] );
}

FABRIC_EXT_EXPORT void klLoadName(
  const KL::Size & name
){
  glLoadName( (GLuint)name );
}

FABRIC_EXT_EXPORT void klLogicOp(
  const KL::Size & opcode
){
  glLogicOp( (GLenum)opcode );
}

FABRIC_EXT_EXPORT void klMap1d(
  const KL::Size & target,
  const KL::Scalar & u1,
  const KL::Scalar & u2,
  const KL::Integer & stride,
  const KL::Integer & order,
  const KL::VariableArray<KL::Scalar> & points
){
  glMap1d( (GLenum)target, (GLdouble)u1, (GLdouble)u2, (GLint)stride, (GLint)order, (const GLdouble*)&points[0] );
}

FABRIC_EXT_EXPORT void klMap1f(
  const KL::Size & target,
  const KL::Scalar & u1,
  const KL::Scalar & u2,
  const KL::Integer & stride,
  const KL::Integer & order,
  const KL::VariableArray<KL::Scalar> & points
){
  glMap1f( (GLenum)target, (GLfloat)u1, (GLfloat)u2, (GLint)stride, (GLint)order, (const GLfloat*)&points[0] );
}

FABRIC_EXT_EXPORT void klMap2d(
  const KL::Size & target,
  const KL::Scalar & u1,
  const KL::Scalar & u2,
  const KL::Integer & ustride,
  const KL::Integer & uorder,
  const KL::Scalar & v1,
  const KL::Scalar & v2,
  const KL::Integer & vstride,
  const KL::Integer & vorder,
  const KL::VariableArray<KL::Scalar> & points
){
  glMap2d( (GLenum)target, (GLdouble)u1, (GLdouble)u2, (GLint)ustride, (GLint)uorder, (GLdouble)v1, (GLdouble)v2, (GLint)vstride, (GLint)vorder, (const GLdouble*)&points[0] );
}

FABRIC_EXT_EXPORT void klMap2f(
  const KL::Size & target,
  const KL::Scalar & u1,
  const KL::Scalar & u2,
  const KL::Integer & ustride,
  const KL::Integer & uorder,
  const KL::Scalar & v1,
  const KL::Scalar & v2,
  const KL::Integer & vstride,
  const KL::Integer & vorder,
  const KL::VariableArray<KL::Scalar> & points
){
  glMap2f( (GLenum)target, (GLfloat)u1, (GLfloat)u2, (GLint)ustride, (GLint)uorder, (GLfloat)v1, (GLfloat)v2, (GLint)vstride, (GLint)vorder, (const GLfloat*)&points[0] );
}

FABRIC_EXT_EXPORT void klMapGrid1d(
  const KL::Integer & un,
  const KL::Scalar & u1,
  const KL::Scalar & u2
){
  glMapGrid1d( (GLint)un, (GLdouble)u1, (GLdouble)u2 );
}

FABRIC_EXT_EXPORT void klMapGrid1f(
  const KL::Integer & un,
  const KL::Scalar & u1,
  const KL::Scalar & u2
){
  glMapGrid1f( (GLint)un, (GLfloat)u1, (GLfloat)u2 );
}

FABRIC_EXT_EXPORT void klMapGrid2d(
  const KL::Integer & un,
  const KL::Scalar & u1,
  const KL::Scalar & u2,
  const KL::Integer & vn,
  const KL::Scalar & v1,
  const KL::Scalar & v2
){
  glMapGrid2d( (GLint)un, (GLdouble)u1, (GLdouble)u2, (GLint)vn, (GLdouble)v1, (GLdouble)v2 );
}

FABRIC_EXT_EXPORT void klMapGrid2f(
  const KL::Integer & un,
  const KL::Scalar & u1,
  const KL::Scalar & u2,
  const KL::Integer & vn,
  const KL::Scalar & v1,
  const KL::Scalar & v2
){
  glMapGrid2f( (GLint)un, (GLfloat)u1, (GLfloat)u2, (GLint)vn, (GLfloat)v1, (GLfloat)v2 );
}

FABRIC_EXT_EXPORT void klMaterialf(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glMaterialf( (GLenum)face, (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klMaterialfv(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glMaterialfv( (GLenum)face, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klMateriali(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::Integer & param
){
  glMateriali( (GLenum)face, (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klMaterialiv(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glMaterialiv( (GLenum)face, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klMatrixMode(
  const KL::Size & mode
){
  glMatrixMode( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klMultMatrixd(
  const KL::VariableArray<KL::Scalar> & m
){
  glMultMatrixd( (const GLdouble*)&m[0] );
}

FABRIC_EXT_EXPORT void klMultMatrixf(
  const KL::VariableArray<KL::Scalar> & m
){
  glMultMatrixf( (const GLfloat*)&m[0] );
}

FABRIC_EXT_EXPORT void klNewList(
  const KL::Size & list,
  const KL::Size & mode
){
  glNewList( (GLuint)list, (GLenum)mode );
}

FABRIC_EXT_EXPORT void klNormal3b(
  const KL::Byte & nx,
  const KL::Byte & ny,
  const KL::Byte & nz
){
  glNormal3b( (GLbyte)nx, (GLbyte)ny, (GLbyte)nz );
}

FABRIC_EXT_EXPORT void klNormal3bv(
  const KL::VariableArray<KL::Byte> & v
){
  glNormal3bv( (const GLbyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klNormal3d(
  const KL::Scalar & nx,
  const KL::Scalar & ny,
  const KL::Scalar & nz
){
  glNormal3d( (GLdouble)nx, (GLdouble)ny, (GLdouble)nz );
}

FABRIC_EXT_EXPORT void klNormal3dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glNormal3dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klNormal3f(
  const KL::Scalar & nx,
  const KL::Scalar & ny,
  const KL::Scalar & nz
){
  glNormal3f( (GLfloat)nx, (GLfloat)ny, (GLfloat)nz );
}

FABRIC_EXT_EXPORT void klNormal3fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glNormal3fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klNormal3i(
  const KL::Integer & nx,
  const KL::Integer & ny,
  const KL::Integer & nz
){
  glNormal3i( (GLint)nx, (GLint)ny, (GLint)nz );
}

FABRIC_EXT_EXPORT void klNormal3iv(
  const KL::VariableArray<KL::Integer> & v
){
  glNormal3iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klNormal3s(
  const KL::Integer & nx,
  const KL::Integer & ny,
  const KL::Integer & nz
){
  glNormal3s( (GLshort)nx, (GLshort)ny, (GLshort)nz );
}

FABRIC_EXT_EXPORT void klNormal3sv(
  const KL::VariableArray<KL::Integer> & v
){
  glNormal3sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klNormalPointer(
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glNormalPointer( (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klOrtho(
  const KL::Scalar & left,
  const KL::Scalar & right,
  const KL::Scalar & bottom,
  const KL::Scalar & top,
  const KL::Scalar & zNear,
  const KL::Scalar & zFar
){
  glOrtho( (GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top, (GLdouble)zNear, (GLdouble)zFar );
}

FABRIC_EXT_EXPORT void klPassThrough(
  const KL::Scalar & token
){
  glPassThrough( (GLfloat)token );
}

FABRIC_EXT_EXPORT void klPixelMapfv(
  const KL::Size & map,
  const KL::Size & mapsize,
  const KL::VariableArray<KL::Scalar> & values
){
  glPixelMapfv( (GLenum)map, (GLsizei)mapsize, (const GLfloat*)&values[0] );
}

FABRIC_EXT_EXPORT void klPixelMapuiv(
  const KL::Size & map,
  const KL::Size & mapsize,
  const KL::VariableArray<KL::Size> & values
){
  glPixelMapuiv( (GLenum)map, (GLsizei)mapsize, (const GLuint*)&values[0] );
}

FABRIC_EXT_EXPORT void klPixelMapusv(
  const KL::Size & map,
  const KL::Size & mapsize,
  const KL::VariableArray<KL::Integer> & values
){
  glPixelMapusv( (GLenum)map, (GLsizei)mapsize, (const GLushort*)&values[0] );
}

FABRIC_EXT_EXPORT void klPixelStoref(
  const KL::Size & pname,
  const KL::Scalar & param
){
  glPixelStoref( (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klPixelStorei(
  const KL::Size & pname,
  const KL::Integer & param
){
  glPixelStorei( (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klPixelTransferf(
  const KL::Size & pname,
  const KL::Scalar & param
){
  glPixelTransferf( (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klPixelTransferi(
  const KL::Size & pname,
  const KL::Integer & param
){
  glPixelTransferi( (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klPixelZoom(
  const KL::Scalar & xfactor,
  const KL::Scalar & yfactor
){
  glPixelZoom( (GLfloat)xfactor, (GLfloat)yfactor );
}

FABRIC_EXT_EXPORT void klPointSize(
  const KL::Scalar & size
){
  glPointSize( (GLfloat)size );
}

FABRIC_EXT_EXPORT void klPolygonMode(
  const KL::Size & face,
  const KL::Size & mode
){
  glPolygonMode( (GLenum)face, (GLenum)mode );
}

FABRIC_EXT_EXPORT void klPolygonOffset(
  const KL::Scalar & factor,
  const KL::Scalar & units
){
  glPolygonOffset( (GLfloat)factor, (GLfloat)units );
}

FABRIC_EXT_EXPORT void klPolygonStipple(
  const KL::VariableArray<KL::Size> & mask
){
  glPolygonStipple( (const GLubyte*)&mask[0] );
}

FABRIC_EXT_EXPORT void klPopAttrib()
{
  glPopAttrib();
}

FABRIC_EXT_EXPORT void klPopClientAttrib()
{
  glPopClientAttrib();
}

FABRIC_EXT_EXPORT void klPopMatrix()
{
  glPopMatrix();
}

FABRIC_EXT_EXPORT void klPopName()
{
  glPopName();
}

FABRIC_EXT_EXPORT void klPrioritizeTextures(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & textures,
  const KL::VariableArray<KL::Scalar> & priorities
){
  glPrioritizeTextures( (GLsizei)n, (const GLuint*)&textures[0], (const GLclampf*)&priorities[0] );
}

FABRIC_EXT_EXPORT void klPushAttrib(
  const KL::Integer & mask
){
  glPushAttrib( (GLbitfield)mask );
}

FABRIC_EXT_EXPORT void klPushClientAttrib(
  const KL::Integer & mask
){
  glPushClientAttrib( (GLbitfield)mask );
}

FABRIC_EXT_EXPORT void klPushMatrix()
{
  glPushMatrix();
}

FABRIC_EXT_EXPORT void klPushName(
  const KL::Size & name
){
  glPushName( (GLuint)name );
}

FABRIC_EXT_EXPORT void klRasterPos2d(
  const KL::Scalar & x,
  const KL::Scalar & y
){
  glRasterPos2d( (GLdouble)x, (GLdouble)y );
}

FABRIC_EXT_EXPORT void klRasterPos2dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glRasterPos2dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klRasterPos2f(
  const KL::Scalar & x,
  const KL::Scalar & y
){
  glRasterPos2f( (GLfloat)x, (GLfloat)y );
}

FABRIC_EXT_EXPORT void klRasterPos2fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glRasterPos2fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klRasterPos2i(
  const KL::Integer & x,
  const KL::Integer & y
){
  glRasterPos2i( (GLint)x, (GLint)y );
}

FABRIC_EXT_EXPORT void klRasterPos2iv(
  const KL::VariableArray<KL::Integer> & v
){
  glRasterPos2iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klRasterPos2s(
  const KL::Integer & x,
  const KL::Integer & y
){
  glRasterPos2s( (GLshort)x, (GLshort)y );
}

FABRIC_EXT_EXPORT void klRasterPos2sv(
  const KL::VariableArray<KL::Integer> & v
){
  glRasterPos2sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klRasterPos3d(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glRasterPos3d( (GLdouble)x, (GLdouble)y, (GLdouble)z );
}

FABRIC_EXT_EXPORT void klRasterPos3dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glRasterPos3dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klRasterPos3f(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
}

FABRIC_EXT_EXPORT void klRasterPos3fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glRasterPos3fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klRasterPos3i(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  glRasterPos3i( (GLint)x, (GLint)y, (GLint)z );
}

FABRIC_EXT_EXPORT void klRasterPos3iv(
  const KL::VariableArray<KL::Integer> & v
){
  glRasterPos3iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klRasterPos3s(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  glRasterPos3s( (GLshort)x, (GLshort)y, (GLshort)z );
}

FABRIC_EXT_EXPORT void klRasterPos3sv(
  const KL::VariableArray<KL::Integer> & v
){
  glRasterPos3sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klRasterPos4d(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  glRasterPos4d( (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
}

FABRIC_EXT_EXPORT void klRasterPos4dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glRasterPos4dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klRasterPos4f(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  glRasterPos4f( (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
}

FABRIC_EXT_EXPORT void klRasterPos4fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glRasterPos4fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klRasterPos4i(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  glRasterPos4i( (GLint)x, (GLint)y, (GLint)z, (GLint)w );
}

FABRIC_EXT_EXPORT void klRasterPos4iv(
  const KL::VariableArray<KL::Integer> & v
){
  glRasterPos4iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klRasterPos4s(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  glRasterPos4s( (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
}

FABRIC_EXT_EXPORT void klRasterPos4sv(
  const KL::VariableArray<KL::Integer> & v
){
  glRasterPos4sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klReadBuffer(
  const KL::Size & mode
){
  glReadBuffer( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klReadPixels(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glReadPixels( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klRectd(
  const KL::Scalar & x1,
  const KL::Scalar & y1,
  const KL::Scalar & x2,
  const KL::Scalar & y2
){
  glRectd( (GLdouble)x1, (GLdouble)y1, (GLdouble)x2, (GLdouble)y2 );
}

FABRIC_EXT_EXPORT void klRectdv(
  const KL::VariableArray<KL::Scalar> & v1,
  const KL::VariableArray<KL::Scalar> & v2
){
  glRectdv( (const GLdouble*)&v1[0], (const GLdouble*)&v2[0] );
}

FABRIC_EXT_EXPORT void klRectf(
  const KL::Scalar & x1,
  const KL::Scalar & y1,
  const KL::Scalar & x2,
  const KL::Scalar & y2
){
  glRectf( (GLfloat)x1, (GLfloat)y1, (GLfloat)x2, (GLfloat)y2 );
}

FABRIC_EXT_EXPORT void klRectfv(
  const KL::VariableArray<KL::Scalar> & v1,
  const KL::VariableArray<KL::Scalar> & v2
){
  glRectfv( (const GLfloat*)&v1[0], (const GLfloat*)&v2[0] );
}

FABRIC_EXT_EXPORT void klRecti(
  const KL::Integer & x1,
  const KL::Integer & y1,
  const KL::Integer & x2,
  const KL::Integer & y2
){
  glRecti( (GLint)x1, (GLint)y1, (GLint)x2, (GLint)y2 );
}

FABRIC_EXT_EXPORT void klRectiv(
  const KL::VariableArray<KL::Integer> & v1,
  const KL::VariableArray<KL::Integer> & v2
){
  glRectiv( (const GLint*)&v1[0], (const GLint*)&v2[0] );
}

FABRIC_EXT_EXPORT void klRects(
  const KL::Integer & x1,
  const KL::Integer & y1,
  const KL::Integer & x2,
  const KL::Integer & y2
){
  glRects( (GLshort)x1, (GLshort)y1, (GLshort)x2, (GLshort)y2 );
}

FABRIC_EXT_EXPORT void klRectsv(
  const KL::VariableArray<KL::Integer> & v1,
  const KL::VariableArray<KL::Integer> & v2
){
  glRectsv( (const GLshort*)&v1[0], (const GLshort*)&v2[0] );
}

FABRIC_EXT_EXPORT KL::Integer klRenderMode(
  const KL::Size & mode
){
  GLint result = glRenderMode( (GLenum)mode );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT void klRotated(
  const KL::Scalar & angle,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glRotated( (GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z );
}

FABRIC_EXT_EXPORT void klRotatef(
  const KL::Scalar & angle,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glRotatef( (GLfloat)angle, (GLfloat)x, (GLfloat)y, (GLfloat)z );
}

FABRIC_EXT_EXPORT void klScaled(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glScaled( (GLdouble)x, (GLdouble)y, (GLdouble)z );
}

FABRIC_EXT_EXPORT void klScalef(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glScalef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
}

FABRIC_EXT_EXPORT void klScissor(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glScissor( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klSelectBuffer(
  const KL::Size & size,
  KL::VariableArray<KL::Size> & buffer
){
  glSelectBuffer( (GLsizei)size, (GLuint*)&buffer[0] );
}

FABRIC_EXT_EXPORT void klShadeModel(
  const KL::Size & mode
){
  glShadeModel( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klStencilFunc(
  const KL::Size & func,
  const KL::Integer & ref,
  const KL::Size & mask
){
  glStencilFunc( (GLenum)func, (GLint)ref, (GLuint)mask );
}

FABRIC_EXT_EXPORT void klStencilMask(
  const KL::Size & mask
){
  glStencilMask( (GLuint)mask );
}

FABRIC_EXT_EXPORT void klStencilOp(
  const KL::Size & fail,
  const KL::Size & zfail,
  const KL::Size & zpass
){
  glStencilOp( (GLenum)fail, (GLenum)zfail, (GLenum)zpass );
}

FABRIC_EXT_EXPORT void klTexCoord1d(
  const KL::Scalar & s
){
  glTexCoord1d( (GLdouble)s );
}

FABRIC_EXT_EXPORT void klTexCoord1dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glTexCoord1dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord1f(
  const KL::Scalar & s
){
  glTexCoord1f( (GLfloat)s );
}

FABRIC_EXT_EXPORT void klTexCoord1fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glTexCoord1fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord1i(
  const KL::Integer & s
){
  glTexCoord1i( (GLint)s );
}

FABRIC_EXT_EXPORT void klTexCoord1iv(
  const KL::VariableArray<KL::Integer> & v
){
  glTexCoord1iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord1s(
  const KL::Integer & s
){
  glTexCoord1s( (GLshort)s );
}

FABRIC_EXT_EXPORT void klTexCoord1sv(
  const KL::VariableArray<KL::Integer> & v
){
  glTexCoord1sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord2d(
  const KL::Scalar & s,
  const KL::Scalar & t
){
  glTexCoord2d( (GLdouble)s, (GLdouble)t );
}

FABRIC_EXT_EXPORT void klTexCoord2dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glTexCoord2dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord2f(
  const KL::Scalar & s,
  const KL::Scalar & t
){
  glTexCoord2f( (GLfloat)s, (GLfloat)t );
}

FABRIC_EXT_EXPORT void klTexCoord2fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glTexCoord2fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord2i(
  const KL::Integer & s,
  const KL::Integer & t
){
  glTexCoord2i( (GLint)s, (GLint)t );
}

FABRIC_EXT_EXPORT void klTexCoord2iv(
  const KL::VariableArray<KL::Integer> & v
){
  glTexCoord2iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord2s(
  const KL::Integer & s,
  const KL::Integer & t
){
  glTexCoord2s( (GLshort)s, (GLshort)t );
}

FABRIC_EXT_EXPORT void klTexCoord2sv(
  const KL::VariableArray<KL::Integer> & v
){
  glTexCoord2sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord3d(
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r
){
  glTexCoord3d( (GLdouble)s, (GLdouble)t, (GLdouble)r );
}

FABRIC_EXT_EXPORT void klTexCoord3dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glTexCoord3dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord3f(
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r
){
  glTexCoord3f( (GLfloat)s, (GLfloat)t, (GLfloat)r );
}

FABRIC_EXT_EXPORT void klTexCoord3fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glTexCoord3fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord3i(
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r
){
  glTexCoord3i( (GLint)s, (GLint)t, (GLint)r );
}

FABRIC_EXT_EXPORT void klTexCoord3iv(
  const KL::VariableArray<KL::Integer> & v
){
  glTexCoord3iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord3s(
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r
){
  glTexCoord3s( (GLshort)s, (GLshort)t, (GLshort)r );
}

FABRIC_EXT_EXPORT void klTexCoord3sv(
  const KL::VariableArray<KL::Integer> & v
){
  glTexCoord3sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord4d(
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r,
  const KL::Scalar & q
){
  glTexCoord4d( (GLdouble)s, (GLdouble)t, (GLdouble)r, (GLdouble)q );
}

FABRIC_EXT_EXPORT void klTexCoord4dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glTexCoord4dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord4f(
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r,
  const KL::Scalar & q
){
  glTexCoord4f( (GLfloat)s, (GLfloat)t, (GLfloat)r, (GLfloat)q );
}

FABRIC_EXT_EXPORT void klTexCoord4fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glTexCoord4fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord4i(
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r,
  const KL::Integer & q
){
  glTexCoord4i( (GLint)s, (GLint)t, (GLint)r, (GLint)q );
}

FABRIC_EXT_EXPORT void klTexCoord4iv(
  const KL::VariableArray<KL::Integer> & v
){
  glTexCoord4iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoord4s(
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r,
  const KL::Integer & q
){
  glTexCoord4s( (GLshort)s, (GLshort)t, (GLshort)r, (GLshort)q );
}

FABRIC_EXT_EXPORT void klTexCoord4sv(
  const KL::VariableArray<KL::Integer> & v
){
  glTexCoord4sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klTexCoordPointer(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glTexCoordPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klTexEnvf(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glTexEnvf( (GLenum)target, (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klTexEnvfv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glTexEnvfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klTexEnvi(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  glTexEnvi( (GLenum)target, (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klTexEnviv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glTexEnviv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klTexGend(
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glTexGend( (GLenum)coord, (GLenum)pname, (GLdouble)param );
}

FABRIC_EXT_EXPORT void klTexGendv(
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glTexGendv( (GLenum)coord, (GLenum)pname, (const GLdouble*)&params[0] );
}

FABRIC_EXT_EXPORT void klTexGenf(
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glTexGenf( (GLenum)coord, (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klTexGenfv(
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glTexGenfv( (GLenum)coord, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klTexGeni(
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::Integer & param
){
  glTexGeni( (GLenum)coord, (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klTexGeniv(
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glTexGeniv( (GLenum)coord, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klTexImage1D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & internalformat,
  const KL::Size & width,
  const KL::Integer & border,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTexImage1D( (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLint)border, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTexImage2D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Integer & border,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTexImage2D( (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTexParameterf(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glTexParameterf( (GLenum)target, (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klTexParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glTexParameterfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klTexParameteri(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  glTexParameteri( (GLenum)target, (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klTexParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glTexParameteriv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klTexSubImage1D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTexSubImage1D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTexSubImage2D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTexSubImage2D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTranslated(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glTranslated( (GLdouble)x, (GLdouble)y, (GLdouble)z );
}

FABRIC_EXT_EXPORT void klTranslatef(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
}

FABRIC_EXT_EXPORT void klVertex2d(
  const KL::Scalar & x,
  const KL::Scalar & y
){
  glVertex2d( (GLdouble)x, (GLdouble)y );
}

FABRIC_EXT_EXPORT void klVertex2dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glVertex2dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertex2f(
  const KL::Scalar & x,
  const KL::Scalar & y
){
  glVertex2f( (GLfloat)x, (GLfloat)y );
}

FABRIC_EXT_EXPORT void klVertex2fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glVertex2fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertex2i(
  const KL::Integer & x,
  const KL::Integer & y
){
  glVertex2i( (GLint)x, (GLint)y );
}

FABRIC_EXT_EXPORT void klVertex2iv(
  const KL::VariableArray<KL::Integer> & v
){
  glVertex2iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertex2s(
  const KL::Integer & x,
  const KL::Integer & y
){
  glVertex2s( (GLshort)x, (GLshort)y );
}

FABRIC_EXT_EXPORT void klVertex2sv(
  const KL::VariableArray<KL::Integer> & v
){
  glVertex2sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertex3d(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glVertex3d( (GLdouble)x, (GLdouble)y, (GLdouble)z );
}

FABRIC_EXT_EXPORT void klVertex3dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glVertex3dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertex3f(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glVertex3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
}

FABRIC_EXT_EXPORT void klVertex3fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glVertex3fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertex3i(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  glVertex3i( (GLint)x, (GLint)y, (GLint)z );
}

FABRIC_EXT_EXPORT void klVertex3iv(
  const KL::VariableArray<KL::Integer> & v
){
  glVertex3iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertex3s(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  glVertex3s( (GLshort)x, (GLshort)y, (GLshort)z );
}

FABRIC_EXT_EXPORT void klVertex3sv(
  const KL::VariableArray<KL::Integer> & v
){
  glVertex3sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertex4d(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  glVertex4d( (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
}

FABRIC_EXT_EXPORT void klVertex4dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glVertex4dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertex4f(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  glVertex4f( (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
}

FABRIC_EXT_EXPORT void klVertex4fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glVertex4fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertex4i(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  glVertex4i( (GLint)x, (GLint)y, (GLint)z, (GLint)w );
}

FABRIC_EXT_EXPORT void klVertex4iv(
  const KL::VariableArray<KL::Integer> & v
){
  glVertex4iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertex4s(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  glVertex4s( (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
}

FABRIC_EXT_EXPORT void klVertex4sv(
  const KL::VariableArray<KL::Integer> & v
){
  glVertex4sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexPointer(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glVertexPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klViewport(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glViewport( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klCopyTexSubImage3D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & zoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glCopyTexSubImage3D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klDrawRangeElements(
  const KL::Size & mode,
  const KL::Size & start,
  const KL::Size & end,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices
){
  glDrawRangeElements( (GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, indices );
}

FABRIC_EXT_EXPORT void klTexImage3D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & internalFormat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Integer & border,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTexImage3D( (GLenum)target, (GLint)level, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTexSubImage3D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & zoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTexSubImage3D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klActiveTexture(
  const KL::Size & texture
){
  glActiveTexture( (GLenum)texture );
}

FABRIC_EXT_EXPORT void klClientActiveTexture(
  const KL::Size & texture
){
  glClientActiveTexture( (GLenum)texture );
}

FABRIC_EXT_EXPORT void klCompressedTexImage1D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Integer & border,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedTexImage1D( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLint)border, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedTexImage2D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Integer & border,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedTexImage2D( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedTexImage3D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Integer & border,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedTexImage3D( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedTexSubImage1D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedTexSubImage1D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedTexSubImage2D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedTexSubImage2D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedTexSubImage3D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & zoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Size & format,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedTexSubImage3D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klGetCompressedTexImage(
  const KL::Size & target,
  const KL::Integer & lod,
  KL::Data img
){
  glGetCompressedTexImage( (GLenum)target, (GLint)lod, img );
}

FABRIC_EXT_EXPORT void klMultiTexCoord1d(
  const KL::Size & target,
  const KL::Scalar & s
){
  glMultiTexCoord1d( (GLenum)target, (GLdouble)s );
}

FABRIC_EXT_EXPORT void klMultiTexCoord1dv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  glMultiTexCoord1dv( (GLenum)target, (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord1f(
  const KL::Size & target,
  const KL::Scalar & s
){
  glMultiTexCoord1f( (GLenum)target, (GLfloat)s );
}

FABRIC_EXT_EXPORT void klMultiTexCoord1fv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  glMultiTexCoord1fv( (GLenum)target, (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord1i(
  const KL::Size & target,
  const KL::Integer & s
){
  glMultiTexCoord1i( (GLenum)target, (GLint)s );
}

FABRIC_EXT_EXPORT void klMultiTexCoord1iv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  glMultiTexCoord1iv( (GLenum)target, (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord1s(
  const KL::Size & target,
  const KL::Integer & s
){
  glMultiTexCoord1s( (GLenum)target, (GLshort)s );
}

FABRIC_EXT_EXPORT void klMultiTexCoord1sv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  glMultiTexCoord1sv( (GLenum)target, (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord2d(
  const KL::Size & target,
  const KL::Scalar & s,
  const KL::Scalar & t
){
  glMultiTexCoord2d( (GLenum)target, (GLdouble)s, (GLdouble)t );
}

FABRIC_EXT_EXPORT void klMultiTexCoord2dv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  glMultiTexCoord2dv( (GLenum)target, (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord2f(
  const KL::Size & target,
  const KL::Scalar & s,
  const KL::Scalar & t
){
  glMultiTexCoord2f( (GLenum)target, (GLfloat)s, (GLfloat)t );
}

FABRIC_EXT_EXPORT void klMultiTexCoord2fv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  glMultiTexCoord2fv( (GLenum)target, (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord2i(
  const KL::Size & target,
  const KL::Integer & s,
  const KL::Integer & t
){
  glMultiTexCoord2i( (GLenum)target, (GLint)s, (GLint)t );
}

FABRIC_EXT_EXPORT void klMultiTexCoord2iv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  glMultiTexCoord2iv( (GLenum)target, (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord2s(
  const KL::Size & target,
  const KL::Integer & s,
  const KL::Integer & t
){
  glMultiTexCoord2s( (GLenum)target, (GLshort)s, (GLshort)t );
}

FABRIC_EXT_EXPORT void klMultiTexCoord2sv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  glMultiTexCoord2sv( (GLenum)target, (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord3d(
  const KL::Size & target,
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r
){
  glMultiTexCoord3d( (GLenum)target, (GLdouble)s, (GLdouble)t, (GLdouble)r );
}

FABRIC_EXT_EXPORT void klMultiTexCoord3dv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  glMultiTexCoord3dv( (GLenum)target, (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord3f(
  const KL::Size & target,
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r
){
  glMultiTexCoord3f( (GLenum)target, (GLfloat)s, (GLfloat)t, (GLfloat)r );
}

FABRIC_EXT_EXPORT void klMultiTexCoord3fv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  glMultiTexCoord3fv( (GLenum)target, (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord3i(
  const KL::Size & target,
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r
){
  glMultiTexCoord3i( (GLenum)target, (GLint)s, (GLint)t, (GLint)r );
}

FABRIC_EXT_EXPORT void klMultiTexCoord3iv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  glMultiTexCoord3iv( (GLenum)target, (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord3s(
  const KL::Size & target,
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r
){
  glMultiTexCoord3s( (GLenum)target, (GLshort)s, (GLshort)t, (GLshort)r );
}

FABRIC_EXT_EXPORT void klMultiTexCoord3sv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  glMultiTexCoord3sv( (GLenum)target, (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord4d(
  const KL::Size & target,
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r,
  const KL::Scalar & q
){
  glMultiTexCoord4d( (GLenum)target, (GLdouble)s, (GLdouble)t, (GLdouble)r, (GLdouble)q );
}

FABRIC_EXT_EXPORT void klMultiTexCoord4dv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  glMultiTexCoord4dv( (GLenum)target, (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord4f(
  const KL::Size & target,
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r,
  const KL::Scalar & q
){
  glMultiTexCoord4f( (GLenum)target, (GLfloat)s, (GLfloat)t, (GLfloat)r, (GLfloat)q );
}

FABRIC_EXT_EXPORT void klMultiTexCoord4fv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  glMultiTexCoord4fv( (GLenum)target, (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord4i(
  const KL::Size & target,
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r,
  const KL::Integer & q
){
  glMultiTexCoord4i( (GLenum)target, (GLint)s, (GLint)t, (GLint)r, (GLint)q );
}

FABRIC_EXT_EXPORT void klMultiTexCoord4iv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  glMultiTexCoord4iv( (GLenum)target, (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klMultiTexCoord4s(
  const KL::Size & target,
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r,
  const KL::Integer & q
){
  glMultiTexCoord4s( (GLenum)target, (GLshort)s, (GLshort)t, (GLshort)r, (GLshort)q );
}

FABRIC_EXT_EXPORT void klMultiTexCoord4sv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  glMultiTexCoord4sv( (GLenum)target, (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klSampleCoverage(
  const KL::Scalar & value,
  const KL::Boolean & invert
){
  glSampleCoverage( (GLclampf)value, (GLboolean)invert );
}

FABRIC_EXT_EXPORT void klBlendColor(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue,
  const KL::Scalar & alpha
){
  glBlendColor( (GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha );
}

FABRIC_EXT_EXPORT void klBlendEquation(
  const KL::Size & mode
){
  glBlendEquation( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klBlendFuncSeparate(
  const KL::Size & sfactorRGB,
  const KL::Size & dfactorRGB,
  const KL::Size & sfactorAlpha,
  const KL::Size & dfactorAlpha
){
  glBlendFuncSeparate( (GLenum)sfactorRGB, (GLenum)dfactorRGB, (GLenum)sfactorAlpha, (GLenum)dfactorAlpha );
}

FABRIC_EXT_EXPORT void klFogCoordPointer(
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glFogCoordPointer( (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klFogCoordd(
  const KL::Scalar & coord
){
  glFogCoordd( (GLdouble)coord );
}

FABRIC_EXT_EXPORT void klFogCoorddv(
  const KL::VariableArray<KL::Scalar> & coord
){
  glFogCoorddv( (const GLdouble*)&coord[0] );
}

FABRIC_EXT_EXPORT void klFogCoordf(
  const KL::Scalar & coord
){
  glFogCoordf( (GLfloat)coord );
}

FABRIC_EXT_EXPORT void klFogCoordfv(
  const KL::VariableArray<KL::Scalar> & coord
){
  glFogCoordfv( (const GLfloat*)&coord[0] );
}

FABRIC_EXT_EXPORT void klMultiDrawArrays(
  const KL::Size & mode,
  KL::VariableArray<KL::Integer> & first,
  KL::VariableArray<KL::Size> & count,
  const KL::Size & primcount
){
  glMultiDrawArrays( (GLenum)mode, (GLint*)&first[0], (GLsizei*)&count[0], (GLsizei)primcount );
}

FABRIC_EXT_EXPORT void klPointParameterf(
  const KL::Size & pname,
  const KL::Scalar & param
){
  glPointParameterf( (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klPointParameterfv(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glPointParameterfv( (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klPointParameteri(
  const KL::Size & pname,
  const KL::Integer & param
){
  glPointParameteri( (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klPointParameteriv(
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glPointParameteriv( (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3b(
  const KL::Byte & red,
  const KL::Byte & green,
  const KL::Byte & blue
){
  glSecondaryColor3b( (GLbyte)red, (GLbyte)green, (GLbyte)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3bv(
  const KL::VariableArray<KL::Byte> & v
){
  glSecondaryColor3bv( (const GLbyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3d(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue
){
  glSecondaryColor3d( (GLdouble)red, (GLdouble)green, (GLdouble)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3dv(
  const KL::VariableArray<KL::Scalar> & v
){
  glSecondaryColor3dv( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3f(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue
){
  glSecondaryColor3f( (GLfloat)red, (GLfloat)green, (GLfloat)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3fv(
  const KL::VariableArray<KL::Scalar> & v
){
  glSecondaryColor3fv( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3i(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  glSecondaryColor3i( (GLint)red, (GLint)green, (GLint)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3iv(
  const KL::VariableArray<KL::Integer> & v
){
  glSecondaryColor3iv( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3s(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  glSecondaryColor3s( (GLshort)red, (GLshort)green, (GLshort)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3sv(
  const KL::VariableArray<KL::Integer> & v
){
  glSecondaryColor3sv( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3ub(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue
){
  glSecondaryColor3ub( (GLubyte)red, (GLubyte)green, (GLubyte)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3ubv(
  const KL::VariableArray<KL::Size> & v
){
  glSecondaryColor3ubv( (const GLubyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3ui(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue
){
  glSecondaryColor3ui( (GLuint)red, (GLuint)green, (GLuint)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3uiv(
  const KL::VariableArray<KL::Size> & v
){
  glSecondaryColor3uiv( (const GLuint*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3us(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  glSecondaryColor3us( (GLushort)red, (GLushort)green, (GLushort)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3usv(
  const KL::VariableArray<KL::Integer> & v
){
  glSecondaryColor3usv( (const GLushort*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColorPointer(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glSecondaryColorPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klWindowPos2d(
  const KL::Scalar & x,
  const KL::Scalar & y
){
  glWindowPos2d( (GLdouble)x, (GLdouble)y );
}

FABRIC_EXT_EXPORT void klWindowPos2dv(
  const KL::VariableArray<KL::Scalar> & p
){
  glWindowPos2dv( (const GLdouble*)&p[0] );
}

FABRIC_EXT_EXPORT void klWindowPos2f(
  const KL::Scalar & x,
  const KL::Scalar & y
){
  glWindowPos2f( (GLfloat)x, (GLfloat)y );
}

FABRIC_EXT_EXPORT void klWindowPos2fv(
  const KL::VariableArray<KL::Scalar> & p
){
  glWindowPos2fv( (const GLfloat*)&p[0] );
}

FABRIC_EXT_EXPORT void klWindowPos2i(
  const KL::Integer & x,
  const KL::Integer & y
){
  glWindowPos2i( (GLint)x, (GLint)y );
}

FABRIC_EXT_EXPORT void klWindowPos2iv(
  const KL::VariableArray<KL::Integer> & p
){
  glWindowPos2iv( (const GLint*)&p[0] );
}

FABRIC_EXT_EXPORT void klWindowPos2s(
  const KL::Integer & x,
  const KL::Integer & y
){
  glWindowPos2s( (GLshort)x, (GLshort)y );
}

FABRIC_EXT_EXPORT void klWindowPos2sv(
  const KL::VariableArray<KL::Integer> & p
){
  glWindowPos2sv( (const GLshort*)&p[0] );
}

FABRIC_EXT_EXPORT void klWindowPos3d(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glWindowPos3d( (GLdouble)x, (GLdouble)y, (GLdouble)z );
}

FABRIC_EXT_EXPORT void klWindowPos3dv(
  const KL::VariableArray<KL::Scalar> & p
){
  glWindowPos3dv( (const GLdouble*)&p[0] );
}

FABRIC_EXT_EXPORT void klWindowPos3f(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glWindowPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
}

FABRIC_EXT_EXPORT void klWindowPos3fv(
  const KL::VariableArray<KL::Scalar> & p
){
  glWindowPos3fv( (const GLfloat*)&p[0] );
}

FABRIC_EXT_EXPORT void klWindowPos3i(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  glWindowPos3i( (GLint)x, (GLint)y, (GLint)z );
}

FABRIC_EXT_EXPORT void klWindowPos3iv(
  const KL::VariableArray<KL::Integer> & p
){
  glWindowPos3iv( (const GLint*)&p[0] );
}

FABRIC_EXT_EXPORT void klWindowPos3s(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  glWindowPos3s( (GLshort)x, (GLshort)y, (GLshort)z );
}

FABRIC_EXT_EXPORT void klWindowPos3sv(
  const KL::VariableArray<KL::Integer> & p
){
  glWindowPos3sv( (const GLshort*)&p[0] );
}

FABRIC_EXT_EXPORT void klBeginQuery(
  const KL::Size & target,
  const KL::Size & id
){
  glBeginQuery( (GLenum)target, (GLuint)id );
}

FABRIC_EXT_EXPORT void klBindBuffer(
  const KL::Size & target,
  const KL::Size & buffer
){
  glBindBuffer( (GLenum)target, (GLuint)buffer );
}

FABRIC_EXT_EXPORT void klBufferData(
  const KL::Size & target,
  const KL::Size & size,
  KL::Data data,
  const KL::Size & usage
){
  glBufferData( (GLenum)target, (GLsizeiptr)size, data, (GLenum)usage );
}

FABRIC_EXT_EXPORT void klBufferSubData(
  const KL::Size & target,
  const KL::Integer & offset,
  const KL::Size & size,
  KL::Data data
){
  glBufferSubData( (GLenum)target, (GLintptr)offset, (GLsizeiptr)size, data );
}

FABRIC_EXT_EXPORT void klDeleteBuffers(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & buffers
){
  glDeleteBuffers( (GLsizei)n, (const GLuint*)&buffers[0] );
}

FABRIC_EXT_EXPORT void klDeleteQueries(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & ids
){
  glDeleteQueries( (GLsizei)n, (const GLuint*)&ids[0] );
}

FABRIC_EXT_EXPORT void klEndQuery(
  const KL::Size & target
){
  glEndQuery( (GLenum)target );
}

FABRIC_EXT_EXPORT void klGenBuffers(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & buffers
){
  glGenBuffers( (GLsizei)n, (GLuint*)&buffers[0] );
}

FABRIC_EXT_EXPORT void klGenQueries(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & ids
){
  glGenQueries( (GLsizei)n, (GLuint*)&ids[0] );
}

FABRIC_EXT_EXPORT void klGetBufferParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetBufferParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetBufferSubData(
  const KL::Size & target,
  const KL::Integer & offset,
  const KL::Size & size,
  KL::Data data
){
  glGetBufferSubData( (GLenum)target, (GLintptr)offset, (GLsizeiptr)size, data );
}

FABRIC_EXT_EXPORT void klGetQueryObjectiv(
  const KL::Size & id,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetQueryObjectiv( (GLuint)id, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetQueryObjectuiv(
  const KL::Size & id,
  const KL::Size & pname,
  KL::VariableArray<KL::Size> & params
){
  glGetQueryObjectuiv( (GLuint)id, (GLenum)pname, (GLuint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetQueryiv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetQueryiv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT KL::Boolean klIsBuffer(
  const KL::Size & buffer
){
  GLboolean result = glIsBuffer( (GLuint)buffer );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean klIsQuery(
  const KL::Size & id
){
  GLboolean result = glIsQuery( (GLuint)id );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT GLvoid* klMapBuffer(
  const KL::Size & target,
  const KL::Size & access
){
  glMapBuffer( (GLenum)target, (GLenum)access );
}

FABRIC_EXT_EXPORT KL::Boolean klUnmapBuffer(
  const KL::Size & target
){
  GLboolean result = glUnmapBuffer( (GLenum)target );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void klAttachShader(
  const KL::Size & program,
  const KL::Size & shader
){
  glAttachShader( (GLuint)program, (GLuint)shader );
}

FABRIC_EXT_EXPORT void klBindAttribLocation(
  const KL::Size & program,
  const KL::Size & index,
  const KL::String &name
){
  glBindAttribLocation( (GLuint)program, (GLuint)index, (const GLchar*)name.data() );
}

FABRIC_EXT_EXPORT void klBlendEquationSeparate(
  const KL::Size & var0,
  const KL::Size & var1
){
  glBlendEquationSeparate( (GLenum)var0, (GLenum)var1 );
}

FABRIC_EXT_EXPORT void klCompileShader(
  const KL::Size & shader
){
  glCompileShader( (GLuint)shader );
}

FABRIC_EXT_EXPORT KL::Size klCreateProgram()
{
  GLuint result = glCreateProgram();
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size klCreateShader(
  const KL::Size & type
){
  GLuint result = glCreateShader( (GLenum)type );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klDeleteProgram(
  const KL::Size & program
){
  glDeleteProgram( (GLuint)program );
}

FABRIC_EXT_EXPORT void klDeleteShader(
  const KL::Size & shader
){
  glDeleteShader( (GLuint)shader );
}

FABRIC_EXT_EXPORT void klDetachShader(
  const KL::Size & program,
  const KL::Size & shader
){
  glDetachShader( (GLuint)program, (GLuint)shader );
}

FABRIC_EXT_EXPORT void klDisableVertexAttribArray(
  const KL::Size & var0
){
  glDisableVertexAttribArray( (GLuint)var0 );
}

FABRIC_EXT_EXPORT void klDrawBuffers(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & bufs
){
  glDrawBuffers( (GLsizei)n, (const GLenum*)&bufs[0] );
}

FABRIC_EXT_EXPORT void klEnableVertexAttribArray(
  const KL::Size & var0
){
  glEnableVertexAttribArray( (GLuint)var0 );
}

FABRIC_EXT_EXPORT void klGetActiveAttrib(
  const KL::Size & program,
  const KL::Size & index,
  const KL::Size & maxLength,
  KL::VariableArray<KL::Size> & length,
  KL::VariableArray<KL::Integer> & size,
  KL::VariableArray<KL::Size> & type,
  KL::String & name
){
  char * nameStr = new char[1024];
  glGetActiveAttrib( (GLuint)program, (GLuint)index, (GLsizei)maxLength, (GLsizei*)&length[0], (GLint*)&size[0], (GLenum*)&type[0], (GLchar*)nameStr );
  name = KL::String(nameStr);
  delete( nameStr );
}

FABRIC_EXT_EXPORT void klGetActiveUniform(
  const KL::Size & program,
  const KL::Size & index,
  const KL::Size & maxLength,
  KL::VariableArray<KL::Size> & length,
  KL::VariableArray<KL::Integer> & size,
  KL::VariableArray<KL::Size> & type,
  KL::String & name
){
  char * nameStr = new char[1024];
  glGetActiveUniform( (GLuint)program, (GLuint)index, (GLsizei)maxLength, (GLsizei*)&length[0], (GLint*)&size[0], (GLenum*)&type[0], (GLchar*)nameStr );
  name = KL::String(nameStr);
  delete( nameStr );
}

FABRIC_EXT_EXPORT void klGetAttachedShaders(
  const KL::Size & program,
  const KL::Size & maxCount,
  KL::VariableArray<KL::Size> & count,
  KL::VariableArray<KL::Size> & shaders
){
  glGetAttachedShaders( (GLuint)program, (GLsizei)maxCount, (GLsizei*)&count[0], (GLuint*)&shaders[0] );
}

FABRIC_EXT_EXPORT KL::Integer klGetAttribLocation(
  const KL::Size & program,
  const KL::String &name
){
  GLint result = glGetAttribLocation( (GLuint)program, (const GLchar*)name.data() );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT void klGetProgramInfoLog(
  const KL::Size & program,
  const KL::Size & bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::String & infoLog
){
  char * infoLogStr = new char[1024];
  glGetProgramInfoLog( (GLuint)program, (GLsizei)bufSize, (GLsizei*)&length[0], (GLchar*)infoLogStr );
  infoLog = KL::String(infoLogStr);
  delete( infoLogStr );
}

FABRIC_EXT_EXPORT void klGetProgramiv(
  const KL::Size & program,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & param
){
  glGetProgramiv( (GLuint)program, (GLenum)pname, (GLint*)&param[0] );
}

FABRIC_EXT_EXPORT void klGetShaderInfoLog(
  const KL::Size & shader,
  const KL::Size & bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::String & infoLog
){
  char * infoLogStr = new char[1024];
  glGetShaderInfoLog( (GLuint)shader, (GLsizei)bufSize, (GLsizei*)&length[0], (GLchar*)infoLogStr );
  infoLog = KL::String(infoLogStr);
  delete( infoLogStr );
}

FABRIC_EXT_EXPORT void klGetShaderSource(
  const KL::Integer & obj,
  const KL::Size & maxLength,
  KL::VariableArray<KL::Size> & length,
  KL::String & source
){
  char * sourceStr = new char[1024];
  glGetShaderSource( (GLint)obj, (GLsizei)maxLength, (GLsizei*)&length[0], (GLchar*)sourceStr );
  source = KL::String(sourceStr);
  delete( sourceStr );
}

FABRIC_EXT_EXPORT void klGetShaderiv(
  const KL::Size & shader,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & param
){
  glGetShaderiv( (GLuint)shader, (GLenum)pname, (GLint*)&param[0] );
}

FABRIC_EXT_EXPORT KL::Integer klGetUniformLocation(
  const KL::Size & program,
  const KL::String &name
){
  GLint result = glGetUniformLocation( (GLuint)program, (const GLchar*)name.data() );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT void klGetUniformfv(
  const KL::Size & program,
  const KL::Integer & location,
  KL::VariableArray<KL::Scalar> & params
){
  glGetUniformfv( (GLuint)program, (GLint)location, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetUniformiv(
  const KL::Size & program,
  const KL::Integer & location,
  KL::VariableArray<KL::Integer> & params
){
  glGetUniformiv( (GLuint)program, (GLint)location, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetVertexAttribPointerv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::Data var2
){
  glGetVertexAttribPointerv( (GLuint)var0, (GLenum)var1, var2 );
}

FABRIC_EXT_EXPORT void klGetVertexAttribdv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Scalar> & var2
){
  glGetVertexAttribdv( (GLuint)var0, (GLenum)var1, (GLdouble*)&var2[0] );
}

FABRIC_EXT_EXPORT void klGetVertexAttribfv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Scalar> & var2
){
  glGetVertexAttribfv( (GLuint)var0, (GLenum)var1, (GLfloat*)&var2[0] );
}

FABRIC_EXT_EXPORT void klGetVertexAttribiv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Integer> & var2
){
  glGetVertexAttribiv( (GLuint)var0, (GLenum)var1, (GLint*)&var2[0] );
}

FABRIC_EXT_EXPORT KL::Boolean klIsProgram(
  const KL::Size & program
){
  GLboolean result = glIsProgram( (GLuint)program );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean klIsShader(
  const KL::Size & shader
){
  GLboolean result = glIsShader( (GLuint)shader );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void klLinkProgram(
  const KL::Size & program
){
  glLinkProgram( (GLuint)program );
}

FABRIC_EXT_EXPORT void klShaderSource(
  const KL::Size & shader,
  const KL::Size & count,
  const KL::VariableArray<KL::String> &strings,
  const KL::VariableArray<KL::Integer> & lengths
){
  glShaderSource( (GLuint)shader, (GLsizei)count, (const GLchar**)&strings[0], (const GLint*)&lengths[0] );
}

FABRIC_EXT_EXPORT void klStencilFuncSeparate(
  const KL::Size & frontfunc,
  const KL::Size & backfunc,
  const KL::Integer & ref,
  const KL::Size & mask
){
  glStencilFuncSeparate( (GLenum)frontfunc, (GLenum)backfunc, (GLint)ref, (GLuint)mask );
}

FABRIC_EXT_EXPORT void klStencilMaskSeparate(
  const KL::Size & var0,
  const KL::Size & var1
){
  glStencilMaskSeparate( (GLenum)var0, (GLuint)var1 );
}

FABRIC_EXT_EXPORT void klStencilOpSeparate(
  const KL::Size & face,
  const KL::Size & sfail,
  const KL::Size & dpfail,
  const KL::Size & dppass
){
  glStencilOpSeparate( (GLenum)face, (GLenum)sfail, (GLenum)dpfail, (GLenum)dppass );
}

FABRIC_EXT_EXPORT void klUniform1f(
  const KL::Integer & location,
  const KL::Scalar & v0
){
  glUniform1f( (GLint)location, (GLfloat)v0 );
}

FABRIC_EXT_EXPORT void klUniform1fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniform1fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniform1i(
  const KL::Integer & location,
  const KL::Integer & v0
){
  glUniform1i( (GLint)location, (GLint)v0 );
}

FABRIC_EXT_EXPORT void klUniform1iv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  glUniform1iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniform2f(
  const KL::Integer & location,
  const KL::Scalar & v0,
  const KL::Scalar & v1
){
  glUniform2f( (GLint)location, (GLfloat)v0, (GLfloat)v1 );
}

FABRIC_EXT_EXPORT void klUniform2fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniform2fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniform2i(
  const KL::Integer & location,
  const KL::Integer & v0,
  const KL::Integer & v1
){
  glUniform2i( (GLint)location, (GLint)v0, (GLint)v1 );
}

FABRIC_EXT_EXPORT void klUniform2iv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  glUniform2iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniform3f(
  const KL::Integer & location,
  const KL::Scalar & v0,
  const KL::Scalar & v1,
  const KL::Scalar & v2
){
  glUniform3f( (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2 );
}

FABRIC_EXT_EXPORT void klUniform3fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniform3fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniform3i(
  const KL::Integer & location,
  const KL::Integer & v0,
  const KL::Integer & v1,
  const KL::Integer & v2
){
  glUniform3i( (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2 );
}

FABRIC_EXT_EXPORT void klUniform3iv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  glUniform3iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniform4f(
  const KL::Integer & location,
  const KL::Scalar & v0,
  const KL::Scalar & v1,
  const KL::Scalar & v2,
  const KL::Scalar & v3
){
  glUniform4f( (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2, (GLfloat)v3 );
}

FABRIC_EXT_EXPORT void klUniform4fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniform4fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniform4i(
  const KL::Integer & location,
  const KL::Integer & v0,
  const KL::Integer & v1,
  const KL::Integer & v2,
  const KL::Integer & v3
){
  glUniform4i( (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2, (GLint)v3 );
}

FABRIC_EXT_EXPORT void klUniform4iv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  glUniform4iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniformMatrix2fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniformMatrix2fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniformMatrix3fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniformMatrix3fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniformMatrix4fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniformMatrix4fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klUseProgram(
  const KL::Size & program
){
  glUseProgram( (GLuint)program );
}

FABRIC_EXT_EXPORT void klValidateProgram(
  const KL::Size & program
){
  glValidateProgram( (GLuint)program );
}

FABRIC_EXT_EXPORT void klVertexAttrib1d(
  const KL::Size & index,
  const KL::Scalar & x
){
  glVertexAttrib1d( (GLuint)index, (GLdouble)x );
}

FABRIC_EXT_EXPORT void klVertexAttrib1dv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  glVertexAttrib1dv( (GLuint)index, (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib1f(
  const KL::Size & index,
  const KL::Scalar & x
){
  glVertexAttrib1f( (GLuint)index, (GLfloat)x );
}

FABRIC_EXT_EXPORT void klVertexAttrib1fv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  glVertexAttrib1fv( (GLuint)index, (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib1s(
  const KL::Size & index,
  const KL::Integer & x
){
  glVertexAttrib1s( (GLuint)index, (GLshort)x );
}

FABRIC_EXT_EXPORT void klVertexAttrib1sv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttrib1sv( (GLuint)index, (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib2d(
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y
){
  glVertexAttrib2d( (GLuint)index, (GLdouble)x, (GLdouble)y );
}

FABRIC_EXT_EXPORT void klVertexAttrib2dv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  glVertexAttrib2dv( (GLuint)index, (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib2f(
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y
){
  glVertexAttrib2f( (GLuint)index, (GLfloat)x, (GLfloat)y );
}

FABRIC_EXT_EXPORT void klVertexAttrib2fv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  glVertexAttrib2fv( (GLuint)index, (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib2s(
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y
){
  glVertexAttrib2s( (GLuint)index, (GLshort)x, (GLshort)y );
}

FABRIC_EXT_EXPORT void klVertexAttrib2sv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttrib2sv( (GLuint)index, (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib3d(
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glVertexAttrib3d( (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z );
}

FABRIC_EXT_EXPORT void klVertexAttrib3dv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  glVertexAttrib3dv( (GLuint)index, (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib3f(
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glVertexAttrib3f( (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z );
}

FABRIC_EXT_EXPORT void klVertexAttrib3fv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  glVertexAttrib3fv( (GLuint)index, (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib3s(
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  glVertexAttrib3s( (GLuint)index, (GLshort)x, (GLshort)y, (GLshort)z );
}

FABRIC_EXT_EXPORT void klVertexAttrib3sv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttrib3sv( (GLuint)index, (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4Nbv(
  const KL::Size & index,
  const KL::VariableArray<KL::Byte> & v
){
  glVertexAttrib4Nbv( (GLuint)index, (const GLbyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4Niv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttrib4Niv( (GLuint)index, (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4Nsv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttrib4Nsv( (GLuint)index, (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4Nub(
  const KL::Size & index,
  const KL::Size & x,
  const KL::Size & y,
  const KL::Size & z,
  const KL::Size & w
){
  glVertexAttrib4Nub( (GLuint)index, (GLubyte)x, (GLubyte)y, (GLubyte)z, (GLubyte)w );
}

FABRIC_EXT_EXPORT void klVertexAttrib4Nubv(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  glVertexAttrib4Nubv( (GLuint)index, (const GLubyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4Nuiv(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  glVertexAttrib4Nuiv( (GLuint)index, (const GLuint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4Nusv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttrib4Nusv( (GLuint)index, (const GLushort*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4bv(
  const KL::Size & index,
  const KL::VariableArray<KL::Byte> & v
){
  glVertexAttrib4bv( (GLuint)index, (const GLbyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4d(
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  glVertexAttrib4d( (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
}

FABRIC_EXT_EXPORT void klVertexAttrib4dv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  glVertexAttrib4dv( (GLuint)index, (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4f(
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  glVertexAttrib4f( (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
}

FABRIC_EXT_EXPORT void klVertexAttrib4fv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  glVertexAttrib4fv( (GLuint)index, (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4iv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttrib4iv( (GLuint)index, (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4s(
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  glVertexAttrib4s( (GLuint)index, (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
}

FABRIC_EXT_EXPORT void klVertexAttrib4sv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttrib4sv( (GLuint)index, (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4ubv(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  glVertexAttrib4ubv( (GLuint)index, (const GLubyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4uiv(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  glVertexAttrib4uiv( (GLuint)index, (const GLuint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttrib4usv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttrib4usv( (GLuint)index, (const GLushort*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribPointer(
  const KL::Size & index,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Boolean & normalized,
  const KL::Size & stride,
  KL::Data pointer
){
  glVertexAttribPointer( (GLuint)index, (GLint)size, (GLenum)type, (GLboolean)normalized, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klUniformMatrix2x3fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniformMatrix2x3fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniformMatrix2x4fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniformMatrix2x4fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniformMatrix3x2fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniformMatrix3x2fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniformMatrix3x4fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniformMatrix3x4fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniformMatrix4x2fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniformMatrix4x2fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniformMatrix4x3fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glUniformMatrix4x3fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klBeginConditionalRender(
  const KL::Size & var0,
  const KL::Size & var1
){
  glBeginConditionalRender( (GLuint)var0, (GLenum)var1 );
}

FABRIC_EXT_EXPORT void klBeginTransformFeedback(
  const KL::Size & var0
){
  glBeginTransformFeedback( (GLenum)var0 );
}

FABRIC_EXT_EXPORT void klBindFragDataLocation(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::String &var2
){
  glBindFragDataLocation( (GLuint)var0, (GLuint)var1, (const GLchar*)var2.data() );
}

FABRIC_EXT_EXPORT void klClampColor(
  const KL::Size & var0,
  const KL::Size & var1
){
  glClampColor( (GLenum)var0, (GLenum)var1 );
}

FABRIC_EXT_EXPORT void klClearBufferfi(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::Scalar & var2,
  const KL::Integer & var3
){
  glClearBufferfi( (GLenum)var0, (GLint)var1, (GLfloat)var2, (GLint)var3 );
}

FABRIC_EXT_EXPORT void klClearBufferfv(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::VariableArray<KL::Scalar> & var2
){
  glClearBufferfv( (GLenum)var0, (GLint)var1, (const GLfloat*)&var2[0] );
}

FABRIC_EXT_EXPORT void klClearBufferiv(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::VariableArray<KL::Integer> & var2
){
  glClearBufferiv( (GLenum)var0, (GLint)var1, (const GLint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klClearBufferuiv(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::VariableArray<KL::Size> & var2
){
  glClearBufferuiv( (GLenum)var0, (GLint)var1, (const GLuint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klColorMaski(
  const KL::Size & var0,
  const KL::Boolean & var1,
  const KL::Boolean & var2,
  const KL::Boolean & var3,
  const KL::Boolean & var4
){
  glColorMaski( (GLuint)var0, (GLboolean)var1, (GLboolean)var2, (GLboolean)var3, (GLboolean)var4 );
}

FABRIC_EXT_EXPORT void klDisablei(
  const KL::Size & var0,
  const KL::Size & var1
){
  glDisablei( (GLenum)var0, (GLuint)var1 );
}

FABRIC_EXT_EXPORT void klEnablei(
  const KL::Size & var0,
  const KL::Size & var1
){
  glEnablei( (GLenum)var0, (GLuint)var1 );
}

FABRIC_EXT_EXPORT void klEndConditionalRender()
{
  glEndConditionalRender();
}

FABRIC_EXT_EXPORT void klEndTransformFeedback()
{
  glEndTransformFeedback();
}

FABRIC_EXT_EXPORT void klGetBooleani_v(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Boolean> & var2
){
  glGetBooleani_v( (GLenum)var0, (GLuint)var1, (GLboolean*)&var2[0] );
}

FABRIC_EXT_EXPORT KL::Integer klGetFragDataLocation(
  const KL::Size & var0,
  const KL::String &var1
){
  GLint result = glGetFragDataLocation( (GLuint)var0, (const GLchar*)var1.data() );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT void klGetTexParameterIiv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Integer> & var2
){
  glGetTexParameterIiv( (GLenum)var0, (GLenum)var1, (GLint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klGetTexParameterIuiv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Size> & var2
){
  glGetTexParameterIuiv( (GLenum)var0, (GLenum)var1, (GLuint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klGetTransformFeedbackVarying(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Integer> & var2
){
  glGetTransformFeedbackVarying( (GLuint)var0, (GLuint)var1, (GLint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klGetUniformuiv(
  const KL::Size & var0,
  const KL::Integer & var1,
  KL::VariableArray<KL::Size> & var2
){
  glGetUniformuiv( (GLuint)var0, (GLint)var1, (GLuint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klGetVertexAttribIiv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Integer> & var2
){
  glGetVertexAttribIiv( (GLuint)var0, (GLenum)var1, (GLint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klGetVertexAttribIuiv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Size> & var2
){
  glGetVertexAttribIuiv( (GLuint)var0, (GLenum)var1, (GLuint*)&var2[0] );
}

FABRIC_EXT_EXPORT KL::Boolean klIsEnabledi(
  const KL::Size & var0,
  const KL::Size & var1
){
  GLboolean result = glIsEnabledi( (GLenum)var0, (GLuint)var1 );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void klTexParameterIiv(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::Integer> & var2
){
  glTexParameterIiv( (GLenum)var0, (GLenum)var1, (const GLint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klTexParameterIuiv(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::Size> & var2
){
  glTexParameterIuiv( (GLenum)var0, (GLenum)var1, (const GLuint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klTransformFeedbackVaryings(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::String> &var2,
  const KL::Size & var3
){
  glTransformFeedbackVaryings( (GLuint)var0, (GLsizei)var1, (const GLchar**)&var2[0], (GLenum)var3 );
}

FABRIC_EXT_EXPORT void klUniform1ui(
  const KL::Integer & var0,
  const KL::Size & var1
){
  glUniform1ui( (GLint)var0, (GLuint)var1 );
}

FABRIC_EXT_EXPORT void klUniform1uiv(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::Size> & var2
){
  glUniform1uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klUniform2ui(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::Size & var2
){
  glUniform2ui( (GLint)var0, (GLuint)var1, (GLuint)var2 );
}

FABRIC_EXT_EXPORT void klUniform2uiv(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::Size> & var2
){
  glUniform2uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klUniform3ui(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::Size & var2,
  const KL::Size & var3
){
  glUniform3ui( (GLint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3 );
}

FABRIC_EXT_EXPORT void klUniform3uiv(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::Size> & var2
){
  glUniform3uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klUniform4ui(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::Size & var2,
  const KL::Size & var3,
  const KL::Size & var4
){
  glUniform4ui( (GLint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3, (GLuint)var4 );
}

FABRIC_EXT_EXPORT void klUniform4uiv(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::Size> & var2
){
  glUniform4uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI1i(
  const KL::Size & var0,
  const KL::Integer & var1
){
  glVertexAttribI1i( (GLuint)var0, (GLint)var1 );
}

FABRIC_EXT_EXPORT void klVertexAttribI1iv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Integer> & var1
){
  glVertexAttribI1iv( (GLuint)var0, (const GLint*)&var1[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI1ui(
  const KL::Size & var0,
  const KL::Size & var1
){
  glVertexAttribI1ui( (GLuint)var0, (GLuint)var1 );
}

FABRIC_EXT_EXPORT void klVertexAttribI1uiv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Size> & var1
){
  glVertexAttribI1uiv( (GLuint)var0, (const GLuint*)&var1[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI2i(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::Integer & var2
){
  glVertexAttribI2i( (GLuint)var0, (GLint)var1, (GLint)var2 );
}

FABRIC_EXT_EXPORT void klVertexAttribI2iv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Integer> & var1
){
  glVertexAttribI2iv( (GLuint)var0, (const GLint*)&var1[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI2ui(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::Size & var2
){
  glVertexAttribI2ui( (GLuint)var0, (GLuint)var1, (GLuint)var2 );
}

FABRIC_EXT_EXPORT void klVertexAttribI2uiv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Size> & var1
){
  glVertexAttribI2uiv( (GLuint)var0, (const GLuint*)&var1[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI3i(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::Integer & var2,
  const KL::Integer & var3
){
  glVertexAttribI3i( (GLuint)var0, (GLint)var1, (GLint)var2, (GLint)var3 );
}

FABRIC_EXT_EXPORT void klVertexAttribI3iv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Integer> & var1
){
  glVertexAttribI3iv( (GLuint)var0, (const GLint*)&var1[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI3ui(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::Size & var2,
  const KL::Size & var3
){
  glVertexAttribI3ui( (GLuint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3 );
}

FABRIC_EXT_EXPORT void klVertexAttribI3uiv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Size> & var1
){
  glVertexAttribI3uiv( (GLuint)var0, (const GLuint*)&var1[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI4bv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Byte> & var1
){
  glVertexAttribI4bv( (GLuint)var0, (const GLbyte*)&var1[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI4i(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::Integer & var2,
  const KL::Integer & var3,
  const KL::Integer & var4
){
  glVertexAttribI4i( (GLuint)var0, (GLint)var1, (GLint)var2, (GLint)var3, (GLint)var4 );
}

FABRIC_EXT_EXPORT void klVertexAttribI4iv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Integer> & var1
){
  glVertexAttribI4iv( (GLuint)var0, (const GLint*)&var1[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI4sv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Integer> & var1
){
  glVertexAttribI4sv( (GLuint)var0, (const GLshort*)&var1[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI4ubv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Size> & var1
){
  glVertexAttribI4ubv( (GLuint)var0, (const GLubyte*)&var1[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI4ui(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::Size & var2,
  const KL::Size & var3,
  const KL::Size & var4
){
  glVertexAttribI4ui( (GLuint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3, (GLuint)var4 );
}

FABRIC_EXT_EXPORT void klVertexAttribI4uiv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Size> & var1
){
  glVertexAttribI4uiv( (GLuint)var0, (const GLuint*)&var1[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI4usv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Integer> & var1
){
  glVertexAttribI4usv( (GLuint)var0, (const GLushort*)&var1[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribIPointer(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::Size & var2,
  const KL::Size & var3,
  KL::Data var4
){
  glVertexAttribIPointer( (GLuint)var0, (GLint)var1, (GLenum)var2, (GLsizei)var3, var4 );
}

FABRIC_EXT_EXPORT void klDrawArraysInstanced(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::Size & var2,
  const KL::Size & var3
){
  glDrawArraysInstanced( (GLenum)var0, (GLint)var1, (GLsizei)var2, (GLsizei)var3 );
}

FABRIC_EXT_EXPORT void klDrawElementsInstanced(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::Size & var2,
  KL::Data var3,
  const KL::Size & var4
){
  glDrawElementsInstanced( (GLenum)var0, (GLsizei)var1, (GLenum)var2, var3, (GLsizei)var4 );
}

FABRIC_EXT_EXPORT void klPrimitiveRestartIndex(
  const KL::Size & var0
){
  glPrimitiveRestartIndex( (GLuint)var0 );
}

FABRIC_EXT_EXPORT void klTexBuffer(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::Size & var2
){
  glTexBuffer( (GLenum)var0, (GLenum)var1, (GLuint)var2 );
}

FABRIC_EXT_EXPORT void klFramebufferTexture(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::Size & var2,
  const KL::Integer & var3
){
  glFramebufferTexture( (GLenum)var0, (GLenum)var1, (GLuint)var2, (GLint)var3 );
}

FABRIC_EXT_EXPORT void klTbufferMask3DFX(
  const KL::Size & mask
){
  glTbufferMask3DFX( (GLuint)mask );
}

FABRIC_EXT_EXPORT void klCopyBufferSubData(
  const KL::Size & readtarget,
  const KL::Size & writetarget,
  const KL::Integer & readoffset,
  const KL::Integer & writeoffset,
  const KL::Size & size
){
  glCopyBufferSubData( (GLenum)readtarget, (GLenum)writetarget, (GLintptr)readoffset, (GLintptr)writeoffset, (GLsizeiptr)size );
}

FABRIC_EXT_EXPORT void klDrawElementsBaseVertex(
  const KL::Size & mode,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices,
  const KL::Integer & basevertex
){
  glDrawElementsBaseVertex( (GLenum)mode, (GLsizei)count, (GLenum)type, indices, (GLint)basevertex );
}

FABRIC_EXT_EXPORT void klDrawElementsInstancedBaseVertex(
  const KL::Size & mode,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices,
  const KL::Size & primcount,
  const KL::Integer & basevertex
){
  glDrawElementsInstancedBaseVertex( (GLenum)mode, (GLsizei)count, (GLenum)type, indices, (GLsizei)primcount, (GLint)basevertex );
}

FABRIC_EXT_EXPORT void klDrawRangeElementsBaseVertex(
  const KL::Size & mode,
  const KL::Size & start,
  const KL::Size & end,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices,
  const KL::Integer & basevertex
){
  glDrawRangeElementsBaseVertex( (GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, indices, (GLint)basevertex );
}

FABRIC_EXT_EXPORT void klBindFramebuffer(
  const KL::Size & target,
  const KL::Size & framebuffer
){
  glBindFramebuffer( (GLenum)target, (GLuint)framebuffer );
}

FABRIC_EXT_EXPORT void klBindRenderbuffer(
  const KL::Size & target,
  const KL::Size & renderbuffer
){
  glBindRenderbuffer( (GLenum)target, (GLuint)renderbuffer );
}

FABRIC_EXT_EXPORT void klBlitFramebuffer(
  const KL::Integer & srcX0,
  const KL::Integer & srcY0,
  const KL::Integer & srcX1,
  const KL::Integer & srcY1,
  const KL::Integer & dstX0,
  const KL::Integer & dstY0,
  const KL::Integer & dstX1,
  const KL::Integer & dstY1,
  const KL::Integer & mask,
  const KL::Size & filter
){
  glBlitFramebuffer( (GLint)srcX0, (GLint)srcY0, (GLint)srcX1, (GLint)srcY1, (GLint)dstX0, (GLint)dstY0, (GLint)dstX1, (GLint)dstY1, (GLbitfield)mask, (GLenum)filter );
}

FABRIC_EXT_EXPORT KL::Size klCheckFramebufferStatus(
  const KL::Size & target
){
  GLenum result = glCheckFramebufferStatus( (GLenum)target );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klDeleteFramebuffers(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & framebuffers
){
  glDeleteFramebuffers( (GLsizei)n, (const GLuint*)&framebuffers[0] );
}

FABRIC_EXT_EXPORT void klDeleteRenderbuffers(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & renderbuffers
){
  glDeleteRenderbuffers( (GLsizei)n, (const GLuint*)&renderbuffers[0] );
}

FABRIC_EXT_EXPORT void klFramebufferRenderbuffer(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & renderbuffertarget,
  const KL::Size & renderbuffer
){
  glFramebufferRenderbuffer( (GLenum)target, (GLenum)attachment, (GLenum)renderbuffertarget, (GLuint)renderbuffer );
}

FABRIC_EXT_EXPORT void klFramebufferTexture1D(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level
){
  glFramebufferTexture1D( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
}

FABRIC_EXT_EXPORT void klFramebufferTexture2D(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level
){
  glFramebufferTexture2D( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
}

FABRIC_EXT_EXPORT void klFramebufferTexture3D(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Integer & layer
){
  glFramebufferTexture3D( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level, (GLint)layer );
}

FABRIC_EXT_EXPORT void klFramebufferTextureLayer(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Integer & layer
){
  glFramebufferTextureLayer( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer );
}

FABRIC_EXT_EXPORT void klGenFramebuffers(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & framebuffers
){
  glGenFramebuffers( (GLsizei)n, (GLuint*)&framebuffers[0] );
}

FABRIC_EXT_EXPORT void klGenRenderbuffers(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & renderbuffers
){
  glGenRenderbuffers( (GLsizei)n, (GLuint*)&renderbuffers[0] );
}

FABRIC_EXT_EXPORT void klGenerateMipmap(
  const KL::Size & target
){
  glGenerateMipmap( (GLenum)target );
}

FABRIC_EXT_EXPORT void klGetFramebufferAttachmentParameteriv(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetFramebufferAttachmentParameteriv( (GLenum)target, (GLenum)attachment, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetRenderbufferParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetRenderbufferParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT KL::Boolean klIsFramebuffer(
  const KL::Size & framebuffer
){
  GLboolean result = glIsFramebuffer( (GLuint)framebuffer );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean klIsRenderbuffer(
  const KL::Size & renderbuffer
){
  GLboolean result = glIsRenderbuffer( (GLuint)renderbuffer );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void klRenderbufferStorage(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  glRenderbufferStorage( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klRenderbufferStorageMultisample(
  const KL::Size & target,
  const KL::Size & samples,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  glRenderbufferStorageMultisample( (GLenum)target, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klColorSubTable(
  const KL::Size & target,
  const KL::Size & start,
  const KL::Size & count,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data data
){
  glColorSubTable( (GLenum)target, (GLsizei)start, (GLsizei)count, (GLenum)format, (GLenum)type, data );
}

FABRIC_EXT_EXPORT void klColorTable(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data table
){
  glColorTable( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLenum)format, (GLenum)type, table );
}

FABRIC_EXT_EXPORT void klColorTableParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glColorTableParameterfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klColorTableParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glColorTableParameteriv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klConvolutionFilter1D(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data image
){
  glConvolutionFilter1D( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLenum)format, (GLenum)type, image );
}

FABRIC_EXT_EXPORT void klConvolutionFilter2D(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data image
){
  glConvolutionFilter2D( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, image );
}

FABRIC_EXT_EXPORT void klConvolutionParameterf(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & params
){
  glConvolutionParameterf( (GLenum)target, (GLenum)pname, (GLfloat)params );
}

FABRIC_EXT_EXPORT void klConvolutionParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glConvolutionParameterfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klConvolutionParameteri(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & params
){
  glConvolutionParameteri( (GLenum)target, (GLenum)pname, (GLint)params );
}

FABRIC_EXT_EXPORT void klConvolutionParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glConvolutionParameteriv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klCopyColorSubTable(
  const KL::Size & target,
  const KL::Size & start,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  glCopyColorSubTable( (GLenum)target, (GLsizei)start, (GLint)x, (GLint)y, (GLsizei)width );
}

FABRIC_EXT_EXPORT void klCopyColorTable(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  glCopyColorTable( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width );
}

FABRIC_EXT_EXPORT void klCopyConvolutionFilter1D(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  glCopyConvolutionFilter1D( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width );
}

FABRIC_EXT_EXPORT void klCopyConvolutionFilter2D(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glCopyConvolutionFilter2D( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klGetColorTable(
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data table
){
  glGetColorTable( (GLenum)target, (GLenum)format, (GLenum)type, table );
}

FABRIC_EXT_EXPORT void klGetColorTableParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetColorTableParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetColorTableParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetColorTableParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetConvolutionFilter(
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data image
){
  glGetConvolutionFilter( (GLenum)target, (GLenum)format, (GLenum)type, image );
}

FABRIC_EXT_EXPORT void klGetConvolutionParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetConvolutionParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetConvolutionParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetConvolutionParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetHistogram(
  const KL::Size & target,
  const KL::Boolean & reset,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data values
){
  glGetHistogram( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, values );
}

FABRIC_EXT_EXPORT void klGetHistogramParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetHistogramParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetHistogramParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetHistogramParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMinmax(
  const KL::Size & target,
  const KL::Boolean & reset,
  const KL::Size & format,
  const KL::Size & types,
  KL::Data values
){
  glGetMinmax( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)types, values );
}

FABRIC_EXT_EXPORT void klGetMinmaxParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetMinmaxParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMinmaxParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetMinmaxParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetSeparableFilter(
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data row,
  KL::Data column,
  KL::Data span
){
  glGetSeparableFilter( (GLenum)target, (GLenum)format, (GLenum)type, row, column, span );
}

FABRIC_EXT_EXPORT void klHistogram(
  const KL::Size & target,
  const KL::Size & width,
  const KL::Size & internalformat,
  const KL::Boolean & sink
){
  glHistogram( (GLenum)target, (GLsizei)width, (GLenum)internalformat, (GLboolean)sink );
}

FABRIC_EXT_EXPORT void klMinmax(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Boolean & sink
){
  glMinmax( (GLenum)target, (GLenum)internalformat, (GLboolean)sink );
}

FABRIC_EXT_EXPORT void klResetHistogram(
  const KL::Size & target
){
  glResetHistogram( (GLenum)target );
}

FABRIC_EXT_EXPORT void klResetMinmax(
  const KL::Size & target
){
  glResetMinmax( (GLenum)target );
}

FABRIC_EXT_EXPORT void klSeparableFilter2D(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data row,
  KL::Data column
){
  glSeparableFilter2D( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, row, column );
}

FABRIC_EXT_EXPORT void klFlushMappedBufferRange(
  const KL::Size & target,
  const KL::Integer & offset,
  const KL::Size & length
){
  glFlushMappedBufferRange( (GLenum)target, (GLintptr)offset, (GLsizeiptr)length );
}

FABRIC_EXT_EXPORT GLvoid klMapBufferRange(
  const KL::Size & target,
  const KL::Integer & offset,
  const KL::Size & length,
  const KL::Integer & access
){
  glMapBufferRange( (GLenum)target, (GLintptr)offset, (GLsizeiptr)length, (GLbitfield)access );
}

FABRIC_EXT_EXPORT void klProvokingVertex(
  const KL::Size & mode
){
  glProvokingVertex( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klGetMultisamplefv(
  const KL::Size & pname,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & val
){
  glGetMultisamplefv( (GLenum)pname, (GLuint)index, (GLfloat*)&val[0] );
}

FABRIC_EXT_EXPORT void klSampleMaski(
  const KL::Size & index,
  const KL::Integer & mask
){
  glSampleMaski( (GLuint)index, (GLbitfield)mask );
}

FABRIC_EXT_EXPORT void klTexImage2DMultisample(
  const KL::Size & target,
  const KL::Size & samples,
  const KL::Integer & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Boolean & fixedsamplelocations
){
  glTexImage2DMultisample( (GLenum)target, (GLsizei)samples, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLboolean)fixedsamplelocations );
}

FABRIC_EXT_EXPORT void klTexImage3DMultisample(
  const KL::Size & target,
  const KL::Size & samples,
  const KL::Integer & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Boolean & fixedsamplelocations
){
  glTexImage3DMultisample( (GLenum)target, (GLsizei)samples, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLboolean)fixedsamplelocations );
}

FABRIC_EXT_EXPORT void klBindBufferBase(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & buffer
){
  glBindBufferBase( (GLenum)target, (GLuint)index, (GLuint)buffer );
}

FABRIC_EXT_EXPORT void klBindBufferRange(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & buffer,
  const KL::Integer & offset,
  const KL::Size & size
){
  glBindBufferRange( (GLenum)target, (GLuint)index, (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size );
}

FABRIC_EXT_EXPORT void klGetActiveUniformBlockName(
  const KL::Size & program,
  const KL::Size & uniformBlockIndex,
  const KL::Size & bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::String & uniformBlockName
){
  char * uniformBlockNameStr = new char[1024];
  glGetActiveUniformBlockName( (GLuint)program, (GLuint)uniformBlockIndex, (GLsizei)bufSize, (GLsizei*)&length[0], (char*)uniformBlockNameStr );
  uniformBlockName = KL::String(uniformBlockNameStr);
  delete( uniformBlockNameStr );
}

FABRIC_EXT_EXPORT void klGetActiveUniformBlockiv(
  const KL::Size & program,
  const KL::Size & uniformBlockIndex,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetActiveUniformBlockiv( (GLuint)program, (GLuint)uniformBlockIndex, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetActiveUniformName(
  const KL::Size & program,
  const KL::Size & uniformIndex,
  const KL::Size & bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::String & uniformName
){
  char * uniformNameStr = new char[1024];
  glGetActiveUniformName( (GLuint)program, (GLuint)uniformIndex, (GLsizei)bufSize, (GLsizei*)&length[0], (char*)uniformNameStr );
  uniformName = KL::String(uniformNameStr);
  delete( uniformNameStr );
}

FABRIC_EXT_EXPORT void klGetActiveUniformsiv(
  const KL::Size & program,
  const KL::Size & uniformCount,
  const KL::VariableArray<KL::Size> & uniformIndices,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetActiveUniformsiv( (GLuint)program, (GLsizei)uniformCount, (const GLuint*)&uniformIndices[0], (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetIntegeri_v(
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Integer> & data
){
  glGetIntegeri_v( (GLenum)target, (GLuint)index, (GLint*)&data[0] );
}

FABRIC_EXT_EXPORT KL::Size klGetUniformBlockIndex(
  const KL::Size & program,
  const KL::String &uniformBlockName
){
  GLuint result = glGetUniformBlockIndex( (GLuint)program, (const char*)uniformBlockName.data() );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klGetUniformIndices(
  const KL::Size & program,
  const KL::Size & uniformCount,
  const KL::VariableArray<KL::String> &uniformNames,
  KL::VariableArray<KL::Size> & uniformIndices
){
  glGetUniformIndices( (GLuint)program, (GLsizei)uniformCount, (const char**)&uniformNames[0], (GLuint*)&uniformIndices[0] );
}

FABRIC_EXT_EXPORT void klUniformBlockBinding(
  const KL::Size & program,
  const KL::Size & uniformBlockIndex,
  const KL::Size & uniformBlockBinding
){
  glUniformBlockBinding( (GLuint)program, (GLuint)uniformBlockIndex, (GLuint)uniformBlockBinding );
}

FABRIC_EXT_EXPORT void klBindVertexArray(
  const KL::Size & array
){
  glBindVertexArray( (GLuint)array );
}

FABRIC_EXT_EXPORT void klDeleteVertexArrays(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & arrays
){
  glDeleteVertexArrays( (GLsizei)n, (const GLuint*)&arrays[0] );
}

FABRIC_EXT_EXPORT void klGenVertexArrays(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & arrays
){
  glGenVertexArrays( (GLsizei)n, (GLuint*)&arrays[0] );
}

FABRIC_EXT_EXPORT KL::Boolean klIsVertexArray(
  const KL::Size & array
){
  GLboolean result = glIsVertexArray( (GLuint)array );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Integer klGetUniformBufferSizeEXT(
  const KL::Size & program,
  const KL::Integer & location
){
  GLint result = glGetUniformBufferSizeEXT( (GLuint)program, (GLint)location );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer klGetUniformOffsetEXT(
  const KL::Size & program,
  const KL::Integer & location
){
  GLintptr result = glGetUniformOffsetEXT( (GLuint)program, (GLint)location );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT void klUniformBufferEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & buffer
){
  glUniformBufferEXT( (GLuint)program, (GLint)location, (GLuint)buffer );
}

FABRIC_EXT_EXPORT void klBlendColorEXT(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue,
  const KL::Scalar & alpha
){
  glBlendColorEXT( (GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha );
}

FABRIC_EXT_EXPORT void klBlendEquationSeparateEXT(
  const KL::Size & modeRGB,
  const KL::Size & modeAlpha
){
  glBlendEquationSeparateEXT( (GLenum)modeRGB, (GLenum)modeAlpha );
}

FABRIC_EXT_EXPORT void klBlendFuncSeparateEXT(
  const KL::Size & sfactorRGB,
  const KL::Size & dfactorRGB,
  const KL::Size & sfactorAlpha,
  const KL::Size & dfactorAlpha
){
  glBlendFuncSeparateEXT( (GLenum)sfactorRGB, (GLenum)dfactorRGB, (GLenum)sfactorAlpha, (GLenum)dfactorAlpha );
}

FABRIC_EXT_EXPORT void klBlendEquationEXT(
  const KL::Size & mode
){
  glBlendEquationEXT( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klColorSubTableEXT(
  const KL::Size & target,
  const KL::Size & start,
  const KL::Size & count,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data data
){
  glColorSubTableEXT( (GLenum)target, (GLsizei)start, (GLsizei)count, (GLenum)format, (GLenum)type, data );
}

FABRIC_EXT_EXPORT void klCopyColorSubTableEXT(
  const KL::Size & target,
  const KL::Size & start,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  glCopyColorSubTableEXT( (GLenum)target, (GLsizei)start, (GLint)x, (GLint)y, (GLsizei)width );
}

FABRIC_EXT_EXPORT void klLockArraysEXT(
  const KL::Integer & first,
  const KL::Size & count
){
  glLockArraysEXT( (GLint)first, (GLsizei)count );
}

FABRIC_EXT_EXPORT void klUnlockArraysEXT()
{
  glUnlockArraysEXT();
}

FABRIC_EXT_EXPORT void klConvolutionFilter1DEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data image
){
  glConvolutionFilter1DEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLenum)format, (GLenum)type, image );
}

FABRIC_EXT_EXPORT void klConvolutionFilter2DEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data image
){
  glConvolutionFilter2DEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, image );
}

FABRIC_EXT_EXPORT void klConvolutionParameterfEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glConvolutionParameterfEXT( (GLenum)target, (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klConvolutionParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glConvolutionParameterfvEXT( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klConvolutionParameteriEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  glConvolutionParameteriEXT( (GLenum)target, (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klConvolutionParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glConvolutionParameterivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klCopyConvolutionFilter1DEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  glCopyConvolutionFilter1DEXT( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width );
}

FABRIC_EXT_EXPORT void klCopyConvolutionFilter2DEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glCopyConvolutionFilter2DEXT( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klGetConvolutionFilterEXT(
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data image
){
  glGetConvolutionFilterEXT( (GLenum)target, (GLenum)format, (GLenum)type, image );
}

FABRIC_EXT_EXPORT void klGetConvolutionParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetConvolutionParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetConvolutionParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetConvolutionParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetSeparableFilterEXT(
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data row,
  KL::Data column,
  KL::Data span
){
  glGetSeparableFilterEXT( (GLenum)target, (GLenum)format, (GLenum)type, row, column, span );
}

FABRIC_EXT_EXPORT void klSeparableFilter2DEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data row,
  KL::Data column
){
  glSeparableFilter2DEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, row, column );
}

FABRIC_EXT_EXPORT void klBinormalPointerEXT(
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glBinormalPointerEXT( (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klTangentPointerEXT(
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glTangentPointerEXT( (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klCopyTexImage1DEXT(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Integer & border
){
  glCopyTexImage1DEXT( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border );
}

FABRIC_EXT_EXPORT void klCopyTexImage2DEXT(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Integer & border
){
  glCopyTexImage2DEXT( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border );
}

FABRIC_EXT_EXPORT void klCopyTexSubImage1DEXT(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  glCopyTexSubImage1DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width );
}

FABRIC_EXT_EXPORT void klCopyTexSubImage2DEXT(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glCopyTexSubImage2DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klCopyTexSubImage3DEXT(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & zoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glCopyTexSubImage3DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klCullParameterdvEXT(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glCullParameterdvEXT( (GLenum)pname, (GLdouble*)&params[0] );
}

FABRIC_EXT_EXPORT void klCullParameterfvEXT(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glCullParameterfvEXT( (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klDepthBoundsEXT(
  const KL::Scalar & zmin,
  const KL::Scalar & zmax
){
  glDepthBoundsEXT( (GLclampd)zmin, (GLclampd)zmax );
}

FABRIC_EXT_EXPORT void klBindMultiTextureEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & texture
){
  glBindMultiTextureEXT( (GLenum)texunit, (GLenum)target, (GLuint)texture );
}

FABRIC_EXT_EXPORT KL::Size klCheckNamedFramebufferStatusEXT(
  const KL::Size & framebuffer,
  const KL::Size & target
){
  GLenum result = glCheckNamedFramebufferStatusEXT( (GLuint)framebuffer, (GLenum)target );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klClientAttribDefaultEXT(
  const KL::Integer & mask
){
  glClientAttribDefaultEXT( (GLbitfield)mask );
}

FABRIC_EXT_EXPORT void klCompressedMultiTexImage1DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Integer & border,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedMultiTexImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLint)border, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedMultiTexImage2DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Integer & border,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedMultiTexImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedMultiTexImage3DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Integer & border,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedMultiTexImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedMultiTexSubImage1DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedMultiTexSubImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedMultiTexSubImage2DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedMultiTexSubImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedMultiTexSubImage3DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & zoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Size & format,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedMultiTexSubImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedTextureImage1DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Integer & border,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedTextureImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLint)border, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedTextureImage2DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Integer & border,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedTextureImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedTextureImage3DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Integer & border,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedTextureImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedTextureSubImage1DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedTextureSubImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedTextureSubImage2DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedTextureSubImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCompressedTextureSubImage3DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & zoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Size & format,
  const KL::Size & imageSize,
  KL::Data data
){
  glCompressedTextureSubImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLsizei)imageSize, data );
}

FABRIC_EXT_EXPORT void klCopyMultiTexImage1DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Integer & border
){
  glCopyMultiTexImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border );
}

FABRIC_EXT_EXPORT void klCopyMultiTexImage2DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Integer & border
){
  glCopyMultiTexImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border );
}

FABRIC_EXT_EXPORT void klCopyMultiTexSubImage1DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  glCopyMultiTexSubImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width );
}

FABRIC_EXT_EXPORT void klCopyMultiTexSubImage2DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glCopyMultiTexSubImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klCopyMultiTexSubImage3DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & zoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glCopyMultiTexSubImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klCopyTextureImage1DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Integer & border
){
  glCopyTextureImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border );
}

FABRIC_EXT_EXPORT void klCopyTextureImage2DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Integer & border
){
  glCopyTextureImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border );
}

FABRIC_EXT_EXPORT void klCopyTextureSubImage1DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  glCopyTextureSubImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width );
}

FABRIC_EXT_EXPORT void klCopyTextureSubImage2DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glCopyTextureSubImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klCopyTextureSubImage3DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & zoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glCopyTextureSubImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klDisableClientStateIndexedEXT(
  const KL::Size & array,
  const KL::Size & index
){
  glDisableClientStateIndexedEXT( (GLenum)array, (GLuint)index );
}

FABRIC_EXT_EXPORT void klDisableClientStateiEXT(
  const KL::Size & array,
  const KL::Size & index
){
  glDisableClientStateiEXT( (GLenum)array, (GLuint)index );
}

FABRIC_EXT_EXPORT void klDisableVertexArrayAttribEXT(
  const KL::Size & vaobj,
  const KL::Size & index
){
  glDisableVertexArrayAttribEXT( (GLuint)vaobj, (GLuint)index );
}

FABRIC_EXT_EXPORT void klDisableVertexArrayEXT(
  const KL::Size & vaobj,
  const KL::Size & array
){
  glDisableVertexArrayEXT( (GLuint)vaobj, (GLenum)array );
}

FABRIC_EXT_EXPORT void klEnableClientStateIndexedEXT(
  const KL::Size & array,
  const KL::Size & index
){
  glEnableClientStateIndexedEXT( (GLenum)array, (GLuint)index );
}

FABRIC_EXT_EXPORT void klEnableClientStateiEXT(
  const KL::Size & array,
  const KL::Size & index
){
  glEnableClientStateiEXT( (GLenum)array, (GLuint)index );
}

FABRIC_EXT_EXPORT void klEnableVertexArrayAttribEXT(
  const KL::Size & vaobj,
  const KL::Size & index
){
  glEnableVertexArrayAttribEXT( (GLuint)vaobj, (GLuint)index );
}

FABRIC_EXT_EXPORT void klEnableVertexArrayEXT(
  const KL::Size & vaobj,
  const KL::Size & array
){
  glEnableVertexArrayEXT( (GLuint)vaobj, (GLenum)array );
}

FABRIC_EXT_EXPORT void klFlushMappedNamedBufferRangeEXT(
  const KL::Size & buffer,
  const KL::Integer & offset,
  const KL::Size & length
){
  glFlushMappedNamedBufferRangeEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)length );
}

FABRIC_EXT_EXPORT void klFramebufferDrawBufferEXT(
  const KL::Size & framebuffer,
  const KL::Size & mode
){
  glFramebufferDrawBufferEXT( (GLuint)framebuffer, (GLenum)mode );
}

FABRIC_EXT_EXPORT void klFramebufferDrawBuffersEXT(
  const KL::Size & framebuffer,
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & bufs
){
  glFramebufferDrawBuffersEXT( (GLuint)framebuffer, (GLsizei)n, (const GLenum*)&bufs[0] );
}

FABRIC_EXT_EXPORT void klFramebufferReadBufferEXT(
  const KL::Size & framebuffer,
  const KL::Size & mode
){
  glFramebufferReadBufferEXT( (GLuint)framebuffer, (GLenum)mode );
}

FABRIC_EXT_EXPORT void klGenerateMultiTexMipmapEXT(
  const KL::Size & texunit,
  const KL::Size & target
){
  glGenerateMultiTexMipmapEXT( (GLenum)texunit, (GLenum)target );
}

FABRIC_EXT_EXPORT void klGenerateTextureMipmapEXT(
  const KL::Size & texture,
  const KL::Size & target
){
  glGenerateTextureMipmapEXT( (GLuint)texture, (GLenum)target );
}

FABRIC_EXT_EXPORT void klGetCompressedMultiTexImageEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  KL::Data img
){
  glGetCompressedMultiTexImageEXT( (GLenum)texunit, (GLenum)target, (GLint)level, img );
}

FABRIC_EXT_EXPORT void klGetCompressedTextureImageEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  KL::Data img
){
  glGetCompressedTextureImageEXT( (GLuint)texture, (GLenum)target, (GLint)level, img );
}

FABRIC_EXT_EXPORT void klGetDoubleIndexedvEXT(
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & params
){
  glGetDoubleIndexedvEXT( (GLenum)target, (GLuint)index, (GLdouble*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetDoublei_vEXT(
  const KL::Size & pname,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & params
){
  glGetDoublei_vEXT( (GLenum)pname, (GLuint)index, (GLdouble*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetFloatIndexedvEXT(
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & params
){
  glGetFloatIndexedvEXT( (GLenum)target, (GLuint)index, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetFloati_vEXT(
  const KL::Size & pname,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & params
){
  glGetFloati_vEXT( (GLenum)pname, (GLuint)index, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetFramebufferParameterivEXT(
  const KL::Size & framebuffer,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & param
){
  glGetFramebufferParameterivEXT( (GLuint)framebuffer, (GLenum)pname, (GLint*)&param[0] );
}

FABRIC_EXT_EXPORT void klGetMultiTexEnvfvEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetMultiTexEnvfvEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMultiTexEnvivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetMultiTexEnvivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMultiTexGendvEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetMultiTexGendvEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLdouble*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMultiTexGenfvEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetMultiTexGenfvEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMultiTexGenivEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetMultiTexGenivEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMultiTexImageEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glGetMultiTexImageEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klGetMultiTexLevelParameterfvEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetMultiTexLevelParameterfvEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMultiTexLevelParameterivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetMultiTexLevelParameterivEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMultiTexParameterIivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetMultiTexParameterIivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMultiTexParameterIuivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Size> & params
){
  glGetMultiTexParameterIuivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLuint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMultiTexParameterfvEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetMultiTexParameterfvEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMultiTexParameterivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetMultiTexParameterivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetNamedBufferParameterivEXT(
  const KL::Size & buffer,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetNamedBufferParameterivEXT( (GLuint)buffer, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetNamedBufferSubDataEXT(
  const KL::Size & buffer,
  const KL::Integer & offset,
  const KL::Size & size,
  KL::Data data
){
  glGetNamedBufferSubDataEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size, data );
}

FABRIC_EXT_EXPORT void klGetNamedFramebufferAttachmentParameterivEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetNamedFramebufferAttachmentParameterivEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetNamedProgramLocalParameterIivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Integer> & params
){
  glGetNamedProgramLocalParameterIivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetNamedProgramLocalParameterIuivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Size> & params
){
  glGetNamedProgramLocalParameterIuivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLuint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetNamedProgramLocalParameterdvEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & params
){
  glGetNamedProgramLocalParameterdvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLdouble*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetNamedProgramLocalParameterfvEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & params
){
  glGetNamedProgramLocalParameterfvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetNamedProgramStringEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & pname,
  KL::Data string
){
  glGetNamedProgramStringEXT( (GLuint)program, (GLenum)target, (GLenum)pname, string );
}

FABRIC_EXT_EXPORT void klGetNamedProgramivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetNamedProgramivEXT( (GLuint)program, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetNamedRenderbufferParameterivEXT(
  const KL::Size & renderbuffer,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetNamedRenderbufferParameterivEXT( (GLuint)renderbuffer, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTextureImageEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glGetTextureImageEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klGetTextureLevelParameterfvEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetTextureLevelParameterfvEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTextureLevelParameterivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetTextureLevelParameterivEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTextureParameterIivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetTextureParameterIivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTextureParameterIuivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Size> & params
){
  glGetTextureParameterIuivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLuint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTextureParameterfvEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetTextureParameterfvEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTextureParameterivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetTextureParameterivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetVertexArrayIntegeri_vEXT(
  const KL::Size & vaobj,
  const KL::Size & index,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & param
){
  glGetVertexArrayIntegeri_vEXT( (GLuint)vaobj, (GLuint)index, (GLenum)pname, (GLint*)&param[0] );
}

FABRIC_EXT_EXPORT void klGetVertexArrayIntegervEXT(
  const KL::Size & vaobj,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & param
){
  glGetVertexArrayIntegervEXT( (GLuint)vaobj, (GLenum)pname, (GLint*)&param[0] );
}

FABRIC_EXT_EXPORT GLvoid klMapNamedBufferEXT(
  const KL::Size & buffer,
  const KL::Size & access
){
  glMapNamedBufferEXT( (GLuint)buffer, (GLenum)access );
}

FABRIC_EXT_EXPORT GLvoid klMapNamedBufferRangeEXT(
  const KL::Size & buffer,
  const KL::Integer & offset,
  const KL::Size & length,
  const KL::Integer & access
){
  glMapNamedBufferRangeEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)length, (GLbitfield)access );
}

FABRIC_EXT_EXPORT void klMatrixFrustumEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & l,
  const KL::Scalar & r,
  const KL::Scalar & b,
  const KL::Scalar & t,
  const KL::Scalar & n,
  const KL::Scalar & f
){
  glMatrixFrustumEXT( (GLenum)matrixMode, (GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t, (GLdouble)n, (GLdouble)f );
}

FABRIC_EXT_EXPORT void klMatrixLoadIdentityEXT(
  const KL::Size & matrixMode
){
  glMatrixLoadIdentityEXT( (GLenum)matrixMode );
}

FABRIC_EXT_EXPORT void klMatrixLoadTransposedEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  glMatrixLoadTransposedEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
}

FABRIC_EXT_EXPORT void klMatrixLoadTransposefEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  glMatrixLoadTransposefEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
}

FABRIC_EXT_EXPORT void klMatrixLoaddEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  glMatrixLoaddEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
}

FABRIC_EXT_EXPORT void klMatrixLoadfEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  glMatrixLoadfEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
}

FABRIC_EXT_EXPORT void klMatrixMultTransposedEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  glMatrixMultTransposedEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
}

FABRIC_EXT_EXPORT void klMatrixMultTransposefEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  glMatrixMultTransposefEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
}

FABRIC_EXT_EXPORT void klMatrixMultdEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  glMatrixMultdEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
}

FABRIC_EXT_EXPORT void klMatrixMultfEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  glMatrixMultfEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
}

FABRIC_EXT_EXPORT void klMatrixOrthoEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & l,
  const KL::Scalar & r,
  const KL::Scalar & b,
  const KL::Scalar & t,
  const KL::Scalar & n,
  const KL::Scalar & f
){
  glMatrixOrthoEXT( (GLenum)matrixMode, (GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t, (GLdouble)n, (GLdouble)f );
}

FABRIC_EXT_EXPORT void klMatrixPopEXT(
  const KL::Size & matrixMode
){
  glMatrixPopEXT( (GLenum)matrixMode );
}

FABRIC_EXT_EXPORT void klMatrixPushEXT(
  const KL::Size & matrixMode
){
  glMatrixPushEXT( (GLenum)matrixMode );
}

FABRIC_EXT_EXPORT void klMatrixRotatedEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & angle,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glMatrixRotatedEXT( (GLenum)matrixMode, (GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z );
}

FABRIC_EXT_EXPORT void klMatrixRotatefEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & angle,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glMatrixRotatefEXT( (GLenum)matrixMode, (GLfloat)angle, (GLfloat)x, (GLfloat)y, (GLfloat)z );
}

FABRIC_EXT_EXPORT void klMatrixScaledEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glMatrixScaledEXT( (GLenum)matrixMode, (GLdouble)x, (GLdouble)y, (GLdouble)z );
}

FABRIC_EXT_EXPORT void klMatrixScalefEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glMatrixScalefEXT( (GLenum)matrixMode, (GLfloat)x, (GLfloat)y, (GLfloat)z );
}

FABRIC_EXT_EXPORT void klMatrixTranslatedEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glMatrixTranslatedEXT( (GLenum)matrixMode, (GLdouble)x, (GLdouble)y, (GLdouble)z );
}

FABRIC_EXT_EXPORT void klMatrixTranslatefEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  glMatrixTranslatefEXT( (GLenum)matrixMode, (GLfloat)x, (GLfloat)y, (GLfloat)z );
}

FABRIC_EXT_EXPORT void klMultiTexBufferEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & buffer
){
  glMultiTexBufferEXT( (GLenum)texunit, (GLenum)target, (GLenum)internalformat, (GLuint)buffer );
}

FABRIC_EXT_EXPORT void klMultiTexCoordPointerEXT(
  const KL::Size & texunit,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glMultiTexCoordPointerEXT( (GLenum)texunit, (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klMultiTexEnvfEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glMultiTexEnvfEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klMultiTexEnvfvEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glMultiTexEnvfvEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klMultiTexEnviEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  glMultiTexEnviEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klMultiTexEnvivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glMultiTexEnvivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klMultiTexGendEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glMultiTexGendEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLdouble)param );
}

FABRIC_EXT_EXPORT void klMultiTexGendvEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glMultiTexGendvEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (const GLdouble*)&params[0] );
}

FABRIC_EXT_EXPORT void klMultiTexGenfEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glMultiTexGenfEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klMultiTexGenfvEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glMultiTexGenfvEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klMultiTexGeniEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::Integer & param
){
  glMultiTexGeniEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klMultiTexGenivEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glMultiTexGenivEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klMultiTexImage1DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & internalformat,
  const KL::Size & width,
  const KL::Integer & border,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glMultiTexImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLint)border, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klMultiTexImage2DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Integer & border,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glMultiTexImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klMultiTexImage3DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Integer & border,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glMultiTexImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klMultiTexParameterIivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glMultiTexParameterIivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klMultiTexParameterIuivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Size> & params
){
  glMultiTexParameterIuivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLuint*)&params[0] );
}

FABRIC_EXT_EXPORT void klMultiTexParameterfEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glMultiTexParameterfEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klMultiTexParameterfvEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & param
){
  glMultiTexParameterfvEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLfloat*)&param[0] );
}

FABRIC_EXT_EXPORT void klMultiTexParameteriEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  glMultiTexParameteriEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klMultiTexParameterivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & param
){
  glMultiTexParameterivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLint*)&param[0] );
}

FABRIC_EXT_EXPORT void klMultiTexRenderbufferEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & renderbuffer
){
  glMultiTexRenderbufferEXT( (GLenum)texunit, (GLenum)target, (GLuint)renderbuffer );
}

FABRIC_EXT_EXPORT void klMultiTexSubImage1DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glMultiTexSubImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klMultiTexSubImage2DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glMultiTexSubImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klMultiTexSubImage3DEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & zoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glMultiTexSubImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klNamedBufferDataEXT(
  const KL::Size & buffer,
  const KL::Size & size,
  KL::Data data,
  const KL::Size & usage
){
  glNamedBufferDataEXT( (GLuint)buffer, (GLsizeiptr)size, data, (GLenum)usage );
}

FABRIC_EXT_EXPORT void klNamedBufferSubDataEXT(
  const KL::Size & buffer,
  const KL::Integer & offset,
  const KL::Size & size,
  KL::Data data
){
  glNamedBufferSubDataEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size, data );
}

FABRIC_EXT_EXPORT void klNamedCopyBufferSubDataEXT(
  const KL::Size & readBuffer,
  const KL::Size & writeBuffer,
  const KL::Integer & readOffset,
  const KL::Integer & writeOffset,
  const KL::Size & size
){
  glNamedCopyBufferSubDataEXT( (GLuint)readBuffer, (GLuint)writeBuffer, (GLintptr)readOffset, (GLintptr)writeOffset, (GLsizeiptr)size );
}

FABRIC_EXT_EXPORT void klNamedFramebufferRenderbufferEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & renderbuffertarget,
  const KL::Size & renderbuffer
){
  glNamedFramebufferRenderbufferEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)renderbuffertarget, (GLuint)renderbuffer );
}

FABRIC_EXT_EXPORT void klNamedFramebufferTexture1DEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level
){
  glNamedFramebufferTexture1DEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
}

FABRIC_EXT_EXPORT void klNamedFramebufferTexture2DEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level
){
  glNamedFramebufferTexture2DEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
}

FABRIC_EXT_EXPORT void klNamedFramebufferTexture3DEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Integer & zoffset
){
  glNamedFramebufferTexture3DEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level, (GLint)zoffset );
}

FABRIC_EXT_EXPORT void klNamedFramebufferTextureEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level
){
  glNamedFramebufferTextureEXT( (GLuint)framebuffer, (GLenum)attachment, (GLuint)texture, (GLint)level );
}

FABRIC_EXT_EXPORT void klNamedFramebufferTextureFaceEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Size & face
){
  glNamedFramebufferTextureFaceEXT( (GLuint)framebuffer, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLenum)face );
}

FABRIC_EXT_EXPORT void klNamedFramebufferTextureLayerEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Integer & layer
){
  glNamedFramebufferTextureLayerEXT( (GLuint)framebuffer, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer );
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameter4dEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  glNamedProgramLocalParameter4dEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameter4dvEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & params
){
  glNamedProgramLocalParameter4dvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (const GLdouble*)&params[0] );
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameter4fEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  glNamedProgramLocalParameter4fEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameter4fvEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & params
){
  glNamedProgramLocalParameter4fvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameterI4iEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  glNamedProgramLocalParameterI4iEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLint)x, (GLint)y, (GLint)z, (GLint)w );
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameterI4ivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & params
){
  glNamedProgramLocalParameterI4ivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameterI4uiEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & x,
  const KL::Size & y,
  const KL::Size & z,
  const KL::Size & w
){
  glNamedProgramLocalParameterI4uiEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z, (GLuint)w );
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameterI4uivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & params
){
  glNamedProgramLocalParameterI4uivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (const GLuint*)&params[0] );
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameters4fvEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & params
){
  glNamedProgramLocalParameters4fvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLsizei)count, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParametersI4ivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & params
){
  glNamedProgramLocalParametersI4ivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLsizei)count, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParametersI4uivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & params
){
  glNamedProgramLocalParametersI4uivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLsizei)count, (const GLuint*)&params[0] );
}

FABRIC_EXT_EXPORT void klNamedProgramStringEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & len,
  KL::Data string
){
  glNamedProgramStringEXT( (GLuint)program, (GLenum)target, (GLenum)format, (GLsizei)len, string );
}

FABRIC_EXT_EXPORT void klNamedRenderbufferStorageEXT(
  const KL::Size & renderbuffer,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  glNamedRenderbufferStorageEXT( (GLuint)renderbuffer, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klNamedRenderbufferStorageMultisampleCoverageEXT(
  const KL::Size & renderbuffer,
  const KL::Size & coverageSamples,
  const KL::Size & colorSamples,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  glNamedRenderbufferStorageMultisampleCoverageEXT( (GLuint)renderbuffer, (GLsizei)coverageSamples, (GLsizei)colorSamples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klNamedRenderbufferStorageMultisampleEXT(
  const KL::Size & renderbuffer,
  const KL::Size & samples,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  glNamedRenderbufferStorageMultisampleEXT( (GLuint)renderbuffer, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klProgramUniform1fEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Scalar & v0
){
  glProgramUniform1fEXT( (GLuint)program, (GLint)location, (GLfloat)v0 );
}

FABRIC_EXT_EXPORT void klProgramUniform1fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniform1fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniform1iEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Integer & v0
){
  glProgramUniform1iEXT( (GLuint)program, (GLint)location, (GLint)v0 );
}

FABRIC_EXT_EXPORT void klProgramUniform1ivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  glProgramUniform1ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniform1uiEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & v0
){
  glProgramUniform1uiEXT( (GLuint)program, (GLint)location, (GLuint)v0 );
}

FABRIC_EXT_EXPORT void klProgramUniform1uivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  glProgramUniform1uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniform2fEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Scalar & v0,
  const KL::Scalar & v1
){
  glProgramUniform2fEXT( (GLuint)program, (GLint)location, (GLfloat)v0, (GLfloat)v1 );
}

FABRIC_EXT_EXPORT void klProgramUniform2fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniform2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniform2iEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Integer & v0,
  const KL::Integer & v1
){
  glProgramUniform2iEXT( (GLuint)program, (GLint)location, (GLint)v0, (GLint)v1 );
}

FABRIC_EXT_EXPORT void klProgramUniform2ivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  glProgramUniform2ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniform2uiEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & v0,
  const KL::Size & v1
){
  glProgramUniform2uiEXT( (GLuint)program, (GLint)location, (GLuint)v0, (GLuint)v1 );
}

FABRIC_EXT_EXPORT void klProgramUniform2uivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  glProgramUniform2uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniform3fEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Scalar & v0,
  const KL::Scalar & v1,
  const KL::Scalar & v2
){
  glProgramUniform3fEXT( (GLuint)program, (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2 );
}

FABRIC_EXT_EXPORT void klProgramUniform3fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniform3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniform3iEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Integer & v0,
  const KL::Integer & v1,
  const KL::Integer & v2
){
  glProgramUniform3iEXT( (GLuint)program, (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2 );
}

FABRIC_EXT_EXPORT void klProgramUniform3ivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  glProgramUniform3ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniform3uiEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & v0,
  const KL::Size & v1,
  const KL::Size & v2
){
  glProgramUniform3uiEXT( (GLuint)program, (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2 );
}

FABRIC_EXT_EXPORT void klProgramUniform3uivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  glProgramUniform3uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniform4fEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Scalar & v0,
  const KL::Scalar & v1,
  const KL::Scalar & v2,
  const KL::Scalar & v3
){
  glProgramUniform4fEXT( (GLuint)program, (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2, (GLfloat)v3 );
}

FABRIC_EXT_EXPORT void klProgramUniform4fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniform4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniform4iEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Integer & v0,
  const KL::Integer & v1,
  const KL::Integer & v2,
  const KL::Integer & v3
){
  glProgramUniform4iEXT( (GLuint)program, (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2, (GLint)v3 );
}

FABRIC_EXT_EXPORT void klProgramUniform4ivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  glProgramUniform4ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniform4uiEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & v0,
  const KL::Size & v1,
  const KL::Size & v2,
  const KL::Size & v3
){
  glProgramUniform4uiEXT( (GLuint)program, (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2, (GLuint)v3 );
}

FABRIC_EXT_EXPORT void klProgramUniform4uivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  glProgramUniform4uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix2fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniformMatrix2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix2x3fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniformMatrix2x3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix2x4fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniformMatrix2x4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix3fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniformMatrix3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix3x2fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniformMatrix3x2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix3x4fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniformMatrix3x4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix4fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniformMatrix4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix4x2fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniformMatrix4x2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix4x3fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  glProgramUniformMatrix4x3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
}

FABRIC_EXT_EXPORT void klPushClientAttribDefaultEXT(
  const KL::Integer & mask
){
  glPushClientAttribDefaultEXT( (GLbitfield)mask );
}

FABRIC_EXT_EXPORT void klTextureBufferEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & buffer
){
  glTextureBufferEXT( (GLuint)texture, (GLenum)target, (GLenum)internalformat, (GLuint)buffer );
}

FABRIC_EXT_EXPORT void klTextureImage1DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & internalformat,
  const KL::Size & width,
  const KL::Integer & border,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTextureImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLint)border, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTextureImage2DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Integer & border,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTextureImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTextureImage3DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Integer & border,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTextureImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTextureParameterIivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glTextureParameterIivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klTextureParameterIuivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Size> & params
){
  glTextureParameterIuivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLuint*)&params[0] );
}

FABRIC_EXT_EXPORT void klTextureParameterfEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glTextureParameterfEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klTextureParameterfvEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & param
){
  glTextureParameterfvEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLfloat*)&param[0] );
}

FABRIC_EXT_EXPORT void klTextureParameteriEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  glTextureParameteriEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klTextureParameterivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & param
){
  glTextureParameterivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLint*)&param[0] );
}

FABRIC_EXT_EXPORT void klTextureRenderbufferEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & renderbuffer
){
  glTextureRenderbufferEXT( (GLuint)texture, (GLenum)target, (GLuint)renderbuffer );
}

FABRIC_EXT_EXPORT void klTextureSubImage1DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTextureSubImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTextureSubImage2DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTextureSubImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTextureSubImage3DEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & zoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTextureSubImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT KL::Boolean klUnmapNamedBufferEXT(
  const KL::Size & buffer
){
  GLboolean result = glUnmapNamedBufferEXT( (GLuint)buffer );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void klVertexArrayColorOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  glVertexArrayColorOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
}

FABRIC_EXT_EXPORT void klVertexArrayEdgeFlagOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Size & stride,
  const KL::Integer & offset
){
  glVertexArrayEdgeFlagOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLsizei)stride, (GLintptr)offset );
}

FABRIC_EXT_EXPORT void klVertexArrayFogCoordOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  glVertexArrayFogCoordOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
}

FABRIC_EXT_EXPORT void klVertexArrayIndexOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  glVertexArrayIndexOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
}

FABRIC_EXT_EXPORT void klVertexArrayMultiTexCoordOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Size & texunit,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  glVertexArrayMultiTexCoordOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLenum)texunit, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
}

FABRIC_EXT_EXPORT void klVertexArrayNormalOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  glVertexArrayNormalOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
}

FABRIC_EXT_EXPORT void klVertexArraySecondaryColorOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  glVertexArraySecondaryColorOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
}

FABRIC_EXT_EXPORT void klVertexArrayTexCoordOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  glVertexArrayTexCoordOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
}

FABRIC_EXT_EXPORT void klVertexArrayVertexAttribIOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Size & index,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  glVertexArrayVertexAttribIOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLuint)index, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
}

FABRIC_EXT_EXPORT void klVertexArrayVertexAttribOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Size & index,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Boolean & normalized,
  const KL::Size & stride,
  const KL::Integer & offset
){
  glVertexArrayVertexAttribOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLuint)index, (GLint)size, (GLenum)type, (GLboolean)normalized, (GLsizei)stride, (GLintptr)offset );
}

FABRIC_EXT_EXPORT void klVertexArrayVertexOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  glVertexArrayVertexOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
}

FABRIC_EXT_EXPORT void klColorMaskIndexedEXT(
  const KL::Size & buf,
  const KL::Boolean & r,
  const KL::Boolean & g,
  const KL::Boolean & b,
  const KL::Boolean & a
){
  glColorMaskIndexedEXT( (GLuint)buf, (GLboolean)r, (GLboolean)g, (GLboolean)b, (GLboolean)a );
}

FABRIC_EXT_EXPORT void klDisableIndexedEXT(
  const KL::Size & target,
  const KL::Size & index
){
  glDisableIndexedEXT( (GLenum)target, (GLuint)index );
}

FABRIC_EXT_EXPORT void klEnableIndexedEXT(
  const KL::Size & target,
  const KL::Size & index
){
  glEnableIndexedEXT( (GLenum)target, (GLuint)index );
}

FABRIC_EXT_EXPORT void klGetBooleanIndexedvEXT(
  const KL::Size & value,
  const KL::Size & index,
  KL::VariableArray<KL::Boolean> & data
){
  glGetBooleanIndexedvEXT( (GLenum)value, (GLuint)index, (GLboolean*)&data[0] );
}

FABRIC_EXT_EXPORT void klGetIntegerIndexedvEXT(
  const KL::Size & value,
  const KL::Size & index,
  KL::VariableArray<KL::Integer> & data
){
  glGetIntegerIndexedvEXT( (GLenum)value, (GLuint)index, (GLint*)&data[0] );
}

FABRIC_EXT_EXPORT KL::Boolean klIsEnabledIndexedEXT(
  const KL::Size & target,
  const KL::Size & index
){
  GLboolean result = glIsEnabledIndexedEXT( (GLenum)target, (GLuint)index );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void klDrawArraysInstancedEXT(
  const KL::Size & mode,
  const KL::Integer & start,
  const KL::Size & count,
  const KL::Size & primcount
){
  glDrawArraysInstancedEXT( (GLenum)mode, (GLint)start, (GLsizei)count, (GLsizei)primcount );
}

FABRIC_EXT_EXPORT void klDrawElementsInstancedEXT(
  const KL::Size & mode,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices,
  const KL::Size & primcount
){
  glDrawElementsInstancedEXT( (GLenum)mode, (GLsizei)count, (GLenum)type, indices, (GLsizei)primcount );
}

FABRIC_EXT_EXPORT void klDrawRangeElementsEXT(
  const KL::Size & mode,
  const KL::Size & start,
  const KL::Size & end,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices
){
  glDrawRangeElementsEXT( (GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, indices );
}

FABRIC_EXT_EXPORT void klFogCoordPointerEXT(
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glFogCoordPointerEXT( (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klFogCoorddEXT(
  const KL::Scalar & coord
){
  glFogCoorddEXT( (GLdouble)coord );
}

FABRIC_EXT_EXPORT void klFogCoorddvEXT(
  const KL::VariableArray<KL::Scalar> & coord
){
  glFogCoorddvEXT( (const GLdouble*)&coord[0] );
}

FABRIC_EXT_EXPORT void klFogCoordfEXT(
  const KL::Scalar & coord
){
  glFogCoordfEXT( (GLfloat)coord );
}

FABRIC_EXT_EXPORT void klFogCoordfvEXT(
  const KL::VariableArray<KL::Scalar> & coord
){
  glFogCoordfvEXT( (const GLfloat*)&coord[0] );
}

FABRIC_EXT_EXPORT void klFragmentColorMaterialEXT(
  const KL::Size & face,
  const KL::Size & mode
){
  glFragmentColorMaterialEXT( (GLenum)face, (GLenum)mode );
}

FABRIC_EXT_EXPORT void klFragmentLightModelfEXT(
  const KL::Size & pname,
  const KL::Scalar & param
){
  glFragmentLightModelfEXT( (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klFragmentLightModelfvEXT(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glFragmentLightModelfvEXT( (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klFragmentLightModeliEXT(
  const KL::Size & pname,
  const KL::Integer & param
){
  glFragmentLightModeliEXT( (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klFragmentLightModelivEXT(
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glFragmentLightModelivEXT( (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klFragmentLightfEXT(
  const KL::Size & light,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glFragmentLightfEXT( (GLenum)light, (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klFragmentLightfvEXT(
  const KL::Size & light,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glFragmentLightfvEXT( (GLenum)light, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klFragmentLightiEXT(
  const KL::Size & light,
  const KL::Size & pname,
  const KL::Integer & param
){
  glFragmentLightiEXT( (GLenum)light, (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klFragmentLightivEXT(
  const KL::Size & light,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glFragmentLightivEXT( (GLenum)light, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klFragmentMaterialfEXT(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glFragmentMaterialfEXT( (GLenum)face, (GLenum)pname, (const GLfloat)param );
}

FABRIC_EXT_EXPORT void klFragmentMaterialfvEXT(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glFragmentMaterialfvEXT( (GLenum)face, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klFragmentMaterialiEXT(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::Integer & param
){
  glFragmentMaterialiEXT( (GLenum)face, (GLenum)pname, (const GLint)param );
}

FABRIC_EXT_EXPORT void klFragmentMaterialivEXT(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glFragmentMaterialivEXT( (GLenum)face, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetFragmentLightfvEXT(
  const KL::Size & light,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetFragmentLightfvEXT( (GLenum)light, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetFragmentLightivEXT(
  const KL::Size & light,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetFragmentLightivEXT( (GLenum)light, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetFragmentMaterialfvEXT(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glGetFragmentMaterialfvEXT( (GLenum)face, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetFragmentMaterialivEXT(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glGetFragmentMaterialivEXT( (GLenum)face, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klLightEnviEXT(
  const KL::Size & pname,
  const KL::Integer & param
){
  glLightEnviEXT( (GLenum)pname, (GLint)param );
}

FABRIC_EXT_EXPORT void klBlitFramebufferEXT(
  const KL::Integer & srcX0,
  const KL::Integer & srcY0,
  const KL::Integer & srcX1,
  const KL::Integer & srcY1,
  const KL::Integer & dstX0,
  const KL::Integer & dstY0,
  const KL::Integer & dstX1,
  const KL::Integer & dstY1,
  const KL::Integer & mask,
  const KL::Size & filter
){
  glBlitFramebufferEXT( (GLint)srcX0, (GLint)srcY0, (GLint)srcX1, (GLint)srcY1, (GLint)dstX0, (GLint)dstY0, (GLint)dstX1, (GLint)dstY1, (GLbitfield)mask, (GLenum)filter );
}

FABRIC_EXT_EXPORT void klRenderbufferStorageMultisampleEXT(
  const KL::Size & target,
  const KL::Size & samples,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  glRenderbufferStorageMultisampleEXT( (GLenum)target, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klBindFramebufferEXT(
  const KL::Size & target,
  const KL::Size & framebuffer
){
  glBindFramebufferEXT( (GLenum)target, (GLuint)framebuffer );
}

FABRIC_EXT_EXPORT void klBindRenderbufferEXT(
  const KL::Size & target,
  const KL::Size & renderbuffer
){
  glBindRenderbufferEXT( (GLenum)target, (GLuint)renderbuffer );
}

FABRIC_EXT_EXPORT KL::Size klCheckFramebufferStatusEXT(
  const KL::Size & target
){
  GLenum result = glCheckFramebufferStatusEXT( (GLenum)target );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klDeleteFramebuffersEXT(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & framebuffers
){
  glDeleteFramebuffersEXT( (GLsizei)n, (const GLuint*)&framebuffers[0] );
}

FABRIC_EXT_EXPORT void klDeleteRenderbuffersEXT(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & renderbuffers
){
  glDeleteRenderbuffersEXT( (GLsizei)n, (const GLuint*)&renderbuffers[0] );
}

FABRIC_EXT_EXPORT void klFramebufferRenderbufferEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & renderbuffertarget,
  const KL::Size & renderbuffer
){
  glFramebufferRenderbufferEXT( (GLenum)target, (GLenum)attachment, (GLenum)renderbuffertarget, (GLuint)renderbuffer );
}

FABRIC_EXT_EXPORT void klFramebufferTexture1DEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level
){
  glFramebufferTexture1DEXT( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
}

FABRIC_EXT_EXPORT void klFramebufferTexture2DEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level
){
  glFramebufferTexture2DEXT( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
}

FABRIC_EXT_EXPORT void klFramebufferTexture3DEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Integer & zoffset
){
  glFramebufferTexture3DEXT( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level, (GLint)zoffset );
}

FABRIC_EXT_EXPORT void klGenFramebuffersEXT(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & framebuffers
){
  glGenFramebuffersEXT( (GLsizei)n, (GLuint*)&framebuffers[0] );
}

FABRIC_EXT_EXPORT void klGenRenderbuffersEXT(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & renderbuffers
){
  glGenRenderbuffersEXT( (GLsizei)n, (GLuint*)&renderbuffers[0] );
}

FABRIC_EXT_EXPORT void klGenerateMipmapEXT(
  const KL::Size & target
){
  glGenerateMipmapEXT( (GLenum)target );
}

FABRIC_EXT_EXPORT void klGetFramebufferAttachmentParameterivEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetFramebufferAttachmentParameterivEXT( (GLenum)target, (GLenum)attachment, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetRenderbufferParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetRenderbufferParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT KL::Boolean klIsFramebufferEXT(
  const KL::Size & framebuffer
){
  GLboolean result = glIsFramebufferEXT( (GLuint)framebuffer );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean klIsRenderbufferEXT(
  const KL::Size & renderbuffer
){
  GLboolean result = glIsRenderbufferEXT( (GLuint)renderbuffer );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void klRenderbufferStorageEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  glRenderbufferStorageEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klFramebufferTextureEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level
){
  glFramebufferTextureEXT( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level );
}

FABRIC_EXT_EXPORT void klFramebufferTextureFaceEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Size & face
){
  glFramebufferTextureFaceEXT( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLenum)face );
}

FABRIC_EXT_EXPORT void klFramebufferTextureLayerEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Integer & layer
){
  glFramebufferTextureLayerEXT( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer );
}

FABRIC_EXT_EXPORT void klProgramParameteriEXT(
  const KL::Size & program,
  const KL::Size & pname,
  const KL::Integer & value
){
  glProgramParameteriEXT( (GLuint)program, (GLenum)pname, (GLint)value );
}

FABRIC_EXT_EXPORT void klProgramEnvParameters4fvEXT(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & params
){
  glProgramEnvParameters4fvEXT( (GLenum)target, (GLuint)index, (GLsizei)count, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klProgramLocalParameters4fvEXT(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & params
){
  glProgramLocalParameters4fvEXT( (GLenum)target, (GLuint)index, (GLsizei)count, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klBindFragDataLocationEXT(
  const KL::Size & program,
  const KL::Size & color,
  const KL::String &name
){
  glBindFragDataLocationEXT( (GLuint)program, (GLuint)color, (const GLchar*)name.data() );
}

FABRIC_EXT_EXPORT KL::Integer klGetFragDataLocationEXT(
  const KL::Size & program,
  const KL::String &name
){
  GLint result = glGetFragDataLocationEXT( (GLuint)program, (const GLchar*)name.data() );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT void klGetUniformuivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  KL::VariableArray<KL::Size> & params
){
  glGetUniformuivEXT( (GLuint)program, (GLint)location, (GLuint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetVertexAttribIivEXT(
  const KL::Size & index,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetVertexAttribIivEXT( (GLuint)index, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetVertexAttribIuivEXT(
  const KL::Size & index,
  const KL::Size & pname,
  KL::VariableArray<KL::Size> & params
){
  glGetVertexAttribIuivEXT( (GLuint)index, (GLenum)pname, (GLuint*)&params[0] );
}

FABRIC_EXT_EXPORT void klUniform1uiEXT(
  const KL::Integer & location,
  const KL::Size & v0
){
  glUniform1uiEXT( (GLint)location, (GLuint)v0 );
}

FABRIC_EXT_EXPORT void klUniform1uivEXT(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  glUniform1uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniform2uiEXT(
  const KL::Integer & location,
  const KL::Size & v0,
  const KL::Size & v1
){
  glUniform2uiEXT( (GLint)location, (GLuint)v0, (GLuint)v1 );
}

FABRIC_EXT_EXPORT void klUniform2uivEXT(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  glUniform2uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniform3uiEXT(
  const KL::Integer & location,
  const KL::Size & v0,
  const KL::Size & v1,
  const KL::Size & v2
){
  glUniform3uiEXT( (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2 );
}

FABRIC_EXT_EXPORT void klUniform3uivEXT(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  glUniform3uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
}

FABRIC_EXT_EXPORT void klUniform4uiEXT(
  const KL::Integer & location,
  const KL::Size & v0,
  const KL::Size & v1,
  const KL::Size & v2,
  const KL::Size & v3
){
  glUniform4uiEXT( (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2, (GLuint)v3 );
}

FABRIC_EXT_EXPORT void klUniform4uivEXT(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  glUniform4uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI1iEXT(
  const KL::Size & index,
  const KL::Integer & x
){
  glVertexAttribI1iEXT( (GLuint)index, (GLint)x );
}

FABRIC_EXT_EXPORT void klVertexAttribI1ivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttribI1ivEXT( (GLuint)index, (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI1uiEXT(
  const KL::Size & index,
  const KL::Size & x
){
  glVertexAttribI1uiEXT( (GLuint)index, (GLuint)x );
}

FABRIC_EXT_EXPORT void klVertexAttribI1uivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  glVertexAttribI1uivEXT( (GLuint)index, (const GLuint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI2iEXT(
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y
){
  glVertexAttribI2iEXT( (GLuint)index, (GLint)x, (GLint)y );
}

FABRIC_EXT_EXPORT void klVertexAttribI2ivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttribI2ivEXT( (GLuint)index, (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI2uiEXT(
  const KL::Size & index,
  const KL::Size & x,
  const KL::Size & y
){
  glVertexAttribI2uiEXT( (GLuint)index, (GLuint)x, (GLuint)y );
}

FABRIC_EXT_EXPORT void klVertexAttribI2uivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  glVertexAttribI2uivEXT( (GLuint)index, (const GLuint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI3iEXT(
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  glVertexAttribI3iEXT( (GLuint)index, (GLint)x, (GLint)y, (GLint)z );
}

FABRIC_EXT_EXPORT void klVertexAttribI3ivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttribI3ivEXT( (GLuint)index, (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI3uiEXT(
  const KL::Size & index,
  const KL::Size & x,
  const KL::Size & y,
  const KL::Size & z
){
  glVertexAttribI3uiEXT( (GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z );
}

FABRIC_EXT_EXPORT void klVertexAttribI3uivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  glVertexAttribI3uivEXT( (GLuint)index, (const GLuint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI4bvEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Byte> & v
){
  glVertexAttribI4bvEXT( (GLuint)index, (const GLbyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI4iEXT(
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  glVertexAttribI4iEXT( (GLuint)index, (GLint)x, (GLint)y, (GLint)z, (GLint)w );
}

FABRIC_EXT_EXPORT void klVertexAttribI4ivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttribI4ivEXT( (GLuint)index, (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI4svEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttribI4svEXT( (GLuint)index, (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI4ubvEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  glVertexAttribI4ubvEXT( (GLuint)index, (const GLubyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI4uiEXT(
  const KL::Size & index,
  const KL::Size & x,
  const KL::Size & y,
  const KL::Size & z,
  const KL::Size & w
){
  glVertexAttribI4uiEXT( (GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z, (GLuint)w );
}

FABRIC_EXT_EXPORT void klVertexAttribI4uivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  glVertexAttribI4uivEXT( (GLuint)index, (const GLuint*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribI4usvEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  glVertexAttribI4usvEXT( (GLuint)index, (const GLushort*)&v[0] );
}

FABRIC_EXT_EXPORT void klVertexAttribIPointerEXT(
  const KL::Size & index,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glVertexAttribIPointerEXT( (GLuint)index, (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klGetHistogramEXT(
  const KL::Size & target,
  const KL::Boolean & reset,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data values
){
  glGetHistogramEXT( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, values );
}

FABRIC_EXT_EXPORT void klGetHistogramParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetHistogramParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetHistogramParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetHistogramParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMinmaxEXT(
  const KL::Size & target,
  const KL::Boolean & reset,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data values
){
  glGetMinmaxEXT( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, values );
}

FABRIC_EXT_EXPORT void klGetMinmaxParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetMinmaxParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetMinmaxParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetMinmaxParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klHistogramEXT(
  const KL::Size & target,
  const KL::Size & width,
  const KL::Size & internalformat,
  const KL::Boolean & sink
){
  glHistogramEXT( (GLenum)target, (GLsizei)width, (GLenum)internalformat, (GLboolean)sink );
}

FABRIC_EXT_EXPORT void klMinmaxEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Boolean & sink
){
  glMinmaxEXT( (GLenum)target, (GLenum)internalformat, (GLboolean)sink );
}

FABRIC_EXT_EXPORT void klResetHistogramEXT(
  const KL::Size & target
){
  glResetHistogramEXT( (GLenum)target );
}

FABRIC_EXT_EXPORT void klResetMinmaxEXT(
  const KL::Size & target
){
  glResetMinmaxEXT( (GLenum)target );
}

FABRIC_EXT_EXPORT void klIndexFuncEXT(
  const KL::Size & func,
  const KL::Scalar & ref
){
  glIndexFuncEXT( (GLenum)func, (GLfloat)ref );
}

FABRIC_EXT_EXPORT void klIndexMaterialEXT(
  const KL::Size & face,
  const KL::Size & mode
){
  glIndexMaterialEXT( (GLenum)face, (GLenum)mode );
}

FABRIC_EXT_EXPORT void klApplyTextureEXT(
  const KL::Size & mode
){
  glApplyTextureEXT( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klTextureLightEXT(
  const KL::Size & pname
){
  glTextureLightEXT( (GLenum)pname );
}

FABRIC_EXT_EXPORT void klTextureMaterialEXT(
  const KL::Size & face,
  const KL::Size & mode
){
  glTextureMaterialEXT( (GLenum)face, (GLenum)mode );
}

FABRIC_EXT_EXPORT void klMultiDrawArraysEXT(
  const KL::Size & mode,
  KL::VariableArray<KL::Integer> & first,
  KL::VariableArray<KL::Size> & count,
  const KL::Size & primcount
){
  glMultiDrawArraysEXT( (GLenum)mode, (GLint*)&first[0], (GLsizei*)&count[0], (GLsizei)primcount );
}

FABRIC_EXT_EXPORT void klSampleMaskEXT(
  const KL::Scalar & value,
  const KL::Boolean & invert
){
  glSampleMaskEXT( (GLclampf)value, (GLboolean)invert );
}

FABRIC_EXT_EXPORT void klSamplePatternEXT(
  const KL::Size & pattern
){
  glSamplePatternEXT( (GLenum)pattern );
}

FABRIC_EXT_EXPORT void klColorTableEXT(
  const KL::Size & target,
  const KL::Size & internalFormat,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data data
){
  glColorTableEXT( (GLenum)target, (GLenum)internalFormat, (GLsizei)width, (GLenum)format, (GLenum)type, data );
}

FABRIC_EXT_EXPORT void klGetColorTableEXT(
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data data
){
  glGetColorTableEXT( (GLenum)target, (GLenum)format, (GLenum)type, data );
}

FABRIC_EXT_EXPORT void klGetColorTableParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glGetColorTableParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetColorTableParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetColorTableParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetPixelTransformParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glGetPixelTransformParameterfvEXT( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetPixelTransformParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glGetPixelTransformParameterivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klPixelTransformParameterfEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  glPixelTransformParameterfEXT( (GLenum)target, (GLenum)pname, (const GLfloat)param );
}

FABRIC_EXT_EXPORT void klPixelTransformParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  glPixelTransformParameterfvEXT( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klPixelTransformParameteriEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  glPixelTransformParameteriEXT( (GLenum)target, (GLenum)pname, (const GLint)param );
}

FABRIC_EXT_EXPORT void klPixelTransformParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glPixelTransformParameterivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klPointParameterfEXT(
  const KL::Size & pname,
  const KL::Scalar & param
){
  glPointParameterfEXT( (GLenum)pname, (GLfloat)param );
}

FABRIC_EXT_EXPORT void klPointParameterfvEXT(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  glPointParameterfvEXT( (GLenum)pname, (GLfloat*)&params[0] );
}

FABRIC_EXT_EXPORT void klPolygonOffsetEXT(
  const KL::Scalar & factor,
  const KL::Scalar & bias
){
  glPolygonOffsetEXT( (GLfloat)factor, (GLfloat)bias );
}

FABRIC_EXT_EXPORT void klProvokingVertexEXT(
  const KL::Size & mode
){
  glProvokingVertexEXT( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klBeginSceneEXT()
{
  glBeginSceneEXT();
}

FABRIC_EXT_EXPORT void klEndSceneEXT()
{
  glEndSceneEXT();
}

FABRIC_EXT_EXPORT void klSecondaryColor3bEXT(
  const KL::Byte & red,
  const KL::Byte & green,
  const KL::Byte & blue
){
  glSecondaryColor3bEXT( (GLbyte)red, (GLbyte)green, (GLbyte)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3bvEXT(
  const KL::VariableArray<KL::Byte> & v
){
  glSecondaryColor3bvEXT( (const GLbyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3dEXT(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue
){
  glSecondaryColor3dEXT( (GLdouble)red, (GLdouble)green, (GLdouble)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3dvEXT(
  const KL::VariableArray<KL::Scalar> & v
){
  glSecondaryColor3dvEXT( (const GLdouble*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3fEXT(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue
){
  glSecondaryColor3fEXT( (GLfloat)red, (GLfloat)green, (GLfloat)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3fvEXT(
  const KL::VariableArray<KL::Scalar> & v
){
  glSecondaryColor3fvEXT( (const GLfloat*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3iEXT(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  glSecondaryColor3iEXT( (GLint)red, (GLint)green, (GLint)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3ivEXT(
  const KL::VariableArray<KL::Integer> & v
){
  glSecondaryColor3ivEXT( (const GLint*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3sEXT(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  glSecondaryColor3sEXT( (GLshort)red, (GLshort)green, (GLshort)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3svEXT(
  const KL::VariableArray<KL::Integer> & v
){
  glSecondaryColor3svEXT( (const GLshort*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3ubEXT(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue
){
  glSecondaryColor3ubEXT( (GLubyte)red, (GLubyte)green, (GLubyte)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3ubvEXT(
  const KL::VariableArray<KL::Size> & v
){
  glSecondaryColor3ubvEXT( (const GLubyte*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3uiEXT(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue
){
  glSecondaryColor3uiEXT( (GLuint)red, (GLuint)green, (GLuint)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3uivEXT(
  const KL::VariableArray<KL::Size> & v
){
  glSecondaryColor3uivEXT( (const GLuint*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColor3usEXT(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  glSecondaryColor3usEXT( (GLushort)red, (GLushort)green, (GLushort)blue );
}

FABRIC_EXT_EXPORT void klSecondaryColor3usvEXT(
  const KL::VariableArray<KL::Integer> & v
){
  glSecondaryColor3usvEXT( (const GLushort*)&v[0] );
}

FABRIC_EXT_EXPORT void klSecondaryColorPointerEXT(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glSecondaryColorPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klActiveProgramEXT(
  const KL::Size & program
){
  glActiveProgramEXT( (GLuint)program );
}

FABRIC_EXT_EXPORT KL::Size klCreateShaderProgramEXT(
  const KL::Size & type,
  const KL::String &string
){
  GLuint result = glCreateShaderProgramEXT( (GLenum)type, (const char*)string.data() );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klUseShaderProgramEXT(
  const KL::Size & type,
  const KL::Size & program
){
  glUseShaderProgramEXT( (GLenum)type, (GLuint)program );
}

FABRIC_EXT_EXPORT void klActiveStencilFaceEXT(
  const KL::Size & face
){
  glActiveStencilFaceEXT( (GLenum)face );
}

FABRIC_EXT_EXPORT void klTexSubImage1DEXT(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTexSubImage1DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTexSubImage2DEXT(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTexSubImage2DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTexSubImage3DEXT(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & yoffset,
  const KL::Integer & zoffset,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTexSubImage3DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTexImage3DEXT(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Integer & border,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  glTexImage3DEXT( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
}

FABRIC_EXT_EXPORT void klTexBufferEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & buffer
){
  glTexBufferEXT( (GLenum)target, (GLenum)internalformat, (GLuint)buffer );
}

FABRIC_EXT_EXPORT void klClearColorIiEXT(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue,
  const KL::Integer & alpha
){
  glClearColorIiEXT( (GLint)red, (GLint)green, (GLint)blue, (GLint)alpha );
}

FABRIC_EXT_EXPORT void klClearColorIuiEXT(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue,
  const KL::Size & alpha
){
  glClearColorIuiEXT( (GLuint)red, (GLuint)green, (GLuint)blue, (GLuint)alpha );
}

FABRIC_EXT_EXPORT void klGetTexParameterIivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  glGetTexParameterIivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klGetTexParameterIuivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Size> & params
){
  glGetTexParameterIuivEXT( (GLenum)target, (GLenum)pname, (GLuint*)&params[0] );
}

FABRIC_EXT_EXPORT void klTexParameterIivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  glTexParameterIivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
}

FABRIC_EXT_EXPORT void klTexParameterIuivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Size> & params
){
  glTexParameterIuivEXT( (GLenum)target, (GLenum)pname, (const GLuint*)&params[0] );
}

FABRIC_EXT_EXPORT KL::Boolean klAreTexturesResidentEXT(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & textures,
  KL::VariableArray<KL::Boolean> & residences
){
  GLboolean result = glAreTexturesResidentEXT( (GLsizei)n, (const GLuint*)&textures[0], (GLboolean*)&residences[0] );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void klBindTextureEXT(
  const KL::Size & target,
  const KL::Size & texture
){
  glBindTextureEXT( (GLenum)target, (GLuint)texture );
}

FABRIC_EXT_EXPORT void klDeleteTexturesEXT(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & textures
){
  glDeleteTexturesEXT( (GLsizei)n, (const GLuint*)&textures[0] );
}

FABRIC_EXT_EXPORT void klGenTexturesEXT(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & textures
){
  glGenTexturesEXT( (GLsizei)n, (GLuint*)&textures[0] );
}

FABRIC_EXT_EXPORT KL::Boolean klIsTextureEXT(
  const KL::Size & texture
){
  GLboolean result = glIsTextureEXT( (GLuint)texture );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void klPrioritizeTexturesEXT(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & textures,
  const KL::VariableArray<KL::Scalar> & priorities
){
  glPrioritizeTexturesEXT( (GLsizei)n, (const GLuint*)&textures[0], (const GLclampf*)&priorities[0] );
}

FABRIC_EXT_EXPORT void klTextureNormalEXT(
  const KL::Size & mode
){
  glTextureNormalEXT( (GLenum)mode );
}

FABRIC_EXT_EXPORT void klBeginTransformFeedbackEXT(
  const KL::Size & primitiveMode
){
  glBeginTransformFeedbackEXT( (GLenum)primitiveMode );
}

FABRIC_EXT_EXPORT void klBindBufferBaseEXT(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & buffer
){
  glBindBufferBaseEXT( (GLenum)target, (GLuint)index, (GLuint)buffer );
}

FABRIC_EXT_EXPORT void klBindBufferOffsetEXT(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & buffer,
  const KL::Integer & offset
){
  glBindBufferOffsetEXT( (GLenum)target, (GLuint)index, (GLuint)buffer, (GLintptr)offset );
}

FABRIC_EXT_EXPORT void klBindBufferRangeEXT(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & buffer,
  const KL::Integer & offset,
  const KL::Size & size
){
  glBindBufferRangeEXT( (GLenum)target, (GLuint)index, (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size );
}

FABRIC_EXT_EXPORT void klEndTransformFeedbackEXT()
{
  glEndTransformFeedbackEXT();
}

FABRIC_EXT_EXPORT void klGetTransformFeedbackVaryingEXT(
  const KL::Size & program,
  const KL::Size & index,
  const KL::Size & bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::VariableArray<KL::Size> & size,
  KL::VariableArray<KL::Size> & type,
  KL::String & name
){
  char * nameStr = new char[1024];
  glGetTransformFeedbackVaryingEXT( (GLuint)program, (GLuint)index, (GLsizei)bufSize, (GLsizei*)&length[0], (GLsizei*)&size[0], (GLenum*)&type[0], (char*)nameStr );
  name = KL::String(nameStr);
  delete( nameStr );
}

FABRIC_EXT_EXPORT void klTransformFeedbackVaryingsEXT(
  const KL::Size & program,
  const KL::Size & count,
  const KL::VariableArray<KL::String> &varyings,
  const KL::Size & bufferMode
){
  glTransformFeedbackVaryingsEXT( (GLuint)program, (GLsizei)count, (const char**)&varyings[0], (GLenum)bufferMode );
}

FABRIC_EXT_EXPORT void klArrayElementEXT(
  const KL::Integer & i
){
  glArrayElementEXT( (GLint)i );
}

FABRIC_EXT_EXPORT void klColorPointerEXT(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Size & count,
  KL::Data pointer
){
  glColorPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
}

FABRIC_EXT_EXPORT void klDrawArraysEXT(
  const KL::Size & mode,
  const KL::Integer & first,
  const KL::Size & count
){
  glDrawArraysEXT( (GLenum)mode, (GLint)first, (GLsizei)count );
}

FABRIC_EXT_EXPORT void klEdgeFlagPointerEXT(
  const KL::Size & stride,
  const KL::Size & count,
  const KL::VariableArray<KL::Boolean> & pointer
){
  glEdgeFlagPointerEXT( (GLsizei)stride, (GLsizei)count, (const GLboolean*)&pointer[0] );
}

FABRIC_EXT_EXPORT void klIndexPointerEXT(
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Size & count,
  KL::Data pointer
){
  glIndexPointerEXT( (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
}

FABRIC_EXT_EXPORT void klNormalPointerEXT(
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Size & count,
  KL::Data pointer
){
  glNormalPointerEXT( (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
}

FABRIC_EXT_EXPORT void klTexCoordPointerEXT(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Size & count,
  KL::Data pointer
){
  glTexCoordPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
}

FABRIC_EXT_EXPORT void klVertexPointerEXT(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Size & count,
  KL::Data pointer
){
  glVertexPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
}

FABRIC_EXT_EXPORT void klBeginVertexShaderEXT()
{
  glBeginVertexShaderEXT();
}

FABRIC_EXT_EXPORT KL::Size klBindLightParameterEXT(
  const KL::Size & light,
  const KL::Size & value
){
  GLuint result = glBindLightParameterEXT( (GLenum)light, (GLenum)value );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size klBindMaterialParameterEXT(
  const KL::Size & face,
  const KL::Size & value
){
  GLuint result = glBindMaterialParameterEXT( (GLenum)face, (GLenum)value );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size klBindParameterEXT(
  const KL::Size & value
){
  GLuint result = glBindParameterEXT( (GLenum)value );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size klBindTexGenParameterEXT(
  const KL::Size & unit,
  const KL::Size & coord,
  const KL::Size & value
){
  GLuint result = glBindTexGenParameterEXT( (GLenum)unit, (GLenum)coord, (GLenum)value );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size klBindTextureUnitParameterEXT(
  const KL::Size & unit,
  const KL::Size & value
){
  GLuint result = glBindTextureUnitParameterEXT( (GLenum)unit, (GLenum)value );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klBindVertexShaderEXT(
  const KL::Size & id
){
  glBindVertexShaderEXT( (GLuint)id );
}

FABRIC_EXT_EXPORT void klDeleteVertexShaderEXT(
  const KL::Size & id
){
  glDeleteVertexShaderEXT( (GLuint)id );
}

FABRIC_EXT_EXPORT void klDisableVariantClientStateEXT(
  const KL::Size & id
){
  glDisableVariantClientStateEXT( (GLuint)id );
}

FABRIC_EXT_EXPORT void klEnableVariantClientStateEXT(
  const KL::Size & id
){
  glEnableVariantClientStateEXT( (GLuint)id );
}

FABRIC_EXT_EXPORT void klEndVertexShaderEXT()
{
  glEndVertexShaderEXT();
}

FABRIC_EXT_EXPORT void klExtractComponentEXT(
  const KL::Size & res,
  const KL::Size & src,
  const KL::Size & num
){
  glExtractComponentEXT( (GLuint)res, (GLuint)src, (GLuint)num );
}

FABRIC_EXT_EXPORT KL::Size klGenSymbolsEXT(
  const KL::Size & dataType,
  const KL::Size & storageType,
  const KL::Size & range,
  const KL::Size & components
){
  GLuint result = glGenSymbolsEXT( (GLenum)dataType, (GLenum)storageType, (GLenum)range, (GLuint)components );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size klGenVertexShadersEXT(
  const KL::Size & range
){
  GLuint result = glGenVertexShadersEXT( (GLuint)range );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klGetInvariantBooleanvEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Boolean> & data
){
  glGetInvariantBooleanvEXT( (GLuint)id, (GLenum)value, (GLboolean*)&data[0] );
}

FABRIC_EXT_EXPORT void klGetInvariantFloatvEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Scalar> & data
){
  glGetInvariantFloatvEXT( (GLuint)id, (GLenum)value, (GLfloat*)&data[0] );
}

FABRIC_EXT_EXPORT void klGetInvariantIntegervEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Integer> & data
){
  glGetInvariantIntegervEXT( (GLuint)id, (GLenum)value, (GLint*)&data[0] );
}

FABRIC_EXT_EXPORT void klGetLocalConstantBooleanvEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Boolean> & data
){
  glGetLocalConstantBooleanvEXT( (GLuint)id, (GLenum)value, (GLboolean*)&data[0] );
}

FABRIC_EXT_EXPORT void klGetLocalConstantFloatvEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Scalar> & data
){
  glGetLocalConstantFloatvEXT( (GLuint)id, (GLenum)value, (GLfloat*)&data[0] );
}

FABRIC_EXT_EXPORT void klGetLocalConstantIntegervEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Integer> & data
){
  glGetLocalConstantIntegervEXT( (GLuint)id, (GLenum)value, (GLint*)&data[0] );
}

FABRIC_EXT_EXPORT void klGetVariantBooleanvEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Boolean> & data
){
  glGetVariantBooleanvEXT( (GLuint)id, (GLenum)value, (GLboolean*)&data[0] );
}

FABRIC_EXT_EXPORT void klGetVariantFloatvEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Scalar> & data
){
  glGetVariantFloatvEXT( (GLuint)id, (GLenum)value, (GLfloat*)&data[0] );
}

FABRIC_EXT_EXPORT void klGetVariantIntegervEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Integer> & data
){
  glGetVariantIntegervEXT( (GLuint)id, (GLenum)value, (GLint*)&data[0] );
}

FABRIC_EXT_EXPORT void klInsertComponentEXT(
  const KL::Size & res,
  const KL::Size & src,
  const KL::Size & num
){
  glInsertComponentEXT( (GLuint)res, (GLuint)src, (GLuint)num );
}

FABRIC_EXT_EXPORT KL::Boolean klIsVariantEnabledEXT(
  const KL::Size & id,
  const KL::Size & cap
){
  GLboolean result = glIsVariantEnabledEXT( (GLuint)id, (GLenum)cap );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT void klSetInvariantEXT(
  const KL::Size & id,
  const KL::Size & type,
  KL::Data addr
){
  glSetInvariantEXT( (GLuint)id, (GLenum)type, addr );
}

FABRIC_EXT_EXPORT void klSetLocalConstantEXT(
  const KL::Size & id,
  const KL::Size & type,
  KL::Data addr
){
  glSetLocalConstantEXT( (GLuint)id, (GLenum)type, addr );
}

FABRIC_EXT_EXPORT void klShaderOp1EXT(
  const KL::Size & op,
  const KL::Size & res,
  const KL::Size & arg1
){
  glShaderOp1EXT( (GLenum)op, (GLuint)res, (GLuint)arg1 );
}

FABRIC_EXT_EXPORT void klShaderOp2EXT(
  const KL::Size & op,
  const KL::Size & res,
  const KL::Size & arg1,
  const KL::Size & arg2
){
  glShaderOp2EXT( (GLenum)op, (GLuint)res, (GLuint)arg1, (GLuint)arg2 );
}

FABRIC_EXT_EXPORT void klShaderOp3EXT(
  const KL::Size & op,
  const KL::Size & res,
  const KL::Size & arg1,
  const KL::Size & arg2,
  const KL::Size & arg3
){
  glShaderOp3EXT( (GLenum)op, (GLuint)res, (GLuint)arg1, (GLuint)arg2, (GLuint)arg3 );
}

FABRIC_EXT_EXPORT void klSwizzleEXT(
  const KL::Size & res,
  const KL::Size & in,
  const KL::Size & outX,
  const KL::Size & outY,
  const KL::Size & outZ,
  const KL::Size & outW
){
  glSwizzleEXT( (GLuint)res, (GLuint)in, (GLenum)outX, (GLenum)outY, (GLenum)outZ, (GLenum)outW );
}

FABRIC_EXT_EXPORT void klVariantPointerEXT(
  const KL::Size & id,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data addr
){
  glVariantPointerEXT( (GLuint)id, (GLenum)type, (GLuint)stride, addr );
}

FABRIC_EXT_EXPORT void klVariantbvEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Byte> & addr
){
  glVariantbvEXT( (GLuint)id, (GLbyte*)&addr[0] );
}

FABRIC_EXT_EXPORT void klVariantdvEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Scalar> & addr
){
  glVariantdvEXT( (GLuint)id, (GLdouble*)&addr[0] );
}

FABRIC_EXT_EXPORT void klVariantfvEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Scalar> & addr
){
  glVariantfvEXT( (GLuint)id, (GLfloat*)&addr[0] );
}

FABRIC_EXT_EXPORT void klVariantivEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Integer> & addr
){
  glVariantivEXT( (GLuint)id, (GLint*)&addr[0] );
}

FABRIC_EXT_EXPORT void klVariantsvEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Integer> & addr
){
  glVariantsvEXT( (GLuint)id, (GLshort*)&addr[0] );
}

FABRIC_EXT_EXPORT void klVariantubvEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Size> & addr
){
  glVariantubvEXT( (GLuint)id, (GLubyte*)&addr[0] );
}

FABRIC_EXT_EXPORT void klVariantuivEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Size> & addr
){
  glVariantuivEXT( (GLuint)id, (GLuint*)&addr[0] );
}

FABRIC_EXT_EXPORT void klVariantusvEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Integer> & addr
){
  glVariantusvEXT( (GLuint)id, (GLushort*)&addr[0] );
}

FABRIC_EXT_EXPORT void klWriteMaskEXT(
  const KL::Size & res,
  const KL::Size & in,
  const KL::Size & outX,
  const KL::Size & outY,
  const KL::Size & outZ,
  const KL::Size & outW
){
  glWriteMaskEXT( (GLuint)res, (GLuint)in, (GLenum)outX, (GLenum)outY, (GLenum)outZ, (GLenum)outW );
}

FABRIC_EXT_EXPORT void klVertexWeightPointerEXT(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  glVertexWeightPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
}

FABRIC_EXT_EXPORT void klVertexWeightfEXT(
  const KL::Scalar & weight
){
  glVertexWeightfEXT( (GLfloat)weight );
}

FABRIC_EXT_EXPORT void klVertexWeightfvEXT(
  KL::VariableArray<KL::Scalar> & weight
){
  glVertexWeightfvEXT( (GLfloat*)&weight[0] );
}

FABRIC_EXT_EXPORT KL::Size klBufferRegionEnabledEXT()
{
  GLuint result = glBufferRegionEnabledEXT();
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klDeleteBufferRegionEXT(
  const KL::Size & region
){
  glDeleteBufferRegionEXT( (GLenum)region );
}

FABRIC_EXT_EXPORT void klDrawBufferRegionEXT(
  const KL::Size & region,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Integer & xDest,
  const KL::Integer & yDest
){
  glDrawBufferRegionEXT( (GLuint)region, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)xDest, (GLint)yDest );
}

FABRIC_EXT_EXPORT KL::Size klNewBufferRegionEXT(
  const KL::Size & region
){
  GLuint result = glNewBufferRegionEXT( (GLenum)region );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void klReadBufferRegionEXT(
  const KL::Size & region,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  glReadBufferRegionEXT( (GLuint)region, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
}

FABRIC_EXT_EXPORT void klXWaitGL()
{
  glXWaitGL();
}

FABRIC_EXT_EXPORT void klXWaitX()
{
  glXWaitX();
}

FABRIC_EXT_EXPORT KL::Boolean klxewIsSupported(
  const KL::String &name
){
  GLboolean result = glxewIsSupported( (const char*)name.data() );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Boolean klxewGetExtension(
  const KL::String &name
){
  GLboolean result = glxewGetExtension( (const char*)name.data() );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Integer kluBuild1DMipmapLevels(
  const KL::Size & target,
  const KL::Integer & internalFormat,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  const KL::Integer & level,
  const KL::Integer & base,
  const KL::Integer & max,
  KL::Data data
){
  GLint result = gluBuild1DMipmapLevels( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLenum)format, (GLenum)type, (GLint)level, (GLint)base, (GLint)max, data );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer kluBuild1DMipmaps(
  const KL::Size & target,
  const KL::Integer & internalFormat,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data data
){
  GLint result = gluBuild1DMipmaps( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLenum)format, (GLenum)type, data );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer kluBuild2DMipmapLevels(
  const KL::Size & target,
  const KL::Integer & internalFormat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  const KL::Integer & level,
  const KL::Integer & base,
  const KL::Integer & max,
  KL::Data data
){
  GLint result = gluBuild2DMipmapLevels( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, (GLint)level, (GLint)base, (GLint)max, data );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer kluBuild2DMipmaps(
  const KL::Size & target,
  const KL::Integer & internalFormat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data data
){
  GLint result = gluBuild2DMipmaps( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, data );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer kluBuild3DMipmapLevels(
  const KL::Size & target,
  const KL::Integer & internalFormat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Size & format,
  const KL::Size & type,
  const KL::Integer & level,
  const KL::Integer & base,
  const KL::Integer & max,
  KL::Data data
){
  GLint result = gluBuild3DMipmapLevels( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, (GLint)level, (GLint)base, (GLint)max, data );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer kluBuild3DMipmaps(
  const KL::Size & target,
  const KL::Integer & internalFormat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & depth,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data data
){
  GLint result = gluBuild3DMipmaps( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, data );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Boolean kluCheckExtension(
  const KL::VariableArray<KL::Size> & extName,
  const KL::VariableArray<KL::Size> & extString
){
  GLboolean result = gluCheckExtension( (const GLubyte*)&extName[0], (const GLubyte*)&extString[0] );
  return (KL::Boolean)result;
}

FABRIC_EXT_EXPORT KL::Size kluErrorString(
  const KL::Size & error
){
  const GLubyte* result = gluErrorString( (GLenum)error );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT KL::Size kluGetString(
  const KL::Size & name
){
  const GLubyte* result = gluGetString( (GLenum)name );
  return (KL::Size)result;
}

FABRIC_EXT_EXPORT void kluLookAt(
  const KL::Scalar & eyeX,
  const KL::Scalar & eyeY,
  const KL::Scalar & eyeZ,
  const KL::Scalar & centerX,
  const KL::Scalar & centerY,
  const KL::Scalar & centerZ,
  const KL::Scalar & upX,
  const KL::Scalar & upY,
  const KL::Scalar & upZ
){
  gluLookAt( (GLdouble)eyeX, (GLdouble)eyeY, (GLdouble)eyeZ, (GLdouble)centerX, (GLdouble)centerY, (GLdouble)centerZ, (GLdouble)upX, (GLdouble)upY, (GLdouble)upZ );
}

FABRIC_EXT_EXPORT void kluOrtho2D(
  const KL::Scalar & left,
  const KL::Scalar & right,
  const KL::Scalar & bottom,
  const KL::Scalar & top
){
  gluOrtho2D( (GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top );
}

FABRIC_EXT_EXPORT void kluPerspective(
  const KL::Scalar & fovy,
  const KL::Scalar & aspect,
  const KL::Scalar & zNear,
  const KL::Scalar & zFar
){
  gluPerspective( (GLdouble)fovy, (GLdouble)aspect, (GLdouble)zNear, (GLdouble)zFar );
}

FABRIC_EXT_EXPORT void kluPickMatrix(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & delX,
  const KL::Scalar & delY,
  KL::VariableArray<KL::Integer> & viewport
){
  gluPickMatrix( (GLdouble)x, (GLdouble)y, (GLdouble)delX, (GLdouble)delY, (GLint*)&viewport[0] );
}

FABRIC_EXT_EXPORT KL::Integer kluProject(
  const KL::Scalar & objX,
  const KL::Scalar & objY,
  const KL::Scalar & objZ,
  const KL::VariableArray<KL::Scalar> & model,
  const KL::VariableArray<KL::Scalar> & proj,
  const KL::VariableArray<KL::Integer> & view,
  KL::VariableArray<KL::Scalar> & winX,
  KL::VariableArray<KL::Scalar> & winY,
  KL::VariableArray<KL::Scalar> & winZ
){
  GLint result = gluProject( (GLdouble)objX, (GLdouble)objY, (GLdouble)objZ, (const GLdouble*)&model[0], (const GLdouble*)&proj[0], (const GLint*)&view[0], (GLdouble*)&winX[0], (GLdouble*)&winY[0], (GLdouble*)&winZ[0] );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer kluScaleImage(
  const KL::Size & format,
  const KL::Size & wIn,
  const KL::Size & hIn,
  const KL::Size & typeIn,
  KL::Data dataIn,
  const KL::Size & wOut,
  const KL::Size & hOut,
  const KL::Size & typeOut,
  KL::Data dataOut
){
  GLint result = gluScaleImage( (GLenum)format, (GLsizei)wIn, (GLsizei)hIn, (GLenum)typeIn, dataIn, (GLsizei)wOut, (GLsizei)hOut, (GLenum)typeOut, dataOut );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer kluUnProject(
  const KL::Scalar & winX,
  const KL::Scalar & winY,
  const KL::Scalar & winZ,
  const KL::VariableArray<KL::Scalar> & model,
  const KL::VariableArray<KL::Scalar> & proj,
  const KL::VariableArray<KL::Integer> & view,
  KL::VariableArray<KL::Scalar> & objX,
  KL::VariableArray<KL::Scalar> & objY,
  KL::VariableArray<KL::Scalar> & objZ
){
  GLint result = gluUnProject( (GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, (const GLdouble*)&model[0], (const GLdouble*)&proj[0], (const GLint*)&view[0], (GLdouble*)&objX[0], (GLdouble*)&objY[0], (GLdouble*)&objZ[0] );
  return (KL::Integer)result;
}

FABRIC_EXT_EXPORT KL::Integer kluUnProject4(
  const KL::Scalar & winX,
  const KL::Scalar & winY,
  const KL::Scalar & winZ,
  const KL::Scalar & clipW,
  const KL::VariableArray<KL::Scalar> & model,
  const KL::VariableArray<KL::Scalar> & proj,
  const KL::VariableArray<KL::Integer> & view,
  const KL::Scalar & nearVal,
  const KL::Scalar & farVal,
  KL::VariableArray<KL::Scalar> & objX,
  KL::VariableArray<KL::Scalar> & objY,
  KL::VariableArray<KL::Scalar> & objZ,
  KL::VariableArray<KL::Scalar> & objW
){
  GLint result = gluUnProject4( (GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, (GLdouble)clipW, (const GLdouble*)&model[0], (const GLdouble*)&proj[0], (const GLint*)&view[0], (GLdouble)nearVal, (GLdouble)farVal, (GLdouble*)&objX[0], (GLdouble*)&objY[0], (GLdouble*)&objZ[0], (GLdouble*)&objW[0] );
  return (KL::Integer)result;
}

