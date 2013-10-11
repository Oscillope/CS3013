//Jason Rosenman & Neil Pomerleau

#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	int i = 0;
	char* teststring;
	char* teststring2;
	while(1) {
		teststring = (char*)malloc(strlen("Hello, world!")+1);
		strncpy(teststring, "Hello, world!", strlen("Hello, world!") + 1);
		printf("%s, %d @ %p\n", teststring, strlen(teststring), teststring);
		free(teststring);
		teststring2 = (char*)malloc(strlen("Something else!")+1);
		strncpy(teststring2, "Something else!", strlen("Something else!")+1);
		printf("%s @ %p, %d\n", teststring2, teststring2, i);
		free(teststring2);
		//sleep(1);
		i++;
	}
	return 0;
}
