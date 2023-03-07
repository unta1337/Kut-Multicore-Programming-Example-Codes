/**
This is a sample solution code for Lab 2-1. Matrix-Vector Multiplicaatoin
@author : Duksu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../DSTimer/DS_timer.h"
#include "../DSTimer/DS_definitions.h"

#define m (10000)
#define n (10000)

#define GenFloat (rand() % 100 + ((float)(rand() % 100) / 100.0))
void genRandomInput();

float A[m][n];
float X[n];
float Y_serial[m];
float Y_parallel[m];

int main(int argc, char* argv[])
{
	DS_timer timer(2);
	timer.setTimerName(0, "Serial");
	timer.setTimerName(1, "Parallel");

	genRandomInput();

	// Serial code
	timer.onTimer(0);
	for (int i = 0; i < m; i++) {
		Y_serial[i] = 0.0;
		for (int j = 0; j < n; j++) {
			Y_serial[i] += A[i][j] * X[j];
		}
	}
	timer.offTimer(0);

	// Parallel code
	timer.onTimer(1);
	#pragma omp parallel for num_threads(4)
	for (int i = 0; i < m; i++) {
		Y_parallel[i] = 0.0;
		for (int j = 0; j < n; j++) {
			Y_parallel[i] += A[i][j] * X[j];
		}
	}
	timer.offTimer(1);

	// Check reulsts
	LOOP_I(m) {
		if (Y_serial[i] != Y_parallel[i]) {
			printf("Results are not matched :(");
			EXIT_WIHT_KEYPRESS;
		}
	}

	timer.printTimer();
	EXIT_WIHT_KEYPRESS;
}

void genRandomInput(void) {
	// A matrix
	LOOP_INDEX(row, m) {
		LOOP_INDEX(col, n) {
			A[row][col] = GenFloat;
		}
	}

	LOOP_I(n)
		X[i] = GenFloat;

	memset(Y_serial, 0, sizeof(float)*m);
	memset(Y_parallel, 0, sizeof(float)*m);
}