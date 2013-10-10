#Jason Rosenman & Neil Pomerleau

CC = gcc
CFLAGS = -Wall -pthread

all: test_malloc stress_malloc shell shell2
debug: CFLAGS += -DDEBUG -ggdb
debug: test_malloc stress_malloc shell shell2

meh_malloc:
	$(CC) $(CFLAGS) -c meh_malloc.c
	
malloc:
	$(CC) $(CFLAGS) -c malloc.c
	
test_malloc: test_malloc.o meh_malloc
	$(CC) $(CFLAGS) test_malloc.o meh_malloc.o -o test_malloc
test_malloc.o:
	$(CC) $(CFLAGS) -c test_malloc.c
	
stress_malloc: stress_malloc.o malloc
	$(CC) $(CFLAGS) stress_malloc.o malloc.o -o stress_malloc
stress_malloc.o:
	$(CC) $(CFLAGS) -c stress_malloc.c
	
shell: shell.o malloc
	$(CC) $(CFLAGS) shell.o malloc.o -o shell
shell.o:
	$(CC) $(CFLAGS) -c shell.c

shell2: shell2.o malloc
	$(CC) $(CFLAGS) shell2.o malloc.o -o shell2
shell2.o:
	$(CC) $(CFLAGS) -c shell2.c
	
clean:
	rm -f *~ *.o
