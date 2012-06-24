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
 *************************************************************************/
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
