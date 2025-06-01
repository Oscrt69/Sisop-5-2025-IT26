# Makefile untuk AmbatubOS

# Tools
AS = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

# Flags
CFLAGS = -ffreestanding -m32 -c -Wall -Wextra
ASFLAGS = -f elf

# Files
ASM_SRC = kernel.asm
C_SRC = kernel.c shell.c std_lib.c
OBJ = kernel.o kernelasm.o shell.o std_lib.o
BIN = kernel.bin

# Build steps
all: $(BIN)

prepare:
	mkdir -p build

kernelasm.o: kernel.asm
	$(AS) $(ASFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(BIN): $(OBJ)
	$(LD) -m elf_i386 -Ttext 0x1000 -o kernel.elf $(OBJ)
	$(OBJCOPY) -O binary kernel.elf $(BIN)

clean:
	rm -f *.o *.elf *.bin
