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
 ************************************************************************/
#include <kernel.h>
#include <memory.h>
#include <multiboot.h>
#include <types.h>
#include <panic.h>

void memory_init(struct multiboot_info *mbi)
{
    /* Make it shorter */
    typedef struct multiboot_mmap_entry mb_mmap_t;

    uint64_t length_addr;
    unsigned int size;
    const char *avail = NULL;
    mb_mmap_t *mmap = (mb_mmap_t *)mbi->mmap_addr;
    kheap_start = (uint32_t)&__end_data_kernel;
    kheap_end = kheap_start;

    if (!(mbi->flags & MB_INFO_MEM_MAP)) {
        kpanic("Fatal! No memory mapping information found!\n");
    }

    while (mmap < (mb_mmap_t *)((uint32_t)mbi->mmap_addr + mbi->mmap_length)) {
        length_addr = mmap->addr + mmap->length;
        size = mmap->length/1024;
        if (size != 0 && mmap->addr != 0 && length_addr != 0)
            kprintf("\t\t- 0x%X%X - 0x%X%X, size: %dKB [%s]\n"
                , mmap->addr, length_addr, size
                , (mmap->type == MB_MEMORY_RESERVED) ? "reserved" : "available");

        if (mmap->addr == 0x100000 
                && mmap->type == MB_MEMORY_AVAILABLE) {
            kprintf("Got address!\n");
            pmm_init(mmap->addr, mmap->length);
            kheap_max = (uint32_t)VADDR((uint32_t)length_addr);
        }

        mmap = (mb_mmap_t *)((uint32_t)mmap
            + mmap->size + sizeof(mmap->size));
    }
}

void *expand_kheap(size_t size)
{
    uint32_t new_addr = kheap_end + size;    
    return (void *)new_addr;
}
