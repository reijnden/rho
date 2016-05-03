#!/bin/sh
set -e
. ./iso.sh

#
# qemu max memory 2047!
#
# read boot disk from command line
# d = cdrom
# c = hard-disk
qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom rho.iso -m 1024 -display curses -hda rho.img -boot order=$1
