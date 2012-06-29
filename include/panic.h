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
#ifndef PANIC_H
#define PANIC_H
#include <types.h>

/* 
 * WARNING!: Ugly, really architecture-dependent code comes! 
 *                      You were warned!
*/

#define EOF_STACK_MAGIC 0xE0F514C7
#define SOF_STACK_MAGIC 0x514514C7

struct __packed x86_registers {
#define X86_REG_EAX      0
    uint32_t eax;
#define X86_REG_EBX      1
    uint32_t ebx;
#define X86_REG_ECX      2
    uint32_t ecx;
#define X86_REG_EDX      3
    uint32_t edx;
#define X86_REG_ESP      4
    uint32_t esp;
#define X86_REG_EBP      5
    uint32_t ebp;
#define X86_REG_ESI      6
    uint32_t esi;
#define X86_REG_EDI      7
    uint32_t edi;
/* Also pushed by save_registers() */
/* NOTICE: The segment regs are 16bit long! */
#define X86_REG_SS       8
    uint16_t ss;
#define X86_REG_CS       9
    uint16_t cs;
#define X86_REG_DS       10
    uint16_t ds;
#define X86_REG_ES       11
    uint16_t es;
#define X86_REG_FS       12
    uint16_t fs;
#define X86_REG_GS       13
    uint16_t gs;
};

union __packed x86_regs_u {
    struct x86_registers s_reg; 
    uint32_t a_reg[8+3];
};

/* Just to get the name, like for eax:
  `x86_register_name[X86_REG_EAX] == "EAX"` */
static const char *x86_register_name[] = {
    "EAX", "EBX", "ECX", "EDX"
  , "ESP", "EBP", "ESI", "EDI"
  , "SS", "CS", "DS", "ES", "FS", "GS"
  , NULL 
};

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

void __panic(struct x86_registers, const char *,  const char *, int,  const char *, ...);
#define kpanic(fmt, ...) __panic(save_regs(), __FUNCTION__, __FILE__, __LINE__,  fmt, ## __VA_ARGS__)
//void __backtrace(unsigned, unsigned, unsigned);

#endif // PANIC_H
