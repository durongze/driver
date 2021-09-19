#!/bin/bash

# 取回自己为树莓派定制的Linux内核源码树

export PLATFORM=PC

UserInput=`echo -ne "\033"`

KernelCodeName="$(lsb_release --codename | cut -f2)"
KernelDir="ubuntu-${KernelCodeName}"

function GetUserInput()
{
    akey=(0 0 0)
    cESC=`echo -ne "\033"`

    while :
    do
        #这里要注意，这里有-n选项，后跟一个数字，指定输入的字符长度最大值，
        #所以不管key变量的值有多少个字符，都是一个值一个值做循环， 有多少个值就循环多少次
        #输入键盘的上下左右键时，都有三个字符(ESC键算一个)，所以都是做三次循环，
        #每做一次循环，akey的值都会发生改变
        read -s -n 1 key

        akey[0]=${akey[1]}
        akey[1]=${akey[2]}
        akey[2]=${key}

        if [[ ${key} == ${cESC} && ${akey[1]} == ${cESC} ]];then
            echo "ESC键"
        elif [[ ${akey[0]} == ${cESC} && ${akey[1]} == "[" ]];then
            if [[ ${key} == "A" ]];then echo "上键"
            elif [[ ${key} == "B" ]];then echo "向下"
            elif [[ ${key} == "D" ]];then echo "向左"
            elif [[ ${key} == "C" ]];then echo "向右"
            fi
        fi
    done
}

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
    UserInput=$2
    pushd $KernelDir >> /dev/null
        git fetch --unshallow
        git fetch --tags
        cp /boot/config-`uname -r` .config
        sudo make menuconfig 
        sed -i 's/CONFIG_SYSTEM_TRUSTED_KEYS=.*/CONFIG_SYSTEM_TRUSTED_KEYS=""/g' .config
        #sed -i 's/CONFIG_DEBUG_INFO_BTF=y/CONFIG_DEBUG_INFO_BTF=n/g' .config
        cp drivers/tty/vt/defkeymap.c_shipped drivers/tty/vt/defkeymap.c
        sudo make -j 8 
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

function RunModule()
{
	KoFile=$(ls *.ko)
	for kf in $KoFile
    do
        KoName=${kf%.*}
        sudo insmod $kf
        echo "KoName:$KoName"
		sudo rmmod $KoName
    done
}

function CompileModule()
{
    ModDir=$1
    echo -e "\033[32m $FUNCNAME $ModDir \033[0m"
    pushd $ModDir >> /dev/null
        Log=$(make -f Makefile)
        if [ $? -ne 0 ];then
           echo "make : $Log"  
           exit
        else
           RunModule
        fi
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
    ModsDir="$(find ${RootDir} -iname "Makefile" | sort -u)"
    for mod in $(echo $ModsDir | sort)
    do
        mod=${mod%/*}
        CompileModule "$mod"
    done
}

function InstallKernel()
{
    pushd ubuntu-${KernelCodeName} >> /dev/null
        sudo make modules_install
        sudo make install
        sudo mkinitramfs -o /boot/initrd.img-5.4.124
        sudo update-initramfs -c -k 5.4.124
        sudo update-grub2
        #sudo vi /boot/grub/grub.cfg # set timeout=30
    popd >> dev/null
}

function FindKeyBoardDriver()
{
	DriverInfo=$(lspci -v | grep -A 5 "USB" | grep "driver" | cut -d":" -f2 | xargs -I {} modinfo {})
	echo $DriverInfo
	echo -e "\033[31m MODULE_SOFTDEP MODULE_LICENSE MODULE_DESCRIPTION \033[0m"
	echo "make menuconfig -> \"/\" -> \"name\" -> path(${KernelDir}/drivers/usb/host)"
	echo "make menuconfig -> \"/\" -> DYNAMIC_DEBUG -> ${KernelDir}/Documentation/filesystems/debugfs.txt"
	echo "sudo cat /sys/kernel/debug/dynamic_debug/control"
}

function InstallCompileTool()
{
    sudo apt install libelf-dev dwarves 
}

#GetUserInput 
DownloadKernel
InstallCompileTool
CompileKernel "$KernelDir" 
CompileAllModule "src" "${UserInput}"
FindKeyBoardDriver
