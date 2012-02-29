#include <basic.h>
#include <console.h>

void kmain(void* mbd __unused, unsigned int magic)
{
   if ( magic != 0x2BADB002 )
   {
       kprint("Ooops, something went wrong! Terminated!n\n");
   }
   init_console();
   kprint("Booting Akosix...\n\n");
}
