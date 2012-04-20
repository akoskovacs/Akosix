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
void *get_ksymbol(const char *, symbol_type_t) __weak;
#define get_kfunction(name) get_ksymbol((name), SYM_CODE)
#define get_kvar(name) get_ksymbol((name), SYM_VARIABLE)
#define get_symbol(name) get_ksymbol((name), SYM_ANY)

#endif /* KSYMBOL_H */
