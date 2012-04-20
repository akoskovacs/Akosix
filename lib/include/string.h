#ifndef STRING_H
#define STRING_H

#include <types.h>

char *strcat(char *, const char *);
char *strcpy(char *, const char *);
char *strncat(char *, const char *, size_t);
char *strncpy(char *, const char *, size_t);
char *strstr(const char *, const char *);
char hexdigit(int);
int memcmp(const void *, const void *, size_t);
int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);
int strindex(const char *, char);
size_t strlen(const char *);
void *memclr(void *, size_t);
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
void itoa(int, int, char *);
void strrev(char *, char *);
void uitoa(unsigned int, unsigned int, char *);

#endif // STRING_H
