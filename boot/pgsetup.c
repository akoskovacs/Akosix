/* 
 * This code setup the identity mapping and the higher half page tables and directories.
 * It simply returns the physical address of the page directory to the loader code.
 * Which is then enable the paging.
*/
#include <types.h>
#include <page.h>

long page_directory[1024] __setup_data __align(PAGE_SIZE);
long page_table[1024] __setup_data __align(PAGE_SIZE);

long __setup setup_pd(void)
{
    register int i;
    for (i = 0; i < 1024; i++) {
        page_directory[i] = 0;
        page_table[i] = 0;
        /* Map the first 4MB and set the appropriate flags */
        page_table[i] = (i * PAGE_SIZE) | PT_RW | PT_PRESENT;
    }
    
    page_directory[0] = (long)page_table | PD_RW | PD_PRESENT;
    page_directory[768] = page_directory[0];
    return (long)page_directory;
}
