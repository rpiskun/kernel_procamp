#!/bin/bash
DEST_PATH=lesson3
rm -rf $ROOTFS/lib/modules/$DEST_PATH
mkdir $ROOTFS/lib/modules/$DEST_PATH
cp hello.ko $ROOTFS/lib/modules/$DEST_PATH
pushd .
cd $ROOTFS
find . | cpio -o -H newc | gzip > ../rootfs.cpio.gz
popd
