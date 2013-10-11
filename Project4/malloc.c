//Jason Rosenman & Neil Pomerleau

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
	if(size == 0)
		return NULL;
	cur = head;
	if(head == NULL) {
		chunk_size = size * 10 > 30 ? size * 10 : (size + 20) * 10;
		head = (mem_list*)sbrk(chunk_size);
		if(head == NULL)
			return head;
		free_space = chunk_size;
		cur = head;
		cur->block_size = chunk_size;
		cur->next = (mem_list*)((char*)cur + size + 20);	//Set up an emty header for next.
		cur->prev = NULL;	//The beginning of the list should never have a previous.
	}
	else {
		while(cur->next != NULL && cur->block_size < size + 20) cur = cur->next;
		if(cur->next == NULL) {
			if(cur->block_size < size + 32) {
				if(size + 20 > cur->block_size) {
					mem_list* old_cur = cur;
					chunk_size = size * 10 > 30 ? size * 10 : (size + 20) * 10;
					free_space += chunk_size;
					cur = (mem_list*)sbrk(chunk_size);
					if(cur == NULL)
						return cur;
					cur->block_size = chunk_size;
					cur->next = (mem_list*)((char*)cur + size + 20);
					cur->prev = old_cur->prev;
					if(cur->prev != NULL)
						cur->prev->next = cur;
				}
				else {
					chunk_size = size * 10 > 30 ? size * 10 : (size + 20) * 10;
					free_space += chunk_size;
					cur->next = (mem_list*)sbrk(chunk_size);
					cur->next->block_size = chunk_size;
					cur->next->next = (mem_list*)((char*)cur->next + size + 20);
					cur->next->prev = cur;
				}
			}
			else
				cur->next = (mem_list*)((char*)cur + size + 20);
		}
		
	}
	#ifdef DEBUG
		printf("My %d-byte block starts at %p, I'm going to give the process a %d byte block starting at %p.\n", cur->block_size, cur, size, (cur + 1));
	#endif
	free_space -= (size + 20);
	if(cur->next != NULL) {
		if(cur->next->checksum != 0xFFFFFFFF) {
			cur->next->block_size = cur->block_size - size - 20;
			cur->next->next = NULL;
		}
		cur->next->prev = cur;			//Set the next header's previous pointer to where we are now.
	}
	cur->block_size = 0;					//We're going to write here, so the block is no longer free.
	cur->proc_size = size;
	cur->checksum = 0xFFFFFFFF;				//There will be a checksum here eventually.
	return (void*)(cur + 1);
}

void free(void* ptr) {
	if(ptr == NULL)
		return;
	#ifdef DEBUG
		printf("Called free on %p!\n", ptr);
	#endif
	cur = ptr - 20;
	if(cur->checksum == 0xFFFFFFFF) {
		cur->block_size = cur->proc_size + 20;
		free_space += cur->block_size;
		/*if (cur->prev != NULL) {
        		cur->prev->next = cur->next;
        }
        else if (cur->next != NULL) {
			cur->next->prev = cur->prev;
		}*/
	} else {
		fprintf(stderr, "Not a valid memory location.\n");
	}
	return;
}

void *calloc(size_t nmemb, size_t size) {
	size = size * nmemb;
	void *ptr = malloc(size);
	bzero(ptr, size);
	return ptr;
}

void *realloc(void *ptr, size_t size) {
	void *dst = malloc(size);
	bcopy(ptr, dst, size);
	free(ptr);
	return dst;
}
