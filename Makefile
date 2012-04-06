CC  = gcc
AS  = gcc
CAT = cat
LD  = ld
RM  = rm
SH  = bash
QEMU = qemu

CFLAGS   := -std=c99 -Wall -O3 -march=i586 -nostdinc -fno-builtin -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -Wall -Wextra -Werror -ffreestanding -Wno-unused -ggdb
ASFLAGS := -Wall -Wextra -Werror -ggdb
LDFLAGS  := -nostartfiles -nodefaultlibs -nostdlib -static -ggdb -T linker.ld
INCLUDES := -I include -I lib/include
TARGET := akx_kernel
MAPFILE := akx_kernel.map
ISO := akosix.iso 
DIST := dist/

OBJECTS := kmain.o lib/string.o console.o lib/kprintf.o memory.o boot/pgsetup.o boot/boot.o pmm.o
all: $(TARGET)

%.o : %.c
	@echo "CC $*.c"
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $*.o $*.c

%.o : %.S
	@echo "AS $*.S"
	@$(AS) $(ASFLAGS) -c -o $*.o $*.S
	
$(TARGET): $(OBJECTS)
	@echo "LINK $@"
	@$(LD) $(LDFLAGS) -o $@ -ggdb $^ -Map $(MAPFILE)

iso: $(TARGET)
	@echo "Creating akosix.iso with GRUB..."
	@$(SH) scripts/make_iso.sh

qemu: $(TARGET)
	@qemu -kernel $(TARGET)

bochs: iso
	@$(SH) scripts/run_bochs.sh

help:
	@echo "make       - Build and link the whole kernel"	
	@echo "make iso   - Build an iso image"	
	@echo "make qemu  - Test the kernel in qemu"	
	@echo "make bochs - Test the kernel in bochs"	
	@echo "make clean - Clean the working directory"

.PHONY: clean
clean:
	$(RM) -rf $(OBJECTS) $(TARGET) $(MAPFILE) $(ISO) $(DIST)
