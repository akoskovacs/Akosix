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
#ifndef KSYMBOL_H
#define KSYMBOL_H

#include <basic.h>
#include <types.h>

typedef enum {
    SYM_BSS  = 0x001,
    SYM_DATA = 0x002,
    SYM_CODE = 0x004,
    SYM_ANY = SYM_BSS | SYM_CODE | SYM_DATA,
    SYM_VARIABLE = SYM_BSS | SYM_DATA,
    SYM_FUNCTION = SYM_CODE
} symbol_type_t;

struct ksymbol {
    uint32_t      ks_address;
    symbol_type_t ks_type;
    const char   *ks_name;
};

const char *get_ksymbol_name(unsigned long address) __weak;
void *get_ksymbol(const char *, symbol_type_t) __weak;
#define get_kfunction(name) get_ksymbol((name), SYM_CODE)
#define get_kvar(name) get_ksymbol((name), SYM_VARIABLE)
#define get_symbol(name) get_ksymbol((name), SYM_ANY)

#endif /* KSYMBOL_H */
