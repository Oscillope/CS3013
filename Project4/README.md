Project 4
=========

To compile, run `make`. It will compile with a suite of test programs. `stress_malloc`, `runCommand`, `shell`, and `shell2` will use our malloc functions. To test the meh_malloc functions, run `make test_malloc`.

`stress_malloc` should be used for testing the malloc functions. The first section tests the ability to malloc() and free(). The first two strings should occupy the same addresses every loop. The third string should represent a memory leak and should be assigned a different address every time. `stress_malloc` then tests the ability to allocate a 0-sized pointer (it should return NULL), allocate a 1-sized pointer, and allocate a very large pointer. It then tests calloc with an array of integers and realloc with a string that expands.

By Jason Rosenman & Neil Pomerleau
