MODNAME=fb
ifeq ($(PLATFORM),PC)
	KERNELDIR=/lib/modules/$(shell uname -r)/build
       EXTRA_CFLAGS=-Dsimple_DEBUG
else
	KERNELDIR=$(shell echo ${HOME})/linux 
       EXTRA_CFLAGS=-Dsimple_DEBUG
endif

CC=gcc

LIBS= 
SRCS=*.c

default:
	$(CC) -g -o $(MODNAME) $(SRCS) $(LIBS)
test:
	ls -l /dev/fb0
	cat /proc/devices | grep "fb"

clean:
	rm -fr *.dwo .*.symvers.cmd .*.order.cmd *.ko *.mod
	rm -fr *.mod.* *.o  .$(MODNAME).* .tmp* Module.* modules.* *.img $(MODNAME) 


