#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_CARS 20

sem_t road_nw;
sem_t road_ne;
sem_t road_sw;
sem_t road_se;

void car(int *car_num);

int main(int argc, char** argv) {
    sem_init(&road_nw, 0, 1);
    sem_init(&road_ne, 0, 1);
    sem_init(&road_sw, 0, 1);
    sem_init(&road_se, 0, 1);
	pthread_t car_threads[NUM_CARS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	int i;
	for(i = 0; i < NUM_CARS; i++)
		pthread_create(&car_threads[i], &attr, (void*)&car, &i);
	return 0;
}

void car(int *car_num) {
    int random = rand();
    int randRoad = random % 4;
    int randDirection = random % 3;

    switch(randRoad) {
        case 0:
            printf("Car #%d entering from the north!\n", *car_num);
	    break;
        case 1:
            printf("Car #%d entering from the east!\n", *car_num);
            break;
        case 2:
            printf("Car #%d entering from the south!\n", *car_num);
            break;
        case 3:
            printf("Car #%d entering from the west!\n", *car_num);
            break;
    }
    switch(randDirection) {
        case 0:
            printf("Car #%d going left!\n", *car_num);
            break;
        case 1:
            printf("Car #%d going straight!\n", *car_num);
            break;
        case 2:
            printf("Car #%d going right!\n", *car_num);
            break;
    }
}
