#!/bin/bash

function ExportConfig()
{
    modprobe configs
    zcat /proc/config.gz
}

function ModifyMakeFile()
{
    # find ./ -iname "Makefile" | xargs -I {}  sed '/.*KERNELDIR=/{n;/EXTRA_FLAGS=/ba;s!.*!EXTRA_CFLAGS=-Dsimple_DEBUG\n&!;:a}' -i {} 
    mkFile=$(find ./ -iname "Makefile")
    for mk in $mkFile
    do
        echo "sed '/.*KERNELDIR=/{n;/EXTRA_FLAGS=/ba;s!.*!EXTRA_CFLAGS=-Dsimple_DEBUG\n&!;:a}' -i $mk"
        sed '/.*KERNELDIR=/{n;/EXTRA_FLAGS=/ba;s!.*!EXTRA_CFLAGS=-Dsimple_DEBUG\n&!;:a}' -i $mk
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

ModifyMakeFile 
