CC = gcc
CFLAGS = -Wall

all: runCommand shell
debug: CFLAGS += -DDEBUG -ggdb
debug: shell
runCommand: runCommand.o
	$(CC) $(CFLAGS) runCommand.o -o runCommand
runCommand.o:
	$(CC) $(CFLAGS) -c runCommand.c
shell: shell.o
	$(CC) $(CFLAGS) shell.o -o shell
shell.o:
	$(CC) $(CFLAGS) -c shell.c

clean:
	rm -f *~ *.o
