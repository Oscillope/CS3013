//Jason Rosenman & Neil Pomerleau
//TODO: Instead of making the pointers a char* and casting to int*, make a void* and cast to char* for arithmetic.
//Also use more structs

#include "malloc.h"

struct mem_list_ {
	int block_size;
	struct mem_list_* next;
	struct mem_list_* prev;
	int proc_size;
	int checksum;
};

typedef struct mem_list_ mem_list;

mem_list* head = NULL;
mem_list* cur = NULL;

int chunk_size = 0;
int free_space = 0;

void* malloc(size_t size) {
	cur = head;
	if(head == NULL) {
		chunk_size = size * 10;
		head = (mem_list*)sbrk(chunk_size);
		free_space = chunk_size;
		cur = head;
		cur->block_size = chunk_size;
		cur->next = NULL;	//No next yet.
		cur->prev = NULL;	//The beginning of the list should never have a previous.
	}
	else if(cur->block_size == 0) {
		while(cur->next != NULL && cur->block_size < size + 20) cur = cur->next;
		if(cur->next == NULL && size + 32 >= cur->block_size) {
			chunk_size += size * 10;
			free_space += size * 10;
			cur = (mem_list*)sbrk(size * 10);
			cur->block_size = size * 10;
			cur->next = NULL;
		}
	}
	#ifdef DEBUG
		printf("My %d-byte block starts at %p, I'm going to give the process a %d byte block starting at %p.\n", cur->block_size, cur, size, (cur + 1));
	#endif
	free_space -= (size + 20);
	cur->block_size = 0;					//We're going to write here, so the block is no longer free.
	if(cur->next != (char*)cur + size + 20) {
		cur->next = (char*)cur + size + 20;	//Set the next pointer to where the next header is going to be.
		cur->next->block_size = free_space;
		cur->next->next = NULL;
	}
	cur->next->prev = cur;			//Set the next header's previous pointer to where we are now.
	cur->proc_size = size;
	cur->checksum = 1;				//There will be a checksum here eventually.
	cur++;						//Advance to start of data block.
	return (void*)cur;
}

void free(void* ptr) {
	printf("Called free on %p!\n", ptr);
	cur = ptr - 12;
	cur->block_size = cur->proc_size + 20;
	return;
}

