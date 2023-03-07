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
	DS_timer timer(1);
	timer.initTimers();

	int *a = new int[VECTOR_SIZE];
	int *b = new int[VECTOR_SIZE];
	int *c = new int[VECTOR_SIZE];

	for (int i = 0; i < VECTOR_SIZE; i++) {
		a[i] = (int)(rand() & 0xFF);
		b[i] = (int)(rand() & 0xFF);
	}

	printf(" * Using %d thread(s)\n", numThreads);

	timer.onTimer(0);

	#pragma omp parallel num_threads(numThreads)
	{
		#pragma omp for
		for (int i = 0; i < VECTOR_SIZE; i++) {
			c[i] = a[i] + b[i];
		}
	}

	timer.offTimer(0);

	timer.printTimer();

	getchar();

	delete a; delete b; delete c;

}
