#!/bin/bash

INITRD="initrd"
Version="5.14.0" #"$(uname -r)"
LoopDev=$(sudo losetup -f)

function GenRootFs()
{
	InitRd=$1
	if [[ -d ${InitRd} ]];then
		rm -rf ${InitRd}
	fi
	mkdir ${InitRd}
	pushd ${InitRd}
	InitRdFile="/boot/initrd.img-${Version}"
	InitRdPkg="./initrd.img.gz"
	if [[ -f ${InitRdFile} ]];then
		cp ${InitRdFile} ${InitRdPkg}
	else
		InitRdFile="${InitRdFile}-generic"
		cp ${InitRdFile} ${InitRdPkg}
	fi 
	echo -e "\033[32m InitRdFile:${InitRdFile} \033[0m"
	FileType=$(file initrd.img.gz | grep "cpio")
	if [[ -z $FileType ]];then
		gunzip ${InitRdPkg}
	else
		mv ${InitRdPkg} initrd.img
	fi
	cpio -ivmd < initrd.img
	popd
}

function VmLinuZToVmLinux()
{
	VmLinuZ="/boot/vmlinuz-${Version}"
	if [[ ! -f ${VmLinuZ} ]];then
		VmLinuZ="${VmLinuZ}-generic"
	fi
	echo -e "\033[32m VmLinuZ:$VmLinuZ \033[0m"
	echo "sudo od -A d -t x1 ${VmLinuZ} | grep '1f 8b 08 00'"
	odCtx=$(sudo od -A d -t x1 ${VmLinuZ} | grep '1f 8b 08 00')
	echo "odCtx:${odCtx}"
	StartCur=$(echo ${odCtx} | cut -d' ' -f1)
	echo "StartCur:$StartCur"
	odCtx=${odCtx%%1f*}
	OffsetWord=$(echo ${odCtx} | wc | awk '{print $2}')
	OffsetWord=$(expr ${OffsetWord} - 1)
	echo "OffsetWord:$OffsetWord"
	StartCur=$(expr ${StartCur} + ${OffsetWord})
	echo "StartCur:$StartCur"
	echo "dd if=$VmLinuZ bs=1 skip=$StartCur | zcat > vmlinux"
	dd if=$VmLinuZ bs=1 skip=$StartCur | zcat > vmlinux
}

function CreateDiskImg()
{
	echo -e "\033[32m $FUNCNAME:$LINENO \033[0m"
	DiskImg="Start.img"
	touch $DiskImg
	dd if=/dev/zero of=./$DiskImg bs=1M count=100
	echo -e "n\np\n1\n\n\nw" | fdisk ./$DiskImg # // disk.txt
	hd ./$DiskImg -n 512
}

function CreateRootFs()
{
	echo -e "\033[32m $FUNCNAME:$LINENO \033[0m"
	DiskImg="Start.img"
	echo "DiskImg:$DiskImg"
	echo "LoopDev:$LoopDev"
	LoopDevPart=${LoopDev/dev/dev\/mapper}p1
	echo "sudo losetup $LoopDev $DiskImg"
	sudo losetup $LoopDev $DiskImg
	echo "sudo kpartx -av $LoopDev"
	sudo kpartx -av $DiskImg 
	sudo losetup -l
	ls ${LoopDevPart}
	echo -e "\033[32m sudo mkfs.ext4 ${LoopDevPart} \033[0m"
	sudo mkfs.ext4 ${LoopDevPart}
	RootFs="rootfs"
	if [[ -d ${RootFs} ]];then
		rm ${RootFs} -rf
	fi
	mkdir $RootFs
	sudo mount ${LoopDevPart} $RootFs
	#cp -a busybox/_install/* /rootfs/
}

function RemoveLoopDev()
{
	echo -e "\033[32m $FUNCNAME:$LINENO \033[0m"
	LoopDev=$1
	LoopDevPart=${LoopDev/dev/dev\/mapper}p1
	echo "sudo umount $LoopDevPart"
	sudo umount $LoopDevPart
	echo "sudo dmsetup remove $LoopDevPart"
	sudo dmsetup remove $LoopDevPart
	echo "sudo losetup -d $LoopDev"
	sudo losetup -d $LoopDev
}

function RemoveLoopDevAll()
{
	echo -e "\033[32m $FUNCNAME:$LINENO \033[0m"
	LoopDevList=$(sudo losetup -l | grep "Start.img" | cut -d' ' -f1)
	for dev in $LoopDevList
	do
		echo "RemoveLoopDev $dev"
		RemoveLoopDev "$dev"
	done
}

function InstallBootLoader()
{
	echo -e "\033[32m $FUNCNAME:$LINENO \033[0m"
	RootFs="rootfs"
	RootFs="$(pwd)/$RootFs"
	echo "LoopDev:$LoopDev,RootFs:$RootFs"
	sudo grub-install --root-directory=$RootFs $LoopDev
	hd $DiskImg -n 512 
}

GenRootFs "${INITRD}"
VmLinuZToVmLinux 
CreateDiskImg 
CreateRootFs
InstallBootLoader 
RemoveLoopDevAll


