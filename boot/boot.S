#########################################################################
#   Copyright (c) 2012 Ákos Kovács - Akosix operating system
#              http://akoskovacs.github.com/Akosix
# 
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
#########################################################################
.global loader
.extern higher_stack

#include <config.h>

.section .setup
.set ALIGN,    0x001
.set MEMINFO,  0x002
.set VBEINFO,  0x004
.set PAGE_OFFSET, 0xC0000000
#ifdef CONFIG_ENABLE_VBE
.set FLAGS,    ALIGN | MEMINFO | VBEINFO
#else
.set FLAGS,    ALIGN | MEMINFO
#endif

.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)
.set STACKSIZE, CONFIG_STACK_SIZE  # 8kb by default

.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
#ifdef CONFIG_ENABLE_VBE
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long CONFIG_VBE_WIDTH
.long CONFIG_VBE_HEIGHT
.long CONFIG_VBE_DEPTH
#endif

.extern setup_pd

loader:
    mov   $(kernel_stack + STACKSIZE), %esp
    addl  $PAGE_OFFSET, %ebx
    pushl %eax
    pushl %ebx
    call  enable_paging
# Higher-half address of the stack
    addl  $PAGE_OFFSET, %esp 
    call  kmain
    jmp   hang

enable_paging:
    call  setup_pd
# The %eax contains the physical address of the
# first page directory, then move it to %cr3
    movl  %eax, %cr3
    movl  %cr0, %eax
# Enable paging, by setting the 31th bit of
# the %cr0 control register
    orl   $0x80000000, %eax
    movl  %eax, %cr0
ret

hang:
    hlt
    jmp   hang

# reserve 8KB stack on a quadword boundary, for lower-half
.comm kernel_stack, STACKSIZE, 32
