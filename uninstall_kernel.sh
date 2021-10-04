#!/bin/bash
#ver_no="5.4.128"
#ver_no="5.11.0-34"
ver_no="5.14.0"

function RemoveBootCfg()
{
    ver_no="$1"
    suffix="$2"

    echo "${ver_no}${suffix}"

    sudo rm /boot/vmlinuz-${ver_no}${suffix}
    sudo rm /boot/initrd.img-${ver_no}${suffix}
    sudo rm /boot/System.map-${ver_no}${suffix}
    sudo rm /boot/config-${ver_no}${suffix}
    sudo rm /lib/modules/${ver_no}${suffix} -rf
}

ver_no="5.14.0"
suffix="-generic"
suffix=""

RemoveBootCfg "$ver_no" "$suffix"

#sudo update-grub

