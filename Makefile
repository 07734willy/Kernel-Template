TOPDIR := .
INCDIR := $(TOPDIR)/include
RUNDIR := $(TOPDIR)/runtime
CC = gcc
# For use in compiling C source for the kernel.elf
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
		 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I$(INCDIR) -c
# For use in compiling C unit tests
CTESTFLAGS = -m32 -Wall -Wextra -Werror -I$(INCDIR)
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

# All unit tests- used to exclude them from the kernel.elf build
TESTS := $(patsubst %.c, %, $(shell find . -wholename "*/unit/*.c"))
# All runtime tests
RUNTESTS := $(patsubst %.c, %, $(shell find $(RUNDIR) -name "*.c"))

# The subset of unit tests to be built- can be refined with TEST and TESTDIR
DOTESTS := $(TESTS)
ifdef TESTDIR
DOTESTS := $(filter $(patsubst %.c, %, $(shell find . -wholename \
           "*"/$(TESTDIR)"/unit/*.c")), $(DOTESTS))
endif
ifdef TEST
DOTESTS := $(filter $(patsubst %.c, %, $(shell find . -wholename \
           "*/unit/"$(TEST)".c")), $(DOTESTS))
endif

# All non-test C source files (extension modified)
CSRC := $(patsubst %.c, %.o, $(filter-out $(addsuffix .c, $(TESTS) $(RUNTESTS)), $(shell find . -name "*.c")))
# All assembly source files (extension modified)
SSRC := $(patsubst %.S, %.o, $(shell find . -name "*.S"))

# All objects to be compiled from C source
COBJS := $(join $(dir $(CSRC)), $(addprefix object/, $(notdir $(CSRC))))
# ALl objects to be assembled from ASM source
SOBJS := $(join $(dir $(SSRC)), $(addprefix object/, $(notdir $(SSRC))))

# All unit tests that have a corresponding C source
CTEST := $(filter $(DOTESTS), $(patsubst %.o, %_test, $(join $(dir $(CSRC)),\
         $(addprefix unit/, $(notdir $(CSRC))))))
# All unit tests that have a corresponding ASM source
STEST := $(filter $(DOTESTS), $(patsubst %.o, %_test, $(join $(dir $(SSRC)),\
         $(addprefix unit/, $(notdir $(SSRC))))))

all: kernel.elf

# to remove all object files: rm -rf $(ANYDIR)/*.o
# If SOURCE is provided, run the associated runtime test
ifdef SOURCE
RUNTEST := RUN_TEST
kernel.elf: $(SOBJS) $(COBJS)
	@echo $(RUNTESTS)
	@$(CC) $(CFLAGS) runtime/$(SOURCE).c -o $(RUNDIR)/runtime_test.o
	@ld $(LDFLAGS) -o $(TOPDIR)/iso/boot/kernel.elf $^ $(RUNDIR)/runtime_test.o
	@rm $(RUNDIR)/runtime_test.o
else
RUNTEST := NO_RUN_TEST
# Link the objects normally to build kernel.elf
kernel.elf: $(SOBJS) $(COBJS)
	@ld $(LDFLAGS) -o $(TOPDIR)/iso/boot/kernel.elf $^
endif

# may need to specify: --xorriso=xorriso
# Produce an iso image from the kernel.elf, note /grub/i386-pc
os.iso: kernel.elf
	grub-mkrescue -d /usr/lib/grub/i386-pc -o $(TOPDIR)/os.iso $(TOPDIR)/iso 

# Run the emulator on the produced disk image, with boshsrc.txt config file
run: os.iso
	echo "continue" | bochs -f $(TOPDIR)/bochsrc.txt -q || true

# Run the emulator, but not skipping the debug prompt
debug: os.iso
	bochs -f $(TOPDIR)/bochsrc.txt -q || true

# Compile all non-test C sources
$(CSRC): %.o: %.c
	@mkdir -p $(*D)/object
	@$(CC) $(CFLAGS) -D $(RUNTEST) $< -o $(*D)/object/$(*F).o

# Assemble all ASM sources
$(SSRC): %.o: %.S
	@mkdir -p $(*D)/object
	@$(AS) $(ASFLAGS) $< -o $(*D)/object/$(*F).o

$(COBJS): $(CSRC)

$(SOBJS): $(SSRC)

# && echo [Pass] $(lastword $^) || echo [FAIL] $(lastword $^)
# Compile all C-source unit tests, and link to every object using the compiler
$(CTEST): %: $(COBJS) $(SOBJS) %.c
	@$(CC) $(CTESTFLAGS) -c $(lastword $^) -o tmp.o
	@$(CC) $(CTESTFLAGS) -o tmp $(COBJS) $(SOBJS) tmp.o
	@rm tmp.o
	@./tmp && echo [COMPLETED] $(lastword $^) || echo [CRASHED] $(lastword $^)
	@rm tmp

# Compile all ASM-source unit tests, and link every object using the compiler
$(STEST): %: $(COBJS) $(SOBJS) %.C
	@$(CC) $(CTESTFLAGS) -c $(lastword $^) -o tmp.o
	@$(CC) $(CTESTFLAGS) -o tmp $(COBJS) $(SOBJS) tmp.o
	@rm tmp.o
	@./tmp && echo [COMPLETED] $(lastword $^) || echo [CRASHED] $(lastword $^)
	@rm tmp

# Build all unit tests
test: $(CTEST) $(STEST)

# Remove all object files, and the disk image
clean:
	@rm -rf $(shell find $(TOPDIR) -name "*.o") $(TOPDIR)/iso/boot/kernel.elf \
    $(TOPDIR)/os.iso
