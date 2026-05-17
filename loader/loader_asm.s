    .section .multiboot
    .align 4
    .long 0x1BADB002              /* magic */
    .long 0x00000003              /* flags: align + mem info */
    .long -(0x1BADB002 + 0x00000003)  /* checksum */

    .section .text
    .globl loader_entry
loader_entry:
    / On entry:
    /  eax = multiboot magic
    /  ebx = multiboot info pointer
    /
    push %ebx
    push %eax
    call _loader_main
1:  hlt
    jmp 1b
