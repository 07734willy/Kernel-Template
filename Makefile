TOPDIR := .
INCDIR := $(TOPDIR)/include
TESTDIR := $(TOPDIR)/runtime
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
		 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I$(INCDIR) -c

CTESTFLAGS = -m32 -Wall -Wextra -Werror -I$(INCDIR)

LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf


TESTS := $(patsubst %.c, %, $(shell find . -wholename "*/unit/*.c"))
RUNTESTS := $(patsubst %.c, %, $(shell find $(TESTDIR) -name "*.c"))

CSRC := $(patsubst %.c, %.o, $(filter-out $(addsuffix .c, $(TESTS) $(RUNTESTS)), $(shell find . -name "*.c")))
SSRC := $(patsubst %.S, %.o, $(shell find . -name "*.S"))

COBJS := $(join $(dir $(CSRC)), $(addprefix object/, $(notdir $(CSRC))))
SOBJS := $(join $(dir $(SSRC)), $(addprefix object/, $(notdir $(SSRC))))

CTEST := $(filter $(TESTS), $(patsubst %.o, %_test, $(join $(dir $(CSRC)), $(addprefix unit/, $(notdir $(CSRC))))))
STEST := $(filter $(TESTS), $(patsubst %.o, %_test, $(join $(dir $(SSRC)), $(addprefix unit/, $(notdir $(SSRC))))))

all: kernel.elf

# to remove all object files: rm -rf $(ANYDIR)/*.o
ifdef SOURCE
DOTEST := RUN_TEST
kernel.elf: $(SOBJS) $(COBJS)
	@echo $(RUNTESTS)
	@$(CC) $(CFLAGS) runtime/$(SOURCE) -o $(TESTDIR)/runtime_test.o
	@ld $(LDFLAGS) -o $(TOPDIR)/iso/boot/kernel.elf $^ $(TESTDIR)/runtime_test.o
	@rm $(TESTDIR)/runtime_test.o
else
DOTEST := NO_RUN_TEST
kernel.elf: $(SOBJS) $(COBJS)
	@ld $(LDFLAGS) -o $(TOPDIR)/iso/boot/kernel.elf $^
endif

# may need to specify: --xorriso=xorriso
os.iso: kernel.elf
	grub-mkrescue -d /usr/lib/grub/i386-pc -o $(TOPDIR)/os.iso $(TOPDIR)/iso 

run: os.iso
	bochs -f $(TOPDIR)/bochsrc.txt -q

$(CSRC): %.o: %.c
	@mkdir -p $(*D)/object
	@$(CC) $(CFLAGS) -D $(DOTEST) $< -o $(*D)/object/$(*F).o

$(SSRC): %.o: %.S
	@mkdir -p $(*D)/object
	@$(AS) $(ASFLAGS) $< -o $(*D)/object/$(*F).o

$(COBJS): $(CSRC)

$(SOBJS): $(SSRC)

# && echo [Pass] $(lastword $^) || echo [FAIL] $(lastword $^)
$(CTEST): %: $(COBJS) $(SOBJS) %.c
	@$(CC) $(CTESTFLAGS) -c $(lastword $^) -o tmp.o
	@$(CC) $(CTESTFLAGS) -o tmp $(COBJS) $(SOBJS) tmp.o
	@rm tmp.o
	@./tmp && echo [COMPLETED] $(lastword $^) || echo [CRASHED] $(lastword $^)
	@rm tmp

$(STEST): %: $(COBJS) $(SOBJS) %.S
	@$(AS) $(ASFLAGS) $(lastword $^) -o tmp.o
	@$(CC) $(CTESTFLAGS) -o tmp $(COBJS) $(SOBJS) tmp.o
	@rm tmp.o
	@./tmp && echo [COMPLETED] $(lastword $^) || echo [CRASHED] $(lastword $^)
	@rm tmp

test: $(CTEST) $(STEST)

clean:
	@rm -rf $(shell find . -name "*.o") $(TOPDIR)/iso/boot/kernel.elf $(TOPDIR)/os.iso
