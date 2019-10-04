#!/bin/bash
cp hello.ko $ROOTFS/lib/modules
pushd .
cd $ROOTFS
find . | cpio -o -H newc | gzip > ../rootfs.cpio.gz
popd
