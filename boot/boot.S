.global loader
.extern higher_stack

.section .setup
.set ALIGN,    0x001
.set MEMINFO,  0x002
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)
.set STACKSIZE, 0x4000                  # that is, 16k.

.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.extern setup_pd

loader:
    mov   $(lower_stack + STACKSIZE/16), %esp
    pushl %eax
    pushl %ebx
    call  enable_paging
    popl  %ebx
    popl  %eax
    movl  $(higher_stack + STACKSIZE), %esp
    pushl %eax
    pushl %ebx
    call  kmain
    jmp   hang

enable_paging:
    call  setup_pd
    movl  %eax, %cr3
    movl  %cr0, %eax
    orl   $0x80000000, %eax
    movl  %eax, %cr0
ret

hang:
    hlt
    jmp   hang

# reserve 1KB stack on a quadword boundary, for lower-half
.comm lower_stack, STACKSIZE/16, 32
