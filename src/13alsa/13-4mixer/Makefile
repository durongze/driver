MODNAME=mixer
ifeq ($(PLATFORM),PC)
	KERNELDIR=/lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS=-Dsimple_DEBUG
else
	KERNELDIR=/home/pi/linux 
EXTRA_CFLAGS=-Dsimple_DEBUG
endif

CC=gcc

LIBS= -lasound
SRCS=*.c

default:
	$(CC) -g -o $(MODNAME) $(SRCS) $(LIBS)

clean:
	rm -fr *.mod.* *.o  .$(MODNAME).* .tmp* Module.* modules.* *.img $(MODNAME) 


