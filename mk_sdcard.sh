#!/bin/bash

# usage: bash mk_sdcard.sh /dev/sdb u-boot.imx

#sudo sfdisk ${1} <<EOF
#label: dos
#16065,128520,0x0C,*
#144585,,,-
#EOF

sudo sfdisk ${1} < sd.layout --force

sudo mkfs.fat -F32 -v -n "BOOT" ${1}2

sudo mkfs.ext4 -F -L "rootfs" ${1}3

sudo dd if=${2} of=${1} bs=512 seek=2

sync
