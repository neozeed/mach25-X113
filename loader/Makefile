
default: loader

loader_asm.o: loader_asm.s
	a386 loader_asm.s -o loader_asm.o

loader.o: loader.c
	gcc -c loader.c

loader: loader_asm.o loader.o
	ld -T loader.ld -o loader loader_asm.o loader.o

clean:
	rm -f loader loader_asm.o loader.o