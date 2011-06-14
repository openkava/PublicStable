/*
 *
 *  Created by Peter Zion on 2010-08-03.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#include "OGL.h"
#include "Debug.h"
#include <Fabric/Core/CG/IntegerAdapter.h>
#include <Fabric/Core/CG/BooleanAdapter.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/Scope.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Core/RT/BooleanDesc.h>
#include <Fabric/Core/RT/ByteDesc.h>
#include <Fabric/Core/RT/IntegerDesc.h>
#include <Fabric/Core/RT/ScalarDesc.h>
#include <Fabric/Core/RT/StringDesc.h>
#include <Fabric/Core/RT/StructDesc.h>
#include <Fabric/Core/RT/VariableArrayDesc.h>
#include <Fabric/Core/RT/StructMemberInfo.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/Util/Log.h>
#include <Fabric/Core/Util/UnorderedMap.h>
#include <stdio.h>

namespace Fabric
{
  
  
  namespace OGL
  {
    static RC::Handle<RT::Manager> rtManager;

    typedef Util::UnorderedMap< std::string, void * > FuncTable;
    static FuncTable  llvmFuncTable;
    
    static RC::ConstHandle<RT::StringDesc> stringDesc;

    class ErrorReporter
    {
    public:
    
      ErrorReporter( char const *desc )
        : m_desc( desc )
      {
        //FABRIC_OGL_TRACE( "ErrorReporter::ErrorReporter( '%s' )", desc );
#if defined(FABRIC_OS_LINUX) || defined(FABRIC_OS_WINDOWS)
        if( !s_glewInitialized )
        {
          GLenum err = glewInit();
          FABRIC_ASSERT( err == GLEW_OK );
          //FABRIC_OGL_TRACE_NOTE( "OpenGL 2.0 is %s", GLEW_VERSION_2_0? "supported": "NOT SUPPORTED" );
          s_glewInitialized = true;
        }
#endif
        
        if ( s_bracketCount == 0 )
          glGetError(); // Throw away last error
      }
      
      ~ErrorReporter()
      {
        //FABRIC_OGL_TRACE( "ErrorReporter::~ErrorReporter()" );
        if ( s_bracketCount == 0 )
        {
          GLenum error = glGetError();
          if ( error != GL_NO_ERROR )
          {
            const char *errorStr;
            switch ( error )
            {
              case GL_INVALID_ENUM: errorStr = "GL_INVALID_ENUM"; break;
              case GL_INVALID_VALUE: errorStr = "GL_INVALID_VALUE"; break;
              case GL_INVALID_OPERATION: errorStr = "GL_INVALID_OPERATION"; break;
              case GL_STACK_OVERFLOW: errorStr = "GL_STACK_OVERFLOW"; break;
              case GL_STACK_UNDERFLOW: errorStr = "GL_STACK_UNDERFLOW"; break;
              case GL_OUT_OF_MEMORY: errorStr = "GL_OUT_OF_MEMORY"; break;
              case GL_TABLE_TOO_LARGE: errorStr = "GL_TABLE_TOO_LARGE"; break;
              default: errorStr = "(unknown error)"; break;
            }
#if defined( FABRIC_OS_WINDOWS )
            ::OutputDebugStringA( "*** The OpenGL error '" );
            ::OutputDebugStringA( errorStr );
            ::OutputDebugStringA( "' (0x%04X) occurred calling " );
            ::OutputDebugStringA( m_desc );
            ::OutputDebugStringA( "()\n" );
#else
            FABRIC_DEBUG_LOG( "The OpenGL error '%s' (0x%04X) occurred calling %s()", errorStr, (unsigned)error, m_desc );
#endif
          }
        }
      }
    
      void incBracket()
      {
        ++s_bracketCount;
      }
      
      void decBracket()
      {
        --s_bracketCount;
      }
    
    private:
    
      char const *m_desc;
      static size_t s_bracketCount;
#if defined(FABRIC_OS_LINUX) || defined(FABRIC_OS_WINDOWS)
      static bool s_glewInitialized;
#endif
    };
    size_t ErrorReporter::s_bracketCount = 0;

#if defined(FABRIC_OS_LINUX) || defined(FABRIC_OS_WINDOWS)
    bool ErrorReporter::s_glewInitialized = false;
#endif
    
    static void ActiveTexture( GLenum texture )
    {
      FABRIC_OGL_TRACE( "ActiveTexture( 0x%04X )", (unsigned)texture );
      ErrorReporter errorReporter( "glActiveTexture" );
      glActiveTexture( texture );
    }

    static void AttachShader (GLuint program, GLuint shader)
    {
      FABRIC_OGL_TRACE( "AttachShader( %u, %u )", (unsigned)program, (unsigned)shader );
      ErrorReporter errorReporter( "glAttachShader" );
      glAttachShader( program, shader );
    }
    
    static void Begin( GLenum mode )
    {
      FABRIC_OGL_TRACE( "Begin( 0x%04X )", (unsigned)mode );
      ErrorReporter errorReporter( "glBegin" );
      glBegin( mode );
      errorReporter.incBracket();
    }
    
    static void BindBuffer( GLenum target, GLuint buffer )
    {
      FABRIC_OGL_TRACE( "BindBuffer( 0x%04X, %u )", (unsigned)target, (unsigned)buffer );
      ErrorReporter errorReporter( "glBindBuffer" );
      glBindBuffer( target, buffer );
    }
    
    static void BindTexture( GLenum target, GLuint texture )
    {
      FABRIC_OGL_TRACE( "BindTexture( 0x%04X, %u )", (unsigned)target, (unsigned)texture );
      ErrorReporter errorReporter( "glBindTexture" );
      glBindTexture( target, texture );
    }
    
    static void BlendFunc( GLenum sfactor, GLenum dfactor )
    {
      FABRIC_OGL_TRACE( "BlendFunc( %d, %d )", (int)sfactor, (int)dfactor );
      ErrorReporter errorReporter( "glBlendFunc" );
      glBlendFunc( sfactor, dfactor );
    }

    static void BufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
    {
      FABRIC_OGL_TRACE( "BufferData( 0x%04X, %u, %p, 0x%04X )", (unsigned)target, (unsigned)size, data, (unsigned)usage );
      ErrorReporter errorReporter( "glBufferData" );
      glBufferData( target, size, data, usage );
    }
    
    static void BufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data)
    {
      FABRIC_OGL_TRACE( "BufferSubData( 0x%04X, %d, %u, %p )", (unsigned)target, (int)offset, (unsigned)size, data );
      ErrorReporter errorReporter( "glBufferSubData" );
      glBufferSubData( target, offset, size, data );
    }
    
    static void Clear( GLbitfield mask )
    {
      FABRIC_OGL_TRACE( "Clear( 0x%04X )", (unsigned)mask );
      ErrorReporter errorReporter( "glClear" );
      glClear( mask );
    }

    static void ClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
    {
      FABRIC_OGL_TRACE( "ClearColor( %f, %f, %f, %f )", (float)red, (float)green, (float)blue, (float)alpha );
      ErrorReporter errorReporter( "glClearColor" );
      glClearColor( red, green, blue, alpha );
    }
    
    static void Color3f( GLfloat red, GLfloat green, GLfloat blue )
    {
      FABRIC_OGL_TRACE( "Color3f( %g, %g, %g )", (float)red, (float)green, (float)blue );
      ErrorReporter errorReporter( "glColor3f" );
      glColor3f( red, green, blue );
    }
    
    static void Color4f( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha )
    {
      FABRIC_OGL_TRACE( "Color3f( %g, %g, %g, %g )", (float)red, (float)green, (float)blue, (float)alpha );
      ErrorReporter errorReporter( "glColor4f" );
      glColor4f( red, green, blue, alpha );
    }

    static void ColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha )
    {
      FABRIC_OGL_TRACE( "ColorMask( %g, %g, %g, %g )", (bool)red, (bool)green, (bool)blue, (bool)alpha );
      ErrorReporter errorReporter( "glColorMask" );
      glColorMask( red, green, blue, alpha );
    }

    static void CompileShader (GLuint shader)
    {
      FABRIC_OGL_TRACE( "CompileShader( %u )", (unsigned)shader );
      ErrorReporter errorReporter( "glCompileShader" );
      glCompileShader( shader );
    }
    
    static GLuint CreateProgram (void)
    {
      FABRIC_OGL_TRACE( "CreateProgram()" );
      ErrorReporter errorReporter( "glCreateProgram" );
      GLuint result = glCreateProgram();
      FABRIC_OGL_TRACE_NOTE( "Result is %u", (unsigned)result );
      return result;
    }
            
    static GLuint CreateShader (GLenum type)
    {
      FABRIC_OGL_TRACE( "CreateShader( 0x%04X )", (unsigned)type );
      ErrorReporter errorReporter( "glCreateShader" );
      GLuint result = glCreateShader( type );
      FABRIC_OGL_TRACE_NOTE( "Returning %u", (unsigned)result );
      return result;
    }
    
    static void CullFace( GLenum mode )
    {
      FABRIC_OGL_TRACE( "CullFace( %d )", (int)mode );
      ErrorReporter errorReporter( "glCullFace" );
      glCullFace( mode );
    }

    static void DeleteBuffers( GLsizei n, const GLuint *buffers )
    {
      FABRIC_OGL_TRACE( "DeleteBuffers( %u, %p )", (unsigned)n, buffers );
      ErrorReporter errorReporter( "glDeleteBuffers" );
      glDeleteBuffers( n, buffers );
    }
    
    static void DeleteProgram (GLuint program)
    {
      FABRIC_OGL_TRACE( "DeleteProgram( %u )", (unsigned)program );
      ErrorReporter errorReporter( "glDeleteProgram" );
      glDeleteProgram( program );
    }
    
    static void DeleteShader (GLuint shader)
    {
      FABRIC_OGL_TRACE( "DeleteShader( %u )", (unsigned)shader );
      ErrorReporter errorReporter( "glDeleteShader" );
      glDeleteShader( shader );
    }
    
    static void DeleteTextures( GLsizei n, const GLuint *textures )
    {
      FABRIC_OGL_TRACE( "DeleteTextures( %u, %p )", (unsigned)n, textures );
      ErrorReporter errorReporter( "glDeleteTextures" );
      glDeleteTextures( n, textures );
    }

    static void DepthMask( GLboolean flag )
    {
      FABRIC_OGL_TRACE( "DepthMask( %d )", (int)flag );
      ErrorReporter errorReporter( "glDepthMask" );
      glDepthMask( flag );
    }

    static void DepthFunc( GLenum func )
    {
      FABRIC_OGL_TRACE( "DepthFunc( %d )", (int)func );
      ErrorReporter errorReporter( "glDepthFunc" );
      glDepthFunc( func );
    }
    
    static void DetachShader (GLuint program, GLuint shader)
    {
      FABRIC_OGL_TRACE( "DetachShader( %u, %u )", (unsigned)program, (unsigned)shader );
      ErrorReporter errorReporter( "glDetachShader" );
      glDetachShader( program, shader );
    }
    
    static void Disable( GLenum cap )
    {
      FABRIC_OGL_TRACE( "Disable( 0x%04X )", (unsigned)cap );
      ErrorReporter errorReporter( "glDisable" );
      glDisable( cap );
    }
    
    static void DisableClientState(GLenum array)
    {
      FABRIC_OGL_TRACE( "DisableClientState( %0x04X )", (unsigned)array );
      ErrorReporter errorReporter( "glDisableClientState" );
      glDisableClientState( array );
    }
    
    static void ReadBuffer (GLenum mode)
    {
      FABRIC_OGL_TRACE( "ReadBuffer( 0x%04X )", (unsigned)mode );
      ErrorReporter errorReporter( "glReadBuffer" );
      glReadBuffer( mode );
    }
    
    static void DrawBuffer (GLenum mode)
    {
      FABRIC_OGL_TRACE( "DrawBuffer( 0x%04X )", (unsigned)mode );
      ErrorReporter errorReporter( "glDrawBuffer" );
      glDrawBuffer( mode );
    }
    
    static void DrawArrays (GLenum mode, GLsizei first, GLsizei count)
    {
      FABRIC_OGL_TRACE( "DrawArrays( 0x%04X, %u, %u, %u )", (unsigned)mode, (unsigned)first, (unsigned)count );
      ErrorReporter errorReporter( "glDrawArrays" );
      glDrawArrays( mode, first, count );
    }
    
    static void DrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
    {
      FABRIC_OGL_TRACE( "DrawElements( 0x%04X, %u, 0x%04X, %p )", (unsigned)mode, (unsigned)count, (unsigned)type, indices );
      ErrorReporter errorReporter( "glDrawElements" );
      glDrawElements( mode, count, type, indices );
    }
    
    static void Enable( GLenum cap )
    {
      FABRIC_OGL_TRACE( "Enable( 0x%04X )", (unsigned)cap );
      ErrorReporter errorReporter( "glEnable" );
      glEnable( cap );
    }
    
    static void EnableClientState(GLenum array)
    {
      FABRIC_OGL_TRACE( "EnableClientState( %0x04X )", (unsigned)array );
      ErrorReporter errorReporter( "glEnableClientState" );
      glEnableClientState( array );
    }
    
    static void EnableVertexAttribArray (GLuint index)
    {
      FABRIC_OGL_TRACE( "EnableVertexAttribArray( %u )", (unsigned)index );
      ErrorReporter errorReporter( "glEnableVertexAttribArray" );
      glEnableVertexAttribArray( index );
    }
    
    static void End()
    {
      FABRIC_OGL_TRACE( "End()" );
      ErrorReporter errorReporter( "glEnd" );
      errorReporter.decBracket();
      glEnd();
    }
      
    static void Finish()
    {
      FABRIC_OGL_TRACE( "Finish()" );
      ErrorReporter errorReporter( "glFinish" );
      glFinish();
    }

    static void Flush()
    {
      FABRIC_OGL_TRACE( "Flush()" );
      ErrorReporter errorReporter( "glFlush" );
      glFlush();
    }

    static void GenBuffers( GLsizei n, GLuint *buffers )
    {
      FABRIC_OGL_TRACE( "GenBuffers( %u, %p )", (unsigned)n, buffers );
      ErrorReporter errorReporter( "glGenBuffers" );
      glGenBuffers( n, buffers );
    }
    
    
    
    static void GenTextures( GLsizei n, GLuint *textures )
    {
      FABRIC_OGL_TRACE( "GenTextures( %u, %p )", (unsigned)n, textures );
      ErrorReporter errorReporter( "glGenTextures" );
      glGenTextures( n, textures );
    }
    
    static void GetActiveAttrib( GLuint program, GLuint index, GLint *size, GLenum *type, void *nameData )
    {
      FABRIC_OGL_TRACE( "GetActiveAttrib( )" );
      ErrorReporter errorReporter( "glGetActiveAttrib" );
      
      GLint maxLength;
      glGetProgramiv( program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength );
      char *name = new char[ maxLength ];
      GLsizei   nbWritten = 0;
      glGetActiveAttrib(program, index, maxLength, &nbWritten, size, type, name );
      stringDesc->setValue( name, nbWritten, nameData );
    }
    
    static void GetActiveUniform( GLuint program, GLuint index, GLint *size, GLenum *type, void *nameData )
    {
      FABRIC_OGL_TRACE( "GetActiveUniform( )" );
      ErrorReporter errorReporter( "glGetActiveUniform" );
      
      GLint maxLength;
      glGetProgramiv( program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength );
      char *name = new char[ maxLength ];
      GLsizei   nbWritten = 0;
      glGetActiveUniform(program, index, maxLength, &nbWritten, size, type, name );
      stringDesc->setValue( name, nbWritten, nameData );
    }

    static GLint GetAttribLocation( GLuint program, const void *nameData )
    {
      FABRIC_OGL_TRACE( "GetAttribLocation( %u, %p )", (unsigned)program, nameData );
      ErrorReporter errorReporter( "glGetAttribLocation" );
      char const *name = stringDesc->getValueData( &nameData );
      GLint result = glGetAttribLocation( program, name );
      return result;
    }
    
    static void GetIntegerv(GLenum pname, GLint *params)
    {
      FABRIC_OGL_TRACE( "GetIntegerv( 0x%04X, %p )", (unsigned)pname, params );
      ErrorReporter errorReporter( "glGetIntegerv" );
      glGetIntegerv( pname, params );
    }    
    
    static void GetProgramiv (GLuint program, GLenum pname, GLint *params)
    {
      FABRIC_OGL_TRACE( "GetProgramiv( %u, 0x%04X, %p )", (unsigned)program, (unsigned)pname, params );
      ErrorReporter errorReporter( "glGetProgramiv" );
      glGetProgramiv( program, pname, params );
    }

    static void GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, void *infoLogData)
    {
      FABRIC_OGL_TRACE( "GetProgramInfoLog( %u, %u, %p, %p )", (unsigned)program, (unsigned)bufSize, length, infoLogData );
      ErrorReporter errorReporter( "glGetProgramInfoLog" );

      GLint infoLen = 0;
      glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLen );

      char *infoLog = new char[ infoLen + 1 ];

      GLsizei   resultSize = 0;

      glGetProgramInfoLog( program, bufSize, &resultSize, infoLog );
      stringDesc->setValue( infoLog, resultSize, infoLogData );

      if( length )
        *length = resultSize;
    }

    static void GetShaderiv (GLuint shader, GLenum pname, GLint *params)
    {
      FABRIC_OGL_TRACE( "GetShaderiv( %u, 0x%04X, %p )", (unsigned)shader, (unsigned)pname, params );
      ErrorReporter errorReporter( "glGetShaderiv" );
      glGetShaderiv( shader, pname, params );
    }
    
    static void GetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, void *infoLogData)
    {
      FABRIC_OGL_TRACE( "GetShaderInfoLog( %u, %u, %p, %p )", (unsigned)shader, (unsigned)bufSize, length, infoLogData );
      ErrorReporter errorReporter( "glGetShaderInfoLog" );

      GLint infoLen = 0;
      glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infoLen );

      char *infoLog = new char[ infoLen + 1 ];

      GLsizei   resultSize = 0;

      glGetShaderInfoLog( shader, bufSize, &resultSize, infoLog );
      stringDesc->setValue( infoLog, resultSize, infoLogData );

      if( length )
        *length = resultSize;
    }
    
    static void GetTexLevelParameterfv( GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
      FABRIC_OGL_TRACE( "GetTexLevelParameterfv( 0x%04X, %d, 0x%04X, %p )", (unsigned)target, level, (unsigned)pname, params );
      ErrorReporter errorReporter( "glGetTexLevelParameterfv" );
      glGetTexLevelParameterfv( target, level, pname, params );
    }
    
    static void GetTexLevelParameteriv( GLenum target, GLint level, GLenum pname, GLint *params)
    {
      FABRIC_OGL_TRACE( "GetTexLevelParameteriv( 0x%04X, %d, 0x%04X, %p )", (unsigned)target, level, (unsigned)pname, params );
      ErrorReporter errorReporter( "glGetTexLevelParameteriv" );
      glGetTexLevelParameteriv( target, level, pname, params );
    }
    
    static void GetTexParameterfv( GLenum target, GLenum pname, GLfloat *params)
    {
      FABRIC_OGL_TRACE( "GetTexParameterfv( 0x%04X, 0x%04X, %p )", (unsigned)target, (unsigned)pname, params );
      ErrorReporter errorReporter( "glGetTexParameterfv" );
      glGetTexParameterfv( target, pname, params );
    }
    
    static void GetTexParameteriv( GLenum target, GLenum pname, GLint *params)
    {
      FABRIC_OGL_TRACE( "GetTexParameteriv( 0x%04X, 0x%04X, %p )", (unsigned)target, (unsigned)pname, params );
      ErrorReporter errorReporter( "glGetTexParameteriv" );
      glGetTexParameteriv( target, pname, params );
    }
    

    static GLint GetUniformLocation( GLuint program, const void *nameData )
    {
      FABRIC_OGL_TRACE( "GetUniformLocation( %u, %p )", (unsigned)program, nameData );
      ErrorReporter errorReporter( "glGetUniformLocation" );
      char const *name = stringDesc->getValueData( &nameData );
      GLint result = glGetUniformLocation( program, name );
      return result;
    }

    static void LineStipple( GLint factor, GLuint /* GLushort */ pattern )
    {
      FABRIC_OGL_TRACE( "LineStipple( %d, %04X )", factor, pattern );
      ErrorReporter errorReporter( "glLineStipple" );
      glLineStipple( factor, (GLushort)pattern );
    }
    
    static void LineWidth( GLfloat width )
    {
      FABRIC_OGL_TRACE( "LineWidth( %g )", (float)width );
      ErrorReporter errorReporter( "glLineWidth" );
      glLineWidth( width );
    }
    
    static void LinkProgram (GLuint program)
    {
      FABRIC_OGL_TRACE( "LinkProgram( %u )", (unsigned)program );
      ErrorReporter errorReporter( "glLinkProgram" );
      glLinkProgram( program );
    }
    
    static void LoadMatrix( const GLfloat *m )
    {
      FABRIC_OGL_TRACE( "LoadMatrix( %p )", m );
      ErrorReporter errorReporter( "glLoadMatrix" );
      glLoadMatrixf( m );
    };
    
    static void MatrixMode( GLenum mode)
    {
      FABRIC_OGL_TRACE( "LinkProgram( 0x%04X )", (unsigned)mode );
      ErrorReporter errorReporter( "glMatrixMode" );
      glMatrixMode( mode );
    }
    
    static void PixelStorei( GLenum pname, GLint param )
    {
      FABRIC_OGL_TRACE( "PixelStorei( 0x%04X, %d )", (unsigned)pname, (int)param );
      ErrorReporter errorReporter( "glPixelStorei" );
      glPixelStorei( pname, param );
    }

    static void PointParameterf( GLenum pname, GLfloat param )
    {
      FABRIC_OGL_TRACE( "PointParameterf( %04X, %g )", (unsigned)pname, (float)param );
      ErrorReporter errorReporter( "glPointParameterf" );
      glPointParameterf( pname, param );
    }

    static void PointParameteri( GLenum pname, GLint param )
    {
      FABRIC_OGL_TRACE( "PointParameteri( %04X, %d )", (unsigned)pname, (int)param );
      ErrorReporter errorReporter( "glPointParameteri" );
      glPointParameteri( pname, param );
    }

    static void PointParameterfv( GLenum pname, GLfloat *params )
    {
      FABRIC_OGL_TRACE( "PointParameterfv( %04X, %p )", (unsigned)pname, params );
      ErrorReporter errorReporter( "glPointParameterfv" );
      glPointParameterfv( pname, params );
    }

    static void PointParameteriv( GLenum pname, GLint *params )
    {
      FABRIC_OGL_TRACE( "PointParameteriv( %04X, %p )", (unsigned)pname, params );
      ErrorReporter errorReporter( "glPointParameteriv" );
      glPointParameteriv( pname, params );
    }
    
    static void PointSize( GLfloat size )
    {
      FABRIC_OGL_TRACE( "PointSize( %g )", (float)size );
      ErrorReporter errorReporter( "glPointSize" );
      glPointSize( size );
    }
    
    static void PolygonMode( GLenum face, GLenum mode )
    {
      FABRIC_OGL_TRACE( "PolygonMode( %d, %d )", (int)face, (int)mode );
      ErrorReporter errorReporter( "glPolygonMode" );
      glPolygonMode( face, mode );
    }
    
    static void PopAttrib()
    {
      FABRIC_OGL_TRACE( "PopAttrib()" );
      ErrorReporter errorReporter( "glPopAttrib" );
      glPopAttrib();
    }
    
    static void PushAttrib( GLbitfield mask )
    {
      FABRIC_OGL_TRACE( "PushAttrib( 0x%04X )", (unsigned)mask );
      ErrorReporter errorReporter( "glPushAttrib" );
      glPushAttrib( mask );
    }

    static void ShaderSource (GLuint shader, GLsizei count, void *stringData, const GLint *length)
    {
      FABRIC_OGL_TRACE( "ShaderSource( %u, %u, %p, %p )", (unsigned)shader, (unsigned)count, stringData, length );
      ErrorReporter errorReporter( "glShaderSource" );

      // TODO: Use a string array instead.
      char const *string = stringDesc->getValueData( &stringData );

      glShaderSource( shader, 1, &string, NULL );
    }
    
    static void TexEnvf( GLenum target, GLenum pname, GLfloat param )
    {
      FABRIC_OGL_TRACE( "TexEnvf( %04X, %04X, %g )", (unsigned)target, (unsigned)pname, (float)param );
      ErrorReporter errorReporter( "glTexEnvf" );
      glTexEnvf( target, pname, param );
    }

    static void TexEnvi( GLenum target, GLenum pname, GLint param )
    {
      FABRIC_OGL_TRACE( "TexEnvi( %04X, %04X, %d )", (unsigned)target, (unsigned)pname, (int)param );
      ErrorReporter errorReporter( "glTexEnvi" );
      glTexEnvi( target, pname, param );
    }

    static void TexEnvfv( GLenum target, GLenum pname, const GLfloat *params )
    {
      FABRIC_OGL_TRACE( "TexEnvfv( %04X, %04X, %p )", (unsigned)target, (unsigned)pname, params );
      ErrorReporter errorReporter( "glTexEnvfv" );
      glTexEnvfv( target, pname, params );
    }

    static void TexEnviv( GLenum target, GLenum pname, const GLint *params )
    {
      FABRIC_OGL_TRACE( "TexEnviv( %04X, %04X, %p )", (unsigned)target, (unsigned)pname, params );
      ErrorReporter errorReporter( "glTexEnviv" );
      glTexEnviv( target, pname, params );
    }

    static void TexCoord2f( GLfloat s, GLfloat t )
    {
      FABRIC_OGL_TRACE( "TexCoord2f( %f, %f )", (float)s, (float)t );
      ErrorReporter errorReporter( "glTexCoord2f" );
      glTexCoord2f( s, t );
    }
    
    static void TexCoord3f( GLfloat s, GLfloat t, GLfloat r )
    {
      FABRIC_OGL_TRACE( "TexCoord3f( %f, %f, %f )", (float)s, (float)t, (float)r );
      ErrorReporter errorReporter( "glTexCoord3f" );
      glTexCoord3f( s, t, r );
    }
    
    static void TexImage1D(
      GLenum  	target,
      GLint  	level,
      GLint  	internalFormat,
      GLsizei  	width,
      GLint  	border,
      GLenum  	format,
      GLenum  	type,
      const GLvoid *pixels
      )
    {
      FABRIC_OGL_TRACE( "TexImage1D( 0x%04X, %d, %d, %u, %d, 0x%04X, 0x%04X, %p )",
                       (int)target,
                       (int)level,
                       (int)internalFormat,
                       (unsigned)width,
                       (int)border,
                       (unsigned)format,
                       (unsigned)type,
                       pixels
                       );
      ErrorReporter errorReporter( "glTexImage1D" );
      glTexImage1D( target, level, internalFormat, width, border, format, type, pixels );
    }
    
    static void TexImage2D(
      GLenum  	target,
      GLint  	level,
      GLint  	internalFormat,
      GLsizei  	width,
      GLsizei  	height,
      GLint  	border,
      GLenum  	format,
      GLenum  	type,
      const GLvoid *pixels
      )
    {
      FABRIC_OGL_TRACE( "TexImage2D( 0x%04X, %d, %d, %u, %u, %d, 0x%04X, 0x%04X, %p )",
                       (int)target,
                       (int)level,
                       (int)internalFormat,
                       (unsigned)width,
                       (unsigned)height,
                       (int)border,
                       (unsigned)format,
                       (unsigned)type,
                       pixels
                       );
      ErrorReporter errorReporter( "glTexImage2D" );
      glTexImage2D( target, level, internalFormat, width, height, border, format, type, pixels );
    }
    
    static void TexParameteri(	GLenum target, GLenum pname, GLint param )
    {
      FABRIC_OGL_TRACE( "TexParameteri( 0x%04X, 0x%04X, %d )", (unsigned)target, (unsigned)pname, (int)param );
      ErrorReporter errorReporter( "glTexParameteri" );
      glTexParameteri( target, pname, param );
    }
    
    static void TexParameterf(	GLenum target, GLenum pname, GLfloat param )
    {
      FABRIC_OGL_TRACE( "TexParameterf( 0x%04X, 0x%04X, %f )", (unsigned)target, (unsigned)pname, (float)param );
      ErrorReporter errorReporter( "glTexParameterf" );
      glTexParameterf( target, pname, param );
    }

    static void TexParameterfv(	GLenum target, GLenum pname, GLfloat *param )
    {
      FABRIC_OGL_TRACE( "TexParameterfv( 0x%04X, 0x%04X, %p )", (unsigned)target, (unsigned)pname, param );
      ErrorReporter errorReporter( "glTexParameterfv" );
      glTexParameterfv( target, pname, param );
    }

    static void Uniform1f( GLint location, GLfloat v0 )
    {
      FABRIC_OGL_TRACE( "Uniform1f( %d, %f )", (int)location, (float)v0 );
      ErrorReporter errorReporter( "glUniform1f" );
      glUniform1f( location, v0 );
    }
    
    static void Uniform1i( GLint location, GLint v0 )
    {
      FABRIC_OGL_TRACE( "Uniform1i( %d, %d )", (int)location, (int)v0 );
      ErrorReporter errorReporter( "glUniform1i" );
      glUniform1i( location, v0 );
    }
    
    static void Uniform2f( GLint location, GLfloat v0, GLfloat v1 )
    {
      FABRIC_OGL_TRACE( "Uniform2f( %d, %f, %f )", (int)location, (float)v0, (float)v1 );
      ErrorReporter errorReporter( "glUniform2f" );
      glUniform2f( location, v0, v1 );
    }

    static void Uniform3f( GLint location, GLfloat v0, GLfloat v1, GLfloat v2 )
    {
      FABRIC_OGL_TRACE( "Uniform3f( %d, %f, %f, %f )", (int)location, (float)v0, (float)v1, (float)v2 );
      ErrorReporter errorReporter( "glUniform3f" );
      glUniform3f( location, v0, v1, v2 );
    }

    static void Uniform4f( GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 )
    {
      FABRIC_OGL_TRACE( "Uniform4f( %d, %f, %f, %f, %f )", (int)location, (float)v0, (float)v1, (float)v2, (float)v3 );
      ErrorReporter errorReporter( "glUniform4f" );
      glUniform4f( location, v0, v1, v2, v3 );
    }
    
    static void UniformMatrix2fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )
    {
      FABRIC_OGL_TRACE( "UniformMatrix2fv( %d, %u, %s, %p )", (int)location, (unsigned)count, transpose?"true":"false", value );
      ErrorReporter errorReporter( "glUniformMatrix2fv" );
      glUniformMatrix2fv( location, count, transpose, value );
    }

    static void UniformMatrix3fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )
    {
      FABRIC_OGL_TRACE( "UniformMatrix3fv( %d, %u, %s, %p )", (int)location, (unsigned)count, transpose?"true":"false", value );
      ErrorReporter errorReporter( "glUniformMatrix3fv" );
      glUniformMatrix3fv( location, count, transpose, value );
    }

    static void UniformMatrix4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )
    {
      FABRIC_OGL_TRACE( "UniformMatrix4fv( %d, %u, %s, %p )", (int)location, (unsigned)count, transpose?"true":"false", value );
      ErrorReporter errorReporter( "glUniformMatrix4fv" );
      glUniformMatrix4fv( location, count, transpose, value );
    }
        
    static void UseProgram (GLuint program)
    {
      FABRIC_OGL_TRACE( "UseProgram( %u )", (unsigned)program );
      ErrorReporter errorReporter( "glUseProgram" );
      glUseProgram( program );
    }

    static void Vertex3f( GLfloat x, GLfloat y, GLfloat z )
    {
      FABRIC_OGL_TRACE( "Vertex3f( %f, %f, %f )", (float)x, (float)y, (float)z );
      ErrorReporter errorReporter( "glVertex3f" );
      glVertex3f( x, y, z );
    }

    static void VertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
    {
      FABRIC_OGL_TRACE( "VertexAttribPointer( %u, %d, 0x%04X, %s, %u, %p )", (unsigned)index, (int)size, (unsigned)type, normalized? "true": "false", (unsigned)stride, pointer );
      ErrorReporter errorReporter( "glVertexAttribPointer" );
      glVertexAttribPointer( index, size, type, normalized, stride, pointer );
    }
    
    static void VertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      FABRIC_OGL_TRACE( "VertexPointer( %d, 0x%04X, %u, %p )", (int)size, (unsigned)type, (unsigned)stride, pointer );
      ErrorReporter errorReporter( "glVertexPointer" );
      glVertexPointer( size, type, stride, pointer );
    }
    
    static void Viewport( GLint x, GLint y, GLsizei width, GLsizei height )
    {
      FABRIC_OGL_TRACE( "ViewPort( %d, %d, %u, %u )", (int)x, (int)y, (unsigned)width, (unsigned)height );
      ErrorReporter errorReporter( "glViewport" );
      glViewport( x, y, width, height );
    }
    
    // === GL_EXT_framebuffer_object ===
    static void BindFramebufferEXT( GLenum target, GLuint framebuffer )
    {
      FABRIC_OGL_TRACE( "BindFramebufferEXT( 0x%04X, %u )", (unsigned)target, (unsigned)framebuffer );
      ErrorReporter errorReporter( "glBindFramebufferEXT" );
      glBindFramebufferEXT( target, framebuffer );
    }
    
    static void BindRenderbufferEXT( GLenum target, GLuint renderbuffer )
    {
      FABRIC_OGL_TRACE( "BindRenderbufferEXT( 0x%04X, %u )", (unsigned)target, (unsigned)renderbuffer );
      ErrorReporter errorReporter( "glBindRenderbufferEXT" );
      glBindRenderbufferEXT( target, renderbuffer );
    }
    
    static GLenum CheckFramebufferStatusEXT( GLenum target )
    {
      FABRIC_OGL_TRACE( "CheckFramebufferStatusEXT( 0x%04X )", (unsigned)target );
      ErrorReporter errorReporter( "glCheckFramebufferStatusEXT" );
      return glCheckFramebufferStatusEXT( target );
    }
    
    static void DeleteFramebuffersEXT( GLsizei n, const GLuint *framebuffers )
    {
      FABRIC_OGL_TRACE( "DeleteFramebuffersEXT( %d, %p )", n, framebuffers );
      ErrorReporter errorReporter( "glDeleteFramebuffersEXT" );
      glDeleteFramebuffersEXT( n, framebuffers );
    }
    
    static void DeleteRenderbuffersEXT( GLsizei n, const GLuint *renderbuffers )
    {
      FABRIC_OGL_TRACE( "DeleteRenderbuffersEXT( %d, %p )", n, renderbuffers );
      ErrorReporter errorReporter( "glDeleteRenderbuffersEXT" );
      glDeleteRenderbuffersEXT( n, renderbuffers );
    }
    
    static void FramebufferRenderbufferEXT( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
    {
      FABRIC_OGL_TRACE( "FramebufferRenderbufferEXT( 0x%04X, 0x%04X, 0x%04X, %u )", (unsigned)target, (unsigned)attachment, (unsigned)renderbuffertarget, (unsigned)renderbuffer );
      ErrorReporter errorReporter( "glFramebufferRenderbufferEXT" );
      glFramebufferRenderbufferEXT( target, attachment, renderbuffertarget, renderbuffer );
    }
    
    static void FramebufferTexture1DEXT( GLenum target, GLenum attachment, GLenum textarget, GLenum texture, GLint level )
    {
      FABRIC_OGL_TRACE( "FramebufferTexture1DEXT( 0x%04X, 0x%04X, 0x%04X, 0x%04X, %d )", (unsigned)target, (unsigned)attachment, (unsigned)textarget, (unsigned)texture, level );
      ErrorReporter errorReporter( "glFramebufferTexture1DEXT" );
      glFramebufferTexture1DEXT( target, attachment, textarget, texture, level );
     }
    
    static void FramebufferTexture2DEXT( GLenum target, GLenum attachment, GLenum textarget, GLenum texture, GLint level )
    {
      FABRIC_OGL_TRACE( "FramebufferTexture2DEXT( 0x%04X, 0x%04X, 0x%04X, 0x%04X, %d )", (unsigned)target, (unsigned)attachment, (unsigned)textarget, (unsigned)texture, level );
      ErrorReporter errorReporter( "glFramebufferTexture2DEXT" );
      glFramebufferTexture2DEXT( target, attachment, textarget, texture, level );
    }
    
    static void FramebufferTexture3DEXT( GLenum target, GLenum attachment, GLenum textarget, GLenum texture, GLint level, GLint zoffset )
    {
      FABRIC_OGL_TRACE( "FramebufferTexture3DEXT( 0x%04X, 0x%04X, 0x%04X, 0x%04X, %d, %d )", (unsigned)target, (unsigned)attachment, (unsigned)textarget, (unsigned)texture, level, zoffset );
      ErrorReporter errorReporter( "glFramebufferTexture3DEXT" );
      glFramebufferTexture3DEXT( target, attachment, textarget, texture, level, zoffset );
    }
    
    static void GenFramebuffersEXT( GLsizei n, GLuint *framebuffers )
    {
      FABRIC_OGL_TRACE( "GenFramebuffersEXT( %d, %p )", n, framebuffers );
      ErrorReporter errorReporter( "glGenFramebuffersEXT" );
      glGenFramebuffersEXT( n, framebuffers );
    }
    
    static void GenRenderbuffersEXT( GLsizei n, GLuint *renderbuffers )
    {
      FABRIC_OGL_TRACE( "GenRenderbuffersEXT( %d, %p )", n, renderbuffers );
      ErrorReporter errorReporter( "glGenRenderbuffersEXT" );
      glGenRenderbuffersEXT( n, renderbuffers );
    }
    
    static void GenerateMipmapEXT( GLenum target )
    {
      FABRIC_OGL_TRACE( "GenerateMipmapEXT( 0x%04X )", (unsigned)target );
      ErrorReporter errorReporter( "glGenerateMipmapEXT" );
      glGenerateMipmapEXT( target );
    }
    
    static void GetFramebufferAttachmentParameterivEXT( GLenum target, GLenum attachment, GLenum pname, GLint *params )
    {
      FABRIC_OGL_TRACE( "GetFramebufferAttachmentParameterivEXT( 0x%04X, 0x%04X, 0x%04X, %p )", (unsigned)target, (unsigned)attachment, (unsigned)pname, params );
      ErrorReporter errorReporter( "glGetFramebufferAttachmentParameterivEXT" );
      glGetFramebufferAttachmentParameterivEXT( target, attachment, pname, params );
    }
    
    static void GetRenderbufferParameterivEXT( GLenum target, GLenum pname, GLint *params )
    {
      FABRIC_OGL_TRACE( "GetRenderbufferParameterivEXT( 0x%04X, 0x%04X, 0x%04X, %p )", (unsigned)target, (unsigned)pname, params );
      ErrorReporter errorReporter( "glGetRenderbufferParameterivEXT" );
      glGetRenderbufferParameterivEXT( target, pname, params );
    }
    
    static GLboolean IsFramebufferEXT( GLuint framebuffer )
    {
      FABRIC_OGL_TRACE( "IsFramebufferEXT( 0x%04X )", (unsigned)framebuffer );
      ErrorReporter errorReporter( "glIsFramebufferEXT" );
      return glIsFramebufferEXT( framebuffer );
    }
    
    static GLboolean IsRenderbufferEXT( GLuint renderbuffer )
    {
      FABRIC_OGL_TRACE( "IsRenderbufferEXT( 0x%04X )", (unsigned)renderbuffer );
      ErrorReporter errorReporter( "glIsRenderbufferEXT" );
      return glIsRenderbufferEXT( renderbuffer );
    }
    
    static void RenderbufferStorageEXT( GLenum target, GLenum internalformat, GLsizei width, GLsizei height )
    {
      FABRIC_OGL_TRACE( "RenderbufferStorageEXT( 0x%04X, 0x%04X, %d, %d )", (unsigned)target, (unsigned)internalformat, width, height );
      ErrorReporter errorReporter( "glRenderbufferStorageEXT" );
      glRenderbufferStorageEXT( target, internalformat, width, height );
    }

    // == GL_EXT_geometry_shader4 ==
    static void FramebufferTextureEXT( GLenum target, GLenum attachment, GLuint texture, GLint level )
    {
      FABRIC_OGL_TRACE( "FramebufferTextureEXT( 0x%04X, 0x%04X, %d, %d )", (unsigned)target, (unsigned)attachment, (unsigned)texture, level );
      ErrorReporter errorReporter( "glFramebufferTextureEXT" );
      glFramebufferTextureEXT( target, attachment, texture, level );
    }
    
    static void FramebufferTextureFaceEXT( GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face )
    {
      FABRIC_OGL_TRACE( "FramebufferTextureFaceEXT( 0x%04X, 0x%04X, %d, %d, 0x%04X )", (unsigned)target, (unsigned)attachment, (unsigned)texture, level, (unsigned)face );
      ErrorReporter errorReporter( "glFramebufferTextureFaceEXT" );
      glFramebufferTextureFaceEXT( target, attachment, texture, level, face );
    }
    
    static void FramebufferTextureLayerEXT( GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer )
    {
      FABRIC_OGL_TRACE( "FramebufferTextureLayerEXT( 0x%04X, 0x%04X, %d, %d, %d )", (unsigned)target, (unsigned)attachment, (unsigned)texture, level, (unsigned)layer );
      ErrorReporter errorReporter( "glFramebufferTextureLayerEXT" );
      glFramebufferTextureLayerEXT( target, attachment, texture, level, layer );
    }
    
    static void ProgramParameteriEXT( GLuint program, GLenum pname, GLint value )
    {
      FABRIC_OGL_TRACE( "ProgramParameteriEXT( %d, %04X, %d )", program, (unsigned)pname, value );
      ErrorReporter errorReporter( "glProgramParameteriEXT" );
      glProgramParameteriEXT( program, pname, value );
    }
    

#define ADD_CONST_BOOL( name, value ) \
  scope.put( name, CG::ConstantSymbol::Create( CG::ExprValue( booleanAdapter, CG::USAGE_RVALUE, booleanAdapter->llvmConst( value ) ) ) )
#define ADD_CONST_INT( name, value ) \
  scope.put( name, CG::ConstantSymbol::Create( CG::ExprValue( integerAdapter, CG::USAGE_RVALUE, integerAdapter->llvmConst( value ) ) ) )
#define ADD_FUNC_QUOTE( x ) #x
#define ADD_FUNC( name, paramLayout ) \
  CG::FunctionBuilder( moduleBuilder, ADD_FUNC_QUOTE(gl##name), paramLayout); \
  llvmFuncTable[ ADD_FUNC_QUOTE(gl##name) ] = (void *)&name;
    
    void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, RC::Handle<RT::Manager> const &rtManager )
    {
      FABRIC_OGL_TRACE( "llvmPrepareModule()" );
            
      stringDesc = rtManager->getStringDesc();

      llvmFuncTable.clear();
      
      CG::ModuleScope &scope = moduleBuilder.getScope();
      RC::ConstHandle<CG::BooleanAdapter> booleanAdapter = moduleBuilder.getManager()->getBooleanAdapter();
      RC::ConstHandle<CG::IntegerAdapter> integerAdapter = moduleBuilder.getManager()->getIntegerAdapter();
      
      ADD_FUNC( ActiveTexture, "<Integer texture" );
      ADD_FUNC( AttachShader, "<Integer program,<Integer shader" );
      ADD_FUNC( Begin, "<Integer mode" );
      ADD_FUNC( BindBuffer, "<Integer target,<Integer buffer" );
      ADD_FUNC( BindTexture, "<Integer target,<Integer texture" );
      ADD_FUNC( BlendFunc, "<Integer sfactor,<Integer dfactor" );
      ADD_FUNC( BufferData, "<Integer target,<Size size,<Data data,<Integer usage" );
      ADD_FUNC( BufferSubData, "<Integer target,<Size offset,<Size size,<Data data" );
      ADD_FUNC( Clear, "<Integer mask" );
      ADD_FUNC( ClearColor, "<Scalar red,<Scalar green,<Scalar blue,<Scalar alpha" );
      ADD_FUNC( Color3f, "<Scalar red,<Scalar green,<Scalar blue" );
      ADD_FUNC( Color4f, "<Scalar red,<Scalar green,<Scalar blue,<Scalar alpha" );
      ADD_FUNC( ColorMask, "<Boolean red,<Boolean green,<Boolean blue,<Boolean alpha" );
      ADD_FUNC( CompileShader, "<Integer shader" );
      ADD_FUNC( CreateProgram, "=Integer" );
      ADD_FUNC( CreateShader, "=Integer,<Integer enum" );
      ADD_FUNC( CullFace, "<Integer mode" );
      ADD_FUNC( DeleteBuffers, "<Integer n,<Data buffers" );
      ADD_FUNC( DeleteProgram, "<Integer program" );
      ADD_FUNC( DeleteShader, "<Integer shader" );
      ADD_FUNC( DeleteTextures, "<Integer n,<Data textures" );
      ADD_FUNC( DepthFunc, "<Integer func" );
      ADD_FUNC( DepthMask, "<Boolean flag" );
      ADD_FUNC( Disable, "<Integer cap" );
      ADD_FUNC( DisableClientState, "<Integer array" );
      ADD_FUNC( ReadBuffer, "<Integer mode" );
      ADD_FUNC( DrawBuffer, "<Integer mode" );
      ADD_FUNC( DrawArrays, "<Integer mode,<Integer first,<Integer count" );
      ADD_FUNC( DrawElements, "<Integer mode,<Integer count,<Integer type,<Data indices" );
      ADD_FUNC( Enable, "<Integer cap" );
      ADD_FUNC( EnableClientState, "<Integer array" );
      ADD_FUNC( EnableVertexAttribArray, "<Integer index" );
      ADD_FUNC( End, "" );
      ADD_FUNC( Finish, "" );
      ADD_FUNC( Flush, "" );
      ADD_FUNC( GenBuffers, "<Integer n,<Data buffers" );
      ADD_FUNC( GenTextures, "<Integer n,<Data textures" );
      ADD_FUNC( GetActiveAttrib, "<Integer program,<Integer index,>Integer size,>Integer type,>String name");
      ADD_FUNC( GetActiveUniform, "<Integer program,<Integer index,>Integer size,>Integer type,>String name");
      ADD_FUNC( GetAttribLocation, "=Integer,<Integer program,<String nameData" );
      ADD_FUNC( GetIntegerv, "<Integer param,<Data params" );
      ADD_FUNC( GetProgramInfoLog, "<Integer Shader,<Size bufSize,>Integer length,>String infoLog" );
      ADD_FUNC( GetProgramiv, "<Integer program,<Integer pname,>Integer params" );
      ADD_FUNC( GetShaderInfoLog, "<Integer Shader,<Size bufSize,>Integer length,>String infoLog" );
      ADD_FUNC( GetShaderiv, "<Integer shader,<Integer pname,>Integer params" );
      ADD_FUNC( GetTexLevelParameterfv, "<Integer target,<Integer level,<Integer pname,<Data params" );
      ADD_FUNC( GetTexLevelParameteriv, "<Integer target,<Integer level,<Integer pname,<Data params" );
      ADD_FUNC( GetTexParameterfv, "<Integer target,<Integer pname,<Data params" );
      ADD_FUNC( GetTexParameteriv, "<Integer target,<Integer pname,<Data params" );
      ADD_FUNC( GetUniformLocation, "=Integer,<Integer program,<String nameData" );
      ADD_FUNC( LineStipple, "<Integer factor,<Integer pattern" );
      ADD_FUNC( LineWidth, "<Scalar width" );
      ADD_FUNC( LinkProgram, "<Integer program" );
      ADD_FUNC( LoadMatrix, "<Data value" );
      ADD_FUNC( MatrixMode, "<Integer mode" );
      ADD_FUNC( PixelStorei, "<Integer pname,<Integer param" );
      ADD_FUNC( PointParameterf, "<Integer pname,<Scalar param" );
      ADD_FUNC( PointParameteri, "<Integer pname,<Integer param" );
      ADD_FUNC( PointParameterfv, "<Integer pname,<Data params" );
      ADD_FUNC( PointParameteriv, "<Integer pname,<Data params" );
      ADD_FUNC( PointSize, "<Scalar size" );
      ADD_FUNC( PolygonMode, "<Integer face,<Integer mode" );
      ADD_FUNC( PopAttrib, "" );
      ADD_FUNC( PushAttrib, "<Integer mask" );
      ADD_FUNC( ShaderSource, "<Integer shader,<Integer count,<String source,>Integer length" );
      ADD_FUNC( TexEnvf, "<Integer target,<Integer pnane,<Scalar param" );
      ADD_FUNC( TexEnvfv, "<Integer target,<Integer pnane,<Data params" );
      ADD_FUNC( TexEnvi, "<Integer target,<Integer pnane,<Integer param" );
      ADD_FUNC( TexEnviv, "<Integer target,<Integer pnane,<Data params" );
      ADD_FUNC( TexCoord2f, "<Scalar s,<Scalar t" );
      ADD_FUNC( TexImage1D, 
        "<Integer target,<Integer level,<Integer internalformat,"
        "<Integer width,<Integer border,"
        "<Integer format,<Integer type,<Data pixels" );
      ADD_FUNC( TexImage2D, 
        "<Integer target,<Integer level,<Integer internalformat,"
        "<Integer width,<Integer height,<Integer border,"
        "<Integer format,<Integer type,<Data pixels" );
      ADD_FUNC( TexParameteri, "<Integer target,<Integer pname,<Integer param" );
      ADD_FUNC( TexParameterf, "<Integer target,<Integer pname,<Scalar param" );
      ADD_FUNC( TexParameterfv, "<Integer target,<Integer pname,<Data param" );
      ADD_FUNC( Uniform1i, "<Integer location,<Integer v0" );
      ADD_FUNC( Uniform1f, "<Integer location,<Scalar v0" );
      ADD_FUNC( Uniform2f, "<Integer location,<Scalar v0,<Scalar v1" );
      ADD_FUNC( Uniform3f, "<Integer location,<Scalar v0,<Scalar v1,<Scalar v2" );
      ADD_FUNC( Uniform4f, "<Integer location,<Scalar v0,<Scalar v1,<Scalar v2,<Scalar v3" );
      ADD_FUNC( UniformMatrix3fv, "<Integer location,<Integer count,<Boolean transpose,<Data value" );
      ADD_FUNC( UniformMatrix4fv, "<Integer location,<Integer count,<Boolean transpose,<Data value" );
      ADD_FUNC( UseProgram, "<Integer program" );
      ADD_FUNC( Vertex3f, "<Scalar x,<Scalar y,<Scalar z" );
      ADD_FUNC( VertexAttribPointer, "<Integer index,<Integer size,<Integer type,<Boolean normalized,<Integer stride,<Data pointer" );
      ADD_FUNC( VertexPointer, "<Integer size,<Integer type,<Integer stride,<Data pointer" );
      ADD_FUNC( Viewport, "<Integer x,<Integer y,<Integer width,<Integer height" );

      // == GL_EXT_framebuffer_object ==
      ADD_CONST_INT( "GL_MAX_RENDERBUFFER_SIZE_EXT", 0x84E8 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_BINDING_EXT",  0x8CA6 );
      ADD_CONST_INT( "GL_RENDERBUFFER_BINDING_EXT",  0x8CA7 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT",  0x8CD0 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT",  0x8CD1 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT",  0x8CD2 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT",  0x8CD3 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT",  0x8CD4 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_COMPLETE_EXT",  0x8CD5 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT",  0x8CD6 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT",  0x8CD7 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT",  0x8CD9 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT",  0x8CDA );
      ADD_CONST_INT( "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT",  0x8CDB );
      ADD_CONST_INT( "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT",  0x8CDC );
      ADD_CONST_INT( "GL_FRAMEBUFFER_UNSUPPORTED_EXT",  0x8CDD );
      ADD_CONST_INT( "GL_MAX_COLOR_ATTACHMENTS_EXT",  0x8CDF );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT0_EXT",  0x8CE0 );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT1_EXT",  0x8CE1 );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT2_EXT",  0x8CE2 );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT3_EXT",  0x8CE3 );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT4_EXT",  0x8CE4 );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT5_EXT",  0x8CE5 );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT6_EXT",  0x8CE6 );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT7_EXT",  0x8CE7 );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT8_EXT",  0x8CE8 );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT9_EXT",  0x8CE9 );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT10_EXT",  0x8CEA );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT11_EXT",  0x8CEB );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT12_EXT",  0x8CEC );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT13_EXT",  0x8CED );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT14_EXT",  0x8CEE );
      ADD_CONST_INT( "GL_COLOR_ATTACHMENT15_EXT",  0x8CEF );
      ADD_CONST_INT( "GL_DEPTH_ATTACHMENT_EXT",  0x8D00 );
      ADD_CONST_INT( "GL_STENCIL_ATTACHMENT_EXT",  0x8D20 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_EXT",  0x8D40 );
      ADD_CONST_INT( "GL_RENDERBUFFER_EXT",  0x8D41 );
      ADD_CONST_INT( "GL_RENDERBUFFER_WIDTH_EXT",  0x8D42 );
      ADD_CONST_INT( "GL_RENDERBUFFER_HEIGHT_EXT",  0x8D43 );
      ADD_CONST_INT( "GL_RENDERBUFFER_INTERNAL_FORMAT_EXT",  0x8D44 );
      ADD_CONST_INT( "GL_STENCIL_INDEX1_EXT",  0x8D46 );
      ADD_CONST_INT( "GL_STENCIL_INDEX4_EXT",  0x8D47 );
      ADD_CONST_INT( "GL_STENCIL_INDEX8_EXT",  0x8D48 );
      ADD_CONST_INT( "GL_STENCIL_INDEX16_EXT",  0x8D49 );
      ADD_CONST_INT( "GL_RENDERBUFFER_RED_SIZE_EXT",  0x8D50 );
      ADD_CONST_INT( "GL_RENDERBUFFER_GREEN_SIZE_EXT",  0x8D51 );
      ADD_CONST_INT( "GL_RENDERBUFFER_BLUE_SIZE_EXT",  0x8D52 );
      ADD_CONST_INT( "GL_RENDERBUFFER_ALPHA_SIZE_EXT",  0x8D53 );
      ADD_CONST_INT( "GL_RENDERBUFFER_DEPTH_SIZE_EXT",  0x8D54 );
      ADD_CONST_INT( "GL_RENDERBUFFER_STENCIL_SIZE_EXT", 0x8D55 );
	  

/* ----------------------------- GL_VERSION_1_1 ---------------------------- */

      ADD_CONST_INT( "GL_TEXTURE_WIDTH", 0x1000 );
      ADD_CONST_INT( "GL_TEXTURE_HEIGHT", 0x1001 );
      ADD_CONST_INT( "GL_TEXTURE_INTERNAL_FORMAT", 0x1003 );
      ADD_CONST_INT( "GL_TEXTURE_BORDER_COLOR", 0x1004 );
      ADD_CONST_INT( "GL_TEXTURE_BORDER", 0x1005 );
	  
/* ----------------------------- GL_VERSION_1_3 ---------------------------- */


      ADD_CONST_INT( "GL_MULTISAMPLE", 0x809D );
      ADD_CONST_INT( "GL_SAMPLE_ALPHA_TO_COVERAGE", 0x809E );
      ADD_CONST_INT( "GL_SAMPLE_ALPHA_TO_ONE", 0x809F );
      ADD_CONST_INT( "GL_SAMPLE_COVERAGE", 0x80A0 );
      ADD_CONST_INT( "GL_SAMPLE_BUFFERS", 0x80A8 );
      ADD_CONST_INT( "GL_SAMPLES", 0x80A9 );
      ADD_CONST_INT( "GL_SAMPLE_COVERAGE_VALUE", 0x80AA );
      ADD_CONST_INT( "GL_SAMPLE_COVERAGE_INVERT", 0x80AB );
      ADD_CONST_INT( "GL_CLAMP_TO_BORDER", 0x812D );
      ADD_CONST_INT( "GL_TEXTURE0", 0x84C0 );
      ADD_CONST_INT( "GL_TEXTURE1", 0x84C1 );
      ADD_CONST_INT( "GL_TEXTURE2", 0x84C2 );
      ADD_CONST_INT( "GL_TEXTURE3", 0x84C3 );
      ADD_CONST_INT( "GL_TEXTURE4", 0x84C4 );
      ADD_CONST_INT( "GL_TEXTURE5", 0x84C5 );
      ADD_CONST_INT( "GL_TEXTURE6", 0x84C6 );
      ADD_CONST_INT( "GL_TEXTURE7", 0x84C7 );
      ADD_CONST_INT( "GL_TEXTURE8", 0x84C8 );
      ADD_CONST_INT( "GL_TEXTURE9", 0x84C9 );
      ADD_CONST_INT( "GL_TEXTURE10", 0x84CA );
      ADD_CONST_INT( "GL_TEXTURE11", 0x84CB );
      ADD_CONST_INT( "GL_TEXTURE12", 0x84CC );
      ADD_CONST_INT( "GL_TEXTURE13", 0x84CD );
      ADD_CONST_INT( "GL_TEXTURE14", 0x84CE );
      ADD_CONST_INT( "GL_TEXTURE15", 0x84CF );
      ADD_CONST_INT( "GL_TEXTURE16", 0x84D0 );
      ADD_CONST_INT( "GL_TEXTURE17", 0x84D1 );
      ADD_CONST_INT( "GL_TEXTURE18", 0x84D2 );
      ADD_CONST_INT( "GL_TEXTURE19", 0x84D3 );
      ADD_CONST_INT( "GL_TEXTURE20", 0x84D4 );
      ADD_CONST_INT( "GL_TEXTURE21", 0x84D5 );
      ADD_CONST_INT( "GL_TEXTURE22", 0x84D6 );
      ADD_CONST_INT( "GL_TEXTURE23", 0x84D7 );
      ADD_CONST_INT( "GL_TEXTURE24", 0x84D8 );
      ADD_CONST_INT( "GL_TEXTURE25", 0x84D9 );
      ADD_CONST_INT( "GL_TEXTURE26", 0x84DA );
      ADD_CONST_INT( "GL_TEXTURE27", 0x84DB );
      ADD_CONST_INT( "GL_TEXTURE28", 0x84DC );
      ADD_CONST_INT( "GL_TEXTURE29", 0x84DD );
      ADD_CONST_INT( "GL_TEXTURE30", 0x84DE );
      ADD_CONST_INT( "GL_TEXTURE31", 0x84DF );
      ADD_CONST_INT( "GL_ACTIVE_TEXTURE", 0x84E0 );
      ADD_CONST_INT( "GL_CLIENT_ACTIVE_TEXTURE", 0x84E1 );
      ADD_CONST_INT( "GL_MAX_TEXTURE_UNITS", 0x84E2 );
      ADD_CONST_INT( "GL_TRANSPOSE_MODELVIEW_MATRIX", 0x84E3 );
      ADD_CONST_INT( "GL_TRANSPOSE_PROJECTION_MATRIX", 0x84E4 );
      ADD_CONST_INT( "GL_TRANSPOSE_TEXTURE_MATRIX", 0x84E5 );
      ADD_CONST_INT( "GL_TRANSPOSE_COLOR_MATRIX", 0x84E6 );
      ADD_CONST_INT( "GL_SUBTRACT", 0x84E7 );
      ADD_CONST_INT( "GL_COMPRESSED_ALPHA", 0x84E9 );
      ADD_CONST_INT( "GL_COMPRESSED_LUMINANCE", 0x84EA );
      ADD_CONST_INT( "GL_COMPRESSED_LUMINANCE_ALPHA", 0x84EB );
      ADD_CONST_INT( "GL_COMPRESSED_INTENSITY", 0x84EC );
      ADD_CONST_INT( "GL_COMPRESSED_RGB", 0x84ED );
      ADD_CONST_INT( "GL_COMPRESSED_RGBA", 0x84EE );
      ADD_CONST_INT( "GL_TEXTURE_COMPRESSION_HINT", 0x84EF );
      ADD_CONST_INT( "GL_NORMAL_MAP", 0x8511 );
      ADD_CONST_INT( "GL_REFLECTION_MAP", 0x8512 );
      ADD_CONST_INT( "GL_TEXTURE_CUBE_MAP", 0x8513 );
      ADD_CONST_INT( "GL_TEXTURE_BINDING_CUBE_MAP", 0x8514 );
      ADD_CONST_INT( "GL_TEXTURE_CUBE_MAP_POSITIVE_X", 0x8515 );
      ADD_CONST_INT( "GL_TEXTURE_CUBE_MAP_NEGATIVE_X", 0x8516 );
      ADD_CONST_INT( "GL_TEXTURE_CUBE_MAP_POSITIVE_Y", 0x8517 );
      ADD_CONST_INT( "GL_TEXTURE_CUBE_MAP_NEGATIVE_Y", 0x8518 );
      ADD_CONST_INT( "GL_TEXTURE_CUBE_MAP_POSITIVE_Z", 0x8519 );
      ADD_CONST_INT( "GL_TEXTURE_CUBE_MAP_NEGATIVE_Z", 0x851A );
      ADD_CONST_INT( "GL_PROXY_TEXTURE_CUBE_MAP", 0x851B );
      ADD_CONST_INT( "GL_MAX_CUBE_MAP_TEXTURE_SIZE", 0x851C );
      ADD_CONST_INT( "GL_COMBINE", 0x8570 );
      ADD_CONST_INT( "GL_COMBINE_RGB", 0x8571 );
      ADD_CONST_INT( "GL_COMBINE_ALPHA", 0x8572 );
      ADD_CONST_INT( "GL_RGB_SCALE", 0x8573 );
      ADD_CONST_INT( "GL_ADD_SIGNED", 0x8574 );
      ADD_CONST_INT( "GL_INTERPOLATE", 0x8575 );
      ADD_CONST_INT( "GL_CONSTANT", 0x8576 );
      ADD_CONST_INT( "GL_PRIMARY_COLOR", 0x8577 );
      ADD_CONST_INT( "GL_PREVIOUS", 0x8578 );
      ADD_CONST_INT( "GL_SOURCE0_RGB", 0x8580 );
      ADD_CONST_INT( "GL_SOURCE1_RGB", 0x8581 );
      ADD_CONST_INT( "GL_SOURCE2_RGB", 0x8582 );
      ADD_CONST_INT( "GL_SOURCE0_ALPHA", 0x8588 );
      ADD_CONST_INT( "GL_SOURCE1_ALPHA", 0x8589 );
      ADD_CONST_INT( "GL_SOURCE2_ALPHA", 0x858A );
      ADD_CONST_INT( "GL_OPERAND0_RGB", 0x8590 );
      ADD_CONST_INT( "GL_OPERAND1_RGB", 0x8591 );
      ADD_CONST_INT( "GL_OPERAND2_RGB", 0x8592 );
      ADD_CONST_INT( "GL_OPERAND0_ALPHA", 0x8598 );
      ADD_CONST_INT( "GL_OPERAND1_ALPHA", 0x8599 );
      ADD_CONST_INT( "GL_OPERAND2_ALPHA", 0x859A );
      ADD_CONST_INT( "GL_TEXTURE_COMPRESSED_IMAGE_SIZE", 0x86A0 );
      ADD_CONST_INT( "GL_TEXTURE_COMPRESSED", 0x86A1 );
      ADD_CONST_INT( "GL_NUM_COMPRESSED_TEXTURE_FORMATS", 0x86A2 );
      ADD_CONST_INT( "GL_COMPRESSED_TEXTURE_FORMATS", 0x86A3 );
      ADD_CONST_INT( "GL_DOT3_RGB", 0x86AE );
      ADD_CONST_INT( "GL_DOT3_RGBA", 0x86AF );
      ADD_CONST_INT( "GL_MULTISAMPLE_BIT", 0x20000000 );
	  
	  
/* ----------------------------- GL_VERSION_1_4 ---------------------------- */
      ADD_CONST_INT( "GL_BLEND_DST_RGB", 0x80C8 );
      ADD_CONST_INT( "GL_BLEND_SRC_RGB", 0x80C9 );
      ADD_CONST_INT( "GL_BLEND_DST_ALPHA", 0x80CA );
      ADD_CONST_INT( "GL_BLEND_SRC_ALPHA", 0x80CB );
      ADD_CONST_INT( "GL_POINT_SIZE_MIN", 0x8126 );
      ADD_CONST_INT( "GL_POINT_SIZE_MAX", 0x8127 );
      ADD_CONST_INT( "GL_POINT_FADE_THRESHOLD_SIZE", 0x8128 );
      ADD_CONST_INT( "GL_POINT_DISTANCE_ATTENUATION", 0x8129 );
      ADD_CONST_INT( "GL_GENERATE_MIPMAP", 0x8191 );
      ADD_CONST_INT( "GL_GENERATE_MIPMAP_HINT", 0x8192 );
      ADD_CONST_INT( "GL_DEPTH_COMPONENT16", 0x81A5 );
      ADD_CONST_INT( "GL_DEPTH_COMPONENT24", 0x81A6 );
      ADD_CONST_INT( "GL_DEPTH_COMPONENT32", 0x81A7 );
      ADD_CONST_INT( "GL_MIRRORED_REPEAT", 0x8370 );
      ADD_CONST_INT( "GL_FOG_COORDINATE_SOURCE", 0x8450 );
      ADD_CONST_INT( "GL_FOG_COORDINATE", 0x8451 );
      ADD_CONST_INT( "GL_FRAGMENT_DEPTH", 0x8452 );
      ADD_CONST_INT( "GL_CURRENT_FOG_COORDINATE", 0x8453 );
      ADD_CONST_INT( "GL_FOG_COORDINATE_ARRAY_TYPE", 0x8454 );
      ADD_CONST_INT( "GL_FOG_COORDINATE_ARRAY_STRIDE", 0x8455 );
      ADD_CONST_INT( "GL_FOG_COORDINATE_ARRAY_POINTER", 0x8456 );
      ADD_CONST_INT( "GL_FOG_COORDINATE_ARRAY", 0x8457 );
      ADD_CONST_INT( "GL_COLOR_SUM", 0x8458 );
      ADD_CONST_INT( "GL_CURRENT_SECONDARY_COLOR", 0x8459 );
      ADD_CONST_INT( "GL_SECONDARY_COLOR_ARRAY_SIZE", 0x845A );
      ADD_CONST_INT( "GL_SECONDARY_COLOR_ARRAY_TYPE", 0x845B );
      ADD_CONST_INT( "GL_SECONDARY_COLOR_ARRAY_STRIDE", 0x845C );
      ADD_CONST_INT( "GL_SECONDARY_COLOR_ARRAY_POINTER", 0x845D );
      ADD_CONST_INT( "GL_SECONDARY_COLOR_ARRAY", 0x845E );
      ADD_CONST_INT( "GL_MAX_TEXTURE_LOD_BIAS", 0x84FD );
      ADD_CONST_INT( "GL_TEXTURE_FILTER_CONTROL", 0x8500 );
      ADD_CONST_INT( "GL_TEXTURE_LOD_BIAS", 0x8501 );
      ADD_CONST_INT( "GL_INCR_WRAP", 0x8507 );
      ADD_CONST_INT( "GL_DECR_WRAP", 0x8508 );
      ADD_CONST_INT( "GL_TEXTURE_DEPTH_SIZE", 0x884A );
      ADD_CONST_INT( "GL_DEPTH_TEXTURE_MODE", 0x884B );
      ADD_CONST_INT( "GL_TEXTURE_COMPARE_MODE", 0x884C );
      ADD_CONST_INT( "GL_TEXTURE_COMPARE_FUNC", 0x884D );
      ADD_CONST_INT( "GL_COMPARE_R_TO_TEXTURE", 0x884E );
	  
/* -------------------------- GL_ARB_texture_float ------------------------- */
      ADD_CONST_INT( "GL_RGBA32F_ARB", 0x8814 );
      ADD_CONST_INT( "GL_RGB32F_ARB", 0x8815 );
      ADD_CONST_INT( "GL_ALPHA32F_ARB", 0x8816 );
      ADD_CONST_INT( "GL_INTENSITY32F_ARB", 0x8817 );
      ADD_CONST_INT( "GL_LUMINANCE32F_ARB", 0x8818 );
      ADD_CONST_INT( "GL_LUMINANCE_ALPHA32F_ARB", 0x8819 );
      ADD_CONST_INT( "GL_RGBA16F_ARB", 0x881A );
      ADD_CONST_INT( "GL_RGB16F_ARB", 0x881B );
      ADD_CONST_INT( "GL_ALPHA16F_ARB", 0x881C );
      ADD_CONST_INT( "GL_INTENSITY16F_ARB", 0x881D );
      ADD_CONST_INT( "GL_LUMINANCE16F_ARB", 0x881E );
      ADD_CONST_INT( "GL_LUMINANCE_ALPHA16F_ARB", 0x881F );
      ADD_CONST_INT( "GL_TEXTURE_RED_TYPE_ARB", 0x8C10 );
      ADD_CONST_INT( "GL_TEXTURE_GREEN_TYPE_ARB", 0x8C11 );
      ADD_CONST_INT( "GL_TEXTURE_BLUE_TYPE_ARB", 0x8C12 );
      ADD_CONST_INT( "GL_TEXTURE_ALPHA_TYPE_ARB", 0x8C13 );
      ADD_CONST_INT( "GL_TEXTURE_LUMINANCE_TYPE_ARB", 0x8C14 );
      ADD_CONST_INT( "GL_TEXTURE_INTENSITY_TYPE_ARB", 0x8C15 );
      ADD_CONST_INT( "GL_TEXTURE_DEPTH_TYPE_ARB", 0x8C16 );
      ADD_CONST_INT( "GL_UNSIGNED_NORMALIZED_ARB", 0x8C17 );
	 

      ADD_FUNC( BindFramebufferEXT, "<Integer target,<Integer framebuffer" );
      ADD_FUNC( BindRenderbufferEXT, "<Integer target,<Integer renderbuffer" );
      ADD_FUNC( CheckFramebufferStatusEXT, "=Integer,<Integer target" );
      ADD_FUNC( DeleteFramebuffersEXT, "<Integer n,<Data framebuffers" );
      ADD_FUNC( DeleteRenderbuffersEXT, "<Integer n,<Data renderbuffers" );
      ADD_FUNC( FramebufferRenderbufferEXT, "<Integer target,<Integer attachment,<Integer renderbuffertarget,<Integer renderbuffer" );
      ADD_FUNC( FramebufferTexture1DEXT, "<Integer target,<Integer attachment,<Integer textarget,<Integer texture,<Integer level" );
      ADD_FUNC( FramebufferTexture2DEXT, "<Integer target,<Integer attachment,<Integer textarget,<Integer texture,<Integer level" );
      ADD_FUNC( FramebufferTexture3DEXT, "<Integer target,<Integer attachment,<Integer textarget,<Integer texture,<Integer level,<Integer zoffset" );
      ADD_FUNC( GenFramebuffersEXT, "<Integer n,<Data framebuffers" );
      ADD_FUNC( GenRenderbuffersEXT, "<Integer n,<Data renderbuffers" );
      ADD_FUNC( GenerateMipmapEXT, "<Integer target" );
      ADD_FUNC( GetFramebufferAttachmentParameterivEXT, "<Integer target,<Integer attachment,<Integer pname,<Data params" );
      ADD_FUNC( GetRenderbufferParameterivEXT, "<Integer target,<Integer attachment,<Data params" );
      ADD_FUNC( IsFramebufferEXT, "=Boolean,<Integer framebuffer" );
      ADD_FUNC( IsRenderbufferEXT, "=Boolean,<Integer renderbuffer" );
      ADD_FUNC( RenderbufferStorageEXT, "<Integer target,<Integer internalformat,<Integer width,<Integer height" );

      // == GL_EXT_geometry_shader4 ==
      ADD_CONST_INT( "GL_LINES_ADJACENCY_EXT", 0xA );
      ADD_CONST_INT( "GL_LINE_STRIP_ADJACENCY_EXT", 0xB );
      ADD_CONST_INT( "GL_TRIANGLES_ADJACENCY_EXT", 0xC );
      ADD_CONST_INT( "GL_TRIANGLE_STRIP_ADJACENCY_EXT", 0xD );
      ADD_CONST_INT( "GL_PROGRAM_POINT_SIZE_EXT", 0x8642 );
      ADD_CONST_INT( "GL_MAX_VARYING_COMPONENTS_EXT", 0x8B4B );
      ADD_CONST_INT( "GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT", 0x8C29 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT", 0x8CD4 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT", 0x8DA7 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT", 0x8DA8 );
      ADD_CONST_INT( "GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_EXT", 0x8DA9 );
      ADD_CONST_INT( "GL_GEOMETRY_SHADER_EXT", 0x8DD9 );
      ADD_CONST_INT( "GL_GEOMETRY_VERTICES_OUT_EXT", 0x8DDA );
      ADD_CONST_INT( "GL_GEOMETRY_INPUT_TYPE_EXT", 0x8DDB );
      ADD_CONST_INT( "GL_GEOMETRY_OUTPUT_TYPE_EXT", 0x8DDC );
      ADD_CONST_INT( "GL_MAX_GEOMETRY_VARYING_COMPONENTS_EXT", 0x8DDD );
      ADD_CONST_INT( "GL_MAX_VERTEX_VARYING_COMPONENTS_EXT", 0x8DDE );
      ADD_CONST_INT( "GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT", 0x8DDF );
      ADD_CONST_INT( "GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT", 0x8DE0 );
      ADD_CONST_INT( "GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT", 0x8DE1 );

      ADD_FUNC( FramebufferTextureEXT, "<Integer target,<Integer attachment,<Integer texture,<Integer level" );
      ADD_FUNC( FramebufferTextureFaceEXT, "<Integer target,<Integer attachment,<Integer texture,<Integer level,<Integer face" );
      ADD_FUNC( FramebufferTextureLayerEXT, "<Integer target,<Integer attachment,<Integer texture,<Integer level,<Integer layer" );
      ADD_FUNC( ProgramParameteriEXT, "<Integer program,<Integer pname,<Integer value" );

      /* DataType */
      ADD_CONST_INT( "GL_BYTE", 0x1400 );
      ADD_CONST_INT( "GL_UNSIGNED_BYTE", 0x1401 );
      ADD_CONST_INT( "GL_SHORT", 0x1402 );
      ADD_CONST_INT( "GL_UNSIGNED_SHORT", 0x1403 );
      ADD_CONST_INT( "GL_INT", 0x1404 );
      ADD_CONST_INT( "GL_UNSIGNED_INT", 0x1405 );
      ADD_CONST_INT( "GL_FLOAT", 0x1406 );
      ADD_CONST_INT( "GL_2_BYTES", 0x1407 );
      ADD_CONST_INT( "GL_3_BYTES", 0x1408 );
      ADD_CONST_INT( "GL_4_BYTES", 0x1409 );
      ADD_CONST_INT( "GL_DOUBLE", 0x140A );
      ADD_CONST_INT( "GL_FLOAT_VEC2", 0x8B50 );
      ADD_CONST_INT( "GL_FLOAT_VEC3", 0x8B51 );
      ADD_CONST_INT( "GL_FLOAT_VEC4", 0x8B52 );
      ADD_CONST_INT( "GL_INT_VEC2", 0x8B53 );
      ADD_CONST_INT( "GL_INT_VEC3", 0x8B54 );
      ADD_CONST_INT( "GL_INT_VEC4", 0x8B55 );
      ADD_CONST_INT( "GL_BOOL", 0x8B56 );
      ADD_CONST_INT( "GL_BOOL_VEC2", 0x8B57 );
      ADD_CONST_INT( "GL_BOOL_VEC3", 0x8B58 );
      ADD_CONST_INT( "GL_BOOL_VEC4", 0x8B59 );
      ADD_CONST_INT( "GL_FLOAT_MAT2", 0x8B5A );
      ADD_CONST_INT( "GL_FLOAT_MAT3", 0x8B5B );
      ADD_CONST_INT( "GL_FLOAT_MAT4", 0x8B5C );
      ADD_CONST_INT( "GL_SAMPLER_1D", 0x8B5D );
      ADD_CONST_INT( "GL_SAMPLER_2D", 0x8B5E );
      ADD_CONST_INT( "GL_SAMPLER_3D", 0x8B5F );
      ADD_CONST_INT( "GL_SAMPLER_CUBE", 0x8B60 );
      ADD_CONST_INT( "GL_SAMPLER_1D_SHADOW", 0x8B61 );
      ADD_CONST_INT( "GL_SAMPLER_2D_SHADOW", 0x8B62 );

      // glEnableClientState
      ADD_CONST_INT( "GL_VERTEX_ARRAY", 0x8074 );
      ADD_CONST_INT( "GL_NORMAL_ARRAY", 0x8075 );
      ADD_CONST_INT( "GL_COLOR_ARRAY", 0x8076 );
      ADD_CONST_INT( "GL_INDEX_ARRAY", 0x8077 );
      ADD_CONST_INT( "GL_TEXTURE_COORD_ARRAY", 0x8078 );
      ADD_CONST_INT( "GL_EDGE_FLAG_ARRAY", 0x8079 );
      
      // GL_POINTS constants
      ADD_CONST_INT( "GL_ALIASED_POINT_SIZE_RANGE", 0x846D );
      ADD_CONST_INT( "GL_POINT_SIZE_MIN", 0x8126 );
      ADD_CONST_INT( "GL_POINT_SIZE_MAX", 0x8127 );
      ADD_CONST_INT( "GL_POINT_FADE_THRESHOLD_SIZE", 0x8128 );
      ADD_CONST_INT( "GL_POINT_DISTANCE_ATTENUATION", 0x8129 );
      ADD_CONST_INT( "GL_POINT_SPRITE", 0x8861 );
      ADD_CONST_INT( "GL_COORD_REPLACE", 0x8862 );
      ADD_CONST_INT( "GL_POINT_SPRITE_COORD_ORIGIN", 0x8CA0 );
      ADD_CONST_INT( "GL_POINT_SMOOTH", 0x0B10 );
      ADD_CONST_INT( "GL_POINT_SIZE", 0x0B11 );
      ADD_CONST_INT( "GL_POINT_SMOOTH_HINT", 0x0C51 );
      ADD_CONST_INT( "GL_SMOOTH_POINT_SIZE_RANGE", 0x0B12 );
      ADD_CONST_INT( "GL_SMOOTH_POINT_SIZE_GRANULARITY", 0x0B13 );
      ADD_CONST_INT( "GL_VERTEX_PROGRAM_POINT_SIZE", 0x8642 );
      
      // GL_LINES constants
      ADD_CONST_INT( "GL_ALIASED_LINE_WIDTH_RANGE", 0x846E );
      ADD_CONST_INT( "GL_LINE_SMOOTH", 0x0B20 );
      ADD_CONST_INT( "GL_LINE_WIDTH", 0x0B21 );
      ADD_CONST_INT( "GL_LINE_STIPPLE", 0x0B24 );
      ADD_CONST_INT( "GL_LINE_STIPPLE_PATTERN", 0x0B25 );
      ADD_CONST_INT( "GL_LINE_STIPPLE_REPEAT", 0x0B26 );
      ADD_CONST_INT( "GL_LINE_SMOOTH_HINT", 0x0C52 );
      ADD_CONST_INT( "GL_SMOOTH_LINE_WIDTH_RANGE", 0x0B22 );
      ADD_CONST_INT( "GL_SMOOTH_LINE_WIDTH_GRANULARITY", 0x0B23 );
      
      ADD_CONST_INT( "GL_GENERATE_MIPMAP", 0x8191 );
      ADD_CONST_INT( "GL_GENERATE_MIPMAP_HINT", 0x8192 );
      ADD_CONST_INT( "GL_NEAREST_MIPMAP_NEAREST", 0x2700 );
      ADD_CONST_INT( "GL_LINEAR_MIPMAP_NEAREST", 0x2701 );
      ADD_CONST_INT( "GL_NEAREST_MIPMAP_LINEAR", 0x2702 );
      ADD_CONST_INT( "GL_LINEAR_MIPMAP_LINEAR", 0x2703 );
      ADD_CONST_INT( "GL_ALPHA4", 0x803B );
      ADD_CONST_INT( "GL_ALPHA8", 0x803C );
      ADD_CONST_INT( "GL_ALPHA12", 0x803D );
      ADD_CONST_INT( "GL_ALPHA16", 0x803E );
      ADD_CONST_INT( "GL_LUMINANCE4", 0x803F );
      ADD_CONST_INT( "GL_LUMINANCE8", 0x8040 );
      ADD_CONST_INT( "GL_LUMINANCE12", 0x8041 );
      ADD_CONST_INT( "GL_LUMINANCE16", 0x8042 );
      ADD_CONST_INT( "GL_LUMINANCE4_ALPHA4", 0x8043 );
      ADD_CONST_INT( "GL_LUMINANCE6_ALPHA2", 0x8044 );
      ADD_CONST_INT( "GL_LUMINANCE8_ALPHA8", 0x8045 );
      ADD_CONST_INT( "GL_LUMINANCE12_ALPHA4", 0x8046 );
      ADD_CONST_INT( "GL_LUMINANCE12_ALPHA12", 0x8047 );
      ADD_CONST_INT( "GL_LUMINANCE16_ALPHA16", 0x8048 );
      ADD_CONST_INT( "GL_INTENSITY", 0x8049 );
      ADD_CONST_INT( "GL_INTENSITY4", 0x804A );
      ADD_CONST_INT( "GL_INTENSITY8", 0x804B );
      ADD_CONST_INT( "GL_INTENSITY12", 0x804C );
      ADD_CONST_INT( "GL_INTENSITY16", 0x804D );
      ADD_CONST_INT( "GL_R3_G3_B2", 0x2A10 );
      ADD_CONST_INT( "GL_RGB4", 0x804F );
      ADD_CONST_INT( "GL_RGB5", 0x8050 );
      ADD_CONST_INT( "GL_RGB8", 0x8051 );
      ADD_CONST_INT( "GL_RGB10", 0x8052 );
      ADD_CONST_INT( "GL_RGB12", 0x8053 );
      ADD_CONST_INT( "GL_RGB16", 0x8054 );
      ADD_CONST_INT( "GL_RGBA2", 0x8055 );
      ADD_CONST_INT( "GL_RGBA4", 0x8056 );
      ADD_CONST_INT( "GL_RGB5_A1", 0x8057 );
      ADD_CONST_INT( "GL_RGBA8", 0x8058 );
      ADD_CONST_INT( "GL_RGB10_A2", 0x8059 );
      ADD_CONST_INT( "GL_RGBA12", 0x805A );
      ADD_CONST_INT( "GL_RGBA16", 0x805B );
      ADD_CONST_INT( "GL_LIST_MODE", 0x0B30 );
      ADD_CONST_INT( "GL_MAX_LIST_NESTING", 0x0B31 );
      ADD_CONST_INT( "GL_LIST_BASE", 0x0B32 );
      ADD_CONST_INT( "GL_LIST_INDEX", 0x0B33 );
      ADD_CONST_INT( "GL_POLYGON_MODE", 0x0B40 );
      ADD_CONST_INT( "GL_POLYGON_SMOOTH", 0x0B41 );
      ADD_CONST_INT( "GL_POLYGON_STIPPLE", 0x0B42 );
      ADD_CONST_INT( "GL_EDGE_FLAG", 0x0B43 );
      ADD_CONST_INT( "GL_CULL_FACE", 0x0B44 );
      ADD_CONST_INT( "GL_CULL_FACE_MODE", 0x0B45 );
      ADD_CONST_INT( "GL_FRONT_FACE", 0x0B46 );
      ADD_CONST_INT( "GL_LIGHTING", 0x0B50 );
      ADD_CONST_INT( "GL_LIGHT_MODEL_LOCAL_VIEWER", 0x0B51 );
      ADD_CONST_INT( "GL_LIGHT_MODEL_TWO_SIDE", 0x0B52 );
      ADD_CONST_INT( "GL_LIGHT_MODEL_AMBIENT", 0x0B53 );
      ADD_CONST_INT( "GL_SHADE_MODEL", 0x0B54 );
      ADD_CONST_INT( "GL_COLOR_MATERIAL_FACE", 0x0B55 );
      ADD_CONST_INT( "GL_COLOR_MATERIAL_PARAMETER", 0x0B56 );
      ADD_CONST_INT( "GL_COLOR_MATERIAL", 0x0B57 );
      ADD_CONST_INT( "GL_FOG", 0x0B60 );
      ADD_CONST_INT( "GL_FOG_INDEX", 0x0B61 );
      ADD_CONST_INT( "GL_FOG_DENSITY", 0x0B62 );
      ADD_CONST_INT( "GL_FOG_START", 0x0B63 );
      ADD_CONST_INT( "GL_FOG_END", 0x0B64 );
      ADD_CONST_INT( "GL_FOG_MODE", 0x0B65 );
      ADD_CONST_INT( "GL_FOG_COLOR", 0x0B66 );
      ADD_CONST_INT( "GL_DEPTH_RANGE", 0x0B70 );
      ADD_CONST_INT( "GL_DEPTH_TEST", 0x0B71 );
      ADD_CONST_INT( "GL_DEPTH_WRITEMASK", 0x0B72 );
      ADD_CONST_INT( "GL_DEPTH_CLEAR_VALUE", 0x0B73 );
      ADD_CONST_INT( "GL_DEPTH_FUNC", 0x0B74 );
      ADD_CONST_INT( "GL_ACCUM_CLEAR_VALUE", 0x0B80 );
      ADD_CONST_INT( "GL_STENCIL_TEST", 0x0B90 );
      ADD_CONST_INT( "GL_STENCIL_CLEAR_VALUE", 0x0B91 );
      ADD_CONST_INT( "GL_STENCIL_FUNC", 0x0B92 );
      ADD_CONST_INT( "GL_STENCIL_VALUE_MASK", 0x0B93 );
      ADD_CONST_INT( "GL_STENCIL_FAIL", 0x0B94 );
      ADD_CONST_INT( "GL_STENCIL_PASS_DEPTH_FAIL", 0x0B95 );
      ADD_CONST_INT( "GL_STENCIL_PASS_DEPTH_PASS", 0x0B96 );
      ADD_CONST_INT( "GL_STENCIL_REF", 0x0B97 );
      ADD_CONST_INT( "GL_STENCIL_WRITEMASK", 0x0B98 );
      ADD_CONST_INT( "GL_MATRIX_MODE", 0x0BA0 );
      ADD_CONST_INT( "GL_NORMALIZE", 0x0BA1 );
      ADD_CONST_INT( "GL_VIEWPORT", 0x0BA2 );
      ADD_CONST_INT( "GL_MODELVIEW_STACK_DEPTH", 0x0BA3 );
      ADD_CONST_INT( "GL_PROJECTION_STACK_DEPTH", 0x0BA4 );
      ADD_CONST_INT( "GL_TEXTURE_STACK_DEPTH", 0x0BA5 );
      ADD_CONST_INT( "GL_MODELVIEW_MATRIX", 0x0BA6 );
      ADD_CONST_INT( "GL_PROJECTION_MATRIX", 0x0BA7 );
      ADD_CONST_INT( "GL_TEXTURE_MATRIX", 0x0BA8 );
      ADD_CONST_INT( "GL_ATTRIB_STACK_DEPTH", 0x0BB0 );
      ADD_CONST_INT( "GL_CLIENT_ATTRIB_STACK_DEPTH", 0x0BB1 );
      ADD_CONST_INT( "GL_ALPHA_TEST", 0x0BC0 );
      ADD_CONST_INT( "GL_ALPHA_TEST_FUNC", 0x0BC1 );
      ADD_CONST_INT( "GL_ALPHA_TEST_REF", 0x0BC2 );
      ADD_CONST_INT( "GL_DITHER", 0x0BD0 );
      ADD_CONST_INT( "GL_BLEND_DST", 0x0BE0 );
      ADD_CONST_INT( "GL_BLEND_SRC", 0x0BE1 );
      ADD_CONST_INT( "GL_BLEND", 0x0BE2 );
      ADD_CONST_INT( "GL_LOGIC_OP_MODE", 0x0BF0 );
      ADD_CONST_INT( "GL_INDEX_LOGIC_OP", 0x0BF1 );
      ADD_CONST_INT( "GL_COLOR_LOGIC_OP", 0x0BF2 );
      ADD_CONST_INT( "GL_AUX_BUFFERS", 0x0C00 );
      ADD_CONST_INT( "GL_DRAW_BUFFER", 0x0C01 );
      ADD_CONST_INT( "GL_READ_BUFFER", 0x0C02 );
      ADD_CONST_INT( "GL_SCISSOR_BOX", 0x0C10 );
      ADD_CONST_INT( "GL_SCISSOR_TEST", 0x0C11 );
      ADD_CONST_INT( "GL_INDEX_CLEAR_VALUE", 0x0C20 );
      ADD_CONST_INT( "GL_INDEX_WRITEMASK", 0x0C21 );
      ADD_CONST_INT( "GL_COLOR_CLEAR_VALUE", 0x0C22 );
      ADD_CONST_INT( "GL_COLOR_WRITEMASK", 0x0C23 );
      ADD_CONST_INT( "GL_INDEX_MODE", 0x0C30 );
      ADD_CONST_INT( "GL_RGBA_MODE", 0x0C31 );
      ADD_CONST_INT( "GL_DOUBLEBUFFER", 0x0C32 );
      ADD_CONST_INT( "GL_STEREO", 0x0C33 );
      ADD_CONST_INT( "GL_RENDER_MODE", 0x0C40 );
      ADD_CONST_INT( "GL_PERSPECTIVE_CORRECTION_HINT", 0x0C50 );
      ADD_CONST_INT( "GL_POINT_SMOOTH_HINT", 0x0C51 );
      ADD_CONST_INT( "GL_LINE_SMOOTH_HINT", 0x0C52 );
      ADD_CONST_INT( "GL_POLYGON_SMOOTH_HINT", 0x0C53 );
      ADD_CONST_INT( "GL_FOG_HINT", 0x0C54 );
      ADD_CONST_INT( "GL_TEXTURE_GEN_S", 0x0C60 );
      ADD_CONST_INT( "GL_TEXTURE_GEN_T", 0x0C61 );
      ADD_CONST_INT( "GL_TEXTURE_GEN_R", 0x0C62 );
      ADD_CONST_INT( "GL_TEXTURE_GEN_Q", 0x0C63 );
      ADD_CONST_INT( "GL_PIXEL_MAP_I_TO_I", 0x0C70 );
      ADD_CONST_INT( "GL_PIXEL_MAP_S_TO_S", 0x0C71 );
      ADD_CONST_INT( "GL_PIXEL_MAP_I_TO_R", 0x0C72 );
      ADD_CONST_INT( "GL_PIXEL_MAP_I_TO_G", 0x0C73 );
      ADD_CONST_INT( "GL_PIXEL_MAP_I_TO_B", 0x0C74 );
      ADD_CONST_INT( "GL_PIXEL_MAP_I_TO_A", 0x0C75 );
      ADD_CONST_INT( "GL_PIXEL_MAP_R_TO_R", 0x0C76 );
      ADD_CONST_INT( "GL_PIXEL_MAP_G_TO_G", 0x0C77 );
      ADD_CONST_INT( "GL_PIXEL_MAP_B_TO_B", 0x0C78 );
      ADD_CONST_INT( "GL_PIXEL_MAP_A_TO_A", 0x0C79 );
      ADD_CONST_INT( "GL_PIXEL_MAP_I_TO_I_SIZE", 0x0CB0 );
      ADD_CONST_INT( "GL_PIXEL_MAP_S_TO_S_SIZE", 0x0CB1 );
      ADD_CONST_INT( "GL_PIXEL_MAP_I_TO_R_SIZE", 0x0CB2 );
      ADD_CONST_INT( "GL_PIXEL_MAP_I_TO_G_SIZE", 0x0CB3 );
      ADD_CONST_INT( "GL_PIXEL_MAP_I_TO_B_SIZE", 0x0CB4 );
      ADD_CONST_INT( "GL_PIXEL_MAP_I_TO_A_SIZE", 0x0CB5 );
      ADD_CONST_INT( "GL_PIXEL_MAP_R_TO_R_SIZE", 0x0CB6 );
      ADD_CONST_INT( "GL_PIXEL_MAP_G_TO_G_SIZE", 0x0CB7 );
      ADD_CONST_INT( "GL_PIXEL_MAP_B_TO_B_SIZE", 0x0CB8 );
      ADD_CONST_INT( "GL_PIXEL_MAP_A_TO_A_SIZE", 0x0CB9 );
      ADD_CONST_INT( "GL_UNPACK_SWAP_BYTES", 0x0CF0 );
      ADD_CONST_INT( "GL_UNPACK_LSB_FIRST", 0x0CF1 );
      ADD_CONST_INT( "GL_UNPACK_ROW_LENGTH", 0x0CF2 );
      ADD_CONST_INT( "GL_UNPACK_SKIP_ROWS", 0x0CF3 );
      ADD_CONST_INT( "GL_UNPACK_SKIP_PIXELS", 0x0CF4 );
      ADD_CONST_INT( "GL_UNPACK_ALIGNMENT", 0x0CF5 );
      ADD_CONST_INT( "GL_PACK_SWAP_BYTES", 0x0D00 );
      ADD_CONST_INT( "GL_PACK_LSB_FIRST", 0x0D01 );
      ADD_CONST_INT( "GL_PACK_ROW_LENGTH", 0x0D02 );
      ADD_CONST_INT( "GL_PACK_SKIP_ROWS", 0x0D03 );
      ADD_CONST_INT( "GL_PACK_SKIP_PIXELS", 0x0D04 );
      ADD_CONST_INT( "GL_PACK_ALIGNMENT", 0x0D05 );
      ADD_CONST_INT( "GL_MAP_COLOR", 0x0D10 );
      ADD_CONST_INT( "GL_MAP_STENCIL", 0x0D11 );
      ADD_CONST_INT( "GL_INDEX_SHIFT", 0x0D12 );
      ADD_CONST_INT( "GL_INDEX_OFFSET", 0x0D13 );
      ADD_CONST_INT( "GL_RED_SCALE", 0x0D14 );
      ADD_CONST_INT( "GL_RED_BIAS", 0x0D15 );
      ADD_CONST_INT( "GL_ZOOM_X", 0x0D16 );
      ADD_CONST_INT( "GL_ZOOM_Y", 0x0D17 );
      ADD_CONST_INT( "GL_GREEN_SCALE", 0x0D18 );
      ADD_CONST_INT( "GL_GREEN_BIAS", 0x0D19 );
      ADD_CONST_INT( "GL_BLUE_SCALE", 0x0D1A );
      ADD_CONST_INT( "GL_BLUE_BIAS", 0x0D1B );
      ADD_CONST_INT( "GL_ALPHA_SCALE", 0x0D1C );
      ADD_CONST_INT( "GL_ALPHA_BIAS", 0x0D1D );
      ADD_CONST_INT( "GL_DEPTH_SCALE", 0x0D1E );
      ADD_CONST_INT( "GL_DEPTH_BIAS", 0x0D1F );
      ADD_CONST_INT( "GL_MAX_EVAL_ORDER", 0x0D30 );
      ADD_CONST_INT( "GL_MAX_LIGHTS", 0x0D31 );
      ADD_CONST_INT( "GL_MAX_CLIP_PLANES", 0x0D32 );
      ADD_CONST_INT( "GL_MAX_TEXTURE_SIZE", 0x0D33 );
      ADD_CONST_INT( "GL_MAX_PIXEL_MAP_TABLE", 0x0D34 );
      ADD_CONST_INT( "GL_MAX_ATTRIB_STACK_DEPTH", 0x0D35 );
      ADD_CONST_INT( "GL_MAX_MODELVIEW_STACK_DEPTH", 0x0D36 );
      ADD_CONST_INT( "GL_MAX_NAME_STACK_DEPTH", 0x0D37 );
      ADD_CONST_INT( "GL_MAX_PROJECTION_STACK_DEPTH", 0x0D38 );
      ADD_CONST_INT( "GL_MAX_TEXTURE_STACK_DEPTH", 0x0D39 );
      ADD_CONST_INT( "GL_MAX_VIEWPORT_DIMS", 0x0D3A );
      ADD_CONST_INT( "GL_MAX_CLIENT_ATTRIB_STACK_DEPTH", 0x0D3B );
      ADD_CONST_INT( "GL_SUBPIXEL_BITS", 0x0D50 );
      ADD_CONST_INT( "GL_INDEX_BITS", 0x0D51 );
      ADD_CONST_INT( "GL_RED_BITS", 0x0D52 );
      ADD_CONST_INT( "GL_GREEN_BITS", 0x0D53 );
      ADD_CONST_INT( "GL_BLUE_BITS", 0x0D54 );
      ADD_CONST_INT( "GL_ALPHA_BITS", 0x0D55 );
      ADD_CONST_INT( "GL_DEPTH_BITS", 0x0D56 );
      ADD_CONST_INT( "GL_STENCIL_BITS", 0x0D57 );
      ADD_CONST_INT( "GL_ACCUM_RED_BITS", 0x0D58 );
      ADD_CONST_INT( "GL_ACCUM_GREEN_BITS", 0x0D59 );
      ADD_CONST_INT( "GL_ACCUM_BLUE_BITS", 0x0D5A );
      ADD_CONST_INT( "GL_ACCUM_ALPHA_BITS", 0x0D5B );
      ADD_CONST_INT( "GL_NAME_STACK_DEPTH", 0x0D70 );
      ADD_CONST_INT( "GL_AUTO_NORMAL", 0x0D80 );
      ADD_CONST_INT( "GL_MAP1_COLOR_4", 0x0D90 );
      ADD_CONST_INT( "GL_MAP1_INDEX", 0x0D91 );
      ADD_CONST_INT( "GL_MAP1_NORMAL", 0x0D92 );
      ADD_CONST_INT( "GL_MAP1_TEXTURE_COORD_1", 0x0D93 );
      ADD_CONST_INT( "GL_MAP1_TEXTURE_COORD_2", 0x0D94 );
      ADD_CONST_INT( "GL_MAP1_TEXTURE_COORD_3", 0x0D95 );
      ADD_CONST_INT( "GL_MAP1_TEXTURE_COORD_4", 0x0D96 );
      ADD_CONST_INT( "GL_MAP1_VERTEX_3", 0x0D97 );
      ADD_CONST_INT( "GL_MAP1_VERTEX_4", 0x0D98 );
      ADD_CONST_INT( "GL_MAP2_COLOR_4", 0x0DB0 );
      ADD_CONST_INT( "GL_MAP2_INDEX", 0x0DB1 );
      ADD_CONST_INT( "GL_MAP2_NORMAL", 0x0DB2 );
      ADD_CONST_INT( "GL_MAP2_TEXTURE_COORD_1", 0x0DB3 );
      ADD_CONST_INT( "GL_MAP2_TEXTURE_COORD_2", 0x0DB4 );
      ADD_CONST_INT( "GL_MAP2_TEXTURE_COORD_3", 0x0DB5 );
      ADD_CONST_INT( "GL_MAP2_TEXTURE_COORD_4", 0x0DB6 );
      ADD_CONST_INT( "GL_MAP2_VERTEX_3", 0x0DB7 );
      ADD_CONST_INT( "GL_MAP2_VERTEX_4", 0x0DB8 );
      ADD_CONST_INT( "GL_MAP1_GRID_DOMAIN", 0x0DD0 );
      ADD_CONST_INT( "GL_MAP1_GRID_SEGMENTS", 0x0DD1 );
      ADD_CONST_INT( "GL_MAP2_GRID_DOMAIN", 0x0DD2 );
      ADD_CONST_INT( "GL_MAP2_GRID_SEGMENTS", 0x0DD3 );
      ADD_CONST_INT( "GL_TEXTURE_1D", 0x0DE0 );
      ADD_CONST_INT( "GL_TEXTURE_2D", 0x0DE1 );
      ADD_CONST_INT( "GL_FEEDBACK_BUFFER_POINTER", 0x0DF0 );
      ADD_CONST_INT( "GL_FEEDBACK_BUFFER_SIZE", 0x0DF1 );
      ADD_CONST_INT( "GL_FEEDBACK_BUFFER_TYPE", 0x0DF2 );
      ADD_CONST_INT( "GL_SELECTION_BUFFER_POINTER", 0x0DF3 );
      ADD_CONST_INT( "GL_SELECTION_BUFFER_SIZE", 0x0DF4 );

      /* BeginMode */
      ADD_CONST_INT( "GL_POINTS", 0x0000 );
      ADD_CONST_INT( "GL_LINES", 0x0001 );
      ADD_CONST_INT( "GL_LINE_LOOP", 0x0002 );
      ADD_CONST_INT( "GL_LINE_STRIP", 0x0003 );
      ADD_CONST_INT( "GL_TRIANGLES", 0x0004 );
      ADD_CONST_INT( "GL_TRIANGLE_STRIP", 0x0005 );
      ADD_CONST_INT( "GL_TRIANGLE_FAN", 0x0006 );
      ADD_CONST_INT( "GL_QUADS", 0x0007 );
      ADD_CONST_INT( "GL_QUAD_STRIP", 0x0008 );
      ADD_CONST_INT( "GL_POLYGON", 0x0009 );

      ADD_CONST_BOOL( "GL_TRUE", true );
      ADD_CONST_BOOL( "GL_FALSE", false );

      ADD_CONST_INT( "GL_DEPTH_TEST", 0x0B71 );
      ADD_CONST_INT( "GL_CULL_FACE", 0x0B44 );

      ADD_CONST_INT( "GL_ARRAY_BUFFER", 0x8892 );
      ADD_CONST_INT( "GL_ELEMENT_ARRAY_BUFFER", 0x8893 );
      ADD_CONST_INT( "GL_STATIC_DRAW", 0x88E4 );
      ADD_CONST_INT( "GL_DYNAMIC_DRAW", 0x88E8 );

      ADD_CONST_INT( "GL_VERTEX_SHADER", 0x8B31 );
      ADD_CONST_INT( "GL_FRAGMENT_SHADER", 0x8B30 );
      ADD_CONST_INT( "GL_DELETE_STATUS", 0x8B80 );
      ADD_CONST_INT( "GL_COMPILE_STATUS", 0x8B81 );
      ADD_CONST_INT( "GL_LINK_STATUS", 0x8B82 );
      ADD_CONST_INT( "GL_VALIDATE_STATUS", 0x8B83 );
      ADD_CONST_INT( "GL_INFO_LOG_LENGTH", 0x8B84 );
      ADD_CONST_INT( "GL_ATTACHED_SHADERS", 0x8B85 );
      ADD_CONST_INT( "GL_ACTIVE_UNIFORMS", 0x8B86 );
      ADD_CONST_INT( "GL_ACTIVE_UNIFORM_MAX_LENGTH", 0x8B87 );
      ADD_CONST_INT( "GL_SHADER_SOURCE_LENGTH", 0x8B88 );
      ADD_CONST_INT( "GL_ACTIVE_ATTRIBUTES", 0x8B89 );
      ADD_CONST_INT( "GL_ACTIVE_ATTRIBUTE_MAX_LENGTH", 0x8B8A );
      
      ADD_CONST_INT( "GL_INFO_LOG_LENGTH", 0x8B84 );

      ADD_CONST_INT( "GL_NEVER", 0x0200 );
      ADD_CONST_INT( "GL_LESS", 0x0201 );
      ADD_CONST_INT( "GL_EQUAL", 0x0202 );
      ADD_CONST_INT( "GL_LEQUAL", 0x0203 );
      ADD_CONST_INT( "GL_GREATER", 0x0204 );
      ADD_CONST_INT( "GL_NOTEQUAL", 0x0205 );
      ADD_CONST_INT( "GL_GEQUAL", 0x0206 );
      ADD_CONST_INT( "GL_ALWAYS", 0x0207 );


      // glPushAttrib
      ADD_CONST_INT( "GL_CURRENT_BIT", 0x00000001 );
      ADD_CONST_INT( "GL_POINT_BIT", 0x00000002 );
      ADD_CONST_INT( "GL_LINE_BIT", 0x00000004 );
      ADD_CONST_INT( "GL_POLYGON_BIT", 0x00000008 );
      ADD_CONST_INT( "GL_POLYGON_STIPPLE_BIT", 0x00000010 );
      ADD_CONST_INT( "GL_PIXEL_MODE_BIT", 0x00000020 );
      ADD_CONST_INT( "GL_LIGHTING_BIT", 0x00000040 );
      ADD_CONST_INT( "GL_FOG_BIT", 0x00000080 );
      ADD_CONST_INT( "GL_DEPTH_BUFFER_BIT", 0x00000100 );
      ADD_CONST_INT( "GL_ACCUM_BUFFER_BIT", 0x00000200 );
      ADD_CONST_INT( "GL_STENCIL_BUFFER_BIT", 0x00000400 );
      ADD_CONST_INT( "GL_VIEWPORT_BIT", 0x00000800 );
      ADD_CONST_INT( "GL_TRANSFORM_BIT", 0x00001000 );
      ADD_CONST_INT( "GL_ENABLE_BIT", 0x00002000 );
      ADD_CONST_INT( "GL_COLOR_BUFFER_BIT", 0x00004000 );
      ADD_CONST_INT( "GL_HINT_BIT", 0x00008000 );
      ADD_CONST_INT( "GL_EVAL_BIT", 0x00010000 );
      ADD_CONST_INT( "GL_LIST_BIT", 0x00020000 );
      ADD_CONST_INT( "GL_TEXTURE_BIT", 0x00040000 );
      ADD_CONST_INT( "GL_SCISSOR_BIT", 0x00080000 );
      ADD_CONST_INT( "GL_ALL_ATTRIB_BITS", 0x000fffff );

      
      ADD_CONST_INT( "GL_MODELVIEW", 0x1700 );
      ADD_CONST_INT( "GL_PROJECTION", 0x1701 );
      ADD_CONST_INT( "GL_TEXTURE", 0x1702 );

      ADD_CONST_INT( "GL_ZERO", 0 );
      ADD_CONST_INT( "GL_ONE", 1 );
      ADD_CONST_INT( "GL_SRC_COLOR", 0x0300 );
      ADD_CONST_INT( "GL_ONE_MINUS_SRC_COLOR", 0x0301 );
      ADD_CONST_INT( "GL_SRC_ALPHA", 0x0302 );
      ADD_CONST_INT( "GL_ONE_MINUS_SRC_ALPHA", 0x0303 );
      ADD_CONST_INT( "GL_DST_ALPHA", 0x0304 );
      ADD_CONST_INT( "GL_ONE_MINUS_DST_ALPHA", 0x0305 );
      ADD_CONST_INT( "GL_DST_COLOR", 0x0306 );
      ADD_CONST_INT( "GL_ONE_MINUS_DST_COLOR", 0x0307 );
      ADD_CONST_INT( "GL_SRC_ALPHA_SATURATE", 0x0308 );

      ADD_CONST_INT( "GL_POINT", 0x1B00 );
      ADD_CONST_INT( "GL_LINE", 0x1B01 );
      ADD_CONST_INT( "GL_FILL", 0x1B02 );


      ADD_CONST_INT( "GL_TEXTURE0", 0x84C0 );
      ADD_CONST_INT( "GL_TEXTURE1", 0x84C1 );
      ADD_CONST_INT( "GL_TEXTURE2", 0x84C2 );
      ADD_CONST_INT( "GL_TEXTURE3", 0x84C3 );
      ADD_CONST_INT( "GL_TEXTURE4", 0x84C4 );
      ADD_CONST_INT( "GL_TEXTURE5", 0x84C5 );
      ADD_CONST_INT( "GL_TEXTURE6", 0x84C6 );
      ADD_CONST_INT( "GL_TEXTURE7", 0x84C7 );
      ADD_CONST_INT( "GL_TEXTURE8", 0x84C8 );
      ADD_CONST_INT( "GL_TEXTURE9", 0x84C9 );
      ADD_CONST_INT( "GL_TEXTURE10", 0x84CA );
      ADD_CONST_INT( "GL_TEXTURE11", 0x84CB );
      ADD_CONST_INT( "GL_TEXTURE12", 0x84CC );
      ADD_CONST_INT( "GL_TEXTURE13", 0x84CD );
      ADD_CONST_INT( "GL_TEXTURE14", 0x84CE );
      ADD_CONST_INT( "GL_TEXTURE15", 0x84CF );
      ADD_CONST_INT( "GL_TEXTURE16", 0x84D0 );
      ADD_CONST_INT( "GL_TEXTURE17", 0x84D1 );
      ADD_CONST_INT( "GL_TEXTURE18", 0x84D2 );
      ADD_CONST_INT( "GL_TEXTURE19", 0x84D3 );
      ADD_CONST_INT( "GL_TEXTURE20", 0x84D4 );
      ADD_CONST_INT( "GL_TEXTURE21", 0x84D5 );
      ADD_CONST_INT( "GL_TEXTURE22", 0x84D6 );
      ADD_CONST_INT( "GL_TEXTURE23", 0x84D7 );
      ADD_CONST_INT( "GL_TEXTURE24", 0x84D8 );
      ADD_CONST_INT( "GL_TEXTURE25", 0x84D9 );
      ADD_CONST_INT( "GL_TEXTURE26", 0x84DA );
      ADD_CONST_INT( "GL_TEXTURE27", 0x84DB );
      ADD_CONST_INT( "GL_TEXTURE28", 0x84DC );
      ADD_CONST_INT( "GL_TEXTURE29", 0x84DD );
      ADD_CONST_INT( "GL_TEXTURE30", 0x84DE );
      ADD_CONST_INT( "GL_TEXTURE31", 0x84DF );
      ADD_CONST_INT( "GL_ACTIVE_TEXTURE", 0x84E0 );
      ADD_CONST_INT( "GL_CLIENT_ACTIVE_TEXTURE", 0x84E1 );
      ADD_CONST_INT( "GL_MAX_TEXTURE_UNITS", 0x84E2 );

      ADD_CONST_INT( "GL_TEXTURE_1D", 0x0DE0 );
      ADD_CONST_INT( "GL_TEXTURE_2D", 0x0DE1 );
      ADD_CONST_INT( "GL_FEEDBACK_BUFFER_POINTER", 0x0DF0 );
      ADD_CONST_INT( "GL_FEEDBACK_BUFFER_SIZE", 0x0DF1 );
      ADD_CONST_INT( "GL_FEEDBACK_BUFFER_TYPE", 0x0DF2 );
      ADD_CONST_INT( "GL_SELECTION_BUFFER_POINTER", 0x0DF3 );
      ADD_CONST_INT( "GL_SELECTION_BUFFER_SIZE", 0x0DF4 );

      ADD_CONST_INT( "GL_UNPACK_SWAP_BYTES", 0x0CF0 );
      ADD_CONST_INT( "GL_UNPACK_LSB_FIRST", 0x0CF1 );
      ADD_CONST_INT( "GL_UNPACK_ROW_LENGTH", 0x0CF2 );
      ADD_CONST_INT( "GL_UNPACK_SKIP_ROWS", 0x0CF3 );
      ADD_CONST_INT( "GL_UNPACK_SKIP_PIXELS", 0x0CF4 );
      ADD_CONST_INT( "GL_UNPACK_ALIGNMENT", 0x0CF5 );
      ADD_CONST_INT( "GL_PACK_SWAP_BYTES", 0x0D00 );
      ADD_CONST_INT( "GL_PACK_LSB_FIRST", 0x0D01 );
      ADD_CONST_INT( "GL_PACK_ROW_LENGTH", 0x0D02 );
      ADD_CONST_INT( "GL_PACK_SKIP_ROWS", 0x0D03 );
      ADD_CONST_INT( "GL_PACK_SKIP_PIXELS", 0x0D04 );
      ADD_CONST_INT( "GL_PACK_ALIGNMENT", 0x0D05 );

      /* PixelFormat */
      ADD_CONST_INT( "GL_COLOR_INDEX", 0x1900 );
      ADD_CONST_INT( "GL_STENCIL_INDEX", 0x1901 );
      ADD_CONST_INT( "GL_DEPTH_COMPONENT", 0x1902 );
      ADD_CONST_INT( "GL_RED", 0x1903 );
      ADD_CONST_INT( "GL_GREEN", 0x1904 );
      ADD_CONST_INT( "GL_BLUE", 0x1905 );
      ADD_CONST_INT( "GL_ALPHA", 0x1906 );
      ADD_CONST_INT( "GL_RGB", 0x1907 );
      ADD_CONST_INT( "GL_RGBA", 0x1908 );
      ADD_CONST_INT( "GL_LUMINANCE", 0x1909 );
      ADD_CONST_INT( "GL_LUMINANCE_ALPHA", 0x190A );

      /* TextureMagFilter */
      ADD_CONST_INT( "GL_NEAREST", 0x2600 );
      ADD_CONST_INT( "GL_LINEAR", 0x2601 );

      /* TextureMinFilter */
      ADD_CONST_INT( "GL_NEAREST_MIPMAP_NEAREST", 0x2700 );
      ADD_CONST_INT( "GL_LINEAR_MIPMAP_NEAREST", 0x2701 );
      ADD_CONST_INT( "GL_NEAREST_MIPMAP_LINEAR", 0x2702 );
      ADD_CONST_INT( "GL_LINEAR_MIPMAP_LINEAR", 0x2703 );

      /* TextureParameterName */
      ADD_CONST_INT( "GL_TEXTURE_MAG_FILTER", 0x2800 );
      ADD_CONST_INT( "GL_TEXTURE_MIN_FILTER", 0x2801 );
      ADD_CONST_INT( "GL_TEXTURE_WRAP_S", 0x2802 );
      ADD_CONST_INT( "GL_TEXTURE_WRAP_T", 0x2803 );
      ADD_CONST_INT( "GL_TEXTURE_BORDER_COLOR", 0x1004 );
/*      GL_TEXTURE_PRIORITY */
/*      GL_TEXTURE_WRAP_R */
/*      GL_TEXTURE_MIN_LOD */
/*      GL_TEXTURE_MAX_LOD */
/*      GL_TEXTURE_BASE_LEVEL */
/*      GL_TEXTURE_MAX_LEVEL */

/* TextureTarget */
/*      GL_TEXTURE_1D */
/*      GL_TEXTURE_2D */
/*      GL_PROXY_TEXTURE_1D */
/*      GL_PROXY_TEXTURE_2D */
/*      GL_TEXTURE_3D */
/*      GL_PROXY_TEXTURE_3D */

      /* TextureWrapMode */
      ADD_CONST_INT( "GL_CLAMP", 0x2900 );
      ADD_CONST_INT( "GL_REPEAT", 0x2901 );
/*      GL_CLAMP_TO_EDGE */

/* DrawBufferMode */
      ADD_CONST_INT( "GL_NONE", 0 );
      ADD_CONST_INT( "GL_FRONT_LEFT", 0x0400 );
      ADD_CONST_INT( "GL_FRONT_RIGHT", 0x0401 );
      ADD_CONST_INT( "GL_BACK_LEFT", 0x0402 );
      ADD_CONST_INT( "GL_BACK_RIGHT", 0x0403 );
      ADD_CONST_INT( "GL_FRONT", 0x0404 );
      ADD_CONST_INT( "GL_BACK", 0x0405 );
      ADD_CONST_INT( "GL_LEFT", 0x0406 );
      ADD_CONST_INT( "GL_RIGHT", 0x0407 );
      ADD_CONST_INT( "GL_FRONT_AND_BACK", 0x0408 );
      ADD_CONST_INT( "GL_AUX0", 0x0409 );
      ADD_CONST_INT( "GL_AUX1", 0x040A );
      ADD_CONST_INT( "GL_AUX2", 0x040B );
      ADD_CONST_INT( "GL_AUX3", 0x040C );
      
      // 
    }
    
    void *llvmResolveExternalFunction( std::string const &name )
    {
      FuncTable::const_iterator   it;
      it = llvmFuncTable.find( name );
      if( it != llvmFuncTable.end() )
        return( it->second );

      return 0;
    }
  };
};
