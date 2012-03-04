#ifndef STRING_H
#define STRING_H

#include <types.h>

char *strcpy(char *, const char *);
size_t strlen(const char *);
char *strncpy(char *, const char *, size_t);
void strrev(char *, char *);
char *itoa(int, char *, int);

#endif // STRING_H
