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
   kprintf("Panic! Kernel heap corrupted at: %p\n", (void *)ptr); \
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
   void *ptr;
   struct kmalloc_area *marea;
   struct kmalloc_area *tmp, *mtmp;

   if (size == 0)
       return expand_kheap(0);
   
   if (flags & M_NORMAL) {
       if ((!STAILQ_EMPTY(&free_mem_areas)) && (size <= biggest_free_size))
           STAILQ_FOREACH_MUTABLE(tmp, &free_mem_areas, ka_free_entries, mtmp) {
               if (tmp->ka_size >= size) {
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
   } else if (flags & M_ALIGNED) {
       void *kbrk = expand_kheap(0);
       void *aligned_ptr = PAGE_ALIGN(kbrk) + PAGE_SIZE;
       if (((unsigned int)(aligned_ptr - kbrk)) > (sizeof(struct kmalloc_area)*2)) {
           /* We need a free block to cover the gap between the last allocated
            * area and the new aligned area. It must have enough room to con-
            * tain it's own kmalloc_area and the new aligned kmalloc_area. */
           kfree(kmalloc(aligned_ptr-kbrk-(sizeof(struct kmalloc_area)*2), M_NORMAL));
       } else { /* Too small to create a free gap */
           /* TODO */
       }
       marea = (struct kmalloc_area *)
                expand_kheap(sizeof(struct kmalloc_area) + size);
       ptr = (void *)marea + sizeof(struct kmalloc_area);
   } else {
       return NULL;
   }

   marea->ka_size = size;
   marea->ka_magic = KMALLOC_AREA_MAGIC;
   marea->ka_flags = KMALLOC_AREA_USED;
   if (last_mem_area != NULL) {
       CHECK_KMALLOC_AREA_MAGIC(last_mem_area);
       LIST_INSERT_AFTER(last_mem_area, marea, ka_entries);
   } else {
           LIST_INSERT_HEAD(&mem_areas, marea, ka_entries);
   }
   last_mem_area = marea;

   if (flags & M_ZEROED) {
       memclr(ptr, sizeof(size));
   }
   return ptr;
}

void kfree(void *ptr)
{
    struct kmalloc_area *marea;

    if (ptr == NULL)
        return;

    marea = (struct kmalloc_area *)ptr - sizeof(struct kmalloc_area);
    CHECK_KMALLOC_AREA_MAGIC(marea);
    if (marea->ka_size > biggest_free_size)
        biggest_free_size = marea->ka_size;

    marea->ka_flags &= KMALLOC_AREA_USED;
    STAILQ_INSERT_TAIL(&free_mem_areas, marea, ka_free_entries);
}

#if defined DEBUG && defined DEBUG_MALLOC
void dump_kmallocs(void)
{
    struct kmalloc_area *tmp;
    kprintf("-------- DUMPING ALLOCATIONS --------\n");
    if (LIST_EMPTY(&mem_areas)) {
        kprintf("No allocations, yet.\n");
    } else {
        LIST_FOREACH(tmp, &mem_areas, ka_entries) {
            kprintf("Allocation size: %d\n"
                    "Allocation start: %p\n"
                    "Type: %s\n"
                    "Magic %s\n\n"
                    , tmp->ka_size, (void *)tmp + sizeof(struct kmalloc_area)
                    , (tmp->ka_flags & KMALLOC_AREA_USED) ? "used" : "free"
                    , (tmp->ka_magic == KMALLOC_AREA_MAGIC) ? "correct" : "uncorrect");
        }
    }
    kprintf("---- END OF DUMPING ALLOCATIONS -----\n");
}
#endif /* DEBUG && DEBUG_MALLOC */
