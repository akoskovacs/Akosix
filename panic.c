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
#include <panic.h>
#include <console.h>
#include <string.h>
#include <config.h>

#define BSIZE    2 * CONFIG_CONSOLE_WIDTH

void __panic(struct x86_registers regs, const char *fmt, ...)
{
    char buffer[BSIZE];
    const char *panic_str = CONFIG_PANIC_STRING; 
    size_t panic_str_len = strlen(panic_str);
    size_t line_len = 0;
    va_list ap;
    int x, y, i;
    union x86_regs_u r;
    r.s_reg = regs;
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
    for (i = 0; i <= X86_REG_EDI; i++) {
        kprintf("\t%s: %d [%x]\n", x86_register_name[i], r.a_reg[i], r.a_reg[i]);
    }
    kprintf("\n");
    /* Segment registers are 16 bit long. Split the 32 bit array 
      elements to high and low variables to overcome this. */
    for (i = X86_REG_SS; i <= X86_REG_GS; i++) {
        uint16_t high, low;
        high = r.a_reg[i] >> 16;
        low = r.a_reg[i] & 0xFFFF;
        kprintf("\t%s: %d [%x]\n", x86_register_name[i], high, high);
        kprintf("\t%s: %d [%x]\n", x86_register_name[++i], low, low);
    }
    hang();
}
