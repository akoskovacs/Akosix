#include <types.h>
#include <multiboot.h>
#include <memory.h>
#include <kernel.h>

void memory_init(struct multiboot_info *mbi)
{
    /* Make it shorter */
    typedef struct multiboot_mmap_entry mb_mmap_t;

    uint64_t length_addr;
    unsigned int size;
    const char *avail = NULL;
    mb_mmap_t *mmap = (mb_mmap_t *)mbi->mmap_addr;
    kheap_start = &__end_data_kernel;

    if (!(mbi->flags & MB_INFO_MEM_MAP)) {
        kprintf("Fatal error! No memory mapping information found!\n");
    }

    while (mmap < (mb_mmap_t *)((uint32_t)mbi->mmap_addr + mbi->mmap_length)) {
        length_addr = mmap->addr + mmap->length;
        size = mmap->length/1024;
        kprintf("\t\t- 0x%X%X - 0x%X%X, size: %dKB [%s]\n"
        , mmap->addr, length_addr, size
        , (mmap->type == MB_MEMORY_RESERVED) ? "reserved" : "available");

        if (mmap->addr == 0x100000 
                && mmap->type == MB_MEMORY_AVAILABLE) {
            pmm_init(mmap->addr, mmap->length);
            kheap_max = VADDR((uint32_t)length_addr);
        }

        mmap = (mb_mmap_t *)((uint32_t)mmap
            + mmap->size + sizeof(mmap->size));
    }
}

void *expand_kheap(size_t size)
{
    void *alloc = NULL;
    if (kheap_end + size > kheap_max) {
       return NULL;
    }
    alloc = (void *)kheap_end;
    kheap_end += size;
    return alloc;
}

void *contract_kheap(size_t size)
{
    if (kheap_end - size < kheap_start) {
        return NULL;
    }
    kheap_end -= size;
    return (void *)kheap_end;
}
