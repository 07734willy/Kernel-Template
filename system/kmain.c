#include "framebuffer.h"

void kmain() {
	#ifdef RUN_TEST
	extern void runtime_test();
	runtime_test();
	#else
    // just for debug
    // kmain() should use fb_write() instead
    fb_write_cell(0, 'T', FB_WHITE, FB_LIGHT_GREEN, FB_NOBLINK);
    fb_write_cell(1, 'E', FB_LIGHT_BLUE, FB_CYAN, FB_NOBLINK);
    fb_write_cell(2, 'S', FB_DARK_GREY, FB_LIGHT_RED, FB_NOBLINK);
    fb_write_cell(3, 'T', FB_RED, FB_LIGHT_GREEN, FB_BLINK);
	#endif
}
