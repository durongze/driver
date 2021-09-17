MODNAME=ioctl
ifeq ($(PLATFORM),PC)
	KERNELDIR=/lib/modules/$(shell uname -r)/build
       EXTRA_CFLAGS=-Dsimple_DEBUG
else
	KERNELDIR=$(shell echo ${HOME})/linux 
       EXTRA_CFLAGS=-Dsimple_DEBUG
endif

CC=gcc

LIBS= -lasound
SRCS=*.c
INPUT_DEVS:=$(shell ls /dev/input/even*)

default:
	$(CC) -g -o $(MODNAME) $(SRCS) $(LIBS)

test:
	@echo "INPUT_DEVS:${INPUT_DEVS}"
	@for d in $(INPUT_DEVS) ; do echo "\033[32mdev:$$d \033[0m"; sudo ./ioctl $$d; done;

clean:
	rm -fr *.dwo .*.symvers.cmd .*.order.cmd *.ko *.mod
	rm -fr *.mod.* *.o  .$(MODNAME).* .tmp* Module.* modules.* *.img $(MODNAME) 


