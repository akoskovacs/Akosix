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
#ifndef CONSOLE_H
#define CONSOLE_H

#include <basic.h>
#include <types.h>
#include <config.h>

#define CONFIG_CONSOLE_WIDTH  80
#define CONFIG_CONSOLE_HEIGHT 25
#define CONSOLE_LAST_COLUMN    CONFIG_CONSOLE_WIDTH - 1
#define CONSOLE_LAST_ROW       CONFIG_CONSOLE_HEIGHT - 1

typedef enum { 
    LIGHT            = 0x08,
    BLINK            = 0x80,
    BG_COLOR_RED     = 0x40,
    BG_COLOR_GREEN   = 0x20,
    BG_COLOR_BLUE    = 0x10,
    BG_COLOR_CYAN    = BG_COLOR_GREEN | BG_COLOR_BLUE,
    BG_COLOR_MAGENTA = BG_COLOR_RED | BG_COLOR_BLUE,
    BG_COLOR_BROWN   = BG_COLOR_RED | BG_COLOR_GREEN,
    BG_COLOR_WHITE   = BG_COLOR_RED | BG_COLOR_GREEN | BG_COLOR_BLUE,
    BG_COLOR_BLACK   = 0x00,
    FG_COLOR_RED     = 0x04,
    FG_COLOR_GREEN   = 0x02,
    FG_COLOR_BLUE    = 0x01,
    FG_COLOR_CYAN    = FG_COLOR_GREEN | FG_COLOR_BLUE,
    FG_COLOR_MAGENTA = FG_COLOR_RED | FG_COLOR_BLUE,
    FG_COLOR_BROWN   = FG_COLOR_RED | FG_COLOR_GREEN,
    FG_COLOR_WHITE   = FG_COLOR_RED | FG_COLOR_GREEN | FG_COLOR_BLUE,
    FG_COLOR_BLACK   = 0x00,
} console_attr_t;

void console_init();
void clear_console();
void set_console_attributes(console_attr_t);
console_attr_t get_console_attributes(void);

void kxya_putchar(int, int, console_attr_t, char);
void kxy_putchar(int, int, char);
void ka_putchar(console_attr_t, char);
void kputchar(char);

size_t kxya_print(int, int, console_attr_t, const char *);
size_t kxy_print(int, int, const char *);
size_t ka_print(console_attr_t, const char *);
size_t kprint(const char *);

void scroll_down_console(int);
void scroll_up_console(int);
void set_xy(int, int);
int get_x(void);
int get_y(void);
void disable_cursor(void);

size_t kprint(const char *);
void move_cursor(int, int);
void kputchar(char);
typedef uint16_t console_font_t;

#endif // CONSOLE_H
