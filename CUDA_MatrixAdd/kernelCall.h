#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

enum ThreadLayout
{
	G1D_B1D = 0x00,
	G1D_B2D = 0x01,
	G2D_B1D = 0x10,
	G2D_B2D = 0x11
};

/**
Interface function for kernel call
*/
bool kernelCall
	(float *_MatA, float *_MatB, float *_MatC, int _nRow, int _nCol
	, int _layout, dim3 _griDim, dim3 _blockDim);