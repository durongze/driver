MODNAME=ioremap
ifeq ($(PLATFORM),PC)
	KERNELDIR=/lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS=-Dsimple_DEBUG
else
	KERNELDIR=/home/pi/linux 
EXTRA_CFLAGS=-Dsimple_DEBUG
endif

obj-m += $(MODNAME).o

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -fr *.mod.* *.o  .$(MODNAME).* .tmp* Module.* modules.*


