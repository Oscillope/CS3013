#include "meh_malloc.h"

void* head = NULL;
void* next = NULL;

int chunk_size = 256;

void* meh_malloc(size_t size) {
	if(head == NULL) {
		head = sbrk(256);
		next = head;
	}
	else if(next + size >= head + chunk_size) {
		chunk_size += 256;
		next = sbrk(256);
	}
	else
		next += (size + 8);
	int* mem_size = (int*)next;
	*mem_size = size;		//Put size in beginning of block.
	next += 4;
	int* checksum = (int*)next;
	*checksum = 1;			//There will be a checksum here eventually.
	next += 4;
	return next;
}

void meh_free(void* ptr) {
	return;
}
