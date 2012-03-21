#ifndef PAGE_H
#define PAGE_H

#include <types.h>
#include <basic.h>

struct page_entry {
    uint32_t present  : 1;
    uint32_t rw       : 1;
    uint32_t user     : 1;
    uint32_t accessed : 1;
    uint32_t dirty    : 1;
    uint32_t unused   : 7;
    uint32_t frame    : 20;
};

/* Bit flags for page-directory entries */
enum PDE_FLAGS {
    PD_PRESENT       = 0x01,
    PD_RW            = 0x02,
    PD_USER          = 0x04,
    PD_PWT           = 0x08, /* Page level write-through */
    PD_CACHE_DISABLE = 0x10,
    PD_ACESSED       = 0x20,
    /* 6th bit ignored */
    PD_4MB_PAGES     = 0x80
    /* 8th bit ignored */
    /* 12-32 physical address of page table */
};

/* Bit flags for page-table entries */
enum PTE_FLAGS {
    PT_PRESENT       = 0x01,
    PT_RW            = 0x02,
    PT_USER          = 0x04,
    PT_PWT           = 0x08, /* Page level write-through */
    PT_CACHE_DISABLE = 0x10,
    PT_ACESSED       = 0x20,
    PT_DIRTY         = 0x40,
    PT_PAT           = 0x40,
    PT_GLOBAL        = 0x80  /* Global translation */
    /* 11-9 ignored */
    /* 12-32 physical address of the page */
};

#endif // PAGE_H
