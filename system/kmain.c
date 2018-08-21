#include "framebuffer.h"
#include <segmentation.h>

void kmain() {
	#ifdef RUN_TEST
	extern void runtime_test();
	runtime_test();
	#else
	// this will contain code to setup segmentation, interrupts, etc.
    init_GDT();
	// this is just here to quickly see if segmentation fails
	fb_write_char('?');
	#endif
}
