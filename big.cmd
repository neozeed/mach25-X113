\qemu.mingw64\qemu-system-i386 -m 32 -machine pc,accel=tcg ^
  -M isapc -parallel none ^
  -drive file=Mach25-MK35.vmdk,format=vmdk,if=ide,bus=0,unit=0 ^
  -drive file=2G.vmdk,format=vmdk,if=ide,bus=1,unit=0 ^
  -drive file=fat:rw:./iso,format=raw,if=ide,bus=0,unit=1 ^
  -drive file=mach.iso,format=raw,if=ide,bus=1,unit=1,media=cdrom ^
  -fda dos6\dos622_1.img -boot d