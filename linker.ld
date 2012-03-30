OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY (loader)
hhalf = 0xc0000000;
page_size = 4096;

SECTIONS
{
    . = 0x00100000;
    .setup :
    {
    __start_setup = .;
        *(.setup)
        *(.setup.data)
        boot/boot.o(.bss)
        boot/boot.o(COMMON)
    __end_setup = .;
    }

    /* Higher half */
    . += 0xc0000000;
    .text : AT(ADDR(.text) - hhalf) 
    {
    __start_cs_init = .;
        *(.init.text)
    __end_cs_init = .;
        *(.text)
    }

    .rodata ALIGN(page_size) : AT(ADDR(.rodata) - hhalf)
    {
    __start_data_kernel = .;
        *(.rodata*)
    }

    .data ALIGN(page_size) : AT(ADDR(.data) - hhalf)
    {
        *(.data)
    }

    .bss ALIGN(page_size) : AT(ADDR(.bss) - hhalf)
    {
        sbss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
    __end_data_kernel = .;
    }
}
