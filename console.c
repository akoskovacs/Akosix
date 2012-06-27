/************************************************************************
 *   Copyright (c) 2012 Ákos Kovács - Akosix operating system
 *              http://akoskovacs.github.com/Akosix
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ************************************************************************/
/* General console driver */

#include <basic.h>
#include <console.h>
#include <string.h>
#include <system.h>
#include <types.h>
#include <config.h>

#define CONFIG_CONSOLE_WIDTH  80
#define CONFIG_CONSOLE_HEIGHT 25
#define CONSOLE_LAST_COLUMN    CONFIG_CONSOLE_WIDTH - 1
#define CONSOLE_LAST_ROW       CONFIG_CONSOLE_HEIGHT - 1
#define TABSIZE        4

// Standard color screen in higher-half
#define VIDEORAM (console_font_t *)VADDR(0xb8000)
static void print_spaces(int, int, int);
static console_font_t *video_mem = VIDEORAM;
static console_attr_t console_attributes = 0;
static int pos_x = 0;
static int pos_y = 0;

void set_console_attributes(console_attr_t attr)
{
    console_attributes = attr;
}

console_attr_t get_console_attributes(void)
{
    return console_attributes;
}

void console_init()
{
    if (console_attributes == 0)
        console_attributes = (FG_COLOR_BLUE | LIGHT | BG_COLOR_WHITE);
    clear_console();
}

void clear_console()
{
    print_spaces(0, 0, CONFIG_CONSOLE_WIDTH * CONFIG_CONSOLE_HEIGHT);
}

void kpos_putchar(int x, int y, char ch)
{
    console_font_t font = 0;

    if (x > CONSOLE_LAST_COLUMN || y > CONSOLE_LAST_ROW)
        return;
    
    switch (ch) {
        case '\n': case '\r':
        return;
        case '\t':
            print_spaces(x, y, TABSIZE);
        return;
    }

    font |= ch;
    font |= (console_attributes << 8);
    video_mem[y * CONFIG_CONSOLE_WIDTH + x] = font; // Copy to screen
}

void kputchar(char ch)
{
    int i;
    int pos;
    switch (ch) {
        case '\n':
            pos = CONFIG_CONSOLE_WIDTH - pos_x + 1;
            for (i = 0 ; i < pos; i++) {
                kputchar(' ');
            }
        return;

        case '\t':
            for (i = 0 ; i < TABSIZE; i++) {
                kputchar(' ');
            }
        return;
    }
    

    kpos_putchar(pos_x, pos_y, ch);
    if (pos_x > CONSOLE_LAST_COLUMN) { // At the end of the line
        pos_x = 0;
        pos_y++;
        if (pos_y > CONSOLE_LAST_ROW) // At the end of the screen
            scroll_up_console(1);

    } else {
        pos_x++;
    }
}

size_t kpos_print(int x, int y, const char *line)
{
    size_t size = strlen(line);
    unsigned int i;              // shut up GCC
    for (i = 0; i < size; i++) {
        kpos_putchar(x+i, y, line[i]);
    }
    return size;
}

size_t kprint(const char *line) /* No formatting */
{
    size_t size = strlen(line);
    unsigned int i;              // shut up GCC
    for (i = 0; i < size; i++) {
        kputchar(line[i]);
    }
    return size;
}

void scroll_up_console(int count)
{
    int i;
    while (count--) {
        for (i = 0; i < CONFIG_CONSOLE_HEIGHT * CONFIG_CONSOLE_WIDTH; i++) {
            video_mem[i] = video_mem[CONFIG_CONSOLE_WIDTH + i];
        }
    }
    pos_y = CONSOLE_LAST_ROW;
    pos_x = 0; 
    print_spaces(pos_x, pos_y, CONFIG_CONSOLE_WIDTH);
}

void scroll_down_console(int count)
{
    int i;
    while (count--) {
        for (i = CONFIG_CONSOLE_HEIGHT * CONFIG_CONSOLE_WIDTH; i > 0; i++) {
            video_mem[CONFIG_CONSOLE_WIDTH - i] = video_mem[i];
        }
    }
    pos_y = 0;
    pos_x = 0; 
    print_spaces(pos_x, pos_y, CONFIG_CONSOLE_WIDTH);
}

void print_spaces(int x, int y, int count)
{
    while (count) {
        if (x > CONSOLE_LAST_COLUMN) {
            x = 0;
            y++;
        }
        kpos_putchar(x, y, ' ');
        x++;
        count--;
    }
}

void set_xy(int x, int y)
{
	pos_x = x;
	pos_y = y;
}

int get_x(void) { return pos_x; }
int get_y(void) { return pos_y; }

void move_console_cursor(int x, int y)
{
    unsigned temp;

    temp = y * CONFIG_CONSOLE_WIDTH + x;

    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}
