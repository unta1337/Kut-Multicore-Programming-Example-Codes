/**
This code is from Professoinal CUDA C Programming, NVIDIA <br>
This is an exmple code used in the CUDA Lecture 1 (Quick Lab. 8-3) <br>
*/

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void checkIndex(void) {
	printf("threadIdx:(%d, %d, %d) blockIdx:(%d, %d, %d) blockDim:(%d, %d, %d) gridDim:(%d, %d, %d)\n"
		, threadIdx.x, threadIdx.y, threadIdx.z
		, blockIdx.x, blockIdx.y, blockIdx.z
		, blockDim.x, blockDim.y, blockDim.z
		, gridDim.x, gridDim.y, gridDim.z);
}

int main(void)
{
	int nElem = 6;

	dim3 block(3);
	dim3 grid((nElem + block.x - 1) / block.x);

	printf("grid.x %d grid.y %d grid.z %d\n", grid.x, grid.y, grid.z);
	printf("block.x %d block.y %d block.z %d\n", block.x, block.y, block.z);

	checkIndex <<< grid, block >>> ();
	cudaDeviceReset();
	return(0);
}
