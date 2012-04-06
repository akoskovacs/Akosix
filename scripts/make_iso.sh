#!/bin/bash

cd ..
mkdir -p dist/boot/grub
cp scripts/grub.cfg dist/boot/grub
cp akx_kernel dist/boot/
grub-mkrescue -o akosix.iso dist/
