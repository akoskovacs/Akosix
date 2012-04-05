#ifndef BASIC_H
#define BASIC_H

#define NULL ((void *)0)

#define PAGE_SIZE   4096
#define PAGE_SHIFT  12
#define PAGE_OFFSET 0xC0000000

// Virtual to physical 
#define VADDR(x) (vaddr_t)(x + PAGE_OFFSET)
// Physical to virtual
#define PADDR(x) (paddr_t)(x - PAGE_OFFSET)

#define PAGE_ALIGN(addr) ((addr) & (~0x0F))

/* Simple bit operations, works on the Nth bit of the value */
#define SET_BIT(V, N) ((V) |= (1 << (N)))
#define CLEAR_BIT(V, N) ((V) &= ~(1 << (N)))
#define IS_BIT_SET(V, N) TEST_BIT(V, N)
#define IS_BIT_NOT_SET(V, N) (!TEST_BIT(V, N))
#define TEST_BIT(V, N) ((V) & (1 << (N)))

/* General GCC attributes */
#define __inline __attribute__((__inline__))
#define __unused __attribute__((unused))
#define __used __attribute__((used))
#define __packed __attribute__((packed))
#define __align(A) __attribute__((aligned(A)))

/* Section macros for initcalls */
#define __init __section(".init.text")
#define __initdata __section(".init.data")
#define __exit __section(".exit.text")
#define __exitdata __section(".exit.data")
#define __section(name) __attribute((__section__(#name)))
typedef int (*initcall_t)(void);

/* Setup section */
#define __setup __section(".setup")
#define __setup_data __section(".setup.data")

/* Macros for va_list */
#define va_start(v,l)	__builtin_va_start(v,l)
#define va_end(v)	__builtin_va_end(v)
#define va_arg(v,l)	__builtin_va_arg(v,l)
#define va_copy(d,s)	__builtin_va_copy(d,s)
typedef __builtin_va_list va_list;

#endif // BASIC_H
