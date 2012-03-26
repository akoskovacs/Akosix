#include <kernel.h>
#include <console.h>
#include <system.h>
#include <string.h>
#include <multiboot.h>
#include <page.h>

extern char __end_data_kernel;

void kmain(struct multiboot_info *mbi __unused, unsigned int magic)
{
   unsigned int mem = mbi->mem_upper + mbi->mem_lower;
   set_console_attributes(FG_COLOR_WHITE | LIGHT | BG_COLOR_BLUE);
   init_console();
   if (magic != 0x2BADB002) {
       kprint("Ooops, something went wrong! Terminated!n\n");
   }
   kprintf("Booting Akosix...\nAt kmain() %p\n", kmain);
   kprintf("Memory present %dKB.\n", mem);
   kprintf("command line: %s\n", (const char *)mbi->cmdline);
   kprintf("End of kernel %p\n", (void*)&__end_data_kernel);
   kprintf("Paging enabled!\n");
   kprintf("Multiboot info:\n");
   if (mbi->flags & MB_INFO_BOOTDEV) 
       kprintf("\tHas multiboot bootdev\n");
   if (mbi->flags & MB_INFO_CMDLINE) 
       kprintf("\tHas multiboot command line\n");
   if (mbi->flags & MB_INFO_ELF_SECT) 
       kprintf("\tHas multiboot elf section info\n");
   if (mbi->flags & MB_INFO_DRIVE_INFO) 
       kprintf("\tHas multiboot drive info\n");
   if (mbi->flags & MB_INFO_VIDEO) 
       kprintf("\tHas multiboot video info\n");
   if (mbi->flags & MB_INFO_MODS) 
       kprintf("\tHas multiboot module info\n");
   if (mbi->flags & MB_INFO_MEM_MAP) {
       kprintf("\tHas multiboot memory-map info:\n");
       unsigned int i = 0;
       struct multiboot_mmap_entry *mmap;
       mmap = (struct multiboot_mmap_entry *)mbi->mmap_addr;
       while (i < mbi->mmap_length) {
           mmap = (struct multiboot_mmap_entry *)mbi->mmap_addr+i;
           kprintf("\t\tsize %d, addr %x, length %d [%savailable]\n"
           , mmap->size, mmap->addr
           , ((mmap->type == MB_MEMORY_AVAILABLE) ? "" : "not "));
           i += sizeof(struct multiboot_mmap_entry);
       }
   }
   if (mbi->flags & MB_INFO_MEMORY) 
       kprintf("\tHas multiboot memory\n\n");
}
