/**
This is an exmple code used in the OpenMP - lecture 2 <br>
@author : Duksu Kim and Young-Gyu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../DSTimer/DS_timer.h"

int main()
{
	int tID = 0;
	#pragma omp parallel private (tID)
	{
		tID = omp_get_thread_num();
		if (tID % 2 == 0)
			Sleep(10);

		printf("[%d] before\n", tID);

		#pragma omp barrier
		printf("[%d] after\n", tID);
	}

	getchar();
}
