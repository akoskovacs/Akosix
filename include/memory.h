#ifndef MEMORY_H
#define MEMORY_H

#include <types.h>

struct multiboot_memory_map {
    uint32_t size;
    uint32_t base_low_addr;
    uint32_t base_high_addr;
    uint32_t low_length;
    uint32_t high_length;
    uint32_t type;
};

#endif // MEMORY_H
