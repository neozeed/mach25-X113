ENTRY(_pstart)

/* Mach 2.5 kernel virtual base */
VIRT = 0xC0100000;

/* Physical load address (1 MB) */
PHYS = 0x00100000;

PHDRS
{
  text PT_LOAD FLAGS(0x5);  /* R+X */
  data PT_LOAD FLAGS(0x6);  /* R+W */
}

SECTIONS
{
  /* Link at high virtual address */
  . = VIRT;

  /* TEXT: goes in PT_LOAD "text", LMA = PHYS */
  .text : AT(PHYS) {
      *(.text*)
  } :text
  _etext = .;

  /* RODATA: still in "text" segment, contiguous after .text */
  .rodata : AT(PHYS + (. - VIRT)) {
      *(.rodata*)
  } :text

  /* DATA: start "data" PT_LOAD, LMA tracks VIRT→PHYS offset */
  .data : AT(PHYS + (. - VIRT)) { *(.data*) } :data
  _edata = .;

  /* BSS: also in "data" PT_LOAD, zeroed by loader */
  .bss  : AT(PHYS + (. - VIRT)) { *(.bss*) *(COMMON) } :data
  _end = .;

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
