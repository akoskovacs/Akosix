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
   char buffer[20];
   int num;

   while (*fmt != '\0') {
       if (*fmt == '%') {
           fmt++;
           switch (*fmt) {
               case 's':
                   tmp = va_arg(ap, char *);
                   nsize = strlen(tmp);
                   strncpy(dest+asize, tmp, size-asize);
                   asize += nsize;
               break;
               
               case 'd': case 'i':
                   tmp = itoa(va_arg(ap, int), buffer, 10);
                   nsize = strlen(tmp);
                   strncpy(dest+asize, tmp, size-asize);
                   asize += nsize;
               break;

               case 'x': case 'p':
                    if (*fmt == 'x')
                        num = va_arg(ap, int);
                    else 
                        num = (unsigned int)va_arg(ap, void *);

                    tmp = itoa(num, buffer, 16);
                    nsize = strlen(tmp);
                    strncpy(dest+asize, tmp, size-asize);
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
