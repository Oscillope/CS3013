#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_CARS 20
#define LEFT 0
#define STRAIGHT 1
#define RIGHT 2

sem_t road_nw;
sem_t road_ne;
sem_t road_sw;
sem_t road_se;

void masshole(void);
void add_car(car* head, car* add);

typedef struct {
	pthread_t thread;
	char turn;
	car* next;
} car;

// Initialize us some linked lists
car north_cars;
north_cars.next = &north_cars;
car east_cars;
east_cars.next = &east_cars;
car south_cars;
south_cars.next = &south_cars;
car west_cars;
west_cars.next = &west_cars;

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
		pthread_create(&car_threads[i], &attr, (void*)&masshole, NULL);
	pthread_join(car_threads[NUM_CARS-1], NULL);
	return 0;
}

void masshole(void) {
    int random = rand();
    int randRoad = random % 4;
    int randDirection = random % 3;
    car this_car;
    this_car.thread = pthread_self();
    this_car.turn = randDirection;
    unsigned int car_num = (unsigned int)this_car.thread;
    switch(randRoad) {
        case 0:
            printf("Car #%u entering from the north!\n", car_num);
            add_car(&north_cars, &this_car);
			break;
        case 1:
            printf("Car #%u entering from the east!\n", car_num);
            add_car(&east_cars, &this_car);
            break;
        case 2:
            printf("Car #%u entering from the south!\n", car_num);
            add_car(&south_cars, &this_car);
            break;
        case 3:
            printf("Car #%u entering from the west!\n", car_num);
            add_car(&west_cars, &this_car);
            break;
    }
    switch(randDirection) {
        case 0:
            printf("Car #%u going left!\n", car_num);
            break;
        case 1:
            printf("Car #%u going straight!\n", car_num);
            break;
        case 2:
            printf("Car #%u going right!\n", car_num);
            break;
    }
}

void add_car(car* head, car* add) {
	add->next = head->next;
	head->next = add;
}
