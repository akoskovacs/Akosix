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
#ifndef SYSTEM_H
#define SYSTEM_H

#include <types.h>
#include <basic.h>

static inline
void outb(uint16_t port, uint8_t value)
{
    __asm__ __volatile__("outb %0, %1"
            : 
            : "a"(value), "Nd"(port)
            ); 
}

static inline
void outw(uint16_t port, uint16_t value)
{
    __asm__ __volatile__("outw %0, %1"
            : 
            : "a"(value), "Nd"(port)
            ); 
}

static inline 
uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ __volatile__("inb %1, %0" 
    : "=a"(value) 
    : "Nd"(port)
    ); 
    return value;
}

static inline 
uint16_t inw(uint16_t port)
{
    uint16_t value;
    __asm__ __volatile__("inb %1, %0" 
    : "=a"(value) 
    : "Nd"(port)
    ); 
    return value;
}

static inline bool is_irq_on()
{
    int f;
    __asm__ __volatile__("pushf\n\tpopl %0"
    : "=g"(f));
    return f & (1<<9);
}

static inline
void rdtsc(uint32_t *upper, uint32_t *lower)
{
    __asm__ __volatile__("rdtsc" : "=a"(*lower), "=d"(*upper));
}

static inline
uint32_t read_cr0(void)
{
    uint32_t value;
    __asm__ __volatile__("movl %%cr0, %0" : "=r"(value));
    return value;
}

static inline
void write_cr0(uint32_t value)
{
    __asm__ __volatile("movl %0, %%cr0" : /* No output */
                                        : "r"(value));
}

static inline
uint32_t read_cr1(void)
{
    uint32_t value;
    __asm__ __volatile__("movl %%cr1, %0" : "=r"(value));
    return value;
}

static inline
uint32_t read_cr2(void)
{
    uint32_t value;
    __asm__ __volatile__("movl %%cr2, %0" : "=r"(value));
    return value;
}

static inline
uint32_t read_cr3(void)
{
    uint32_t value;
    __asm__ __volatile__("movl %%cr3, %0" : "=r"(value));
    return value;
}

static inline
void write_cr3(uint32_t value)
{
    __asm__ __volatile("movl %0, %%cr3" : /* No output */
                                        : "r"(value));
}

static inline
void halt(void)
{
    __asm__ __volatile__("hlt");
}

#endif // SYSTEM_H
