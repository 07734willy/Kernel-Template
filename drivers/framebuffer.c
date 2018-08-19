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
static char * const fb = (char *)FB_ADDRESS;
static unsigned short cursor_pos = 0;

void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
    cursor_pos = pos;
}

void fb_write_cell_color(unsigned int i, char c, fb_color color)
{
    fb[2 * i] = c;
    fb[2 * i + 1] = color;
}

void fb_write_cell(unsigned int i, char c)
{
    fb_write_cell_color(i, c, default_color);
}

void fb_write_color(const char* s, fb_color color)
{
    fb_color bkp = default_color;
    default_color = color;
    fb_write(s);
    default_color = bkp;
}

void fb_write(const char* s)
{
    for (const char* p = s; *p != '\0'; p++) {
        fb_write_char(*p);
    }
}

void fb_write_char(char c)
{
    fb_write_cell(cursor_pos, c);
    if (cursor_pos == FB_ROWS * FB_COLUMNS - 1) {
        fb_move_cursor((FB_ROWS - 1) * FB_COLUMNS);
        fb_scroll();
    }
    else {
        fb_move_cursor(cursor_pos + 1);
    }
}

void fb_scroll()
{
    for (int i = 1; i < FB_ROWS; i++) {
        for (int j = 0; j < FB_COLUMNS * 2; j++) {
            fb[(i-1)*FB_COLUMNS*2 + j] = fb[i*FB_COLUMNS*2 + j];
        }
    }

    // Clear last line
    for (int i = 0; i < FB_COLUMNS; i++) {
        fb_write_cell_color((FB_ROWS - 1)*FB_COLUMNS + i, ' ', 0);
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