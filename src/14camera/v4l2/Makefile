MODNAME=v4l2
ifeq ($(PLATFORM),PC)
	KERNELDIR=/lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS=-Dsimple_DEBUG
else
	KERNELDIR=/home/pi/linux 
EXTRA_CFLAGS=-Dsimple_DEBUG
endif

CC=gcc

SRCS=*.c

default:
	$(CC) -o $(MODNAME) $(SRCS) 

clean:
	rm -fr *.mod.* *.o  .$(MODNAME).* .tmp* Module.* modules.* *.img $(MODNAME) 


