#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include <types.h>
#include <basic.h>

struct gdt_entry {
    uint16_t low_limit;
    uint16_t low_base;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  high_based;
} __packed;

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __packed;

struct gdt_entry gdt[3];
struct gdt_ptr;

#endif // DESCRIPTORS_H
