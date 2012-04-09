#ifndef MEMORY_H
#define MEMORY_H

#include <types.h>
#include <basic.h>
#include <page.h>

#define DEBUG
#define DEBUG_MALLOC 

#define ADDR_TO_PFN(n) (PAGE_ALIGN((n)) / PAGE_SIZE)
#define PFN_TO_ADDR(n) (PAGE_ALIGN((n)) / PAGE_SIZE)
typedef uint32_t pfn_t;
typedef uint32_t page_t;
struct multiboot_info;

void memory_init(struct multiboot_info *);

void *expand_kheap(size_t size);
void *contract_kheap(size_t size);

typedef enum {
    M_NORMAL   = 0x000, 
    M_ALIGNED  = 0x001,
    M_USER     = 0x002,
    M_ZEROED   = 0x003
} malloc_flags_t;

void *kmalloc(size_t, malloc_flags_t);
void kfree(void *);

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

/* debug */
#if defined DEBUG && defined DEBUG_MALLOC
void dump_kmallocs(void);
#endif

#endif // MEMORY_H
