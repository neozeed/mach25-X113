/* mb_start.S — Multiboot entry shim for Mach 2.6 */
#include <i386/asm.h>

    .text
    .globl _start
    .globl _pstart
    .globl _mb_magic
    .globl _mb_info

_start:
    /* Save Multiboot registers */
    movl %eax, _mb_magic
    movl %ebx, _mb_info

    /* Fake AT386 bootloader stack layout */
    pushl $0          /* esym */
    pushl $0          /* boothowto */
    pushl $640        /* cnvmem */
    pushl $32768      /* extmem */
    pushl $1          /* boottype */

    jmp _pstart

    .data
_mb_magic:
    .long 0

_mb_info:
    .long 0



#if 0
#define EXT(x) _##x

    .text
    .globl _start
    .globl EXT(pstart)
    .globl EXT(mb_magic)
    .globl EXT(mb_info)

_start:
    /* Save Multiboot registers */
    movl %eax, EXT(mb_magic)
    movl %ebx, EXT(mb_info)

    /* Fake the old AT386 bootloader stack layout */

    /* esym (ignored by Mach unless KDB) */
    pushl $0

    /* boothowto */
    pushl $0

    /* cnvmem */
    pushl $640        /* conventional memory in KB */

    /* extmem */
    pushl $32768      /* 32MB example; GRUB gives real value in mb_info */

    /* boottype */
    pushl $1          /* pretend AT386 */

    /* Jump into Mach's real entry */
    jmp EXT(pstart)
#endif