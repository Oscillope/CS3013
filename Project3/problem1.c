#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_PIRATES 18
#define NUM_NINJAS 14

pthread_mutex_t chair1;
pthread_mutex_t chair2;

int main(int argc, char** argv) {
	pthread_mutex_init(chair1);
	pthread_mutex_init(chair2);
	pthread_t* pirates[NUM_PIRATES];
	pthread_t* ninjas[NUM_NINJAS];
	
	return 0;
}

int pirate() {
	printf("AAAARRRRGHH\n");
	
}

int ninja() {
	printf("...\n");
	
}
