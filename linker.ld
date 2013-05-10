/************************************************************************
 *   Copyright (c) 2012 Ákos Kovács - Akosix operating system
 *              http://akoskovacs.github.com/Akosix
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ************************************************************************/
OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY (loader)
hhalf = 0xc0000000;
page_size = 4096;

SECTIONS
{
    . = 0x00100000;
    .setup :
    {
    __start_setup = .;
        *(.setup)
        *(.setup.data)
        boot/boot.o(.bss)
        boot/boot.o(COMMON)
    __end_setup = .;
    }

    /* Higher half */
    . += 0xc0000000;
    .text : AT(ADDR(.text) - hhalf) 
    {
    __start_cs_init = .;
        *(.init.text)
    __end_cs_init = .;
        *(.text)
    }

    .rodata ALIGN(page_size) : AT(ADDR(.rodata) - hhalf)
    {
    __start_data_kernel = .;
        *(.rodata*)
    }

    .data ALIGN(page_size) : AT(ADDR(.data) - hhalf)
    {
        *(.data)
    }

    .bss ALIGN(page_size) : AT(ADDR(.bss) - hhalf)
    {
        sbss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
    __end_data_kernel = .;
    }

    .note ALIGN(page_size) : AT(ADDR(.note) - hhalf)
    {
        *(.note.gnu.build-id)
    }
}
