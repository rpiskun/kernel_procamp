#!/bin/bash
DEST_PATH=lesson4
mkdir $ROOTFS/lib/modules/$DEST_PATH
cp hrtimer.ko $ROOTFS/lib/modules/$DEST_PATH
pushd .
cd $ROOTFS
find . | cpio -o -H newc | gzip > ../rootfs.cpio.gz
popd
