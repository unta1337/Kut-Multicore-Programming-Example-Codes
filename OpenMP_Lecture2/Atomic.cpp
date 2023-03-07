/**
This is an exmple code used in the OpenMP - lecture 2 <br>
@author : Duksu Kim and Young-Gyu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../DSTimer/DS_timer.h"
#include "../DSTimer/DS_definitions.h"

#define numThreads (4)

int main()
{
	DS_timer timer(2);
	int ic = 0;

	timer.onTimer(0);
	#pragma omp parallel num_threads(numThreads)
	{
		for (int i = 0; i < 10240; i++)
		{
			#pragma omp atomic
			ic += 1;
		}
	}
	timer.offTimer(0);
	printf("count = %d\n", ic);

	timer.printTimer();
	getchar();

	return 0;
}


