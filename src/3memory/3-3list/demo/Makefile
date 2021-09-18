MODNAME=smodule
ifeq ($(PLATFORM),PC)
	KERNELDIR=/lib/modules/$(shell uname -r)/build
       EXTRA_CFLAGS=-Dsimple_DEBUG
else
	KERNELDIR=$(shell echo ${HOME})/linux 
       EXTRA_CFLAGS=-Dsimple_DEBUG
endif

obj-m += $(MODNAME).o

IS_EXIST=$(shell lsmod | grep ${MODNAME})

ifeq ($(IS_EXIST),)
    IS_EXIST="$(shell modinfo ${MODNAME}.ko)"
else
    IS_EXIST="$(shell sudo rmmod ${MODNAME})"
endif

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

test:default
	@echo ${MODNAME} status : ${IS_EXIST} .
	sudo insmod ${MODNAME}.ko
	#sudo rmmod ${MODNAME}

clean:
	rm -fr *.dwo .*.symvers.cmd .*.order.cmd *.ko *.mod
	rm -fr *.mod.* *.o  .$(MODNAME).* .tmp* Module.* modules.*


