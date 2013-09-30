#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_CARS 20

int main(int argc, char** argv) {
	pthread_t car_threads[NUM_CARS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	int i;
	for(i = 0; i < NUM_CARS; i++)
		pthread_create(car_threads[i], &attr, (void*)&masshole, NULL);
	while(1) {
		
	}
	return 0;
}
