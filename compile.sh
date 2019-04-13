#!/bin/sh

# 取回自己为树莓派定制的Linux内核源码树
git clone --branch rpi-4.9.y-devel3 --depth=1 https://github.com/liuqun/linux.git
pushd linux

# 载入树莓派默认内核配置模板 bcm2709_defconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcm2709_defconfig

# 进入 menuconfig 菜单 
# 进入子菜单Device Drivers--->Character Devices--->TPM Hardware support--->
# 选中模块 TPM Interface Spectification 1.3 Interface / TPM 2.0 FIFO Interface (SPI)
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig

make ARCH=arm \
    CROSS_COMPILE=arm-linux-gnueabihf- \
    -j $(nproc)\
    bzImage dtbs modules


scripts/mkknlimg arch/arm/boot/zImage kernel7.img
mkdir -p dtb-files/overlays
cp arch/arm/boot/dts/*.dtb dtb-files
cp arch/arm/boot/dts/overlays/*.dtb* dtb-files/overlays

## 驱动模块 
# make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules_install INSTALL_MOD_PATH=`pwd`/rootfs
#

popd

