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

#if 0
static inline
void lidt(void *base, size_t size)
{
    size_t i[2];
    i[0] = size << 16; 
    i[1] = (unsigned int)base;
    __asm__("lidt (%0)" : : "p"(((char *) i)+2));
}
#endif

static inline
void lidt(void *base, size_t size)
{
    struct {
        uint16_t length;
        uint64_t base;
    } __packed IDTR;
    IDTR.length = size;
    IDTR.base = (uint64_t)base;
    __asm__("lidt (%0)" 
    :
    : "p"(&IDTR));
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

#endif // SYSTEM_H
