#!/bin/bash

modprobe configs
zcat /proc/config.gz

find ./ -iname "Makefile" | xargs -I {}  sed '/.*KERNELDIR=/{n;/EXTRA_FLAGS=/ba;s!.*!EXTRA_CFLAGS=-Dsimple_DEBUG\n&!;:a}' -i {} 

echo ttyS0,115200 > /sys/module/kgdboc/parameters/kgdboc
echo g > /proc/sysrq-trigger

gdb vmlinux
#  >set serial baud 115200
#  >target remote /dev/ttyUSB0
