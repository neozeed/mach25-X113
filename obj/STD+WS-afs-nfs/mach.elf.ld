ENTRY(pstart)

PHDRS
{
    text PT_LOAD FLAGS(5);   /* R+X */
    data PT_LOAD FLAGS(6);   /* R+W */
}

PHYS = 0x00100000;
VIRT = 0xC0000000 + PHYS;


SECTIONS
{
    . = VIRT;

/*    .text : AT(PHYS) {	*/
    .text VIRT : AT(PHYS) {
        *(.text*)
    } :text
    etext = .;

    .rodata : AT(PHYS + (ADDR(.rodata) - VIRT)) {
        *(.rodata*)
    } :text

.data : AT(PHYS + (ADDR(.data) - VIRT)) {
    . = ALIGN(4096);
    *(.data*)
} :data
    edata = .;

.bss : AT(PHYS + (ADDR(.bss) - VIRT)) {
    . = ALIGN(4096);
    *(.bss*)
    *(COMMON)
} :data
    end = .;

  /DISCARD/ : {
      *(.comment)
      *(.note*)
      *(.eh_frame)
/*    *(.stab*)		we kinda want a debug kernel
      *(.debug*)
      *(.symtab)	*/
      *(.strtab)
  }
}


