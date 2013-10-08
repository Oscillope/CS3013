//Jason Rosenman & Neil Pomerleau

#include "meh_malloc.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	char* teststring = meh_malloc(sizeof("Hello, world!"));
	teststring = "Hello, world!";
	printf("%s\n", teststring);
	meh_free(teststring);
	char* teststring2 = meh_malloc(sizeof("Something else!"));
	teststring2 = "Something else!";
	printf("%s\n", teststring2);
	//free(teststring);
	return 0;
}
