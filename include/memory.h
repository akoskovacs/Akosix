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
 *************************************************************************/
#ifndef MEMORY_H
#define MEMORY_H

#include <types.h>
#include <basic.h>
#include <page.h>

#define ADDR_TO_PFN(n) (((pfn_t)n) / PAGE_SIZE)
#define PFN_TO_ADDR(n) vaddr_t(n * PAGE_SIZE)
typedef uint32_t pfn_t;
typedef uint32_t page_t;
struct multiboot_info;

void memory_init(struct multiboot_info *);

void *expand_kheap(size_t size);
void *contract_kheap(size_t size);

typedef enum {
    M_NORMAL   = 0x001, 
    M_ALIGNED  = 0x002,
    M_USER     = 0x004,
    M_ZEROED   = 0x008
} malloc_flags_t;

void *kmalloc(size_t, malloc_flags_t);
void kfree(void *);

uint32_t kheap_start;
uint32_t kheap_end;
uint32_t kheap_max;
uint8_t __end_data_kernel;

/* pmm.c */

void pmm_init(uint32_t start_addr, uint32_t length);
void pmm_set_frame(pfn_t);
void pmm_clear_frame(pfn_t);
bool pmm_is_free_frame(pfn_t);
pfn_t pmm_first_frame(void);
pfn_t pmm_first_frames(unsigned int);
void pmm_alloc_frame(page_t *);
void pmm_free_frame(page_t *);

struct kmalloc_area;
void dump_kmallocs(void);
void dump_kmalloc_area(struct kmalloc_area *);
void dump_kmalloc(void *);

#endif // MEMORY_H
