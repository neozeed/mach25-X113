typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

/* ------------------------------------------------------------- */
/* VGA                                                           */
/* ------------------------------------------------------------- */

#define VGA ((uint16_t *)0xB8000)
#define VERBOSE

static int vga_pos = 0;

static void vga_putc(char c)
{
    if (c == '\n') {
        vga_pos = (vga_pos / 80 + 1) * 80;
        return;
    }
#ifdef VERBOSE
    VGA[vga_pos++] = (0x07 << 8) | (unsigned char)c;
#endif
}

static void vga_print(const char *s)
{
    while (*s) vga_putc(*s++);
}

static void vga_hex(uint32_t v)
{
    static const char *h = "0123456789ABCDEF";
    int i;
    for (i = 28; i >= 0; i -= 4)
        vga_putc(h[(v >> i) & 0xF]);
}

static void panic(const char *s)
{
    vga_print("PANIC: ");
    vga_print(s);
    vga_putc('\n');
    for (;;) ;
}

static void vga_clear(void)
{
    int i;
    for (i = 0; i < 80 * 25; i++)
        VGA[i] = (0x07 << 8) | ' ';
    vga_pos = 0;
}

/* ------------------------------------------------------------- */
/* MULTIBOOT                                                     */
/* ------------------------------------------------------------- */

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
};

/* ------------------------------------------------------------- */
/* ELF32                                                         */
/* ------------------------------------------------------------- */

/*#pragma pack(1)
*/

typedef struct {
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
} Elf32_Ehdr;

typedef struct {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
} Elf32_Phdr;

typedef struct {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
} Elf32_Shdr;

typedef struct {
    uint32_t st_name;
    uint32_t st_value;   /* virtual address */
    uint32_t st_size;
    uint8_t  st_info;
    uint8_t  st_other;
    uint16_t st_shndx;
} Elf32_Sym;

/*#pragma pack()
*/

#define PT_LOAD   1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define KERN_VIRT_BASE 0xC0000000UL

/* ------------------------------------------------------------- */
/* mem utilities                                                 */
/* ------------------------------------------------------------- */

static void memcpy_safe(void *dst, const void *src, uint32_t len)
{
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    while (len--) *d++ = *s++;
}

static void memset_safe(void *dst, uint8_t v, uint32_t len)
{
    uint8_t *d = (uint8_t *)dst;
    while (len--) *d++ = v;
}

