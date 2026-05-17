ENTRY(_start)

VIRT = 0xC0100000;
PHYS = 0x00100000;

SECTIONS
{
  /* Kernel runs here virtually */
  . = VIRT;

  .text : AT(PHYS) {
      *(.multiboot)      /* put Multiboot header first */
      *(.text*)
  }
  _etext = .;		/* Define a.out-style symbols manually */

  .rodata : AT(PHYS + (. - VIRT)) {
      *(.rodata*)
  }

  .data : AT(PHYS + (. - VIRT)) {
      *(.data*)
  }
  _edata = .;		/* Define a.out-style symbols manually */

  .bss : AT(PHYS + (. - VIRT)) {
      *(.bss*)
      *(COMMON)
  }
  _end = .;		/* Define a.out-style symbols manually */


/DISCARD/ : {
    *(.comment)
    *(.note*)
    *(.eh_frame)
    *(.stab*)
    *(.debug*)
    *(.symtab)
    *(.strtab)
    }
}
