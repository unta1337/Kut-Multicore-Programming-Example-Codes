// On building ...

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "helper_cuda.h"

#include "../DSTimer/DS_Timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOOP_I(_size) for(int i = 0; i<_size; i++)

__global__ void histogram(int *a, volatile int *histo, int n) {
	int tid = blockIdx.x * blockDim.x + threadIdx.x;
	__shared__ int localHist[256];

	if (tid >= n) return;

	//histo[a[tid]]++;

	localHist[a[tid]]++;
	__syncthreads();

	if (threadIdx.x < 256)
		histo[threadIdx.x] = localHist[threadIdx.x];
}

__global__ void histogram_atomic(int *a, int *histo, int n) {
	int tid = blockIdx.x * blockDim.x + threadIdx.x;
	if (tid >= n) return;
	atomicAdd(histo + a[tid], 1);
}

__global__ void histogram_shared(int *a, int *histo, int n) {
	int tid = blockIdx.x * blockDim.x + threadIdx.x;

	__shared__ int sh[256];
	if (threadIdx.x < 256) sh[threadIdx.x] = 0;
	__syncthreads();

	if (tid < n) atomicAdd(&sh[a[tid]], 1);
	__syncthreads();

	if (threadIdx.x < 256) atomicAdd(&histo[threadIdx.x], sh[threadIdx.x]);
}

int main(void) {
	DS_timer timer(10);
	timer.initTimers();
	timer.setTimerName(1, "[No Sync.]");
	timer.setTimerName(2, "[Global Sync.]");
	timer.setTimerName(3, "[Shared Sync. -> Global Sync.]");
	timer.setTimerName(8, "- Host -> Device");
	timer.setTimerName(9, "- Devic -> Host");
	timer.setTimerName(0, "[CPU]");

	const int N = 1024 * 1024 * 1024;
	int *a, histo[256];
	a = (int *)malloc(sizeof(int) * N);
	for (int i = 0; i<N; i++)
		a[i] = rand() % 256;

	for (int i = 0; i<256; i++) histo[i] = 0;

	timer.onTimer(0);
	LOOP_I(N)
	{
		histo[a[i]]++;
	}
	timer.offTimer(0);

	int *da, *dh;

	cudaMalloc((void **)&dh, sizeof(int) * 256);
	cudaMalloc((void **)&da, sizeof(int)*N);

	timer.onTimer(8);
	cudaMemcpy(da, a, sizeof(int)*N, cudaMemcpyHostToDevice);
	cudaMemset(dh, 0, sizeof(int) * 256);
	timer.offTimer(8);

	//timer.onTimer(1);
	//histogram<< <N / 1024, 1024 >> >(da, dh, N);
	//cudaDeviceSynchronize();
	//timer.offTimer(1);

	cudaMemset(dh, 0, sizeof(int) * 256);
	timer.onTimer(2);
	histogram_atomic << <N / 1024, 1024 >> >(da, dh, N);
	cudaDeviceSynchronize();
	timer.offTimer(2);

	cudaMemset(dh, 0, sizeof(int) * 256);
	timer.onTimer(3);
	histogram_shared << <N / 1024, 1024 >> >(da, dh, N);
	cudaDeviceSynchronize();
	timer.offTimer(3);

	timer.onTimer(9);
	cudaMemcpy(histo, dh, sizeof(int) * 256, cudaMemcpyDeviceToHost);
	timer.offTimer(9);


	//for (int i = 0; i<256; i++) {
	//	printf("histo[%d] = %d, ", i, histo[i]);
	//	if (i % 4 == 3) printf("\n");
	//}

	free(a);
	cudaFree(da);
	cudaFree(dh);

	timer.printTimer();

	return 0;
}