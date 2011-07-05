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
  try
  {
    glAccum( (GLenum)op, (GLfloat)value );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glAccum( %d, %f )", (int)op, (float)value);
  }
}

FABRIC_EXT_EXPORT void klAlphaFunc(
  const KL::Size & func,
  const KL::Scalar & ref
){
  try
  {
    glAlphaFunc( (GLenum)func, (GLclampf)ref );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glAlphaFunc( %d, %f )", (int)func, (float)ref);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klAreTexturesResident(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & textures,
  KL::VariableArray<KL::Boolean> & residences
){
  try
  {
    GLboolean result = glAreTexturesResident( (GLsizei)n, (const GLuint*)&textures[0], (GLboolean*)&residences[0] );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glAreTexturesResident( %d, GLuint*, GLboolean* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klArrayElement(
  const KL::Integer & i
){
  try
  {
    glArrayElement( (GLint)i );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glArrayElement( %d )", (int)i);
  }
}

FABRIC_EXT_EXPORT void klBegin(
  const KL::Size & mode
){
  try
  {
    glBegin( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBegin( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klBindTexture(
  const KL::Size & target,
  const KL::Size & texture
){
  try
  {
    glBindTexture( (GLenum)target, (GLuint)texture );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindTexture( %d, 0x%04X )", (int)target, (unsigned)texture);
  }
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
  try
  {
    glBitmap( (GLsizei)width, (GLsizei)height, (GLfloat)xorig, (GLfloat)yorig, (GLfloat)xmove, (GLfloat)ymove, (const GLubyte*)&bitmap[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBitmap( %d, %d, %f, %f, %f, %f, GLubyte* )", (int)width, (int)height, (float)xorig, (float)yorig, (float)xmove, (float)ymove);
  }
}

FABRIC_EXT_EXPORT void klBlendFunc(
  const KL::Size & sfactor,
  const KL::Size & dfactor
){
  try
  {
    glBlendFunc( (GLenum)sfactor, (GLenum)dfactor );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBlendFunc( %d, %d )", (int)sfactor, (int)dfactor);
  }
}

FABRIC_EXT_EXPORT void klCallList(
  const KL::Size & list
){
  try
  {
    glCallList( (GLuint)list );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCallList( 0x%04X )", (unsigned)list);
  }
}

FABRIC_EXT_EXPORT void klCallLists(
  const KL::Size & n,
  const KL::Size & type,
  KL::Data lists
){
  try
  {
    glCallLists( (GLsizei)n, (GLenum)type, lists );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCallLists( %d, %d, GLvoid* )", (int)n, (int)type);
  }
}

FABRIC_EXT_EXPORT void klClear(
  const KL::Size & mask
){
  try
  {
    glClear( (GLbitfield)mask );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClear( 0x%04X )", (unsigned)mask);
  }
}

FABRIC_EXT_EXPORT void klClearAccum(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue,
  const KL::Scalar & alpha
){
  try
  {
    glClearAccum( (GLfloat)red, (GLfloat)green, (GLfloat)blue, (GLfloat)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClearAccum( %f, %f, %f, %f )", (float)red, (float)green, (float)blue, (float)alpha);
  }
}

FABRIC_EXT_EXPORT void klClearColor(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue,
  const KL::Scalar & alpha
){
  try
  {
    glClearColor( (GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClearColor( %f, %f, %f, %f )", (float)red, (float)green, (float)blue, (float)alpha);
  }
}

FABRIC_EXT_EXPORT void klClearDepth(
  const KL::Scalar & depth
){
  try
  {
    glClearDepth( (GLclampd)depth );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClearDepth( %f )", (float)depth);
  }
}

FABRIC_EXT_EXPORT void klClearIndex(
  const KL::Scalar & c
){
  try
  {
    glClearIndex( (GLfloat)c );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClearIndex( %f )", (float)c);
  }
}

FABRIC_EXT_EXPORT void klClearStencil(
  const KL::Integer & s
){
  try
  {
    glClearStencil( (GLint)s );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClearStencil( %d )", (int)s);
  }
}

FABRIC_EXT_EXPORT void klClipPlane(
  const KL::Size & plane,
  const KL::VariableArray<KL::Scalar> & equation
){
  try
  {
    glClipPlane( (GLenum)plane, (const GLdouble*)&equation[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClipPlane( %d, GLdouble* )", (int)plane);
  }
}

FABRIC_EXT_EXPORT void klColor3b(
  const KL::Byte & red,
  const KL::Byte & green,
  const KL::Byte & blue
){
  try
  {
    glColor3b( (GLbyte)red, (GLbyte)green, (GLbyte)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3b( %d, %d, %d )", (int)red, (int)green, (int)blue);
  }
}

FABRIC_EXT_EXPORT void klColor3bv(
  const KL::VariableArray<KL::Byte> & v
){
  try
  {
    glColor3bv( (const GLbyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3bv( GLbyte* )");
  }
}

FABRIC_EXT_EXPORT void klColor3d(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue
){
  try
  {
    glColor3d( (GLdouble)red, (GLdouble)green, (GLdouble)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3d( %f, %f, %f )", (float)red, (float)green, (float)blue);
  }
}

FABRIC_EXT_EXPORT void klColor3dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glColor3dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klColor3f(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue
){
  try
  {
    glColor3f( (GLfloat)red, (GLfloat)green, (GLfloat)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3f( %f, %f, %f )", (float)red, (float)green, (float)blue);
  }
}

FABRIC_EXT_EXPORT void klColor3fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glColor3fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klColor3i(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  try
  {
    glColor3i( (GLint)red, (GLint)green, (GLint)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3i( %d, %d, %d )", (int)red, (int)green, (int)blue);
  }
}

FABRIC_EXT_EXPORT void klColor3iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glColor3iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klColor3s(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  try
  {
    glColor3s( (GLshort)red, (GLshort)green, (GLshort)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3s( %d, %d, %d )", (int)red, (int)green, (int)blue);
  }
}

FABRIC_EXT_EXPORT void klColor3sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glColor3sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klColor3ub(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue
){
  try
  {
    glColor3ub( (GLubyte)red, (GLubyte)green, (GLubyte)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3ub( 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue);
  }
}

FABRIC_EXT_EXPORT void klColor3ubv(
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glColor3ubv( (const GLubyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3ubv( GLubyte* )");
  }
}

FABRIC_EXT_EXPORT void klColor3ui(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue
){
  try
  {
    glColor3ui( (GLuint)red, (GLuint)green, (GLuint)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3ui( 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue);
  }
}

FABRIC_EXT_EXPORT void klColor3uiv(
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glColor3uiv( (const GLuint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3uiv( GLuint* )");
  }
}

FABRIC_EXT_EXPORT void klColor3us(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  try
  {
    glColor3us( (GLushort)red, (GLushort)green, (GLushort)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3us( 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue);
  }
}

FABRIC_EXT_EXPORT void klColor3usv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glColor3usv( (const GLushort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor3usv( GLushort* )");
  }
}

FABRIC_EXT_EXPORT void klColor4b(
  const KL::Byte & red,
  const KL::Byte & green,
  const KL::Byte & blue,
  const KL::Byte & alpha
){
  try
  {
    glColor4b( (GLbyte)red, (GLbyte)green, (GLbyte)blue, (GLbyte)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4b( %d, %d, %d, %d )", (int)red, (int)green, (int)blue, (int)alpha);
  }
}

FABRIC_EXT_EXPORT void klColor4bv(
  const KL::VariableArray<KL::Byte> & v
){
  try
  {
    glColor4bv( (const GLbyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4bv( GLbyte* )");
  }
}

FABRIC_EXT_EXPORT void klColor4d(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue,
  const KL::Scalar & alpha
){
  try
  {
    glColor4d( (GLdouble)red, (GLdouble)green, (GLdouble)blue, (GLdouble)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4d( %f, %f, %f, %f )", (float)red, (float)green, (float)blue, (float)alpha);
  }
}

FABRIC_EXT_EXPORT void klColor4dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glColor4dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klColor4f(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue,
  const KL::Scalar & alpha
){
  try
  {
    glColor4f( (GLfloat)red, (GLfloat)green, (GLfloat)blue, (GLfloat)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4f( %f, %f, %f, %f )", (float)red, (float)green, (float)blue, (float)alpha);
  }
}

FABRIC_EXT_EXPORT void klColor4fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glColor4fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klColor4i(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue,
  const KL::Integer & alpha
){
  try
  {
    glColor4i( (GLint)red, (GLint)green, (GLint)blue, (GLint)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4i( %d, %d, %d, %d )", (int)red, (int)green, (int)blue, (int)alpha);
  }
}

FABRIC_EXT_EXPORT void klColor4iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glColor4iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klColor4s(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue,
  const KL::Integer & alpha
){
  try
  {
    glColor4s( (GLshort)red, (GLshort)green, (GLshort)blue, (GLshort)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4s( %d, %d, %d, %d )", (int)red, (int)green, (int)blue, (int)alpha);
  }
}

FABRIC_EXT_EXPORT void klColor4sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glColor4sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klColor4ub(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue,
  const KL::Size & alpha
){
  try
  {
    glColor4ub( (GLubyte)red, (GLubyte)green, (GLubyte)blue, (GLubyte)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4ub( 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue, (unsigned)alpha);
  }
}

FABRIC_EXT_EXPORT void klColor4ubv(
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glColor4ubv( (const GLubyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4ubv( GLubyte* )");
  }
}

FABRIC_EXT_EXPORT void klColor4ui(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue,
  const KL::Size & alpha
){
  try
  {
    glColor4ui( (GLuint)red, (GLuint)green, (GLuint)blue, (GLuint)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4ui( 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue, (unsigned)alpha);
  }
}

FABRIC_EXT_EXPORT void klColor4uiv(
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glColor4uiv( (const GLuint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4uiv( GLuint* )");
  }
}

FABRIC_EXT_EXPORT void klColor4us(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue,
  const KL::Integer & alpha
){
  try
  {
    glColor4us( (GLushort)red, (GLushort)green, (GLushort)blue, (GLushort)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4us( 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue, (unsigned)alpha);
  }
}

FABRIC_EXT_EXPORT void klColor4usv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glColor4usv( (const GLushort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColor4usv( GLushort* )");
  }
}

FABRIC_EXT_EXPORT void klColorMask(
  const KL::Boolean & red,
  const KL::Boolean & green,
  const KL::Boolean & blue,
  const KL::Boolean & alpha
){
  try
  {
    glColorMask( (GLboolean)red, (GLboolean)green, (GLboolean)blue, (GLboolean)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColorMask( %b, %b, %b, %b )", (bool)red, (bool)green, (bool)blue, (bool)alpha);
  }
}

FABRIC_EXT_EXPORT void klColorMaterial(
  const KL::Size & face,
  const KL::Size & mode
){
  try
  {
    glColorMaterial( (GLenum)face, (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColorMaterial( %d, %d )", (int)face, (int)mode);
  }
}

FABRIC_EXT_EXPORT void klColorPointer(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glColorPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColorPointer( %d, %d, %d, GLvoid* )", (int)size, (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klCopyPixels(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & type
){
  try
  {
    glCopyPixels( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLenum)type );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyPixels( %d, %d, %d, %d, %d )", (int)x, (int)y, (int)width, (int)height, (int)type);
  }
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
  try
  {
    glCopyTexImage1D( (GLenum)target, (GLint)level, (GLenum)internalFormat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTexImage1D( %d, %d, %d, %d, %d, %d, %d )", (int)target, (int)level, (int)internalFormat, (int)x, (int)y, (int)width, (int)border);
  }
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
  try
  {
    glCopyTexImage2D( (GLenum)target, (GLint)level, (GLenum)internalFormat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTexImage2D( %d, %d, %d, %d, %d, %d, %d, %d )", (int)target, (int)level, (int)internalFormat, (int)x, (int)y, (int)width, (int)height, (int)border);
  }
}

FABRIC_EXT_EXPORT void klCopyTexSubImage1D(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  try
  {
    glCopyTexSubImage1D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTexSubImage1D( %d, %d, %d, %d, %d, %d )", (int)target, (int)level, (int)xoffset, (int)x, (int)y, (int)width);
  }
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
  try
  {
    glCopyTexSubImage2D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTexSubImage2D( %d, %d, %d, %d, %d, %d, %d, %d )", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)x, (int)y, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klCullFace(
  const KL::Size & mode
){
  try
  {
    glCullFace( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCullFace( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klDeleteLists(
  const KL::Size & list,
  const KL::Size & range
){
  try
  {
    glDeleteLists( (GLuint)list, (GLsizei)range );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteLists( 0x%04X, %d )", (unsigned)list, (int)range);
  }
}

FABRIC_EXT_EXPORT void klDeleteTextures(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & textures
){
  try
  {
    glDeleteTextures( (GLsizei)n, (const GLuint*)&textures[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteTextures( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klDepthFunc(
  const KL::Size & func
){
  try
  {
    glDepthFunc( (GLenum)func );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDepthFunc( %d )", (int)func);
  }
}

FABRIC_EXT_EXPORT void klDepthMask(
  const KL::Boolean & flag
){
  try
  {
    glDepthMask( (GLboolean)flag );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDepthMask( %b )", (bool)flag);
  }
}

FABRIC_EXT_EXPORT void klDepthRange(
  const KL::Scalar & zNear,
  const KL::Scalar & zFar
){
  try
  {
    glDepthRange( (GLclampd)zNear, (GLclampd)zFar );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDepthRange( %f, %f )", (float)zNear, (float)zFar);
  }
}

FABRIC_EXT_EXPORT void klDisable(
  const KL::Size & cap
){
  try
  {
    glDisable( (GLenum)cap );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDisable( %d )", (int)cap);
  }
}

FABRIC_EXT_EXPORT void klDisableClientState(
  const KL::Size & array
){
  try
  {
    glDisableClientState( (GLenum)array );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDisableClientState( %d )", (int)array);
  }
}

FABRIC_EXT_EXPORT void klDrawArrays(
  const KL::Size & mode,
  const KL::Integer & first,
  const KL::Size & count
){
  try
  {
    glDrawArrays( (GLenum)mode, (GLint)first, (GLsizei)count );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawArrays( %d, %d, %d )", (int)mode, (int)first, (int)count);
  }
}

FABRIC_EXT_EXPORT void klDrawBuffer(
  const KL::Size & mode
){
  try
  {
    glDrawBuffer( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawBuffer( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klDrawElements(
  const KL::Size & mode,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices
){
  try
  {
    glDrawElements( (GLenum)mode, (GLsizei)count, (GLenum)type, indices );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawElements( %d, %d, %d, GLvoid* )", (int)mode, (int)count, (int)type);
  }
}

FABRIC_EXT_EXPORT void klDrawPixels(
  const KL::Size & width,
  const KL::Size & height,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  try
  {
    glDrawPixels( (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawPixels( %d, %d, %d, %d, GLvoid* )", (int)width, (int)height, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klEdgeFlag(
  const KL::Boolean & flag
){
  try
  {
    glEdgeFlag( (GLboolean)flag );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEdgeFlag( %b )", (bool)flag);
  }
}

FABRIC_EXT_EXPORT void klEdgeFlagPointer(
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glEdgeFlagPointer( (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEdgeFlagPointer( %d, GLvoid* )", (int)stride);
  }
}

FABRIC_EXT_EXPORT void klEdgeFlagv(
  const KL::VariableArray<KL::Boolean> & flag
){
  try
  {
    glEdgeFlagv( (const GLboolean*)&flag[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEdgeFlagv( GLboolean* )");
  }
}

FABRIC_EXT_EXPORT void klEnable(
  const KL::Size & cap
){
  try
  {
    glEnable( (GLenum)cap );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEnable( %d )", (int)cap);
  }
}

FABRIC_EXT_EXPORT void klEnableClientState(
  const KL::Size & array
){
  try
  {
    glEnableClientState( (GLenum)array );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEnableClientState( %d )", (int)array);
  }
}

FABRIC_EXT_EXPORT void klEnd()
{
  try
  {
    glEnd();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEnd(  )");
  }
}

FABRIC_EXT_EXPORT void klEndList()
{
  try
  {
    glEndList();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEndList(  )");
  }
}

FABRIC_EXT_EXPORT void klEvalCoord1d(
  const KL::Scalar & u
){
  try
  {
    glEvalCoord1d( (GLdouble)u );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEvalCoord1d( %f )", (float)u);
  }
}

FABRIC_EXT_EXPORT void klEvalCoord1dv(
  const KL::VariableArray<KL::Scalar> & u
){
  try
  {
    glEvalCoord1dv( (const GLdouble*)&u[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEvalCoord1dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klEvalCoord1f(
  const KL::Scalar & u
){
  try
  {
    glEvalCoord1f( (GLfloat)u );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEvalCoord1f( %f )", (float)u);
  }
}

FABRIC_EXT_EXPORT void klEvalCoord1fv(
  const KL::VariableArray<KL::Scalar> & u
){
  try
  {
    glEvalCoord1fv( (const GLfloat*)&u[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEvalCoord1fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klEvalCoord2d(
  const KL::Scalar & u,
  const KL::Scalar & v
){
  try
  {
    glEvalCoord2d( (GLdouble)u, (GLdouble)v );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEvalCoord2d( %f, %f )", (float)u, (float)v);
  }
}

FABRIC_EXT_EXPORT void klEvalCoord2dv(
  const KL::VariableArray<KL::Scalar> & u
){
  try
  {
    glEvalCoord2dv( (const GLdouble*)&u[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEvalCoord2dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klEvalCoord2f(
  const KL::Scalar & u,
  const KL::Scalar & v
){
  try
  {
    glEvalCoord2f( (GLfloat)u, (GLfloat)v );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEvalCoord2f( %f, %f )", (float)u, (float)v);
  }
}

FABRIC_EXT_EXPORT void klEvalCoord2fv(
  const KL::VariableArray<KL::Scalar> & u
){
  try
  {
    glEvalCoord2fv( (const GLfloat*)&u[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEvalCoord2fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klEvalMesh1(
  const KL::Size & mode,
  const KL::Integer & i1,
  const KL::Integer & i2
){
  try
  {
    glEvalMesh1( (GLenum)mode, (GLint)i1, (GLint)i2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEvalMesh1( %d, %d, %d )", (int)mode, (int)i1, (int)i2);
  }
}

FABRIC_EXT_EXPORT void klEvalMesh2(
  const KL::Size & mode,
  const KL::Integer & i1,
  const KL::Integer & i2,
  const KL::Integer & j1,
  const KL::Integer & j2
){
  try
  {
    glEvalMesh2( (GLenum)mode, (GLint)i1, (GLint)i2, (GLint)j1, (GLint)j2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEvalMesh2( %d, %d, %d, %d, %d )", (int)mode, (int)i1, (int)i2, (int)j1, (int)j2);
  }
}

FABRIC_EXT_EXPORT void klEvalPoint1(
  const KL::Integer & i
){
  try
  {
    glEvalPoint1( (GLint)i );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEvalPoint1( %d )", (int)i);
  }
}

FABRIC_EXT_EXPORT void klEvalPoint2(
  const KL::Integer & i,
  const KL::Integer & j
){
  try
  {
    glEvalPoint2( (GLint)i, (GLint)j );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEvalPoint2( %d, %d )", (int)i, (int)j);
  }
}

FABRIC_EXT_EXPORT void klFeedbackBuffer(
  const KL::Size & size,
  const KL::Size & type,
  KL::VariableArray<KL::Scalar> & buffer
){
  try
  {
    glFeedbackBuffer( (GLsizei)size, (GLenum)type, (GLfloat*)&buffer[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFeedbackBuffer( %d, %d, GLfloat* )", (int)size, (int)type);
  }
}

FABRIC_EXT_EXPORT void klFinish()
{
  try
  {
    glFinish();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFinish(  )");
  }
}

FABRIC_EXT_EXPORT void klFlush()
{
  try
  {
    glFlush();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFlush(  )");
  }
}

FABRIC_EXT_EXPORT void klFogf(
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glFogf( (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogf( %d, %f )", (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klFogfv(
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glFogfv( (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogfv( %d, GLfloat* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klFogi(
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glFogi( (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogi( %d, %d )", (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klFogiv(
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glFogiv( (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogiv( %d, GLint* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klFrontFace(
  const KL::Size & mode
){
  try
  {
    glFrontFace( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFrontFace( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klFrustum(
  const KL::Scalar & left,
  const KL::Scalar & right,
  const KL::Scalar & bottom,
  const KL::Scalar & top,
  const KL::Scalar & zNear,
  const KL::Scalar & zFar
){
  try
  {
    glFrustum( (GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top, (GLdouble)zNear, (GLdouble)zFar );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFrustum( %f, %f, %f, %f, %f, %f )", (float)left, (float)right, (float)bottom, (float)top, (float)zNear, (float)zFar);
  }
}

FABRIC_EXT_EXPORT KL::Size klGenLists(
  const KL::Size & range
){
  try
  {
    GLuint result = glGenLists( (GLsizei)range );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenLists( %d )", (int)range);
  }
}

FABRIC_EXT_EXPORT void klGenTextures(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & textures
){
  try
  {
    glGenTextures( (GLsizei)n, (GLuint*)&textures[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenTextures( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klGetBooleanv(
  const KL::Size & pname,
  KL::VariableArray<KL::Boolean> & params
){
  try
  {
    glGetBooleanv( (GLenum)pname, (GLboolean*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetBooleanv( %d, GLboolean* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetClipPlane(
  const KL::Size & plane,
  KL::VariableArray<KL::Scalar> & equation
){
  try
  {
    glGetClipPlane( (GLenum)plane, (GLdouble*)&equation[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetClipPlane( %d, GLdouble* )", (int)plane);
  }
}

FABRIC_EXT_EXPORT void klGetDoublev(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetDoublev( (GLenum)pname, (GLdouble*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetDoublev( %d, GLdouble* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT KL::Size klGetError()
{
  try
  {
    GLenum result = glGetError();
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetError(  )");
  }
}

FABRIC_EXT_EXPORT void klGetFloatv(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetFloatv( (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetFloatv( %d, GLfloat* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetIntegerv(
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetIntegerv( (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetIntegerv( %d, GLint* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetLightfv(
  const KL::Size & light,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetLightfv( (GLenum)light, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetLightfv( %d, %d, GLfloat* )", (int)light, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetLightiv(
  const KL::Size & light,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetLightiv( (GLenum)light, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetLightiv( %d, %d, GLint* )", (int)light, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMapdv(
  const KL::Size & target,
  const KL::Size & query,
  KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glGetMapdv( (GLenum)target, (GLenum)query, (GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMapdv( %d, %d, GLdouble* )", (int)target, (int)query);
  }
}

FABRIC_EXT_EXPORT void klGetMapfv(
  const KL::Size & target,
  const KL::Size & query,
  KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glGetMapfv( (GLenum)target, (GLenum)query, (GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMapfv( %d, %d, GLfloat* )", (int)target, (int)query);
  }
}

FABRIC_EXT_EXPORT void klGetMapiv(
  const KL::Size & target,
  const KL::Size & query,
  KL::VariableArray<KL::Integer> & v
){
  try
  {
    glGetMapiv( (GLenum)target, (GLenum)query, (GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMapiv( %d, %d, GLint* )", (int)target, (int)query);
  }
}

FABRIC_EXT_EXPORT void klGetMaterialfv(
  const KL::Size & face,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetMaterialfv( (GLenum)face, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMaterialfv( %d, %d, GLfloat* )", (int)face, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMaterialiv(
  const KL::Size & face,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetMaterialiv( (GLenum)face, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMaterialiv( %d, %d, GLint* )", (int)face, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetPixelMapfv(
  const KL::Size & map,
  KL::VariableArray<KL::Scalar> & values
){
  try
  {
    glGetPixelMapfv( (GLenum)map, (GLfloat*)&values[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetPixelMapfv( %d, GLfloat* )", (int)map);
  }
}

FABRIC_EXT_EXPORT void klGetPixelMapuiv(
  const KL::Size & map,
  KL::VariableArray<KL::Size> & values
){
  try
  {
    glGetPixelMapuiv( (GLenum)map, (GLuint*)&values[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetPixelMapuiv( %d, GLuint* )", (int)map);
  }
}

FABRIC_EXT_EXPORT void klGetPixelMapusv(
  const KL::Size & map,
  KL::VariableArray<KL::Integer> & values
){
  try
  {
    glGetPixelMapusv( (GLenum)map, (GLushort*)&values[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetPixelMapusv( %d, GLushort* )", (int)map);
  }
}

FABRIC_EXT_EXPORT void klGetPolygonStipple(
  KL::VariableArray<KL::Size> & mask
){
  try
  {
    glGetPolygonStipple( (GLubyte*)&mask[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetPolygonStipple( GLubyte* )");
  }
}

FABRIC_EXT_EXPORT KL::Size klGetString(
  const KL::Size & name
){
  try
  {
    const GLubyte* result = glGetString( (GLenum)name );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetString( %d )", (int)name);
  }
}

FABRIC_EXT_EXPORT void klGetTexEnvfv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetTexEnvfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexEnvfv( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTexEnviv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetTexEnviv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexEnviv( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTexGendv(
  const KL::Size & coord,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetTexGendv( (GLenum)coord, (GLenum)pname, (GLdouble*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexGendv( %d, %d, GLdouble* )", (int)coord, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTexGenfv(
  const KL::Size & coord,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetTexGenfv( (GLenum)coord, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexGenfv( %d, %d, GLfloat* )", (int)coord, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTexGeniv(
  const KL::Size & coord,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetTexGeniv( (GLenum)coord, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexGeniv( %d, %d, GLint* )", (int)coord, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTexImage(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  try
  {
    glGetTexImage( (GLenum)target, (GLint)level, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexImage( %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klGetTexLevelParameterfv(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetTexLevelParameterfv( (GLenum)target, (GLint)level, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexLevelParameterfv( %d, %d, %d, GLfloat* )", (int)target, (int)level, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTexLevelParameteriv(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetTexLevelParameteriv( (GLenum)target, (GLint)level, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexLevelParameteriv( %d, %d, %d, GLint* )", (int)target, (int)level, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTexParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetTexParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexParameterfv( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTexParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetTexParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexParameteriv( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klHint(
  const KL::Size & target,
  const KL::Size & mode
){
  try
  {
    glHint( (GLenum)target, (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glHint( %d, %d )", (int)target, (int)mode);
  }
}

FABRIC_EXT_EXPORT void klIndexMask(
  const KL::Size & mask
){
  try
  {
    glIndexMask( (GLuint)mask );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexMask( 0x%04X )", (unsigned)mask);
  }
}

FABRIC_EXT_EXPORT void klIndexPointer(
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glIndexPointer( (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexPointer( %d, %d, GLvoid* )", (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klIndexd(
  const KL::Scalar & c
){
  try
  {
    glIndexd( (GLdouble)c );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexd( %f )", (float)c);
  }
}

FABRIC_EXT_EXPORT void klIndexdv(
  const KL::VariableArray<KL::Scalar> & c
){
  try
  {
    glIndexdv( (const GLdouble*)&c[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexdv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klIndexf(
  const KL::Scalar & c
){
  try
  {
    glIndexf( (GLfloat)c );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexf( %f )", (float)c);
  }
}

FABRIC_EXT_EXPORT void klIndexfv(
  const KL::VariableArray<KL::Scalar> & c
){
  try
  {
    glIndexfv( (const GLfloat*)&c[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexfv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klIndexi(
  const KL::Integer & c
){
  try
  {
    glIndexi( (GLint)c );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexi( %d )", (int)c);
  }
}

FABRIC_EXT_EXPORT void klIndexiv(
  const KL::VariableArray<KL::Integer> & c
){
  try
  {
    glIndexiv( (const GLint*)&c[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexiv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klIndexs(
  const KL::Integer & c
){
  try
  {
    glIndexs( (GLshort)c );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexs( %d )", (int)c);
  }
}

FABRIC_EXT_EXPORT void klIndexsv(
  const KL::VariableArray<KL::Integer> & c
){
  try
  {
    glIndexsv( (const GLshort*)&c[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexsv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klIndexub(
  const KL::Size & c
){
  try
  {
    glIndexub( (GLubyte)c );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexub( 0x%04X )", (unsigned)c);
  }
}

FABRIC_EXT_EXPORT void klIndexubv(
  const KL::VariableArray<KL::Size> & c
){
  try
  {
    glIndexubv( (const GLubyte*)&c[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexubv( GLubyte* )");
  }
}

FABRIC_EXT_EXPORT void klInitNames()
{
  try
  {
    glInitNames();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glInitNames(  )");
  }
}

FABRIC_EXT_EXPORT void klInterleavedArrays(
  const KL::Size & format,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glInterleavedArrays( (GLenum)format, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glInterleavedArrays( %d, %d, GLvoid* )", (int)format, (int)stride);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsEnabled(
  const KL::Size & cap
){
  try
  {
    GLboolean result = glIsEnabled( (GLenum)cap );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsEnabled( %d )", (int)cap);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsList(
  const KL::Size & list
){
  try
  {
    GLboolean result = glIsList( (GLuint)list );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsList( 0x%04X )", (unsigned)list);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsTexture(
  const KL::Size & texture
){
  try
  {
    GLboolean result = glIsTexture( (GLuint)texture );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsTexture( 0x%04X )", (unsigned)texture);
  }
}

FABRIC_EXT_EXPORT void klLightModelf(
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glLightModelf( (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLightModelf( %d, %f )", (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klLightModelfv(
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glLightModelfv( (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLightModelfv( %d, GLfloat* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klLightModeli(
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glLightModeli( (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLightModeli( %d, %d )", (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klLightModeliv(
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glLightModeliv( (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLightModeliv( %d, GLint* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klLightf(
  const KL::Size & light,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glLightf( (GLenum)light, (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLightf( %d, %d, %f )", (int)light, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klLightfv(
  const KL::Size & light,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glLightfv( (GLenum)light, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLightfv( %d, %d, GLfloat* )", (int)light, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klLighti(
  const KL::Size & light,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glLighti( (GLenum)light, (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLighti( %d, %d, %d )", (int)light, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klLightiv(
  const KL::Size & light,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glLightiv( (GLenum)light, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLightiv( %d, %d, GLint* )", (int)light, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klLineStipple(
  const KL::Integer & factor,
  const KL::Integer & pattern
){
  try
  {
    glLineStipple( (GLint)factor, (GLushort)pattern );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLineStipple( %d, 0x%04X )", (int)factor, (unsigned)pattern);
  }
}

FABRIC_EXT_EXPORT void klLineWidth(
  const KL::Scalar & width
){
  try
  {
    glLineWidth( (GLfloat)width );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLineWidth( %f )", (float)width);
  }
}

FABRIC_EXT_EXPORT void klListBase(
  const KL::Size & base
){
  try
  {
    glListBase( (GLuint)base );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glListBase( 0x%04X )", (unsigned)base);
  }
}

FABRIC_EXT_EXPORT void klLoadIdentity()
{
  try
  {
    glLoadIdentity();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLoadIdentity(  )");
  }
}

FABRIC_EXT_EXPORT void klLoadMatrixd(
  const KL::VariableArray<KL::Scalar> & m
){
  try
  {
    glLoadMatrixd( (const GLdouble*)&m[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLoadMatrixd( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klLoadMatrixf(
  const KL::VariableArray<KL::Scalar> & m
){
  try
  {
    glLoadMatrixf( (const GLfloat*)&m[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLoadMatrixf( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klLoadName(
  const KL::Size & name
){
  try
  {
    glLoadName( (GLuint)name );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLoadName( 0x%04X )", (unsigned)name);
  }
}

FABRIC_EXT_EXPORT void klLogicOp(
  const KL::Size & opcode
){
  try
  {
    glLogicOp( (GLenum)opcode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLogicOp( %d )", (int)opcode);
  }
}

FABRIC_EXT_EXPORT void klMap1d(
  const KL::Size & target,
  const KL::Scalar & u1,
  const KL::Scalar & u2,
  const KL::Integer & stride,
  const KL::Integer & order,
  const KL::VariableArray<KL::Scalar> & points
){
  try
  {
    glMap1d( (GLenum)target, (GLdouble)u1, (GLdouble)u2, (GLint)stride, (GLint)order, (const GLdouble*)&points[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMap1d( %d, %f, %f, %d, %d, GLdouble* )", (int)target, (float)u1, (float)u2, (int)stride, (int)order);
  }
}

FABRIC_EXT_EXPORT void klMap1f(
  const KL::Size & target,
  const KL::Scalar & u1,
  const KL::Scalar & u2,
  const KL::Integer & stride,
  const KL::Integer & order,
  const KL::VariableArray<KL::Scalar> & points
){
  try
  {
    glMap1f( (GLenum)target, (GLfloat)u1, (GLfloat)u2, (GLint)stride, (GLint)order, (const GLfloat*)&points[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMap1f( %d, %f, %f, %d, %d, GLfloat* )", (int)target, (float)u1, (float)u2, (int)stride, (int)order);
  }
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
  try
  {
    glMap2d( (GLenum)target, (GLdouble)u1, (GLdouble)u2, (GLint)ustride, (GLint)uorder, (GLdouble)v1, (GLdouble)v2, (GLint)vstride, (GLint)vorder, (const GLdouble*)&points[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMap2d( %d, %f, %f, %d, %d, %f, %f, %d, %d, GLdouble* )", (int)target, (float)u1, (float)u2, (int)ustride, (int)uorder, (float)v1, (float)v2, (int)vstride, (int)vorder);
  }
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
  try
  {
    glMap2f( (GLenum)target, (GLfloat)u1, (GLfloat)u2, (GLint)ustride, (GLint)uorder, (GLfloat)v1, (GLfloat)v2, (GLint)vstride, (GLint)vorder, (const GLfloat*)&points[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMap2f( %d, %f, %f, %d, %d, %f, %f, %d, %d, GLfloat* )", (int)target, (float)u1, (float)u2, (int)ustride, (int)uorder, (float)v1, (float)v2, (int)vstride, (int)vorder);
  }
}

FABRIC_EXT_EXPORT void klMapGrid1d(
  const KL::Integer & un,
  const KL::Scalar & u1,
  const KL::Scalar & u2
){
  try
  {
    glMapGrid1d( (GLint)un, (GLdouble)u1, (GLdouble)u2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMapGrid1d( %d, %f, %f )", (int)un, (float)u1, (float)u2);
  }
}

FABRIC_EXT_EXPORT void klMapGrid1f(
  const KL::Integer & un,
  const KL::Scalar & u1,
  const KL::Scalar & u2
){
  try
  {
    glMapGrid1f( (GLint)un, (GLfloat)u1, (GLfloat)u2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMapGrid1f( %d, %f, %f )", (int)un, (float)u1, (float)u2);
  }
}

FABRIC_EXT_EXPORT void klMapGrid2d(
  const KL::Integer & un,
  const KL::Scalar & u1,
  const KL::Scalar & u2,
  const KL::Integer & vn,
  const KL::Scalar & v1,
  const KL::Scalar & v2
){
  try
  {
    glMapGrid2d( (GLint)un, (GLdouble)u1, (GLdouble)u2, (GLint)vn, (GLdouble)v1, (GLdouble)v2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMapGrid2d( %d, %f, %f, %d, %f, %f )", (int)un, (float)u1, (float)u2, (int)vn, (float)v1, (float)v2);
  }
}

FABRIC_EXT_EXPORT void klMapGrid2f(
  const KL::Integer & un,
  const KL::Scalar & u1,
  const KL::Scalar & u2,
  const KL::Integer & vn,
  const KL::Scalar & v1,
  const KL::Scalar & v2
){
  try
  {
    glMapGrid2f( (GLint)un, (GLfloat)u1, (GLfloat)u2, (GLint)vn, (GLfloat)v1, (GLfloat)v2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMapGrid2f( %d, %f, %f, %d, %f, %f )", (int)un, (float)u1, (float)u2, (int)vn, (float)v1, (float)v2);
  }
}

FABRIC_EXT_EXPORT void klMaterialf(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glMaterialf( (GLenum)face, (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMaterialf( %d, %d, %f )", (int)face, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klMaterialfv(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glMaterialfv( (GLenum)face, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMaterialfv( %d, %d, GLfloat* )", (int)face, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klMateriali(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glMateriali( (GLenum)face, (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMateriali( %d, %d, %d )", (int)face, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klMaterialiv(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glMaterialiv( (GLenum)face, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMaterialiv( %d, %d, GLint* )", (int)face, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klMatrixMode(
  const KL::Size & mode
){
  try
  {
    glMatrixMode( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixMode( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klMultMatrixd(
  const KL::VariableArray<KL::Scalar> & m
){
  try
  {
    glMultMatrixd( (const GLdouble*)&m[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultMatrixd( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klMultMatrixf(
  const KL::VariableArray<KL::Scalar> & m
){
  try
  {
    glMultMatrixf( (const GLfloat*)&m[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultMatrixf( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klNewList(
  const KL::Size & list,
  const KL::Size & mode
){
  try
  {
    glNewList( (GLuint)list, (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNewList( 0x%04X, %d )", (unsigned)list, (int)mode);
  }
}

FABRIC_EXT_EXPORT void klNormal3b(
  const KL::Byte & nx,
  const KL::Byte & ny,
  const KL::Byte & nz
){
  try
  {
    glNormal3b( (GLbyte)nx, (GLbyte)ny, (GLbyte)nz );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNormal3b( %d, %d, %d )", (int)nx, (int)ny, (int)nz);
  }
}

FABRIC_EXT_EXPORT void klNormal3bv(
  const KL::VariableArray<KL::Byte> & v
){
  try
  {
    glNormal3bv( (const GLbyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNormal3bv( GLbyte* )");
  }
}

FABRIC_EXT_EXPORT void klNormal3d(
  const KL::Scalar & nx,
  const KL::Scalar & ny,
  const KL::Scalar & nz
){
  try
  {
    glNormal3d( (GLdouble)nx, (GLdouble)ny, (GLdouble)nz );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNormal3d( %f, %f, %f )", (float)nx, (float)ny, (float)nz);
  }
}

FABRIC_EXT_EXPORT void klNormal3dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glNormal3dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNormal3dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klNormal3f(
  const KL::Scalar & nx,
  const KL::Scalar & ny,
  const KL::Scalar & nz
){
  try
  {
    glNormal3f( (GLfloat)nx, (GLfloat)ny, (GLfloat)nz );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNormal3f( %f, %f, %f )", (float)nx, (float)ny, (float)nz);
  }
}

FABRIC_EXT_EXPORT void klNormal3fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glNormal3fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNormal3fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klNormal3i(
  const KL::Integer & nx,
  const KL::Integer & ny,
  const KL::Integer & nz
){
  try
  {
    glNormal3i( (GLint)nx, (GLint)ny, (GLint)nz );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNormal3i( %d, %d, %d )", (int)nx, (int)ny, (int)nz);
  }
}

FABRIC_EXT_EXPORT void klNormal3iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glNormal3iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNormal3iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klNormal3s(
  const KL::Integer & nx,
  const KL::Integer & ny,
  const KL::Integer & nz
){
  try
  {
    glNormal3s( (GLshort)nx, (GLshort)ny, (GLshort)nz );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNormal3s( %d, %d, %d )", (int)nx, (int)ny, (int)nz);
  }
}

FABRIC_EXT_EXPORT void klNormal3sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glNormal3sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNormal3sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klNormalPointer(
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glNormalPointer( (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNormalPointer( %d, %d, GLvoid* )", (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klOrtho(
  const KL::Scalar & left,
  const KL::Scalar & right,
  const KL::Scalar & bottom,
  const KL::Scalar & top,
  const KL::Scalar & zNear,
  const KL::Scalar & zFar
){
  try
  {
    glOrtho( (GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top, (GLdouble)zNear, (GLdouble)zFar );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glOrtho( %f, %f, %f, %f, %f, %f )", (float)left, (float)right, (float)bottom, (float)top, (float)zNear, (float)zFar);
  }
}

FABRIC_EXT_EXPORT void klPassThrough(
  const KL::Scalar & token
){
  try
  {
    glPassThrough( (GLfloat)token );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPassThrough( %f )", (float)token);
  }
}

FABRIC_EXT_EXPORT void klPixelMapfv(
  const KL::Size & map,
  const KL::Size & mapsize,
  const KL::VariableArray<KL::Scalar> & values
){
  try
  {
    glPixelMapfv( (GLenum)map, (GLsizei)mapsize, (const GLfloat*)&values[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPixelMapfv( %d, %d, GLfloat* )", (int)map, (int)mapsize);
  }
}

FABRIC_EXT_EXPORT void klPixelMapuiv(
  const KL::Size & map,
  const KL::Size & mapsize,
  const KL::VariableArray<KL::Size> & values
){
  try
  {
    glPixelMapuiv( (GLenum)map, (GLsizei)mapsize, (const GLuint*)&values[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPixelMapuiv( %d, %d, GLuint* )", (int)map, (int)mapsize);
  }
}

FABRIC_EXT_EXPORT void klPixelMapusv(
  const KL::Size & map,
  const KL::Size & mapsize,
  const KL::VariableArray<KL::Integer> & values
){
  try
  {
    glPixelMapusv( (GLenum)map, (GLsizei)mapsize, (const GLushort*)&values[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPixelMapusv( %d, %d, GLushort* )", (int)map, (int)mapsize);
  }
}

FABRIC_EXT_EXPORT void klPixelStoref(
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glPixelStoref( (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPixelStoref( %d, %f )", (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klPixelStorei(
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glPixelStorei( (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPixelStorei( %d, %d )", (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klPixelTransferf(
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glPixelTransferf( (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPixelTransferf( %d, %f )", (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klPixelTransferi(
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glPixelTransferi( (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPixelTransferi( %d, %d )", (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klPixelZoom(
  const KL::Scalar & xfactor,
  const KL::Scalar & yfactor
){
  try
  {
    glPixelZoom( (GLfloat)xfactor, (GLfloat)yfactor );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPixelZoom( %f, %f )", (float)xfactor, (float)yfactor);
  }
}

FABRIC_EXT_EXPORT void klPointSize(
  const KL::Scalar & size
){
  try
  {
    glPointSize( (GLfloat)size );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPointSize( %f )", (float)size);
  }
}

FABRIC_EXT_EXPORT void klPolygonMode(
  const KL::Size & face,
  const KL::Size & mode
){
  try
  {
    glPolygonMode( (GLenum)face, (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPolygonMode( %d, %d )", (int)face, (int)mode);
  }
}

FABRIC_EXT_EXPORT void klPolygonOffset(
  const KL::Scalar & factor,
  const KL::Scalar & units
){
  try
  {
    glPolygonOffset( (GLfloat)factor, (GLfloat)units );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPolygonOffset( %f, %f )", (float)factor, (float)units);
  }
}

FABRIC_EXT_EXPORT void klPolygonStipple(
  const KL::VariableArray<KL::Size> & mask
){
  try
  {
    glPolygonStipple( (const GLubyte*)&mask[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPolygonStipple( GLubyte* )");
  }
}

FABRIC_EXT_EXPORT void klPopAttrib()
{
  try
  {
    glPopAttrib();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPopAttrib(  )");
  }
}

FABRIC_EXT_EXPORT void klPopClientAttrib()
{
  try
  {
    glPopClientAttrib();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPopClientAttrib(  )");
  }
}

FABRIC_EXT_EXPORT void klPopMatrix()
{
  try
  {
    glPopMatrix();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPopMatrix(  )");
  }
}

FABRIC_EXT_EXPORT void klPopName()
{
  try
  {
    glPopName();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPopName(  )");
  }
}

FABRIC_EXT_EXPORT void klPrioritizeTextures(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & textures,
  const KL::VariableArray<KL::Scalar> & priorities
){
  try
  {
    glPrioritizeTextures( (GLsizei)n, (const GLuint*)&textures[0], (const GLclampf*)&priorities[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPrioritizeTextures( %d, GLuint*, GLclampf* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klPushAttrib(
  const KL::Size & mask
){
  try
  {
    glPushAttrib( (GLbitfield)mask );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPushAttrib( 0x%04X )", (unsigned)mask);
  }
}

FABRIC_EXT_EXPORT void klPushClientAttrib(
  const KL::Size & mask
){
  try
  {
    glPushClientAttrib( (GLbitfield)mask );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPushClientAttrib( 0x%04X )", (unsigned)mask);
  }
}

FABRIC_EXT_EXPORT void klPushMatrix()
{
  try
  {
    glPushMatrix();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPushMatrix(  )");
  }
}

FABRIC_EXT_EXPORT void klPushName(
  const KL::Size & name
){
  try
  {
    glPushName( (GLuint)name );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPushName( 0x%04X )", (unsigned)name);
  }
}

FABRIC_EXT_EXPORT void klRasterPos2d(
  const KL::Scalar & x,
  const KL::Scalar & y
){
  try
  {
    glRasterPos2d( (GLdouble)x, (GLdouble)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos2d( %f, %f )", (float)x, (float)y);
  }
}

FABRIC_EXT_EXPORT void klRasterPos2dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glRasterPos2dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos2dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klRasterPos2f(
  const KL::Scalar & x,
  const KL::Scalar & y
){
  try
  {
    glRasterPos2f( (GLfloat)x, (GLfloat)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos2f( %f, %f )", (float)x, (float)y);
  }
}

FABRIC_EXT_EXPORT void klRasterPos2fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glRasterPos2fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos2fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klRasterPos2i(
  const KL::Integer & x,
  const KL::Integer & y
){
  try
  {
    glRasterPos2i( (GLint)x, (GLint)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos2i( %d, %d )", (int)x, (int)y);
  }
}

FABRIC_EXT_EXPORT void klRasterPos2iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glRasterPos2iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos2iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klRasterPos2s(
  const KL::Integer & x,
  const KL::Integer & y
){
  try
  {
    glRasterPos2s( (GLshort)x, (GLshort)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos2s( %d, %d )", (int)x, (int)y);
  }
}

FABRIC_EXT_EXPORT void klRasterPos2sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glRasterPos2sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos2sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klRasterPos3d(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glRasterPos3d( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos3d( %f, %f, %f )", (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klRasterPos3dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glRasterPos3dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos3dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klRasterPos3f(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos3f( %f, %f, %f )", (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klRasterPos3fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glRasterPos3fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos3fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klRasterPos3i(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  try
  {
    glRasterPos3i( (GLint)x, (GLint)y, (GLint)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos3i( %d, %d, %d )", (int)x, (int)y, (int)z);
  }
}

FABRIC_EXT_EXPORT void klRasterPos3iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glRasterPos3iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos3iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klRasterPos3s(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  try
  {
    glRasterPos3s( (GLshort)x, (GLshort)y, (GLshort)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos3s( %d, %d, %d )", (int)x, (int)y, (int)z);
  }
}

FABRIC_EXT_EXPORT void klRasterPos3sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glRasterPos3sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos3sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klRasterPos4d(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  try
  {
    glRasterPos4d( (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos4d( %f, %f, %f, %f )", (float)x, (float)y, (float)z, (float)w);
  }
}

FABRIC_EXT_EXPORT void klRasterPos4dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glRasterPos4dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos4dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klRasterPos4f(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  try
  {
    glRasterPos4f( (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos4f( %f, %f, %f, %f )", (float)x, (float)y, (float)z, (float)w);
  }
}

FABRIC_EXT_EXPORT void klRasterPos4fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glRasterPos4fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos4fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klRasterPos4i(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  try
  {
    glRasterPos4i( (GLint)x, (GLint)y, (GLint)z, (GLint)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos4i( %d, %d, %d, %d )", (int)x, (int)y, (int)z, (int)w);
  }
}

FABRIC_EXT_EXPORT void klRasterPos4iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glRasterPos4iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos4iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klRasterPos4s(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  try
  {
    glRasterPos4s( (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos4s( %d, %d, %d, %d )", (int)x, (int)y, (int)z, (int)w);
  }
}

FABRIC_EXT_EXPORT void klRasterPos4sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glRasterPos4sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRasterPos4sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klReadBuffer(
  const KL::Size & mode
){
  try
  {
    glReadBuffer( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glReadBuffer( %d )", (int)mode);
  }
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
  try
  {
    glReadPixels( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glReadPixels( %d, %d, %d, %d, %d, %d, GLvoid* )", (int)x, (int)y, (int)width, (int)height, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klRectd(
  const KL::Scalar & x1,
  const KL::Scalar & y1,
  const KL::Scalar & x2,
  const KL::Scalar & y2
){
  try
  {
    glRectd( (GLdouble)x1, (GLdouble)y1, (GLdouble)x2, (GLdouble)y2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRectd( %f, %f, %f, %f )", (float)x1, (float)y1, (float)x2, (float)y2);
  }
}

FABRIC_EXT_EXPORT void klRectdv(
  const KL::VariableArray<KL::Scalar> & v1,
  const KL::VariableArray<KL::Scalar> & v2
){
  try
  {
    glRectdv( (const GLdouble*)&v1[0], (const GLdouble*)&v2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRectdv( GLdouble*, GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klRectf(
  const KL::Scalar & x1,
  const KL::Scalar & y1,
  const KL::Scalar & x2,
  const KL::Scalar & y2
){
  try
  {
    glRectf( (GLfloat)x1, (GLfloat)y1, (GLfloat)x2, (GLfloat)y2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRectf( %f, %f, %f, %f )", (float)x1, (float)y1, (float)x2, (float)y2);
  }
}

FABRIC_EXT_EXPORT void klRectfv(
  const KL::VariableArray<KL::Scalar> & v1,
  const KL::VariableArray<KL::Scalar> & v2
){
  try
  {
    glRectfv( (const GLfloat*)&v1[0], (const GLfloat*)&v2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRectfv( GLfloat*, GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klRecti(
  const KL::Integer & x1,
  const KL::Integer & y1,
  const KL::Integer & x2,
  const KL::Integer & y2
){
  try
  {
    glRecti( (GLint)x1, (GLint)y1, (GLint)x2, (GLint)y2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRecti( %d, %d, %d, %d )", (int)x1, (int)y1, (int)x2, (int)y2);
  }
}

FABRIC_EXT_EXPORT void klRectiv(
  const KL::VariableArray<KL::Integer> & v1,
  const KL::VariableArray<KL::Integer> & v2
){
  try
  {
    glRectiv( (const GLint*)&v1[0], (const GLint*)&v2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRectiv( GLint*, GLint* )");
  }
}

FABRIC_EXT_EXPORT void klRects(
  const KL::Integer & x1,
  const KL::Integer & y1,
  const KL::Integer & x2,
  const KL::Integer & y2
){
  try
  {
    glRects( (GLshort)x1, (GLshort)y1, (GLshort)x2, (GLshort)y2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRects( %d, %d, %d, %d )", (int)x1, (int)y1, (int)x2, (int)y2);
  }
}

FABRIC_EXT_EXPORT void klRectsv(
  const KL::VariableArray<KL::Integer> & v1,
  const KL::VariableArray<KL::Integer> & v2
){
  try
  {
    glRectsv( (const GLshort*)&v1[0], (const GLshort*)&v2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRectsv( GLshort*, GLshort* )");
  }
}

FABRIC_EXT_EXPORT KL::Integer klRenderMode(
  const KL::Size & mode
){
  try
  {
    GLint result = glRenderMode( (GLenum)mode );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRenderMode( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klRotated(
  const KL::Scalar & angle,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glRotated( (GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRotated( %f, %f, %f, %f )", (float)angle, (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klRotatef(
  const KL::Scalar & angle,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glRotatef( (GLfloat)angle, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRotatef( %f, %f, %f, %f )", (float)angle, (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klScaled(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glScaled( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glScaled( %f, %f, %f )", (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klScalef(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glScalef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glScalef( %f, %f, %f )", (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klScissor(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  try
  {
    glScissor( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glScissor( %d, %d, %d, %d )", (int)x, (int)y, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klSelectBuffer(
  const KL::Size & size,
  KL::VariableArray<KL::Size> & buffer
){
  try
  {
    glSelectBuffer( (GLsizei)size, (GLuint*)&buffer[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSelectBuffer( %d, GLuint* )", (int)size);
  }
}

FABRIC_EXT_EXPORT void klShadeModel(
  const KL::Size & mode
){
  try
  {
    glShadeModel( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glShadeModel( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klStencilFunc(
  const KL::Size & func,
  const KL::Integer & ref,
  const KL::Size & mask
){
  try
  {
    glStencilFunc( (GLenum)func, (GLint)ref, (GLuint)mask );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glStencilFunc( %d, %d, 0x%04X )", (int)func, (int)ref, (unsigned)mask);
  }
}

FABRIC_EXT_EXPORT void klStencilMask(
  const KL::Size & mask
){
  try
  {
    glStencilMask( (GLuint)mask );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glStencilMask( 0x%04X )", (unsigned)mask);
  }
}

FABRIC_EXT_EXPORT void klStencilOp(
  const KL::Size & fail,
  const KL::Size & zfail,
  const KL::Size & zpass
){
  try
  {
    glStencilOp( (GLenum)fail, (GLenum)zfail, (GLenum)zpass );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glStencilOp( %d, %d, %d )", (int)fail, (int)zfail, (int)zpass);
  }
}

FABRIC_EXT_EXPORT void klTexCoord1d(
  const KL::Scalar & s
){
  try
  {
    glTexCoord1d( (GLdouble)s );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord1d( %f )", (float)s);
  }
}

FABRIC_EXT_EXPORT void klTexCoord1dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glTexCoord1dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord1dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord1f(
  const KL::Scalar & s
){
  try
  {
    glTexCoord1f( (GLfloat)s );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord1f( %f )", (float)s);
  }
}

FABRIC_EXT_EXPORT void klTexCoord1fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glTexCoord1fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord1fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord1i(
  const KL::Integer & s
){
  try
  {
    glTexCoord1i( (GLint)s );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord1i( %d )", (int)s);
  }
}

FABRIC_EXT_EXPORT void klTexCoord1iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glTexCoord1iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord1iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord1s(
  const KL::Integer & s
){
  try
  {
    glTexCoord1s( (GLshort)s );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord1s( %d )", (int)s);
  }
}

FABRIC_EXT_EXPORT void klTexCoord1sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glTexCoord1sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord1sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord2d(
  const KL::Scalar & s,
  const KL::Scalar & t
){
  try
  {
    glTexCoord2d( (GLdouble)s, (GLdouble)t );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord2d( %f, %f )", (float)s, (float)t);
  }
}

FABRIC_EXT_EXPORT void klTexCoord2dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glTexCoord2dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord2dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord2f(
  const KL::Scalar & s,
  const KL::Scalar & t
){
  try
  {
    glTexCoord2f( (GLfloat)s, (GLfloat)t );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord2f( %f, %f )", (float)s, (float)t);
  }
}

FABRIC_EXT_EXPORT void klTexCoord2fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glTexCoord2fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord2fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord2i(
  const KL::Integer & s,
  const KL::Integer & t
){
  try
  {
    glTexCoord2i( (GLint)s, (GLint)t );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord2i( %d, %d )", (int)s, (int)t);
  }
}

FABRIC_EXT_EXPORT void klTexCoord2iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glTexCoord2iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord2iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord2s(
  const KL::Integer & s,
  const KL::Integer & t
){
  try
  {
    glTexCoord2s( (GLshort)s, (GLshort)t );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord2s( %d, %d )", (int)s, (int)t);
  }
}

FABRIC_EXT_EXPORT void klTexCoord2sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glTexCoord2sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord2sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord3d(
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r
){
  try
  {
    glTexCoord3d( (GLdouble)s, (GLdouble)t, (GLdouble)r );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord3d( %f, %f, %f )", (float)s, (float)t, (float)r);
  }
}

FABRIC_EXT_EXPORT void klTexCoord3dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glTexCoord3dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord3dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord3f(
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r
){
  try
  {
    glTexCoord3f( (GLfloat)s, (GLfloat)t, (GLfloat)r );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord3f( %f, %f, %f )", (float)s, (float)t, (float)r);
  }
}

FABRIC_EXT_EXPORT void klTexCoord3fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glTexCoord3fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord3fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord3i(
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r
){
  try
  {
    glTexCoord3i( (GLint)s, (GLint)t, (GLint)r );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord3i( %d, %d, %d )", (int)s, (int)t, (int)r);
  }
}

FABRIC_EXT_EXPORT void klTexCoord3iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glTexCoord3iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord3iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord3s(
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r
){
  try
  {
    glTexCoord3s( (GLshort)s, (GLshort)t, (GLshort)r );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord3s( %d, %d, %d )", (int)s, (int)t, (int)r);
  }
}

FABRIC_EXT_EXPORT void klTexCoord3sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glTexCoord3sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord3sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord4d(
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r,
  const KL::Scalar & q
){
  try
  {
    glTexCoord4d( (GLdouble)s, (GLdouble)t, (GLdouble)r, (GLdouble)q );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord4d( %f, %f, %f, %f )", (float)s, (float)t, (float)r, (float)q);
  }
}

FABRIC_EXT_EXPORT void klTexCoord4dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glTexCoord4dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord4dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord4f(
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r,
  const KL::Scalar & q
){
  try
  {
    glTexCoord4f( (GLfloat)s, (GLfloat)t, (GLfloat)r, (GLfloat)q );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord4f( %f, %f, %f, %f )", (float)s, (float)t, (float)r, (float)q);
  }
}

FABRIC_EXT_EXPORT void klTexCoord4fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glTexCoord4fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord4fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord4i(
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r,
  const KL::Integer & q
){
  try
  {
    glTexCoord4i( (GLint)s, (GLint)t, (GLint)r, (GLint)q );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord4i( %d, %d, %d, %d )", (int)s, (int)t, (int)r, (int)q);
  }
}

FABRIC_EXT_EXPORT void klTexCoord4iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glTexCoord4iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord4iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoord4s(
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r,
  const KL::Integer & q
){
  try
  {
    glTexCoord4s( (GLshort)s, (GLshort)t, (GLshort)r, (GLshort)q );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord4s( %d, %d, %d, %d )", (int)s, (int)t, (int)r, (int)q);
  }
}

FABRIC_EXT_EXPORT void klTexCoord4sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glTexCoord4sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoord4sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klTexCoordPointer(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glTexCoordPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoordPointer( %d, %d, %d, GLvoid* )", (int)size, (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klTexEnvf(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glTexEnvf( (GLenum)target, (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexEnvf( %d, %d, %f )", (int)target, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klTexEnvfv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glTexEnvfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexEnvfv( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klTexEnvi(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glTexEnvi( (GLenum)target, (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexEnvi( %d, %d, %d )", (int)target, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klTexEnviv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glTexEnviv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexEnviv( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klTexGend(
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glTexGend( (GLenum)coord, (GLenum)pname, (GLdouble)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexGend( %d, %d, %f )", (int)coord, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klTexGendv(
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glTexGendv( (GLenum)coord, (GLenum)pname, (const GLdouble*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexGendv( %d, %d, GLdouble* )", (int)coord, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klTexGenf(
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glTexGenf( (GLenum)coord, (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexGenf( %d, %d, %f )", (int)coord, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klTexGenfv(
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glTexGenfv( (GLenum)coord, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexGenfv( %d, %d, GLfloat* )", (int)coord, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klTexGeni(
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glTexGeni( (GLenum)coord, (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexGeni( %d, %d, %d )", (int)coord, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klTexGeniv(
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glTexGeniv( (GLenum)coord, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexGeniv( %d, %d, GLint* )", (int)coord, (int)pname);
  }
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
  try
  {
    glTexImage1D( (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexImage1D( %d, %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)format, (int)type);
  }
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
  try
  {
    glTexImage2D( (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexImage2D( %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klTexParameterf(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glTexParameterf( (GLenum)target, (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexParameterf( %d, %d, %f )", (int)target, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klTexParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glTexParameterfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexParameterfv( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klTexParameteri(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glTexParameteri( (GLenum)target, (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexParameteri( %d, %d, %d )", (int)target, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klTexParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glTexParameteriv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexParameteriv( %d, %d, GLint* )", (int)target, (int)pname);
  }
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
  try
  {
    glTexSubImage1D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexSubImage1D( %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)type);
  }
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
  try
  {
    glTexSubImage2D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexSubImage2D( %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klTranslated(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glTranslated( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTranslated( %f, %f, %f )", (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klTranslatef(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTranslatef( %f, %f, %f )", (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klVertex2d(
  const KL::Scalar & x,
  const KL::Scalar & y
){
  try
  {
    glVertex2d( (GLdouble)x, (GLdouble)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex2d( %f, %f )", (float)x, (float)y);
  }
}

FABRIC_EXT_EXPORT void klVertex2dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertex2dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex2dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klVertex2f(
  const KL::Scalar & x,
  const KL::Scalar & y
){
  try
  {
    glVertex2f( (GLfloat)x, (GLfloat)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex2f( %f, %f )", (float)x, (float)y);
  }
}

FABRIC_EXT_EXPORT void klVertex2fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertex2fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex2fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klVertex2i(
  const KL::Integer & x,
  const KL::Integer & y
){
  try
  {
    glVertex2i( (GLint)x, (GLint)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex2i( %d, %d )", (int)x, (int)y);
  }
}

FABRIC_EXT_EXPORT void klVertex2iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertex2iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex2iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klVertex2s(
  const KL::Integer & x,
  const KL::Integer & y
){
  try
  {
    glVertex2s( (GLshort)x, (GLshort)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex2s( %d, %d )", (int)x, (int)y);
  }
}

FABRIC_EXT_EXPORT void klVertex2sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertex2sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex2sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klVertex3d(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glVertex3d( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex3d( %f, %f, %f )", (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klVertex3dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertex3dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex3dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klVertex3f(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glVertex3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex3f( %f, %f, %f )", (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klVertex3fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertex3fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex3fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klVertex3i(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  try
  {
    glVertex3i( (GLint)x, (GLint)y, (GLint)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex3i( %d, %d, %d )", (int)x, (int)y, (int)z);
  }
}

FABRIC_EXT_EXPORT void klVertex3iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertex3iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex3iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klVertex3s(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  try
  {
    glVertex3s( (GLshort)x, (GLshort)y, (GLshort)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex3s( %d, %d, %d )", (int)x, (int)y, (int)z);
  }
}

FABRIC_EXT_EXPORT void klVertex3sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertex3sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex3sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klVertex4d(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  try
  {
    glVertex4d( (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex4d( %f, %f, %f, %f )", (float)x, (float)y, (float)z, (float)w);
  }
}

FABRIC_EXT_EXPORT void klVertex4dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertex4dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex4dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klVertex4f(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  try
  {
    glVertex4f( (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex4f( %f, %f, %f, %f )", (float)x, (float)y, (float)z, (float)w);
  }
}

FABRIC_EXT_EXPORT void klVertex4fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertex4fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex4fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klVertex4i(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  try
  {
    glVertex4i( (GLint)x, (GLint)y, (GLint)z, (GLint)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex4i( %d, %d, %d, %d )", (int)x, (int)y, (int)z, (int)w);
  }
}

FABRIC_EXT_EXPORT void klVertex4iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertex4iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex4iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klVertex4s(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  try
  {
    glVertex4s( (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex4s( %d, %d, %d, %d )", (int)x, (int)y, (int)z, (int)w);
  }
}

FABRIC_EXT_EXPORT void klVertex4sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertex4sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertex4sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klVertexPointer(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glVertexPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexPointer( %d, %d, %d, GLvoid* )", (int)size, (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klViewport(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  try
  {
    glViewport( (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glViewport( %d, %d, %d, %d )", (int)x, (int)y, (int)width, (int)height);
  }
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
  try
  {
    glCopyTexSubImage3D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTexSubImage3D( %d, %d, %d, %d, %d, %d, %d, %d, %d )", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)x, (int)y, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klDrawRangeElements(
  const KL::Size & mode,
  const KL::Size & start,
  const KL::Size & end,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices
){
  try
  {
    glDrawRangeElements( (GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, indices );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawRangeElements( %d, 0x%04X, 0x%04X, %d, %d, GLvoid* )", (int)mode, (unsigned)start, (unsigned)end, (int)count, (int)type);
  }
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
  try
  {
    glTexImage3D( (GLenum)target, (GLint)level, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexImage3D( %d, %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)internalFormat, (int)width, (int)height, (int)depth, (int)border, (int)format, (int)type);
  }
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
  try
  {
    glTexSubImage3D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexSubImage3D( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klActiveTexture(
  const KL::Size & texture
){
  try
  {
    glActiveTexture( (GLenum)texture );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glActiveTexture( %d )", (int)texture);
  }
}

FABRIC_EXT_EXPORT void klClientActiveTexture(
  const KL::Size & texture
){
  try
  {
    glClientActiveTexture( (GLenum)texture );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClientActiveTexture( %d )", (int)texture);
  }
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
  try
  {
    glCompressedTexImage1D( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLint)border, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedTexImage1D( %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)imageSize);
  }
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
  try
  {
    glCompressedTexImage2D( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedTexImage2D( %d, %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)imageSize);
  }
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
  try
  {
    glCompressedTexImage3D( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedTexImage3D( %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)imageSize);
  }
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
  try
  {
    glCompressedTexSubImage1D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedTexSubImage1D( %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)imageSize);
  }
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
  try
  {
    glCompressedTexSubImage2D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedTexSubImage2D( %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)imageSize);
  }
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
  try
  {
    glCompressedTexSubImage3D( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedTexSubImage3D( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)imageSize);
  }
}

FABRIC_EXT_EXPORT void klGetCompressedTexImage(
  const KL::Size & target,
  const KL::Integer & lod,
  KL::Data img
){
  try
  {
    glGetCompressedTexImage( (GLenum)target, (GLint)lod, img );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetCompressedTexImage( %d, %d, GLvoid* )", (int)target, (int)lod);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord1d(
  const KL::Size & target,
  const KL::Scalar & s
){
  try
  {
    glMultiTexCoord1d( (GLenum)target, (GLdouble)s );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord1d( %d, %f )", (int)target, (float)s);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord1dv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glMultiTexCoord1dv( (GLenum)target, (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord1dv( %d, GLdouble* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord1f(
  const KL::Size & target,
  const KL::Scalar & s
){
  try
  {
    glMultiTexCoord1f( (GLenum)target, (GLfloat)s );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord1f( %d, %f )", (int)target, (float)s);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord1fv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glMultiTexCoord1fv( (GLenum)target, (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord1fv( %d, GLfloat* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord1i(
  const KL::Size & target,
  const KL::Integer & s
){
  try
  {
    glMultiTexCoord1i( (GLenum)target, (GLint)s );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord1i( %d, %d )", (int)target, (int)s);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord1iv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glMultiTexCoord1iv( (GLenum)target, (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord1iv( %d, GLint* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord1s(
  const KL::Size & target,
  const KL::Integer & s
){
  try
  {
    glMultiTexCoord1s( (GLenum)target, (GLshort)s );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord1s( %d, %d )", (int)target, (int)s);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord1sv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glMultiTexCoord1sv( (GLenum)target, (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord1sv( %d, GLshort* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord2d(
  const KL::Size & target,
  const KL::Scalar & s,
  const KL::Scalar & t
){
  try
  {
    glMultiTexCoord2d( (GLenum)target, (GLdouble)s, (GLdouble)t );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord2d( %d, %f, %f )", (int)target, (float)s, (float)t);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord2dv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glMultiTexCoord2dv( (GLenum)target, (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord2dv( %d, GLdouble* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord2f(
  const KL::Size & target,
  const KL::Scalar & s,
  const KL::Scalar & t
){
  try
  {
    glMultiTexCoord2f( (GLenum)target, (GLfloat)s, (GLfloat)t );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord2f( %d, %f, %f )", (int)target, (float)s, (float)t);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord2fv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glMultiTexCoord2fv( (GLenum)target, (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord2fv( %d, GLfloat* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord2i(
  const KL::Size & target,
  const KL::Integer & s,
  const KL::Integer & t
){
  try
  {
    glMultiTexCoord2i( (GLenum)target, (GLint)s, (GLint)t );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord2i( %d, %d, %d )", (int)target, (int)s, (int)t);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord2iv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glMultiTexCoord2iv( (GLenum)target, (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord2iv( %d, GLint* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord2s(
  const KL::Size & target,
  const KL::Integer & s,
  const KL::Integer & t
){
  try
  {
    glMultiTexCoord2s( (GLenum)target, (GLshort)s, (GLshort)t );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord2s( %d, %d, %d )", (int)target, (int)s, (int)t);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord2sv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glMultiTexCoord2sv( (GLenum)target, (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord2sv( %d, GLshort* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord3d(
  const KL::Size & target,
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r
){
  try
  {
    glMultiTexCoord3d( (GLenum)target, (GLdouble)s, (GLdouble)t, (GLdouble)r );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord3d( %d, %f, %f, %f )", (int)target, (float)s, (float)t, (float)r);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord3dv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glMultiTexCoord3dv( (GLenum)target, (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord3dv( %d, GLdouble* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord3f(
  const KL::Size & target,
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r
){
  try
  {
    glMultiTexCoord3f( (GLenum)target, (GLfloat)s, (GLfloat)t, (GLfloat)r );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord3f( %d, %f, %f, %f )", (int)target, (float)s, (float)t, (float)r);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord3fv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glMultiTexCoord3fv( (GLenum)target, (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord3fv( %d, GLfloat* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord3i(
  const KL::Size & target,
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r
){
  try
  {
    glMultiTexCoord3i( (GLenum)target, (GLint)s, (GLint)t, (GLint)r );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord3i( %d, %d, %d, %d )", (int)target, (int)s, (int)t, (int)r);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord3iv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glMultiTexCoord3iv( (GLenum)target, (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord3iv( %d, GLint* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord3s(
  const KL::Size & target,
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r
){
  try
  {
    glMultiTexCoord3s( (GLenum)target, (GLshort)s, (GLshort)t, (GLshort)r );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord3s( %d, %d, %d, %d )", (int)target, (int)s, (int)t, (int)r);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord3sv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glMultiTexCoord3sv( (GLenum)target, (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord3sv( %d, GLshort* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord4d(
  const KL::Size & target,
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r,
  const KL::Scalar & q
){
  try
  {
    glMultiTexCoord4d( (GLenum)target, (GLdouble)s, (GLdouble)t, (GLdouble)r, (GLdouble)q );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord4d( %d, %f, %f, %f, %f )", (int)target, (float)s, (float)t, (float)r, (float)q);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord4dv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glMultiTexCoord4dv( (GLenum)target, (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord4dv( %d, GLdouble* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord4f(
  const KL::Size & target,
  const KL::Scalar & s,
  const KL::Scalar & t,
  const KL::Scalar & r,
  const KL::Scalar & q
){
  try
  {
    glMultiTexCoord4f( (GLenum)target, (GLfloat)s, (GLfloat)t, (GLfloat)r, (GLfloat)q );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord4f( %d, %f, %f, %f, %f )", (int)target, (float)s, (float)t, (float)r, (float)q);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord4fv(
  const KL::Size & target,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glMultiTexCoord4fv( (GLenum)target, (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord4fv( %d, GLfloat* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord4i(
  const KL::Size & target,
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r,
  const KL::Integer & q
){
  try
  {
    glMultiTexCoord4i( (GLenum)target, (GLint)s, (GLint)t, (GLint)r, (GLint)q );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord4i( %d, %d, %d, %d, %d )", (int)target, (int)s, (int)t, (int)r, (int)q);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord4iv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glMultiTexCoord4iv( (GLenum)target, (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord4iv( %d, GLint* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord4s(
  const KL::Size & target,
  const KL::Integer & s,
  const KL::Integer & t,
  const KL::Integer & r,
  const KL::Integer & q
){
  try
  {
    glMultiTexCoord4s( (GLenum)target, (GLshort)s, (GLshort)t, (GLshort)r, (GLshort)q );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord4s( %d, %d, %d, %d, %d )", (int)target, (int)s, (int)t, (int)r, (int)q);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoord4sv(
  const KL::Size & target,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glMultiTexCoord4sv( (GLenum)target, (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoord4sv( %d, GLshort* )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klSampleCoverage(
  const KL::Scalar & value,
  const KL::Boolean & invert
){
  try
  {
    glSampleCoverage( (GLclampf)value, (GLboolean)invert );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSampleCoverage( %f, %b )", (float)value, (bool)invert);
  }
}

FABRIC_EXT_EXPORT void klBlendColor(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue,
  const KL::Scalar & alpha
){
  try
  {
    glBlendColor( (GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBlendColor( %f, %f, %f, %f )", (float)red, (float)green, (float)blue, (float)alpha);
  }
}

FABRIC_EXT_EXPORT void klBlendEquation(
  const KL::Size & mode
){
  try
  {
    glBlendEquation( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBlendEquation( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klBlendFuncSeparate(
  const KL::Size & sfactorRGB,
  const KL::Size & dfactorRGB,
  const KL::Size & sfactorAlpha,
  const KL::Size & dfactorAlpha
){
  try
  {
    glBlendFuncSeparate( (GLenum)sfactorRGB, (GLenum)dfactorRGB, (GLenum)sfactorAlpha, (GLenum)dfactorAlpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBlendFuncSeparate( %d, %d, %d, %d )", (int)sfactorRGB, (int)dfactorRGB, (int)sfactorAlpha, (int)dfactorAlpha);
  }
}

FABRIC_EXT_EXPORT void klFogCoordPointer(
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glFogCoordPointer( (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogCoordPointer( %d, %d, GLvoid* )", (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klFogCoordd(
  const KL::Scalar & coord
){
  try
  {
    glFogCoordd( (GLdouble)coord );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogCoordd( %f )", (float)coord);
  }
}

FABRIC_EXT_EXPORT void klFogCoorddv(
  const KL::VariableArray<KL::Scalar> & coord
){
  try
  {
    glFogCoorddv( (const GLdouble*)&coord[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogCoorddv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klFogCoordf(
  const KL::Scalar & coord
){
  try
  {
    glFogCoordf( (GLfloat)coord );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogCoordf( %f )", (float)coord);
  }
}

FABRIC_EXT_EXPORT void klFogCoordfv(
  const KL::VariableArray<KL::Scalar> & coord
){
  try
  {
    glFogCoordfv( (const GLfloat*)&coord[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogCoordfv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klMultiDrawArrays(
  const KL::Size & mode,
  KL::VariableArray<KL::Integer> & first,
  KL::VariableArray<KL::Size> & count,
  const KL::Size & primcount
){
  try
  {
    glMultiDrawArrays( (GLenum)mode, (GLint*)&first[0], (GLsizei*)&count[0], (GLsizei)primcount );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiDrawArrays( %d, GLint*, GLsizei*, %d )", (int)mode, (int)primcount);
  }
}

FABRIC_EXT_EXPORT void klPointParameterf(
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glPointParameterf( (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPointParameterf( %d, %f )", (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klPointParameterfv(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glPointParameterfv( (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPointParameterfv( %d, GLfloat* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klPointParameteri(
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glPointParameteri( (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPointParameteri( %d, %d )", (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klPointParameteriv(
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glPointParameteriv( (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPointParameteriv( %d, GLint* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3b(
  const KL::Byte & red,
  const KL::Byte & green,
  const KL::Byte & blue
){
  try
  {
    glSecondaryColor3b( (GLbyte)red, (GLbyte)green, (GLbyte)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3b( %d, %d, %d )", (int)red, (int)green, (int)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3bv(
  const KL::VariableArray<KL::Byte> & v
){
  try
  {
    glSecondaryColor3bv( (const GLbyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3bv( GLbyte* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3d(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue
){
  try
  {
    glSecondaryColor3d( (GLdouble)red, (GLdouble)green, (GLdouble)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3d( %f, %f, %f )", (float)red, (float)green, (float)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3dv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glSecondaryColor3dv( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3f(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue
){
  try
  {
    glSecondaryColor3f( (GLfloat)red, (GLfloat)green, (GLfloat)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3f( %f, %f, %f )", (float)red, (float)green, (float)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3fv(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glSecondaryColor3fv( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3i(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  try
  {
    glSecondaryColor3i( (GLint)red, (GLint)green, (GLint)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3i( %d, %d, %d )", (int)red, (int)green, (int)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3iv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glSecondaryColor3iv( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3s(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  try
  {
    glSecondaryColor3s( (GLshort)red, (GLshort)green, (GLshort)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3s( %d, %d, %d )", (int)red, (int)green, (int)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3sv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glSecondaryColor3sv( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3ub(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue
){
  try
  {
    glSecondaryColor3ub( (GLubyte)red, (GLubyte)green, (GLubyte)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3ub( 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3ubv(
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glSecondaryColor3ubv( (const GLubyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3ubv( GLubyte* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3ui(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue
){
  try
  {
    glSecondaryColor3ui( (GLuint)red, (GLuint)green, (GLuint)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3ui( 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3uiv(
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glSecondaryColor3uiv( (const GLuint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3uiv( GLuint* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3us(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  try
  {
    glSecondaryColor3us( (GLushort)red, (GLushort)green, (GLushort)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3us( 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3usv(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glSecondaryColor3usv( (const GLushort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3usv( GLushort* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColorPointer(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glSecondaryColorPointer( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColorPointer( %d, %d, %d, GLvoid* )", (int)size, (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klWindowPos2d(
  const KL::Scalar & x,
  const KL::Scalar & y
){
  try
  {
    glWindowPos2d( (GLdouble)x, (GLdouble)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos2d( %f, %f )", (float)x, (float)y);
  }
}

FABRIC_EXT_EXPORT void klWindowPos2dv(
  const KL::VariableArray<KL::Scalar> & p
){
  try
  {
    glWindowPos2dv( (const GLdouble*)&p[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos2dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klWindowPos2f(
  const KL::Scalar & x,
  const KL::Scalar & y
){
  try
  {
    glWindowPos2f( (GLfloat)x, (GLfloat)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos2f( %f, %f )", (float)x, (float)y);
  }
}

FABRIC_EXT_EXPORT void klWindowPos2fv(
  const KL::VariableArray<KL::Scalar> & p
){
  try
  {
    glWindowPos2fv( (const GLfloat*)&p[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos2fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klWindowPos2i(
  const KL::Integer & x,
  const KL::Integer & y
){
  try
  {
    glWindowPos2i( (GLint)x, (GLint)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos2i( %d, %d )", (int)x, (int)y);
  }
}

FABRIC_EXT_EXPORT void klWindowPos2iv(
  const KL::VariableArray<KL::Integer> & p
){
  try
  {
    glWindowPos2iv( (const GLint*)&p[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos2iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klWindowPos2s(
  const KL::Integer & x,
  const KL::Integer & y
){
  try
  {
    glWindowPos2s( (GLshort)x, (GLshort)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos2s( %d, %d )", (int)x, (int)y);
  }
}

FABRIC_EXT_EXPORT void klWindowPos2sv(
  const KL::VariableArray<KL::Integer> & p
){
  try
  {
    glWindowPos2sv( (const GLshort*)&p[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos2sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klWindowPos3d(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glWindowPos3d( (GLdouble)x, (GLdouble)y, (GLdouble)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos3d( %f, %f, %f )", (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klWindowPos3dv(
  const KL::VariableArray<KL::Scalar> & p
){
  try
  {
    glWindowPos3dv( (const GLdouble*)&p[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos3dv( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klWindowPos3f(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glWindowPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos3f( %f, %f, %f )", (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klWindowPos3fv(
  const KL::VariableArray<KL::Scalar> & p
){
  try
  {
    glWindowPos3fv( (const GLfloat*)&p[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos3fv( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klWindowPos3i(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  try
  {
    glWindowPos3i( (GLint)x, (GLint)y, (GLint)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos3i( %d, %d, %d )", (int)x, (int)y, (int)z);
  }
}

FABRIC_EXT_EXPORT void klWindowPos3iv(
  const KL::VariableArray<KL::Integer> & p
){
  try
  {
    glWindowPos3iv( (const GLint*)&p[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos3iv( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klWindowPos3s(
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  try
  {
    glWindowPos3s( (GLshort)x, (GLshort)y, (GLshort)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos3s( %d, %d, %d )", (int)x, (int)y, (int)z);
  }
}

FABRIC_EXT_EXPORT void klWindowPos3sv(
  const KL::VariableArray<KL::Integer> & p
){
  try
  {
    glWindowPos3sv( (const GLshort*)&p[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWindowPos3sv( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klBeginQuery(
  const KL::Size & target,
  const KL::Size & id
){
  try
  {
    glBeginQuery( (GLenum)target, (GLuint)id );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBeginQuery( %d, 0x%04X )", (int)target, (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klBindBuffer(
  const KL::Size & target,
  const KL::Size & buffer
){
  try
  {
    glBindBuffer( (GLenum)target, (GLuint)buffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindBuffer( %d, 0x%04X )", (int)target, (unsigned)buffer);
  }
}

FABRIC_EXT_EXPORT void klBufferData(
  const KL::Size & target,
  const KL::Size & size,
  KL::Data data,
  const KL::Size & usage
){
  try
  {
    glBufferData( (GLenum)target, (GLsizeiptr)size, data, (GLenum)usage );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBufferData( %d, %d, GLvoid*, %d )", (int)target, (int)size, (int)usage);
  }
}

FABRIC_EXT_EXPORT void klBufferSubData(
  const KL::Size & target,
  const KL::Integer & offset,
  const KL::Size & size,
  KL::Data data
){
  try
  {
    glBufferSubData( (GLenum)target, (GLintptr)offset, (GLsizeiptr)size, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBufferSubData( %d, %d, %d, GLvoid* )", (int)target, (int)offset, (int)size);
  }
}

FABRIC_EXT_EXPORT void klDeleteBuffers(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & buffers
){
  try
  {
    glDeleteBuffers( (GLsizei)n, (const GLuint*)&buffers[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteBuffers( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klDeleteQueries(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & ids
){
  try
  {
    glDeleteQueries( (GLsizei)n, (const GLuint*)&ids[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteQueries( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klEndQuery(
  const KL::Size & target
){
  try
  {
    glEndQuery( (GLenum)target );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEndQuery( %d )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klGenBuffers(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & buffers
){
  try
  {
    glGenBuffers( (GLsizei)n, (GLuint*)&buffers[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenBuffers( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klGenQueries(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & ids
){
  try
  {
    glGenQueries( (GLsizei)n, (GLuint*)&ids[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenQueries( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klGetBufferParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetBufferParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetBufferParameteriv( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetBufferSubData(
  const KL::Size & target,
  const KL::Integer & offset,
  const KL::Size & size,
  KL::Data data
){
  try
  {
    glGetBufferSubData( (GLenum)target, (GLintptr)offset, (GLsizeiptr)size, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetBufferSubData( %d, %d, %d, GLvoid* )", (int)target, (int)offset, (int)size);
  }
}

FABRIC_EXT_EXPORT void klGetQueryObjectiv(
  const KL::Size & id,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetQueryObjectiv( (GLuint)id, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetQueryObjectiv( 0x%04X, %d, GLint* )", (unsigned)id, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetQueryObjectuiv(
  const KL::Size & id,
  const KL::Size & pname,
  KL::VariableArray<KL::Size> & params
){
  try
  {
    glGetQueryObjectuiv( (GLuint)id, (GLenum)pname, (GLuint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetQueryObjectuiv( 0x%04X, %d, GLuint* )", (unsigned)id, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetQueryiv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetQueryiv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetQueryiv( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsBuffer(
  const KL::Size & buffer
){
  try
  {
    GLboolean result = glIsBuffer( (GLuint)buffer );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsBuffer( 0x%04X )", (unsigned)buffer);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsQuery(
  const KL::Size & id
){
  try
  {
    GLboolean result = glIsQuery( (GLuint)id );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsQuery( 0x%04X )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT GLvoid* klMapBuffer(
  const KL::Size & target,
  const KL::Size & access
){
  try
  {
    glMapBuffer( (GLenum)target, (GLenum)access );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMapBuffer( %d, %d )", (int)target, (int)access);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klUnmapBuffer(
  const KL::Size & target
){
  try
  {
    GLboolean result = glUnmapBuffer( (GLenum)target );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUnmapBuffer( %d )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klAttachShader(
  const KL::Size & program,
  const KL::Size & shader
){
  try
  {
    glAttachShader( (GLuint)program, (GLuint)shader );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glAttachShader( 0x%04X, 0x%04X )", (unsigned)program, (unsigned)shader);
  }
}

FABRIC_EXT_EXPORT void klBindAttribLocation(
  const KL::Size & program,
  const KL::Size & index,
  const KL::String &name
){
  try
  {
    glBindAttribLocation( (GLuint)program, (GLuint)index, (const GLchar*)name.data() );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindAttribLocation( 0x%04X, 0x%04X, char* )", (unsigned)program, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klBlendEquationSeparate(
  const KL::Size & var0,
  const KL::Size & var1
){
  try
  {
    glBlendEquationSeparate( (GLenum)var0, (GLenum)var1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBlendEquationSeparate( %d, %d )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klCompileShader(
  const KL::Size & shader
){
  try
  {
    glCompileShader( (GLuint)shader );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompileShader( 0x%04X )", (unsigned)shader);
  }
}

FABRIC_EXT_EXPORT KL::Size klCreateProgram()
{
  try
  {
    GLuint result = glCreateProgram();
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCreateProgram(  )");
  }
}

FABRIC_EXT_EXPORT KL::Size klCreateShader(
  const KL::Size & type
){
  try
  {
    GLuint result = glCreateShader( (GLenum)type );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCreateShader( %d )", (int)type);
  }
}

FABRIC_EXT_EXPORT void klDeleteProgram(
  const KL::Size & program
){
  try
  {
    glDeleteProgram( (GLuint)program );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteProgram( 0x%04X )", (unsigned)program);
  }
}

FABRIC_EXT_EXPORT void klDeleteShader(
  const KL::Size & shader
){
  try
  {
    glDeleteShader( (GLuint)shader );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteShader( 0x%04X )", (unsigned)shader);
  }
}

FABRIC_EXT_EXPORT void klDetachShader(
  const KL::Size & program,
  const KL::Size & shader
){
  try
  {
    glDetachShader( (GLuint)program, (GLuint)shader );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDetachShader( 0x%04X, 0x%04X )", (unsigned)program, (unsigned)shader);
  }
}

FABRIC_EXT_EXPORT void klDisableVertexAttribArray(
  const KL::Size & var0
){
  try
  {
    glDisableVertexAttribArray( (GLuint)var0 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDisableVertexAttribArray( 0x%04X )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klDrawBuffers(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & bufs
){
  try
  {
    glDrawBuffers( (GLsizei)n, (const GLenum*)&bufs[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawBuffers( %d, GLenum* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klEnableVertexAttribArray(
  const KL::Size & var0
){
  try
  {
    glEnableVertexAttribArray( (GLuint)var0 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEnableVertexAttribArray( 0x%04X )", (unsigned)var0);
  }
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
  try
  {
    char * nameStr = new char[1024];
    glGetActiveAttrib( (GLuint)program, (GLuint)index, (GLsizei)maxLength, (GLsizei*)&length[0], (GLint*)&size[0], (GLenum*)&type[0], (GLchar*)nameStr );
    name = KL::String(nameStr);
    delete( nameStr );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetActiveAttrib( 0x%04X, 0x%04X, %d, GLsizei*, GLint*, GLenum*, char* )", (unsigned)program, (unsigned)index, (int)maxLength);
  }
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
  try
  {
    char * nameStr = new char[1024];
    glGetActiveUniform( (GLuint)program, (GLuint)index, (GLsizei)maxLength, (GLsizei*)&length[0], (GLint*)&size[0], (GLenum*)&type[0], (GLchar*)nameStr );
    name = KL::String(nameStr);
    delete( nameStr );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetActiveUniform( 0x%04X, 0x%04X, %d, GLsizei*, GLint*, GLenum*, char* )", (unsigned)program, (unsigned)index, (int)maxLength);
  }
}

FABRIC_EXT_EXPORT void klGetAttachedShaders(
  const KL::Size & program,
  const KL::Size & maxCount,
  KL::VariableArray<KL::Size> & count,
  KL::VariableArray<KL::Size> & shaders
){
  try
  {
    glGetAttachedShaders( (GLuint)program, (GLsizei)maxCount, (GLsizei*)&count[0], (GLuint*)&shaders[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetAttachedShaders( 0x%04X, %d, GLsizei*, GLuint* )", (unsigned)program, (int)maxCount);
  }
}

FABRIC_EXT_EXPORT KL::Integer klGetAttribLocation(
  const KL::Size & program,
  const KL::String &name
){
  try
  {
    GLint result = glGetAttribLocation( (GLuint)program, (const GLchar*)name.data() );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetAttribLocation( 0x%04X, char* )", (unsigned)program);
  }
}

FABRIC_EXT_EXPORT void klGetProgramInfoLog(
  const KL::Size & program,
  const KL::Size & bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::String & infoLog
){
  try
  {
    char * infoLogStr = new char[1024];
    glGetProgramInfoLog( (GLuint)program, (GLsizei)bufSize, (GLsizei*)&length[0], (GLchar*)infoLogStr );
    infoLog = KL::String(infoLogStr);
    delete( infoLogStr );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetProgramInfoLog( 0x%04X, %d, GLsizei*, char* )", (unsigned)program, (int)bufSize);
  }
}

FABRIC_EXT_EXPORT void klGetProgramiv(
  const KL::Size & program,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & param
){
  try
  {
    glGetProgramiv( (GLuint)program, (GLenum)pname, (GLint*)&param[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetProgramiv( 0x%04X, %d, GLint* )", (unsigned)program, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetShaderInfoLog(
  const KL::Size & shader,
  const KL::Size & bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::String & infoLog
){
  try
  {
    char * infoLogStr = new char[1024];
    glGetShaderInfoLog( (GLuint)shader, (GLsizei)bufSize, (GLsizei*)&length[0], (GLchar*)infoLogStr );
    infoLog = KL::String(infoLogStr);
    delete( infoLogStr );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetShaderInfoLog( 0x%04X, %d, GLsizei*, char* )", (unsigned)shader, (int)bufSize);
  }
}

FABRIC_EXT_EXPORT void klGetShaderSource(
  const KL::Integer & obj,
  const KL::Size & maxLength,
  KL::VariableArray<KL::Size> & length,
  KL::String & source
){
  try
  {
    char * sourceStr = new char[1024];
    glGetShaderSource( (GLint)obj, (GLsizei)maxLength, (GLsizei*)&length[0], (GLchar*)sourceStr );
    source = KL::String(sourceStr);
    delete( sourceStr );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetShaderSource( %d, %d, GLsizei*, char* )", (int)obj, (int)maxLength);
  }
}

FABRIC_EXT_EXPORT void klGetShaderiv(
  const KL::Size & shader,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & param
){
  try
  {
    glGetShaderiv( (GLuint)shader, (GLenum)pname, (GLint*)&param[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetShaderiv( 0x%04X, %d, GLint* )", (unsigned)shader, (int)pname);
  }
}

FABRIC_EXT_EXPORT KL::Integer klGetUniformLocation(
  const KL::Size & program,
  const KL::String &name
){
  try
  {
    GLint result = glGetUniformLocation( (GLuint)program, (const GLchar*)name.data() );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetUniformLocation( 0x%04X, char* )", (unsigned)program);
  }
}

FABRIC_EXT_EXPORT void klGetUniformfv(
  const KL::Size & program,
  const KL::Integer & location,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetUniformfv( (GLuint)program, (GLint)location, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetUniformfv( 0x%04X, %d, GLfloat* )", (unsigned)program, (int)location);
  }
}

FABRIC_EXT_EXPORT void klGetUniformiv(
  const KL::Size & program,
  const KL::Integer & location,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetUniformiv( (GLuint)program, (GLint)location, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetUniformiv( 0x%04X, %d, GLint* )", (unsigned)program, (int)location);
  }
}

FABRIC_EXT_EXPORT void klGetVertexAttribPointerv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::Data var2
){
  try
  {
    glGetVertexAttribPointerv( (GLuint)var0, (GLenum)var1, var2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVertexAttribPointerv( 0x%04X, %d, GLvoid* )", (unsigned)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klGetVertexAttribdv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Scalar> & var2
){
  try
  {
    glGetVertexAttribdv( (GLuint)var0, (GLenum)var1, (GLdouble*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVertexAttribdv( 0x%04X, %d, GLdouble* )", (unsigned)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klGetVertexAttribfv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Scalar> & var2
){
  try
  {
    glGetVertexAttribfv( (GLuint)var0, (GLenum)var1, (GLfloat*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVertexAttribfv( 0x%04X, %d, GLfloat* )", (unsigned)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klGetVertexAttribiv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Integer> & var2
){
  try
  {
    glGetVertexAttribiv( (GLuint)var0, (GLenum)var1, (GLint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVertexAttribiv( 0x%04X, %d, GLint* )", (unsigned)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsProgram(
  const KL::Size & program
){
  try
  {
    GLboolean result = glIsProgram( (GLuint)program );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsProgram( 0x%04X )", (unsigned)program);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsShader(
  const KL::Size & shader
){
  try
  {
    GLboolean result = glIsShader( (GLuint)shader );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsShader( 0x%04X )", (unsigned)shader);
  }
}

FABRIC_EXT_EXPORT void klLinkProgram(
  const KL::Size & program
){
  try
  {
    glLinkProgram( (GLuint)program );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLinkProgram( 0x%04X )", (unsigned)program);
  }
}

FABRIC_EXT_EXPORT void klShaderSource(
  const KL::Size & shader,
  const KL::Size & count,
  const KL::VariableArray<KL::String> &strings,
  const KL::VariableArray<KL::Integer> & lengths
){
  try
  {
    glShaderSource( (GLuint)shader, (GLsizei)count, (const GLchar**)&strings[0], (const GLint*)&lengths[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glShaderSource( 0x%04X, %d, char*, GLint* )", (unsigned)shader, (int)count);
  }
}

FABRIC_EXT_EXPORT void klStencilFuncSeparate(
  const KL::Size & frontfunc,
  const KL::Size & backfunc,
  const KL::Integer & ref,
  const KL::Size & mask
){
  try
  {
    glStencilFuncSeparate( (GLenum)frontfunc, (GLenum)backfunc, (GLint)ref, (GLuint)mask );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glStencilFuncSeparate( %d, %d, %d, 0x%04X )", (int)frontfunc, (int)backfunc, (int)ref, (unsigned)mask);
  }
}

FABRIC_EXT_EXPORT void klStencilMaskSeparate(
  const KL::Size & var0,
  const KL::Size & var1
){
  try
  {
    glStencilMaskSeparate( (GLenum)var0, (GLuint)var1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glStencilMaskSeparate( %d, 0x%04X )", (int)var0, (unsigned)var1);
  }
}

FABRIC_EXT_EXPORT void klStencilOpSeparate(
  const KL::Size & face,
  const KL::Size & sfail,
  const KL::Size & dpfail,
  const KL::Size & dppass
){
  try
  {
    glStencilOpSeparate( (GLenum)face, (GLenum)sfail, (GLenum)dpfail, (GLenum)dppass );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glStencilOpSeparate( %d, %d, %d, %d )", (int)face, (int)sfail, (int)dpfail, (int)dppass);
  }
}

FABRIC_EXT_EXPORT void klUniform1f(
  const KL::Integer & location,
  const KL::Scalar & v0
){
  try
  {
    glUniform1f( (GLint)location, (GLfloat)v0 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform1f( %d, %f )", (int)location, (float)v0);
  }
}

FABRIC_EXT_EXPORT void klUniform1fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniform1fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform1fv( %d, %d, GLfloat* )", (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klUniform1i(
  const KL::Integer & location,
  const KL::Integer & v0
){
  try
  {
    glUniform1i( (GLint)location, (GLint)v0 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform1i( %d, %d )", (int)location, (int)v0);
  }
}

FABRIC_EXT_EXPORT void klUniform1iv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  try
  {
    glUniform1iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform1iv( %d, %d, GLint* )", (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klUniform2f(
  const KL::Integer & location,
  const KL::Scalar & v0,
  const KL::Scalar & v1
){
  try
  {
    glUniform2f( (GLint)location, (GLfloat)v0, (GLfloat)v1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform2f( %d, %f, %f )", (int)location, (float)v0, (float)v1);
  }
}

FABRIC_EXT_EXPORT void klUniform2fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniform2fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform2fv( %d, %d, GLfloat* )", (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klUniform2i(
  const KL::Integer & location,
  const KL::Integer & v0,
  const KL::Integer & v1
){
  try
  {
    glUniform2i( (GLint)location, (GLint)v0, (GLint)v1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform2i( %d, %d, %d )", (int)location, (int)v0, (int)v1);
  }
}

FABRIC_EXT_EXPORT void klUniform2iv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  try
  {
    glUniform2iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform2iv( %d, %d, GLint* )", (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klUniform3f(
  const KL::Integer & location,
  const KL::Scalar & v0,
  const KL::Scalar & v1,
  const KL::Scalar & v2
){
  try
  {
    glUniform3f( (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform3f( %d, %f, %f, %f )", (int)location, (float)v0, (float)v1, (float)v2);
  }
}

FABRIC_EXT_EXPORT void klUniform3fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniform3fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform3fv( %d, %d, GLfloat* )", (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klUniform3i(
  const KL::Integer & location,
  const KL::Integer & v0,
  const KL::Integer & v1,
  const KL::Integer & v2
){
  try
  {
    glUniform3i( (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform3i( %d, %d, %d, %d )", (int)location, (int)v0, (int)v1, (int)v2);
  }
}

FABRIC_EXT_EXPORT void klUniform3iv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  try
  {
    glUniform3iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform3iv( %d, %d, GLint* )", (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klUniform4f(
  const KL::Integer & location,
  const KL::Scalar & v0,
  const KL::Scalar & v1,
  const KL::Scalar & v2,
  const KL::Scalar & v3
){
  try
  {
    glUniform4f( (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2, (GLfloat)v3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform4f( %d, %f, %f, %f, %f )", (int)location, (float)v0, (float)v1, (float)v2, (float)v3);
  }
}

FABRIC_EXT_EXPORT void klUniform4fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniform4fv( (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform4fv( %d, %d, GLfloat* )", (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klUniform4i(
  const KL::Integer & location,
  const KL::Integer & v0,
  const KL::Integer & v1,
  const KL::Integer & v2,
  const KL::Integer & v3
){
  try
  {
    glUniform4i( (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2, (GLint)v3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform4i( %d, %d, %d, %d, %d )", (int)location, (int)v0, (int)v1, (int)v2, (int)v3);
  }
}

FABRIC_EXT_EXPORT void klUniform4iv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  try
  {
    glUniform4iv( (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform4iv( %d, %d, GLint* )", (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klUniformMatrix2fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniformMatrix2fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniformMatrix2fv( %d, %d, %b, GLfloat* )", (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klUniformMatrix3fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniformMatrix3fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniformMatrix3fv( %d, %d, %b, GLfloat* )", (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klUniformMatrix4fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniformMatrix4fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniformMatrix4fv( %d, %d, %b, GLfloat* )", (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klUseProgram(
  const KL::Size & program
){
  try
  {
    glUseProgram( (GLuint)program );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUseProgram( 0x%04X )", (unsigned)program);
  }
}

FABRIC_EXT_EXPORT void klValidateProgram(
  const KL::Size & program
){
  try
  {
    glValidateProgram( (GLuint)program );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glValidateProgram( 0x%04X )", (unsigned)program);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib1d(
  const KL::Size & index,
  const KL::Scalar & x
){
  try
  {
    glVertexAttrib1d( (GLuint)index, (GLdouble)x );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib1d( 0x%04X, %f )", (unsigned)index, (float)x);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib1dv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertexAttrib1dv( (GLuint)index, (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib1dv( 0x%04X, GLdouble* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib1f(
  const KL::Size & index,
  const KL::Scalar & x
){
  try
  {
    glVertexAttrib1f( (GLuint)index, (GLfloat)x );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib1f( 0x%04X, %f )", (unsigned)index, (float)x);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib1fv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertexAttrib1fv( (GLuint)index, (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib1fv( 0x%04X, GLfloat* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib1s(
  const KL::Size & index,
  const KL::Integer & x
){
  try
  {
    glVertexAttrib1s( (GLuint)index, (GLshort)x );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib1s( 0x%04X, %d )", (unsigned)index, (int)x);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib1sv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttrib1sv( (GLuint)index, (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib1sv( 0x%04X, GLshort* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib2d(
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y
){
  try
  {
    glVertexAttrib2d( (GLuint)index, (GLdouble)x, (GLdouble)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib2d( 0x%04X, %f, %f )", (unsigned)index, (float)x, (float)y);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib2dv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertexAttrib2dv( (GLuint)index, (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib2dv( 0x%04X, GLdouble* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib2f(
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y
){
  try
  {
    glVertexAttrib2f( (GLuint)index, (GLfloat)x, (GLfloat)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib2f( 0x%04X, %f, %f )", (unsigned)index, (float)x, (float)y);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib2fv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertexAttrib2fv( (GLuint)index, (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib2fv( 0x%04X, GLfloat* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib2s(
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y
){
  try
  {
    glVertexAttrib2s( (GLuint)index, (GLshort)x, (GLshort)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib2s( 0x%04X, %d, %d )", (unsigned)index, (int)x, (int)y);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib2sv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttrib2sv( (GLuint)index, (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib2sv( 0x%04X, GLshort* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib3d(
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glVertexAttrib3d( (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib3d( 0x%04X, %f, %f, %f )", (unsigned)index, (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib3dv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertexAttrib3dv( (GLuint)index, (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib3dv( 0x%04X, GLdouble* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib3f(
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glVertexAttrib3f( (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib3f( 0x%04X, %f, %f, %f )", (unsigned)index, (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib3fv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertexAttrib3fv( (GLuint)index, (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib3fv( 0x%04X, GLfloat* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib3s(
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  try
  {
    glVertexAttrib3s( (GLuint)index, (GLshort)x, (GLshort)y, (GLshort)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib3s( 0x%04X, %d, %d, %d )", (unsigned)index, (int)x, (int)y, (int)z);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib3sv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttrib3sv( (GLuint)index, (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib3sv( 0x%04X, GLshort* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4Nbv(
  const KL::Size & index,
  const KL::VariableArray<KL::Byte> & v
){
  try
  {
    glVertexAttrib4Nbv( (GLuint)index, (const GLbyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4Nbv( 0x%04X, GLbyte* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4Niv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttrib4Niv( (GLuint)index, (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4Niv( 0x%04X, GLint* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4Nsv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttrib4Nsv( (GLuint)index, (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4Nsv( 0x%04X, GLshort* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4Nub(
  const KL::Size & index,
  const KL::Size & x,
  const KL::Size & y,
  const KL::Size & z,
  const KL::Size & w
){
  try
  {
    glVertexAttrib4Nub( (GLuint)index, (GLubyte)x, (GLubyte)y, (GLubyte)z, (GLubyte)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4Nub( 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (unsigned)index, (unsigned)x, (unsigned)y, (unsigned)z, (unsigned)w);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4Nubv(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glVertexAttrib4Nubv( (GLuint)index, (const GLubyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4Nubv( 0x%04X, GLubyte* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4Nuiv(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glVertexAttrib4Nuiv( (GLuint)index, (const GLuint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4Nuiv( 0x%04X, GLuint* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4Nusv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttrib4Nusv( (GLuint)index, (const GLushort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4Nusv( 0x%04X, GLushort* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4bv(
  const KL::Size & index,
  const KL::VariableArray<KL::Byte> & v
){
  try
  {
    glVertexAttrib4bv( (GLuint)index, (const GLbyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4bv( 0x%04X, GLbyte* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4d(
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  try
  {
    glVertexAttrib4d( (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4d( 0x%04X, %f, %f, %f, %f )", (unsigned)index, (float)x, (float)y, (float)z, (float)w);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4dv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertexAttrib4dv( (GLuint)index, (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4dv( 0x%04X, GLdouble* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4f(
  const KL::Size & index,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z,
  const KL::Scalar & w
){
  try
  {
    glVertexAttrib4f( (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4f( 0x%04X, %f, %f, %f, %f )", (unsigned)index, (float)x, (float)y, (float)z, (float)w);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4fv(
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glVertexAttrib4fv( (GLuint)index, (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4fv( 0x%04X, GLfloat* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4iv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttrib4iv( (GLuint)index, (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4iv( 0x%04X, GLint* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4s(
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  try
  {
    glVertexAttrib4s( (GLuint)index, (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4s( 0x%04X, %d, %d, %d, %d )", (unsigned)index, (int)x, (int)y, (int)z, (int)w);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4sv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttrib4sv( (GLuint)index, (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4sv( 0x%04X, GLshort* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4ubv(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glVertexAttrib4ubv( (GLuint)index, (const GLubyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4ubv( 0x%04X, GLubyte* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4uiv(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glVertexAttrib4uiv( (GLuint)index, (const GLuint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4uiv( 0x%04X, GLuint* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttrib4usv(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttrib4usv( (GLuint)index, (const GLushort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttrib4usv( 0x%04X, GLushort* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribPointer(
  const KL::Size & index,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Boolean & normalized,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glVertexAttribPointer( (GLuint)index, (GLint)size, (GLenum)type, (GLboolean)normalized, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribPointer( 0x%04X, %d, %d, %b, %d, GLvoid* )", (unsigned)index, (int)size, (int)type, (bool)normalized, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klUniformMatrix2x3fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniformMatrix2x3fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniformMatrix2x3fv( %d, %d, %b, GLfloat* )", (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klUniformMatrix2x4fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniformMatrix2x4fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniformMatrix2x4fv( %d, %d, %b, GLfloat* )", (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klUniformMatrix3x2fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniformMatrix3x2fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniformMatrix3x2fv( %d, %d, %b, GLfloat* )", (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klUniformMatrix3x4fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniformMatrix3x4fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniformMatrix3x4fv( %d, %d, %b, GLfloat* )", (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klUniformMatrix4x2fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniformMatrix4x2fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniformMatrix4x2fv( %d, %d, %b, GLfloat* )", (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klUniformMatrix4x3fv(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glUniformMatrix4x3fv( (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniformMatrix4x3fv( %d, %d, %b, GLfloat* )", (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klBeginConditionalRender(
  const KL::Size & var0,
  const KL::Size & var1
){
  try
  {
    glBeginConditionalRender( (GLuint)var0, (GLenum)var1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBeginConditionalRender( 0x%04X, %d )", (unsigned)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klBeginTransformFeedback(
  const KL::Size & var0
){
  try
  {
    glBeginTransformFeedback( (GLenum)var0 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBeginTransformFeedback( %d )", (int)var0);
  }
}

FABRIC_EXT_EXPORT void klBindFragDataLocation(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::String &var2
){
  try
  {
    glBindFragDataLocation( (GLuint)var0, (GLuint)var1, (const GLchar*)var2.data() );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindFragDataLocation( 0x%04X, 0x%04X, char* )", (unsigned)var0, (unsigned)var1);
  }
}

FABRIC_EXT_EXPORT void klClampColor(
  const KL::Size & var0,
  const KL::Size & var1
){
  try
  {
    glClampColor( (GLenum)var0, (GLenum)var1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClampColor( %d, %d )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klClearBufferfi(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::Scalar & var2,
  const KL::Integer & var3
){
  try
  {
    glClearBufferfi( (GLenum)var0, (GLint)var1, (GLfloat)var2, (GLint)var3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClearBufferfi( %d, %d, %f, %d )", (int)var0, (int)var1, (float)var2, (int)var3);
  }
}

FABRIC_EXT_EXPORT void klClearBufferfv(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::VariableArray<KL::Scalar> & var2
){
  try
  {
    glClearBufferfv( (GLenum)var0, (GLint)var1, (const GLfloat*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClearBufferfv( %d, %d, GLfloat* )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klClearBufferiv(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::VariableArray<KL::Integer> & var2
){
  try
  {
    glClearBufferiv( (GLenum)var0, (GLint)var1, (const GLint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClearBufferiv( %d, %d, GLint* )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klClearBufferuiv(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::VariableArray<KL::Size> & var2
){
  try
  {
    glClearBufferuiv( (GLenum)var0, (GLint)var1, (const GLuint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClearBufferuiv( %d, %d, GLuint* )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klColorMaski(
  const KL::Size & var0,
  const KL::Boolean & var1,
  const KL::Boolean & var2,
  const KL::Boolean & var3,
  const KL::Boolean & var4
){
  try
  {
    glColorMaski( (GLuint)var0, (GLboolean)var1, (GLboolean)var2, (GLboolean)var3, (GLboolean)var4 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColorMaski( 0x%04X, %b, %b, %b, %b )", (unsigned)var0, (bool)var1, (bool)var2, (bool)var3, (bool)var4);
  }
}

FABRIC_EXT_EXPORT void klDisablei(
  const KL::Size & var0,
  const KL::Size & var1
){
  try
  {
    glDisablei( (GLenum)var0, (GLuint)var1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDisablei( %d, 0x%04X )", (int)var0, (unsigned)var1);
  }
}

FABRIC_EXT_EXPORT void klEnablei(
  const KL::Size & var0,
  const KL::Size & var1
){
  try
  {
    glEnablei( (GLenum)var0, (GLuint)var1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEnablei( %d, 0x%04X )", (int)var0, (unsigned)var1);
  }
}

FABRIC_EXT_EXPORT void klEndConditionalRender()
{
  try
  {
    glEndConditionalRender();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEndConditionalRender(  )");
  }
}

FABRIC_EXT_EXPORT void klEndTransformFeedback()
{
  try
  {
    glEndTransformFeedback();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEndTransformFeedback(  )");
  }
}

FABRIC_EXT_EXPORT void klGetBooleani_v(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Boolean> & var2
){
  try
  {
    glGetBooleani_v( (GLenum)var0, (GLuint)var1, (GLboolean*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetBooleani_v( %d, 0x%04X, GLboolean* )", (int)var0, (unsigned)var1);
  }
}

FABRIC_EXT_EXPORT KL::Integer klGetFragDataLocation(
  const KL::Size & var0,
  const KL::String &var1
){
  try
  {
    GLint result = glGetFragDataLocation( (GLuint)var0, (const GLchar*)var1.data() );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetFragDataLocation( 0x%04X, char* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klGetTexParameterIiv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Integer> & var2
){
  try
  {
    glGetTexParameterIiv( (GLenum)var0, (GLenum)var1, (GLint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexParameterIiv( %d, %d, GLint* )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klGetTexParameterIuiv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Size> & var2
){
  try
  {
    glGetTexParameterIuiv( (GLenum)var0, (GLenum)var1, (GLuint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexParameterIuiv( %d, %d, GLuint* )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klGetTransformFeedbackVarying(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Integer> & var2
){
  try
  {
    glGetTransformFeedbackVarying( (GLuint)var0, (GLuint)var1, (GLint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTransformFeedbackVarying( 0x%04X, 0x%04X, GLint* )", (unsigned)var0, (unsigned)var1);
  }
}

FABRIC_EXT_EXPORT void klGetUniformuiv(
  const KL::Size & var0,
  const KL::Integer & var1,
  KL::VariableArray<KL::Size> & var2
){
  try
  {
    glGetUniformuiv( (GLuint)var0, (GLint)var1, (GLuint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetUniformuiv( 0x%04X, %d, GLuint* )", (unsigned)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klGetVertexAttribIiv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Integer> & var2
){
  try
  {
    glGetVertexAttribIiv( (GLuint)var0, (GLenum)var1, (GLint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVertexAttribIiv( 0x%04X, %d, GLint* )", (unsigned)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klGetVertexAttribIuiv(
  const KL::Size & var0,
  const KL::Size & var1,
  KL::VariableArray<KL::Size> & var2
){
  try
  {
    glGetVertexAttribIuiv( (GLuint)var0, (GLenum)var1, (GLuint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVertexAttribIuiv( 0x%04X, %d, GLuint* )", (unsigned)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsEnabledi(
  const KL::Size & var0,
  const KL::Size & var1
){
  try
  {
    GLboolean result = glIsEnabledi( (GLenum)var0, (GLuint)var1 );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsEnabledi( %d, 0x%04X )", (int)var0, (unsigned)var1);
  }
}

FABRIC_EXT_EXPORT void klTexParameterIiv(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::Integer> & var2
){
  try
  {
    glTexParameterIiv( (GLenum)var0, (GLenum)var1, (const GLint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexParameterIiv( %d, %d, GLint* )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klTexParameterIuiv(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::Size> & var2
){
  try
  {
    glTexParameterIuiv( (GLenum)var0, (GLenum)var1, (const GLuint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexParameterIuiv( %d, %d, GLuint* )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klTransformFeedbackVaryings(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::String> &var2,
  const KL::Size & var3
){
  try
  {
    glTransformFeedbackVaryings( (GLuint)var0, (GLsizei)var1, (const GLchar**)&var2[0], (GLenum)var3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTransformFeedbackVaryings( 0x%04X, %d, char*, %d )", (unsigned)var0, (int)var1, (int)var3);
  }
}

FABRIC_EXT_EXPORT void klUniform1ui(
  const KL::Integer & var0,
  const KL::Size & var1
){
  try
  {
    glUniform1ui( (GLint)var0, (GLuint)var1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform1ui( %d, 0x%04X )", (int)var0, (unsigned)var1);
  }
}

FABRIC_EXT_EXPORT void klUniform1uiv(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::Size> & var2
){
  try
  {
    glUniform1uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform1uiv( %d, %d, GLuint* )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klUniform2ui(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::Size & var2
){
  try
  {
    glUniform2ui( (GLint)var0, (GLuint)var1, (GLuint)var2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform2ui( %d, 0x%04X, 0x%04X )", (int)var0, (unsigned)var1, (unsigned)var2);
  }
}

FABRIC_EXT_EXPORT void klUniform2uiv(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::Size> & var2
){
  try
  {
    glUniform2uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform2uiv( %d, %d, GLuint* )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klUniform3ui(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::Size & var2,
  const KL::Size & var3
){
  try
  {
    glUniform3ui( (GLint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform3ui( %d, 0x%04X, 0x%04X, 0x%04X )", (int)var0, (unsigned)var1, (unsigned)var2, (unsigned)var3);
  }
}

FABRIC_EXT_EXPORT void klUniform3uiv(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::Size> & var2
){
  try
  {
    glUniform3uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform3uiv( %d, %d, GLuint* )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klUniform4ui(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::Size & var2,
  const KL::Size & var3,
  const KL::Size & var4
){
  try
  {
    glUniform4ui( (GLint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3, (GLuint)var4 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform4ui( %d, 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (int)var0, (unsigned)var1, (unsigned)var2, (unsigned)var3, (unsigned)var4);
  }
}

FABRIC_EXT_EXPORT void klUniform4uiv(
  const KL::Integer & var0,
  const KL::Size & var1,
  const KL::VariableArray<KL::Size> & var2
){
  try
  {
    glUniform4uiv( (GLint)var0, (GLsizei)var1, (const GLuint*)&var2[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform4uiv( %d, %d, GLuint* )", (int)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI1i(
  const KL::Size & var0,
  const KL::Integer & var1
){
  try
  {
    glVertexAttribI1i( (GLuint)var0, (GLint)var1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI1i( 0x%04X, %d )", (unsigned)var0, (int)var1);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI1iv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Integer> & var1
){
  try
  {
    glVertexAttribI1iv( (GLuint)var0, (const GLint*)&var1[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI1iv( 0x%04X, GLint* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI1ui(
  const KL::Size & var0,
  const KL::Size & var1
){
  try
  {
    glVertexAttribI1ui( (GLuint)var0, (GLuint)var1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI1ui( 0x%04X, 0x%04X )", (unsigned)var0, (unsigned)var1);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI1uiv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Size> & var1
){
  try
  {
    glVertexAttribI1uiv( (GLuint)var0, (const GLuint*)&var1[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI1uiv( 0x%04X, GLuint* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI2i(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::Integer & var2
){
  try
  {
    glVertexAttribI2i( (GLuint)var0, (GLint)var1, (GLint)var2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI2i( 0x%04X, %d, %d )", (unsigned)var0, (int)var1, (int)var2);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI2iv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Integer> & var1
){
  try
  {
    glVertexAttribI2iv( (GLuint)var0, (const GLint*)&var1[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI2iv( 0x%04X, GLint* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI2ui(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::Size & var2
){
  try
  {
    glVertexAttribI2ui( (GLuint)var0, (GLuint)var1, (GLuint)var2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI2ui( 0x%04X, 0x%04X, 0x%04X )", (unsigned)var0, (unsigned)var1, (unsigned)var2);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI2uiv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Size> & var1
){
  try
  {
    glVertexAttribI2uiv( (GLuint)var0, (const GLuint*)&var1[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI2uiv( 0x%04X, GLuint* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI3i(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::Integer & var2,
  const KL::Integer & var3
){
  try
  {
    glVertexAttribI3i( (GLuint)var0, (GLint)var1, (GLint)var2, (GLint)var3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI3i( 0x%04X, %d, %d, %d )", (unsigned)var0, (int)var1, (int)var2, (int)var3);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI3iv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Integer> & var1
){
  try
  {
    glVertexAttribI3iv( (GLuint)var0, (const GLint*)&var1[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI3iv( 0x%04X, GLint* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI3ui(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::Size & var2,
  const KL::Size & var3
){
  try
  {
    glVertexAttribI3ui( (GLuint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI3ui( 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (unsigned)var0, (unsigned)var1, (unsigned)var2, (unsigned)var3);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI3uiv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Size> & var1
){
  try
  {
    glVertexAttribI3uiv( (GLuint)var0, (const GLuint*)&var1[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI3uiv( 0x%04X, GLuint* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4bv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Byte> & var1
){
  try
  {
    glVertexAttribI4bv( (GLuint)var0, (const GLbyte*)&var1[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4bv( 0x%04X, GLbyte* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4i(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::Integer & var2,
  const KL::Integer & var3,
  const KL::Integer & var4
){
  try
  {
    glVertexAttribI4i( (GLuint)var0, (GLint)var1, (GLint)var2, (GLint)var3, (GLint)var4 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4i( 0x%04X, %d, %d, %d, %d )", (unsigned)var0, (int)var1, (int)var2, (int)var3, (int)var4);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4iv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Integer> & var1
){
  try
  {
    glVertexAttribI4iv( (GLuint)var0, (const GLint*)&var1[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4iv( 0x%04X, GLint* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4sv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Integer> & var1
){
  try
  {
    glVertexAttribI4sv( (GLuint)var0, (const GLshort*)&var1[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4sv( 0x%04X, GLshort* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4ubv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Size> & var1
){
  try
  {
    glVertexAttribI4ubv( (GLuint)var0, (const GLubyte*)&var1[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4ubv( 0x%04X, GLubyte* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4ui(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::Size & var2,
  const KL::Size & var3,
  const KL::Size & var4
){
  try
  {
    glVertexAttribI4ui( (GLuint)var0, (GLuint)var1, (GLuint)var2, (GLuint)var3, (GLuint)var4 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4ui( 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (unsigned)var0, (unsigned)var1, (unsigned)var2, (unsigned)var3, (unsigned)var4);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4uiv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Size> & var1
){
  try
  {
    glVertexAttribI4uiv( (GLuint)var0, (const GLuint*)&var1[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4uiv( 0x%04X, GLuint* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4usv(
  const KL::Size & var0,
  const KL::VariableArray<KL::Integer> & var1
){
  try
  {
    glVertexAttribI4usv( (GLuint)var0, (const GLushort*)&var1[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4usv( 0x%04X, GLushort* )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribIPointer(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::Size & var2,
  const KL::Size & var3,
  KL::Data var4
){
  try
  {
    glVertexAttribIPointer( (GLuint)var0, (GLint)var1, (GLenum)var2, (GLsizei)var3, var4 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribIPointer( 0x%04X, %d, %d, %d, GLvoid* )", (unsigned)var0, (int)var1, (int)var2, (int)var3);
  }
}

FABRIC_EXT_EXPORT void klDrawArraysInstanced(
  const KL::Size & var0,
  const KL::Integer & var1,
  const KL::Size & var2,
  const KL::Size & var3
){
  try
  {
    glDrawArraysInstanced( (GLenum)var0, (GLint)var1, (GLsizei)var2, (GLsizei)var3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawArraysInstanced( %d, %d, %d, %d )", (int)var0, (int)var1, (int)var2, (int)var3);
  }
}

FABRIC_EXT_EXPORT void klDrawElementsInstanced(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::Size & var2,
  KL::Data var3,
  const KL::Size & var4
){
  try
  {
    glDrawElementsInstanced( (GLenum)var0, (GLsizei)var1, (GLenum)var2, var3, (GLsizei)var4 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawElementsInstanced( %d, %d, %d, GLvoid*, %d )", (int)var0, (int)var1, (int)var2, (int)var4);
  }
}

FABRIC_EXT_EXPORT void klPrimitiveRestartIndex(
  const KL::Size & var0
){
  try
  {
    glPrimitiveRestartIndex( (GLuint)var0 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPrimitiveRestartIndex( 0x%04X )", (unsigned)var0);
  }
}

FABRIC_EXT_EXPORT void klTexBuffer(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::Size & var2
){
  try
  {
    glTexBuffer( (GLenum)var0, (GLenum)var1, (GLuint)var2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexBuffer( %d, %d, 0x%04X )", (int)var0, (int)var1, (unsigned)var2);
  }
}

FABRIC_EXT_EXPORT void klFramebufferTexture(
  const KL::Size & var0,
  const KL::Size & var1,
  const KL::Size & var2,
  const KL::Integer & var3
){
  try
  {
    glFramebufferTexture( (GLenum)var0, (GLenum)var1, (GLuint)var2, (GLint)var3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferTexture( %d, %d, 0x%04X, %d )", (int)var0, (int)var1, (unsigned)var2, (int)var3);
  }
}

FABRIC_EXT_EXPORT void klTbufferMask3DFX(
  const KL::Size & mask
){
  try
  {
    glTbufferMask3DFX( (GLuint)mask );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTbufferMask3DFX( 0x%04X )", (unsigned)mask);
  }
}

FABRIC_EXT_EXPORT void klCopyBufferSubData(
  const KL::Size & readtarget,
  const KL::Size & writetarget,
  const KL::Integer & readoffset,
  const KL::Integer & writeoffset,
  const KL::Size & size
){
  try
  {
    glCopyBufferSubData( (GLenum)readtarget, (GLenum)writetarget, (GLintptr)readoffset, (GLintptr)writeoffset, (GLsizeiptr)size );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyBufferSubData( %d, %d, %d, %d, %d )", (int)readtarget, (int)writetarget, (int)readoffset, (int)writeoffset, (int)size);
  }
}

FABRIC_EXT_EXPORT void klDrawElementsBaseVertex(
  const KL::Size & mode,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices,
  const KL::Integer & basevertex
){
  try
  {
    glDrawElementsBaseVertex( (GLenum)mode, (GLsizei)count, (GLenum)type, indices, (GLint)basevertex );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawElementsBaseVertex( %d, %d, %d, void*, %d )", (int)mode, (int)count, (int)type, (int)basevertex);
  }
}

FABRIC_EXT_EXPORT void klDrawElementsInstancedBaseVertex(
  const KL::Size & mode,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices,
  const KL::Size & primcount,
  const KL::Integer & basevertex
){
  try
  {
    glDrawElementsInstancedBaseVertex( (GLenum)mode, (GLsizei)count, (GLenum)type, indices, (GLsizei)primcount, (GLint)basevertex );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawElementsInstancedBaseVertex( %d, %d, %d, void*, %d, %d )", (int)mode, (int)count, (int)type, (int)primcount, (int)basevertex);
  }
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
  try
  {
    glDrawRangeElementsBaseVertex( (GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, indices, (GLint)basevertex );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawRangeElementsBaseVertex( %d, 0x%04X, 0x%04X, %d, %d, void*, %d )", (int)mode, (unsigned)start, (unsigned)end, (int)count, (int)type, (int)basevertex);
  }
}

FABRIC_EXT_EXPORT void klBindFramebuffer(
  const KL::Size & target,
  const KL::Size & framebuffer
){
  try
  {
    glBindFramebuffer( (GLenum)target, (GLuint)framebuffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindFramebuffer( %d, 0x%04X )", (int)target, (unsigned)framebuffer);
  }
}

FABRIC_EXT_EXPORT void klBindRenderbuffer(
  const KL::Size & target,
  const KL::Size & renderbuffer
){
  try
  {
    glBindRenderbuffer( (GLenum)target, (GLuint)renderbuffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindRenderbuffer( %d, 0x%04X )", (int)target, (unsigned)renderbuffer);
  }
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
  const KL::Size & mask,
  const KL::Size & filter
){
  try
  {
    glBlitFramebuffer( (GLint)srcX0, (GLint)srcY0, (GLint)srcX1, (GLint)srcY1, (GLint)dstX0, (GLint)dstY0, (GLint)dstX1, (GLint)dstY1, (GLbitfield)mask, (GLenum)filter );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBlitFramebuffer( %d, %d, %d, %d, %d, %d, %d, %d, 0x%04X, %d )", (int)srcX0, (int)srcY0, (int)srcX1, (int)srcY1, (int)dstX0, (int)dstY0, (int)dstX1, (int)dstY1, (unsigned)mask, (int)filter);
  }
}

FABRIC_EXT_EXPORT KL::Size klCheckFramebufferStatus(
  const KL::Size & target
){
  try
  {
    GLenum result = glCheckFramebufferStatus( (GLenum)target );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCheckFramebufferStatus( %d )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klDeleteFramebuffers(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & framebuffers
){
  try
  {
    glDeleteFramebuffers( (GLsizei)n, (const GLuint*)&framebuffers[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteFramebuffers( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klDeleteRenderbuffers(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & renderbuffers
){
  try
  {
    glDeleteRenderbuffers( (GLsizei)n, (const GLuint*)&renderbuffers[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteRenderbuffers( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klFramebufferRenderbuffer(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & renderbuffertarget,
  const KL::Size & renderbuffer
){
  try
  {
    glFramebufferRenderbuffer( (GLenum)target, (GLenum)attachment, (GLenum)renderbuffertarget, (GLuint)renderbuffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferRenderbuffer( %d, %d, %d, 0x%04X )", (int)target, (int)attachment, (int)renderbuffertarget, (unsigned)renderbuffer);
  }
}

FABRIC_EXT_EXPORT void klFramebufferTexture1D(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level
){
  try
  {
    glFramebufferTexture1D( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferTexture1D( %d, %d, %d, 0x%04X, %d )", (int)target, (int)attachment, (int)textarget, (unsigned)texture, (int)level);
  }
}

FABRIC_EXT_EXPORT void klFramebufferTexture2D(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level
){
  try
  {
    glFramebufferTexture2D( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferTexture2D( %d, %d, %d, 0x%04X, %d )", (int)target, (int)attachment, (int)textarget, (unsigned)texture, (int)level);
  }
}

FABRIC_EXT_EXPORT void klFramebufferTexture3D(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Integer & layer
){
  try
  {
    glFramebufferTexture3D( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level, (GLint)layer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferTexture3D( %d, %d, %d, 0x%04X, %d, %d )", (int)target, (int)attachment, (int)textarget, (unsigned)texture, (int)level, (int)layer);
  }
}

FABRIC_EXT_EXPORT void klFramebufferTextureLayer(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Integer & layer
){
  try
  {
    glFramebufferTextureLayer( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferTextureLayer( %d, %d, 0x%04X, %d, %d )", (int)target, (int)attachment, (unsigned)texture, (int)level, (int)layer);
  }
}

FABRIC_EXT_EXPORT void klGenFramebuffers(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & framebuffers
){
  try
  {
    glGenFramebuffers( (GLsizei)n, (GLuint*)&framebuffers[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenFramebuffers( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klGenRenderbuffers(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & renderbuffers
){
  try
  {
    glGenRenderbuffers( (GLsizei)n, (GLuint*)&renderbuffers[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenRenderbuffers( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klGenerateMipmap(
  const KL::Size & target
){
  try
  {
    glGenerateMipmap( (GLenum)target );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenerateMipmap( %d )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klGetFramebufferAttachmentParameteriv(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetFramebufferAttachmentParameteriv( (GLenum)target, (GLenum)attachment, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetFramebufferAttachmentParameteriv( %d, %d, %d, GLint* )", (int)target, (int)attachment, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetRenderbufferParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetRenderbufferParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetRenderbufferParameteriv( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsFramebuffer(
  const KL::Size & framebuffer
){
  try
  {
    GLboolean result = glIsFramebuffer( (GLuint)framebuffer );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsFramebuffer( 0x%04X )", (unsigned)framebuffer);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsRenderbuffer(
  const KL::Size & renderbuffer
){
  try
  {
    GLboolean result = glIsRenderbuffer( (GLuint)renderbuffer );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsRenderbuffer( 0x%04X )", (unsigned)renderbuffer);
  }
}

FABRIC_EXT_EXPORT void klRenderbufferStorage(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  try
  {
    glRenderbufferStorage( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRenderbufferStorage( %d, %d, %d, %d )", (int)target, (int)internalformat, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klRenderbufferStorageMultisample(
  const KL::Size & target,
  const KL::Size & samples,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  try
  {
    glRenderbufferStorageMultisample( (GLenum)target, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRenderbufferStorageMultisample( %d, %d, %d, %d, %d )", (int)target, (int)samples, (int)internalformat, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klColorSubTable(
  const KL::Size & target,
  const KL::Size & start,
  const KL::Size & count,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data data
){
  try
  {
    glColorSubTable( (GLenum)target, (GLsizei)start, (GLsizei)count, (GLenum)format, (GLenum)type, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColorSubTable( %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)start, (int)count, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klColorTable(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data table
){
  try
  {
    glColorTable( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLenum)format, (GLenum)type, table );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColorTable( %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)internalformat, (int)width, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klColorTableParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glColorTableParameterfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColorTableParameterfv( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klColorTableParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glColorTableParameteriv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColorTableParameteriv( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klConvolutionFilter1D(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data image
){
  try
  {
    glConvolutionFilter1D( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLenum)format, (GLenum)type, image );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glConvolutionFilter1D( %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)internalformat, (int)width, (int)format, (int)type);
  }
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
  try
  {
    glConvolutionFilter2D( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, image );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glConvolutionFilter2D( %d, %d, %d, %d, %d, %d, GLvoid* )", (int)target, (int)internalformat, (int)width, (int)height, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klConvolutionParameterf(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & params
){
  try
  {
    glConvolutionParameterf( (GLenum)target, (GLenum)pname, (GLfloat)params );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glConvolutionParameterf( %d, %d, %f )", (int)target, (int)pname, (float)params);
  }
}

FABRIC_EXT_EXPORT void klConvolutionParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glConvolutionParameterfv( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glConvolutionParameterfv( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klConvolutionParameteri(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & params
){
  try
  {
    glConvolutionParameteri( (GLenum)target, (GLenum)pname, (GLint)params );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glConvolutionParameteri( %d, %d, %d )", (int)target, (int)pname, (int)params);
  }
}

FABRIC_EXT_EXPORT void klConvolutionParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glConvolutionParameteriv( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glConvolutionParameteriv( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klCopyColorSubTable(
  const KL::Size & target,
  const KL::Size & start,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  try
  {
    glCopyColorSubTable( (GLenum)target, (GLsizei)start, (GLint)x, (GLint)y, (GLsizei)width );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyColorSubTable( %d, %d, %d, %d, %d )", (int)target, (int)start, (int)x, (int)y, (int)width);
  }
}

FABRIC_EXT_EXPORT void klCopyColorTable(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  try
  {
    glCopyColorTable( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyColorTable( %d, %d, %d, %d, %d )", (int)target, (int)internalformat, (int)x, (int)y, (int)width);
  }
}

FABRIC_EXT_EXPORT void klCopyConvolutionFilter1D(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  try
  {
    glCopyConvolutionFilter1D( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyConvolutionFilter1D( %d, %d, %d, %d, %d )", (int)target, (int)internalformat, (int)x, (int)y, (int)width);
  }
}

FABRIC_EXT_EXPORT void klCopyConvolutionFilter2D(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  try
  {
    glCopyConvolutionFilter2D( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyConvolutionFilter2D( %d, %d, %d, %d, %d, %d )", (int)target, (int)internalformat, (int)x, (int)y, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klGetColorTable(
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data table
){
  try
  {
    glGetColorTable( (GLenum)target, (GLenum)format, (GLenum)type, table );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetColorTable( %d, %d, %d, GLvoid* )", (int)target, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klGetColorTableParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetColorTableParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetColorTableParameterfv( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetColorTableParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetColorTableParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetColorTableParameteriv( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetConvolutionFilter(
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data image
){
  try
  {
    glGetConvolutionFilter( (GLenum)target, (GLenum)format, (GLenum)type, image );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetConvolutionFilter( %d, %d, %d, GLvoid* )", (int)target, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klGetConvolutionParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetConvolutionParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetConvolutionParameterfv( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetConvolutionParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetConvolutionParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetConvolutionParameteriv( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetHistogram(
  const KL::Size & target,
  const KL::Boolean & reset,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data values
){
  try
  {
    glGetHistogram( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, values );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetHistogram( %d, %b, %d, %d, GLvoid* )", (int)target, (bool)reset, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klGetHistogramParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetHistogramParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetHistogramParameterfv( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetHistogramParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetHistogramParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetHistogramParameteriv( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMinmax(
  const KL::Size & target,
  const KL::Boolean & reset,
  const KL::Size & format,
  const KL::Size & types,
  KL::Data values
){
  try
  {
    glGetMinmax( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)types, values );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMinmax( %d, %b, %d, %d, GLvoid* )", (int)target, (bool)reset, (int)format, (int)types);
  }
}

FABRIC_EXT_EXPORT void klGetMinmaxParameterfv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetMinmaxParameterfv( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMinmaxParameterfv( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMinmaxParameteriv(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetMinmaxParameteriv( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMinmaxParameteriv( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetSeparableFilter(
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data row,
  KL::Data column,
  KL::Data span
){
  try
  {
    glGetSeparableFilter( (GLenum)target, (GLenum)format, (GLenum)type, row, column, span );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetSeparableFilter( %d, %d, %d, GLvoid*, GLvoid*, GLvoid* )", (int)target, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klHistogram(
  const KL::Size & target,
  const KL::Size & width,
  const KL::Size & internalformat,
  const KL::Boolean & sink
){
  try
  {
    glHistogram( (GLenum)target, (GLsizei)width, (GLenum)internalformat, (GLboolean)sink );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glHistogram( %d, %d, %d, %b )", (int)target, (int)width, (int)internalformat, (bool)sink);
  }
}

FABRIC_EXT_EXPORT void klMinmax(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Boolean & sink
){
  try
  {
    glMinmax( (GLenum)target, (GLenum)internalformat, (GLboolean)sink );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMinmax( %d, %d, %b )", (int)target, (int)internalformat, (bool)sink);
  }
}

FABRIC_EXT_EXPORT void klResetHistogram(
  const KL::Size & target
){
  try
  {
    glResetHistogram( (GLenum)target );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glResetHistogram( %d )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klResetMinmax(
  const KL::Size & target
){
  try
  {
    glResetMinmax( (GLenum)target );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glResetMinmax( %d )", (int)target);
  }
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
  try
  {
    glSeparableFilter2D( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, row, column );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSeparableFilter2D( %d, %d, %d, %d, %d, %d, GLvoid*, GLvoid* )", (int)target, (int)internalformat, (int)width, (int)height, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klFlushMappedBufferRange(
  const KL::Size & target,
  const KL::Integer & offset,
  const KL::Size & length
){
  try
  {
    glFlushMappedBufferRange( (GLenum)target, (GLintptr)offset, (GLsizeiptr)length );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFlushMappedBufferRange( %d, %d, %d )", (int)target, (int)offset, (int)length);
  }
}

FABRIC_EXT_EXPORT GLvoid klMapBufferRange(
  const KL::Size & target,
  const KL::Integer & offset,
  const KL::Size & length,
  const KL::Size & access
){
  try
  {
    glMapBufferRange( (GLenum)target, (GLintptr)offset, (GLsizeiptr)length, (GLbitfield)access );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMapBufferRange( %d, %d, %d, 0x%04X )", (int)target, (int)offset, (int)length, (unsigned)access);
  }
}

FABRIC_EXT_EXPORT void klProvokingVertex(
  const KL::Size & mode
){
  try
  {
    glProvokingVertex( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProvokingVertex( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klGetMultisamplefv(
  const KL::Size & pname,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & val
){
  try
  {
    glGetMultisamplefv( (GLenum)pname, (GLuint)index, (GLfloat*)&val[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultisamplefv( %d, 0x%04X, GLfloat* )", (int)pname, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klSampleMaski(
  const KL::Size & index,
  const KL::Size & mask
){
  try
  {
    glSampleMaski( (GLuint)index, (GLbitfield)mask );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSampleMaski( 0x%04X, 0x%04X )", (unsigned)index, (unsigned)mask);
  }
}

FABRIC_EXT_EXPORT void klTexImage2DMultisample(
  const KL::Size & target,
  const KL::Size & samples,
  const KL::Integer & internalformat,
  const KL::Size & width,
  const KL::Size & height,
  const KL::Boolean & fixedsamplelocations
){
  try
  {
    glTexImage2DMultisample( (GLenum)target, (GLsizei)samples, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLboolean)fixedsamplelocations );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexImage2DMultisample( %d, %d, %d, %d, %d, %b )", (int)target, (int)samples, (int)internalformat, (int)width, (int)height, (bool)fixedsamplelocations);
  }
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
  try
  {
    glTexImage3DMultisample( (GLenum)target, (GLsizei)samples, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLboolean)fixedsamplelocations );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexImage3DMultisample( %d, %d, %d, %d, %d, %d, %b )", (int)target, (int)samples, (int)internalformat, (int)width, (int)height, (int)depth, (bool)fixedsamplelocations);
  }
}

FABRIC_EXT_EXPORT void klBindBufferBase(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & buffer
){
  try
  {
    glBindBufferBase( (GLenum)target, (GLuint)index, (GLuint)buffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindBufferBase( %d, 0x%04X, 0x%04X )", (int)target, (unsigned)index, (unsigned)buffer);
  }
}

FABRIC_EXT_EXPORT void klBindBufferRange(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & buffer,
  const KL::Integer & offset,
  const KL::Size & size
){
  try
  {
    glBindBufferRange( (GLenum)target, (GLuint)index, (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindBufferRange( %d, 0x%04X, 0x%04X, %d, %d )", (int)target, (unsigned)index, (unsigned)buffer, (int)offset, (int)size);
  }
}

FABRIC_EXT_EXPORT void klGetActiveUniformBlockName(
  const KL::Size & program,
  const KL::Size & uniformBlockIndex,
  const KL::Size & bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::String & uniformBlockName
){
  try
  {
    char * uniformBlockNameStr = new char[1024];
    glGetActiveUniformBlockName( (GLuint)program, (GLuint)uniformBlockIndex, (GLsizei)bufSize, (GLsizei*)&length[0], (char*)uniformBlockNameStr );
    uniformBlockName = KL::String(uniformBlockNameStr);
    delete( uniformBlockNameStr );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetActiveUniformBlockName( 0x%04X, 0x%04X, %d, GLsizei*, char* )", (unsigned)program, (unsigned)uniformBlockIndex, (int)bufSize);
  }
}

FABRIC_EXT_EXPORT void klGetActiveUniformBlockiv(
  const KL::Size & program,
  const KL::Size & uniformBlockIndex,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetActiveUniformBlockiv( (GLuint)program, (GLuint)uniformBlockIndex, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetActiveUniformBlockiv( 0x%04X, 0x%04X, %d, GLint* )", (unsigned)program, (unsigned)uniformBlockIndex, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetActiveUniformName(
  const KL::Size & program,
  const KL::Size & uniformIndex,
  const KL::Size & bufSize,
  KL::VariableArray<KL::Size> & length,
  KL::String & uniformName
){
  try
  {
    char * uniformNameStr = new char[1024];
    glGetActiveUniformName( (GLuint)program, (GLuint)uniformIndex, (GLsizei)bufSize, (GLsizei*)&length[0], (char*)uniformNameStr );
    uniformName = KL::String(uniformNameStr);
    delete( uniformNameStr );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetActiveUniformName( 0x%04X, 0x%04X, %d, GLsizei*, char* )", (unsigned)program, (unsigned)uniformIndex, (int)bufSize);
  }
}

FABRIC_EXT_EXPORT void klGetActiveUniformsiv(
  const KL::Size & program,
  const KL::Size & uniformCount,
  const KL::VariableArray<KL::Size> & uniformIndices,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetActiveUniformsiv( (GLuint)program, (GLsizei)uniformCount, (const GLuint*)&uniformIndices[0], (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetActiveUniformsiv( 0x%04X, %d, GLuint*, %d, GLint* )", (unsigned)program, (int)uniformCount, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetIntegeri_v(
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Integer> & data
){
  try
  {
    glGetIntegeri_v( (GLenum)target, (GLuint)index, (GLint*)&data[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetIntegeri_v( %d, 0x%04X, GLint* )", (int)target, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT KL::Size klGetUniformBlockIndex(
  const KL::Size & program,
  const KL::String &uniformBlockName
){
  try
  {
    GLuint result = glGetUniformBlockIndex( (GLuint)program, (const char*)uniformBlockName.data() );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetUniformBlockIndex( 0x%04X, char* )", (unsigned)program);
  }
}

FABRIC_EXT_EXPORT void klGetUniformIndices(
  const KL::Size & program,
  const KL::Size & uniformCount,
  const KL::VariableArray<KL::String> &uniformNames,
  KL::VariableArray<KL::Size> & uniformIndices
){
  try
  {
    glGetUniformIndices( (GLuint)program, (GLsizei)uniformCount, (const char**)&uniformNames[0], (GLuint*)&uniformIndices[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetUniformIndices( 0x%04X, %d, char*, GLuint* )", (unsigned)program, (int)uniformCount);
  }
}

FABRIC_EXT_EXPORT void klUniformBlockBinding(
  const KL::Size & program,
  const KL::Size & uniformBlockIndex,
  const KL::Size & uniformBlockBinding
){
  try
  {
    glUniformBlockBinding( (GLuint)program, (GLuint)uniformBlockIndex, (GLuint)uniformBlockBinding );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniformBlockBinding( 0x%04X, 0x%04X, 0x%04X )", (unsigned)program, (unsigned)uniformBlockIndex, (unsigned)uniformBlockBinding);
  }
}

FABRIC_EXT_EXPORT void klBindVertexArray(
  const KL::Size & array
){
  try
  {
    glBindVertexArray( (GLuint)array );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindVertexArray( 0x%04X )", (unsigned)array);
  }
}

FABRIC_EXT_EXPORT void klDeleteVertexArrays(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & arrays
){
  try
  {
    glDeleteVertexArrays( (GLsizei)n, (const GLuint*)&arrays[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteVertexArrays( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klGenVertexArrays(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & arrays
){
  try
  {
    glGenVertexArrays( (GLsizei)n, (GLuint*)&arrays[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenVertexArrays( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsVertexArray(
  const KL::Size & array
){
  try
  {
    GLboolean result = glIsVertexArray( (GLuint)array );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsVertexArray( 0x%04X )", (unsigned)array);
  }
}

FABRIC_EXT_EXPORT KL::Integer klGetUniformBufferSizeEXT(
  const KL::Size & program,
  const KL::Integer & location
){
  try
  {
    GLint result = glGetUniformBufferSizeEXT( (GLuint)program, (GLint)location );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetUniformBufferSizeEXT( 0x%04X, %d )", (unsigned)program, (int)location);
  }
}

FABRIC_EXT_EXPORT KL::Integer klGetUniformOffsetEXT(
  const KL::Size & program,
  const KL::Integer & location
){
  try
  {
    GLintptr result = glGetUniformOffsetEXT( (GLuint)program, (GLint)location );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetUniformOffsetEXT( 0x%04X, %d )", (unsigned)program, (int)location);
  }
}

FABRIC_EXT_EXPORT void klUniformBufferEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & buffer
){
  try
  {
    glUniformBufferEXT( (GLuint)program, (GLint)location, (GLuint)buffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniformBufferEXT( 0x%04X, %d, 0x%04X )", (unsigned)program, (int)location, (unsigned)buffer);
  }
}

FABRIC_EXT_EXPORT void klBlendColorEXT(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue,
  const KL::Scalar & alpha
){
  try
  {
    glBlendColorEXT( (GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBlendColorEXT( %f, %f, %f, %f )", (float)red, (float)green, (float)blue, (float)alpha);
  }
}

FABRIC_EXT_EXPORT void klBlendEquationSeparateEXT(
  const KL::Size & modeRGB,
  const KL::Size & modeAlpha
){
  try
  {
    glBlendEquationSeparateEXT( (GLenum)modeRGB, (GLenum)modeAlpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBlendEquationSeparateEXT( %d, %d )", (int)modeRGB, (int)modeAlpha);
  }
}

FABRIC_EXT_EXPORT void klBlendFuncSeparateEXT(
  const KL::Size & sfactorRGB,
  const KL::Size & dfactorRGB,
  const KL::Size & sfactorAlpha,
  const KL::Size & dfactorAlpha
){
  try
  {
    glBlendFuncSeparateEXT( (GLenum)sfactorRGB, (GLenum)dfactorRGB, (GLenum)sfactorAlpha, (GLenum)dfactorAlpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBlendFuncSeparateEXT( %d, %d, %d, %d )", (int)sfactorRGB, (int)dfactorRGB, (int)sfactorAlpha, (int)dfactorAlpha);
  }
}

FABRIC_EXT_EXPORT void klBlendEquationEXT(
  const KL::Size & mode
){
  try
  {
    glBlendEquationEXT( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBlendEquationEXT( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klColorSubTableEXT(
  const KL::Size & target,
  const KL::Size & start,
  const KL::Size & count,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data data
){
  try
  {
    glColorSubTableEXT( (GLenum)target, (GLsizei)start, (GLsizei)count, (GLenum)format, (GLenum)type, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColorSubTableEXT( %d, %d, %d, %d, %d, void* )", (int)target, (int)start, (int)count, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klCopyColorSubTableEXT(
  const KL::Size & target,
  const KL::Size & start,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  try
  {
    glCopyColorSubTableEXT( (GLenum)target, (GLsizei)start, (GLint)x, (GLint)y, (GLsizei)width );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyColorSubTableEXT( %d, %d, %d, %d, %d )", (int)target, (int)start, (int)x, (int)y, (int)width);
  }
}

FABRIC_EXT_EXPORT void klLockArraysEXT(
  const KL::Integer & first,
  const KL::Size & count
){
  try
  {
    glLockArraysEXT( (GLint)first, (GLsizei)count );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLockArraysEXT( %d, %d )", (int)first, (int)count);
  }
}

FABRIC_EXT_EXPORT void klUnlockArraysEXT()
{
  try
  {
    glUnlockArraysEXT();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUnlockArraysEXT(  )");
  }
}

FABRIC_EXT_EXPORT void klConvolutionFilter1DEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data image
){
  try
  {
    glConvolutionFilter1DEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLenum)format, (GLenum)type, image );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glConvolutionFilter1DEXT( %d, %d, %d, %d, %d, void* )", (int)target, (int)internalformat, (int)width, (int)format, (int)type);
  }
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
  try
  {
    glConvolutionFilter2DEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, image );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glConvolutionFilter2DEXT( %d, %d, %d, %d, %d, %d, void* )", (int)target, (int)internalformat, (int)width, (int)height, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klConvolutionParameterfEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glConvolutionParameterfEXT( (GLenum)target, (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glConvolutionParameterfEXT( %d, %d, %f )", (int)target, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klConvolutionParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glConvolutionParameterfvEXT( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glConvolutionParameterfvEXT( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klConvolutionParameteriEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glConvolutionParameteriEXT( (GLenum)target, (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glConvolutionParameteriEXT( %d, %d, %d )", (int)target, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klConvolutionParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glConvolutionParameterivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glConvolutionParameterivEXT( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klCopyConvolutionFilter1DEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  try
  {
    glCopyConvolutionFilter1DEXT( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyConvolutionFilter1DEXT( %d, %d, %d, %d, %d )", (int)target, (int)internalformat, (int)x, (int)y, (int)width);
  }
}

FABRIC_EXT_EXPORT void klCopyConvolutionFilter2DEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  try
  {
    glCopyConvolutionFilter2DEXT( (GLenum)target, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyConvolutionFilter2DEXT( %d, %d, %d, %d, %d, %d )", (int)target, (int)internalformat, (int)x, (int)y, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klGetConvolutionFilterEXT(
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data image
){
  try
  {
    glGetConvolutionFilterEXT( (GLenum)target, (GLenum)format, (GLenum)type, image );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetConvolutionFilterEXT( %d, %d, %d, void* )", (int)target, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klGetConvolutionParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetConvolutionParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetConvolutionParameterfvEXT( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetConvolutionParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetConvolutionParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetConvolutionParameterivEXT( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetSeparableFilterEXT(
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data row,
  KL::Data column,
  KL::Data span
){
  try
  {
    glGetSeparableFilterEXT( (GLenum)target, (GLenum)format, (GLenum)type, row, column, span );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetSeparableFilterEXT( %d, %d, %d, void*, void*, void* )", (int)target, (int)format, (int)type);
  }
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
  try
  {
    glSeparableFilter2DEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, row, column );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSeparableFilter2DEXT( %d, %d, %d, %d, %d, %d, void*, void* )", (int)target, (int)internalformat, (int)width, (int)height, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klBinormalPointerEXT(
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glBinormalPointerEXT( (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBinormalPointerEXT( %d, %d, void* )", (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klTangentPointerEXT(
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glTangentPointerEXT( (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTangentPointerEXT( %d, %d, void* )", (int)type, (int)stride);
  }
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
  try
  {
    glCopyTexImage1DEXT( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTexImage1DEXT( %d, %d, %d, %d, %d, %d, %d )", (int)target, (int)level, (int)internalformat, (int)x, (int)y, (int)width, (int)border);
  }
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
  try
  {
    glCopyTexImage2DEXT( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTexImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d )", (int)target, (int)level, (int)internalformat, (int)x, (int)y, (int)width, (int)height, (int)border);
  }
}

FABRIC_EXT_EXPORT void klCopyTexSubImage1DEXT(
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Integer & xoffset,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width
){
  try
  {
    glCopyTexSubImage1DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTexSubImage1DEXT( %d, %d, %d, %d, %d, %d )", (int)target, (int)level, (int)xoffset, (int)x, (int)y, (int)width);
  }
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
  try
  {
    glCopyTexSubImage2DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTexSubImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d )", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)x, (int)y, (int)width, (int)height);
  }
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
  try
  {
    glCopyTexSubImage3DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTexSubImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d )", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)x, (int)y, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klCullParameterdvEXT(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glCullParameterdvEXT( (GLenum)pname, (GLdouble*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCullParameterdvEXT( %d, GLdouble* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klCullParameterfvEXT(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glCullParameterfvEXT( (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCullParameterfvEXT( %d, GLfloat* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klDepthBoundsEXT(
  const KL::Scalar & zmin,
  const KL::Scalar & zmax
){
  try
  {
    glDepthBoundsEXT( (GLclampd)zmin, (GLclampd)zmax );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDepthBoundsEXT( %f, %f )", (float)zmin, (float)zmax);
  }
}

FABRIC_EXT_EXPORT void klBindMultiTextureEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & texture
){
  try
  {
    glBindMultiTextureEXT( (GLenum)texunit, (GLenum)target, (GLuint)texture );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindMultiTextureEXT( %d, %d, 0x%04X )", (int)texunit, (int)target, (unsigned)texture);
  }
}

FABRIC_EXT_EXPORT KL::Size klCheckNamedFramebufferStatusEXT(
  const KL::Size & framebuffer,
  const KL::Size & target
){
  try
  {
    GLenum result = glCheckNamedFramebufferStatusEXT( (GLuint)framebuffer, (GLenum)target );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCheckNamedFramebufferStatusEXT( 0x%04X, %d )", (unsigned)framebuffer, (int)target);
  }
}

FABRIC_EXT_EXPORT void klClientAttribDefaultEXT(
  const KL::Size & mask
){
  try
  {
    glClientAttribDefaultEXT( (GLbitfield)mask );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClientAttribDefaultEXT( 0x%04X )", (unsigned)mask);
  }
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
  try
  {
    glCompressedMultiTexImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLint)border, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedMultiTexImage1DEXT( %d, %d, %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)imageSize);
  }
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
  try
  {
    glCompressedMultiTexImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedMultiTexImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)imageSize);
  }
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
  try
  {
    glCompressedMultiTexImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedMultiTexImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)imageSize);
  }
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
  try
  {
    glCompressedMultiTexSubImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedMultiTexSubImage1DEXT( %d, %d, %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)imageSize);
  }
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
  try
  {
    glCompressedMultiTexSubImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedMultiTexSubImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)imageSize);
  }
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
  try
  {
    glCompressedMultiTexSubImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedMultiTexSubImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)imageSize);
  }
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
  try
  {
    glCompressedTextureImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLint)border, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedTextureImage1DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)imageSize);
  }
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
  try
  {
    glCompressedTextureImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedTextureImage2DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)imageSize);
  }
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
  try
  {
    glCompressedTextureImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedTextureImage3DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)imageSize);
  }
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
  try
  {
    glCompressedTextureSubImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedTextureSubImage1DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)imageSize);
  }
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
  try
  {
    glCompressedTextureSubImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedTextureSubImage2DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)imageSize);
  }
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
  try
  {
    glCompressedTextureSubImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLsizei)imageSize, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCompressedTextureSubImage3DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)imageSize);
  }
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
  try
  {
    glCopyMultiTexImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyMultiTexImage1DEXT( %d, %d, %d, %d, %d, %d, %d, %d )", (int)texunit, (int)target, (int)level, (int)internalformat, (int)x, (int)y, (int)width, (int)border);
  }
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
  try
  {
    glCopyMultiTexImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyMultiTexImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d )", (int)texunit, (int)target, (int)level, (int)internalformat, (int)x, (int)y, (int)width, (int)height, (int)border);
  }
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
  try
  {
    glCopyMultiTexSubImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyMultiTexSubImage1DEXT( %d, %d, %d, %d, %d, %d, %d )", (int)texunit, (int)target, (int)level, (int)xoffset, (int)x, (int)y, (int)width);
  }
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
  try
  {
    glCopyMultiTexSubImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyMultiTexSubImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d )", (int)texunit, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)x, (int)y, (int)width, (int)height);
  }
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
  try
  {
    glCopyMultiTexSubImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyMultiTexSubImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d )", (int)texunit, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)x, (int)y, (int)width, (int)height);
  }
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
  try
  {
    glCopyTextureImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTextureImage1DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d )", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)x, (int)y, (int)width, (int)border);
  }
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
  try
  {
    glCopyTextureImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTextureImage2DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d )", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)x, (int)y, (int)width, (int)height, (int)border);
  }
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
  try
  {
    glCopyTextureSubImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTextureSubImage1DEXT( 0x%04X, %d, %d, %d, %d, %d, %d )", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)x, (int)y, (int)width);
  }
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
  try
  {
    glCopyTextureSubImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTextureSubImage2DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d )", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)x, (int)y, (int)width, (int)height);
  }
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
  try
  {
    glCopyTextureSubImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCopyTextureSubImage3DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, %d )", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)x, (int)y, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klDisableClientStateIndexedEXT(
  const KL::Size & array,
  const KL::Size & index
){
  try
  {
    glDisableClientStateIndexedEXT( (GLenum)array, (GLuint)index );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDisableClientStateIndexedEXT( %d, 0x%04X )", (int)array, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klDisableClientStateiEXT(
  const KL::Size & array,
  const KL::Size & index
){
  try
  {
    glDisableClientStateiEXT( (GLenum)array, (GLuint)index );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDisableClientStateiEXT( %d, 0x%04X )", (int)array, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klDisableVertexArrayAttribEXT(
  const KL::Size & vaobj,
  const KL::Size & index
){
  try
  {
    glDisableVertexArrayAttribEXT( (GLuint)vaobj, (GLuint)index );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDisableVertexArrayAttribEXT( 0x%04X, 0x%04X )", (unsigned)vaobj, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klDisableVertexArrayEXT(
  const KL::Size & vaobj,
  const KL::Size & array
){
  try
  {
    glDisableVertexArrayEXT( (GLuint)vaobj, (GLenum)array );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDisableVertexArrayEXT( 0x%04X, %d )", (unsigned)vaobj, (int)array);
  }
}

FABRIC_EXT_EXPORT void klEnableClientStateIndexedEXT(
  const KL::Size & array,
  const KL::Size & index
){
  try
  {
    glEnableClientStateIndexedEXT( (GLenum)array, (GLuint)index );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEnableClientStateIndexedEXT( %d, 0x%04X )", (int)array, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klEnableClientStateiEXT(
  const KL::Size & array,
  const KL::Size & index
){
  try
  {
    glEnableClientStateiEXT( (GLenum)array, (GLuint)index );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEnableClientStateiEXT( %d, 0x%04X )", (int)array, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klEnableVertexArrayAttribEXT(
  const KL::Size & vaobj,
  const KL::Size & index
){
  try
  {
    glEnableVertexArrayAttribEXT( (GLuint)vaobj, (GLuint)index );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEnableVertexArrayAttribEXT( 0x%04X, 0x%04X )", (unsigned)vaobj, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klEnableVertexArrayEXT(
  const KL::Size & vaobj,
  const KL::Size & array
){
  try
  {
    glEnableVertexArrayEXT( (GLuint)vaobj, (GLenum)array );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEnableVertexArrayEXT( 0x%04X, %d )", (unsigned)vaobj, (int)array);
  }
}

FABRIC_EXT_EXPORT void klFlushMappedNamedBufferRangeEXT(
  const KL::Size & buffer,
  const KL::Integer & offset,
  const KL::Size & length
){
  try
  {
    glFlushMappedNamedBufferRangeEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)length );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFlushMappedNamedBufferRangeEXT( 0x%04X, %d, %d )", (unsigned)buffer, (int)offset, (int)length);
  }
}

FABRIC_EXT_EXPORT void klFramebufferDrawBufferEXT(
  const KL::Size & framebuffer,
  const KL::Size & mode
){
  try
  {
    glFramebufferDrawBufferEXT( (GLuint)framebuffer, (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferDrawBufferEXT( 0x%04X, %d )", (unsigned)framebuffer, (int)mode);
  }
}

FABRIC_EXT_EXPORT void klFramebufferDrawBuffersEXT(
  const KL::Size & framebuffer,
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & bufs
){
  try
  {
    glFramebufferDrawBuffersEXT( (GLuint)framebuffer, (GLsizei)n, (const GLenum*)&bufs[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferDrawBuffersEXT( 0x%04X, %d, GLenum* )", (unsigned)framebuffer, (int)n);
  }
}

FABRIC_EXT_EXPORT void klFramebufferReadBufferEXT(
  const KL::Size & framebuffer,
  const KL::Size & mode
){
  try
  {
    glFramebufferReadBufferEXT( (GLuint)framebuffer, (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferReadBufferEXT( 0x%04X, %d )", (unsigned)framebuffer, (int)mode);
  }
}

FABRIC_EXT_EXPORT void klGenerateMultiTexMipmapEXT(
  const KL::Size & texunit,
  const KL::Size & target
){
  try
  {
    glGenerateMultiTexMipmapEXT( (GLenum)texunit, (GLenum)target );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenerateMultiTexMipmapEXT( %d, %d )", (int)texunit, (int)target);
  }
}

FABRIC_EXT_EXPORT void klGenerateTextureMipmapEXT(
  const KL::Size & texture,
  const KL::Size & target
){
  try
  {
    glGenerateTextureMipmapEXT( (GLuint)texture, (GLenum)target );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenerateTextureMipmapEXT( 0x%04X, %d )", (unsigned)texture, (int)target);
  }
}

FABRIC_EXT_EXPORT void klGetCompressedMultiTexImageEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  KL::Data img
){
  try
  {
    glGetCompressedMultiTexImageEXT( (GLenum)texunit, (GLenum)target, (GLint)level, img );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetCompressedMultiTexImageEXT( %d, %d, %d, void* )", (int)texunit, (int)target, (int)level);
  }
}

FABRIC_EXT_EXPORT void klGetCompressedTextureImageEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  KL::Data img
){
  try
  {
    glGetCompressedTextureImageEXT( (GLuint)texture, (GLenum)target, (GLint)level, img );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetCompressedTextureImageEXT( 0x%04X, %d, %d, void* )", (unsigned)texture, (int)target, (int)level);
  }
}

FABRIC_EXT_EXPORT void klGetDoubleIndexedvEXT(
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetDoubleIndexedvEXT( (GLenum)target, (GLuint)index, (GLdouble*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetDoubleIndexedvEXT( %d, 0x%04X, GLdouble* )", (int)target, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klGetDoublei_vEXT(
  const KL::Size & pname,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetDoublei_vEXT( (GLenum)pname, (GLuint)index, (GLdouble*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetDoublei_vEXT( %d, 0x%04X, GLdouble* )", (int)pname, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klGetFloatIndexedvEXT(
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetFloatIndexedvEXT( (GLenum)target, (GLuint)index, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetFloatIndexedvEXT( %d, 0x%04X, GLfloat* )", (int)target, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klGetFloati_vEXT(
  const KL::Size & pname,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetFloati_vEXT( (GLenum)pname, (GLuint)index, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetFloati_vEXT( %d, 0x%04X, GLfloat* )", (int)pname, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klGetFramebufferParameterivEXT(
  const KL::Size & framebuffer,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & param
){
  try
  {
    glGetFramebufferParameterivEXT( (GLuint)framebuffer, (GLenum)pname, (GLint*)&param[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetFramebufferParameterivEXT( 0x%04X, %d, GLint* )", (unsigned)framebuffer, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMultiTexEnvfvEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetMultiTexEnvfvEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultiTexEnvfvEXT( %d, %d, %d, GLfloat* )", (int)texunit, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMultiTexEnvivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetMultiTexEnvivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultiTexEnvivEXT( %d, %d, %d, GLint* )", (int)texunit, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMultiTexGendvEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetMultiTexGendvEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLdouble*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultiTexGendvEXT( %d, %d, %d, GLdouble* )", (int)texunit, (int)coord, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMultiTexGenfvEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetMultiTexGenfvEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultiTexGenfvEXT( %d, %d, %d, GLfloat* )", (int)texunit, (int)coord, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMultiTexGenivEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetMultiTexGenivEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultiTexGenivEXT( %d, %d, %d, GLint* )", (int)texunit, (int)coord, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMultiTexImageEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  try
  {
    glGetMultiTexImageEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultiTexImageEXT( %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klGetMultiTexLevelParameterfvEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetMultiTexLevelParameterfvEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultiTexLevelParameterfvEXT( %d, %d, %d, %d, GLfloat* )", (int)texunit, (int)target, (int)level, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMultiTexLevelParameterivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetMultiTexLevelParameterivEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultiTexLevelParameterivEXT( %d, %d, %d, %d, GLint* )", (int)texunit, (int)target, (int)level, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMultiTexParameterIivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetMultiTexParameterIivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultiTexParameterIivEXT( %d, %d, %d, GLint* )", (int)texunit, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMultiTexParameterIuivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Size> & params
){
  try
  {
    glGetMultiTexParameterIuivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLuint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultiTexParameterIuivEXT( %d, %d, %d, GLuint* )", (int)texunit, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMultiTexParameterfvEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetMultiTexParameterfvEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultiTexParameterfvEXT( %d, %d, %d, GLfloat* )", (int)texunit, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMultiTexParameterivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetMultiTexParameterivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMultiTexParameterivEXT( %d, %d, %d, GLint* )", (int)texunit, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetNamedBufferParameterivEXT(
  const KL::Size & buffer,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetNamedBufferParameterivEXT( (GLuint)buffer, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetNamedBufferParameterivEXT( 0x%04X, %d, GLint* )", (unsigned)buffer, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetNamedBufferSubDataEXT(
  const KL::Size & buffer,
  const KL::Integer & offset,
  const KL::Size & size,
  KL::Data data
){
  try
  {
    glGetNamedBufferSubDataEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetNamedBufferSubDataEXT( 0x%04X, %d, %d, void* )", (unsigned)buffer, (int)offset, (int)size);
  }
}

FABRIC_EXT_EXPORT void klGetNamedFramebufferAttachmentParameterivEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetNamedFramebufferAttachmentParameterivEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetNamedFramebufferAttachmentParameterivEXT( 0x%04X, %d, %d, GLint* )", (unsigned)framebuffer, (int)attachment, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetNamedProgramLocalParameterIivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetNamedProgramLocalParameterIivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetNamedProgramLocalParameterIivEXT( 0x%04X, %d, 0x%04X, GLint* )", (unsigned)program, (int)target, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klGetNamedProgramLocalParameterIuivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Size> & params
){
  try
  {
    glGetNamedProgramLocalParameterIuivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLuint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetNamedProgramLocalParameterIuivEXT( 0x%04X, %d, 0x%04X, GLuint* )", (unsigned)program, (int)target, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klGetNamedProgramLocalParameterdvEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetNamedProgramLocalParameterdvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLdouble*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetNamedProgramLocalParameterdvEXT( 0x%04X, %d, 0x%04X, GLdouble* )", (unsigned)program, (int)target, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klGetNamedProgramLocalParameterfvEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetNamedProgramLocalParameterfvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetNamedProgramLocalParameterfvEXT( 0x%04X, %d, 0x%04X, GLfloat* )", (unsigned)program, (int)target, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klGetNamedProgramStringEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & pname,
  KL::Data string
){
  try
  {
    glGetNamedProgramStringEXT( (GLuint)program, (GLenum)target, (GLenum)pname, string );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetNamedProgramStringEXT( 0x%04X, %d, %d, void* )", (unsigned)program, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetNamedProgramivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetNamedProgramivEXT( (GLuint)program, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetNamedProgramivEXT( 0x%04X, %d, %d, GLint* )", (unsigned)program, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetNamedRenderbufferParameterivEXT(
  const KL::Size & renderbuffer,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetNamedRenderbufferParameterivEXT( (GLuint)renderbuffer, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetNamedRenderbufferParameterivEXT( 0x%04X, %d, GLint* )", (unsigned)renderbuffer, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTextureImageEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data pixels
){
  try
  {
    glGetTextureImageEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTextureImageEXT( 0x%04X, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klGetTextureLevelParameterfvEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetTextureLevelParameterfvEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTextureLevelParameterfvEXT( 0x%04X, %d, %d, %d, GLfloat* )", (unsigned)texture, (int)target, (int)level, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTextureLevelParameterivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Integer & level,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetTextureLevelParameterivEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTextureLevelParameterivEXT( 0x%04X, %d, %d, %d, GLint* )", (unsigned)texture, (int)target, (int)level, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTextureParameterIivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetTextureParameterIivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTextureParameterIivEXT( 0x%04X, %d, %d, GLint* )", (unsigned)texture, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTextureParameterIuivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Size> & params
){
  try
  {
    glGetTextureParameterIuivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLuint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTextureParameterIuivEXT( 0x%04X, %d, %d, GLuint* )", (unsigned)texture, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTextureParameterfvEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetTextureParameterfvEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTextureParameterfvEXT( 0x%04X, %d, %d, GLfloat* )", (unsigned)texture, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTextureParameterivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetTextureParameterivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTextureParameterivEXT( 0x%04X, %d, %d, GLint* )", (unsigned)texture, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetVertexArrayIntegeri_vEXT(
  const KL::Size & vaobj,
  const KL::Size & index,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & param
){
  try
  {
    glGetVertexArrayIntegeri_vEXT( (GLuint)vaobj, (GLuint)index, (GLenum)pname, (GLint*)&param[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVertexArrayIntegeri_vEXT( 0x%04X, 0x%04X, %d, GLint* )", (unsigned)vaobj, (unsigned)index, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetVertexArrayIntegervEXT(
  const KL::Size & vaobj,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & param
){
  try
  {
    glGetVertexArrayIntegervEXT( (GLuint)vaobj, (GLenum)pname, (GLint*)&param[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVertexArrayIntegervEXT( 0x%04X, %d, GLint* )", (unsigned)vaobj, (int)pname);
  }
}

FABRIC_EXT_EXPORT GLvoid klMapNamedBufferEXT(
  const KL::Size & buffer,
  const KL::Size & access
){
  try
  {
    glMapNamedBufferEXT( (GLuint)buffer, (GLenum)access );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMapNamedBufferEXT( 0x%04X, %d )", (unsigned)buffer, (int)access);
  }
}

FABRIC_EXT_EXPORT GLvoid klMapNamedBufferRangeEXT(
  const KL::Size & buffer,
  const KL::Integer & offset,
  const KL::Size & length,
  const KL::Size & access
){
  try
  {
    glMapNamedBufferRangeEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)length, (GLbitfield)access );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMapNamedBufferRangeEXT( 0x%04X, %d, %d, 0x%04X )", (unsigned)buffer, (int)offset, (int)length, (unsigned)access);
  }
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
  try
  {
    glMatrixFrustumEXT( (GLenum)matrixMode, (GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t, (GLdouble)n, (GLdouble)f );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixFrustumEXT( %d, %f, %f, %f, %f, %f, %f )", (int)matrixMode, (float)l, (float)r, (float)b, (float)t, (float)n, (float)f);
  }
}

FABRIC_EXT_EXPORT void klMatrixLoadIdentityEXT(
  const KL::Size & matrixMode
){
  try
  {
    glMatrixLoadIdentityEXT( (GLenum)matrixMode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixLoadIdentityEXT( %d )", (int)matrixMode);
  }
}

FABRIC_EXT_EXPORT void klMatrixLoadTransposedEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  try
  {
    glMatrixLoadTransposedEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixLoadTransposedEXT( %d, GLdouble* )", (int)matrixMode);
  }
}

FABRIC_EXT_EXPORT void klMatrixLoadTransposefEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  try
  {
    glMatrixLoadTransposefEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixLoadTransposefEXT( %d, GLfloat* )", (int)matrixMode);
  }
}

FABRIC_EXT_EXPORT void klMatrixLoaddEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  try
  {
    glMatrixLoaddEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixLoaddEXT( %d, GLdouble* )", (int)matrixMode);
  }
}

FABRIC_EXT_EXPORT void klMatrixLoadfEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  try
  {
    glMatrixLoadfEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixLoadfEXT( %d, GLfloat* )", (int)matrixMode);
  }
}

FABRIC_EXT_EXPORT void klMatrixMultTransposedEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  try
  {
    glMatrixMultTransposedEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixMultTransposedEXT( %d, GLdouble* )", (int)matrixMode);
  }
}

FABRIC_EXT_EXPORT void klMatrixMultTransposefEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  try
  {
    glMatrixMultTransposefEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixMultTransposefEXT( %d, GLfloat* )", (int)matrixMode);
  }
}

FABRIC_EXT_EXPORT void klMatrixMultdEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  try
  {
    glMatrixMultdEXT( (GLenum)matrixMode, (const GLdouble*)&m[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixMultdEXT( %d, GLdouble* )", (int)matrixMode);
  }
}

FABRIC_EXT_EXPORT void klMatrixMultfEXT(
  const KL::Size & matrixMode,
  const KL::VariableArray<KL::Scalar> & m
){
  try
  {
    glMatrixMultfEXT( (GLenum)matrixMode, (const GLfloat*)&m[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixMultfEXT( %d, GLfloat* )", (int)matrixMode);
  }
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
  try
  {
    glMatrixOrthoEXT( (GLenum)matrixMode, (GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t, (GLdouble)n, (GLdouble)f );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixOrthoEXT( %d, %f, %f, %f, %f, %f, %f )", (int)matrixMode, (float)l, (float)r, (float)b, (float)t, (float)n, (float)f);
  }
}

FABRIC_EXT_EXPORT void klMatrixPopEXT(
  const KL::Size & matrixMode
){
  try
  {
    glMatrixPopEXT( (GLenum)matrixMode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixPopEXT( %d )", (int)matrixMode);
  }
}

FABRIC_EXT_EXPORT void klMatrixPushEXT(
  const KL::Size & matrixMode
){
  try
  {
    glMatrixPushEXT( (GLenum)matrixMode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixPushEXT( %d )", (int)matrixMode);
  }
}

FABRIC_EXT_EXPORT void klMatrixRotatedEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & angle,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glMatrixRotatedEXT( (GLenum)matrixMode, (GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixRotatedEXT( %d, %f, %f, %f, %f )", (int)matrixMode, (float)angle, (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klMatrixRotatefEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & angle,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glMatrixRotatefEXT( (GLenum)matrixMode, (GLfloat)angle, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixRotatefEXT( %d, %f, %f, %f, %f )", (int)matrixMode, (float)angle, (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klMatrixScaledEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glMatrixScaledEXT( (GLenum)matrixMode, (GLdouble)x, (GLdouble)y, (GLdouble)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixScaledEXT( %d, %f, %f, %f )", (int)matrixMode, (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klMatrixScalefEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glMatrixScalefEXT( (GLenum)matrixMode, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixScalefEXT( %d, %f, %f, %f )", (int)matrixMode, (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klMatrixTranslatedEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glMatrixTranslatedEXT( (GLenum)matrixMode, (GLdouble)x, (GLdouble)y, (GLdouble)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixTranslatedEXT( %d, %f, %f, %f )", (int)matrixMode, (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klMatrixTranslatefEXT(
  const KL::Size & matrixMode,
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & z
){
  try
  {
    glMatrixTranslatefEXT( (GLenum)matrixMode, (GLfloat)x, (GLfloat)y, (GLfloat)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMatrixTranslatefEXT( %d, %f, %f, %f )", (int)matrixMode, (float)x, (float)y, (float)z);
  }
}

FABRIC_EXT_EXPORT void klMultiTexBufferEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & buffer
){
  try
  {
    glMultiTexBufferEXT( (GLenum)texunit, (GLenum)target, (GLenum)internalformat, (GLuint)buffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexBufferEXT( %d, %d, %d, 0x%04X )", (int)texunit, (int)target, (int)internalformat, (unsigned)buffer);
  }
}

FABRIC_EXT_EXPORT void klMultiTexCoordPointerEXT(
  const KL::Size & texunit,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glMultiTexCoordPointerEXT( (GLenum)texunit, (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexCoordPointerEXT( %d, %d, %d, %d, void* )", (int)texunit, (int)size, (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klMultiTexEnvfEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glMultiTexEnvfEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexEnvfEXT( %d, %d, %d, %f )", (int)texunit, (int)target, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klMultiTexEnvfvEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glMultiTexEnvfvEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexEnvfvEXT( %d, %d, %d, GLfloat* )", (int)texunit, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klMultiTexEnviEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glMultiTexEnviEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexEnviEXT( %d, %d, %d, %d )", (int)texunit, (int)target, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klMultiTexEnvivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glMultiTexEnvivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexEnvivEXT( %d, %d, %d, GLint* )", (int)texunit, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klMultiTexGendEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glMultiTexGendEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLdouble)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexGendEXT( %d, %d, %d, %f )", (int)texunit, (int)coord, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klMultiTexGendvEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glMultiTexGendvEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (const GLdouble*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexGendvEXT( %d, %d, %d, GLdouble* )", (int)texunit, (int)coord, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klMultiTexGenfEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glMultiTexGenfEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexGenfEXT( %d, %d, %d, %f )", (int)texunit, (int)coord, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klMultiTexGenfvEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glMultiTexGenfvEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexGenfvEXT( %d, %d, %d, GLfloat* )", (int)texunit, (int)coord, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klMultiTexGeniEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glMultiTexGeniEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexGeniEXT( %d, %d, %d, %d )", (int)texunit, (int)coord, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klMultiTexGenivEXT(
  const KL::Size & texunit,
  const KL::Size & coord,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glMultiTexGenivEXT( (GLenum)texunit, (GLenum)coord, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexGenivEXT( %d, %d, %d, GLint* )", (int)texunit, (int)coord, (int)pname);
  }
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
  try
  {
    glMultiTexImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexImage1DEXT( %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)format, (int)type);
  }
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
  try
  {
    glMultiTexImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)format, (int)type);
  }
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
  try
  {
    glMultiTexImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klMultiTexParameterIivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glMultiTexParameterIivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexParameterIivEXT( %d, %d, %d, GLint* )", (int)texunit, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klMultiTexParameterIuivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Size> & params
){
  try
  {
    glMultiTexParameterIuivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLuint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexParameterIuivEXT( %d, %d, %d, GLuint* )", (int)texunit, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klMultiTexParameterfEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glMultiTexParameterfEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexParameterfEXT( %d, %d, %d, %f )", (int)texunit, (int)target, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klMultiTexParameterfvEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & param
){
  try
  {
    glMultiTexParameterfvEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLfloat*)&param[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexParameterfvEXT( %d, %d, %d, GLfloat* )", (int)texunit, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klMultiTexParameteriEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glMultiTexParameteriEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexParameteriEXT( %d, %d, %d, %d )", (int)texunit, (int)target, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klMultiTexParameterivEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & param
){
  try
  {
    glMultiTexParameterivEXT( (GLenum)texunit, (GLenum)target, (GLenum)pname, (const GLint*)&param[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexParameterivEXT( %d, %d, %d, GLint* )", (int)texunit, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klMultiTexRenderbufferEXT(
  const KL::Size & texunit,
  const KL::Size & target,
  const KL::Size & renderbuffer
){
  try
  {
    glMultiTexRenderbufferEXT( (GLenum)texunit, (GLenum)target, (GLuint)renderbuffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexRenderbufferEXT( %d, %d, 0x%04X )", (int)texunit, (int)target, (unsigned)renderbuffer);
  }
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
  try
  {
    glMultiTexSubImage1DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexSubImage1DEXT( %d, %d, %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)type);
  }
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
  try
  {
    glMultiTexSubImage2DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexSubImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)type);
  }
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
  try
  {
    glMultiTexSubImage3DEXT( (GLenum)texunit, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiTexSubImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)texunit, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klNamedBufferDataEXT(
  const KL::Size & buffer,
  const KL::Size & size,
  KL::Data data,
  const KL::Size & usage
){
  try
  {
    glNamedBufferDataEXT( (GLuint)buffer, (GLsizeiptr)size, data, (GLenum)usage );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedBufferDataEXT( 0x%04X, %d, void*, %d )", (unsigned)buffer, (int)size, (int)usage);
  }
}

FABRIC_EXT_EXPORT void klNamedBufferSubDataEXT(
  const KL::Size & buffer,
  const KL::Integer & offset,
  const KL::Size & size,
  KL::Data data
){
  try
  {
    glNamedBufferSubDataEXT( (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedBufferSubDataEXT( 0x%04X, %d, %d, void* )", (unsigned)buffer, (int)offset, (int)size);
  }
}

FABRIC_EXT_EXPORT void klNamedCopyBufferSubDataEXT(
  const KL::Size & readBuffer,
  const KL::Size & writeBuffer,
  const KL::Integer & readOffset,
  const KL::Integer & writeOffset,
  const KL::Size & size
){
  try
  {
    glNamedCopyBufferSubDataEXT( (GLuint)readBuffer, (GLuint)writeBuffer, (GLintptr)readOffset, (GLintptr)writeOffset, (GLsizeiptr)size );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedCopyBufferSubDataEXT( 0x%04X, 0x%04X, %d, %d, %d )", (unsigned)readBuffer, (unsigned)writeBuffer, (int)readOffset, (int)writeOffset, (int)size);
  }
}

FABRIC_EXT_EXPORT void klNamedFramebufferRenderbufferEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & renderbuffertarget,
  const KL::Size & renderbuffer
){
  try
  {
    glNamedFramebufferRenderbufferEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)renderbuffertarget, (GLuint)renderbuffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedFramebufferRenderbufferEXT( 0x%04X, %d, %d, 0x%04X )", (unsigned)framebuffer, (int)attachment, (int)renderbuffertarget, (unsigned)renderbuffer);
  }
}

FABRIC_EXT_EXPORT void klNamedFramebufferTexture1DEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level
){
  try
  {
    glNamedFramebufferTexture1DEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedFramebufferTexture1DEXT( 0x%04X, %d, %d, 0x%04X, %d )", (unsigned)framebuffer, (int)attachment, (int)textarget, (unsigned)texture, (int)level);
  }
}

FABRIC_EXT_EXPORT void klNamedFramebufferTexture2DEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level
){
  try
  {
    glNamedFramebufferTexture2DEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedFramebufferTexture2DEXT( 0x%04X, %d, %d, 0x%04X, %d )", (unsigned)framebuffer, (int)attachment, (int)textarget, (unsigned)texture, (int)level);
  }
}

FABRIC_EXT_EXPORT void klNamedFramebufferTexture3DEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Integer & zoffset
){
  try
  {
    glNamedFramebufferTexture3DEXT( (GLuint)framebuffer, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level, (GLint)zoffset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedFramebufferTexture3DEXT( 0x%04X, %d, %d, 0x%04X, %d, %d )", (unsigned)framebuffer, (int)attachment, (int)textarget, (unsigned)texture, (int)level, (int)zoffset);
  }
}

FABRIC_EXT_EXPORT void klNamedFramebufferTextureEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level
){
  try
  {
    glNamedFramebufferTextureEXT( (GLuint)framebuffer, (GLenum)attachment, (GLuint)texture, (GLint)level );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedFramebufferTextureEXT( 0x%04X, %d, 0x%04X, %d )", (unsigned)framebuffer, (int)attachment, (unsigned)texture, (int)level);
  }
}

FABRIC_EXT_EXPORT void klNamedFramebufferTextureFaceEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Size & face
){
  try
  {
    glNamedFramebufferTextureFaceEXT( (GLuint)framebuffer, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLenum)face );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedFramebufferTextureFaceEXT( 0x%04X, %d, 0x%04X, %d, %d )", (unsigned)framebuffer, (int)attachment, (unsigned)texture, (int)level, (int)face);
  }
}

FABRIC_EXT_EXPORT void klNamedFramebufferTextureLayerEXT(
  const KL::Size & framebuffer,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Integer & layer
){
  try
  {
    glNamedFramebufferTextureLayerEXT( (GLuint)framebuffer, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedFramebufferTextureLayerEXT( 0x%04X, %d, 0x%04X, %d, %d )", (unsigned)framebuffer, (int)attachment, (unsigned)texture, (int)level, (int)layer);
  }
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
  try
  {
    glNamedProgramLocalParameter4dEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLdouble)x, (GLdouble)y, (GLdouble)z, (GLdouble)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramLocalParameter4dEXT( 0x%04X, %d, 0x%04X, %f, %f, %f, %f )", (unsigned)program, (int)target, (unsigned)index, (float)x, (float)y, (float)z, (float)w);
  }
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameter4dvEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glNamedProgramLocalParameter4dvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (const GLdouble*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramLocalParameter4dvEXT( 0x%04X, %d, 0x%04X, GLdouble* )", (unsigned)program, (int)target, (unsigned)index);
  }
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
  try
  {
    glNamedProgramLocalParameter4fEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramLocalParameter4fEXT( 0x%04X, %d, 0x%04X, %f, %f, %f, %f )", (unsigned)program, (int)target, (unsigned)index, (float)x, (float)y, (float)z, (float)w);
  }
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameter4fvEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glNamedProgramLocalParameter4fvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramLocalParameter4fvEXT( 0x%04X, %d, 0x%04X, GLfloat* )", (unsigned)program, (int)target, (unsigned)index);
  }
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
  try
  {
    glNamedProgramLocalParameterI4iEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLint)x, (GLint)y, (GLint)z, (GLint)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramLocalParameterI4iEXT( 0x%04X, %d, 0x%04X, %d, %d, %d, %d )", (unsigned)program, (int)target, (unsigned)index, (int)x, (int)y, (int)z, (int)w);
  }
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameterI4ivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glNamedProgramLocalParameterI4ivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramLocalParameterI4ivEXT( 0x%04X, %d, 0x%04X, GLint* )", (unsigned)program, (int)target, (unsigned)index);
  }
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
  try
  {
    glNamedProgramLocalParameterI4uiEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z, (GLuint)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramLocalParameterI4uiEXT( 0x%04X, %d, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (unsigned)program, (int)target, (unsigned)index, (unsigned)x, (unsigned)y, (unsigned)z, (unsigned)w);
  }
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameterI4uivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & params
){
  try
  {
    glNamedProgramLocalParameterI4uivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (const GLuint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramLocalParameterI4uivEXT( 0x%04X, %d, 0x%04X, GLuint* )", (unsigned)program, (int)target, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParameters4fvEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glNamedProgramLocalParameters4fvEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLsizei)count, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramLocalParameters4fvEXT( 0x%04X, %d, 0x%04X, %d, GLfloat* )", (unsigned)program, (int)target, (unsigned)index, (int)count);
  }
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParametersI4ivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glNamedProgramLocalParametersI4ivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLsizei)count, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramLocalParametersI4ivEXT( 0x%04X, %d, 0x%04X, %d, GLint* )", (unsigned)program, (int)target, (unsigned)index, (int)count);
  }
}

FABRIC_EXT_EXPORT void klNamedProgramLocalParametersI4uivEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & params
){
  try
  {
    glNamedProgramLocalParametersI4uivEXT( (GLuint)program, (GLenum)target, (GLuint)index, (GLsizei)count, (const GLuint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramLocalParametersI4uivEXT( 0x%04X, %d, 0x%04X, %d, GLuint* )", (unsigned)program, (int)target, (unsigned)index, (int)count);
  }
}

FABRIC_EXT_EXPORT void klNamedProgramStringEXT(
  const KL::Size & program,
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & len,
  KL::Data string
){
  try
  {
    glNamedProgramStringEXT( (GLuint)program, (GLenum)target, (GLenum)format, (GLsizei)len, string );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedProgramStringEXT( 0x%04X, %d, %d, %d, void* )", (unsigned)program, (int)target, (int)format, (int)len);
  }
}

FABRIC_EXT_EXPORT void klNamedRenderbufferStorageEXT(
  const KL::Size & renderbuffer,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  try
  {
    glNamedRenderbufferStorageEXT( (GLuint)renderbuffer, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedRenderbufferStorageEXT( 0x%04X, %d, %d, %d )", (unsigned)renderbuffer, (int)internalformat, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klNamedRenderbufferStorageMultisampleCoverageEXT(
  const KL::Size & renderbuffer,
  const KL::Size & coverageSamples,
  const KL::Size & colorSamples,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  try
  {
    glNamedRenderbufferStorageMultisampleCoverageEXT( (GLuint)renderbuffer, (GLsizei)coverageSamples, (GLsizei)colorSamples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedRenderbufferStorageMultisampleCoverageEXT( 0x%04X, %d, %d, %d, %d, %d )", (unsigned)renderbuffer, (int)coverageSamples, (int)colorSamples, (int)internalformat, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klNamedRenderbufferStorageMultisampleEXT(
  const KL::Size & renderbuffer,
  const KL::Size & samples,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  try
  {
    glNamedRenderbufferStorageMultisampleEXT( (GLuint)renderbuffer, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNamedRenderbufferStorageMultisampleEXT( 0x%04X, %d, %d, %d, %d )", (unsigned)renderbuffer, (int)samples, (int)internalformat, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform1fEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Scalar & v0
){
  try
  {
    glProgramUniform1fEXT( (GLuint)program, (GLint)location, (GLfloat)v0 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform1fEXT( 0x%04X, %d, %f )", (unsigned)program, (int)location, (float)v0);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform1fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniform1fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform1fvEXT( 0x%04X, %d, %d, GLfloat* )", (unsigned)program, (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform1iEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Integer & v0
){
  try
  {
    glProgramUniform1iEXT( (GLuint)program, (GLint)location, (GLint)v0 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform1iEXT( 0x%04X, %d, %d )", (unsigned)program, (int)location, (int)v0);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform1ivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  try
  {
    glProgramUniform1ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform1ivEXT( 0x%04X, %d, %d, GLint* )", (unsigned)program, (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform1uiEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & v0
){
  try
  {
    glProgramUniform1uiEXT( (GLuint)program, (GLint)location, (GLuint)v0 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform1uiEXT( 0x%04X, %d, 0x%04X )", (unsigned)program, (int)location, (unsigned)v0);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform1uivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  try
  {
    glProgramUniform1uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform1uivEXT( 0x%04X, %d, %d, GLuint* )", (unsigned)program, (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform2fEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Scalar & v0,
  const KL::Scalar & v1
){
  try
  {
    glProgramUniform2fEXT( (GLuint)program, (GLint)location, (GLfloat)v0, (GLfloat)v1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform2fEXT( 0x%04X, %d, %f, %f )", (unsigned)program, (int)location, (float)v0, (float)v1);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform2fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniform2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform2fvEXT( 0x%04X, %d, %d, GLfloat* )", (unsigned)program, (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform2iEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Integer & v0,
  const KL::Integer & v1
){
  try
  {
    glProgramUniform2iEXT( (GLuint)program, (GLint)location, (GLint)v0, (GLint)v1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform2iEXT( 0x%04X, %d, %d, %d )", (unsigned)program, (int)location, (int)v0, (int)v1);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform2ivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  try
  {
    glProgramUniform2ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform2ivEXT( 0x%04X, %d, %d, GLint* )", (unsigned)program, (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform2uiEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & v0,
  const KL::Size & v1
){
  try
  {
    glProgramUniform2uiEXT( (GLuint)program, (GLint)location, (GLuint)v0, (GLuint)v1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform2uiEXT( 0x%04X, %d, 0x%04X, 0x%04X )", (unsigned)program, (int)location, (unsigned)v0, (unsigned)v1);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform2uivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  try
  {
    glProgramUniform2uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform2uivEXT( 0x%04X, %d, %d, GLuint* )", (unsigned)program, (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform3fEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Scalar & v0,
  const KL::Scalar & v1,
  const KL::Scalar & v2
){
  try
  {
    glProgramUniform3fEXT( (GLuint)program, (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform3fEXT( 0x%04X, %d, %f, %f, %f )", (unsigned)program, (int)location, (float)v0, (float)v1, (float)v2);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform3fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniform3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform3fvEXT( 0x%04X, %d, %d, GLfloat* )", (unsigned)program, (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform3iEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Integer & v0,
  const KL::Integer & v1,
  const KL::Integer & v2
){
  try
  {
    glProgramUniform3iEXT( (GLuint)program, (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform3iEXT( 0x%04X, %d, %d, %d, %d )", (unsigned)program, (int)location, (int)v0, (int)v1, (int)v2);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform3ivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  try
  {
    glProgramUniform3ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform3ivEXT( 0x%04X, %d, %d, GLint* )", (unsigned)program, (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform3uiEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & v0,
  const KL::Size & v1,
  const KL::Size & v2
){
  try
  {
    glProgramUniform3uiEXT( (GLuint)program, (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform3uiEXT( 0x%04X, %d, 0x%04X, 0x%04X, 0x%04X )", (unsigned)program, (int)location, (unsigned)v0, (unsigned)v1, (unsigned)v2);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform3uivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  try
  {
    glProgramUniform3uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform3uivEXT( 0x%04X, %d, %d, GLuint* )", (unsigned)program, (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform4fEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Scalar & v0,
  const KL::Scalar & v1,
  const KL::Scalar & v2,
  const KL::Scalar & v3
){
  try
  {
    glProgramUniform4fEXT( (GLuint)program, (GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2, (GLfloat)v3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform4fEXT( 0x%04X, %d, %f, %f, %f, %f )", (unsigned)program, (int)location, (float)v0, (float)v1, (float)v2, (float)v3);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform4fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniform4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform4fvEXT( 0x%04X, %d, %d, GLfloat* )", (unsigned)program, (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform4iEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Integer & v0,
  const KL::Integer & v1,
  const KL::Integer & v2,
  const KL::Integer & v3
){
  try
  {
    glProgramUniform4iEXT( (GLuint)program, (GLint)location, (GLint)v0, (GLint)v1, (GLint)v2, (GLint)v3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform4iEXT( 0x%04X, %d, %d, %d, %d, %d )", (unsigned)program, (int)location, (int)v0, (int)v1, (int)v2, (int)v3);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform4ivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Integer> & value
){
  try
  {
    glProgramUniform4ivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform4ivEXT( 0x%04X, %d, %d, GLint* )", (unsigned)program, (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform4uiEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & v0,
  const KL::Size & v1,
  const KL::Size & v2,
  const KL::Size & v3
){
  try
  {
    glProgramUniform4uiEXT( (GLuint)program, (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2, (GLuint)v3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform4uiEXT( 0x%04X, %d, 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (unsigned)program, (int)location, (unsigned)v0, (unsigned)v1, (unsigned)v2, (unsigned)v3);
  }
}

FABRIC_EXT_EXPORT void klProgramUniform4uivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  try
  {
    glProgramUniform4uivEXT( (GLuint)program, (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniform4uivEXT( 0x%04X, %d, %d, GLuint* )", (unsigned)program, (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix2fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniformMatrix2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniformMatrix2fvEXT( 0x%04X, %d, %d, %b, GLfloat* )", (unsigned)program, (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix2x3fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniformMatrix2x3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniformMatrix2x3fvEXT( 0x%04X, %d, %d, %b, GLfloat* )", (unsigned)program, (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix2x4fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniformMatrix2x4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniformMatrix2x4fvEXT( 0x%04X, %d, %d, %b, GLfloat* )", (unsigned)program, (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix3fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniformMatrix3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniformMatrix3fvEXT( 0x%04X, %d, %d, %b, GLfloat* )", (unsigned)program, (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix3x2fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniformMatrix3x2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniformMatrix3x2fvEXT( 0x%04X, %d, %d, %b, GLfloat* )", (unsigned)program, (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix3x4fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniformMatrix3x4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniformMatrix3x4fvEXT( 0x%04X, %d, %d, %b, GLfloat* )", (unsigned)program, (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix4fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniformMatrix4fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniformMatrix4fvEXT( 0x%04X, %d, %d, %b, GLfloat* )", (unsigned)program, (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix4x2fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniformMatrix4x2fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniformMatrix4x2fvEXT( 0x%04X, %d, %d, %b, GLfloat* )", (unsigned)program, (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klProgramUniformMatrix4x3fvEXT(
  const KL::Size & program,
  const KL::Integer & location,
  const KL::Size & count,
  const KL::Boolean & transpose,
  const KL::VariableArray<KL::Scalar> & value
){
  try
  {
    glProgramUniformMatrix4x3fvEXT( (GLuint)program, (GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramUniformMatrix4x3fvEXT( 0x%04X, %d, %d, %b, GLfloat* )", (unsigned)program, (int)location, (int)count, (bool)transpose);
  }
}

FABRIC_EXT_EXPORT void klPushClientAttribDefaultEXT(
  const KL::Size & mask
){
  try
  {
    glPushClientAttribDefaultEXT( (GLbitfield)mask );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPushClientAttribDefaultEXT( 0x%04X )", (unsigned)mask);
  }
}

FABRIC_EXT_EXPORT void klTextureBufferEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & buffer
){
  try
  {
    glTextureBufferEXT( (GLuint)texture, (GLenum)target, (GLenum)internalformat, (GLuint)buffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureBufferEXT( 0x%04X, %d, %d, 0x%04X )", (unsigned)texture, (int)target, (int)internalformat, (unsigned)buffer);
  }
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
  try
  {
    glTextureImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureImage1DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)width, (int)border, (int)format, (int)type);
  }
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
  try
  {
    glTextureImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureImage2DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)border, (int)format, (int)type);
  }
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
  try
  {
    glTextureImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureImage3DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klTextureParameterIivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glTextureParameterIivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureParameterIivEXT( 0x%04X, %d, %d, GLint* )", (unsigned)texture, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klTextureParameterIuivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Size> & params
){
  try
  {
    glTextureParameterIuivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLuint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureParameterIuivEXT( 0x%04X, %d, %d, GLuint* )", (unsigned)texture, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klTextureParameterfEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glTextureParameterfEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureParameterfEXT( 0x%04X, %d, %d, %f )", (unsigned)texture, (int)target, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klTextureParameterfvEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & param
){
  try
  {
    glTextureParameterfvEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLfloat*)&param[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureParameterfvEXT( 0x%04X, %d, %d, GLfloat* )", (unsigned)texture, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klTextureParameteriEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glTextureParameteriEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureParameteriEXT( 0x%04X, %d, %d, %d )", (unsigned)texture, (int)target, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klTextureParameterivEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & param
){
  try
  {
    glTextureParameterivEXT( (GLuint)texture, (GLenum)target, (GLenum)pname, (const GLint*)&param[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureParameterivEXT( 0x%04X, %d, %d, GLint* )", (unsigned)texture, (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klTextureRenderbufferEXT(
  const KL::Size & texture,
  const KL::Size & target,
  const KL::Size & renderbuffer
){
  try
  {
    glTextureRenderbufferEXT( (GLuint)texture, (GLenum)target, (GLuint)renderbuffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureRenderbufferEXT( 0x%04X, %d, 0x%04X )", (unsigned)texture, (int)target, (unsigned)renderbuffer);
  }
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
  try
  {
    glTextureSubImage1DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureSubImage1DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)type);
  }
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
  try
  {
    glTextureSubImage2DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureSubImage2DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)type);
  }
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
  try
  {
    glTextureSubImage3DEXT( (GLuint)texture, (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureSubImage3DEXT( 0x%04X, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (unsigned)texture, (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klUnmapNamedBufferEXT(
  const KL::Size & buffer
){
  try
  {
    GLboolean result = glUnmapNamedBufferEXT( (GLuint)buffer );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUnmapNamedBufferEXT( 0x%04X )", (unsigned)buffer);
  }
}

FABRIC_EXT_EXPORT void klVertexArrayColorOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  try
  {
    glVertexArrayColorOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexArrayColorOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d, %d )", (unsigned)vaobj, (unsigned)buffer, (int)size, (int)type, (int)stride, (int)offset);
  }
}

FABRIC_EXT_EXPORT void klVertexArrayEdgeFlagOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Size & stride,
  const KL::Integer & offset
){
  try
  {
    glVertexArrayEdgeFlagOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLsizei)stride, (GLintptr)offset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexArrayEdgeFlagOffsetEXT( 0x%04X, 0x%04X, %d, %d )", (unsigned)vaobj, (unsigned)buffer, (int)stride, (int)offset);
  }
}

FABRIC_EXT_EXPORT void klVertexArrayFogCoordOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  try
  {
    glVertexArrayFogCoordOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexArrayFogCoordOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d )", (unsigned)vaobj, (unsigned)buffer, (int)type, (int)stride, (int)offset);
  }
}

FABRIC_EXT_EXPORT void klVertexArrayIndexOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  try
  {
    glVertexArrayIndexOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexArrayIndexOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d )", (unsigned)vaobj, (unsigned)buffer, (int)type, (int)stride, (int)offset);
  }
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
  try
  {
    glVertexArrayMultiTexCoordOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLenum)texunit, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexArrayMultiTexCoordOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d, %d, %d )", (unsigned)vaobj, (unsigned)buffer, (int)texunit, (int)size, (int)type, (int)stride, (int)offset);
  }
}

FABRIC_EXT_EXPORT void klVertexArrayNormalOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  try
  {
    glVertexArrayNormalOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexArrayNormalOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d )", (unsigned)vaobj, (unsigned)buffer, (int)type, (int)stride, (int)offset);
  }
}

FABRIC_EXT_EXPORT void klVertexArraySecondaryColorOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  try
  {
    glVertexArraySecondaryColorOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexArraySecondaryColorOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d, %d )", (unsigned)vaobj, (unsigned)buffer, (int)size, (int)type, (int)stride, (int)offset);
  }
}

FABRIC_EXT_EXPORT void klVertexArrayTexCoordOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  try
  {
    glVertexArrayTexCoordOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexArrayTexCoordOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d, %d )", (unsigned)vaobj, (unsigned)buffer, (int)size, (int)type, (int)stride, (int)offset);
  }
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
  try
  {
    glVertexArrayVertexAttribIOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLuint)index, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexArrayVertexAttribIOffsetEXT( 0x%04X, 0x%04X, 0x%04X, %d, %d, %d, %d )", (unsigned)vaobj, (unsigned)buffer, (unsigned)index, (int)size, (int)type, (int)stride, (int)offset);
  }
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
  try
  {
    glVertexArrayVertexAttribOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLuint)index, (GLint)size, (GLenum)type, (GLboolean)normalized, (GLsizei)stride, (GLintptr)offset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexArrayVertexAttribOffsetEXT( 0x%04X, 0x%04X, 0x%04X, %d, %d, %b, %d, %d )", (unsigned)vaobj, (unsigned)buffer, (unsigned)index, (int)size, (int)type, (bool)normalized, (int)stride, (int)offset);
  }
}

FABRIC_EXT_EXPORT void klVertexArrayVertexOffsetEXT(
  const KL::Size & vaobj,
  const KL::Size & buffer,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Integer & offset
){
  try
  {
    glVertexArrayVertexOffsetEXT( (GLuint)vaobj, (GLuint)buffer, (GLint)size, (GLenum)type, (GLsizei)stride, (GLintptr)offset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexArrayVertexOffsetEXT( 0x%04X, 0x%04X, %d, %d, %d, %d )", (unsigned)vaobj, (unsigned)buffer, (int)size, (int)type, (int)stride, (int)offset);
  }
}

FABRIC_EXT_EXPORT void klColorMaskIndexedEXT(
  const KL::Size & buf,
  const KL::Boolean & r,
  const KL::Boolean & g,
  const KL::Boolean & b,
  const KL::Boolean & a
){
  try
  {
    glColorMaskIndexedEXT( (GLuint)buf, (GLboolean)r, (GLboolean)g, (GLboolean)b, (GLboolean)a );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColorMaskIndexedEXT( 0x%04X, %b, %b, %b, %b )", (unsigned)buf, (bool)r, (bool)g, (bool)b, (bool)a);
  }
}

FABRIC_EXT_EXPORT void klDisableIndexedEXT(
  const KL::Size & target,
  const KL::Size & index
){
  try
  {
    glDisableIndexedEXT( (GLenum)target, (GLuint)index );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDisableIndexedEXT( %d, 0x%04X )", (int)target, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klEnableIndexedEXT(
  const KL::Size & target,
  const KL::Size & index
){
  try
  {
    glEnableIndexedEXT( (GLenum)target, (GLuint)index );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEnableIndexedEXT( %d, 0x%04X )", (int)target, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klGetBooleanIndexedvEXT(
  const KL::Size & value,
  const KL::Size & index,
  KL::VariableArray<KL::Boolean> & data
){
  try
  {
    glGetBooleanIndexedvEXT( (GLenum)value, (GLuint)index, (GLboolean*)&data[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetBooleanIndexedvEXT( %d, 0x%04X, GLboolean* )", (int)value, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klGetIntegerIndexedvEXT(
  const KL::Size & value,
  const KL::Size & index,
  KL::VariableArray<KL::Integer> & data
){
  try
  {
    glGetIntegerIndexedvEXT( (GLenum)value, (GLuint)index, (GLint*)&data[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetIntegerIndexedvEXT( %d, 0x%04X, GLint* )", (int)value, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsEnabledIndexedEXT(
  const KL::Size & target,
  const KL::Size & index
){
  try
  {
    GLboolean result = glIsEnabledIndexedEXT( (GLenum)target, (GLuint)index );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsEnabledIndexedEXT( %d, 0x%04X )", (int)target, (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klDrawArraysInstancedEXT(
  const KL::Size & mode,
  const KL::Integer & start,
  const KL::Size & count,
  const KL::Size & primcount
){
  try
  {
    glDrawArraysInstancedEXT( (GLenum)mode, (GLint)start, (GLsizei)count, (GLsizei)primcount );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawArraysInstancedEXT( %d, %d, %d, %d )", (int)mode, (int)start, (int)count, (int)primcount);
  }
}

FABRIC_EXT_EXPORT void klDrawElementsInstancedEXT(
  const KL::Size & mode,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices,
  const KL::Size & primcount
){
  try
  {
    glDrawElementsInstancedEXT( (GLenum)mode, (GLsizei)count, (GLenum)type, indices, (GLsizei)primcount );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawElementsInstancedEXT( %d, %d, %d, GLvoid*, %d )", (int)mode, (int)count, (int)type, (int)primcount);
  }
}

FABRIC_EXT_EXPORT void klDrawRangeElementsEXT(
  const KL::Size & mode,
  const KL::Size & start,
  const KL::Size & end,
  const KL::Size & count,
  const KL::Size & type,
  KL::Data indices
){
  try
  {
    glDrawRangeElementsEXT( (GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, indices );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawRangeElementsEXT( %d, 0x%04X, 0x%04X, %d, %d, GLvoid* )", (int)mode, (unsigned)start, (unsigned)end, (int)count, (int)type);
  }
}

FABRIC_EXT_EXPORT void klFogCoordPointerEXT(
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glFogCoordPointerEXT( (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogCoordPointerEXT( %d, %d, GLvoid* )", (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klFogCoorddEXT(
  const KL::Scalar & coord
){
  try
  {
    glFogCoorddEXT( (GLdouble)coord );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogCoorddEXT( %f )", (float)coord);
  }
}

FABRIC_EXT_EXPORT void klFogCoorddvEXT(
  const KL::VariableArray<KL::Scalar> & coord
){
  try
  {
    glFogCoorddvEXT( (const GLdouble*)&coord[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogCoorddvEXT( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klFogCoordfEXT(
  const KL::Scalar & coord
){
  try
  {
    glFogCoordfEXT( (GLfloat)coord );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogCoordfEXT( %f )", (float)coord);
  }
}

FABRIC_EXT_EXPORT void klFogCoordfvEXT(
  const KL::VariableArray<KL::Scalar> & coord
){
  try
  {
    glFogCoordfvEXT( (const GLfloat*)&coord[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFogCoordfvEXT( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klFragmentColorMaterialEXT(
  const KL::Size & face,
  const KL::Size & mode
){
  try
  {
    glFragmentColorMaterialEXT( (GLenum)face, (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentColorMaterialEXT( %d, %d )", (int)face, (int)mode);
  }
}

FABRIC_EXT_EXPORT void klFragmentLightModelfEXT(
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glFragmentLightModelfEXT( (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentLightModelfEXT( %d, %f )", (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klFragmentLightModelfvEXT(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glFragmentLightModelfvEXT( (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentLightModelfvEXT( %d, GLfloat* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klFragmentLightModeliEXT(
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glFragmentLightModeliEXT( (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentLightModeliEXT( %d, %d )", (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klFragmentLightModelivEXT(
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glFragmentLightModelivEXT( (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentLightModelivEXT( %d, GLint* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klFragmentLightfEXT(
  const KL::Size & light,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glFragmentLightfEXT( (GLenum)light, (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentLightfEXT( %d, %d, %f )", (int)light, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klFragmentLightfvEXT(
  const KL::Size & light,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glFragmentLightfvEXT( (GLenum)light, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentLightfvEXT( %d, %d, GLfloat* )", (int)light, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klFragmentLightiEXT(
  const KL::Size & light,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glFragmentLightiEXT( (GLenum)light, (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentLightiEXT( %d, %d, %d )", (int)light, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klFragmentLightivEXT(
  const KL::Size & light,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glFragmentLightivEXT( (GLenum)light, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentLightivEXT( %d, %d, GLint* )", (int)light, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klFragmentMaterialfEXT(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glFragmentMaterialfEXT( (GLenum)face, (GLenum)pname, (const GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentMaterialfEXT( %d, %d, %f )", (int)face, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klFragmentMaterialfvEXT(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glFragmentMaterialfvEXT( (GLenum)face, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentMaterialfvEXT( %d, %d, GLfloat* )", (int)face, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klFragmentMaterialiEXT(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glFragmentMaterialiEXT( (GLenum)face, (GLenum)pname, (const GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentMaterialiEXT( %d, %d, %d )", (int)face, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klFragmentMaterialivEXT(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glFragmentMaterialivEXT( (GLenum)face, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFragmentMaterialivEXT( %d, %d, GLint* )", (int)face, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetFragmentLightfvEXT(
  const KL::Size & light,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetFragmentLightfvEXT( (GLenum)light, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetFragmentLightfvEXT( %d, %d, GLfloat* )", (int)light, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetFragmentLightivEXT(
  const KL::Size & light,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetFragmentLightivEXT( (GLenum)light, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetFragmentLightivEXT( %d, %d, GLint* )", (int)light, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetFragmentMaterialfvEXT(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetFragmentMaterialfvEXT( (GLenum)face, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetFragmentMaterialfvEXT( %d, %d, GLfloat* )", (int)face, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetFragmentMaterialivEXT(
  const KL::Size & face,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetFragmentMaterialivEXT( (GLenum)face, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetFragmentMaterialivEXT( %d, %d, GLint* )", (int)face, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klLightEnviEXT(
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glLightEnviEXT( (GLenum)pname, (GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glLightEnviEXT( %d, %d )", (int)pname, (int)param);
  }
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
  const KL::Size & mask,
  const KL::Size & filter
){
  try
  {
    glBlitFramebufferEXT( (GLint)srcX0, (GLint)srcY0, (GLint)srcX1, (GLint)srcY1, (GLint)dstX0, (GLint)dstY0, (GLint)dstX1, (GLint)dstY1, (GLbitfield)mask, (GLenum)filter );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBlitFramebufferEXT( %d, %d, %d, %d, %d, %d, %d, %d, 0x%04X, %d )", (int)srcX0, (int)srcY0, (int)srcX1, (int)srcY1, (int)dstX0, (int)dstY0, (int)dstX1, (int)dstY1, (unsigned)mask, (int)filter);
  }
}

FABRIC_EXT_EXPORT void klRenderbufferStorageMultisampleEXT(
  const KL::Size & target,
  const KL::Size & samples,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  try
  {
    glRenderbufferStorageMultisampleEXT( (GLenum)target, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRenderbufferStorageMultisampleEXT( %d, %d, %d, %d, %d )", (int)target, (int)samples, (int)internalformat, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klBindFramebufferEXT(
  const KL::Size & target,
  const KL::Size & framebuffer
){
  try
  {
    glBindFramebufferEXT( (GLenum)target, (GLuint)framebuffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindFramebufferEXT( %d, 0x%04X )", (int)target, (unsigned)framebuffer);
  }
}

FABRIC_EXT_EXPORT void klBindRenderbufferEXT(
  const KL::Size & target,
  const KL::Size & renderbuffer
){
  try
  {
    glBindRenderbufferEXT( (GLenum)target, (GLuint)renderbuffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindRenderbufferEXT( %d, 0x%04X )", (int)target, (unsigned)renderbuffer);
  }
}

FABRIC_EXT_EXPORT KL::Size klCheckFramebufferStatusEXT(
  const KL::Size & target
){
  try
  {
    GLenum result = glCheckFramebufferStatusEXT( (GLenum)target );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCheckFramebufferStatusEXT( %d )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klDeleteFramebuffersEXT(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & framebuffers
){
  try
  {
    glDeleteFramebuffersEXT( (GLsizei)n, (const GLuint*)&framebuffers[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteFramebuffersEXT( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klDeleteRenderbuffersEXT(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & renderbuffers
){
  try
  {
    glDeleteRenderbuffersEXT( (GLsizei)n, (const GLuint*)&renderbuffers[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteRenderbuffersEXT( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klFramebufferRenderbufferEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & renderbuffertarget,
  const KL::Size & renderbuffer
){
  try
  {
    glFramebufferRenderbufferEXT( (GLenum)target, (GLenum)attachment, (GLenum)renderbuffertarget, (GLuint)renderbuffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferRenderbufferEXT( %d, %d, %d, 0x%04X )", (int)target, (int)attachment, (int)renderbuffertarget, (unsigned)renderbuffer);
  }
}

FABRIC_EXT_EXPORT void klFramebufferTexture1DEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level
){
  try
  {
    glFramebufferTexture1DEXT( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferTexture1DEXT( %d, %d, %d, 0x%04X, %d )", (int)target, (int)attachment, (int)textarget, (unsigned)texture, (int)level);
  }
}

FABRIC_EXT_EXPORT void klFramebufferTexture2DEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level
){
  try
  {
    glFramebufferTexture2DEXT( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferTexture2DEXT( %d, %d, %d, 0x%04X, %d )", (int)target, (int)attachment, (int)textarget, (unsigned)texture, (int)level);
  }
}

FABRIC_EXT_EXPORT void klFramebufferTexture3DEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & textarget,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Integer & zoffset
){
  try
  {
    glFramebufferTexture3DEXT( (GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level, (GLint)zoffset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferTexture3DEXT( %d, %d, %d, 0x%04X, %d, %d )", (int)target, (int)attachment, (int)textarget, (unsigned)texture, (int)level, (int)zoffset);
  }
}

FABRIC_EXT_EXPORT void klGenFramebuffersEXT(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & framebuffers
){
  try
  {
    glGenFramebuffersEXT( (GLsizei)n, (GLuint*)&framebuffers[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenFramebuffersEXT( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klGenRenderbuffersEXT(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & renderbuffers
){
  try
  {
    glGenRenderbuffersEXT( (GLsizei)n, (GLuint*)&renderbuffers[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenRenderbuffersEXT( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klGenerateMipmapEXT(
  const KL::Size & target
){
  try
  {
    glGenerateMipmapEXT( (GLenum)target );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenerateMipmapEXT( %d )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klGetFramebufferAttachmentParameterivEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetFramebufferAttachmentParameterivEXT( (GLenum)target, (GLenum)attachment, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetFramebufferAttachmentParameterivEXT( %d, %d, %d, GLint* )", (int)target, (int)attachment, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetRenderbufferParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetRenderbufferParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetRenderbufferParameterivEXT( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsFramebufferEXT(
  const KL::Size & framebuffer
){
  try
  {
    GLboolean result = glIsFramebufferEXT( (GLuint)framebuffer );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsFramebufferEXT( 0x%04X )", (unsigned)framebuffer);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsRenderbufferEXT(
  const KL::Size & renderbuffer
){
  try
  {
    GLboolean result = glIsRenderbufferEXT( (GLuint)renderbuffer );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsRenderbufferEXT( 0x%04X )", (unsigned)renderbuffer);
  }
}

FABRIC_EXT_EXPORT void klRenderbufferStorageEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & width,
  const KL::Size & height
){
  try
  {
    glRenderbufferStorageEXT( (GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glRenderbufferStorageEXT( %d, %d, %d, %d )", (int)target, (int)internalformat, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klFramebufferTextureEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level
){
  try
  {
    glFramebufferTextureEXT( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferTextureEXT( %d, %d, 0x%04X, %d )", (int)target, (int)attachment, (unsigned)texture, (int)level);
  }
}

FABRIC_EXT_EXPORT void klFramebufferTextureFaceEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Size & face
){
  try
  {
    glFramebufferTextureFaceEXT( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLenum)face );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferTextureFaceEXT( %d, %d, 0x%04X, %d, %d )", (int)target, (int)attachment, (unsigned)texture, (int)level, (int)face);
  }
}

FABRIC_EXT_EXPORT void klFramebufferTextureLayerEXT(
  const KL::Size & target,
  const KL::Size & attachment,
  const KL::Size & texture,
  const KL::Integer & level,
  const KL::Integer & layer
){
  try
  {
    glFramebufferTextureLayerEXT( (GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glFramebufferTextureLayerEXT( %d, %d, 0x%04X, %d, %d )", (int)target, (int)attachment, (unsigned)texture, (int)level, (int)layer);
  }
}

FABRIC_EXT_EXPORT void klProgramParameteriEXT(
  const KL::Size & program,
  const KL::Size & pname,
  const KL::Integer & value
){
  try
  {
    glProgramParameteriEXT( (GLuint)program, (GLenum)pname, (GLint)value );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramParameteriEXT( 0x%04X, %d, %d )", (unsigned)program, (int)pname, (int)value);
  }
}

FABRIC_EXT_EXPORT void klProgramEnvParameters4fvEXT(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glProgramEnvParameters4fvEXT( (GLenum)target, (GLuint)index, (GLsizei)count, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramEnvParameters4fvEXT( %d, 0x%04X, %d, GLfloat* )", (int)target, (unsigned)index, (int)count);
  }
}

FABRIC_EXT_EXPORT void klProgramLocalParameters4fvEXT(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & count,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glProgramLocalParameters4fvEXT( (GLenum)target, (GLuint)index, (GLsizei)count, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProgramLocalParameters4fvEXT( %d, 0x%04X, %d, GLfloat* )", (int)target, (unsigned)index, (int)count);
  }
}

FABRIC_EXT_EXPORT void klBindFragDataLocationEXT(
  const KL::Size & program,
  const KL::Size & color,
  const KL::String &name
){
  try
  {
    glBindFragDataLocationEXT( (GLuint)program, (GLuint)color, (const GLchar*)name.data() );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindFragDataLocationEXT( 0x%04X, 0x%04X, char* )", (unsigned)program, (unsigned)color);
  }
}

FABRIC_EXT_EXPORT KL::Integer klGetFragDataLocationEXT(
  const KL::Size & program,
  const KL::String &name
){
  try
  {
    GLint result = glGetFragDataLocationEXT( (GLuint)program, (const GLchar*)name.data() );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetFragDataLocationEXT( 0x%04X, char* )", (unsigned)program);
  }
}

FABRIC_EXT_EXPORT void klGetUniformuivEXT(
  const KL::Size & program,
  const KL::Integer & location,
  KL::VariableArray<KL::Size> & params
){
  try
  {
    glGetUniformuivEXT( (GLuint)program, (GLint)location, (GLuint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetUniformuivEXT( 0x%04X, %d, GLuint* )", (unsigned)program, (int)location);
  }
}

FABRIC_EXT_EXPORT void klGetVertexAttribIivEXT(
  const KL::Size & index,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetVertexAttribIivEXT( (GLuint)index, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVertexAttribIivEXT( 0x%04X, %d, GLint* )", (unsigned)index, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetVertexAttribIuivEXT(
  const KL::Size & index,
  const KL::Size & pname,
  KL::VariableArray<KL::Size> & params
){
  try
  {
    glGetVertexAttribIuivEXT( (GLuint)index, (GLenum)pname, (GLuint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVertexAttribIuivEXT( 0x%04X, %d, GLuint* )", (unsigned)index, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klUniform1uiEXT(
  const KL::Integer & location,
  const KL::Size & v0
){
  try
  {
    glUniform1uiEXT( (GLint)location, (GLuint)v0 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform1uiEXT( %d, 0x%04X )", (int)location, (unsigned)v0);
  }
}

FABRIC_EXT_EXPORT void klUniform1uivEXT(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  try
  {
    glUniform1uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform1uivEXT( %d, %d, GLuint* )", (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klUniform2uiEXT(
  const KL::Integer & location,
  const KL::Size & v0,
  const KL::Size & v1
){
  try
  {
    glUniform2uiEXT( (GLint)location, (GLuint)v0, (GLuint)v1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform2uiEXT( %d, 0x%04X, 0x%04X )", (int)location, (unsigned)v0, (unsigned)v1);
  }
}

FABRIC_EXT_EXPORT void klUniform2uivEXT(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  try
  {
    glUniform2uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform2uivEXT( %d, %d, GLuint* )", (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klUniform3uiEXT(
  const KL::Integer & location,
  const KL::Size & v0,
  const KL::Size & v1,
  const KL::Size & v2
){
  try
  {
    glUniform3uiEXT( (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform3uiEXT( %d, 0x%04X, 0x%04X, 0x%04X )", (int)location, (unsigned)v0, (unsigned)v1, (unsigned)v2);
  }
}

FABRIC_EXT_EXPORT void klUniform3uivEXT(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  try
  {
    glUniform3uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform3uivEXT( %d, %d, GLuint* )", (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klUniform4uiEXT(
  const KL::Integer & location,
  const KL::Size & v0,
  const KL::Size & v1,
  const KL::Size & v2,
  const KL::Size & v3
){
  try
  {
    glUniform4uiEXT( (GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2, (GLuint)v3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform4uiEXT( %d, 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (int)location, (unsigned)v0, (unsigned)v1, (unsigned)v2, (unsigned)v3);
  }
}

FABRIC_EXT_EXPORT void klUniform4uivEXT(
  const KL::Integer & location,
  const KL::Size & count,
  const KL::VariableArray<KL::Size> & value
){
  try
  {
    glUniform4uivEXT( (GLint)location, (GLsizei)count, (const GLuint*)&value[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUniform4uivEXT( %d, %d, GLuint* )", (int)location, (int)count);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI1iEXT(
  const KL::Size & index,
  const KL::Integer & x
){
  try
  {
    glVertexAttribI1iEXT( (GLuint)index, (GLint)x );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI1iEXT( 0x%04X, %d )", (unsigned)index, (int)x);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI1ivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttribI1ivEXT( (GLuint)index, (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI1ivEXT( 0x%04X, GLint* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI1uiEXT(
  const KL::Size & index,
  const KL::Size & x
){
  try
  {
    glVertexAttribI1uiEXT( (GLuint)index, (GLuint)x );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI1uiEXT( 0x%04X, 0x%04X )", (unsigned)index, (unsigned)x);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI1uivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glVertexAttribI1uivEXT( (GLuint)index, (const GLuint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI1uivEXT( 0x%04X, GLuint* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI2iEXT(
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y
){
  try
  {
    glVertexAttribI2iEXT( (GLuint)index, (GLint)x, (GLint)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI2iEXT( 0x%04X, %d, %d )", (unsigned)index, (int)x, (int)y);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI2ivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttribI2ivEXT( (GLuint)index, (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI2ivEXT( 0x%04X, GLint* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI2uiEXT(
  const KL::Size & index,
  const KL::Size & x,
  const KL::Size & y
){
  try
  {
    glVertexAttribI2uiEXT( (GLuint)index, (GLuint)x, (GLuint)y );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI2uiEXT( 0x%04X, 0x%04X, 0x%04X )", (unsigned)index, (unsigned)x, (unsigned)y);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI2uivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glVertexAttribI2uivEXT( (GLuint)index, (const GLuint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI2uivEXT( 0x%04X, GLuint* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI3iEXT(
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z
){
  try
  {
    glVertexAttribI3iEXT( (GLuint)index, (GLint)x, (GLint)y, (GLint)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI3iEXT( 0x%04X, %d, %d, %d )", (unsigned)index, (int)x, (int)y, (int)z);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI3ivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttribI3ivEXT( (GLuint)index, (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI3ivEXT( 0x%04X, GLint* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI3uiEXT(
  const KL::Size & index,
  const KL::Size & x,
  const KL::Size & y,
  const KL::Size & z
){
  try
  {
    glVertexAttribI3uiEXT( (GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI3uiEXT( 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (unsigned)index, (unsigned)x, (unsigned)y, (unsigned)z);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI3uivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glVertexAttribI3uivEXT( (GLuint)index, (const GLuint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI3uivEXT( 0x%04X, GLuint* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4bvEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Byte> & v
){
  try
  {
    glVertexAttribI4bvEXT( (GLuint)index, (const GLbyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4bvEXT( 0x%04X, GLbyte* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4iEXT(
  const KL::Size & index,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Integer & z,
  const KL::Integer & w
){
  try
  {
    glVertexAttribI4iEXT( (GLuint)index, (GLint)x, (GLint)y, (GLint)z, (GLint)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4iEXT( 0x%04X, %d, %d, %d, %d )", (unsigned)index, (int)x, (int)y, (int)z, (int)w);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4ivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttribI4ivEXT( (GLuint)index, (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4ivEXT( 0x%04X, GLint* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4svEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttribI4svEXT( (GLuint)index, (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4svEXT( 0x%04X, GLshort* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4ubvEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glVertexAttribI4ubvEXT( (GLuint)index, (const GLubyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4ubvEXT( 0x%04X, GLubyte* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4uiEXT(
  const KL::Size & index,
  const KL::Size & x,
  const KL::Size & y,
  const KL::Size & z,
  const KL::Size & w
){
  try
  {
    glVertexAttribI4uiEXT( (GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z, (GLuint)w );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4uiEXT( 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (unsigned)index, (unsigned)x, (unsigned)y, (unsigned)z, (unsigned)w);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4uivEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glVertexAttribI4uivEXT( (GLuint)index, (const GLuint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4uivEXT( 0x%04X, GLuint* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribI4usvEXT(
  const KL::Size & index,
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glVertexAttribI4usvEXT( (GLuint)index, (const GLushort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribI4usvEXT( 0x%04X, GLushort* )", (unsigned)index);
  }
}

FABRIC_EXT_EXPORT void klVertexAttribIPointerEXT(
  const KL::Size & index,
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glVertexAttribIPointerEXT( (GLuint)index, (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexAttribIPointerEXT( 0x%04X, %d, %d, %d, GLvoid* )", (unsigned)index, (int)size, (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klGetHistogramEXT(
  const KL::Size & target,
  const KL::Boolean & reset,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data values
){
  try
  {
    glGetHistogramEXT( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, values );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetHistogramEXT( %d, %b, %d, %d, void* )", (int)target, (bool)reset, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klGetHistogramParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetHistogramParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetHistogramParameterfvEXT( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetHistogramParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetHistogramParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetHistogramParameterivEXT( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMinmaxEXT(
  const KL::Size & target,
  const KL::Boolean & reset,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data values
){
  try
  {
    glGetMinmaxEXT( (GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, values );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMinmaxEXT( %d, %b, %d, %d, void* )", (int)target, (bool)reset, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klGetMinmaxParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetMinmaxParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMinmaxParameterfvEXT( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetMinmaxParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetMinmaxParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetMinmaxParameterivEXT( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klHistogramEXT(
  const KL::Size & target,
  const KL::Size & width,
  const KL::Size & internalformat,
  const KL::Boolean & sink
){
  try
  {
    glHistogramEXT( (GLenum)target, (GLsizei)width, (GLenum)internalformat, (GLboolean)sink );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glHistogramEXT( %d, %d, %d, %b )", (int)target, (int)width, (int)internalformat, (bool)sink);
  }
}

FABRIC_EXT_EXPORT void klMinmaxEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Boolean & sink
){
  try
  {
    glMinmaxEXT( (GLenum)target, (GLenum)internalformat, (GLboolean)sink );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMinmaxEXT( %d, %d, %b )", (int)target, (int)internalformat, (bool)sink);
  }
}

FABRIC_EXT_EXPORT void klResetHistogramEXT(
  const KL::Size & target
){
  try
  {
    glResetHistogramEXT( (GLenum)target );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glResetHistogramEXT( %d )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klResetMinmaxEXT(
  const KL::Size & target
){
  try
  {
    glResetMinmaxEXT( (GLenum)target );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glResetMinmaxEXT( %d )", (int)target);
  }
}

FABRIC_EXT_EXPORT void klIndexFuncEXT(
  const KL::Size & func,
  const KL::Scalar & ref
){
  try
  {
    glIndexFuncEXT( (GLenum)func, (GLfloat)ref );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexFuncEXT( %d, %f )", (int)func, (float)ref);
  }
}

FABRIC_EXT_EXPORT void klIndexMaterialEXT(
  const KL::Size & face,
  const KL::Size & mode
){
  try
  {
    glIndexMaterialEXT( (GLenum)face, (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexMaterialEXT( %d, %d )", (int)face, (int)mode);
  }
}

FABRIC_EXT_EXPORT void klApplyTextureEXT(
  const KL::Size & mode
){
  try
  {
    glApplyTextureEXT( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glApplyTextureEXT( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klTextureLightEXT(
  const KL::Size & pname
){
  try
  {
    glTextureLightEXT( (GLenum)pname );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureLightEXT( %d )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klTextureMaterialEXT(
  const KL::Size & face,
  const KL::Size & mode
){
  try
  {
    glTextureMaterialEXT( (GLenum)face, (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureMaterialEXT( %d, %d )", (int)face, (int)mode);
  }
}

FABRIC_EXT_EXPORT void klMultiDrawArraysEXT(
  const KL::Size & mode,
  KL::VariableArray<KL::Integer> & first,
  KL::VariableArray<KL::Size> & count,
  const KL::Size & primcount
){
  try
  {
    glMultiDrawArraysEXT( (GLenum)mode, (GLint*)&first[0], (GLsizei*)&count[0], (GLsizei)primcount );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glMultiDrawArraysEXT( %d, GLint*, GLsizei*, %d )", (int)mode, (int)primcount);
  }
}

FABRIC_EXT_EXPORT void klSampleMaskEXT(
  const KL::Scalar & value,
  const KL::Boolean & invert
){
  try
  {
    glSampleMaskEXT( (GLclampf)value, (GLboolean)invert );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSampleMaskEXT( %f, %b )", (float)value, (bool)invert);
  }
}

FABRIC_EXT_EXPORT void klSamplePatternEXT(
  const KL::Size & pattern
){
  try
  {
    glSamplePatternEXT( (GLenum)pattern );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSamplePatternEXT( %d )", (int)pattern);
  }
}

FABRIC_EXT_EXPORT void klColorTableEXT(
  const KL::Size & target,
  const KL::Size & internalFormat,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data data
){
  try
  {
    glColorTableEXT( (GLenum)target, (GLenum)internalFormat, (GLsizei)width, (GLenum)format, (GLenum)type, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColorTableEXT( %d, %d, %d, %d, %d, void* )", (int)target, (int)internalFormat, (int)width, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klGetColorTableEXT(
  const KL::Size & target,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data data
){
  try
  {
    glGetColorTableEXT( (GLenum)target, (GLenum)format, (GLenum)type, data );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetColorTableEXT( %d, %d, %d, void* )", (int)target, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klGetColorTableParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetColorTableParameterfvEXT( (GLenum)target, (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetColorTableParameterfvEXT( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetColorTableParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetColorTableParameterivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetColorTableParameterivEXT( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetPixelTransformParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glGetPixelTransformParameterfvEXT( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetPixelTransformParameterfvEXT( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetPixelTransformParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetPixelTransformParameterivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetPixelTransformParameterivEXT( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klPixelTransformParameterfEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glPixelTransformParameterfEXT( (GLenum)target, (GLenum)pname, (const GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPixelTransformParameterfEXT( %d, %d, %f )", (int)target, (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klPixelTransformParameterfvEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glPixelTransformParameterfvEXT( (GLenum)target, (GLenum)pname, (const GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPixelTransformParameterfvEXT( %d, %d, GLfloat* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klPixelTransformParameteriEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::Integer & param
){
  try
  {
    glPixelTransformParameteriEXT( (GLenum)target, (GLenum)pname, (const GLint)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPixelTransformParameteriEXT( %d, %d, %d )", (int)target, (int)pname, (int)param);
  }
}

FABRIC_EXT_EXPORT void klPixelTransformParameterivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glPixelTransformParameterivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPixelTransformParameterivEXT( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klPointParameterfEXT(
  const KL::Size & pname,
  const KL::Scalar & param
){
  try
  {
    glPointParameterfEXT( (GLenum)pname, (GLfloat)param );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPointParameterfEXT( %d, %f )", (int)pname, (float)param);
  }
}

FABRIC_EXT_EXPORT void klPointParameterfvEXT(
  const KL::Size & pname,
  KL::VariableArray<KL::Scalar> & params
){
  try
  {
    glPointParameterfvEXT( (GLenum)pname, (GLfloat*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPointParameterfvEXT( %d, GLfloat* )", (int)pname);
  }
}

FABRIC_EXT_EXPORT void klPolygonOffsetEXT(
  const KL::Scalar & factor,
  const KL::Scalar & bias
){
  try
  {
    glPolygonOffsetEXT( (GLfloat)factor, (GLfloat)bias );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPolygonOffsetEXT( %f, %f )", (float)factor, (float)bias);
  }
}

FABRIC_EXT_EXPORT void klProvokingVertexEXT(
  const KL::Size & mode
){
  try
  {
    glProvokingVertexEXT( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glProvokingVertexEXT( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klBeginSceneEXT()
{
  try
  {
    glBeginSceneEXT();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBeginSceneEXT(  )");
  }
}

FABRIC_EXT_EXPORT void klEndSceneEXT()
{
  try
  {
    glEndSceneEXT();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEndSceneEXT(  )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3bEXT(
  const KL::Byte & red,
  const KL::Byte & green,
  const KL::Byte & blue
){
  try
  {
    glSecondaryColor3bEXT( (GLbyte)red, (GLbyte)green, (GLbyte)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3bEXT( %d, %d, %d )", (int)red, (int)green, (int)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3bvEXT(
  const KL::VariableArray<KL::Byte> & v
){
  try
  {
    glSecondaryColor3bvEXT( (const GLbyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3bvEXT( GLbyte* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3dEXT(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue
){
  try
  {
    glSecondaryColor3dEXT( (GLdouble)red, (GLdouble)green, (GLdouble)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3dEXT( %f, %f, %f )", (float)red, (float)green, (float)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3dvEXT(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glSecondaryColor3dvEXT( (const GLdouble*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3dvEXT( GLdouble* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3fEXT(
  const KL::Scalar & red,
  const KL::Scalar & green,
  const KL::Scalar & blue
){
  try
  {
    glSecondaryColor3fEXT( (GLfloat)red, (GLfloat)green, (GLfloat)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3fEXT( %f, %f, %f )", (float)red, (float)green, (float)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3fvEXT(
  const KL::VariableArray<KL::Scalar> & v
){
  try
  {
    glSecondaryColor3fvEXT( (const GLfloat*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3fvEXT( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3iEXT(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  try
  {
    glSecondaryColor3iEXT( (GLint)red, (GLint)green, (GLint)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3iEXT( %d, %d, %d )", (int)red, (int)green, (int)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3ivEXT(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glSecondaryColor3ivEXT( (const GLint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3ivEXT( GLint* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3sEXT(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  try
  {
    glSecondaryColor3sEXT( (GLshort)red, (GLshort)green, (GLshort)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3sEXT( %d, %d, %d )", (int)red, (int)green, (int)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3svEXT(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glSecondaryColor3svEXT( (const GLshort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3svEXT( GLshort* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3ubEXT(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue
){
  try
  {
    glSecondaryColor3ubEXT( (GLubyte)red, (GLubyte)green, (GLubyte)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3ubEXT( 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3ubvEXT(
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glSecondaryColor3ubvEXT( (const GLubyte*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3ubvEXT( GLubyte* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3uiEXT(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue
){
  try
  {
    glSecondaryColor3uiEXT( (GLuint)red, (GLuint)green, (GLuint)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3uiEXT( 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3uivEXT(
  const KL::VariableArray<KL::Size> & v
){
  try
  {
    glSecondaryColor3uivEXT( (const GLuint*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3uivEXT( GLuint* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3usEXT(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue
){
  try
  {
    glSecondaryColor3usEXT( (GLushort)red, (GLushort)green, (GLushort)blue );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3usEXT( 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue);
  }
}

FABRIC_EXT_EXPORT void klSecondaryColor3usvEXT(
  const KL::VariableArray<KL::Integer> & v
){
  try
  {
    glSecondaryColor3usvEXT( (const GLushort*)&v[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColor3usvEXT( GLushort* )");
  }
}

FABRIC_EXT_EXPORT void klSecondaryColorPointerEXT(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glSecondaryColorPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSecondaryColorPointerEXT( %d, %d, %d, GLvoid* )", (int)size, (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klActiveProgramEXT(
  const KL::Size & program
){
  try
  {
    glActiveProgramEXT( (GLuint)program );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glActiveProgramEXT( 0x%04X )", (unsigned)program);
  }
}

FABRIC_EXT_EXPORT KL::Size klCreateShaderProgramEXT(
  const KL::Size & type,
  const KL::String &string
){
  try
  {
    GLuint result = glCreateShaderProgramEXT( (GLenum)type, (const char*)string.data() );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glCreateShaderProgramEXT( %d, char* )", (int)type);
  }
}

FABRIC_EXT_EXPORT void klUseShaderProgramEXT(
  const KL::Size & type,
  const KL::Size & program
){
  try
  {
    glUseShaderProgramEXT( (GLenum)type, (GLuint)program );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glUseShaderProgramEXT( %d, 0x%04X )", (int)type, (unsigned)program);
  }
}

FABRIC_EXT_EXPORT void klActiveStencilFaceEXT(
  const KL::Size & face
){
  try
  {
    glActiveStencilFaceEXT( (GLenum)face );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glActiveStencilFaceEXT( %d )", (int)face);
  }
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
  try
  {
    glTexSubImage1DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexSubImage1DEXT( %d, %d, %d, %d, %d, %d, void* )", (int)target, (int)level, (int)xoffset, (int)width, (int)format, (int)type);
  }
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
  try
  {
    glTexSubImage2DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexSubImage2DEXT( %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)width, (int)height, (int)format, (int)type);
  }
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
  try
  {
    glTexSubImage3DEXT( (GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexSubImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)target, (int)level, (int)xoffset, (int)yoffset, (int)zoffset, (int)width, (int)height, (int)depth, (int)format, (int)type);
  }
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
  try
  {
    glTexImage3DEXT( (GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, pixels );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexImage3DEXT( %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)target, (int)level, (int)internalformat, (int)width, (int)height, (int)depth, (int)border, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT void klTexBufferEXT(
  const KL::Size & target,
  const KL::Size & internalformat,
  const KL::Size & buffer
){
  try
  {
    glTexBufferEXT( (GLenum)target, (GLenum)internalformat, (GLuint)buffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexBufferEXT( %d, %d, 0x%04X )", (int)target, (int)internalformat, (unsigned)buffer);
  }
}

FABRIC_EXT_EXPORT void klClearColorIiEXT(
  const KL::Integer & red,
  const KL::Integer & green,
  const KL::Integer & blue,
  const KL::Integer & alpha
){
  try
  {
    glClearColorIiEXT( (GLint)red, (GLint)green, (GLint)blue, (GLint)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClearColorIiEXT( %d, %d, %d, %d )", (int)red, (int)green, (int)blue, (int)alpha);
  }
}

FABRIC_EXT_EXPORT void klClearColorIuiEXT(
  const KL::Size & red,
  const KL::Size & green,
  const KL::Size & blue,
  const KL::Size & alpha
){
  try
  {
    glClearColorIuiEXT( (GLuint)red, (GLuint)green, (GLuint)blue, (GLuint)alpha );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glClearColorIuiEXT( 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (unsigned)red, (unsigned)green, (unsigned)blue, (unsigned)alpha);
  }
}

FABRIC_EXT_EXPORT void klGetTexParameterIivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Integer> & params
){
  try
  {
    glGetTexParameterIivEXT( (GLenum)target, (GLenum)pname, (GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexParameterIivEXT( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klGetTexParameterIuivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  KL::VariableArray<KL::Size> & params
){
  try
  {
    glGetTexParameterIuivEXT( (GLenum)target, (GLenum)pname, (GLuint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTexParameterIuivEXT( %d, %d, GLuint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klTexParameterIivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Integer> & params
){
  try
  {
    glTexParameterIivEXT( (GLenum)target, (GLenum)pname, (const GLint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexParameterIivEXT( %d, %d, GLint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT void klTexParameterIuivEXT(
  const KL::Size & target,
  const KL::Size & pname,
  const KL::VariableArray<KL::Size> & params
){
  try
  {
    glTexParameterIuivEXT( (GLenum)target, (GLenum)pname, (const GLuint*)&params[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexParameterIuivEXT( %d, %d, GLuint* )", (int)target, (int)pname);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klAreTexturesResidentEXT(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & textures,
  KL::VariableArray<KL::Boolean> & residences
){
  try
  {
    GLboolean result = glAreTexturesResidentEXT( (GLsizei)n, (const GLuint*)&textures[0], (GLboolean*)&residences[0] );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glAreTexturesResidentEXT( %d, GLuint*, GLboolean* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klBindTextureEXT(
  const KL::Size & target,
  const KL::Size & texture
){
  try
  {
    glBindTextureEXT( (GLenum)target, (GLuint)texture );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindTextureEXT( %d, 0x%04X )", (int)target, (unsigned)texture);
  }
}

FABRIC_EXT_EXPORT void klDeleteTexturesEXT(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & textures
){
  try
  {
    glDeleteTexturesEXT( (GLsizei)n, (const GLuint*)&textures[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteTexturesEXT( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klGenTexturesEXT(
  const KL::Size & n,
  KL::VariableArray<KL::Size> & textures
){
  try
  {
    glGenTexturesEXT( (GLsizei)n, (GLuint*)&textures[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenTexturesEXT( %d, GLuint* )", (int)n);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsTextureEXT(
  const KL::Size & texture
){
  try
  {
    GLboolean result = glIsTextureEXT( (GLuint)texture );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsTextureEXT( 0x%04X )", (unsigned)texture);
  }
}

FABRIC_EXT_EXPORT void klPrioritizeTexturesEXT(
  const KL::Size & n,
  const KL::VariableArray<KL::Size> & textures,
  const KL::VariableArray<KL::Scalar> & priorities
){
  try
  {
    glPrioritizeTexturesEXT( (GLsizei)n, (const GLuint*)&textures[0], (const GLclampf*)&priorities[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glPrioritizeTexturesEXT( %d, GLuint*, GLclampf* )", (int)n);
  }
}

FABRIC_EXT_EXPORT void klTextureNormalEXT(
  const KL::Size & mode
){
  try
  {
    glTextureNormalEXT( (GLenum)mode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTextureNormalEXT( %d )", (int)mode);
  }
}

FABRIC_EXT_EXPORT void klBeginTransformFeedbackEXT(
  const KL::Size & primitiveMode
){
  try
  {
    glBeginTransformFeedbackEXT( (GLenum)primitiveMode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBeginTransformFeedbackEXT( %d )", (int)primitiveMode);
  }
}

FABRIC_EXT_EXPORT void klBindBufferBaseEXT(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & buffer
){
  try
  {
    glBindBufferBaseEXT( (GLenum)target, (GLuint)index, (GLuint)buffer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindBufferBaseEXT( %d, 0x%04X, 0x%04X )", (int)target, (unsigned)index, (unsigned)buffer);
  }
}

FABRIC_EXT_EXPORT void klBindBufferOffsetEXT(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & buffer,
  const KL::Integer & offset
){
  try
  {
    glBindBufferOffsetEXT( (GLenum)target, (GLuint)index, (GLuint)buffer, (GLintptr)offset );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindBufferOffsetEXT( %d, 0x%04X, 0x%04X, %d )", (int)target, (unsigned)index, (unsigned)buffer, (int)offset);
  }
}

FABRIC_EXT_EXPORT void klBindBufferRangeEXT(
  const KL::Size & target,
  const KL::Size & index,
  const KL::Size & buffer,
  const KL::Integer & offset,
  const KL::Size & size
){
  try
  {
    glBindBufferRangeEXT( (GLenum)target, (GLuint)index, (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindBufferRangeEXT( %d, 0x%04X, 0x%04X, %d, %d )", (int)target, (unsigned)index, (unsigned)buffer, (int)offset, (int)size);
  }
}

FABRIC_EXT_EXPORT void klEndTransformFeedbackEXT()
{
  try
  {
    glEndTransformFeedbackEXT();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEndTransformFeedbackEXT(  )");
  }
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
  try
  {
    char * nameStr = new char[1024];
    glGetTransformFeedbackVaryingEXT( (GLuint)program, (GLuint)index, (GLsizei)bufSize, (GLsizei*)&length[0], (GLsizei*)&size[0], (GLenum*)&type[0], (char*)nameStr );
    name = KL::String(nameStr);
    delete( nameStr );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetTransformFeedbackVaryingEXT( 0x%04X, 0x%04X, %d, GLsizei*, GLsizei*, GLenum*, char* )", (unsigned)program, (unsigned)index, (int)bufSize);
  }
}

FABRIC_EXT_EXPORT void klTransformFeedbackVaryingsEXT(
  const KL::Size & program,
  const KL::Size & count,
  const KL::VariableArray<KL::String> &varyings,
  const KL::Size & bufferMode
){
  try
  {
    glTransformFeedbackVaryingsEXT( (GLuint)program, (GLsizei)count, (const char**)&varyings[0], (GLenum)bufferMode );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTransformFeedbackVaryingsEXT( 0x%04X, %d, char*, %d )", (unsigned)program, (int)count, (int)bufferMode);
  }
}

FABRIC_EXT_EXPORT void klArrayElementEXT(
  const KL::Integer & i
){
  try
  {
    glArrayElementEXT( (GLint)i );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glArrayElementEXT( %d )", (int)i);
  }
}

FABRIC_EXT_EXPORT void klColorPointerEXT(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Size & count,
  KL::Data pointer
){
  try
  {
    glColorPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glColorPointerEXT( %d, %d, %d, %d, void* )", (int)size, (int)type, (int)stride, (int)count);
  }
}

FABRIC_EXT_EXPORT void klDrawArraysEXT(
  const KL::Size & mode,
  const KL::Integer & first,
  const KL::Size & count
){
  try
  {
    glDrawArraysEXT( (GLenum)mode, (GLint)first, (GLsizei)count );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawArraysEXT( %d, %d, %d )", (int)mode, (int)first, (int)count);
  }
}

FABRIC_EXT_EXPORT void klEdgeFlagPointerEXT(
  const KL::Size & stride,
  const KL::Size & count,
  const KL::VariableArray<KL::Boolean> & pointer
){
  try
  {
    glEdgeFlagPointerEXT( (GLsizei)stride, (GLsizei)count, (const GLboolean*)&pointer[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEdgeFlagPointerEXT( %d, %d, GLboolean* )", (int)stride, (int)count);
  }
}

FABRIC_EXT_EXPORT void klIndexPointerEXT(
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Size & count,
  KL::Data pointer
){
  try
  {
    glIndexPointerEXT( (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIndexPointerEXT( %d, %d, %d, void* )", (int)type, (int)stride, (int)count);
  }
}

FABRIC_EXT_EXPORT void klNormalPointerEXT(
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Size & count,
  KL::Data pointer
){
  try
  {
    glNormalPointerEXT( (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNormalPointerEXT( %d, %d, %d, void* )", (int)type, (int)stride, (int)count);
  }
}

FABRIC_EXT_EXPORT void klTexCoordPointerEXT(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Size & count,
  KL::Data pointer
){
  try
  {
    glTexCoordPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glTexCoordPointerEXT( %d, %d, %d, %d, void* )", (int)size, (int)type, (int)stride, (int)count);
  }
}

FABRIC_EXT_EXPORT void klVertexPointerEXT(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  const KL::Size & count,
  KL::Data pointer
){
  try
  {
    glVertexPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, (GLsizei)count, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexPointerEXT( %d, %d, %d, %d, void* )", (int)size, (int)type, (int)stride, (int)count);
  }
}

FABRIC_EXT_EXPORT void klBeginVertexShaderEXT()
{
  try
  {
    glBeginVertexShaderEXT();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBeginVertexShaderEXT(  )");
  }
}

FABRIC_EXT_EXPORT KL::Size klBindLightParameterEXT(
  const KL::Size & light,
  const KL::Size & value
){
  try
  {
    GLuint result = glBindLightParameterEXT( (GLenum)light, (GLenum)value );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindLightParameterEXT( %d, %d )", (int)light, (int)value);
  }
}

FABRIC_EXT_EXPORT KL::Size klBindMaterialParameterEXT(
  const KL::Size & face,
  const KL::Size & value
){
  try
  {
    GLuint result = glBindMaterialParameterEXT( (GLenum)face, (GLenum)value );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindMaterialParameterEXT( %d, %d )", (int)face, (int)value);
  }
}

FABRIC_EXT_EXPORT KL::Size klBindParameterEXT(
  const KL::Size & value
){
  try
  {
    GLuint result = glBindParameterEXT( (GLenum)value );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindParameterEXT( %d )", (int)value);
  }
}

FABRIC_EXT_EXPORT KL::Size klBindTexGenParameterEXT(
  const KL::Size & unit,
  const KL::Size & coord,
  const KL::Size & value
){
  try
  {
    GLuint result = glBindTexGenParameterEXT( (GLenum)unit, (GLenum)coord, (GLenum)value );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindTexGenParameterEXT( %d, %d, %d )", (int)unit, (int)coord, (int)value);
  }
}

FABRIC_EXT_EXPORT KL::Size klBindTextureUnitParameterEXT(
  const KL::Size & unit,
  const KL::Size & value
){
  try
  {
    GLuint result = glBindTextureUnitParameterEXT( (GLenum)unit, (GLenum)value );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindTextureUnitParameterEXT( %d, %d )", (int)unit, (int)value);
  }
}

FABRIC_EXT_EXPORT void klBindVertexShaderEXT(
  const KL::Size & id
){
  try
  {
    glBindVertexShaderEXT( (GLuint)id );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBindVertexShaderEXT( 0x%04X )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klDeleteVertexShaderEXT(
  const KL::Size & id
){
  try
  {
    glDeleteVertexShaderEXT( (GLuint)id );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteVertexShaderEXT( 0x%04X )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klDisableVariantClientStateEXT(
  const KL::Size & id
){
  try
  {
    glDisableVariantClientStateEXT( (GLuint)id );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDisableVariantClientStateEXT( 0x%04X )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klEnableVariantClientStateEXT(
  const KL::Size & id
){
  try
  {
    glEnableVariantClientStateEXT( (GLuint)id );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEnableVariantClientStateEXT( 0x%04X )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klEndVertexShaderEXT()
{
  try
  {
    glEndVertexShaderEXT();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glEndVertexShaderEXT(  )");
  }
}

FABRIC_EXT_EXPORT void klExtractComponentEXT(
  const KL::Size & res,
  const KL::Size & src,
  const KL::Size & num
){
  try
  {
    glExtractComponentEXT( (GLuint)res, (GLuint)src, (GLuint)num );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glExtractComponentEXT( 0x%04X, 0x%04X, 0x%04X )", (unsigned)res, (unsigned)src, (unsigned)num);
  }
}

FABRIC_EXT_EXPORT KL::Size klGenSymbolsEXT(
  const KL::Size & dataType,
  const KL::Size & storageType,
  const KL::Size & range,
  const KL::Size & components
){
  try
  {
    GLuint result = glGenSymbolsEXT( (GLenum)dataType, (GLenum)storageType, (GLenum)range, (GLuint)components );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenSymbolsEXT( %d, %d, %d, 0x%04X )", (int)dataType, (int)storageType, (int)range, (unsigned)components);
  }
}

FABRIC_EXT_EXPORT KL::Size klGenVertexShadersEXT(
  const KL::Size & range
){
  try
  {
    GLuint result = glGenVertexShadersEXT( (GLuint)range );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGenVertexShadersEXT( 0x%04X )", (unsigned)range);
  }
}

FABRIC_EXT_EXPORT void klGetInvariantBooleanvEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Boolean> & data
){
  try
  {
    glGetInvariantBooleanvEXT( (GLuint)id, (GLenum)value, (GLboolean*)&data[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetInvariantBooleanvEXT( 0x%04X, %d, GLboolean* )", (unsigned)id, (int)value);
  }
}

FABRIC_EXT_EXPORT void klGetInvariantFloatvEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Scalar> & data
){
  try
  {
    glGetInvariantFloatvEXT( (GLuint)id, (GLenum)value, (GLfloat*)&data[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetInvariantFloatvEXT( 0x%04X, %d, GLfloat* )", (unsigned)id, (int)value);
  }
}

FABRIC_EXT_EXPORT void klGetInvariantIntegervEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Integer> & data
){
  try
  {
    glGetInvariantIntegervEXT( (GLuint)id, (GLenum)value, (GLint*)&data[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetInvariantIntegervEXT( 0x%04X, %d, GLint* )", (unsigned)id, (int)value);
  }
}

FABRIC_EXT_EXPORT void klGetLocalConstantBooleanvEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Boolean> & data
){
  try
  {
    glGetLocalConstantBooleanvEXT( (GLuint)id, (GLenum)value, (GLboolean*)&data[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetLocalConstantBooleanvEXT( 0x%04X, %d, GLboolean* )", (unsigned)id, (int)value);
  }
}

FABRIC_EXT_EXPORT void klGetLocalConstantFloatvEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Scalar> & data
){
  try
  {
    glGetLocalConstantFloatvEXT( (GLuint)id, (GLenum)value, (GLfloat*)&data[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetLocalConstantFloatvEXT( 0x%04X, %d, GLfloat* )", (unsigned)id, (int)value);
  }
}

FABRIC_EXT_EXPORT void klGetLocalConstantIntegervEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Integer> & data
){
  try
  {
    glGetLocalConstantIntegervEXT( (GLuint)id, (GLenum)value, (GLint*)&data[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetLocalConstantIntegervEXT( 0x%04X, %d, GLint* )", (unsigned)id, (int)value);
  }
}

FABRIC_EXT_EXPORT void klGetVariantBooleanvEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Boolean> & data
){
  try
  {
    glGetVariantBooleanvEXT( (GLuint)id, (GLenum)value, (GLboolean*)&data[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVariantBooleanvEXT( 0x%04X, %d, GLboolean* )", (unsigned)id, (int)value);
  }
}

FABRIC_EXT_EXPORT void klGetVariantFloatvEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Scalar> & data
){
  try
  {
    glGetVariantFloatvEXT( (GLuint)id, (GLenum)value, (GLfloat*)&data[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVariantFloatvEXT( 0x%04X, %d, GLfloat* )", (unsigned)id, (int)value);
  }
}

FABRIC_EXT_EXPORT void klGetVariantIntegervEXT(
  const KL::Size & id,
  const KL::Size & value,
  KL::VariableArray<KL::Integer> & data
){
  try
  {
    glGetVariantIntegervEXT( (GLuint)id, (GLenum)value, (GLint*)&data[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glGetVariantIntegervEXT( 0x%04X, %d, GLint* )", (unsigned)id, (int)value);
  }
}

FABRIC_EXT_EXPORT void klInsertComponentEXT(
  const KL::Size & res,
  const KL::Size & src,
  const KL::Size & num
){
  try
  {
    glInsertComponentEXT( (GLuint)res, (GLuint)src, (GLuint)num );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glInsertComponentEXT( 0x%04X, 0x%04X, 0x%04X )", (unsigned)res, (unsigned)src, (unsigned)num);
  }
}

FABRIC_EXT_EXPORT KL::Boolean klIsVariantEnabledEXT(
  const KL::Size & id,
  const KL::Size & cap
){
  try
  {
    GLboolean result = glIsVariantEnabledEXT( (GLuint)id, (GLenum)cap );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glIsVariantEnabledEXT( 0x%04X, %d )", (unsigned)id, (int)cap);
  }
}

FABRIC_EXT_EXPORT void klSetInvariantEXT(
  const KL::Size & id,
  const KL::Size & type,
  KL::Data addr
){
  try
  {
    glSetInvariantEXT( (GLuint)id, (GLenum)type, addr );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSetInvariantEXT( 0x%04X, %d, GLvoid* )", (unsigned)id, (int)type);
  }
}

FABRIC_EXT_EXPORT void klSetLocalConstantEXT(
  const KL::Size & id,
  const KL::Size & type,
  KL::Data addr
){
  try
  {
    glSetLocalConstantEXT( (GLuint)id, (GLenum)type, addr );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSetLocalConstantEXT( 0x%04X, %d, GLvoid* )", (unsigned)id, (int)type);
  }
}

FABRIC_EXT_EXPORT void klShaderOp1EXT(
  const KL::Size & op,
  const KL::Size & res,
  const KL::Size & arg1
){
  try
  {
    glShaderOp1EXT( (GLenum)op, (GLuint)res, (GLuint)arg1 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glShaderOp1EXT( %d, 0x%04X, 0x%04X )", (int)op, (unsigned)res, (unsigned)arg1);
  }
}

FABRIC_EXT_EXPORT void klShaderOp2EXT(
  const KL::Size & op,
  const KL::Size & res,
  const KL::Size & arg1,
  const KL::Size & arg2
){
  try
  {
    glShaderOp2EXT( (GLenum)op, (GLuint)res, (GLuint)arg1, (GLuint)arg2 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glShaderOp2EXT( %d, 0x%04X, 0x%04X, 0x%04X )", (int)op, (unsigned)res, (unsigned)arg1, (unsigned)arg2);
  }
}

FABRIC_EXT_EXPORT void klShaderOp3EXT(
  const KL::Size & op,
  const KL::Size & res,
  const KL::Size & arg1,
  const KL::Size & arg2,
  const KL::Size & arg3
){
  try
  {
    glShaderOp3EXT( (GLenum)op, (GLuint)res, (GLuint)arg1, (GLuint)arg2, (GLuint)arg3 );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glShaderOp3EXT( %d, 0x%04X, 0x%04X, 0x%04X, 0x%04X )", (int)op, (unsigned)res, (unsigned)arg1, (unsigned)arg2, (unsigned)arg3);
  }
}

FABRIC_EXT_EXPORT void klSwizzleEXT(
  const KL::Size & res,
  const KL::Size & in,
  const KL::Size & outX,
  const KL::Size & outY,
  const KL::Size & outZ,
  const KL::Size & outW
){
  try
  {
    glSwizzleEXT( (GLuint)res, (GLuint)in, (GLenum)outX, (GLenum)outY, (GLenum)outZ, (GLenum)outW );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glSwizzleEXT( 0x%04X, 0x%04X, %d, %d, %d, %d )", (unsigned)res, (unsigned)in, (int)outX, (int)outY, (int)outZ, (int)outW);
  }
}

FABRIC_EXT_EXPORT void klVariantPointerEXT(
  const KL::Size & id,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data addr
){
  try
  {
    glVariantPointerEXT( (GLuint)id, (GLenum)type, (GLuint)stride, addr );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVariantPointerEXT( 0x%04X, %d, 0x%04X, GLvoid* )", (unsigned)id, (int)type, (unsigned)stride);
  }
}

FABRIC_EXT_EXPORT void klVariantbvEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Byte> & addr
){
  try
  {
    glVariantbvEXT( (GLuint)id, (GLbyte*)&addr[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVariantbvEXT( 0x%04X, GLbyte* )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klVariantdvEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Scalar> & addr
){
  try
  {
    glVariantdvEXT( (GLuint)id, (GLdouble*)&addr[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVariantdvEXT( 0x%04X, GLdouble* )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klVariantfvEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Scalar> & addr
){
  try
  {
    glVariantfvEXT( (GLuint)id, (GLfloat*)&addr[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVariantfvEXT( 0x%04X, GLfloat* )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klVariantivEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Integer> & addr
){
  try
  {
    glVariantivEXT( (GLuint)id, (GLint*)&addr[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVariantivEXT( 0x%04X, GLint* )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klVariantsvEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Integer> & addr
){
  try
  {
    glVariantsvEXT( (GLuint)id, (GLshort*)&addr[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVariantsvEXT( 0x%04X, GLshort* )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klVariantubvEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Size> & addr
){
  try
  {
    glVariantubvEXT( (GLuint)id, (GLubyte*)&addr[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVariantubvEXT( 0x%04X, GLubyte* )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klVariantuivEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Size> & addr
){
  try
  {
    glVariantuivEXT( (GLuint)id, (GLuint*)&addr[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVariantuivEXT( 0x%04X, GLuint* )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klVariantusvEXT(
  const KL::Size & id,
  KL::VariableArray<KL::Integer> & addr
){
  try
  {
    glVariantusvEXT( (GLuint)id, (GLushort*)&addr[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVariantusvEXT( 0x%04X, GLushort* )", (unsigned)id);
  }
}

FABRIC_EXT_EXPORT void klWriteMaskEXT(
  const KL::Size & res,
  const KL::Size & in,
  const KL::Size & outX,
  const KL::Size & outY,
  const KL::Size & outZ,
  const KL::Size & outW
){
  try
  {
    glWriteMaskEXT( (GLuint)res, (GLuint)in, (GLenum)outX, (GLenum)outY, (GLenum)outZ, (GLenum)outW );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glWriteMaskEXT( 0x%04X, 0x%04X, %d, %d, %d, %d )", (unsigned)res, (unsigned)in, (int)outX, (int)outY, (int)outZ, (int)outW);
  }
}

FABRIC_EXT_EXPORT void klVertexWeightPointerEXT(
  const KL::Integer & size,
  const KL::Size & type,
  const KL::Size & stride,
  KL::Data pointer
){
  try
  {
    glVertexWeightPointerEXT( (GLint)size, (GLenum)type, (GLsizei)stride, pointer );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexWeightPointerEXT( %d, %d, %d, void* )", (int)size, (int)type, (int)stride);
  }
}

FABRIC_EXT_EXPORT void klVertexWeightfEXT(
  const KL::Scalar & weight
){
  try
  {
    glVertexWeightfEXT( (GLfloat)weight );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexWeightfEXT( %f )", (float)weight);
  }
}

FABRIC_EXT_EXPORT void klVertexWeightfvEXT(
  KL::VariableArray<KL::Scalar> & weight
){
  try
  {
    glVertexWeightfvEXT( (GLfloat*)&weight[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glVertexWeightfvEXT( GLfloat* )");
  }
}

FABRIC_EXT_EXPORT KL::Size klBufferRegionEnabledEXT()
{
  try
  {
    GLuint result = glBufferRegionEnabledEXT();
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glBufferRegionEnabledEXT(  )");
  }
}

FABRIC_EXT_EXPORT void klDeleteBufferRegionEXT(
  const KL::Size & region
){
  try
  {
    glDeleteBufferRegionEXT( (GLenum)region );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDeleteBufferRegionEXT( %d )", (int)region);
  }
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
  try
  {
    glDrawBufferRegionEXT( (GLuint)region, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)xDest, (GLint)yDest );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glDrawBufferRegionEXT( 0x%04X, %d, %d, %d, %d, %d, %d )", (unsigned)region, (int)x, (int)y, (int)width, (int)height, (int)xDest, (int)yDest);
  }
}

FABRIC_EXT_EXPORT KL::Size klNewBufferRegionEXT(
  const KL::Size & region
){
  try
  {
    GLuint result = glNewBufferRegionEXT( (GLenum)region );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glNewBufferRegionEXT( %d )", (int)region);
  }
}

FABRIC_EXT_EXPORT void klReadBufferRegionEXT(
  const KL::Size & region,
  const KL::Integer & x,
  const KL::Integer & y,
  const KL::Size & width,
  const KL::Size & height
){
  try
  {
    glReadBufferRegionEXT( (GLuint)region, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glReadBufferRegionEXT( 0x%04X, %d, %d, %d, %d )", (unsigned)region, (int)x, (int)y, (int)width, (int)height);
  }
}

FABRIC_EXT_EXPORT void klXWaitGL()
{
  try
  {
    glXWaitGL();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glXWaitGL(  )");
  }
}

FABRIC_EXT_EXPORT void klXWaitX()
{
  try
  {
    glXWaitX();
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glXWaitX(  )");
  }
}

FABRIC_EXT_EXPORT KL::Boolean klxewIsSupported(
  const KL::String &name
){
  try
  {
    GLboolean result = glxewIsSupported( (const char*)name.data() );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glxewIsSupported( char* )");
  }
}

FABRIC_EXT_EXPORT KL::Boolean klxewGetExtension(
  const KL::String &name
){
  try
  {
    GLboolean result = glxewGetExtension( (const char*)name.data() );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::glxewGetExtension( char* )");
  }
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
  try
  {
    GLint result = gluBuild1DMipmapLevels( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLenum)format, (GLenum)type, (GLint)level, (GLint)base, (GLint)max, data );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluBuild1DMipmapLevels( %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)target, (int)internalFormat, (int)width, (int)format, (int)type, (int)level, (int)base, (int)max);
  }
}

FABRIC_EXT_EXPORT KL::Integer kluBuild1DMipmaps(
  const KL::Size & target,
  const KL::Integer & internalFormat,
  const KL::Size & width,
  const KL::Size & format,
  const KL::Size & type,
  KL::Data data
){
  try
  {
    GLint result = gluBuild1DMipmaps( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLenum)format, (GLenum)type, data );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluBuild1DMipmaps( %d, %d, %d, %d, %d, void* )", (int)target, (int)internalFormat, (int)width, (int)format, (int)type);
  }
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
  try
  {
    GLint result = gluBuild2DMipmapLevels( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, (GLint)level, (GLint)base, (GLint)max, data );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluBuild2DMipmapLevels( %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)target, (int)internalFormat, (int)width, (int)height, (int)format, (int)type, (int)level, (int)base, (int)max);
  }
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
  try
  {
    GLint result = gluBuild2DMipmaps( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, data );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluBuild2DMipmaps( %d, %d, %d, %d, %d, %d, void* )", (int)target, (int)internalFormat, (int)width, (int)height, (int)format, (int)type);
  }
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
  try
  {
    GLint result = gluBuild3DMipmapLevels( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, (GLint)level, (GLint)base, (GLint)max, data );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluBuild3DMipmapLevels( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, void* )", (int)target, (int)internalFormat, (int)width, (int)height, (int)depth, (int)format, (int)type, (int)level, (int)base, (int)max);
  }
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
  try
  {
    GLint result = gluBuild3DMipmaps( (GLenum)target, (GLint)internalFormat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, data );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluBuild3DMipmaps( %d, %d, %d, %d, %d, %d, %d, void* )", (int)target, (int)internalFormat, (int)width, (int)height, (int)depth, (int)format, (int)type);
  }
}

FABRIC_EXT_EXPORT KL::Boolean kluCheckExtension(
  const KL::VariableArray<KL::Size> & extName,
  const KL::VariableArray<KL::Size> & extString
){
  try
  {
    GLboolean result = gluCheckExtension( (const GLubyte*)&extName[0], (const GLubyte*)&extString[0] );
  return (KL::Boolean)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluCheckExtension( GLubyte*, GLubyte* )");
  }
}

FABRIC_EXT_EXPORT KL::Size kluErrorString(
  const KL::Size & error
){
  try
  {
    const GLubyte* result = gluErrorString( (GLenum)error );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluErrorString( %d )", (int)error);
  }
}

FABRIC_EXT_EXPORT KL::Size kluGetString(
  const KL::Size & name
){
  try
  {
    const GLubyte* result = gluGetString( (GLenum)name );
  return (KL::Size)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluGetString( %d )", (int)name);
  }
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
  try
  {
    gluLookAt( (GLdouble)eyeX, (GLdouble)eyeY, (GLdouble)eyeZ, (GLdouble)centerX, (GLdouble)centerY, (GLdouble)centerZ, (GLdouble)upX, (GLdouble)upY, (GLdouble)upZ );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluLookAt( %f, %f, %f, %f, %f, %f, %f, %f, %f )", (float)eyeX, (float)eyeY, (float)eyeZ, (float)centerX, (float)centerY, (float)centerZ, (float)upX, (float)upY, (float)upZ);
  }
}

FABRIC_EXT_EXPORT void kluOrtho2D(
  const KL::Scalar & left,
  const KL::Scalar & right,
  const KL::Scalar & bottom,
  const KL::Scalar & top
){
  try
  {
    gluOrtho2D( (GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluOrtho2D( %f, %f, %f, %f )", (float)left, (float)right, (float)bottom, (float)top);
  }
}

FABRIC_EXT_EXPORT void kluPerspective(
  const KL::Scalar & fovy,
  const KL::Scalar & aspect,
  const KL::Scalar & zNear,
  const KL::Scalar & zFar
){
  try
  {
    gluPerspective( (GLdouble)fovy, (GLdouble)aspect, (GLdouble)zNear, (GLdouble)zFar );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluPerspective( %f, %f, %f, %f )", (float)fovy, (float)aspect, (float)zNear, (float)zFar);
  }
}

FABRIC_EXT_EXPORT void kluPickMatrix(
  const KL::Scalar & x,
  const KL::Scalar & y,
  const KL::Scalar & delX,
  const KL::Scalar & delY,
  KL::VariableArray<KL::Integer> & viewport
){
  try
  {
    gluPickMatrix( (GLdouble)x, (GLdouble)y, (GLdouble)delX, (GLdouble)delY, (GLint*)&viewport[0] );
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluPickMatrix( %f, %f, %f, %f, GLint* )", (float)x, (float)y, (float)delX, (float)delY);
  }
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
  try
  {
    GLint result = gluProject( (GLdouble)objX, (GLdouble)objY, (GLdouble)objZ, (const GLdouble*)&model[0], (const GLdouble*)&proj[0], (const GLint*)&view[0], (GLdouble*)&winX[0], (GLdouble*)&winY[0], (GLdouble*)&winZ[0] );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluProject( %f, %f, %f, GLdouble*, GLdouble*, GLint*, GLdouble*, GLdouble*, GLdouble* )", (float)objX, (float)objY, (float)objZ);
  }
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
  try
  {
    GLint result = gluScaleImage( (GLenum)format, (GLsizei)wIn, (GLsizei)hIn, (GLenum)typeIn, dataIn, (GLsizei)wOut, (GLsizei)hOut, (GLenum)typeOut, dataOut );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluScaleImage( %d, %d, %d, %d, void*, %d, %d, %d, GLvoid* )", (int)format, (int)wIn, (int)hIn, (int)typeIn, (int)wOut, (int)hOut, (int)typeOut);
  }
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
  try
  {
    GLint result = gluUnProject( (GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, (const GLdouble*)&model[0], (const GLdouble*)&proj[0], (const GLint*)&view[0], (GLdouble*)&objX[0], (GLdouble*)&objY[0], (GLdouble*)&objZ[0] );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluUnProject( %f, %f, %f, GLdouble*, GLdouble*, GLint*, GLdouble*, GLdouble*, GLdouble* )", (float)winX, (float)winY, (float)winZ);
  }
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
  try
  {
    GLint result = gluUnProject4( (GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, (GLdouble)clipW, (const GLdouble*)&model[0], (const GLdouble*)&proj[0], (const GLint*)&view[0], (GLdouble)nearVal, (GLdouble)farVal, (GLdouble*)&objX[0], (GLdouble*)&objY[0], (GLdouble*)&objZ[0], (GLdouble*)&objW[0] );
  return (KL::Integer)result;
  }
  catch(Exception e)
  {
    throw Fabric::Exception( "Fabric::OGL::gluUnProject4( %f, %f, %f, %f, GLdouble*, GLdouble*, GLint*, %f, %f, GLdouble*, GLdouble*, GLdouble*, GLdouble* )", (float)winX, (float)winY, (float)winZ, (float)clipW, (float)nearVal, (float)farVal);
  }
}

