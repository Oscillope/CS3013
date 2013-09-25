#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_PIRATES 18
#define NUM_NINJAS 14
#define EMPTY 0
#define PIRATE 1
#define NINJA 2

pthread_mutex_t chair1;
pthread_mutex_t chair2;
pthread_mutex_t state_mutex;
pthread_mutex_t room_lock;
pthread_cond_t room_cv;
int room_state;
int pirates_waiting;
int ninjas_waiting;

void pirate(void);
void ninja(void);

int main(int argc, char** argv) {
	pthread_mutex_init(&chair1, NULL);
	pthread_mutex_init(&chair2, NULL);
	pthread_mutex_init(&state_mutex, NULL);
	pthread_mutex_init(&room_lock, NULL);
	pthread_cond_init(&room_cv, NULL);
	pthread_t pirate_threads[NUM_PIRATES];
	pthread_t ninja_threads[NUM_NINJAS];
	pthread_attr_t attr;
	room_state = EMPTY;
	pirates_waiting = NUM_PIRATES;
	ninjas_waiting = NUM_NINJAS;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	int i;
	for(i = 0; i < NUM_PIRATES; i++)
		pthread_create(&pirate_threads[i], &attr, (void*)&pirate, NULL);
	for(i = 0; i < NUM_NINJAS; i++)
		pthread_create(&ninja_threads[i], &attr, (void*)&ninja, NULL);
	pthread_join(pirate_threads[0], NULL);
	return 0;
}

void pirate(void) {
	printf("AAAARRRRGHH\n");
	while(1) {
		if(pthread_mutex_trylock(&state_mutex) != EBUSY) {
			if(room_state == EMPTY)
				room_state = PIRATE;
			pthread_mutex_unlock(&state_mutex);
		}
		if(room_state == PIRATE && pirates_waiting > 0) {
			if(pthread_mutex_trylock(&chair1) != EBUSY) {
				printf("Getting a costume in chair 1.\n");
				pthread_mutex_lock(&state_mutex);
				pirates_waiting--;
				pthread_mutex_unlock(&state_mutex);
				sleep(2);
				printf("Done! %d of us are left, and %d of them. -- Chair 1\n", pirates_waiting, ninjas_waiting);
				pthread_mutex_unlock(&chair1);
				pthread_cond_broadcast(&room_cv);
				sleep(10);
				if(pthread_mutex_trylock(&state_mutex) != EBUSY) {
					if(pirates_waiting < NUM_PIRATES)
						pirates_waiting++;
					pthread_mutex_unlock(&state_mutex);
				}
			}
			else if(pthread_mutex_trylock(&chair2) != EBUSY) {
				printf("Getting a costume in chair 2.\n");
				pthread_mutex_lock(&state_mutex);
				pirates_waiting--;
				pthread_mutex_unlock(&state_mutex);
				sleep(2);
				printf("Done! %d of us are left, and %d of them. -- Chair 2\n", pirates_waiting, ninjas_waiting);
				if(ninjas_waiting > pirates_waiting) {
					pthread_mutex_lock(&state_mutex);
					pthread_mutex_lock(&chair1);
					printf("I'm the last one here!\n");
					room_state = NINJA;
					pthread_mutex_unlock(&chair1);
					pthread_mutex_unlock(&state_mutex);
				}
				else if(pirates_waiting == 0) {
					pthread_mutex_lock(&state_mutex);
					pthread_mutex_lock(&chair1);
					room_state = EMPTY;
					pthread_mutex_unlock(&chair1);
					pthread_mutex_unlock(&state_mutex);
				}
				pthread_mutex_unlock(&chair2);
				pthread_cond_broadcast(&room_cv);
				sleep(10);
				if(pthread_mutex_trylock(&state_mutex) != EBUSY) {
					if(pirates_waiting < NUM_PIRATES)
						pirates_waiting++;
					pthread_mutex_unlock(&state_mutex);
				}
			}
			else {
				pthread_mutex_lock(&room_lock);
				//printf("zzzzz\n");
				pthread_cond_wait(&room_cv, &room_lock);
				pthread_mutex_unlock(&room_lock);
				//printf("I'm awake!\n");
			}
		}
		else {
			pthread_mutex_lock(&room_lock);
			//printf("This isn't our room!\n");
			pthread_cond_wait(&room_cv, &room_lock);
			pthread_mutex_unlock(&room_lock);
		}
	}
}

void ninja(void) {
	printf("...\n");
	while(1) {
		if(pthread_mutex_trylock(&state_mutex) != EBUSY) {
			if(room_state == EMPTY)
				room_state = NINJA;
			pthread_mutex_unlock(&state_mutex);
		}
		if(room_state == NINJA && ninjas_waiting >= 0) {
			if(pthread_mutex_trylock(&chair1) != EBUSY) {
				printf("Embodying the night in chair 1.\n");
				pthread_mutex_lock(&state_mutex);
				ninjas_waiting--;
				pthread_mutex_unlock(&state_mutex);
				sleep(2);
				printf("It is finished. %d of my friends are at the door, we are %d. -- Chair 1\n", pirates_waiting, ninjas_waiting);;
				pthread_mutex_unlock(&chair1);
				pthread_cond_broadcast(&room_cv);
				sleep(10);
				if(pthread_mutex_trylock(&state_mutex) != EBUSY) {
					if(ninjas_waiting < NUM_NINJAS)
						ninjas_waiting++;
					pthread_mutex_unlock(&state_mutex);
				}
			}
			else if(pthread_mutex_trylock(&chair2) != EBUSY) {
				printf("Embodying the night in chair 2.\n");
				pthread_mutex_lock(&state_mutex);
				ninjas_waiting--;
				pthread_mutex_unlock(&state_mutex);
				sleep(2);
				printf("It is finished. %d of my friends are at the door, we are %d. -- Chair 2\n", pirates_waiting, ninjas_waiting);
				if(pirates_waiting > ninjas_waiting) {
					pthread_mutex_lock(&state_mutex);
					pthread_mutex_lock(&chair1);
					printf("I'm the last one here!\n");
					room_state = PIRATE;
					pthread_mutex_unlock(&chair1);
					pthread_mutex_unlock(&state_mutex);
				}
				else if(ninjas_waiting == 0) {
					pthread_mutex_lock(&state_mutex);
					pthread_mutex_lock(&chair1);
					room_state = EMPTY;
					pthread_mutex_unlock(&chair1);
					pthread_mutex_unlock(&state_mutex);
				}
				pthread_mutex_unlock(&chair2);
				pthread_cond_broadcast(&room_cv);
				sleep(10);
				if(pthread_mutex_trylock(&state_mutex) != EBUSY) {
					if(ninjas_waiting < NUM_NINJAS)
						ninjas_waiting++;
					pthread_mutex_unlock(&state_mutex);
				}
			}
			else {
				pthread_mutex_lock(&room_lock);
				//printf("........\n");
				pthread_cond_wait(&room_cv, &room_lock);
				pthread_mutex_unlock(&room_lock);
			}
		}
		else {
			pthread_mutex_lock(&room_lock);
			//printf("SEPPUKU\n");
			pthread_cond_wait(&room_cv, &room_lock);
			pthread_mutex_unlock(&room_lock);
			//printf("I have been awoken.\n");
		}
	}
}
