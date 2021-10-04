#!/bin/bash
#ver_no="5.4.128"
ver_no="5.11.0-34"
sudo rm /boot/vmlinuz-${ver_no}-generic
sudo rm /boot/initrd.img-${ver_no}-generic
sudo rm /boot/System.map-${ver_no}-generic
sudo rm /boot/config-${ver_no}-generic
sudo rm /lib/modules/${ver_no}-generic -rf
#sudo update-grub

