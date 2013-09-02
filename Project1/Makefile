CC = gcc
CFLAGS = -Wall

all: runCommand
runCommand: runCommand.o
	$(CC) $(CFLAGS) runCommand.o -o runCommand
runCommand.o:
	$(CC) $(CFLAGS) -c runCommand.c
