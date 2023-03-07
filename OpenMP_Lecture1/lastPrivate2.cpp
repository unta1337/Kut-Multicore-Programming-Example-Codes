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
	int tid = 0;
	int priVar;

	#pragma omp parallel for num_threads(4) private(tid) lastprivate(priVar)
	for (int i = 0; i < 16; i++)
	{
		tid = omp_get_thread_num();
		priVar = tid * 10 * i;
		printf("[Thread %d, loop %d] priVar = %d\n", i, tid, priVar);
	}
	printf("[After parallel region] priVar = %d\n", priVar);

	getchar();
}
