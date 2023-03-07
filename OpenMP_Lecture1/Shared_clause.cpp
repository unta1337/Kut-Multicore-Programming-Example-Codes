/**
This is an exmple code used in the OpenMP - lecture 1 <br>
@author : Duksu Kim and Young-Gyu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../DSTimer/DS_timer.h"

#define numThreads (4)
#define VECTOR_SIZE (134217728)

int main()
{

	int *A = new int[VECTOR_SIZE];
	int *B = new int[VECTOR_SIZE];
	int *C = new int[VECTOR_SIZE];

	for (int i = 0; i < VECTOR_SIZE; i++) {
		A[i] = (int)(rand() % 10);
		B[i] = (int)(rand() % 10);
		C[i] = 0;
	}

	#pragma omp parallel num_threads(numThreads) shared(A,B,C)
	{
		#pragma omp for	
		for (int i = 0; i < VECTOR_SIZE; i++) {
			C[i] = A[i] + B[i];
		}

	}

	delete A;
	delete B;
	delete C;

	getchar();
}
