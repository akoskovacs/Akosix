#include <kernel.h>
#include <console.h>
#include <string.h>

#define KPRINTF_BUFFER_SIZE 512

int vsnprintf(char *dest, size_t size, const char *fmt, va_list ap)
{
   char *tmp;
   size_t asize = 0;
   size_t nsize = 0;
   bool alter_fmt = false;
   char buffer[20];
   va_list oarg;
   va_copy(oarg, ap);

   while (*fmt != '\0') {
       if (*fmt == '%') {
           fmt++;
           switch (*fmt) {
               case 'd':
                   tmp = itoa(va_arg(ap, int), buffer, 10);
                   nsize += strlen(tmp);
                   strncpy(dest+asize, tmp, size-asize);
                   asize += nsize;
               break;

               case 's':
                   tmp = va_arg(ap, char *);
                   nsize = strlen(tmp);
                   strncpy(dest+asize, tmp, size-asize);
                   asize += nsize;
               break;
               
               case '#':
                    alter_fmt = true;
               /* FALLTHROUGH */
               case 'x': case 'p':
                    if (alter_fmt) {
                        strcpy(dest+asize, "0x");
                        asize += 2;
                        alter_fmt = true;
                    }

                    if (*fmt == 'x')
                        tmp = itoa(va_arg(ap, int), buffer, 10);
                    else
                        tmp = itoa(*(int *)va_arg(ap, void *), buffer, 10);

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
   /*va_end(ap);*/
   return asize;
}

int kprintf(const char *fmt, ...)
{
    char buffer[KPRINTF_BUFFER_SIZE];
    int size;
    va_list ap;
    /*__asm__("movl 0x2(%%ebp), %%edx" : : : "edx");*/
    va_start(ap, fmt);
    size = vsnprintf(buffer, KPRINTF_BUFFER_SIZE, fmt, ap);
    kprint(buffer);
    va_end(ap);
    /*__asm__("pushl %%edx" : : :"edx");*/
    return size;
}
