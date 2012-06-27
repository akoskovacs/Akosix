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
#ifndef PAGE_H
#define PAGE_H

#include <types.h>
#include <basic.h>

#define IS_PAGE_PRESENT(p) (((p) & PG_PRESENT)?true:false)
#define IS_PAGE_WRITEABLE(p) (((p) & PG_RW)?true : false)
#define IS_PAGE_RO(p) !IS_PAGE_WRITEABLE(p)
#define IS_PAGE_USER(p) (((p) & PG_USER)?true:false)
#define IS_PAGE_SYSTEM(p) !IS_PAGE_USER(p)
#define IS_PAGE_DIRTY(p) (((p) & PG_DIRTY)?true:false)
#define IS_PAGE_WRITE_THROUGH(p) (((p) & PG_CACHE_DISABLE)?true:false)
#define IS_PAGE_WRITE_BACK(p) !IS_PAGE_WRITE_THROUGH(p)
#define PAGE_FRAME(p) ((p) >> PAGE_SHIFT)

#define SET_PAGE_FRAME(p, f) (p |= ((f) << PAGE_SHIFT))
#define SET_PAGE_PRESENT(p) ((p) |= PG_PRESENT)
#define SET_PAGE_WRITEABLE(p) ((p) |= PG_RW)
#define SET_PAGE_RO(p) ((p) &= PG_RW)
#define SET_PAGE_USER(p) ((p) |= PG_USER)
#define SET_PAGE_SYSTEM(p) ((p) &= PG_USER)
#define SET_PAGE_DIRTY(p) ((p) |= PG_USER)
#define SET_PAGE_FLAGS(p, f) ((p) |= (f))

#define CLEAR_PAGE_PRESENT(p) ((p) &= PG_PRESENT)
#define CLEAR_PAGE_WRITEABLE(p) ((p) &= PG_RW)
#define CLEAR_PAGE_DIRTY(p) ((p) &= PG_DIRTY)

#define PAGE_TABLE_SIZE 1024
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
    PG_PRESENT       = 0x01,
    PG_RW            = 0x02,
    PG_USER          = 0x04,
    PG_PWT           = 0x08, /* Page level write-through */
    PG_CACHE_DISABLE = 0x10,
    PG_ACESSED       = 0x20,
    PG_DIRTY         = 0x40,
    PG_PAT           = 0x80,
    PG_GLOBAL        = 0x100  /* Global translation */
    /* 11-9 ignored */
    /* 12-32 physical address of the page */
};
typedef uint32_t pde_t;
typedef uint32_t pte_t;
typedef enum PTE_FLAGS page_flags_t;

#endif // PAGE_H
