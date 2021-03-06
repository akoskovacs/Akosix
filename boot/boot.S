########################################################################
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
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
########################################################################
.global loader
.extern higher_stack

#include <config.h>
.set STACKSIZE, CONFIG_STACK_SIZE  # 8kb by default
.set PAGE_OFFSET, 0xC0000000

.extern setup_pd

.section setup
.align 4

loader:
    mov   $(kernel_stack+STACKSIZE), %esp
# 'Start of Stack' magic, later defined as SOF_STACK_MAGIC
#    pushl $0x514514C7
# 'End of Stack' magic, later defined as EOF_STACK_MAGIC
#    movl  $0xE0F514C7, (kernel_stack)
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
