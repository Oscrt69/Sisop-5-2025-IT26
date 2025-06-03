# Compiler & tools
ASM = nasm
ASMFLAGS = -f bin

CC = gcc
CFLAGS = -m16 -ffreestanding -O2 -Wall -Wextra -Wno-unused -c

LD = ld
LDFLAGS = -T linker.ld -m elf_i386

# File source
ASM_SRC = kernel.asm
C_SRC = kernel.c shell.c std_lib.c

OBJ = kernel.o shell.o std_lib.o

# Target image
IMG = floppy.img

# Rules

all: $(IMG)

kernel.bin: $(ASM_SRC)
	$(ASM) $(ASMFLAGS) -o kernel.bin $<

kernel.o: kernel.c
	$(CC) $(CFLAGS) -o kernel.o kernel.c

shell.o: shell.c
	$(CC) $(CFLAGS) -o shell.o shell.c

std_lib.o: std_lib.c
	$(CC) $(CFLAGS) -o std_lib.o std_lib.c

kernel.elf: $(OBJ)
	$(LD) $(LDFLAGS) -o kernel.elf $(OBJ)

$(IMG): kernel.bin kernel.elf
	# Buat floppy.img 1.44MB kosong
	dd if=/dev/zero of=$(IMG) bs=512 count=2880 status=none
	# Copy kernel.bin ke sector pertama (bootloader)
	dd if=kernel.bin of=$(IMG) bs=512 count=1 conv=notrunc status=none
	# Copy kernel.elf ke image (sesuaikan offset jika perlu)
	# Misal taruh kernel.elf mulai sector ke-2 dst, tergantung bootloader
	# (kalau butuh loader, sesuaikan ini)
	# dd if=kernel.elf of=$(IMG) bs=512 seek=1 conv=notrunc

clean:
	rm -f *.o *.bin *.elf $(IMG)

.PHONY: all clean
