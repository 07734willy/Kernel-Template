TOPDIR := ..
ANYDIR := $(TOPDIR)/**
INCDIR := $(TOPDIR)/include
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
		 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I$(INCDIR) -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

OBJS := $(patsubst %.S, %.o, $(wildcard $(ANYDIR)/*.S))
OBJS += $(patsubst %.c, %.o, $(wildcard $(ANYDIR)/*.c))

all: kernel.elf

# to remove all object files: rm -rf $(ANYDIR)/*.o
kernel.elf: $(OBJS)
	ld $(LDFLAGS) $(OBJS) -o $(TOPDIR)/iso/boot/kernel.elf

# may need to specify: --xorriso=xorriso
os.iso: kernel.elf
	grub-mkrescue -o $(TOPDIR)/os.iso $(TOPDIR)/iso 

run: os.iso
	bochs -f $(TOPDIR)/bochsrc.txt -q

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(ANYDIR)/*.o $(TOPDIR)/iso/boot/kernel.elf $(TOPDIR)/os.iso
