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

	#pragma omp parallel num_threads(numThreads)
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				printf("Section A executed by thread %d\n", omp_get_thread_num());
			}

			#pragma omp section
			{
				printf("Section B executed by thread %d\n", omp_get_thread_num());
			}
		}
	}

	getchar();
}
