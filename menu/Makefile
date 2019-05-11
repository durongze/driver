#
# Makefile for Menu Program
#

CC_PTHREAD_FLAGS			 = -lpthread
CC_FLAGS                     = -c 
CC_OUTPUT_FLAGS				 = -o
CC                           = gcc
RM                           = rm
RM_FLAGS                     = -f
QEMU=qemu-system-x86_64

TARGET  =   test
OBJS    =   lib/linktable.o  lib/menu.o lib/proc.o

INC_PATH+=-Ilib

ARCH=x64

KERNEL_DIR=../linux-5.0.1
IMAGE=$(KERNEL_DIR)/arch/x86_64/boot/bzImage
ROOTFS=../rootfs.img

ifeq ($(ARCH),)
	CFLAGS += -m32
else
	CFLAGS += -m64
endif

all: $(OBJS)
	$(CC) $(CC_OUTPUT_FLAGS) $(TARGET) init_test.c $(OBJS) ${INC_PATH}
rootfs: $(OBJS)
	$(CC) -o init init_reply.c $(OBJS) $(CFLAGS) -static -lpthread $(INC_PATH)
	$(CC) -o hello app_hello.c $(CFLAGS) -static $(INC_PATH)
	find init hello | cpio -o -Hnewc |gzip -9 > $(ROOTFS)
	$(QEMU) -kernel $(IMAGE) -initrd $(ROOTFS)
%.o:%.c
	$(CC) $(CC_FLAGS) $< -o $@ $(INC_PATH)

clean:
	$(RM) $(RM_FLAGS)  $(OBJS) $(TARGET) *.bak
