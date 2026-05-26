my goal is cross building Mach from a Win32/elf tool chain.

<img width="902" height="540" alt="image" src="https://github.com/user-attachments/assets/b98135a0-61cc-4b0b-8684-022785d9739d" />

Just booted into single user mode!

loading vmunix.sys
ld -o a.out -e _pstart btptes.o tables.o  locore.o cmu_syscalls.o init_main.o 
expr.o format.o input.o kdb_main.o message.o opset.o output.o pcs.o print.o runpcs.o setup.o stack.o sym.o tbls.o utls.o clock.o copy_user.o fpsup.o FPU_start.o  FPU_end.o gcc.o i386_init.o pic.o pit.o in_cksum.o kdb.o machdep.o mem.o ntoh.o pcb.o phys.o pmap.o sys_machdep.o str.o trap.o ufs_machdep.o vm_machdep.o vx_exception.o autoconf.o com.o lpr.o conf.o fd.o hd.o iopl.o if_par.o if_3c501.o if_pc586.o if_ns8390.o kd.o kdasm.o kd_event.o kd_mouse.o kd_queue.o pic_isa.o rtc.o wt.o   vers.o vmunix.swap
rearranging symbols
text    data    bss     dec     hex     filename
413185  54812   120724  588721  8fbb1   a.out
high_start is located at
c010210b T high_start
ln vmunix.sys vmunix

file vmunix
vmunix: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), statically linked, not stripped

I've had AI help create a multiboot loader / grub cd-rom that can boot the kernel.  I can bring it up under GDB
however the TSS hardware multitasking is currently broken so it'll hard reset when it tries to jump into a thread.
I've updated the hard disk driver to reset the disk, and they initalize.

The grub-cdrom pulls the kernel from the 'iso' directory so you don't have to constnatly re-make the image now
so you will always have the latest kernel populated.

There is no config or mig, as this is a pre-configured tree!

cd to obj\STD+WS-afs-nfs\ and just run Make!

look at the mkiso.cmd & deb.cmd in the object & root for running the kernel under GDB.
