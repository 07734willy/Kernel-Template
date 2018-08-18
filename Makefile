TOPDIR := .
ANYDIR := $(TOPDIR)/**
INCDIR := $(TOPDIR)/include
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
		 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I$(INCDIR) -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

COBJS := $(patsubst %.c, %.o, $(wildcard $(ANYDIR)/*.c))
SOBJS := $(patsubst %.S, %.o, $(wildcard $(ANYDIR)/*.S))
all: kernel.elf

# to remove all object files: rm -rf $(ANYDIR)/*.o
kernel.elf: $(SOBJS) $(COBJS)
	ld $(LDFLAGS) $(join $(dir $(SOBJS)), $(addprefix object/, $(notdir $(SOBJS)))) $(join $(dir $(COBJS)), $(addprefix object/, $(notdir $(COBJS)))) -o $(TOPDIR)/iso/boot/kernel.elf

# may need to specify: --xorriso=xorriso
os.iso: kernel.elf
	grub-mkrescue -o $(TOPDIR)/os.iso $(TOPDIR)/iso 

run: os.iso
	bochs -f $(TOPDIR)/bochsrc.txt -q

$(COBJS): %.o: %.c
	mkdir -p $(*D)/object
	$(CC) $(CFLAGS)  $< -o $(*D)/object/$(*F).o

$(SOBJS): %.o: %.S
	mkdir -p $(*D)/object
	$(AS) $(ASFLAGS) $< -o $(*D)/object/$(*F).o

clean:
	rm -rf $(ANYDIR)/*.o $(TOPDIR)/iso/boot/kernel.elf $(TOPDIR)/os.iso
