CC = gcc
CFLAGS = -Wall -pthread

all: problem1 problem2
debug: CFLAGS += -DDEBUG -ggdb
debug: problem1 problem2

problem1: problem1.o
	$(CC) $(CFLAGS) problem1.o -o problem1
problem1.o:
	$(CC) $(CFLAGS) -c problem1.c
	
problem2: problem2.o
	$(CC) $(CFLAGS) problem2.o -o problem2
problem2.o:
	$(CC) $(CFLAGS) -c problem2.c
	
clean:
	rm -f *~ *.o
