CC  = gcc
AS  = gcc
CAT = cat
LD  = ld
RM  = rm
SH  = bash
QEMU = qemu
PERL = perl

CFLAGS   := -std=c99 -Wall -O3 -march=i586 -nostdinc -fno-builtin -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -Wall -Wextra -Werror -ffreestanding -Wno-unused -ggdb -m32
ASFLAGS := -Wall -Wextra -Werror -ggdb -m32 -I .
LDFLAGS  := -nostartfiles -nodefaultlibs -nostdlib -static -ggdb -T linker.ld
INCLUDES := -I include -I lib/include -I . 
TMP_TARGET := .tmp_akosix.bin
TARGET := akosix.bin
MAPFILE := kernel.map
ISO := akosix.iso 
DIST := dist/
VERSION = "0.1-alpha"

OBJECTS := kmain.o lib/string.o console.o lib/kprintf.o mm/memory.o boot/pgsetup.o boot/boot.o mm/pmm.o
KSYM_OBJ := ksymbol.o
KSYM_SRC := ksymbol.c

all: $(TARGET) 

kconfig: 
	@export KERNELVERSION=$(VERSION)
	@make -C build/kconfig/

menuconfig: .config
conf: .config
config: .config
.config: kconfig
	@export KERNELVERSION=$(VERSION)
	@build/kconfig/mconf Kconfig
	@$(PERL) scripts/genconf.pl
	@echo "Now, you can simply run 'make' to build Akosix"

%.o : %.c config.h
	@echo "CC $*.c"
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $*.o $*.c

%.o : %.S
	@echo "AS $*.S"
	@$(AS) $(ASFLAGS) -c -o $*.o $*.S
	
$(TMP_TARGET): $(OBJECTS)
	@echo "LINK $@"
	@$(LD) $(LDFLAGS) -o $@ $^

iso: $(TARGET)
	@echo "Creating akosix.iso with GRUB..."
	@$(SH) scripts/make_iso.sh

qemu: $(TARGET)
	@qemu -kernel $(TARGET)

bochs: iso
	@$(SH) scripts/run_bochs.sh

$(TARGET): $(OBJECTS) $(TMP_TARGET) config.h
	@echo "GENKSYMS $(TMP_TARGET)"
	@$(PERL) scripts/genksyms.pl $(TMP_TARGET) $(KSYM_SRC)
	@echo "CC $(KSYM_SRC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $(KSYM_SRC)
	@echo "LINK $(TARGET)" 
	@$(LD) $(LDFLAGS) -o $(TARGET) -ggdb $(OBJECTS) $(KSYM_OBJ) -Map $(MAPFILE)

help:
	@echo "make        - Build and link the whole kernel"	
	@echo "make iso    - Build an iso image"	
	@echo "make qemu   - Test the kernel in qemu"	
	@echo "make bochs  - Test the kernel in bochs"	
	@echo "make config - Start the configuration utility"
	@echo "make clean  - Clean the working directory"

.PHONY: clean
clean:
	$(RM) -rf $(OBJECTS) $(TARGET) $(TMP_TARGET) $(MAPFILE) $(ISO) $(DIST)

.PHONY: kconfig_clean
kconfig_clean:
	make clean -C build/kconfig/
