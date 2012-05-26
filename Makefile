CC  = gcc
AS  = gcc
CAT = cat
LD  = ld
RM  = rm
SH  = bash
QEMU = qemu
PERL = perl

VERBOSE_BUILD = false

CFLAGS   := -std=c99 -Wall -O3 -march=i586 -nostdinc -fno-builtin -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -Wall -Wextra -Werror -ffreestanding -Wno-unused -m32 -fno-leading-underscore -ggdb
ASFLAGS := -Wall -Wextra -Werror -m32 -fno-leading-underscore -I . -ggdb
LDFLAGS  := -nostartfiles -nodefaultlibs -nostdlib -static -T linker.ld
INCLUDES := -I include -I lib/include -I . 
TMP_TARGET := .tmp_akosix.bin
TARGET := akosix.bin
MAPFILE := kernel.map
ISO := akosix.iso 
DIST := dist/
VERSION = 0.1-alpha
export KERNELVERSION := $(VERSION)

OBJECTS := kmain.o lib/string.o console.o lib/kprintf.o mm/memory.o boot/pgsetup.o boot/boot.o mm/pmm.o panic.o
KSYM_OBJ := ksymbol.o
KSYM_SRC := ksymbol.c
ifeq ($(VERBOSE_BUILD),true)
	COMP=$(CC) $(CFLAGS) $(INCLUDES) -c -o $*.o $*.c
else
	COMP=@$(CC) $(CFLAGS) $(INCLUDES) -c -o $*.o $*.c
endif

all: $(TARGET) 

kconfig: 
	@export CC=$(CC)
	@export CFLAGS=$(CC)
	@export LD=$(LD)
	@make -C build/kconfig/

menuconfig: .config
conf: .config
config: .config
.config: kconfig
	@build/kconfig/mconf Kconfig
	@$(PERL) scripts/genconf.pl
	@echo "Now, you can simply run 'make' to build Akosix"

%.o : %.c config.h
	@echo "CC $*.c"
	$(COMP)

%.o : %.S config.h
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
