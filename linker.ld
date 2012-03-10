OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY (loader)

SECTIONS
{
    . = 0x00100000;
    .text ALIGN (0x1000) :
    {
    __start_cs_kernel = .;
        *(.text)
    __start_cs_init = .;
        *(.init.text)
    __end_cs_init = .;
    __end_cs_kernel = .;
    }

    .rodata ALIGN (0x1000) :
    {
    __start_data_kernel = .;
        *(.rodata*)
    }

    .data ALIGN (0x1000) :
    {
        *(.data)
    }

    .bss :
    {
        sbss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
    __end_data_kernel = .;
    }
}
