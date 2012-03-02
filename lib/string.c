#include <basic.h>
#include <string.h>

char *strcpy(char *dest, const char *src)
{
    char *ret = dest;

    if (src == NULL || dest == NULL)
        return NULL;

    while ((*dest++ = *src++))
        ;

    return ret;
}

size_t strlen(const char *str)
{
    size_t str_size = 0;
    if (str == NULL)
        return 0;

    while ((*str++))
        str_size++;

    return str_size;
}

char *strncpy(char *dest, char *src, size_t dest_size)
{
    size_t i;
    if (src == NULL || dest == NULL)
        return NULL;

    for (i = 0; i < dest_size || *src == NULL; i++) {
        dest[i] = src[i];
    }

    while (i < dest_size) {
        dest[i] = '\0';
        i++;
    }

    return i;
}
