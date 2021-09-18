SEND_APP=sender
SNIFFER_APP=sniffer

ifeq ($(PLATFORM),PC)
	KERNELDIR=/lib/modules/$(shell uname -r)/build
       EXTRA_CFLAGS=-Dsimple_DEBUG
else
	KERNELDIR=$(shell echo ${HOME})/linux 
       EXTRA_CFLAGS=-Dsimple_DEBUG
endif

CC=gcc

LIBS= 

SEND_SRCS=send*.c
SNIFFER_SRCS=sniffer*.c

DRVNAME=netlink
IS_EXIST=$(shell lsmod | grep ${DRVNAME})

ifeq ($(IS_EXIST),)
    IS_EXIST=" none"
else
    IS_EXIST=" exist"
endif

default:
	$(CC) -g -o $(SEND_APP) $(SEND_SRCS) $(LIBS)
	$(CC) -g -o $(SNIFFER_APP) $(SNIFFER_SRCS) $(LIBS)

test:default
	@echo driver ${IS_EXIST}
	./${SEND_APP}

clean:
	rm -fr *.dwo .*.symvers.cmd .*.order.cmd *.ko *.mod
	rm -fr *.mod.* *.o .tmp* Module.* modules.* *.img $(SEND_APP) $(SNIFFER_APP) 