static int strcmp_simple(const char *a, const char *b)
{
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

/* ------------------------------------------------------------- */
/* ELF symbol lookup                                             */
/*                                                               */
/* Scans the ELF symbol table in the loaded module image to find */
/* a symbol by name. Returns its virtual address or 0.          */
/* ------------------------------------------------------------- */

static uint32_t elf_find_symbol(uint8_t *mod_base, Elf32_Ehdr *eh,
                                 const char *name)
{
    uint32_t i;
    Elf32_Shdr *shdrs;
    Elf32_Shdr *symtab_sh = 0;
    Elf32_Shdr *strtab_sh = 0;
    Elf32_Sym  *syms;
    char       *strtab;
    uint32_t    nsyms;

    if (eh->e_shoff == 0 || eh->e_shnum == 0)
        return 0;

    shdrs = (Elf32_Shdr *)(mod_base + eh->e_shoff);

    /* Find SHT_SYMTAB and its associated SHT_STRTAB */
    for (i = 0; i < eh->e_shnum; i++) {
        if (shdrs[i].sh_type == SHT_SYMTAB) {
            symtab_sh = &shdrs[i];
            strtab_sh = &shdrs[shdrs[i].sh_link];
            break;
        }
    }

    if (!symtab_sh || !strtab_sh)
        return 0;

    syms    = (Elf32_Sym *)(mod_base + symtab_sh->sh_offset);
    strtab  = (char *)     (mod_base + strtab_sh->sh_offset);
    nsyms   = symtab_sh->sh_size / sizeof(Elf32_Sym);

    for (i = 0; i < nsyms; i++) {
        const char *sname = strtab + syms[i].st_name;
        if (strcmp_simple(sname, name) == 0)
            return syms[i].st_value;   /* virtual address */
    }

    return 0;
}

/* Convert kernel virtual address to physical (for writing before paging) */
static uint32_t *kern_sym_phys(uint8_t *mod_base, Elf32_Ehdr *eh,
                                const char *name)
{
    uint32_t vaddr = elf_find_symbol(mod_base, eh, name);
    if (vaddr == 0) return 0;
    if (vaddr < KERN_VIRT_BASE) return 0;
    return (uint32_t *)(vaddr - KERN_VIRT_BASE);
}

/* Write a uint32 into a kernel symbol, converting virt->phys */
static void kern_write(uint8_t *mod_base, Elf32_Ehdr *eh,
                       const char *name, uint32_t value)
{
    uint32_t *ptr = kern_sym_phys(mod_base, eh, name);
    if (!ptr) {
        vga_print("WARN: symbol not found: ");
        vga_print(name);
        vga_putc('\n');
        return;
    }
    *ptr = value;
}

/* ------------------------------------------------------------- */
/* boottype encoding (from swapgeneric.c)                        */
/*                                                               */
/* Mach boottype layout:                                         */
/*  bits [7:0]   = device type (0=hd, 1=fd, 3=aha/sd)          */
/*  bits [15:8]  = partition (0='a', 1='b', ...)                */
/*  bits [19:16] = unit number                                   */
/*  bits [23:20] = controller                                    */
/*  bits [27:24] = adaptor                                       */
/*                                                               */
/* GRUB boot_device:                                             */
/*  bits [31:24] = BIOS drive (0x80=hd0, 0x81=hd1, 0=fd0...)   */
/*  bits [23:16] = top-level partition (0xff=none)              */
/*  bits [15:8]  = sub-partition                                 */
/*  bits [7:0]   = sub-sub-partition                            */
/* ------------------------------------------------------------- */

#define B_TYPESHIFT      0
#define B_PARTITIONSHIFT 8
#define B_UNITSHIFT      16
#define B_CONTROLLERSHIFT 20

/* Boot device -> Mach boottype encoding */
static uint32_t make_boottype(uint32_t grub_boot_device)
{
    uint32_t bios_drive  = (grub_boot_device >> 24) & 0xFF;
    uint32_t partition   = (grub_boot_device >> 16) & 0xFF;
    uint32_t type, unit, slice;

    if (partition == 0xFF) partition = 0;  /* no partition info */

    if (bios_drive & 0x80) {
        /* hard disk: BIOS 0x80=hd0, 0x81=hd1, ... */
        type = 0;                           /* hd = type 0 */
        unit = bios_drive & 0x7F;           /* 0, 1, ... */
    } else {
        /* floppy: BIOS 0x00=fd0, 0x01=fd1, ... */
        type = 1;                           /* fd = type 1 */
        unit = bios_drive & 0x7F;
    }

    slice = partition;                      /* 0='a', 1='b', ... */

    return (type  << B_TYPESHIFT)      |
           (slice << B_PARTITIONSHIFT) |
           (unit  << B_UNITSHIFT);
}

/* ------------------------------------------------------------- */
/* loader_main                                                   */
/* ------------------------------------------------------------- */

void _loader_main(uint32_t magic, struct multiboot_info *mbi)
{
    struct multiboot_module *mods;
    Elf32_Ehdr *eh;
    Elf32_Phdr *ph;
    uint32_t    mod_start, entry_phys, i;
    void      (*entry)(void);
    uint8_t   *safe_mod = (uint8_t *)0x00800000;  /* 8MB staging area */

    vga_clear();
    vga_print("Mach loader\n");

    if (magic != 0x2BADB002UL)
        panic("bad multiboot magic");

    if (!(mbi->flags & (1UL << 3)))
        panic("no modules flag");

    if (mbi->mods_count == 0)
        panic("no modules");

    mods = (struct multiboot_module *)mbi->mods_addr;
    mod_start = mods[0].mod_start;

    vga_print("kernel module @ ");
    vga_hex(mod_start);
    vga_print(", size=");
    vga_hex(mods[0].mod_end - mod_start);
    vga_putc('\n');

    /* Copy module to safe area (avoids overlap during segment loading) */
    memcpy_safe(safe_mod, (void *)mod_start, mods[0].mod_end - mod_start);
    mod_start = (uint32_t)safe_mod;

    eh = (Elf32_Ehdr *)mod_start;

    /* Validate ELF header */
    if (eh->e_ident[0] != 0x7F || eh->e_ident[1] != 'E' ||
        eh->e_ident[2] != 'L'  || eh->e_ident[3] != 'F')
        panic("not ELF");

    vga_print("ELF: entry=");
    vga_hex(eh->e_entry);
    vga_print(" phnum=");
    vga_hex(eh->e_phnum);
    vga_print(" shnum=");
    vga_hex(eh->e_shnum);
    vga_putc('\n');

    ph = (Elf32_Phdr *)(mod_start + eh->e_phoff);

    /* Load PT_LOAD segments */
    for (i = 0; i < eh->e_phnum; i++) {
        uint8_t *src, *dst;

        if (ph[i].p_type != PT_LOAD)
            continue;

        src = (uint8_t *)(mod_start + ph[i].p_offset);
        dst = (uint8_t *)(ph[i].p_vaddr - KERN_VIRT_BASE);

        vga_print("LOAD phys=");
        vga_hex((uint32_t)dst);
        vga_print(" filesz=");
        vga_hex(ph[i].p_filesz);
        vga_print(" memsz=");
        vga_hex(ph[i].p_memsz);
        vga_putc('\n');

        memcpy_safe(dst, src, ph[i].p_filesz);

        if (ph[i].p_memsz > ph[i].p_filesz)
            memset_safe(dst + ph[i].p_filesz, 0,
                        ph[i].p_memsz - ph[i].p_filesz);
    }

    /* ---------------------------------------------------------------
     * Patch kernel globals via ELF symbol table lookup.
     * All writes go to physical addresses (virt - 0xC0000000).
     * This must happen AFTER segments are loaded so BSS is zeroed,
     * and BEFORE the kernel runs so it sees our values.
     * ------------------------------------------------------------- */

    vga_print("Patching kernel globals...\n");

    /* Physical load address of kernel */
    kern_write((uint8_t *)mod_start, eh, "_loadpt", 0x00100000);

    /* Conventional memory (KB) from multiboot */
    if (mbi->flags & 0x1) {
        kern_write((uint8_t *)mod_start, eh, "_cnvmem", mbi->mem_lower);
        kern_write((uint8_t *)mod_start, eh, "_extmem", mbi->mem_upper);
        vga_print("mem_lower=");
        vga_hex(mbi->mem_lower);
        vga_print("KB  mem_upper=");
        vga_hex(mbi->mem_upper);
        vga_print("KB\n");
    } else {
        /* Fallback if GRUB didn't report memory */
        kern_write((uint8_t *)mod_start, eh, "_cnvmem", 640);
        kern_write((uint8_t *)mod_start, eh, "_extmem", 14 * 1024);
        vga_print("mem: using defaults\n");
    }

    /* Pass raw multiboot info pointer and magic for kernel's own use */
    kern_write((uint8_t *)mod_start, eh, "_mb_magic", magic);
    kern_write((uint8_t *)mod_start, eh, "_mb_info",  (uint32_t)mbi);

    /* Boot device -> Mach boottype encoding */
#if 0
    if (mbi->flags & 0x2) {
        uint32_t bt = make_boottype(mbi->boot_device);
        kern_write((uint8_t *)mod_start, eh, "_boottype", bt);
        vga_print("boot_device=");
        vga_hex(mbi->boot_device);
        vga_print(" boottype=");
        vga_hex(bt);
        vga_putc('\n');
    }
#else
    {
        uint32_t bt;
        uint32_t bios_drive = (mbi->flags & 0x2) ? 
                              ((mbi->boot_device >> 24) & 0xFF) : 0xFF;

        /* If booting from CD (0xE0) or any non-standard drive,
         * default to hd0 partition a - same as original Mach loader */
        if (bios_drive == 0x80) {
            bt = make_boottype(mbi->boot_device);
        } else {
            /* CD boot or unknown - hardcode hd0a */
            bt = (0 << B_TYPESHIFT)      |  /* type=0 (hd) */
                 (0 << B_PARTITIONSHIFT) |  /* partition=0 (a) */
                 (0 << B_UNITSHIFT);        /* unit=0 */
            vga_print("CD/unknown boot - defaulting to hd0a\n");
        }

        kern_write((uint8_t *)mod_start, eh, "_boottype", bt);
        vga_print("boottype=");
        vga_hex(bt);
        vga_putc('\n');
    }
#endif

    /* ---------------------------------------------------------------
     * Find physical entry point
     * ------------------------------------------------------------- */

    entry_phys = 0;
    for (i = 0; i < eh->e_phnum; i++) {
        if (ph[i].p_type != PT_LOAD)
            continue;
        if (eh->e_entry >= ph[i].p_vaddr &&
            eh->e_entry <  ph[i].p_vaddr + ph[i].p_memsz) {
            entry_phys = ph[i].p_paddr +
                         (eh->e_entry - ph[i].p_vaddr);
            break;
        }
    }

    if (entry_phys == 0) {
        /* Fallback: entry_phys = entry_virt - KERN_VIRT_BASE */
        if (eh->e_entry >= KERN_VIRT_BASE)
            entry_phys = eh->e_entry - KERN_VIRT_BASE;
        else
            panic("cannot compute entry physical address");
    }

    vga_print("Jumping to kernel @ phys ");
    vga_hex(entry_phys);
    vga_putc('\n');

    __asm__ __volatile__("cli");

    entry = (void (*)(void))entry_phys;
/*for(;;){}*/
    vga_clear();
    entry();

    panic("kernel returned");
}
