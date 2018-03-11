MODNAME=rwlock
ifeq ($(PLATFORM),PC)
	KERNELDIR=/lib/modules/$(shell uname -r)/build
else
	KERNELDIR=/home/pi/linux 
endif

obj-m += $(MODNAME).o

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -fr *.mod.* *.o  .$(MODNAME).* .tmp* Module.* modules.*


