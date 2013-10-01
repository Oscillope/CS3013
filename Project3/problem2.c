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
#define NORTH 3
#define EAST 4
#define SOUTH 5
#define WEST 6

sem_t road_nw;
sem_t road_ne;
sem_t road_sw;
sem_t road_se;

struct car_struct{
	pthread_t thread;
	char from;
	char turn;
	struct car_struct* next;
};
typedef struct car_struct car;

void masshole(void);
void add_car(car* head, car* add);

// Initialize us some linked lists
car north_cars;
car east_cars;
car south_cars;
car west_cars;


int main(int argc, char** argv) {
	north_cars.next = &north_cars;
	east_cars.next = &east_cars;
	south_cars.next = &south_cars;
	west_cars.next = &west_cars;
    sem_init(&road_nw, 0, 1);
    sem_init(&road_ne, 0, 1);
    sem_init(&road_sw, 0, 1);
    sem_init(&road_se, 0, 1);
	pthread_t car_threads[NUM_CARS];
	pthread_t control_threads[4];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	int i;
	for(i = 0; i < NUM_CARS; i++)
		pthread_create(&car_threads[i], &attr, (void*)&masshole, NULL);
	/*pthread_create(&control_threads[0], &attr, (void*)&control_north, NULL);
	pthread_create(&control_threads[1], &attr, (void*)&control_east, NULL);
	pthread_create(&control_threads[2], &attr, (void*)&control_south, NULL);
	pthread_create(&control_threads[3], &attr, (void*)&control_west, NULL);*/
	pthread_join(car_threads[NUM_CARS-1], NULL);
	return 0;
}

void masshole(void) {
    int random = rand();
    int randRoad = (random % 4) + 3;
    int randDirection = random % 3;
    car this_car;
    this_car.thread = pthread_self();
    this_car.from = randRoad;
    this_car.turn = randDirection;
    unsigned long int car_num = (unsigned long int)this_car.thread;
    switch(randRoad) {
        case NORTH:
            printf("Car %#02lx entering from the north!\n", car_num);
            add_car(&north_cars, &this_car);
            switch(randDirection) {
		        case LEFT:
		            printf("Car %#02lx going left!\n", car_num);
		            break;
		        case STRAIGHT:
		            printf("Car %#02lx going straight!\n", car_num);
		            break;
		        case RIGHT:
		            printf("Car %#02lx going right!\n", car_num);
		            break;
		    }
			break;
        case EAST:
            printf("Car %#02lx entering from the east!\n", car_num);
            add_car(&east_cars, &this_car);
            switch(randDirection) {
		        case LEFT:
		            printf("Car %#02lx going left!\n", car_num);
		            break;
		        case STRAIGHT:
		            printf("Car %#02lx going straight!\n", car_num);
		            break;
		        case RIGHT:
		            printf("Car %#02lx going right!\n", car_num);
		            break;
		    }
            break;
        case SOUTH:
            printf("Car %#02lx entering from the south!\n", car_num);
            add_car(&south_cars, &this_car);
            switch(randDirection) {
		        case LEFT:
		            printf("Car %#02lx going left!\n", car_num);
		            break;
		        case STRAIGHT:
		            printf("Car %#02lx going straight!\n", car_num);
		            break;
		        case RIGHT:
		            printf("Car %#02lx going right!\n", car_num);
		            break;
		    }
            break;
        case WEST:
            printf("Car %#02lx entering from the west!\n", car_num);
            add_car(&west_cars, &this_car);
            switch(randDirection) {
		        case LEFT:
		            printf("Car %#02lx going left!\n", car_num);
		            break;
		        case STRAIGHT:
		            printf("Car %#02lx going straight!\n", car_num);
		            break;
		        case RIGHT:
		            printf("Car %#02lx going right!\n", car_num);
		            break;
		    }
            break;
    }
}

void add_car(car* head, car* add) {
	add->next = head->next;
	head->next = add;
}
