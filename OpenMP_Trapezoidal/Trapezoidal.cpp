/**
This is a sample solution code for Lab 2-2. Trapezodial Rule
@author : Duksu Kim
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../DSTimer/DS_timer.h"
#include "../DSTimer/DS_definitions.h"
#include "parameters.h"

#define f(_x) (_x*_x)

enum Algorithm {
	Serial, ImplicitBarrier_NoSync, ImplicitBarrier, ImplicitBarrier_SharedLocalVar, Atomic, Critical, Locking, Reduction,
	END
};

double Trapezodial_Serial(double a, double b, int n, double h);
double Trapezodial_ImplicitBarrier_NoSync(double a, double b, int n, double h);
double Trapezodial_ImplicitBarrier(double a, double b, int n, double h);
double Trapezodial_ImplicitBarrier_SharedLocalVar(double a, double b, int n, double h);
double Trapezodial_Atomic(double a, double b, int n, double h);
double Trapezodial_Critical(double a, double b, int n, double h);
double Trapezodial_Locking(double a, double b, int n, double h);
double Trapezodial_Reduction(double a, double b, int n, double h);

#define RUN_TEST(_func, a, b, n, h) { \
	timer.onTimer(Algorithm::_func); \
	sum = Trapezodial_##_func(a, b, n, h);			\
	timer.offTimer(Algorithm::_func); \
	printf("[%s] area = %lf (%.2f x)\n", #_func, sum		\
		, timer.getTimer_ms(Algorithm::Serial) / timer.getTimer_ms(Algorithm::_func));	\
}

int main(int argc, char* argv[])
{
	if (argc < 4) {
		printf("[Usage] executionFileName a b n\n");
		EXIT_WIHT_KEYPRESS;
	}
	DS_timer timer(Algorithm::END);
	timer.setTimerName(Algorithm::Serial, "[Baseline] Serial algorithm");
	timer.setTimerName(Algorithm::ImplicitBarrier_NoSync, "[Parallel]Implicit barrier w/o Sync.");
	timer.setTimerName(Algorithm::ImplicitBarrier, "[Parallel]Implicit barrier");
	timer.setTimerName(Algorithm::ImplicitBarrier_SharedLocalVar, "[Parallel] Implicit barrier (Shared local Var)");
	timer.setTimerName(Algorithm::Atomic, "[Parallel] Atomic for sum");
	timer.setTimerName(Algorithm::Critical, "[Parallel] Critical for sum");
	timer.setTimerName(Algorithm::Locking, "[Parallel] Locking for sum");
	timer.setTimerName(Algorithm::Reduction, "[Parallel] Using reduction");

	double a = 0, b = 0;
	int n = 0 ;
	a = atof(argv[1]);
	b = atof(argv[2]);
	n = atoi(argv[3]);

	double h = (b - a) / n;
	printf("f(x) = x * x\n");
	printf("range = (%lf, %lf), n = %d, h = %.12lf\n", a, b, n, h);
	double sum = 0;

	RUN_TEST(Serial, a, b, n, h);
	RUN_TEST(ImplicitBarrier_NoSync, a, b, n, h);
	RUN_TEST(ImplicitBarrier, a, b, n, h);
	RUN_TEST(ImplicitBarrier_SharedLocalVar, a, b, n, h);
	RUN_TEST(Atomic, a, b, n, h);
	RUN_TEST(Critical, a, b, n, h);
	RUN_TEST(Locking, a, b, n, h);
	RUN_TEST(Reduction, a, b, n, h);

	timer.printTimer();
}

/* Function definitions */
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

double Trapezodial_ImplicitBarrier_NoSync(double a, double b, int n, double h)
{
	double sum = 0;
	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tid = omp_get_thread_num();

		#pragma omp for
		for (int i = 0; i < n - 1; i++)
		{
			double x_i = a + h * i;
			double x_j = a + h * (i + 1);
			double d = (f(x_i) + f(x_j)) / 2.0;
			sum += d * h;
		}
	}

	return sum;
}

double Trapezodial_ImplicitBarrier(double a, double b, int n, double h)
{
	double sum = 0;
	double local[NUM_THREADS * offset] = { 0 };
	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tid = omp_get_thread_num() * offset;

		#pragma omp for
		for (int i = 0; i < n - 1; i++)
		{
			double x_i = a + h * i;
			double x_j = a + h * (i + 1);
			double d = (f(x_i) + f(x_j)) / 2.0;
			local[tid] += d*h;
		}
	} // implicit barrier here

	  // Gathering the local sums in serial
	LOOP_I(NUM_THREADS)
		sum += local[i * offset];

	return sum;
}

double Trapezodial_ImplicitBarrier_SharedLocalVar(double a, double b, int n, double h)
{
	double sum = 0;
	double local[NUM_THREADS * offset] = { 0 };
	
	double x_i[NUM_THREADS * offset] = { 0 };
	double x_j[NUM_THREADS * offset] = { 0 };
	double d[NUM_THREADS * offset] = { 0 };

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tid = omp_get_thread_num() * offset ;

		#pragma omp for
		for (int i = 0; i < n - 1; i++)
		{
			x_i[tid] = a + h * i;
			x_j[tid] = a + h * (i + 1);
			d[tid] = (f(x_i[tid]) + f(x_j[tid])) / 2.0;
			local[tid] += d[tid] *h;
		}
	} // implicit barrier here

	  // Gathering the local sums in serial
	LOOP_I(NUM_THREADS)
		sum += local[i * offset];

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

double Trapezodial_Reduction(double a, double b, int n, double h)
{
	double sum = 0;
	#pragma omp parallel num_threads(NUM_THREADS) reduction(+:sum)
	{
		int tid = omp_get_thread_num();

		#pragma omp for
		for (int i = 0; i < n - 1; i++)
		{
			double x_i = a + h * i;
			double x_j = a + h * (i + 1);
			double d = (f(x_i) + f(x_j)) / 2.0;
			sum += d*h;
		}
	}

	return sum;
}