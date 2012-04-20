#!/bin/bash

mkdir -p dist/boot/grub
cp scripts/grub.cfg dist/boot/grub
cp akosix.bin dist/boot/
grub-mkrescue -o akosix.iso dist
