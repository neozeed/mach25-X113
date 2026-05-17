wsl grub-mkrescue -o mach.iso iso 
\qemu.mingw64\qemu-system-i386.exe  -m 32 -cdrom mach.iso -boot d -no-reboot