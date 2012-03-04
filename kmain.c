#include <kernel.h>
#include <console.h>

void kmain(void* mbd __unused, unsigned int magic)
{
   if ( magic != 0x2BADB002 )
   {
       kprint("Ooops, something went wrong! Terminated!n\n");
   }
   init_console();
   kprintf("Booting Akosix...\nAt kmain() %p\n", &kmain);
}
