#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/bizim.kernel isodir/boot/bizim.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "bizim" {
	multiboot /boot/bizim.kernel the cmdline is set in iso.sh
}
EOF
grub-mkrescue -o bizim.iso isodir
