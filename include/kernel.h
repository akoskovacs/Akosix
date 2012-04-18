#include <basic.h>
#include <types.h>

int vsnprintf(char *, size_t, const char *, va_list);
int snprintf(char *, size_t, const char *, ...);
int kprintf(const char *fmt, ...);

void hang();
