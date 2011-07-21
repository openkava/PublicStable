FC = createFabricClient();
F = wrapFabricClient(FC);

op = F.DG.createOperator("op");
op.setSourceCode("\
operator entry()\n\
{\n\
  Integer err;\n\
\n\
  cl_platform_id clPlatformIDs[];\n\
  err = clGetPlatformIDs( clPlatformIDs );\n\
  report 'clGetPlatformIDs: ' + err;\n\
  report 'clPlatformIDs.size > 0 = ' + (clPlatformIDs.size > 0);\n\
\n\
  cl_device_id clDeviceIDs[];\n\
  err = clGetDeviceIDs( clPlatformIDs[0], CL_DEVICE_TYPE_ALL, clDeviceIDs );\n\
  report 'clGetDeviceIDs: ' + err;\n\
  report 'clDeviceIDs.size > 0 = ' + (clDeviceIDs.size > 0);\n\
\n\
  cl_context clContext = clCreateContext( clDeviceIDs, err );\n\
  report 'clCreateContext: ' + err;\n\
  report 'clContext: ' + clContext;\n\
\n\
  cl_command_queue clCommandQueue = clCreateCommandQueue( clContext, clDeviceIDs[0], 0, err );\n\
  report 'clCreateCommandQueue: ' + err;\n\
  report 'clCommandQueue: ' + clCommandQueue;\n\
\n\
  String source = '\n\
__kernel void square(                                                  \n\
   __global float* input,                                              \n\
   __global float* output,                                             \n\
   const unsigned int count)                                           \n\
{                                                                      \n\
   int i = get_global_id(0);                                           \n\
   if(i < count)                                                       \n\
       output[i] = input[i] * input[i];                                \n\
}                                                                      \n\
';\n\
  cl_program clProgram = clCreateProgramWithSource( clContext, source, err );\n\
  report 'clCreateProgramWithSource: ' + err;\n\
  report 'clProgram: ' + clProgram;\n\
\n\
  err = clBuildProgram( clProgram, clDeviceIDs, '' );\n\
  report 'clBuildProgram: ' + err;\n\
\n\
  cl_kernel clKernel = clCreateKernel( clProgram, 'square', err );\n\
  report 'clCreateKernel returned ' + err;\n\
  report 'clKernel = ' + clKernel;\n\
\n\
  Data NULL;\n\
  Size count = 1024;\n\
\n\
  Scalar data[];\n\
  for ( Size i=0; i<count; ++i )\n\
    data.push( 5.6*i+2.4 );\n\
  cl_mem input = clCreateBuffer( clContext, CL_MEM_READ_ONLY, data.dataSize, NULL, err );\n\
  report 'clCreateBuffer returned ' + err;\n\
  report 'input = ' + input;\n\
\n\
  Scalar results[];\n\
  results.resize( count );\n\
  cl_mem output = clCreateBuffer( clContext, CL_MEM_WRITE_ONLY, results.dataSize, NULL, err );\n\
  report 'clCreateBuffer returned ' + err;\n\
  report 'output = ' + output;\n\
\n\
  cl_event eventWaitList[];\n\
  cl_event event;\n\
  err = clEnqueueWriteBuffer( clCommandQueue, input, CL_TRUE, 0, data.dataSize, data.data, eventWaitList, event );\n\
  report 'clEnqueueWriteBuffer returned ' + err;\n\
  report 'event = ' + event;\n\
\n\
  err = clSetKernelArg( clKernel, 0, input.dataSize, input.data )\n\
    | clSetKernelArg( clKernel, 1, output.dataSize, output.data )\n\
    | clSetKernelArg( clKernel, 2, count.dataSize, count.data );\n\
  report 'clSetKernelArg returned ' + err;\n\
\n\
  Size local;\n\
  err = clGetKernelWorkGroupInfo( clKernel, clDeviceIDs[0], CL_KERNEL_WORK_GROUP_SIZE, local.dataSize, local.data, NULL );\n\
  report 'clGetKernelWorkGroupInfo returned ' + err;\n\
  report 'local = ' + local;\n\
\n\
  Size global = count;\n\
  err = clEnqueueNDRangeKernel( clCommandQueue, clKernel, 1, NULL, global.data, local.data, eventWaitList, event );\n\
  report 'clEnqueueNDRangeKernel returned ' + err;\n\
  report 'event = ' + event;\n\
\n\
  err = clFinish( clCommandQueue );\n\
  report 'clFinish returned ' + err;\n\
\n\
  err = clEnqueueReadBuffer( clCommandQueue, output, CL_TRUE, 0, results.dataSize, results.data, eventWaitList, event );\n\
  report 'clEnqueueReadBuffer returned ' + err;\n\
  report 'event = ' + event;\n\
\n\
  Size correctCount = 0;\n\
  for ( Size i=0; i<count; ++i )\n\
  {\n\
    if ( results[i] == data[i] * data[i] )\n\
      ++correctCount;\n\
  }\n\
  report 'correctCount = ' + correctCount;\n\
\n\
  err = clReleaseMemObject( output )\n\
    | clReleaseMemObject( input );\n\
  report 'clReleaseMemObject returned ' + err;\n\
}\n\
");
op.setEntryFunctionName("entry");
if (op.getDiagnostics().length > 0) {
  printDeep(op.getDiagnostics());
}
else {
  b = F.DG.createBinding();
  b.setOperator(op);
  b.setParameterLayout([]);

  eh = F.DG.createEventHandler("eventHandler");
  eh.preDescendBindings.append(b);

  e = F.DG.createEvent("event");
  e.appendEventHandler(eh);
  e.fire();
}

F.flush();
FC.dispose();
