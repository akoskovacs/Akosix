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

char *strncpy(char *dest, const char *src, size_t dest_size)
{
    size_t i;
    if (src == NULL || dest == NULL)
        return NULL;

    for (i = 0; i < dest_size || src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    while (i < dest_size) {
        dest[i] = '\0';
        i++;
    }

    return dest;
}

void strrev(char *begin, char *end) {
	
	char aux;
    if (begin == NULL || end == NULL)
        return;
	
	while(end > begin)
		aux = *end, *end-- = *begin, *begin++ = aux;
}

char* itoa(int value, char *result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36) {
        *result = '\0';
        return result;
    }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0)
        *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
