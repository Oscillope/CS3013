obj-m := syscalltest.o new_open.o new_close.o new_syscall2.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: testcall runCommand
	$(MAKE) -C $(KDIR) M=$(PWD) modules
testcall: testcall.o
	gcc testcall.o -o testcall
testcall.o:
	gcc -c testcall.c
runCommand: runCommand.o
	gcc runCommand.o -o runCommand
runCommand.o:
	gcc -c runCommand.c
	
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f *~ *.o
