#include "framebuffer.h"

void kmain() {
    for (int j = 0; j < 25; j++)
        for (int i = 0; i < 80; i++) {
            fb_set_foreground_color(j);
            fb_set_background_color(9 - j);
            fb_set_blink(i+j % 2 == 0);
            fb_write_char('0' + (char)(j % 10));
        }


    fb_write("Hello LittleOS! This is a test.");
}
