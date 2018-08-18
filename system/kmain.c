#include "framebuffer.h"

void kmain() {
    // just for debug
    // kmain() should use fb_write() instead
    fb_write_cell(0, 'T', FB_WHITE, FB_LIGHT_GREEN);
    fb_write_cell(2, 'E', FB_BLACK, FB_CYAN);
    fb_write_cell(4, 'S', FB_DARK_GREY, FB_LIGHT_RED);
    fb_write_cell(6, 'T', FB_RED, FB_LIGHT_GREEN);
}
