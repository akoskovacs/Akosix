#include <basic.h>
#include <string.h>

/* Some of these functions may borrowed from Linux */

/* Not really a string function :-D */
#define abs(n) (((n) < 0) ? -(n) : (n))

char *strcpy(char *dest, const char *src)
{
    char *ret = dest;
    while ((*dest++ = *src++) != '\0')
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

char *strncpy(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	while (count) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}
	return dest;
}

int strcmp(const char *f, const char *s)
{
    while (*f || *s) {
        if (*f == *s)
            continue;
        else if(*f < *s)
            return -1;
        else 
            return 1;

        f++;
        s++;
    }
    return 0;
}

char *strcat(char *dest, const char *src)
{
    char *tmp = dest;

    while (*dest)
        dest++;
    while ((*dest++ = *src++) != '\0')
        ;
    return tmp;
}

char *strncat(char *dest, const char *src, size_t count)
{
    char *tmp = dest;

    if (count) {
        while (*dest)
            dest++;
        while ((*dest++ = *src++) != 0) {
            if (--count == 0) {
                *dest = '\0';
                break;
            }
        }
    }
    return tmp;
}

int strindex(const char *str, char ch)
{
    int idx;
    for (idx = 0; *str != '\0'; idx++) {
        if (str[idx] == ch)
            return idx;
    }

    return -1;
}

int memcmp(const void *cs, const void *ct, size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

char *strstr(const char *s1, const char *s2)
{
    size_t l1, l2;

    l2 = strlen(s2);
    if (!l2)
        return (char *)s1;
    l1 = strlen(s1);
    while (l1 >= l2) {
        l1--;
        if (!memcmp(s1, s2, l2))
            return (char *)s1;
        s1++;
    }
    return NULL;
}

void strrev(char *begin, char *end) {
    
    char aux;
    if (begin == NULL || end == NULL)
        return;
    
    while(end > begin)
        aux = *end, *end-- = *begin, *begin++ = aux;
}

void *memset(void *data, int c, size_t count)
{
    char *d = data;

    while (count--)
        *d++ = c;

    return data;
}

void *memclr(void *data, size_t size)
{
    return memset(data, 0, size);
}

void *memcpy(void *dest, const void *src, size_t count)
{
    char *d = dest;
    const char *s = src;

    while (count--)
        *d++ = *s++;

    return dest;
}

void *memmove(void *dest, const void *src, size_t count)
{
    char *tmp;
    const char *s;

    if (dest <= src) {
        tmp = dest;
        s = src;
        while (count--)
            *tmp++ = *s++;
    } else {
        tmp = dest;
        tmp += count;
        s = src;
        s += count;
        while (count--)
            *--tmp = *--s;
    }
    return dest;
}

char hexdigit(int n)
{
	char x;
	switch(n)
	{
		case 10:
			x = 'A';
			break;
		case 11:
			x = 'B';
			break;
		case 12:
			x = 'C';
			break;
		case 13:
			x = 'D';
			break;
		case 14:
			x = 'E';
			break;
		case 15:
			x = 'F';
			break;
		default:
			x = n + 0x30;
			break;
	}
	return x;
}

void itoa(int num, int base, char *buffer)
{
	unsigned int r = 0;
	int i = 0;
	char tmp[32];
	if(num < 0)
		*buffer++ = '-';
	num = abs(num);

	if(num < base){
		*buffer++ = hexdigit(num);
		*buffer = '\0';
		return;
	}

	if(num >= base)
		while(num)
		{
			r = num % base;
			num /= base;
			tmp[i++] = hexdigit(r);	
		}
	i--;
	for( ; i>=0 ; i--)
		*buffer++ = tmp[i];
	*buffer = '\0';
}

void uitoa(unsigned int num, unsigned int base, char *buffer)
{
	unsigned int r=0;
	int i=0;
	char tmp[32];

	if(num < base){
		*buffer++ = hexdigit(num);
		*buffer = '\0';
		return;
	}

	if(num >= base)
		while(num)
		{
			r = num % base;
			num /= base;
			tmp[i++] = hexdigit(r);	
		}
	i--;
	for( ; i>=0 ; i--)
		*buffer++ = tmp[i];
	*buffer = '\0';
}
