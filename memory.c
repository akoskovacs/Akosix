#include <types.h>
#include <memory.h>

uint8_t higher_stack[16 * 1024];
uint32_t placement_address = 0;
extern char __end_data_kernel[];

void init_memory(void)
{
    if (placement_address == 0) {
        placement_address = (uint32_t)&__end_data_kernel;
    }
}

uint32_t kmalloc_internal(size_t sz, int align, uint32_t *phys)
{
    if (placement_address == 0) {
        placement_address = (uint32_t)&__end_data_kernel;
    }
    if (align == 1 && (placement_address & 0xFFFFF000)) {
        // Align it.
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys) {
        *phys = placement_address;
    }
    uint32_t tmp = placement_address;
    placement_address += sz;
    return tmp;
}

void *kmalloc(size_t size)
{
    return (void *)kmalloc_internal(size, 0, 0);
}

void *kmalloc_align(size_t size)
{
    return (void *)kmalloc_internal(size, 1, 0);
}

void *kmalloc_phys_align(size_t size, uint32_t *phys)
{
    return (void *)kmalloc_internal(size, 1, phys);
}
