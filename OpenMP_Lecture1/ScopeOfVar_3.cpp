/**
This is an exmple code used in the OpenMP - lecture 1 <br>
@author : Duksu Kim and Young-Gyu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../DSTimer/DS_timer.h"

#define numThreads (4)
#define NUM (134217728)

int main()
{
	DS_timer timer(2);
	timer.initTimers();
	
	int *A = new int[NUM];
	int *B = new int[NUM];
	int *C = new int[NUM];

	for (int i = 0; i < NUM; i++) {
		A[i] = (int)(rand() % 10);
		B[i] = (int)(rand() % 10);
	}

	#pragma omp parallel num_threads(numThreads)
	{
		int m = (NUM / omp_get_num_threads());
		
		int st = m*omp_get_thread_num();
		int end = m*(omp_get_thread_num() + 1);
		printf("%d ~ %d\n", st, end);

		timer.onTimer(1);
		for (int i = st; i<end; i++)
			C[i] = (A[i] + B[i]);
		timer.offTimer(1);
	}

	timer.printTimer();

	delete A;
	delete B;
	delete C;
	
	getchar();
}
