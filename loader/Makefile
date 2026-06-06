
# OS X Linux etc
CC=	/usr/local/i586-linux2/bin/i586-linux-gcc
CPP=	/usr/local/i586-linux2/lib/gcc-lib/i586-linux/2.8.1/cpp
LD=	/usr/local/i586-linux2/bin/i586-linux-ld
AS=	/usr/local/i586-linux2/bin/i586-linux-as
SIZE=	/usr/local/i586-linux2/bin/i586-linux-size
STRIP=	/usr/local/i586-linux2/bin/i586-linux-strip

# MinGW
#CC=	gcc-258_old
#AS=	as386
#LD=	ld

default: loader

loader_asm.o: loader_asm.s
	$(AS) loader_asm.s -o loader_asm.o

loader.o: loader.c
	$(CC) -c loader.c

loader: loader_asm.o loader.o
	$(LD) -T loader.ld -o loader loader_asm.o loader.o
	cp loader ../iso/boot

clean:
	rm -f loader loader_asm.o loader.o
