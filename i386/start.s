/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.
 *
 * start.s -- ELF higher-half port of Mach 2.6 i386 startup
 *
 * Physical load address:  0x00100000  (1MB, GRUB multiboot)
 * Virtual link address:   0xC0100000  (3GB higher-half)
 *
 * Boot sequence:
 *   1. GRUB jumps to _pstart (physical, paging OFF, flat GRUB segments)
 *   2. Fill kpte: 3072 PTEs mapping physical 0x00000000-0x00BFFFFF (12MB)
 *   3. Set up kpde: PDE[0,1,2] identity + PDE[768,769,770] higher-half
 *   4. Munge GDT/IDT/gates from Mach init format to hardware format
 *   5. Load GDTR, IDTR with physical addresses
 *   6. Load CR3 (physical kpde), enable paging via CR0
 *   7. Far jump to high_start (flushes pipeline, now at virtual 0xC01xxxxx)
 *   8. Reload segment registers from our GDT
 *   9. Set up kernel stack
 *  10. Hardware task switch via KTSSSEL into kernel TSS -> i386_init
 *
 * Address translation before paging is on:
 *   Virtual 0xCxxxxxxx -> physical: addr & 0x0FFFFFFF  (strips top nibble)
 *   This works because KV = 0xC0000000 and MASK = 0x0FFFFFFF.
 */

#include <i386/asm.h>
#include <mach/vm_param.h>

/* -----------------------------------------------------------------------
 * Constants
 * --------------------------------------------------------------------- */
        .set    GDTLIM,  (8*96-1)        /* GDT limit: 96 descriptors      */
        .set    IDTLIM,  (8*256-1)       /* IDT limit: 256 descriptors      */
        .set    PAGEBIT, 0x80000000      /* CR0.PG bit                      */
        .set    MASK,    0x0FFFFFFF      /* virt->phys: strip top nibble    */
        .set    KV,      0xC0000000      /* kernel virtual base             */
        .set    KCODE,   0x158            /* kernel code segment selector    */
        .set    KDATA,   0x160            /* kernel data segment selector    */

/* -----------------------------------------------------------------------
 * Globals needed by C code
 * --------------------------------------------------------------------- */
        .globl  EXT(cnvmem)
        .globl  EXT(extmem)
        .globl  EXT(boottype)
        .globl  EXT(boothowto)
        .globl  EXT(esym)
        .globl  EXT(pstart)
        .globl  EXT(gdt)
        .globl  EXT(idt)
        .globl  EXT(kpde)
        .globl  EXT(kpte)
        .globl  EXT(scall_dscr)
        .globl  EXT(sigret_dscr)
        .globl  high_start

/* -----------------------------------------------------------------------
 * Kernel stacks -- two separate 4096-byte regions in .text so they land
 * at known low physical addresses.
 * Labels are at the BOTTOM; add 4092 to get a usable top pointer.
 * --------------------------------------------------------------------- */
        .text
DATA(intstack)
        .space  4096
DATA(df_stack)
        .space  4096

/* -----------------------------------------------------------------------
 * GDTR / IDTR pseudo-descriptors (6 bytes: 2-byte limit + 4-byte base).
 * Bases are pre-converted to physical addresses (KV already subtracted)
 * so they are valid before paging is enabled.
 * --------------------------------------------------------------------- */
        .data
        .align  4

GDTdscr:
        .word   GDTLIM
        .long   EXT(gdt) - KV           /* physical address of GDT         */

IDTdscr:
        .word   IDTLIM
        .long   EXT(idt) - KV           /* physical address of IDT         */

/* -----------------------------------------------------------------------
 * _pstart -- kernel entry point from GRUB
 *   On entry: 32-bit protected mode, paging OFF, interrupts OFF (hopefully)
 *   GRUB's flat 4GB code/data segments are still live.
 * --------------------------------------------------------------------- */
        .text
        .globl  _pstart

Entry(pstart)

        /* ----------------------------------------------------------------
         * Disable timer IRQ.  picinit() hasn't run yet; a timer interrupt
         * before that causes an unhandled trap and triple-fault.
         * ---------------------------------------------------------------- */
        mov     $0x21, %edx
        INB
        orb     $1, %al
        OUTB

        /* ----------------------------------------------------------------
         * Fill kpte with 3072 PTEs, mapping physical 0x00000000-0x00BFFFFF
         * (12MB -- enough to cover the full kernel image + BSS).
         * Each PTE: bits[31:12] = page frame number, bits[1:0] = Present+RW
         * ---------------------------------------------------------------- */
        mov     $EXT(kpte), %edi
        and     $MASK, %edi             /* physical address of kpte         */

        xor     %eax, %eax             /* start at physical page 0         */
        mov     $3072, %ecx            /* 3072 pages x 4KB = 12MB          */

