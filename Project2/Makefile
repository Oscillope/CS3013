obj-m := syscalltest.o new_open.o new_close.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc -c testcall.c
	gcc -o testcall testcall.o
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f *~ *.o
