#!/bin/bash

function ExportConfig()
{
    modprobe configs
    zcat /proc/config.gz
}

function ModifyMakeFile()
{
    CodeDir=$1
    # find ./ -iname "Makefile" | xargs -I {}  sed '/.*KERNELDIR=/{n;/EXTRA_FLAGS=/ba;s!.*!EXTRA_CFLAGS=-Dsimple_DEBUG\n&!;:a}' -i {} 
    mkFile=$(find $CodeDir -iname "Makefile")
    for mk in $mkFile
    do
        echo "sed '/.*KERNELDIR=/{n;/EXTRA_FLAGS=/ba;s!.*!EXTRA_CFLAGS=-Dsimple_DEBUG\n&!;:a}' -i $mk"
        sed '/.*KERNELDIR=/{n;/EXTRA_FLAGS=/ba;s!.*!EXTRA_CFLAGS=-Dsimple_DEBUG\n&!;:a}' -i $mk
        echo "sed -e '/clean:/a\\trm *.dwo .*.symvers.cmd .*.order.cmd *.ko *.mod' -i $mk"
        sed -e '/clean:/a\\trm -fr *.dwo .*.symvers.cmd .*.order.cmd *.ko *.mod' -i $mk
        sed -e 's#KERNELDIR=/home/pi/linux#KERNELDIR=$(shell echo ${HOME})/linux#g' -i $mk
    done 
}

function DebugVmlinux()
{
    echo ttyS0,115200 > /sys/module/kgdboc/parameters/kgdboc
    echo g > /proc/sysrq-trigger

    gdb vmlinux
    #  >set serial baud 115200
    #  >target remote /dev/ttyUSB0
}

ModifyMakeFile "src"
