#include <types.h>
#include <basic.h>
#include <kernel.h>
#include <console.h>
#include <string.h>
#include <config.h>

#define BSIZE    2*CONFIG_CONSOLE_WIDTH

void __panic(struct x86_registers regs, const char *fmt, ...)
{
    char buffer[BSIZE];
    const char *panic_str = CONFIG_PANIC_STRING; 
    size_t panic_str_len = strlen(panic_str);
    size_t line_len = 0;
    va_list ap;
    int x, y;
    console_attr_t basic_attrs = BG_COLOR_RED | FG_COLOR_WHITE;
    set_console_attributes(basic_attrs);
    clear_console(); 
    va_start(ap, fmt);
    line_len = vsnprintf(buffer, BSIZE, fmt, ap);
    va_end(ap);
    set_console_attributes(BG_COLOR_BLUE | LIGHT | FG_COLOR_WHITE | BLINK);
    kpos_print((CONFIG_CONSOLE_WIDTH-panic_str_len)/2, 1, panic_str); 
    set_console_attributes(basic_attrs);
    if (line_len < CONFIG_CONSOLE_WIDTH) {
        kpos_print((CONFIG_CONSOLE_WIDTH-line_len)/2, 3, buffer);
    }
    set_xy(0, 5);
    kprintf("\tEAX: %d [%x]\n", regs.eax, regs.eax);
    kprintf("\tEBX: %d [%x]\n", regs.ebx, regs.ebx);
    kprintf("\tECX: %d [%x]\n", regs.ecx, regs.ecx);
    kprintf("\tEDX: %d [%x]\n", regs.edx, regs.edx);
    kprintf("\tESP: %d [%x]\n", regs.esp, regs.esp);
    kprintf("\tEBP: %d [%x]\n", regs.ebp, regs.ebp);
    kprintf("\tEDI: %d [%x]\n", regs.edi, regs.edi);
    kprintf("\tESI: %d [%x]\n\n", regs.esi, regs.esi);
    kprintf("\tCS: %d [%x]\n", regs.cs, regs.cs);
    kprintf("\tDS: %d [%x]\n", regs.ds, regs.ds);
    kprintf("\tSS: %d [%x]\n", regs.ss, regs.ss);
    kprintf("\tES: %d [%x]\n", regs.es, regs.es);
    kprintf("\tFS: %d [%x]\n", regs.fs, regs.fs);
    kprintf("\tGS: %d [%x]\n\n", regs.gs, regs.gs);
#if 0
    kprintf("\tCR0: %d [%x]\n", regs.cr0, regs.cr0);
    kprintf("\tCR1: %d [%x]\n", regs.cr1, regs.cr1);
    kprintf("\tCR2: %d [%x]\n", regs.cr2, regs.cr2);
    kprintf("\tCR4: %d [%x]\n", regs.cr3, regs.cr3);
#endif
    hang();
}
