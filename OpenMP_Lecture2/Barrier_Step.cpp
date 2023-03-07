/**
This is an exmple code used in the OpenMP - lecture 2 <br>
@author : Duksu Kim and Young-Gyu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../DSTimer/DS_timer.h"

#define NUM_THREADS (4)
#define SIZE (16)

int main()
{
	int a[NUM_THREADS] = { 0 };
	int b[SIZE] = { 0 };

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tID = omp_get_thread_num();

		a[tID] = tID * 10;
		printf("a : %2d tID : %2d \n", a[tID], tID);

		#pragma omp barrier
		
		#pragma omp for
		for (int i = 0; i < SIZE; i++)
			b[i] = 2 * a[(i+1) % NUM_THREADS];
	}

	for (int j = 0; j < SIZE; j++)
		printf("b[%2d] b : %2d  \n",j, b[j]);
	
	getchar();
}
