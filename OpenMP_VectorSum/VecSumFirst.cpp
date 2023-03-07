/**
This is a sample solution code for Lab 1-2. Vector sum
@author : Duksu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../DSTimer/DS_timer.h"
#include "../DSTimer/DS_definitions.h"

#define VTYPE int
#define VECTOR_SIZE (128*1024*1024)

int main(void)
{
	DS_timer timer(10);
	timer.initTimers();
	
	timer.setTimerName(0, "Serial");

	VTYPE *a = new VTYPE[VECTOR_SIZE];
	VTYPE *b = new VTYPE[VECTOR_SIZE];
	VTYPE *c = new VTYPE[VECTOR_SIZE];
	VTYPE *sol = new VTYPE[VECTOR_SIZE];

	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		a[i] = rand() % 1000;
		b[i] = rand() % 1000;
	}

	// serial vesrion
	timer.onTimer(0);
	for (int i = 0; i < VECTOR_SIZE; i++) {
		sol[i] = a[i] + b[i];
	}
	timer.offTimer(0);

	char timerName[255] = { 0 };
	for (int numThreads = 2; numThreads <= 4; numThreads *= 2)
	{
		printf("* Using %d thread(s)\n", numThreads);
		sprintf_s(timerName, "%d threads\0", numThreads);
		timer.setTimerName(numThreads, timerName);

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

		timer.onTimer(numThreads);
		#pragma omp parallel num_threads(numThreads)
		{ // parllel region
			int tID = omp_get_thread_num();
			for (int i = start[tID]; i < end[tID]; i++) {
				c[i] = a[i] + b[i];
			}
		}
		timer.offTimer(numThreads);

		// check results
		bool check = true;
		LOOP_I(VECTOR_SIZE) {
			if (c[i] != sol[i]) {
				check = false;
				break;
			}
		}
		if (!check) {
			printf("With %d threads, the resut is not corrent!!!\n", numThreads);
		}
		else {
			printf("With %d threads, it works well\n", numThreads);
		}

		delete start;
		delete end;
	}

	timer.printTimer();
	
	delete a;
	delete b;
	delete c;
	delete sol;
}