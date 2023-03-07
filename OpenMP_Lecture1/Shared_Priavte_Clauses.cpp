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
	
	int a = 0;
	int re[4] = { 0 };

	#pragma omp parallel for num_threads(4) shared(re) private(a)
	for (int i = 0; i < 4; i++)
	{
		a = i;
		a = a*a;
		re[i] = a;
	}

	for (int i = 0; i < 4; i++)
		printf("[Thread %d] a = %d\n", i, re[i]);

	
	getchar();
}
