#include <basic.h>
#include <types.h>
#include <page.h>
#include <multiboot.h>
#include <memory.h>
#include <string.h>
#include <system.h>

static pde_t *kpage_directory;
/* Virtual address to page directory index */
#define VADDR_TO_PDIND(addr) ((addr) >> 22)
#define VADDR_TO_PDE(pdir, vaddr) ((pdir)[VADDR_TO_PDIND(vaddr)])
void vm_switch_pdir(pde_t *);

/*
 * Again, do about the same thing as boot/pgsetup.c, but without the
 * identity mapping. After that we can use more generic functions.
 */
void vm_init(uint32_t start_address, uint32_t length)
{
    int i;
    kpage_directory = (pde_t *)PAGE_ALIGN(expand_kheap(PAGE_SIZE) + PAGE_SIZE);
    pte_t *kpage_table = (pte_t *)PAGE_ALIGN(expand_kheap(PAGE_SIZE) + PAGE_SIZE);
    memclr(kpage_directory, PAGE_SIZE);
    memclr(kpage_table, PAGE_SIZE);
    for (i = 0; i < PAGE_TABLE_SIZE; i++) {
        kpage_table[i] = (i * PAGE_SIZE) | PG_RW | PG_PRESENT;
    }
    VADDR_TO_PDE(kpage_directory, PAGE_SHIFT) = PADDR(kpage_table);
#if 0
    vm_switch_pdir(kpage_directory);
    pmm_init(start_address, length); 
#endif
    length = start_address; // FIXERROR
    length++;
}
#if 0
void vm_map_page(pde_t *dir, page_t *page, vaddr_t addr)
{

}

void vm_map_range(pde_t *pdir, vaddr_t vstart, size_t pg_count, paddr_t paddr)
{

}

pde_t *vm_create_pdir(paddr_t saddr, size_t len, vaddr_t vaddr, page_flags_t flags)
{
   //pde_t *dir = (pde_t *)get_page(); 
   return NULL;
}

void vm_switch_pdir(pde_t *pdir)
{
    paddr_t phys_pd = PADDR(pdir);
    write_cr3(phys_pd);
}

page_t get_free_page(page_flags_t flags)
{
   page_t page;
   pmm_alloc_frame(&page);
   SET_PAGE_FLAGS(page, flags);
   return page;
}

page_t *vm_addr_to_page(pde_t *pdir, void *ptr)
{
    vaddr_t addr = (vaddr_t)ptr;
    pde_t table = VADDR_TO_PDE(pdir, addr);
    if (table & PD_PRESENT) {
        pte_t *pt_addr = (pte_t *)VADDR(table >> 22);
        if (pt_addr != NULL) {
            return (page_t *)&pt_addr[addr & 0x3FF000];
        }
    }
    return NULL;
}

page_t *addr_to_page(void *ptr)
{
    return vm_addr_to_page(kpage_directory, ptr);
}
void vm_map_page(pde_t *dir, page_t *page, vaddr_t addr)
{

}

void vm_map_range(pde_t *pdir, vaddr_t vstart, size_t pg_count, paddr_t paddr)
{

}

pde_t *vm_create_pdir(paddr_t saddr, size_t len, vaddr_t vaddr, page_flags_t flags)
{
   //pde_t *dir = (pde_t *)get_page(); 
   return NULL;
}

void vm_switch_pdir(pde_t *pdir)
{
    paddr_t phys_pd = PADDR(pdir);
    write_cr3(phys_pd);
}

page_t get_free_page(page_flags_t flags)
{
   page_t page;
   pmm_alloc_frame(&page);
   SET_PAGE_FLAGS(page, flags);
   return page;
}

page_t *vm_addr_to_page(pde_t *pdir, void *ptr)
{
    vaddr_t addr = (vaddr_t)ptr;
    pde_t table = VADDR_TO_PDE(pdir, addr);
    if (table & PD_PRESENT) {
        pte_t *pt_addr = (pte_t *)VADDR(table >> 22);
        if (pt_addr != NULL) {
            return (page_t *)&pt_addr[addr & 0x3FF000];
        }
    }
    return NULL;
}

page_t *addr_to_page(void *ptr)
{
    return vm_addr_to_page(kpage_directory, ptr);
}
#endif
