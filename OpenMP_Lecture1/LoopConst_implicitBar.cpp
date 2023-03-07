/**
This is an exmple code used in the OpenMP - lecture 1 <br>
@author : Duksu Kim and Young-Gyu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../DSTimer/DS_timer.h"

#define VECTOR_SIZE (1000)
#define numThreads (4)

int main()
{

	int *a = new int[VECTOR_SIZE];
	int *b = new int[VECTOR_SIZE];

	#pragma omp parallel num_threads(numThreads)
	{
		#pragma omp for
		for (int i = 0; i < VECTOR_SIZE; i++)
			a[i] = i;

		// implicit barrier here

		#pragma omp for
		for (int i = 0; i < VECTOR_SIZE; i++)
			b[i] = 2 * a[i];
	}


	delete a; delete b;

	getchar();
	
}
