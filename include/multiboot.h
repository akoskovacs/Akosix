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
#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <basic.h>

enum multiboot_flags {
    MB_INFO_MEMORY        = 0x0001,
    MB_INFO_BOOTDEV       = 0x0002,
    MB_INFO_CMDLINE       = 0x0004,
    MB_INFO_MODS          = 0x0008,
    MB_INFO_ELF_SECT      = 0x0020,
    MB_INFO_MEM_MAP       = 0x0040,
    MB_INFO_DRIVE_INFO    = 0x0080,
    MB_INFO_CONFIG_TABLE  = 0x0100,
    MB_INFO_BOOT_LOADER   = 0x0200,
    MB_INFO_APM_TABLE     = 0x0400,
    MB_INFO_VIDEO         = 0x0800
};

struct __packed multiboot_elf_section {
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
};

struct __packed multiboot_mmap_entry {
    uint32_t size;
    uint64_t addr;
    uint64_t length;
#define MB_MEMORY_AVAILABLE 1
#define MB_MEMORY_RESERVED 2
    uint32_t type;
};

struct __packed multiboot_module {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t cmdline;
    uint32_t pad;
};

struct __packed multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    struct   multiboot_elf_section elf_sec;
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drivers_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint16_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
};

#endif // MULTIBOOT_H
