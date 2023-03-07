/**
This is an exmple code used in the OpenMP - lecture 1 <br>
@author : Duksu Kim and Young-Gyu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../DSTimer/DS_timer.h"

#define numThreads (8)

int main()
{
	int a ; int b[10] = { 0 };
		
	#pragma omp parallel 
	{
		#pragma omp single nowait
		{
			Sleep(1);
			a = 10;
			printf("Single construct excuted by thread %d\n", omp_get_thread_num());
		}

		#pragma omp for
		for (int i = 0; i < 10; i++)
			b[i] = a;
	}

	for (int i = 0; i < 10; i++)
		printf("b[%d] = %d\n", i, b[i]);

	getchar();
}
