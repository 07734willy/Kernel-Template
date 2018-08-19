#include "framebuffer.h"

void kmain() {
    fb_write_cell(80, 'T');
    fb_set_background_color(FB_LIGHT_BLUE);
    fb_write_cell(81, 'T');
    fb_set_foreground_color(FB_BROWN);
    fb_write_cell(82, 'T');
    fb_set_blink(FB_BLINK);
    fb_write_cell(83, 'T');
}
