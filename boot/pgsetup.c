/************************************************************************
 *   Copyright (c) 2012 Ákos Kovács - Akosix operating system
 *              http://akoskovacs.github.com/Akosix
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ************************************************************************/

/*
 * This code setups the identity mapping and the higher half page tables and directories.
 * It simply returns the physical address of the page directory to the loader code.
*/ 

#include <types.h>
#include <page.h>

long page_directory[PAGE_TABLE_SIZE] __setup_data __align(PAGE_SIZE);
long page_table[PAGE_TABLE_SIZE] __setup_data __align(PAGE_SIZE);

long __setup setup_pd(void)
{
    register int i;
    for (i = 0; i < PAGE_TABLE_SIZE; i++) {
        page_directory[i] = 0;
        page_table[i] = 0;
        /* Map the first 4MB and set the appropriate flags */
        page_table[i] = (i * PAGE_SIZE) | PG_RW | PG_PRESENT;
    }
    
    page_directory[0] = (long)page_table | PD_RW | PD_PRESENT;
    page_directory[768] = page_directory[0];
    return (long)page_directory;
}
