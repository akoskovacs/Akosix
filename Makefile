# TODO: Urrggh, must be replaced in the near future
CC = gcc
CAT = cat
LD = ld
RM = rm
INCLUDES = -I include -I lib/include
OPTS = -Wall -Wextra -Werror -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -nostdinc
KERNEL = kmain.c lib/string.c console.c lib/kprintf.c

all: loader kernel link

loader:
	as boot/boot.S -o boot.o

kernel: 
	$(CC) -c $(KERNEL) $(OPTS) $(INCLUDES) -ggdb

link: loader kernel lib
	$(LD) -T linker.ld -o kernel.bin boot.o kmain.o string.o console.o kprintf.o

clean:
	$(RM) *.o kernel.bin floppy.img 
