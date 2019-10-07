#!/bin/bash
export PATH=/opt/gcc-arm-8.3-2019.03-x86_64-arm-eabi/bin:$PATH
export CROSS_COMPILE='ccache arm-eabi-'
export ARCH=arm
export KDIR=/home/rpiskun/repos/linux-stable
export ROOTFS=/home/rpiskun/repos/busybox/_install
export BUSYBOX_PATH=/home/rpiskun/repos/busybox
