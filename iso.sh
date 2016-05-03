#!/bin/sh
set -e
. ./build.sh

# prepare folder structure
mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

# copy kernel into place
cp sysroot/boot/rho.kernel isodir/boot/rho.kernel

# create grub menu
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "rho" {
	multiboot /boot/rho.kernel command line options are set in iso.sh
}
EOF

# create cd image and disk image
grub-mkrescue --product-name=Rho -o rho.iso isodir
grub-mkrescue --product-name=Rho -o rho.img isodir
