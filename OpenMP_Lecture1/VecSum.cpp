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

	int numElePerThread = VECTOR_SIZE / numThreads;
	int *start = new int[numThreads];
	int *end = new int[numThreads];

	printf("Work decomposition\n");
	for (int tID = 0; tID < numThreads; tID++) {
		start[tID] = numElePerThread * tID;
		end[tID] = numElePerThread * (tID + 1);

		if (tID == numThreads - 1) // for the last thread
			end[numThreads - 1] = VECTOR_SIZE;

		printf("\t[T%d] %d ~ %d\n", tID, start[tID], end[tID]);
	}

	timer.onTimer(0);

	#pragma omp parallel num_threads(numThreads)
	{ // parllel region
		int tID = omp_get_thread_num();
		for (int i = start[tID]; i < end[tID]; i++)
			c[i] = a[i] + b[i];
	}

	timer.offTimer(0);

	timer.printTimer();

	getchar();
	
	delete start; delete end;
		
}
