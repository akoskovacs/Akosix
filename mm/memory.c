#include <types.h>
#include <multiboot.h>
#include <memory.h>
#include <kernel.h>
#include <string.h>

struct kmem_block {
#define KMEM_BLOCK_MAGIC 0xDEAD
    uint16_t           kb_magic;
#define KMEM_BLOCK_FREE  0x0000
#define KMEM_BLOCK_USED  0x0001
#define KMEM_ALIGNED     0x0002
    uint16_t           kb_flags;
    size_t             kb_size;
    struct kmem_block *kb_prev;
    struct kmem_block *kb_next;
};

struct kmem_block *last;
struct kmem_block *last_freed;

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

void *kmalloc(size_t size, malloc_flags_t flags)
{
   void *ptr = NULL;
   struct kmem_block *malloc = NULL;
   if (size == 0)
       return NULL;

   if (flags & M_NORMAL) {
       if (last_freed != NULL && last_freed->kb_size == size
                && last_freed->kb_flags & KMEM_BLOCK_FREE) {
           malloc = last_freed;
           ptr += sizeof(struct kmem_block);
           last_freed = NULL;
       } else {
           ptr = expand_kheap(sizeof(struct kmem_block) + size);
           if (ptr == NULL)
               return NULL;
           malloc = (struct kmem_block *)ptr;
           ptr += sizeof(struct kmem_block);
       }
   } else if (flags & M_ALIGNED) {
       /* Todo */
   }

   malloc->kb_size = size;
   malloc->kb_prev = last;
   malloc->kb_magic = KMEM_BLOCK_MAGIC;
   malloc->kb_flags = KMEM_BLOCK_USED;
   if (last != NULL)
       last->kb_next = malloc;

   if (last->kb_magic != KMEM_BLOCK_MAGIC) {
       kprintf("Panic! Kernel heap corrupted at: %p\n", last);
       return NULL;
   }

   if (flags & M_ZEROED) {
       memclr(ptr, sizeof(size));
   }
   return ptr;
}

void kfree(void *ptr)
{
    struct kmem_block *malloc;
    struct kmem_block *tmp;
    size_t size = 0;
    int blocks = 0;

    if (ptr == NULL)
        return;

    malloc = (struct kmem_block *)ptr - sizeof(struct kmem_block);
    if (malloc->kb_magic != KMEM_BLOCK_MAGIC) {
        kprintf("Panic! Kernel heap corrupted at: %p\n", last);
        return;
    }

    malloc->kb_flags = KMEM_BLOCK_FREE;
    tmp = malloc;
    while (tmp != NULL && tmp->kb_flags & KMEM_BLOCK_FREE) {
        size += tmp->kb_size;
        tmp = tmp->kb_prev;
        blocks++;
    }
    malloc = tmp;

    tmp = malloc->kb_next;
    while (tmp != NULL && tmp->kb_flags & KMEM_BLOCK_FREE) {
        size += tmp->kb_size;
        tmp = tmp->kb_next;
        blocks++;
    }
    size += (blocks - 1) * sizeof(struct kmem_block);
    malloc->kb_size = size;
    malloc->kb_flags |= KMEM_BLOCK_FREE;
    last_freed = malloc;
}
