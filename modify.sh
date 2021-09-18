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
        git checkout $mk
        echo "sed '/KERNELDIR=/,/EXTRA_FLAGS=/{/KERNELDIR/!d}' -i $mk"
        sed -e "/KERNELDIR=/,/EXTRA_CFLAGS=/{/KERNELDIR=/!d}" -i $mk
        sed -e '/KERNELDIR=.*/aEXTRA_CFLAGS=-Dsimple_DEBUG' -i $mk
        sed -e 's/EXTRA_CFLAGS=-Dsimple_DEBUG/       EXTRA_CFLAGS=-Dsimple_DEBUG/g' -i $mk
        echo "sed -e '/clean:/a\\trm *.dwo .*.symvers.cmd .*.order.cmd *.ko *.mod' -i $mk"
        sed -e '/clean:/a\\trm -fr *.dwo .*.symvers.cmd .*.order.cmd *.ko *.mod' -i $mk
        sed -e 's#KERNELDIR=/home/pi/linux#KERNELDIR=$(shell echo ${HOME})/linux#g' -i $mk
    done 
}

function ModifyPrintHeader()
{
    CodeDir=$1
    HeaderFile=$(find $CodeDir -iname "demo.h")
    for HFile in $HeaderFile
    do
        sed -e 's/KERN_DEBUG "DEMO: " fmt,/"%s:[%s:%d]" fmt, KBUILD_MODNAME, __FUNCTION__, __LINE__,/g' -i $HFile
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

function GenVimrcFile()
{
	VimrcFile=${HOME}/.vimrc
	echo "set nu" > ${VimrcFile}
	echo "set tabstop=4" >> ${VimrcFile}
	echo "set tags=~/code/kernel/driver/ubuntu-focal/tags" >> ${VimrcFile}
}

#ModifyMakeFile "src"
#GenVimrcFile
ModifyPrintHeader "src"
