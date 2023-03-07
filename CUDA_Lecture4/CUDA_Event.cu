/**
This is an exmple code used in the CUDA Lecture 4 (Quick Lab. 11-3) <br>
@author : Duksu Kim
*/

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "helper_cuda.h"

#include "../DSTimer/DS_Timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOOP_I(_loop) for(int i=0; i < _loop; i++)

#define NUM_BLOCK (128*1024)
#define NUM_T_IN_B 1024
#define ARRAY_SIZE (NUM_T_IN_B*NUM_BLOCK)

#define NUM_STREAMS 4

__global__ void myKernel2(int *_in, int *_out)
{
	int tID = blockDim.x * blockIdx.x + threadIdx.x;

	int temp = 0;
	for (int i = 0; i < 250; i++) {
		temp = (temp + _in[tID] * 5) % 10;
	}
	_out[tID] = temp;

}

void main(void)
{
	DS_timer timer(1);
	timer.setTimerName(0, "Total");

	int *in = NULL, *out = NULL, *out2 = NULL;

	cudaMallocHost(&in, sizeof(int)*ARRAY_SIZE);
	memset(in, 0, sizeof(int)*ARRAY_SIZE);

	cudaMallocHost(&out, sizeof(int)*ARRAY_SIZE);
	memset(out, 0, sizeof(int)*ARRAY_SIZE);

	cudaMallocHost(&out2, sizeof(int)*ARRAY_SIZE);
	memset(out2, 0, sizeof(int)*ARRAY_SIZE);

	int *dIn, *dOut;
	cudaMalloc(&dIn, sizeof(int)*ARRAY_SIZE);
	cudaMalloc(&dOut, sizeof(int)*ARRAY_SIZE);

	LOOP_I(ARRAY_SIZE)
		in[i] = rand() % 10;

	// Multiple stream version
	cudaStream_t stream[NUM_STREAMS];
	cudaEvent_t start[NUM_STREAMS], end[NUM_STREAMS];

	LOOP_I(NUM_STREAMS) {
		cudaStreamCreate(&stream[i]);
		cudaEventCreate(&start[i]); cudaEventCreate(&end[i]);
	}

	int chunkSize = ARRAY_SIZE / NUM_STREAMS;

	timer.onTimer(0);
	//#pragma omp parallel for num_threads(NUM_STREAMS)
	LOOP_I(NUM_STREAMS)
	{
		int offset = chunkSize * i;
		cudaEventRecord(start[i], stream[i]);
		cudaMemcpyAsync(dIn + offset, in + offset, sizeof(int)*chunkSize, cudaMemcpyHostToDevice, stream[i]);
		myKernel2 << <NUM_BLOCK / NUM_STREAMS, NUM_T_IN_B, 0, stream[i] >> > (dIn + offset, dOut + offset);
		cudaMemcpyAsync(out2 + offset, dOut + offset, sizeof(int)*chunkSize, cudaMemcpyDeviceToHost, stream[i]);
		cudaEventRecord(end[i], stream[i]);
		//cudaStreamSynchronize(stream[i]);
	}

	cudaDeviceSynchronize();
	timer.offTimer(0);
	timer.printTimer();

	LOOP_I(NUM_STREAMS) {
		float time = 0;
		cudaEventElapsedTime(&time, start[i], end[i]);
		printf("Stream[%d] : %f ms\n", i, time);
	}

	LOOP_I(ARRAY_SIZE) {
		if (out[i] != out2[i])
			printf("!");
	}

	LOOP_I(NUM_STREAMS) {
		cudaStreamDestroy(stream[i]);
		cudaEventDestroy(start[i]);
		cudaEventDestroy(end[i]);
	}

	cudaFree(dIn);
	cudaFree(dOut);

	cudaFreeHost(in);
	cudaFreeHost(out);
	cudaFreeHost(out2);
}