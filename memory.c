#include <types.h>
#include <multiboot.h>
#include <memory.h>
#include <kernel.h>

static void *expand_kheap(size_t size);
static void *contract_kheap(size_t size);
static vaddr_t kheap_start;
static vaddr_t kheap_end;
static vaddr_t kheap_max;
static uint8_t __end_data_kernel;

void init_memory(struct multiboot_info *mbi)
{
    /* Make it shorter */
    typedef struct multiboot_mmap_entry mb_mmap_t;

    uint64_t length_addr;
    unsigned int size;
    long long addr;
    const char *avail = NULL;
    mb_mmap_t *mmap = (mb_mmap_t *)mbi->mmap_addr;
    kheap_start = &__end_data_kernel;

    if (!(mbi->flags & MB_INFO_MEM_MAP)) {
        kprintf("Fatal error! No memory mapping information found!\n");
    }

    while (mmap < (mb_mmap_t *)((uint32_t)mbi->mmap_addr + mbi->mmap_length)) {
        switch (mmap->type) {
            case MB_MEMORY_RESERVED:
                avail = "reserved";
            break;
            case MB_MEMORY_AVAILABLE:
                avail = "available";
            break;
        }
        length_addr = mmap->addr + mmap->length;
        size = mmap->length/1024;
        
        kprintf("\t\t- 0x%X%X - 0x%X%X, size: %dKB [%s]\n"
        , mmap->addr
        , length_addr
        , size
        , avail);
        mmap = (mb_mmap_t *)((uint32_t)mmap
            + mmap->size + sizeof(mmap->size));
    }
}

static void *expand_kheap(size_t size)
{
    void *alloc = NULL;
    if (kheap_end + size > kheap_max) {
       return NULL;
    }
    alloc = (void *)kheap_end;
    kheap_end += size;
    return alloc;
}

static void *contract_kheap(size_t size)
{
    if (kheap_end - size < kheap_start) {
        return NULL;
    }
    kheap_end -= size;
    return (void *)kheap_end;
}
