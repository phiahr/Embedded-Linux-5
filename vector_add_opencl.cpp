/**
* Basic integer array addition implemented in OpenCL.
* 26/05/2020 Polytech Nice Sophia GSE5
* S. Bilavarn
* Based on ARM integer array addition example
* CPU: 22.33 ms
* GPU: 2.86447 ms
* SPU: 7.795508419
*/

//#include <...>
#include <stdlib.h>
#include <CL/cl.h>
#include <iostream>
#include "common.h"
#include <stdio.h>
#include <memory.h>
using namespace std;

int main(void) {
    /* VARIABLES DECLARATION */
    int i,j;
    //const int VECTOR_SIZE=1024*1024;
    const int VECTOR_SIZE=1024*1024; // 5 times larger array size
    //const int VECTOR_SIZE=256*256*256;
    void *va;
    void *vb;
    void *vc;
    int *A = new int[VECTOR_SIZE];
    int *B = new int[VECTOR_SIZE];
    int *C = new int[VECTOR_SIZE];

    for(i = 0; i < VECTOR_SIZE;i++){
        A[i] = i;
        B[i] = VECTOR_SIZE - i;
    }
    
    bool rc;
    int errcode_err;
    
    /* 1. SET UP OPENCL ENVIRONMENT: create context, command queue, program and kernel. */
    /* 1.a Create Context */
    /* Create an OpenCL context on a GPU on the first available platform. See createContext in common.h */
    cl_context context;
    rc = createContext(&context);
    if(rc==false) printf("error context");
    
    

    /* 1.b Create Command Queue */
    /* Create an OpenCL command queue for a given context. See create&commandQueue in common.h */
    cl_command_queue commandQueue;
    cl_device_id device;

    rc = createCommandQueue(context, &commandQueue, &device);
    if(rc==false) printf("error queue");
    
    /* 1.c Create Program */
    /* Create an OpenCL program from a given file and compile it. See createProgram in common.h */
    cl_program program;
    string kernelFileName = "vector_add_opencl.cl";
    createProgram(context, device, kernelFileName, &program);

    /* 1.d Create kernel */
    /* Create our OpenCL kernel for the kernel function. See clCreateKernel in OpenCL 1.2 Reference Pages */
    char* kernelName = "vector_add_kernel";
    cl_kernel kernel = clCreateKernel(program, kernelName, &errcode_err);
    if (errcode_err != CL_SUCCESS) 
    {
        cout << "Exit with error_code: " << errcode_err << endl;
        return 0;
    }    
    /* 2. SET UP MEMORY / DATA */
    /* 2.a Create memory buffers */
    /* Create 3 memory buffers for the input/output data. See clCreateBuffer in OpenCL 1.2 Reference Pages */
    
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE*sizeof(int), NULL, &errcode_err);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE*sizeof(int), NULL, &errcode_err);
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, VECTOR_SIZE*sizeof(int), NULL, &errcode_err);
    if (errcode_err != CL_SUCCESS) 
    {
        cout << "Exit with error_code: " << errcode_err << endl;
        return 0;
    }  

    /* 2.b Initialize the input data */
    /* Map the input buffers to pointers. See clEnqueueMapBuffer in OpenCL 1.2 Reference Pages */
    va = clEnqueueMapBuffer(commandQueue, bufferA, CL_TRUE, CL_MAP_READ, NULL, VECTOR_SIZE*sizeof(int), 0, NULL, NULL, &errcode_err);
    vb = clEnqueueMapBuffer(commandQueue, bufferB, CL_TRUE, CL_MAP_READ, NULL, VECTOR_SIZE*sizeof(int), 0, NULL, NULL, &errcode_err);
    if (errcode_err != CL_SUCCESS) 
    {
        cout << "ERRORCOExit with error_codeDE: " << errcode_err << endl;
        return 0;
    }  

    /* Fill the input data */
    //va = (void*)A;
    //vb = (void*)B;

    memcpy(va, A, VECTOR_SIZE*sizeof(int));
    memcpy(vb, B, VECTOR_SIZE*sizeof(int));

    /* Unmap the input data. See clEnqueueUnmapMemObject in OpenCL 1.2 Reference Pages */
    clEnqueueUnmapMemObject(commandQueue, bufferA, va, NULL, NULL, NULL);
    clEnqueueUnmapMemObject(commandQueue, bufferB, vb, NULL, NULL, NULL);

    /* 2.c Set the kernel arguments */
    /* Pass the 3 memory buffers to the kernel as arguments. See clSetKernelArg in OpenCL 1.2 Reference Pages */

    errcode_err = clSetKernelArg(kernel, 0,sizeof(cl_mem), &bufferA);
    if (errcode_err != CL_SUCCESS) 
    {
        cout << "Exit with error_code: " << errcode_err << endl;
        return 0;
    }  
    errcode_err = clSetKernelArg(kernel, 1,sizeof(cl_mem), &bufferB);
    if (errcode_err != CL_SUCCESS) 
    {
        cout << "Exit with error_code: " << errcode_err << endl;
        return 0;
    }  
    errcode_err = clSetKernelArg(kernel, 2,sizeof(cl_mem), &bufferC);
    if (errcode_err != CL_SUCCESS) 
    {
        cout << "Exit with error_code: " << errcode_err << endl;
        return 0;
    }  
    /* 3. EXECUTE THE KERNEL INSTANCES */
    /* 3.a Define the global work size and enqueue the kernel. See clEnqueueNDRangeKernel in OpenCL 1.2 Reference Pages */
    /* Each instance of our OpenCL kernel operates on a single element of each array so the number of instances needed is the number of
    elements in the array. */
    size_t globalSize = VECTOR_SIZE;
    size_t localSize = 1;
    cl_event event;

    clEnqueueNDRangeKernel(commandQueue, kernel, 1, 0, &globalSize, &localSize,0, NULL, &event);
    /* 3.b Wait for kernel execution completion */
    /* See clFinish in OpenCL 1.2 Reference Pages */
    clFinish(commandQueue);
    /* 4. AFTER EXECUTION */
    /* 4.a Retrieve results: */
    /* Map the output buffer to a local pointer. See clEnqueueMapBuffer in OpenCL 1.2 Reference Pages */

    vc = clEnqueueMapBuffer(commandQueue, bufferC, CL_TRUE,CL_MAP_WRITE, 0, VECTOR_SIZE*sizeof(int), 0, NULL, NULL, &errcode_err);
    
    cl_ulong start;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
    cl_ulong end;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
    
    C = (int*) vc;

    cout << "err: " << errcode_err << endl;

    /* Read the results using the mapped pointer */
    for (int i = 0; i < 100 /*VECTOR_SIZE*/; i++) cout << A[VECTOR_SIZE-100+i] << " + " << B[VECTOR_SIZE-100+i] << " = " << C[i] << "\n";
    double time = end - start;
    time = time/1000000;
    cout << "execution time: " << time << "ms" << endl;

    /* Unmap the output data. See clEnqueueUnmapMemObject in OpenCL 1.2 Reference Pages */
    clEnqueueUnmapMemObject(commandQueue, bufferC, vc, 0, NULL, NULL);
    /* 4.b Release OpenCL objects */
    /* See cleanUpOpenCL in common.h */
    cleanUpOpenCL(context, commandQueue, program, kernel, &bufferC, 1);

    return 0;
}
