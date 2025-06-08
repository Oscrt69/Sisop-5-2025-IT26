prepare:
	dd if=/dev/zero of=bin/floppy.img bs=512 count=2880

bootloader:
	nasm -f bin src/bootloader.asm -o bin/bootloader.bin
	dd if=bin/bootloader.bin of=bin/floppy.img bs=512 count=1 conv=notrunc

stdlib:
	bcc -ansi -Iinclude -c src/std_lib.c -o bin/std_lib.o

shell:
	bcc -ansi -Iinclude -c src/shell.c -o bin/shell.o

kernel:
	nasm -f as86 src/kernel.asm -o bin/kernel-asm.o
	bcc -ansi -Iinclude -c src/kernel.c -o bin/kernel.o
   
link:
	ld86 -o bin/kernel.bin -d bin/kernel.o bin/kernel-asm.o  bin/std_lib.o bin/shell.o
	dd if=bin/kernel.bin of=bin/floppy.img bs=512 seek=1 conv=notrunc

build: prepare bootloader stdlib shell kernel link
