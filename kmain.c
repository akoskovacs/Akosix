#include <kernel.h>
#include <console.h>
#include <system.h>
#include <string.h>
#include <multiboot.h>
#include <memory.h>

void kmain(struct multiboot_info *mbi, unsigned int magic)
{
   unsigned int mem = mbi->mem_upper + mbi->mem_lower;
   struct multiboot_info *nmbi;

   set_console_attributes(FG_COLOR_WHITE | LIGHT | BG_COLOR_BLUE);
   console_init();

   if (magic != 0x2BADB002) {
       kprint("Ooops, something went wrong! Terminated!n\n");
   }

   kprintf("Booting Akosix...\nAt kmain() %p\n", kmain);
   kprintf("Memory present %dKB (%dMB).\n", mem, mem/1024);
   kprintf("Bootloader: %s\n", (const char *)mbi->boot_loader_name);
   kprintf("Command line: %s\n", (const char *)mbi->cmdline);
   kprintf("End of kernel %p\n", (void*)&__end_data_kernel);
   kprintf("Paging enabled!\n\n");
   kprintf("Multiboot info:\n");

   if (mbi->flags & MB_INFO_BOOTDEV) 
       kprintf("\to Has multiboot bootdev\n");
   if (mbi->flags & MB_INFO_CMDLINE) 
       kprintf("\to Has multiboot command line\n");
   if (mbi->flags & MB_INFO_ELF_SECT) 
       kprintf("\to Has multiboot elf section info\n");
   if (mbi->flags & MB_INFO_DRIVE_INFO) 
       kprintf("\to Has multiboot drive info\n");
   if (mbi->flags & MB_INFO_VIDEO) 
       kprintf("\to Has multiboot video info\n");
   if (mbi->flags & MB_INFO_MODS) 
       kprintf("\to Has multiboot module info\n");
   if (mbi->flags & MB_INFO_MEMORY) 
       kprintf("\to Has multiboot memory\n");
   if (mbi->flags & MB_INFO_MEM_MAP)
       kprintf("\to Has multiboot memory-map info:\n");

   memory_init(mbi);
   nmbi = kmalloc(sizeof(struct multiboot_info), M_NORMAL | M_ZEROED);
   kprintf("kmalloc() test (must be 0) %d", nmbi->mem_upper);
   kfree(nmbi);
}
