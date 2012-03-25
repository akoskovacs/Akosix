#include <types.h>
#include <multiboot.h>
#include <memory.h>

uint32_t placement_address = 0;
extern char __end_data_kernel[];
#if 0
void init_memory(struct multiboot_info *mbi)
{
   unsigned int i = 0;
   struct multiboot_mmap_entry *mmap;
   mmap = (struct multiboot_mmap_entry *)mbi->mmap_addr;
   while (i < mbi->mmap_length) {
       mmap = (struct multiboot_mmap_entry *)mbi->mmap_addr;
       kprintf("\nsize %d, addr %x, length %d [%savailable]\n"
       , mmap->size, mmap->addr
       , (mmap->type == MB_MEMORY_AVAILABLE) ? "" : "not "); 
       i += sizeof(struct multiboot_mmap_entry);
   }
}
#endif


