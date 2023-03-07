/**
This is an exmple code used in the OpenMP - lecture 3 <br>
@author : Duksu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
	//omp_set_nested(1);
	printf("Nested parallelism : %s\n"
		, omp_get_nested() ? "On" : "Off");

	#pragma omp parallel num_threads(4)
	{
		int parentID = omp_get_thread_num();
		printf("Lv 1 - Thread %d\n", parentID);

		#pragma omp parallel num_threads(2)
		{
			printf("\tLv 2 - Thread %d of %d\n", omp_get_thread_num(), parentID);
		}
	}
}