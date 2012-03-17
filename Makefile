CC  = gcc
AS  = gcc
CAT = cat
LD  = ld
RM  = rm

CFLAGS   := -std=c99 -Wall -O3 -march=i586 -nostdinc -fno-builtin -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -Wall -Wextra -Werror -ggdb
ASFLAGS := -Wall -Wextra -Werror -ggdb
LDFLAGS  := -nostartfiles -nodefaultlibs -nostdlib -static -ggdb -T linker.ld
INCLUDES := -I include -I lib/include
TARGET := akx_kernel
MAPFILE := akx_kernel.map

OBJECTS := kmain.o lib/string.o console.o lib/kprintf.o memory.o boot/pgsetup.o boot/boot.o
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

.PHONY: clean
clean:
	$(RM) -rf $(OBJECTS) $(TARGET) $(MAPFILE)
