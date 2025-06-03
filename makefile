# Tools
ASM=nasm
CC=i686-elf-gcc
LD=i686-elf-ld
OBJCOPY=i686-elf-objcopy

# Flags
ASMFLAGS=-f bin
CFLAGS=-ffreestanding -O2 -Wall -Wextra -m32
LDFLAGS=-T linker.ld -nostdlib -m elf_i386

# Files
BOOTLOADER=bootloader.asm
KERNEL_ASM=kernel.asm
KERNEL_C=kernel.c
SHELL_C=shell.c
STD_LIB_C=std_lib.c

BOOTLOADER_BIN=bootloader.bin
KERNEL_ASM_BIN=kernel_asm.bin
KERNEL_O=kernel.o
SHELL_O=shell.o
STD_LIB_O=std_lib.o

KERNEL_BIN=kernel.bin
FLOPPY_IMG=floppy.img

# Target: Build all
all: $(FLOPPY_IMG)

# Compile bootloader.asm to raw binary 512 bytes
$(BOOTLOADER_BIN): $(BOOTLOADER)
	$(ASM) $(BOOTLOADER) -o $(BOOTLOADER_BIN)

# Compile kernel.asm to raw binary (if kernel.asm is flat binary)
$(KERNEL_ASM_BIN): $(KERNEL_ASM)
	$(ASM) $(KERNEL_ASM) -o $(KERNEL_ASM_BIN)

# Compile C files to object files
$(KERNEL_O): $(KERNEL_C)
	$(CC) $(CFLAGS) -c $< -o $@

$(SHELL_O): $(SHELL_C)
	$(CC) $(CFLAGS) -c $< -o $@

$(STD_LIB_O): $(STD_LIB_C)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel objects and kernel asm binary into kernel.bin (flat binary)
$(KERNEL_BIN): $(KERNEL_O) $(SHELL_O) $(STD_LIB_O) $(KERNEL_ASM_BIN)
	# Convert kernel asm binary to object file
	$(LD) -m elf_i386 -o kernel.elf $(KERNEL_O) $(SHELL_O) $(STD_LIB_O) -T linker.ld --oformat binary
	# Copy the linked ELF to raw binary kernel.bin (adjust as needed)
	$(OBJCOPY) -O binary kernel.elf $(KERNEL_BIN)

# Create floppy image with bootloader + kernel.bin
$(FLOPPY_IMG): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	# Create empty floppy image 1.44MB (1474560 bytes)
	dd if=/dev/zero of=$(FLOPPY_IMG) bs=512 count=2880 status=none
	# Write bootloader (first sector)
	dd if=$(BOOTLOADER_BIN) of=$(FLOPPY_IMG) bs=512 count=1 conv=notrunc status=none
	# Write kernel.bin starting at sector 1
	dd if=$(KERNEL_BIN) of=$(FLOPPY_IMG) bs=512 seek=1 conv=notrunc status=none

# Clean all build files
clean:
	rm -f $(BOOTLOADER_BIN) $(KERNEL_ASM_BIN) $(KERNEL_O) $(SHELL_O) $(STD_LIB_O) kernel.elf $(KERNEL_BIN) $(FLOPPY_IMG)

.PHONY: all clean
