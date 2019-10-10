#!/bin/bash
DEST_PATH=lesson6
sudo rm -rf $ROOTFS/lib/modules/$DEST_PATH
sudo mkdir $ROOTFS/lib/modules/$DEST_PATH
sudo cp irq_handling.ko $ROOTFS/lib/modules/$DEST_PATH

