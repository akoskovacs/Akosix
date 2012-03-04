#include <kernel.h>
#include <console.h>

void kmain(void* mbd __unused, unsigned int magic)
{
   if ( magic != 0x2BADB002 )
   {
       kprint("Ooops, something went wrong! Terminated!n\n");
   }
   init_console();
///   kprintf("Booting Akosix...\n");
   kprintf("This is a test of %s with the answer of %d with a hex is %x\n", "Akosix", 42, 0xFFAACC00);
}
