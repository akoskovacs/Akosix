#ifndef MEMORY_H
#define MEMORY_H

#include <types.h>
#include <basic.h>
#include <page.h>

#define ADDR_TO_PFN(n) (PAGE_ALIGN((n)) / PAGE_SIZE)
#define PFN_TO_ADDR(n) (PAGE_ALIGN((n)) / PAGE_SIZE)
typedef uint32_t pfn_t;
typedef uint32_t page_t;
struct multiboot_info;

void memory_init(struct multiboot_info *);

void *expand_kheap(size_t size);
void *contract_kheap(size_t size);

void *kmalloc(size_t);
void *kmalloc_align(size_t);
void *kmalloc_phys(size_t, uint32_t *);
void *kmalloc_phys_align(size_t, uint32_t *);

vaddr_t kheap_start;
vaddr_t kheap_end;
vaddr_t kheap_max;
uint8_t __end_data_kernel;

/* pmm.c */

void pmm_init(uint32_t start_addr, uint32_t length);
void pmm_set_frame(pfn_t);
void pmm_clear_frame(pfn_t);
bool pmm_is_free_frame(pfn_t);
pfn_t pmm_first_frame(void);
pfn_t pmm_first_frames(int);
void pmm_alloc_frame(page_t *);
void pmm_free_frame(page_t *);

#endif // MEMORY_H
