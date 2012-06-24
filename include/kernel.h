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
 *************************************************************************/
#include <basic.h>
#include <system.h>
#include <types.h>

struct x86_registers {
    /* XXX: Don't change the order!
     'pushad' works in this way. */
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    /* Also pushed by save_registers() */
    uint16_t ss;
    uint16_t cs;
    uint16_t ds;
    uint16_t es;
    uint16_t fs;
    uint16_t gs;
    uint32_t cr0;
    uint32_t cr1;
    uint32_t cr2;
    uint32_t cr3;
} __packed;

static inline struct x86_registers save_regs(void) {
    struct x86_registers r;
    __asm__ __volatile__("movl %%eax, %0\n\t"
                         "movl %%ebx, %1\n\t"
                         "movl %%ecx, %2\n\t"
                         "movl %%edx, %3\n\t"
                         "movl %%esp, %4\n\t"
                         "movl %%edi, %5\n\t"
                         "movl %%esi, %6\n\t"
                         "movw %%ss, %7\n\t"
                         "movw %%cs, %8\n\t"
                         "movw %%ds, %9\n\t"
                         "movw %%fs, %10\n\t"
                         "movw %%gs, %11\n\t"
                         : "=m"(r.eax), "=m"(r.ebx), 
                          "=m"(r.ecx), "=m"(r.edx), 
                          "=m"(r.esp), "=m"(r.edi), 
                          "=m"(r.esi), "=m"(r.ss),
                          "=m"(r.cs), "=m"(r.ds),
                          "=m"(r.es), "=m"(r.fs), "=m"(r.gs));
#if 0
    r.cr0 = read_cr0();
    r.cr1 = read_cr1();
    r.cr2 = read_cr2();
    r.cr3 = read_cr3();
#endif
    return r;
}

int vsnprintf(char *, size_t, const char *, va_list);
int snprintf(char *, size_t, const char *, ...);
int kprintf(const char *fmt, ...);
void __panic(struct x86_registers, const char *, ...);
#define kpanic(fmt, ...) __panic(save_regs(), fmt, ## __VA_ARGS__)

#define hang() do { } while (true)
