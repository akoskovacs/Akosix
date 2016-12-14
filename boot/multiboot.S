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

#include <config.h>

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

.section .data:
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0 # header_addr
.long 0 # load_addr
.long 0 # load_end_addr
.long 0 # bss_end_addr
.long 0 # entry_addr
.long 0 # mode_type
#ifdef CONFIG_ENABLE_VBE
.long CONFIG_VBE_WIDTH
.long CONFIG_VBE_HEIGHT
.long CONFIG_VBE_DEPTH
#endif

