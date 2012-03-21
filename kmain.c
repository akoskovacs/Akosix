#include <kernel.h>
#include <console.h>
#include <system.h>
#include <string.h>
#include <multiboot.h>
#include <page.h>

extern char __end_data_kernel;

void kmain(struct multiboot_info *mbi, unsigned int magic)
{
   unsigned int mem = mbi->mem_upper + mbi->mem_lower;
   if (magic != 0x2BADB002) {
       /*set_forecolor(COLOR_RED);*/
       kprint("Ooops, something went wrong! Terminated!n\n");
   }

   set_backcolor(COLOR_CYAN);
   init_console();
   kprintf("Booting Akosix...\nAt kmain() %p\n", &kmain);
   kprintf("Memory present %dKB.\n", mem);
   kprintf("Paging enabled!\n");
   kprintf("command line: %s\n", (const char *)mbi->cmdline);
   kprintf("End of kernel %p\n", (void*)&__end_data_kernel);
}
