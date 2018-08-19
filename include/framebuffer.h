#ifndef INCLUDE_FRAMEBUFFER_H
#define INCLUDE_FRAMEBUFFER_H

// Framebuffer colors
#define FB_BLACK         0
#define FB_BLUE          1
#define FB_GREEN         2
#define FB_CYAN          3
#define FB_RED           4
#define FB_MAGENTA       5
#define FB_BROWN         6
#define FB_LIGHT_GREY    7
#define FB_DARK_GREY     8
#define FB_LIGHT_BLUE    9
#define FB_LIGHT_GREEN   10
#define FB_LIGHT_CYAN    11
#define FB_LIGHT_RED     12
#define FB_LIGHT_MAGENTA 13
#define FB_LIGHT_BROWN   14
#define FB_WHITE         15

#define FB_NOBLINK       0
#define FB_BLINK         1

typedef unsigned char fb_color;

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos);

/** fb_write_cell:
 *  Writes a character with the given color schema to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param color The color schema, generated with fb_make_color()
 */
void fb_write_cell_color(unsigned int i, char c, fb_color color);

/** fb_write_cell:
 *  Writes a character with the default color schema to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 */
void fb_write_cell(unsigned int i, char c);

/** fb_write_color
 * Writes a null-terminated string in the framebuffer using
 * the specified color schema.
 * It will automatically advance the cursor and
 * scroll the screen if necessary.
 * 
 * @param s The null-terminated string
 * @param color The color schema generated with fb_make_color()
 */
void fb_write_color(const char* s, fb_color color);

/** fb_make_color
 * Generates a color schema
 * 
 * @param background The background color
 * @param foreground The foreground color
 * @param blink Value of the blink bit
 */
fb_color fb_make_color(int background, int foreground, int blink);

/** fb_set_background_color
 * Sets the default background
 * 
 * @param color The background color
 */
void fb_set_background_color(int color);

/** fb_set_foreground_color
 * Sets the default foreground
 * 
 * @param color The foreground color
 */
void fb_set_foreground_color(int color);

/** fb_set_foreground_color
 * Sets the default blink bit
 * 
 * @param color The value of the blink bit
 */
void fb_set_blink(int blink);

/** fb_write
 * Writes a null-terminated string in the framebuffer. It will
 * automatically advance the cursor and scroll the
 * screen if necessary. Uses the default color schema.
 * 
 * @param s The null-terminated string
 */
void fb_write(const char* s);

#endif
