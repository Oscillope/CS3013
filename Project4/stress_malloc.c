//Jason Rosenman & Neil Pomerleau

#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	int i = 0;
	char* teststring;
	char* teststring2;
	while(1) {
		teststring = (char*)malloc(sizeof("Hello, world!"));
		strcpy(teststring, "Hello, world!");
		printf("%s @ %p\n", teststring, teststring);
		//free(teststring);
		teststring2 = (char*)malloc(sizeof("Something else!"));
		strcpy(teststring2, "Something else!");
		printf("%s @ %p, %d\n", teststring2, teststring2, i);
		//free(teststring2);
		//sleep(1);
		i++;
	}
	return 0;
}
