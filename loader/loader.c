typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

/* --- Multiboot --------------------------------------------------------- */

struct multiboot_module {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;
    uint32_t reserved;
};

struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower, mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    /* rest ignored */
};

/* --- ELF --------------------------------------------------------------- */

struct elf32_ehdr {
    uint8_t  e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct elf32_phdr {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
};

#define PT_LOAD 1

static void memcpy32(void *dst, const void *src, uint32_t len)
{
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    while (len--)
        *d++ = *s++;
}

/* --- VGA text output --------------------------------------------------- */

#define VGA_TEXT ((unsigned short *)0xB8000)
static int vga_pos = 0;

static void vga_putc(char c)
{
    if (c == '\n') {
        vga_pos = (vga_pos / 80 + 1) * 80;
        return;
    }
    VGA_TEXT[vga_pos++] = (0x07 << 8) | (unsigned char)c;
}

static void vga_print(const char *s)
{
    while (*s)
        vga_putc(*s++);
}

static void vga_hex32(uint32_t x)
{
    int s;
    const char *hex = "0123456789ABCDEF";
    for (s = 28; s >= 0; s -= 4)
        vga_putc(hex[(x >> s) & 0xF]);
}

static void vga_dec(uint32_t x)
{
    char buf[11];
    int i = 0;

    if (x == 0) {
        vga_putc('0');
        return;
    }

    while (x && i < 10) {
        buf[i++] = '0' + (x % 10);
        x /= 10;
    }
    while (i--)
        vga_putc(buf[i]);
}

static void vga_clear(void)
{
    int i;
    for (i = 0; i < 80 * 25; i++)
        VGA_TEXT[i] = (0x07 << 8) | ' ';
    vga_pos = 0;
}

/* --- Loader core ------------------------------------------------------- */

/*
 * Mach 2.5 kernel:
 *   VIRT (link) = 0xC0100000
 *   PHYS (text) = 0x00200000
 * So the usual kernel virtual base is 0xC0000000.
 */
#define KERN_VIRT_BASE 0xC0000000U

void loader_main(uint32_t magic, struct multiboot_info *mbi)
{
    struct multiboot_module *mods;
    uint8_t *kernel;
    struct elf32_ehdr *eh;
    struct elf32_phdr *ph;
    int i;

    vga_clear();
    vga_print("Loader: entered loader_main()\n");

    if (magic != 0x2BADB002) {
        vga_print("Loader: bad multiboot magic\n");
        goto hang;
    }

    if (!(mbi->flags & (1U << 3))) {
        vga_print("Loader: no modules flag\n");
        goto hang;
    }

    if (mbi->mods_count == 0) {
        vga_print("Loader: mods_count == 0\n");
        goto hang;
    }

    mods = (struct multiboot_module *)mbi->mods_addr;
    vga_print("Loader: mods_addr=");
    vga_hex32(mbi->mods_addr);
    vga_print(" count=");
    vga_dec(mbi->mods_count);
    vga_putc('\n');

    vga_print("Loader: mod[0] start=");
    vga_hex32(mods[0].mod_start);
    vga_print(" end=");
    vga_hex32(mods[0].mod_end);
    vga_putc('\n');

    kernel = (uint8_t *)mods[0].mod_start;
    eh = (struct elf32_ehdr *)kernel;

    vga_print("ELF dump: ");
    {
        int k;
        for (k = 0; k < 32; k++) {
            uint8_t b = ((uint8_t*)eh)[k];
            const char *hex = "0123456789ABCDEF";
            vga_putc(hex[(b >> 4) & 0xF]);
            vga_putc(hex[b & 0xF]);
            vga_putc(' ');
        }
    }
    vga_putc('\n');

    vga_print("Loader: ELF magic=");
    vga_hex32(eh->e_ident[0]);
    vga_putc(' ');
    vga_putc(eh->e_ident[1]);
    vga_putc(eh->e_ident[2]);
    vga_putc(eh->e_ident[3]);
    vga_putc('\n');

    if (eh->e_ident[0] != 0x7f ||
        eh->e_ident[1] != 'E' ||
        eh->e_ident[2] != 'L' ||
        eh->e_ident[3] != 'F') {
        vga_print("Loader: not an ELF kernel\n");
        goto hang;
    }

    vga_print("Loader: phoff=");
    vga_hex32(eh->e_phoff);
    vga_print(" phnum=");
    vga_dec(eh->e_phnum);
    vga_putc('\n');

    ph = (struct elf32_phdr *)((uint8_t *)kernel + eh->e_phoff);

    /* Load all PT_LOAD segments */
    for (i = 0; i < (int)eh->e_phnum; i++) {
        uint32_t j;
        uint8_t *src;
        uint8_t *dst;

        vga_print("Loader: PH[");
        vga_dec((uint32_t)i);
        vga_print("] type=");
        vga_hex32(ph[i].p_type);
        vga_putc('\n');

        if (ph[i].p_type != PT_LOAD)
            continue;

        src = kernel + ph[i].p_offset;

        /*
         * Old binutils sometimes set p_paddr == p_vaddr for high segments.
         * If p_paddr is below KERN_VIRT_BASE, treat it as a real physical
         * address. Otherwise, map via KERN_VIRT_BASE.
         */
        if (ph[i].p_paddr < KERN_VIRT_BASE) {
            dst = (uint8_t *)ph[i].p_paddr;
        } else {
            dst = (uint8_t *)(ph[i].p_vaddr - KERN_VIRT_BASE);
        }

        vga_print("  LOAD off=");
        vga_hex32(ph[i].p_offset);
        vga_print(" vaddr=");
        vga_hex32(ph[i].p_vaddr);
        vga_print(" paddr=");
        vga_hex32(ph[i].p_paddr);
        vga_print(" dst=");
        vga_hex32((uint32_t)dst);
        vga_print(" filesz=");
        vga_hex32(ph[i].p_filesz);
        vga_print(" memsz=");
        vga_hex32(ph[i].p_memsz);
        vga_putc('\n');

        memcpy32(dst, src, ph[i].p_filesz);

        /* zero BSS portion */
        for (j = ph[i].p_filesz; j < ph[i].p_memsz; j++)
            dst[j] = 0;
    }

    vga_print("Loader: finished PH loop\n");

    /*
     * Compute physical entry from first PT_LOAD segment.
     * We assume ph[0] is the text segment:
     *   seg_v = 0xC0100000
     *   seg_p = 0x00200000
     *   e_entry = 0xC0101000
     *   entry_phys = seg_p + (e_entry - seg_v) = 0x00201000
     */
    {
        uint32_t seg_v = 0;
        uint32_t seg_p = 0;
        uint32_t entry_virt = eh->e_entry;
        uint32_t entry_phys = 0;
        void (*entry)(void);

        /* find first PT_LOAD to use as base */
        for (i = 0; i < (int)eh->e_phnum; i++) {
            if (ph[i].p_type == PT_LOAD) {
                seg_v = ph[i].p_vaddr;
                seg_p = (ph[i].p_paddr < KERN_VIRT_BASE)
                        ? ph[i].p_paddr
                        : (ph[i].p_vaddr - KERN_VIRT_BASE);
                break;
            }
        }

        /* fallback: if not found, bail */
        if (seg_v == 0) {
            vga_print("Loader: no PT_LOAD for entry base\n");
            goto hang;
        }

        entry_phys = seg_p + (entry_virt - seg_v);

        vga_print("Loader: e_entry=");
        vga_hex32(entry_virt);
        vga_print(" phys=");
        vga_hex32(entry_phys);
        vga_putc('\n');

        vga_print("Loader: jumping to entry()\n");

        /* if you want to see that we never leave the loader, hang here */
        /* goto hang; */

        __asm__ __volatile__("cli");

        entry = (void (*)(void))entry_phys;
        entry();
    }

hang:
    vga_print("Loader: hanging()\n");
    for (;;)
        __asm__ __volatile__("hlt");
}
