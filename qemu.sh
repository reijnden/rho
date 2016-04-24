#!/bin/sh
set -e
. ./iso.sh

#
# max memory 2047
#
qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom rho.iso -m 1024 -display curses
