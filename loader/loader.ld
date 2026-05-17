ENTRY(loader_entry)

SECTIONS
{
  . = 0x00008000;      /* loader well below 1MB and below kernel */

  .text : {
    *(.multiboot)
    *(.text*)
  }

  .rodata : { *(.rodata*) }

  .data : { *(.data*) }

  .bss : {
    *(.bss*)
    *(COMMON)
  }
}
