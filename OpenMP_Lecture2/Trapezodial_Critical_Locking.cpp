/**
This is an exmple code used in the OpenMP - lecture 2 <br>
@author : Duksu Kim and Young-Gyu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../DSTimer/DS_timer.h"
#include "../DSTimer/DS_definitions.h"

#define NUM_THREADS 4
#define f(_x) (_x*_x)

double Trapezodial_Serial(double a, double b, int n, double h);
double Trapezodial_Atomic(double a, double b, int n, double h);
double Trapezodial_Critical(double a, double b, int n, double h);
double Trapezodial_Locking(double a, double b, int n, double h);

int main()
{
	DS_timer timer(5);
	timer.initTimers();

	double a = -1, b = 1;
	int n = (1024 * 1024 * 256);

	double h = (b - a) / n;
	printf("f(x) = x * x\n");
	printf("range = (%lf, %lf), n = %d, h = %.12lf\n\n", a, b, n, h);
	double sum = 0.0;
	double tempsum = 0.0;

	//Serial
	timer.onTimer(0);
	sum = Trapezodial_Serial(a, b, n, h);
	timer.offTimer(0);	
	printf("Serial Sum =\t%2lf", sum);
	printf("\n");

	//Atomic
	timer.onTimer(1);
	tempsum = Trapezodial_Atomic(a, b, n, h);
	timer.offTimer(1);	
	printf("Atomic Sum =\t%2lf  ", tempsum);
	printf("\n");
	tempsum = 0.0;

	//Critical
	timer.onTimer(2);
	tempsum = Trapezodial_Critical(a, b, n, h);
	timer.offTimer(2);
	printf("Critical Sum =\t%2lf  ", tempsum);
	printf("\n");
	tempsum = 0.0;

	//Locking
	timer.onTimer(3);
	tempsum = Trapezodial_Locking(a, b, n, h);
	timer.offTimer(3);
	printf("Locking Sum =\t%2lf  ", tempsum);
	printf("\n");
	tempsum = 0.0;

	timer.setTimerName(0, "serial");
	timer.setTimerName(1, "Atomic");
	timer.setTimerName(2, "Critical");
	timer.setTimerName(3, "Locking");

	printf("\n");
	timer.printTimer();

	getchar();

	return 0;
}


double Trapezodial_Serial(double a, double b, int n, double h)
{
	double sum = 0;
	for (int i = 0; i < n - 1; i++)
	{
		double x_i = a + h * i;
		double x_j = a + h * (i + 1);
		double d = (f(x_i) + f(x_j)) / 2.0;
		sum += d*h;
	}
	return sum;
}

double Trapezodial_Critical(double a, double b, int n, double h)
{
	double sum = 0;

	#pragma omp parallel for num_threads(NUM_THREADS)
	for (int i = 0; i < n - 1; i++)
	{
		double x_i = a + h * i;
		double x_j = a + h * (i + 1);
		double d = (f(x_i) + f(x_j)) / 2.0;

		#pragma omp critical
		{
			sum += d*h;
		}
	}
	return sum;
}

double Trapezodial_Atomic(double a, double b, int n, double h)
{
	double sum = 0;
	#pragma omp parallel for num_threads(NUM_THREADS)
	for (int i = 0; i < n - 1; i++)
	{
		double x_i = a + h * i;
		double x_j = a + h * (i + 1);
		double d = (f(x_i) + f(x_j)) / 2.0;

		#pragma omp atomic
		sum += d*h;
	}
	return sum;
}

double Trapezodial_Locking(double a, double b, int n, double h)
{
	double sum = 0;

	omp_lock_t sumLock;
	omp_init_lock(&sumLock);

	#pragma omp parallel for num_threads(NUM_THREADS) 
	for (int i = 0; i < n - 1; i++)
	{
		double x_i = a + h * i;
		double x_j = a + h * (i + 1);
		double d = (f(x_i) + f(x_j)) / 2.0;

		omp_set_lock(&sumLock);
		sum += d*h;
		omp_unset_lock(&sumLock);
	}

	omp_destroy_lock(&sumLock);

	return sum;
}

