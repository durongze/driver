#!/bin/bash
Kernel="5.4.124  5.4.124.old"

function RemoveKernel()
{
    for KernelVersion in $Kernel
	do
        sudo rm /boot/vmlinuz-${KernelVersion}
        sudo rm /boot/initrd.img-${KernelVersion}
        sudo rm /boot/System.map-${KernelVersion}
        sudo rm /boot/config-${KernelVersion}
        sudo rm -rf /lib/modules/${KernelVersion}
    done
}

RemoveKernel "$Kernel"
