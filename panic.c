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
#include <types.h>
#include <basic.h>
#include <kernel.h>
#include <ksymbol.h>
#include <panic.h>
#include <console.h>
#include <string.h>
#include <config.h>

#define BSIZE      2 * CONFIG_CONSOLE_WIDTH
#define LINE_COUNT 11
#define LINE_LEN   50

/* Use this function with care! */
static void __backtrace(unsigned x, unsigned y, unsigned int max_frames)
{
    unsigned int *ebp = &x - 2;
    unsigned int frame, eip;
    unsigned int *args;
    const char *sym_name;
    for (frame = 0; frame < max_frames; frame++) {
        eip = ebp[1];
        if (eip == 0)
            break;

        ebp = (unsigned int *)ebp[0];
        args = &ebp[2];
        if (args[2] == SOF_STACK_MAGIC)
            break;

        sym_name = get_ksymbol_name(eip);
        kxy_printf(x, y+frame, "%x: %s()", eip, sym_name);
    }
}

#define CENTERED(size) ((CONFIG_CONSOLE_WIDTH-(size))/2)

void __panic(struct x86_registers regs, const char *func, const char *file, int line,  const char *fmt, ...)
{
    char buffer[BSIZE];
    const char *panic_str = CONFIG_PANIC_STRING; 
    size_t panic_str_len = strlen(panic_str);
    size_t buf_len = 0;
    va_list ap;
    int x, y, i;
    union x86_regs_u r;
    r.s_reg = regs;
    va_start(ap, fmt);
    /* Move the formatted message to the buffer[] */
    buf_len = vsnprintf(buffer, BSIZE, fmt, ap);
    va_end(ap);
    
    /* Set the console color to white on red (Red Screen of Death :) */
    set_console_attributes(BG_COLOR_RED | FG_COLOR_WHITE);
    clear_console(); 
    /* Write the 'AKOSIX KERNEL PANIC' or other 
                                 user-defined panic string centered */
    kxya_print(CENTERED(panic_str_len), 1, BG_COLOR_BLACK 
                    | LIGHT | FG_COLOR_WHITE | BLINK, panic_str); 

    /* The message with more ligher white, also centered */
    kxya_print(CENTERED(buf_len), 3, BG_COLOR_RED|LIGHT|FG_COLOR_WHITE, buffer);

    set_xy(0, 5);
    /* Register dump */
    for (i = 0; i <= X86_REG_EDI; i++) {
        kprintf("\t%s: %d [%x]\n", x86_register_name[i], r.a_reg[i], r.a_reg[i]);
    }
    kprintf("\n");
    /* Segment registers are 16 bit long. Split the 32 bit array 
      elements to high and low variables to overcome this. */
    for (i = X86_REG_SS; i <= X86_REG_GS; i++) {
        uint16_t high, low;
        high = r.a_reg[i] & 0x0000FFFF;
        low = r.a_reg[i]  & 0xFFFF0000;
        kprintf("\t%s: %d [%x]\n", x86_register_name[i], high, high);
        kprintf("\t%s: %d [%x]\n", x86_register_name[++i], low, low);
    }
    // __backtrace(CONFIG_CONSOLE_WIDTH-LINE_LEN+5, 5, 5);
    /* Location of the panic file:function():line */
    kxy_printf(0, CONSOLE_LAST_ROW, "%s:%s():%d", file, func, line);
    hang();
}
