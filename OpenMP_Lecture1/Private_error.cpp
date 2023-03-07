/**
This is an exmple code used in the OpenMP - lecture 1 <br>
@author : Duksu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void)
{
	int i = 10;

	// i is uninitialized!
	#pragma omp parallel private(i)
	{
		printf("thread %d: i = %d\n", omp_get_thread_num(), i);
		i = 1000 + omp_get_thread_num();
	}

	printf("i = %d\n", i);

	return 0;
}
