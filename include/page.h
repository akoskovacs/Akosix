#ifndef PAGE_H
#define PAGE_H

#include <types.h>
#include <basic.h>

#define IS_PAGE_PRESENT(p) (((p) & PT_PRESENT)?true:false)
#define IS_PAGE_WRITEABLE(p) (((p) & PT_RW)?true : false)
#define IS_PAGE_RO(p) !IS_PAGE_WRITEABLE(p)
#define IS_PAGE_USER(p) (((p) & PT_USER)?true:false)
#define IS_PAGE_SYSTEM(p) !IS_PAGE_USER(p)
#define IS_PAGE_DIRTY(p) (((p) & PT_DIRTY)?true:false)
#define IS_PAGE_WRITE_THROUGH(p) (((p) & PT_CACHE_DISABLE)?true:false)
#define IS_PAGE_WRITE_BACK(p) !IS_PAGE_WRITE_THROUGH(p)
#define PAGE_FRAME(p) ((p) >> PAGE_SHIFT)

#define SET_PAGE_FRAME(p, f) (p |= ((f) << PAGE_SHIFT))
#define SET_PAGE_PRESENT(p) ((p) |= PT_PRESENT)
#define SET_PAGE_WRITEABLE(p) ((p) |= PT_RW)
#define SET_PAGE_RO(p) ((p) &= PT_RW)
#define SET_PAGE_USER(p) ((p) |= PT_USER)
#define SET_PAGE_SYSTEM(p) ((p) &= PT_USER)
#define SET_PAGE_DIRTY(p) ((p) |= PT_USER)

#define CLEAR_PAGE_PRESENT(p) ((p) &= PT_PRESENT)
#define CLEAR_PAGE_WRITEABLE(p) ((p) &= PT_RW)
#define CLEAR_PAGE_DIRTY(p) ((p) &= PT_DIRTY)

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
    PT_PAT           = 0x80,
    PT_GLOBAL        = 0x100  /* Global translation */
    /* 11-9 ignored */
    /* 12-32 physical address of the page */
};

typedef enum PTE_FLAGS page_flags_t;

#endif // PAGE_H
