__kernel void vector_add_kernel(    __global int* restrict inputA,
                                    __global int* restrict inputB,
                                    __global int* restrict output)
{
    /*
    * Set i to be the ID of the kernel instance.
    * If the global work size (set by clEnqueueNDRangeKernel) is n,
    * then n kernels will be run and i will be in the range [0, n - 1].
    */
    int i = get_global_id(0);
    /* Use i as an index into the three arrays. */

    output[i] = inputA[i] + inputB[i]; 
 }

