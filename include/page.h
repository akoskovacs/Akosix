#ifndef PAGE_H
#define PAGE_H

#include <types.h>
#include <basic.h>
#define PAGE_SIZE 4096

struct page {
    uint32_t present  : 1;
    uint32_t rw       : 1;
    uint32_t user     : 1;
    uint32_t accessed : 1;
    uint32_t dirty    : 1;
    uint32_t unused   : 7;
    uint32_t frame    : 20;
};

struct page page_table[1024] __align(PAGE_SIZE);
struct page page_directory[1024] __align(PAGE_SIZE);

#endif // PAGE_H
