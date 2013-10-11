//Jason Rosenman & Neil Pomerleau

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"

int main(int argc, char** argv) {
	int i = 0;
	char* teststring;
	char* teststring2;
	char* teststring3;
	for(i = 0; i < 300; i++) {
		teststring = NULL;
		teststring2 = NULL;
		teststring3 = NULL;
		teststring = (char*)malloc(strlen("Hello, world!")+1);
		strcpy(teststring, "Hello, world!");
		printf("%s @ %p\n", teststring, teststring);
		teststring2 = (char*)malloc(strlen("Something else!")+1);
		strncpy(teststring2, "Something else!", strlen("Something else!")+1);
		printf("%s @ %p, %d\n", teststring2, teststring2, i);
		free(teststring);
		free(teststring2);
		teststring3 = (char*)malloc(strlen("This is a pretty long string! I mean, it's definitely not short!")+1);
		strncpy(teststring3, "This is a pretty long string! I mean, it's definitely not short!", strlen("This is a pretty long string! I mean, it's definitely not short!")+1);
		printf("%s @ %p\n", teststring3, teststring3);
	}
	printf("Testing 0-size malloc.\n");
	if(malloc(0) == NULL)
		printf("	Passed!\n");
	else
		printf("	Failed!\n");
	printf("Testing 1-size malloc.\n");
	char* a_byte = malloc(1);
	a_byte = 'a';
	printf("	The value of a_byte is %c.\n", a_byte);
	printf("Trying to allocate a very large amount of space.\n");
	char* bigstring;
	bigstring = malloc(1234567);
	if(bigstring != NULL) {
		bigstring = "It's so roomy in here!";
		printf("	%s\n", bigstring);
	}
	else
		printf("I couldn't get that much memory.\n");
	printf("Testing calloc.\n");
	int* random_int = malloc(sizeof(int));
	printf("	This value should be something in memory: %d\n", *random_int);
	int* empty_int = calloc(20, sizeof(int));
	for(i = 0; i < 20; i++)
		printf("	This value should be zero: %d\n", *(empty_int++));
	printf("Testing realloc.\n");
	char* dynamic_string = malloc(strlen("I'm a string!")+1);
	strcpy(dynamic_string, "I'm a string!");
	printf("	%s @ %p\n", dynamic_string, dynamic_string);
	dynamic_string = realloc(dynamic_string, strlen("I'm a string! Now with more bytes!")+1);
	strcat(dynamic_string, " Now with more bytes!");
	printf("	%s @ %p\n", dynamic_string, dynamic_string);
	return 0;
}
