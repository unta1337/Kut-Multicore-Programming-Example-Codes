#include "kernelCall.h"
#include "../DSTimer/DS_Timer.h"

#define ROW_SIZE (8192)
#define COL_SIZE (8192)
#define MAT_SIZE (ROW_SIZE*COL_SIZE)

#define ID2INDEX(_row,_col) (_row*COL_SIZE + _col)

bool MatAddGPU_2D2D(float *_dA, float *_dB, float *_dC);
bool MatAddGPU_1D1D(float *_dA, float *_dB, float *_dC);
bool MatAddGPU_2D1D(float *_dA, float *_dB, float *_dC);

int main(void)
{
	DS_timer timer(8);
	timer.setTimerName(0, "CUDA Total");
	timer.setTimerName(1, "Computation(Kernel)");
	timer.setTimerName(2, "Data Trans. : Host -> Device");
	timer.setTimerName(3, "Data Trans. : Device -> Host");
	timer.setTimerName(4, "Host Performance");

	timer.setTimerName(5, "Kernel 2D-2D");
	timer.setTimerName(6, "Kernel 1D-1D");
	timer.setTimerName(7, "Kernel 2D-1D");
	timer.initTimers();

	float *A, *B, *C, *hC;
	float *dA, *dB, *dC;

	//// host memory allocation
	A = new float[MAT_SIZE]; memset(A, 0, sizeof(float)*MAT_SIZE);
	B = new float[MAT_SIZE]; memset(B, 0, sizeof(float)*MAT_SIZE);
	C = new float[MAT_SIZE]; memset(C, 0, sizeof(float)*MAT_SIZE);
	hC = new float[MAT_SIZE]; memset(hC, 0, sizeof(float)*MAT_SIZE);

	// device memory allocation
	cudaMalloc(&dA, sizeof(float)*MAT_SIZE); cudaMemset(dA, 0, sizeof(float)*MAT_SIZE);
	cudaMalloc(&dB, sizeof(float)*MAT_SIZE); cudaMemset(dB, 0, sizeof(float)*MAT_SIZE);
	cudaMalloc(&dC, sizeof(float)*MAT_SIZE); cudaMemset(dC, 0, sizeof(float)*MAT_SIZE);

	// input matrix generation
	for (int i = 0; i < MAT_SIZE; i++) {
		A[i] = rand() % 100;
		B[i] = rand() % 100;
	}

	// Check host performamce
	timer.onTimer(4);
	for (int row = 0; row < ROW_SIZE; row++) {
		for (int col = 0; col < COL_SIZE; col++) {
			hC[ID2INDEX(row,col)] = A[ID2INDEX(row, col)] + B[ID2INDEX(row, col)];
		}
	}
	timer.offTimer(4);

	// copy the input matrices from host memory to device memory
	timer.onTimer(2);
	cudaMemcpy(dA, A, sizeof(float)*MAT_SIZE, cudaMemcpyHostToDevice);
	cudaMemcpy(dB, B, sizeof(float)*MAT_SIZE, cudaMemcpyHostToDevice);
	timer.offTimer(2);

	// *** Kernel call

	timer.onTimer(5);
	MatAddGPU_2D2D(dA, dB, dC); cudaDeviceSynchronize();
	timer.offTimer(5);

	//timer.onTimer(6);
	//MatAddGPU_1D1D(dA, dB, dC); cudaDeviceSynchronize();
	//timer.offTimer(6);

	//timer.onTimer(7);
	//MatAddGPU_2D1D(dA, dB, dC); cudaDeviceSynchronize();
	//timer.offTimer(7);

	// ***

	// copy results from device memory to host memory
	timer.onTimer(3);
	cudaMemcpy(C, dC, sizeof(float)*MAT_SIZE, cudaMemcpyDeviceToHost);
	timer.offTimer(3);

	// validation
	bool isCorrect = true;
	for (int i = 0; i < MAT_SIZE; i++) {
		if (hC[i] != C[i]) {
			isCorrect = false;
			break;
		}
	}

	if (isCorrect) printf("GPU works well!\n");
	else printf("GPU fail to make correct resutl(s)..\n");

	timer.printTimer();
	return 0;
}

bool MatAddGPU_2D2D(float *_dA, float *_dB, float *_dC)
{
	dim3 blockDim(32, 32);
	dim3 gridDim(ceil((float)COL_SIZE / blockDim.x)
				, ceil((float)ROW_SIZE / blockDim.y));
	return kernelCall(_dA, _dB, _dC, ROW_SIZE, COL_SIZE
					, ThreadLayout::G2D_B2D, gridDim, blockDim);
}

bool MatAddGPU_1D1D(float *_dA, float *_dB, float *_dC)
{
	dim3 blockDim(32);
	dim3 gridDim(ceil((float)COL_SIZE / blockDim.x));
	return kernelCall(_dA, _dB, _dC, ROW_SIZE, COL_SIZE
					, ThreadLayout::G1D_B1D, gridDim, blockDim);
}

bool MatAddGPU_2D1D(float *_dA, float *_dB, float *_dC)
{
	dim3 blockDim(32);
	dim3 gridDim(ceil((float)COL_SIZE / blockDim.x)
				, ROW_SIZE);
	return kernelCall(_dA, _dB, _dC, ROW_SIZE, COL_SIZE
					, ThreadLayout::G2D_B1D, gridDim, blockDim);

}