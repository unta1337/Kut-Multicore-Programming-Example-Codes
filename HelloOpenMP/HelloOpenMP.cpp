/**
This is a solution code for Lab 1-1. Hello OpenMP!
@author : Duksu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	int numThreads = atoi(argv[1]);

	#pragma omp parallel num_threads(numThreads)
	{
		printf("[Thread %d/%d] Hello OpenMP!\n"
			, omp_get_thread_num(), omp_get_num_threads());
	}
}