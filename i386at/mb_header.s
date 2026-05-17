/* mb_header.S — Multiboot v1 header for Mach 2.6 */

    .section .mbhdr, "ax"
    .align 4

    .globl mbhdr_start
mbhdr_start:

#define MB_MAGIC        0x1BADB002
#define MB_FLAGS        0x00000003	/* no address fields, let ELF drive it */
#define MB_CHECKSUM     -(MB_MAGIC + MB_FLAGS)

    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECKSUM

