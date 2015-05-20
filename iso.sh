#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/rho.kernel isodir/boot/rho.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "rho" {
	multiboot /boot/rho.kernel --debug=1
}
EOF
grub-mkrescue -o rho.iso isodir
