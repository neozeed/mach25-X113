@del Mach25-MK35_minimal.vmdk
@7zip x Mach25-MK35_minimal.vmdk.7z
@REM we now pull the kernel from the 2nd ide disk
@REM as make places the stripped kernel where it needs to be now
@REM wsl grub-mkrescue -o mach.iso iso 
\qemu.mingw64\qemu-system-i386 -m 32 -machine pc,accel=tcg ^
  -M isapc -parallel none ^
  -drive file=Mach25-MK35_minimal.vmdk,format=vmdk,if=ide,bus=0,unit=0 ^
  -drive file=fat:rw:./iso,format=raw,if=ide,bus=0,unit=1 ^
  -drive file=mach.iso,format=raw,if=ide,bus=1,unit=0,media=cdrom ^
  -boot d