.global loader

.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.set STACKSIZE, 0x4000                  # that is, 16k.
.comm stack, STACKSIZE, 32              # reserve 16k stack on a quadword boundary

loader:
    mov   $(stack + STACKSIZE), %esp
    push  %eax                          # Memory map
    push  %ebx

    call kmain

hang:
    hlt
    jmp   hang
