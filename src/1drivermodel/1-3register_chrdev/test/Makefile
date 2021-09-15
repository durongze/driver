MODNAME=test
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

DRVNAME=demo
IS_EXIST=$(shell lsmod | grep ${DRVNAME})

ifeq ($(IS_EXIST),)
    IS_EXIST=" none"
else
    IS_EXIST=" exist"
endif

default:
	$(CC) -g -o $(MODNAME) $(SRCS) $(LIBS)

test:default
	@echo driver ${IS_EXIST}.
	$(shell if [ ! -f /dev/fgj ]; then sudo mknod /dev/fgj c 224 0 ; fi;)
	sudo ./${MODNAME}
clean:
	rm -fr *.dwo .*.symvers.cmd .*.order.cmd *.ko *.mod
	rm -fr *.mod.* *.o  .$(MODNAME).* .tmp* Module.* modules.* *.img $(MODNAME) 

