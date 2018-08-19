#include "io.h"
#include "framebuffer.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

#define FB_ADDRESS 0x000B8000
#define FB_COLUMNS 80
#define FB_ROWS    25

static fb_color default_color = FB_WHITE; // background: 0, blink: 0
static char * const framebuffer = (char *)FB_ADDRESS;

void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

void fb_write_cell_color(unsigned int i, char c, fb_color color)
{
    framebuffer[2 * i] = c;
    framebuffer[2 * i + 1] = color;
}

void fb_write_cell(unsigned int i, char c)
{
    framebuffer[2 * i] = c;
    framebuffer[2 * i + 1] = default_color;
}

void fb_write_color(const char* s, fb_color color)
{
    for (const char* c = s; *c != 0; c++)
    {
        // TODO
        color += 1; // -Werror
    }
}

fb_color fb_make_color(int background, int foreground, int blink)
{
    return (foreground & 15) + ((background & 7) << 4) + ((blink & 1) << 7);
}

void fb_set_background_color(int color)
{
    default_color = (default_color & 15) + ((color & 7) << 4) + (default_color & 128);
}

void fb_set_foreground_color(int color)
{
    default_color = (color & 15) + (default_color & 240);
}

void fb_set_blink(int blink)
{
    default_color = (blink << 7) + (default_color & 127);
}

void fb_write(const char* s)
{
    fb_write_color(s, default_color);
}