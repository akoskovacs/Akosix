#define __SETUP__
#include <page.h>

uint32_t page_directory[1024] __setup_data __align(PAGE_SIZE);
uint32_t page_table[1024] __setup_data __align(PAGE_SIZE);
void     setup_paging(void) __setup;

void setup_paging(void)
{
    int i;
    for (i = 0; i < 1024; i++) {
        page_directory[i] = 0;
        page_table[i] = 0;
        /* Map the first 4MB and set the appropriate flags */
        page_table[i] = (i * PAGE_SIZE) | PT_RW | PT_PRESENT;
    }
    
    page_directory[0] = (uint32_t)page_table | PD_RW | PD_PRESENT;
    page_directory[768] = page_directory[0];
    __asm__ __volatile__("movl %0, %%cr3\n"
                         "movl %%cr0, %%eax\n"
                         "orl $0x80000000, %%eax\n"
                         "movl %%eax, %%cr0\n"
                         : /* No output */
                         : "b"((uint32_t)page_directory)
                         : "eax");
}
