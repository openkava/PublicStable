function entry()
{
  var Integer err;

  var cl_platform_id clPlatformID; // NULL by default since it's opaque
  var cl_device_id[] clDeviceIDs;
  err = clGetDeviceIDs( clPlatformID, CL_DEVICE_TYPE_ALL, 16, clDeviceIDs );
  report "clGetDeviceIDs: " + err;
  for ( var Integer i=0; i<clDeviceIDs.size; ++i )
    report "clDeviceIDs["+i+"]: "+clDeviceIDs[i];

  var cl_context clContext = clCreateContext( clDeviceIDs, err );
  report "clCreateContext: " + err;
  report "clContext: " + clContext;

  var cl_command_queue clCommandQueue = clCreateCommandQueue( clContext, clDeviceIDs[0], 0, err );
  report "clCreateCommandQueue: " + err;
  report "clCommandQueue: " + clCommandQueue;

  var String source = "\n\
__kernel void square(                                                  \n\
   __global float* input,                                              \n\
   __global float* output,                                             \n\
   const unsigned int count)                                           \n\
{                                                                      \n\
   int i = get_global_id(0);                                           \n\
   if(i < count)                                                       \n\
       output[i] = input[i] * input[i];                                \n\
}                                                                      \n\
";
  var cl_program clProgram = clCreateProgramWithSource( clContext, source, err );
  report "clCreateProgramWithSource: " + err;
  report "clProgram: " + clProgram;

  err = clBuildProgram( clProgram, clDeviceIDs, "" );
  report "clBuildProgram: " + err;

  var cl_kernel clKernel = clCreateKernel( clProgram, "square", err );
  report "clCreateKernel returned " + err;
  report "clKernel = " + clKernel;

  var Data NULL;
  var Size count = 1024;

  var Scalar[] data;
  for ( var Size i=0; i<count; ++i )
    data.push( 5.6*i+2.4 );
  var cl_mem input = clCreateBuffer( clContext, CL_MEM_READ_ONLY, data.dataSize, NULL, err );
  report "clCreateBuffer returned " + err;
  report "input = " + input;

  var Scalar[] results;
  results.resize( count );
  var cl_mem output = clCreateBuffer( clContext, CL_MEM_WRITE_ONLY, results.dataSize, NULL, err );
  report "clCreateBuffer returned " + err;
  report "output = " + output;

  var cl_event[] eventWaitList;
  var cl_event event;
  err = clEnqueueWriteBuffer( clCommandQueue, input, CL_TRUE, 0, data.dataSize, data.data, eventWaitList, event );
  report "clEnqueueWriteBuffer returned " + err;
  report "event = " + event;

  err = clSetKernelArg( clKernel, 0, input.dataSize, input.data )
    | clSetKernelArg( clKernel, 1, output.dataSize, output.data )
    | clSetKernelArg( clKernel, 2, count.dataSize, count.data );
  report "clSetKernelArg returned " + err;

  var Size local;
  err = clGetKernelWorkGroupInfo( clKernel, clDeviceIDs[0], CL_KERNEL_WORK_GROUP_SIZE, local.dataSize, local.data, NULL );
  report "clGetKernelWorkGroupInfo returned " + err;
  report "local = " + local;

  var Size global = count;
  err = clEnqueueNDRangeKernel( clCommandQueue, clKernel, 1, NULL, global.data, local.data, eventWaitList, event );
  report "clEnqueueNDRangeKernel returned " + err;
  report "event = " + event;

  err = clFinish( clCommandQueue );
  report "clFinish returned " + err;

  err = clEnqueueReadBuffer( clCommandQueue, output, CL_TRUE, 0, results.dataSize, results.data, eventWaitList, event );
  report "clEnqueueReadBuffer returned " + err;
  report "event = " + event;

  var Size correctCount = 0;
  for ( var Size i=0; i<count; ++i )
  {
    if ( results[i] == data[i] * data[i] )
      ++correctCount;
  }
  report "correctCount = " + correctCount;

  err = clReleaseMemObject( output )
    | clReleaseMemObject( input );
  report "clReleaseMemObject returned " + err;
}
