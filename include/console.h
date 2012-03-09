#ifndef CONSOLE_H
#define CONSOLE_H

#include <basic.h>
#include <types.h>

typedef enum { COLOR_BLACK, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
        COLOR_RED, COLOR_MAGENTA, COLOR_BROWN, COLOR_LIGHT_GREY,
        COLOR_DARK_GREY, COLOR_LIGHT_BLUE, COLOR_LIGHT_GREEN,
        COLOR_LIGHT_CYAN, COLOR_LIGHT_RED, COLOR_LIGHT_MAGENTA,
        COLOR_LIGHT_BROWN, COLOR_WHITE 
} console_color_t;

void init_console();
void clear_console();
void kpos_putchar(char, int, int);
size_t kpos_print(const char *, int, int);
void set_forecolor(console_color_t);
void set_backcolor(console_color_t);
void scroll_down_console(int);
void scroll_up_console(int);

size_t kprint(const char *);
void move_cursor(int, int);
void kputchar(char);

struct console_font {
    char cf_char                 : 8;
    console_color_t cf_forecolor : 4;
    console_color_t cf_backcolor : 4;
} __packed; 

#endif // CONSOLE_H
