WR_APP=write
RD_APP=read

ifeq ($(PLATFORM),PC)
	KERNELDIR=/lib/modules/$(shell uname -r)/build
       EXTRA_CFLAGS=-Dsimple_DEBUG
else
	KERNELDIR=$(shell echo ${HOME})/linux 
       EXTRA_CFLAGS=-Dsimple_DEBUG
endif

CC=gcc

LIBS= 
WR_SRCS=write*.c
RD_SRCS=read*.c

DRVNAME=demo

IS_EXIST=$(shell lsmod | grep ${DRVNAME})

ifeq ($(IS_EXIST),)
    IS_EXIST=" none"
else
    IS_EXIST=" exist"
endif

default:
	$(CC) -g -o $(WR_APP) $(WR_SRCS) $(LIBS)
	$(CC) -g -o $(RD_APP) $(RD_SRCS) $(LIBS)

test:default
	@echo driver ${IS_EXIST}
	sudo ./${RD_APP} &
	sudo ./${WR_APP}

clean:
	rm -fr *.dwo .*.symvers.cmd .*.order.cmd *.ko *.mod
	rm -fr *.mod.* *.o .tmp* Module.* modules.* *.img $(WR_APP) $(RD_APP)


