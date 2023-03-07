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
	int buf = 0; // 0: empty, otherwise: full

	omp_lock_t lock;
	omp_init_lock(&lock);

	bool isFinish = false;

	#pragma omp parallel sections shared(isFinish, lock) num_threads(2)
	{
		#pragma omp section // Producer
		{
			int numProduce = 10;
			while (numProduce > 1) {
				omp_set_lock(&lock);
				if (buf == 0) {
					numProduce--;
					buf = numProduce;
					printf("Produce push %d\n", buf);
				}
				omp_unset_lock(&lock);
				//Sleep(500);
			}
			isFinish = true;
		}

		#pragma omp section // Consumer
		{
			int get = 0;
			while (!isFinish) {
				omp_set_lock(&lock);
				if (buf > 0) {
					get = buf;
					buf = 0;
					printf("Consumer get %d\n", get);
				}
				omp_unset_lock(&lock);
				//Sleep(400);
			}
		}
	}
	omp_destroy_lock(&lock);
	printf("Finished!\n");
}