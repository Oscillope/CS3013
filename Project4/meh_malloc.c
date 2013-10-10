//Jason Rosenman & Neil Pomerleau
//TODO: Instead of making the pointers a char* and casting to int*, make a void* and cast to char* for arithmetic.
//Also use more structs

#include "malloc.h"

char* head = NULL;
char* cur = NULL;

int chunk_size = 0;
int free_space = 0;

void* meh_malloc(size_t size) {
	cur = head;
	if(head == NULL) {
		chunk_size = size * 10;
		head = sbrk(chunk_size);
		cur = head;
		*(int*)cur = chunk_size;
		*(cur + 4) = NULL;	//No next yet.
		*(cur + 8) = NULL;	//The beginning of the list should never have a previous.
	}
	else if(*(int*)cur == 0) {
		while(*(cur + 4) != NULL && *(int*)cur == 0) cur = *(int*)(cur + 4);
		if(cur + size + 32 >= head + chunk_size) {
			chunk_size += size * 10;
			cur = sbrk(size * 10);
			*(int*)cur = size * 10;
			*(cur + 4) = NULL;
		}
	}
	free_space = *(int*)cur - size - 20;
	*(int*)cur = 0;					//We're going to write here, so the block is no longer free.
	#ifdef DEBUG
		printf("My block starts at %p, I'm going to give the process a %d byte block starting at %p.\n", cur, size, (cur + 20));
	#endif
	*(int*)(cur + 4) = cur + size + 20;	//Set the next pointer to where the next header is going to be.
	*(int*)(cur + size + 20) = free_space;
	*(cur + size + 24) = NULL;
	*(cur + size + 28) = cur;		//Set the next header's previous pointer to where we are now.
	cur += 12;						//Get out of the header.
	*(int*)cur = size;				//Put size in beginning of block.
	cur += 4;
	*(int*)cur = 1;					//There will be a checksum here eventually.
	cur += 4;
	return (void*)cur;
}

void meh_free(void* ptr) {
	printf("Called free on %p!\n", ptr);
	return;
}
