#include "framebuffer.h"

void kmain() {
	#ifdef RUN_TEST
	extern void runtime_test();
	runtime_test();
	#else
	// this will contain code to setup segmentation, interrupts, etc.
	#endif
}
