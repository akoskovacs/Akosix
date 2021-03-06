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
#include <kernel.h>
#include <string.h>

#define KPRINTF_BUFFER_SIZE 512
char buffer[KPRINTF_BUFFER_SIZE];

int vsnprintf(char *dest, size_t size, const char *fmt, va_list ap)
{
   char *tmp;
   size_t asize = 0;
   size_t nsize = 0;
   char buffer[30];
   unsigned int unum = 0;

   while (*fmt != '\0') {
       if (*fmt == '%') {
           fmt++;
           switch (*fmt) {
               case 's':
                   tmp = va_arg(ap, char *);
                   if (tmp == NULL)
                       tmp = "(null)";
                   nsize = strlen(tmp);
                   strncpy(dest+asize, tmp, size-asize);
                   asize += nsize;
               break;

               case 'u':
                   uitoa(va_arg(ap, unsigned int), 10, buffer);
                   nsize = strlen(buffer);
                   strncpy(dest+asize, buffer, size-asize);
                   asize += nsize;
               break;

               case 'd': case 'i':
                   itoa(va_arg(ap, int), 10, buffer);
                   nsize = strlen(buffer);
                   strncpy(dest+asize, buffer, size-asize);
                   asize += nsize;
               break;

               case 'x': case 'p': case 'X':
                    if (*fmt == 'x')
                        unum = va_arg(ap, unsigned int);
                    else 
                        unum = (unsigned int)va_arg(ap, void *);

                    if (*fmt != 'X') {
                        strncpy(dest+asize, "0x", size-asize);
                        asize += 2;
                    }
                    uitoa(unum, 16, buffer);
                    nsize = strlen(buffer);
                    strncpy(dest+asize, buffer, size-asize);
                    asize += nsize;
               break; 

               case 'c':
                    dest[asize++] = va_arg(ap, int);
               break;

               case '%':
                    strcpy(dest+asize++, "%");
               break;
           } // end of switch
           if (asize >= size)
               return asize;
       } else {
           if (asize + 1 < size)
               dest[asize++] = *fmt;
       }
       fmt++;
   } // end of while
   dest[asize] = '\0';
   return asize;
}

int snprintf(char *dest, size_t size, const char *fmt, ...)
{
    va_list ap;
    int ret;
    va_start(ap, fmt);
    ret = vsnprintf(dest, size, fmt, ap);
    va_end(ap);
    return ret;
}

/* Very, bad idea... */
#define KPRINT_TEMPLATE \
    int size;  \
    va_list ap; \
    va_start(ap, fmt); \
    size = vsnprintf(buffer, KPRINTF_BUFFER_SIZE, fmt, ap); \
    va_end(ap); 

int kprintf(const char *fmt, ...)
{
    KPRINT_TEMPLATE
    kprint(buffer);
    return size;
}

int kxya_printf(int x, int y, console_attr_t attr, const char *fmt, ...)
{
    KPRINT_TEMPLATE
    kxya_print(x, y, attr, buffer);
    return size;
}

int kxy_printf(int x, int y, const char *fmt, ...)
{
    KPRINT_TEMPLATE
    kxy_print(x, y, buffer);
    return size;
}
