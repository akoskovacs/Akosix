/* General console driver */

#include <basic.h>
#include <console.h>
#include <string.h>
#include <system.h>
#include <types.h>

#define CONSOLE_WIDTH  80
#define CONSOLE_HEIGHT 25

// Standard color screen in higher-half
#define VIDEORAM (struct console_font *)VADDR(0xb8000)

static struct console_font *video_mem = (struct console_font *)VIDEORAM;
static console_color_t console_forecolor = COLOR_WHITE;
static console_color_t console_backcolor = COLOR_BLACK;
static int pos_x = 0, pos_y = 0;
static void print_spaces(int, int, int);

void set_forecolor(console_color_t color) { console_forecolor = color; }
void set_backcolor(console_color_t color) { console_backcolor = color; }

void init_console()
{
    clear_console();
}

void clear_console()
{
    print_spaces(0, 0, CONSOLE_HEIGHT*CONSOLE_WIDTH);
}

void kpos_putchar(char ch, int x, int y)
{
    struct console_font font;

    if (x >= CONSOLE_WIDTH || y >= CONSOLE_HEIGHT)
        return;

    font.cf_char = ch;
    font.cf_forecolor = console_forecolor;
    font.cf_backcolor = console_backcolor;

    video_mem[y * CONSOLE_WIDTH + x] = font; // Copy to screen
}

void kputchar(char ch)
{
    switch (ch) {
        case '\n':
            print_spaces(pos_y, pos_x, CONSOLE_WIDTH - pos_x);
            pos_y++;
            pos_x = 0;
        return;

        case '\t':
            print_spaces(pos_y, pos_x, 4);
            pos_x += 4;
        return;
    }

    kpos_putchar(ch, pos_x, pos_y);
    if (pos_x+1 >= CONSOLE_WIDTH) { // At the end of the line
        pos_x = 0;
        pos_y++;
        if (pos_y+1 >= CONSOLE_HEIGHT) // At the end of the screen
            scroll_up_console(1);

    } else {
        pos_x++;
    }
}

size_t kpos_print(const char *line, int x, int y)
{
    size_t size = strlen(line);
    unsigned int i;              // shut up GCC
    for (i = 0; i < size; i++) {
        kpos_putchar(line[i], x+i, y);
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
        for (i = 0; i < (CONSOLE_HEIGHT-1) * CONSOLE_WIDTH; i++) {
            video_mem[i] = video_mem[CONSOLE_WIDTH + i];
        }
    }
    pos_y = CONSOLE_HEIGHT - 1;
    pos_x = 0; 
    print_spaces(pos_x, pos_y, CONSOLE_WIDTH-1);
}

void scroll_down_console(int count)
{
    int i;
    while (count--) {
        for (i = (CONSOLE_HEIGHT-1) * CONSOLE_WIDTH; i > 0; i++) {
            video_mem[CONSOLE_WIDTH - i] = video_mem[i];
        }
    }
    pos_y = 0;
    pos_x = 0; 
    print_spaces(pos_x, pos_y, CONSOLE_WIDTH-1);
}

void print_spaces(int x, int y, int count)
{
    while (count--) {
        if (x >= CONSOLE_WIDTH) {
            x = 0;
            y++;
        }
        kpos_putchar(' ', x, y);
        x++;
    }
}

void move_console_cursor(int x, int y)
{
    unsigned temp;

    temp = y * CONSOLE_WIDTH + x;

    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}
