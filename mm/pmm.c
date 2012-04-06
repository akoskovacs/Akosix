#include <basic.h>
#include <types.h>
#include <memory.h>
#include <string.h>

static uint32_t *frame_bitmap;
static size_t frame_size;
static size_t frame_count;
static size_t last_frame;
static size_t free_frames;

void pmm_init(uint32_t start_addr, uint32_t length)
{
    frame_count = (start_addr + length)/PAGE_SIZE;
    free_frames = frame_count;
    frame_size = frame_count / sizeof(uint32_t);
    frame_bitmap = (uint32_t *)expand_kheap(frame_size);
    memclr(frame_bitmap, frame_size);
    /*init_vmm(start_addr, length);*/
}

void pmm_set_frame(pfn_t pfn)
{
    uint32_t ind = pfn / 32;
    uint32_t off = pfn % 32;
    SET_BIT(frame_bitmap[ind], off);
}

void pmm_clear_frame(pfn_t pfn)
{
    uint32_t ind = pfn / 32;
    uint32_t off = pfn % 32;
    CLEAR_BIT(frame_bitmap[ind], off);
}

bool pmm_is_free_frame(uint32_t pfn)
{
    uint32_t ind = pfn / 32;
    uint32_t off = pfn % 32;
    return IS_BIT_NOT_SET(frame_bitmap[ind], off);
}

pfn_t pmm_first_frame(void)
{
    pfn_t i = 0;
    if (last_frame == 0 || last_frame == frame_count) {
        while (i < frame_count) {
            if (pmm_is_free_frame(i)) {
                pmm_set_frame(i);
                last_frame = i;
                free_frames--;
                return i;
            }
        }
    } else {
        last_frame++;
        free_frames--;
        return last_frame;
    }
    return -1;
}

pfn_t pmm_first_frames(int count)
{
    pfn_t pfn = 0;
    int c = count;
    pfn = pmm_first_frame();
    if (pfn == (pfn_t)-1) 
        return -1;

    count--;
    while (count--) {
        if (pmm_first_frame() == (pfn_t)-1) {
        }
    }
    return pfn;
}

void pmm_alloc_frame(page_t *p)
{
    pfn_t frame; 

    frame = pmm_first_frame();
    if (frame == (pfn_t) -1)
        return; /* Should panic */

    SET_PAGE_FRAME(*p, frame); 
}

void pmm_free_frame(page_t *p)
{
    pfn_t frame = ADDR_TO_PFN(PAGE_FRAME(*p));
    pmm_clear_frame(frame);
    SET_PAGE_FRAME(*p, 0);
}
