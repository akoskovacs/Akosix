#include <kernel.h>
#include <console.h>
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

int kprintf(const char *fmt, ...)
{
    int size;
    va_list ap;

    va_start(ap, fmt);
    size = vsnprintf(buffer, KPRINTF_BUFFER_SIZE, fmt, ap);
    va_end(ap);

    kprint(buffer);

    return size;
}
