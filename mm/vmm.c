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
#include <types.h>
#include <page.h>
#include <multiboot.h>
#include <string.h>

static pde_t *kpage_directory;
/* Virtual address to page directory index */
#define vaddr_to_pdind(addr) ((addr) >> 22)

/*
 * Again, do about the same thing as boot/pgsetup.c, but without the
 * identity mapping. After that we can use more generic functions.
 */
void vm_init(uint32_t start_address, uint32_t length)
{
    int i;
    kpage_directory = (pde_t *)PAGE_ALIGN(expand_kheap(PAGE_SIZE) + PAGE_SIZE);
    pte_t *kpage_table = (pte_t *)PAGE_ALIGN(expand_kheap(PAGE_SIZE) + PAGE_SIZE);
    memclr(kpage_directory, PAGE_SIZE);
    memclr(kpage_table, PAGE_SIZE);
    for (i = 0; i < PAGE_TABLE_SIZE; i++) {
        kpage_table[i] = (i * PAGE_SIZE) | PG_RW | PG_PRESENT;
    }
    kpage_directory[vaddr_to_pdind(PAGE_OFFSET)] = kpage_table;    
    vmm_switch_pdir(kpage_directory);
}

void vm_map_page(pde_t *dir, page_t *page, vaddr_t addr)
{

}

void vm_map_range(pde_t *pdir, vaddr_t vstart, size_t pg_count, paddr_t paddr)
{

}

pde_t *vm_create_pdir(paddr_t saddr, size_t len, vaddr_t vaddr, page_flags_t flags)
{
   pde_t *dir = (pde_t *)get_page(); 
}

void vm_switch_pdir(pde_t *pdir)
{
    paddr_t phys_pd = PADDR(pdir);
    write_cr3(phys_pd);
}

page_t get_free_page(page_flags_t flags)
{
   page_t page;
   pmm_alloc_frame(&page);
   SET_PAGE_FLAGS(page, flags);
   return page;
}

int kmap_page(page_t *)
{
}

int kmap_pages(page_t *, unsigned count)
{

}

page_t *vm_addr_to_page(pde_t *pdir, void *ptr)
{
    vaddr_t addr = (vaddr_t)ptr;
    pde_t table = kpage_directory[vaddr_to_pdind(addr)];
    if (table & PD_PRESENT) {
        pte_t *pt_addr = (pte_t *)VADDR(table >> 22);
        if (pt_addr != NULL) {
            return (page_t *)&pt_addr[addr & 0x3FF000];
        }
    }

}
page_t *addr_to_page(void *ptr)
{
    return vm_addr_to_page(kpage_directory, ptr);
}
