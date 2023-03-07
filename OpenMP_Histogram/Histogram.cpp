/**
This is a sample solution code for Lab 3. Histogram
@author : Duksu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#include "../DSTimer/DS_timer.h"
#include "../DSTimer/DS_definitions.h"

#include "Parameters.h"

float getRandNum();
void printResults(int* _bins);

int main(void)
{
	DS_timer timer(7);
	timer.setTimerName(0, "Serial\t\t");
	timer.setTimerName(1, "Version 1-1\t");
	timer.setTimerName(2, "Version 1-2\t");
	timer.setTimerName(3, "Version 2-1\t");
	timer.setTimerName(4, "Version 2-2\t");
	timer.setTimerName(5, "Version 3-1\t");
	timer.setTimerName(6, "Version 3-2\t");
	timer.initTimers();

	float* data = new float[NUM_ELE];

	#pragma omp parallel for num_threads(NUM_THREADS)
	LOOP_I(NUM_ELE)
		data[i] = getRandNum();

	//*** serial	
	int bin_serial[NUM_BINS] = { 0 };

	timer.onTimer(0);
	LOOP_I(NUM_ELE)
		bin_serial[(int)data[i]]++;
	timer.offTimer(0);

	printResults(bin_serial);

#ifdef ON_V1
	//*** version 1-1

	omp_lock_t glock;
	omp_init_lock(&glock);
	int bin_v1_1[NUM_BINS] = { 0 };

	timer.onTimer(1);
	#pragma omp parallel for num_threads(NUM_THREADS)
	LOOP_I(NUM_ELE)
	{
		omp_set_lock(&glock);	// use global lock
		bin_v1_1[(int)data[i]]++;
		omp_unset_lock(&glock);
	}
	timer.offTimer(1);
	printResults(bin_v1_1);

	//*** version 1-2
	omp_lock_t locks[NUM_BINS];
	LOOP_I(NUM_BINS)
		omp_init_lock(&locks[i]);

	int bin_v1_2[NUM_BINS] = { 0 };

	timer.onTimer(2);
	#pragma omp parallel for num_threads(NUM_THREADS)
	LOOP_I(NUM_ELE)
	{
		int binID = (int)data[i];
		omp_set_lock(&locks[binID]); // use locks for each bin
		bin_v1_2[binID]++;
		omp_unset_lock(&locks[binID]);
	}
	timer.offTimer(2);
	printResults(bin_v1_2);

#endif

	int localBins[NUM_THREADS][LOCAL_BIN_SIZE] = { 0 };

#ifdef ON_V2
	//*** version 2-1
	int bin_v2[NUM_BINS] = { 0 };

	timer.onTimer(3);
	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tID = omp_get_thread_num();

		#pragma omp for
		LOOP_I(NUM_ELE)
		{
			localBins[tID][(int)data[i]]++;
		}

		#pragma omp for
		LOOP_I(NUM_THREADS)
			LOOP_J(NUM_BINS)
			{
				#pragma omp atomic
				bin_v2[j] += localBins[i][j];
			}
	}

	//LOOP_J_I(NUM_THREADS, NUM_BINS)
	//{
	//	bin_v2[i] += localBins[j][i];
	//}

	timer.offTimer(3);
	printResults(bin_v2);

	//*** version 2-2
	int bin_v2_2[NUM_BINS] = { 0 };
	memset(localBins, 0, sizeof(int)*NUM_THREADS*LOCAL_BIN_SIZE);

	timer.onTimer(4);
	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tID = omp_get_thread_num();

		#pragma omp for
		LOOP_I(NUM_ELE)
		{
			localBins[tID][(int)data[i]]++;
		}

		#pragma omp for
		LOOP_I(NUM_BINS)
		{
			LOOP_J(NUM_THREADS)
				bin_v2_2[i] += localBins[j][i];
		}
	}

	timer.offTimer(4);
	printResults(bin_v2_2);

#endif

#ifdef ON_V3
	//*** version 3-1

	memset(localBins, 0, sizeof(int) * NUM_THREADS * LOCAL_BIN_SIZE);

	timer.onTimer(5);
	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tID = omp_get_thread_num();

		#pragma omp for
		LOOP_I(NUM_ELE)
		{
			localBins[tID][(int)data[i]]++;
		}

		// global sum
		int offset = 1;
		while (offset < NUM_THREADS)
		{
			LOOP_I(NUM_BINS)
			{
				if (tID % (2 * offset) == 0)
					localBins[tID][i] += localBins[tID + offset][i];
			}

			#pragma omp barrier

			offset *= 2;
		}
	}
	timer.offTimer(5);
	printResults(localBins[0]);

	//*** version 3-2

	memset(localBins, 0, sizeof(int) * NUM_THREADS * LOCAL_BIN_SIZE);

	timer.onTimer(6);
	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tID = omp_get_thread_num();

		#pragma omp for
		LOOP_I(NUM_ELE)
		{
			localBins[tID][(int)data[i]]++;
		}

		// global sum
		int offset = 1;
		while (offset < NUM_THREADS)
		{
			if (tID % (2 * offset) == 0)
				for (int i = 0; i < NUM_BINS; i += 2)
					localBins[tID][i] += localBins[tID + offset][i];

			if (tID % (2 * offset) == (offset * 2 - 1))
				for (int i = 1; i < NUM_BINS; i += 2)
					localBins[tID][i] += localBins[tID - offset][i];

			#pragma omp barrier

			offset *= 2;
		}
	}

	for (int i = 1; i < NUM_BINS; i += 2)
	{	// collect resutls of odd threads to master
		localBins[0][i] = localBins[NUM_THREADS - 1][i];
	}

	timer.offTimer(6);
	printResults(localBins[0]);
#endif

	timer.printTimer();

	return 0;
}

float getRandNum()
{
	return (rand() % NUM_BINS + (rand() % 100) / 100.0f);
}

void printResults(int* _bins)
{
	LOOP_I(NUM_BINS)
		printf("%d ", _bins[i]);
	printf("\n");
}