// OpenCL_thunk.cpp
// *****************************************************************************
// (C) 2011 Fabric Technologies, Inc.

#include <Fabric/Base/Config.h>

#if defined( FABRIC_OS_WINDOWS )
# include <windows.h>
# define FABRIC_OCL_THUNKS
#elif defined( FABRIC_OS_MACOSX )
# undef FABRIC_OCL_THUNKS
#else
#error "OpenCL thunks not supported"
#endif

#if defined( FABRIC_OCL_THUNKS )

#include <CL/opencl.h>

static void *loadSymbolRaw( const char *symname )
{
#if defined( FABRIC_OS_WINDOWS )
  static HMODULE    hCLDLL = 0;
  static HMODULE    hModuleNotFound = (HMODULE)INVALID_HANDLE_VALUE;

  if( hCLDLL == hModuleNotFound )
    return 0;

  if( !hCLDLL )
  {
#if defined(WIN64)
    hCLDLL = LoadLibraryA( "OpenCL64.dll" );
#else
    hCLDLL = LoadLibraryA( "OpenCL.dll" );
#endif
    if( hCLDLL == 0 )
    {
      hCLDLL = hModuleNotFound;
      return 0;
    }
  }

  return (void *)GetProcAddress( hCLDLL, symname );
#endif

  return 0;
}

template<typename T>
static T loadSymbol( const char *name )
{
  static T func = 0;
  static T unknownFunc = (T)(intptr_t)1;
  if( func == unknownFunc )
    return( 0 );
  
  if( func == 0 )
  {
    func = (T)loadSymbolRaw( name );
    if( func  == 0 )
    {
      func = unknownFunc;
      return 0;
    }
  }

  return func;
}

extern "C" 
{
#define ERR_ARG(_arg_) \
  if( _arg_ ) *_arg_ = CL_OUT_OF_RESOURCES; \
  return 0;

#define ERR_RET                         \
  return CL_OUT_OF_RESOURCES;

#define CL_IMPL_BASE( _name_, _err_, _return_, _args_, _call_ )   \
  _return_ CL_API_CALL _name_ _args_                      \
  {                                                       \
    typedef _return_ (CL_API_CALL *t_##_name_)_args_;     \
    t_##_name_ func = loadSymbol<t_##_name_>( #_name_ );  \
    if( func == 0 ) { _err_ }                             \
    return( func _call_ );                                \
  }

#define CL_IMPL_0( _name_, _err_, _return_, _args_ ) CL_IMPL_BASE( _name_, _err_, _return_, _args_, () )
#define CL_IMPL_1( _name_, _err_, _return_, _args_ ) CL_IMPL_BASE( _name_, _err_, _return_, _args_, (a0) )
#define CL_IMPL_2( _name_, _err_, _return_, _args_ ) CL_IMPL_BASE( _name_, _err_, _return_, _args_, (a0,a1) )
#define CL_IMPL_3( _name_, _err_, _return_, _args_ ) CL_IMPL_BASE( _name_, _err_, _return_, _args_, (a0,a1,a2) )
#define CL_IMPL_4( _name_, _err_, _return_, _args_ ) CL_IMPL_BASE( _name_, _err_, _return_, _args_, (a0,a1,a2,a3) )
#define CL_IMPL_5( _name_, _err_, _return_, _args_ ) CL_IMPL_BASE( _name_, _err_, _return_, _args_, (a0,a1,a2,a3,a4) )
#define CL_IMPL_6( _name_, _err_, _return_, _args_ ) CL_IMPL_BASE( _name_, _err_, _return_, _args_, (a0,a1,a2,a3,a4,a5) )
#define CL_IMPL_9( _name_, _err_, _return_, _args_ ) CL_IMPL_BASE( _name_, _err_, _return_, _args_, (a0,a1,a2,a3,a4,a5,a6,a7,a8) )

CL_IMPL_6( clBuildProgram, ERR_RET, cl_int, ( cl_program a0, cl_uint a1, const cl_device_id *a2, const char *a3, void (CL_CALLBACK *a4)(cl_program,void *),void *a5 ) )
CL_IMPL_5( clCreateBuffer, ERR_ARG(a4), cl_mem, (cl_context a0, cl_mem_flags a1, size_t a2, void *a3, cl_int *a4 ) )
CL_IMPL_6( clCreateContext, ERR_ARG(a5), cl_context, ( const cl_context_properties *a0, cl_uint a1, const cl_device_id *a2, void (CL_CALLBACK *a3)(const char *,const void*,size_t,void *),void *a4,cl_int *a5 ) )
CL_IMPL_4( clCreateCommandQueue, ERR_ARG(a3), cl_command_queue, ( cl_context a0, cl_device_id a1, cl_command_queue_properties a2,cl_int *a3 ) )
CL_IMPL_3( clCreateKernel, ERR_ARG(a2), cl_kernel, (cl_program a0, const char *a1, cl_int *a2 ) )
CL_IMPL_5( clCreateProgramWithSource, ERR_ARG(a4), cl_program, ( cl_context a0, cl_uint a1,const char **a2,const size_t *a3,cl_int *a4 ) )
CL_IMPL_9( clEnqueueNDRangeKernel, ERR_RET, cl_int, (cl_command_queue a0, cl_kernel a1, cl_uint a2, const size_t *a3, const size_t *a4, const size_t *a5, cl_uint a6, const cl_event *a7, cl_event *a8 ) )
CL_IMPL_9( clEnqueueReadBuffer, ERR_RET, cl_int, (cl_command_queue a0, cl_mem a1, cl_bool a2, size_t a3, size_t a4, void *a5, cl_uint a6, const cl_event *a7, cl_event *a8 ) )
CL_IMPL_9( clEnqueueWriteBuffer, ERR_RET, cl_int, (cl_command_queue a0, cl_mem a1, cl_bool a2, size_t a3, size_t a4, const void *a5, cl_uint a6, const cl_event *a7, cl_event *a8 ) )
CL_IMPL_1( clFinish, ERR_RET, cl_int, (cl_command_queue a0) )
CL_IMPL_5( clGetDeviceIDs, ERR_RET, cl_int, ( cl_platform_id a0, cl_device_type a1, cl_uint a2, cl_device_id *a3, cl_uint *a4 ) )
CL_IMPL_6( clGetKernelWorkGroupInfo, ERR_RET, cl_int, (cl_kernel a0, cl_device_id a1, cl_kernel_work_group_info a2, size_t a3, void *a4, size_t *a5 ) )
CL_IMPL_3( clGetPlatformIDs, ERR_RET, cl_int, (cl_uint a0, cl_platform_id *a1, cl_uint *a2))
CL_IMPL_1( clReleaseMemObject, ERR_RET, cl_int, (cl_mem a0) )
CL_IMPL_4( clSetKernelArg, ERR_RET, cl_int, (cl_kernel a0, cl_uint a1, size_t a2, const void *a3 ) )
CL_IMPL_6( clGetProgramBuildInfo, ERR_RET, cl_int, (cl_program a0, cl_device_id a1, cl_program_build_info a2, size_t a3, void *a4, size_t *a5) );

};

#endif
