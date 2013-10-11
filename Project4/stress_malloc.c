//Jason Rosenman & Neil Pomerleau

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"

int main(int argc, char** argv) {
	int i = 0;
	char* teststring;
	char* teststring2;
	while(1) {
		teststring = (char*)malloc(strlen("Hello, world!")+1);
		strcpy(teststring, "Hello, world!");
		//printf("%s @ %p\n", teststring, teststring);
		printf("%s\n", teststring);
		free(teststring);
		teststring2 = (char*)malloc(strlen("Something else!")+1);
		strncpy(teststring2, "Something else!", strlen("Something else!")+1);
		//printf("%s @ %p, %d\n", teststring2, teststring2, i);
		printf("%s\n", teststring);
		free(teststring2);
		//sleep(1);
		i++;
	}
	return 0;
}
