CC = gcc
CFLAGS = -Wall

all: runCommand shell shell2
debug: CFLAGS += -DDEBUG -ggdb
debug: runCommand shell shell2
runCommand: runCommand.o
	$(CC) $(CFLAGS) runCommand.o -o runCommand
runCommand.o:
	$(CC) $(CFLAGS) -c runCommand.c
shell: shell.o
	$(CC) $(CFLAGS) shell.o -o shell
shell.o:
	$(CC) $(CFLAGS) -c shell.c
shell2: shell2.o
	$(CC) $(CFLAGS) shell2.o -o shell2
shell2.o:
	$(CC) $(CFLAGS) -c shell2.c

clean:
	rm -f *~ *.o