fill_pte:
        mov     %eax, %edx
        or      $0x003, %edx           /* Present + Read/Write             */
        mov     %edx, (%edi)
        add     $4, %edi               /* next PTE slot                    */
        add     $0x1000, %eax          /* next page frame                  */
        loop    fill_pte

        /* ----------------------------------------------------------------
         * Set up kpde (page directory).
         * We need two windows into the same 12MB physical range:
         *   Identity map: PDE[0,1,2]     -> physical 0x00000000-0x00BFFFFF
         *   Higher half:  PDE[768,769,770]-> virtual  0xC0000000-0xC0BFFFFF
         *
         * PDE index for 0xC0000000 = 0xC0000000 >> 22 = 768
         * Each PDE entry = physical base of a 4KB page table | flags
         * kpte is 3 consecutive page tables, each 4096 bytes apart.
         * ---------------------------------------------------------------- */
        mov     $EXT(kpde), %ebx
        and     $MASK, %ebx             /* physical address of kpde         */

        /* First page table (kpte+0): covers 0x00000000-0x003FFFFF */
        mov     $EXT(kpte), %eax
        and     $MASK, %eax
        and     $0xfffff000, %eax       /* page-align                       */
        or      $0x003, %eax           /* Present + R/W                    */
        movl    %eax, 0(%ebx)          /* PDE[0]   identity                */
        movl    %eax, 3072(%ebx)       /* PDE[768] higher-half             */

        /* Second page table (kpte+4096): covers 0x00400000-0x007FFFFF */
        add     $4096, %eax
        movl    %eax, 4(%ebx)          /* PDE[1]                           */
        movl    %eax, 3076(%ebx)       /* PDE[769]                         */

        /* Third page table (kpte+8192): covers 0x00800000-0x00BFFFFF */
        add     $4096, %eax
        movl    %eax, 8(%ebx)          /* PDE[2]                           */
        movl    %eax, 3080(%ebx)       /* PDE[770]                         */

        /* ----------------------------------------------------------------
         * Munge descriptor tables from Mach's compile-time init format
         * into the i386 hardware runtime format.
         * munge_table(%eax = phys base, %ecx = size in bytes)
         * NOTE: munge_table clobbers %eax, %ebx, %ecx, %edx -- do NOT
         * load CR3-destined EDX until after all munge_table calls.
         * ---------------------------------------------------------------- */

        /* Patch GDTdscr limit field before munging */
        mov     $GDTdscr, %ebx
        and     $MASK, %ebx
        mov     $GDTLIM, %ecx
        movw    %cx, (%ebx)

        /* Munge GDT */
        mov     $EXT(gdt), %eax
        and     $MASK, %eax
        mov     $GDTLIM, %ecx
        call    munge_table

        /* Munge IDT */
        mov     $EXT(idt), %eax
        and     $MASK, %eax
        mov     $IDTLIM, %ecx
        call    munge_table

        /* Munge system call gate */
        mov     $EXT(scall_dscr), %eax
        and     $MASK, %eax
        mov     $1, %ecx
        call    munge_table

        /* Munge signal return gate */
        mov     $EXT(sigret_dscr), %eax
        and     $MASK, %eax
        mov     $1, %ecx
        call    munge_table

        /* ----------------------------------------------------------------
         * Load descriptor tables.
         * Paging is still OFF; IDTdscr/GDTdscr already hold physical addrs.
         * ---------------------------------------------------------------- */
        mov     $IDTdscr, %eax
        and     $MASK, %eax
        lidt    (%eax)

        mov     $GDTdscr, %eax
        and     $MASK, %eax
        lgdt    (%eax)

        /* ----------------------------------------------------------------
         * Enable paging.
         * Load CR3 FIRST (Intel manual requirement), then set CR0.PG.
         * EDX is loaded HERE, after all munge_table calls that clobber it.
         * ---------------------------------------------------------------- */
        mov     $EXT(kpde), %edx
        and     $MASK, %edx            /* physical address of kpde -> CR3  */
        mov     %edx, %cr3

        mov     %cr0, %eax
        or      $PAGEBIT, %eax
        mov     %eax, %cr0             /* paging ON                        */

        /* ----------------------------------------------------------------
         * Far jump to flush the prefetch queue.
         * After this, EIP is a virtual 0xC01xxxxx address.
         * The identity map (PDE[0]) keeps the next few physical instructions
         * reachable until we land in high_start.
         * ---------------------------------------------------------------- */
        ljmp    $KCODE, $high_start

