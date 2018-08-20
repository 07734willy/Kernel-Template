#include <framebuffer.h>

void runtime_test() {
	fb_write_cell(0, 'R', FB_WHITE, FB_BLUE, FB_NOBLINK);
}
