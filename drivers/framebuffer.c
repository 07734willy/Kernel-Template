#include "io.h"
#include "framebuffer.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

#define FB_ADDRESS 0xB8000
#define FB_COLUMNS 80
#define FB_ROWS    25

static fb_color default_color = FB_WHITE; // background: black, blink: 0
static char * const fb = (char *)FB_ADDRESS;
static unsigned short cursor_pos = 0;

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
    cursor_pos = pos;
}

/** fb_write_cell:
 *  Writes a character with the given color schema to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param color The color schema, generated with fb_make_color()
 */
void fb_write_cell_color(unsigned int i, char c, fb_color color)
{
    fb[2 * i] = c;
    fb[2 * i + 1] = color;
}

/** fb_write_cell:
 *  Writes a character with the default color schema to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 */
void fb_write_cell(unsigned int i, char c)
{
    fb_write_cell_color(i, c, default_color);
}

/** fb_write_char:
 *  Writes a single character to the framebuffer. It will
 *  move the cursor and scroll if necessary. Handles the
 *  newline '\n' character.
 * 
 * @param c The character
 */
void fb_write_char(char c)
{
    // Newline character
    if (c == '\n') {
        unsigned short cursor_row = cursor_pos / FB_COLUMNS;
        if (cursor_row < FB_ROWS - 1) {
            fb_move_cursor((cursor_row + 1) * FB_COLUMNS);
        }
        else {
            fb_move_cursor((FB_ROWS - 1) * FB_COLUMNS);
            fb_scroll();
        }
    }

    // TODO: TAB character?

    // Normal character
    else {
        fb_write_cell(cursor_pos, c);
        if (cursor_pos == FB_ROWS * FB_COLUMNS - 1) {
            fb_move_cursor((FB_ROWS - 1) * FB_COLUMNS);
            fb_scroll();
        }
        else {
            fb_move_cursor(cursor_pos + 1);
        }
    }
}

/** fb_scroll:
 *  Scrolls the framebuffer down one row. Does not move the
 *  cursor. Also clears the last row.
 */
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

/** fb_write_color
 * Writes a null-terminated string in the framebuffer using
 * the specified color schema.
 * It will automatically advance the cursor and
 * scroll the screen if necessary.
 * 
 * @param s The null-terminated string
 * @param color The color schema generated with fb_make_color()
 */
void fb_write_color(const char* s, fb_color color)
{
    fb_color bkp = default_color;
    default_color = color;
    fb_write(s);
    default_color = bkp;
}

/** fb_make_color
 * Generates a color schema
 * 
 * @param background The background color
 * @param foreground The foreground color
 * @param blink Value of the blink bit
 */
fb_color fb_make_color(int background, int foreground, int blink)
{
    return (foreground & 0xF) | ((background & 0x7) << 0x4) | (!!blink << 0x7);
}

/** fb_set_background_color
 * Sets the default background
 * 
 * @param color The background color
 */
void fb_set_background_color(int color)
{
    default_color = (default_color & 0xF) | ((color & 0x7) << 0x4) | (default_color & 0x80);
}

/** fb_set_foreground_color
 * Sets the default foreground
 * 
 * @param color The foreground color
 */
void fb_set_foreground_color(int color)
{
    default_color = (color & 0xFF) | (default_color & 0xF0);
}

/** fb_set_blink:
 * Sets the default blink bit
 * 
 * @param color The value of the blink bit
 */
void fb_set_blink(int blink)
{
    default_color = (blink << 0x7) | (default_color & 0x7F);
}

/** fb_write
 * Writes a null-terminated string in the framebuffer. It will
 * automatically advance the cursor and scroll the
 * screen if necessary. It uses the default color schema.
 * 
 * @param s The null-terminated string
 */
void fb_write(const char* s)
{
    for (const char* p = s; *p != '\0'; p++) {
        fb_write_char(*p);
    }
}

/** fb_clear
 *  Clears the framebuffer and moves the cursor to (0,0).
 */
void fb_clear()
{
    for (int i = 0; i < FB_COLUMNS * FB_ROWS; i++) {
        fb_write_cell_color(i, ' ', 0);
    }
    fb_move_cursor(0);
}