#include <kernel.h>
#include <memory.h>
#include <multiboot.h>
#include <queue.h>
#include <string.h>
#include <types.h>

struct kmalloc_area {
#define KMALLOC_AREA_MAGIC 0xDEAD
    uint16_t           ka_magic;
#define KMALLOC_AREA_FREE  0x0000
#define KMALLOC_AREA_USED  0x0001
#define KMEM_ALIGNED       0x0002
    uint16_t           ka_flags;
    size_t             ka_size;
    LIST_ENTRY(kmalloc_area)   ka_entries;
    STAILQ_ENTRY(kmalloc_area) ka_free_entries;
};
#define CHECK_KMALLOC_AREA_MAGIC(ptr)  do { \
if ((ptr)->ka_magic != KMALLOC_AREA_MAGIC) { \
   kpanic("Kernel heap corrupted at: %p\n", (void *)ptr); \
   hang(); } } while (0)

static size_t biggest_free_size = 0; 
struct kmalloc_area *last_mem_area = NULL;
static LIST_HEAD(, kmalloc_area) mem_areas = 
        LIST_HEAD_INITIALIZER(mem_areas);
static STAILQ_HEAD(, kmalloc_area) free_mem_areas = 
        STAILQ_HEAD_INITIALIZER(free_mem_areas);

void memory_init(struct multiboot_info *mbi)
{
    /* Make it shorter */
    typedef struct multiboot_mmap_entry mb_mmap_t;

    uint64_t length_addr;
    unsigned int size;
    const char *avail = NULL;
    mb_mmap_t *mmap = (mb_mmap_t *)mbi->mmap_addr;
    kheap_start = (uint32_t)&__end_data_kernel;
    kheap_end = kheap_start;

    if (!(mbi->flags & MB_INFO_MEM_MAP)) {
        kprintf("Fatal error! No memory mapping information found!\n");
    }

    while (mmap < (mb_mmap_t *)((uint32_t)mbi->mmap_addr + mbi->mmap_length)) {
        length_addr = mmap->addr + mmap->length;
        size = mmap->length/1024;
        if (size != 0 && mmap->addr != 0 && length_addr != 0)
            kprintf("\t\t- 0x%X%X - 0x%X%X, size: %dKB [%s]\n"
                , mmap->addr, length_addr, size
                , (mmap->type == MB_MEMORY_RESERVED) ? "reserved" : "available");

        if (mmap->addr == 0x100000 
                && mmap->type == MB_MEMORY_AVAILABLE) {
            pmm_init(mmap->addr, mmap->length);
            kheap_max = (uint32_t)VADDR((uint32_t)length_addr);
        }

        mmap = (mb_mmap_t *)((uint32_t)mmap
            + mmap->size + sizeof(mmap->size));
    }
}

void *expand_kheap(size_t size)
{
    void *alloc = NULL;
#if 0
    if (kheap_end + size > kheap_max) {
       return NULL;
    }
#endif
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
   struct kmalloc_area *marea = NULL;
   struct kmalloc_area *tmp, *mtmp;

   if (size == 0)
       return expand_kheap(0);
   
   if (flags & M_NORMAL) {
       if (!STAILQ_EMPTY(&free_mem_areas) && size <= biggest_free_size) {
           STAILQ_FOREACH_MUTABLE(tmp, &free_mem_areas, ka_free_entries, mtmp) {
               if (tmp->ka_size >= size) {
                   /* Yeah! We have a free()'d space which is enough for our new allocation.
                      Get, and remove it from the free queue. */
                   marea = tmp;
                   ptr += sizeof(struct kmalloc_area);
                   STAILQ_REMOVE(&free_mem_areas, tmp, kmalloc_area, ka_free_entries);
               }
           }
       } else {
           ptr = expand_kheap(sizeof(struct kmalloc_area) + size);
           if (ptr == NULL)
               return NULL;
           marea = (struct kmalloc_area *)ptr;
           ptr += sizeof(struct kmalloc_area);
       }
   } 

   marea->ka_size = size;
   marea->ka_flags = KMALLOC_AREA_USED;

   if (last_mem_area == NULL) {
       LIST_INSERT_HEAD(&mem_areas, marea, ka_entries);
   } else {
       CHECK_KMALLOC_AREA_MAGIC(last_mem_area);
       /* In some cases the allocator reuses some free()'d space. Chunks
        * which are already initialized and added to the list. Doesn't do it again!
       */
       if (marea->ka_magic != KMALLOC_AREA_MAGIC) {
           LIST_INSERT_AFTER(last_mem_area, marea, ka_entries);
       }
   }
   /* XXX: This should be the last. See above! */
   marea->ka_magic = KMALLOC_AREA_MAGIC;
   last_mem_area = marea;

   if (flags & M_ZEROED) {
       memclr(ptr, size);
   }
   return ptr;
}

void kfree(void *ptr)
{
    struct kmalloc_area *marea;

    if (ptr == NULL)
        return;

    marea = (struct kmalloc_area *)ptr;
    marea--;
    CHECK_KMALLOC_AREA_MAGIC(marea);
    if (marea->ka_size > biggest_free_size)
        biggest_free_size = marea->ka_size;

    marea->ka_flags ^= KMALLOC_AREA_USED;
    if (marea->ka_size < biggest_free_size)
        STAILQ_INSERT_TAIL(&free_mem_areas, marea, ka_free_entries);
    else
        STAILQ_INSERT_HEAD(&free_mem_areas, marea, ka_free_entries);
}

#ifdef CONFIG_DEBUG_KMALLOC
void dump_kmallocs(void)
{
    struct kmalloc_area *tmp;
    kprintf("-------- DUMPING ALLOCATIONS --------\n");
    if (LIST_EMPTY(&mem_areas)) {
        kprintf("No allocations, yet.\n");
    } else {
        LIST_FOREACH(tmp, &mem_areas, ka_entries) {
            dump_kmalloc_area(tmp);
        }
    }
    kprintf("Free areas:\n");
    if (STAILQ_EMPTY(&free_mem_areas)) {
        kprintf("No allocations, yet.\n");
    } else {
        STAILQ_FOREACH(tmp, &free_mem_areas, ka_free_entries) {
            dump_kmalloc_area(tmp);
        }
    }
    kprintf("---- END OF DUMPING ALLOCATIONS -----\n");
}

void dump_kmalloc_area(struct kmalloc_area *ma)
{
    kprintf("Allocation size: %d\n"
            "Allocation at: %p\n"
            "Descriptor at: %p\n"
            "Type: %s\n"
            "Magic %s\n\n"
            , ma->ka_size, (void *)ma + sizeof(struct kmalloc_area)
            , ma
            , (ma->ka_flags & KMALLOC_AREA_USED) ? "used" : "free"
            , (ma->ka_magic == KMALLOC_AREA_MAGIC) ? "correct" : "uncorrect");
}

void dump_kmalloc(void *ptr)
{
    struct kmalloc_area *ma = (struct kmalloc_area *)ptr;
    ma--;
    dump_kmalloc_area(ma);
}
#else 
void dump_kmallocs(void) {}
void dump_kmalloc_area(struct kmalloc_area *ma __unused) {}
void dump_kmalloc(void *p __unused) {}
#endif /* CONFIG_DEBUG_KMALLOC */
