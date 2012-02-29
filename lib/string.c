#include <string.h>

char *strcpy(char *dest, const char *src)
{
    char *ret = dest;
    while ((*dest++ = *src++))
        ;
    return ret;
}

size_t strlen(const char *str)
{
    size_t str_size = 0;
    while ((*str++))
        str_size++;

    return str_size;
}
