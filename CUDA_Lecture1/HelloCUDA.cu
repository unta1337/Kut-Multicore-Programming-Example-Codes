/**
This is an exmple code used in the CUDA Lecture 1 (Quick Lab. 8-1) <br>
@author : Duksu Kim
*/

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "../DSTimer/DS_Timer.h"
#include <stdio.h>

__global__ void helloCUDA(void)
{
	printf("Hello CUDA from GPU!\n");
}

int main(void)
{
	printf("Hello GPU from CPU!\n");

	helloCUDA <<<1, 10 >>>();

	return 0;
}
