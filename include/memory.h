#ifndef MEMORY_H
#define MEMORY_H

#include <types.h>
void init_memory(void);
void *kmalloc(size_t);
void *kmalloc_align(size_t);
void *kmalloc_phys(size_t, uint32_t *);
void *kmalloc_phys_align(size_t, uint32_t *);

#endif // MEMORY_H
