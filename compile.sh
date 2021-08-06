#!/bin/bash

# 取回自己为树莓派定制的Linux内核源码树

export PLATFORM=PC

KernelCodeName="$(lsb_release --codename | cut -f2)"

function ExportKernelConfig()
{
    cat /boot/config-`uname -r`

    sudo modprobe configs

    zcat /proc/config.gz 
}

function DownloadKernel()
{
	echo $FUNCNAME
    echo "git clone --depth=1 git://kernel.ubuntu.com/ubuntu/ubuntu-${KernelCodeName}.git"
    git clone --depth=1 git://kernel.ubuntu.com/ubuntu/ubuntu-${KernelCodeName}.git
}

function DownloadRpiKernel()
{
	echo $FUNCNAME
    git clone --branch rpi-4.9.y-devel3 --depth=1 https://github.com/liuqun/linux.git
}

function CompileKernel()
{
	echo $FUNCNAME
	KernelDir=$1
	
	pushd $KernelDir >> /dev/null
    	cp /boot/config-`uname -r` .config
		make
	popd >> /dev/null
}

function CompileRpiKernel()
{
    pushd linux

    # 载入树莓派默认内核配置模板 bcm2709_defconfig
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcm2709_defconfig
    
    # 进入 menuconfig 菜单 
    # 进入子菜单Device Drivers--->Character Devices--->TPM Hardware support--->
    # 选中模块 TPM Interface Spectification 1.3 Interface / TPM 2.0 FIFO Interface (SPI)
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j $(nproc) bzImage dtbs modules

    scripts/mkknlimg arch/arm/boot/zImage kernel7.img
    mkdir -p dtb-files/overlays
    cp arch/arm/boot/dts/*.dtb dtb-files
    cp arch/arm/boot/dts/overlays/*.dtb* dtb-files/overlays

    ## 驱动模块 
    # make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules_install INSTALL_MOD_PATH=`pwd`/rootfs
    #

    popd
}

function CompileModule()
{
    ModDir=$1
    echo -e "\033[32m $FUNCNAME $ModDir \033[0m"
    pushd $ModDir >> /dev/null
    	make clean 
    popd >> /dev/null
}

function ListPcAllModule()
{
    HostKoDir="/lib/modules/$(uname -r)/kernel"
    echo "HostKoDir: ${HostKoDir}"
    ls ${HostKoDir}
}

function CompileAllModule()
{
    RootDir=$1
    ModsDir="$(find ${RootDir} -iname "Makefile")"
    for mod in $ModsDir
    do
        mod=${mod%/*}
        CompileModule "$mod"
    done
}

DownloadKernel
CompileKernel "ubuntu-${KernelCodeName}"
CompileAllModule "src"
