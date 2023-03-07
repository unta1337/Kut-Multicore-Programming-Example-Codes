/**
This is an exmple code used in the OpenMP - lecture 3 <br>
@author : Duksu Kim and Young-Gyu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../DSTimer/DS_timer.h"
#include "../DSTimer/DS_definitions.h"

#define NUM_THREADS 4
#define f(_x) (_x*_x)

int main()
{
	DS_timer timer(5);
	timer.initTimers();

	double a = -1, b = 1;
	int n = (1024*1024);

	double h = (b - a) / n;
	printf("f(x) = x * x\n");
	printf("range = (%lf, %lf), n = %d, h = %.12lf\n\n", a, b, n, h);
	double sum = 0.0;

	timer.onTimer(0);
	#pragma omp parallel for num_threads(NUM_THREADS) reduction(+:sum)
	for (int i = 0; i < n - 1; i++)
	{
		double x_i = a + h * i;
		double x_j = a + h * (i + 1);
		double d = (f(x_i) + f(x_j)) / 2.0;
		//#pragma omp critical
		//{
			sum += d*h;
		//}
	}
	timer.offTimer(0);
	printf("Sum =\t%2lf  ", sum);

	timer.printTimer();

	getchar();
	return 0;
}