/* -----------------------------------------------------------------------
 * high_start
 *   Paging is now ON.  We are executing at virtual 0xC01xxxxx.
 *   Reload all segment registers from our freshly-munged GDT.
 * --------------------------------------------------------------------- */
high_start:

        /* Reload data segments from our GDT (KDATA = 0x10) */
        mov     $KDATA, %ax
        mov     %ax, %ds
        mov     %ax, %es
        mov     %ax, %ss
        mov     %ax, %fs
        mov     %ax, %gs

        /* Set up kernel stack explicitly - do NOT trust TSS or GRUB */
        mov     $EXT(intstack), %esp
        add     $4092, %esp
        xor     %ebp, %ebp

        /* Load TR so privilege-change stack switching works later */
        mov     $KTSSSEL, %ax
        ltr     %ax

        /* ----------------------------------------------------------------
         * Hardware task switch into the kernel TSS.
         *
         * KTSSSEL must be the GDT selector for the kernel TSS descriptor.
         * The TSS must have valid ESP0/SS0, CR3, and EIP fields pointing
         * at i386_init before this executes (set up in tables.c/pcb.c).
         *
         * If you need to bypass this for bring-up, replace with:
         *   call  EXT(i386_init)
         * ---------------------------------------------------------------- */
#if 0
        ljmp    $KTSSSEL, $0x0
#else
        /* Call i386_init directly on our own stack */
        /* Call i386_init via register to ensure virtual address */
/*        mov     $EXT(i386_init), %eax	
        call    *%eax	*/
	call  EXT(vstart)
#endif

        /* Should never reach here */
        jmp     hang

/* -----------------------------------------------------------------------
 * munge_table
 *   Walk a descriptor table and reformat each 8-byte entry from Mach's
 *   compile-time layout into the i386 hardware layout.
 *
 *   Entry:  %eax = physical base of table
 *           %ecx = size of table in bytes (limit value)
 *   Clobbers: %eax, %ebx, %ecx, %edx
 * --------------------------------------------------------------------- */
munge_table:
        mov     %eax, %ebx
        add     %ebx, %ecx             /* %ecx = one-past-end address      */

moretable:
        cmp     %ebx, %ecx
        jl      donetable

        movb    7(%ebx), %al           /* fetch type byte                  */
        testb   $0x10, %al            /* segment descriptor?              */
        jne     notagate
        testb   $0x04, %al            /* gate descriptor?                 */
        je      notagate

        /* Gate descriptor: reorder offset[0:15], selector, type, offset[16:31] */
        movw    6(%ebx), %ax           /* type word                        */
        movw    4(%ebx), %dx           /* selector                         */
        movw    %ax, 4(%ebx)           /* type -> [4:5]                    */
        movw    2(%ebx), %ax           /* offset[16:31]                    */
        movw    %dx, 2(%ebx)           /* selector -> [2:3]                */
        movw    %ax, 6(%ebx)           /* offset[16:31] -> [6:7]           */
        jmp     descdone

notagate:
        /* Segment descriptor: reorder base/limit fields */
        movw    4(%ebx), %dx           /* limit[0:15]                      */
        movb    %al, 5(%ebx)           /* type -> [5]                      */
        movw    2(%ebx), %ax           /* base[16:31]                      */
        movb    %al, 4(%ebx)           /* base[16:23] -> [4]               */
        movb    %ah, 7(%ebx)           /* base[24:31] -> [7]               */
        movw    (%ebx), %ax            /* base[0:15]                       */
        movw    %ax, 2(%ebx)           /* base[0:15] -> [2:3]              */
        movw    %dx, (%ebx)            /* limit[0:15] -> [0:1]             */

descdone:
        add     $8, %ebx              /* advance to next descriptor        */
        jmp     moretable

donetable:
        ret

/* -----------------------------------------------------------------------
 * hang -- used if early boot fails catastrophically
 * --------------------------------------------------------------------- */
hang:
        cli
        hlt
        jmp     hang
