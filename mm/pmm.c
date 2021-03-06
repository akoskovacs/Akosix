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
    frame_size = frame_count / sizeof(uint32_t);
    frame_bitmap = (uint32_t *)expand_kheap(frame_size);
    memclr(frame_bitmap, frame_size);
    uint32_t used_frames = PADDR(kheap_end)/PAGE_SIZE;
    /* Set the usage bit for the used pages */
    pmm_first_frames(used_frames);
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

bool pmm_is_free_frame(pfn_t pfn)
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

pfn_t pmm_first_frames(unsigned int count)
{
    pfn_t pfn = 0;
    if (free_frames < count)
        return (pfn_t)-1;

    pfn = pmm_first_frame();
    count--;

    while (count--) {
        pmm_first_frame();
    }
    return pfn;
}

void pmm_alloc_frame(page_t *p)
{
    pfn_t frame; 

    frame = pmm_first_frame();
    if (frame == (pfn_t) -1)
        return;

    SET_PAGE_FRAME(*p, frame); 
}

void pmm_free_frame(page_t *p)
{
    pfn_t frame = ADDR_TO_PFN(PAGE_FRAME(*p));
    pmm_clear_frame(frame);
    SET_PAGE_FRAME(*p, 0);
}
