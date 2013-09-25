CC = gcc
CFLAGS = -Wall -pthread

all: problem1
debug: CFLAGS += -DDEBUG -ggdb
debug: problem1

problem1: problem1.o
	$(CC) $(CFLAGS) problem1.o -o problem1
problem1.o:
	$(CC) $(CFLAGS) -c problem1.c
	
clean:
	rm -f *~ *.o
