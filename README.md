To build the kernel, use the make command in the root directory. To run the OS in an emulator, use "make run". This will require xorriso to be installed on your system.

You must define the environment variable DISPLAY_LIB to the supported display library in order to run the emulator (to either sdl or sdl2 most likely).

You will need to install grub-pc-bin if /usr/lib/grub/i386-pc does not exist on your machine.

In order to run the unit tests on a 64-bit system, you'll need a cross compiler to produce a 32-bit ELF. Installing gcc-multilib should cover this.

To execute runtime tests, use the format

	make run SOURCE=test_name.c

Which will run the test associated with test_name.c.

On non-windows systems, you will need to type 'c' or "continue" to begin emulation in bochs.
