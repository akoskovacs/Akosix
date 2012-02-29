/* General console driver */

#include <basic.h>
#include <console.h>
#include <string.h>
#include <system.h>
#include <types.h>

#define CONSOLE_WIDTH  80
#define CONSOLE_HEIGHT 25
#define VIDEORAM (struct console_font *)0xb8000 // Standard color screen
#define CURRENT_POS_ADDR (VIDEORAM + (pos_y * CONSOLE_WIDTH + pos_x -1))

static struct console_font *video_mem = (struct console_font *)VIDEORAM;
static console_color_t console_forecolor = COLOR_WHITE;
static console_color_t console_backcolor = COLOR_BLACK;
static int pos_x = 0, pos_y = 0;

static void print_spaces(int times);
static struct console_font *next_console_char_addr();

void init_console()
{
    clear_console();
    kprint("Initializing console...\n");
}

void set_forecolor(console_color_t color)
{
    console_forecolor = color;
}

void set_backcolor(console_color_t color)
{
    console_backcolor = color;
}

void print_spaces(int times)
{
    int i;
    for (i = 0; i < times; i++) {
        kputchar(' ');
    }
}

void clear_console()
{
    print_spaces(CONSOLE_WIDTH * CONSOLE_HEIGHT);
    pos_x = 0;
    pos_y = 0;
}

void kputchar(char ch)
{
    struct console_font *addr = next_console_char_addr();
    switch (ch) {
       case '\r':
       break;

       case '\n':
            print_spaces(CONSOLE_WIDTH - pos_x);
       break;

       case '\t':
            print_spaces(4);
       break;

       default:
            addr->cf_char = ch;
       break;
    }
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

/* Every function which somehow modify the videomem should call this function, 
 * becase we must be able to count the actual X and Y coordinates to scroll
 * if necessary.
 *
 * RET: It gives back the next address of the 'font'.
*/
struct console_font *next_console_char_addr()
{
    struct console_font *addr;
    if (pos_x >= (CONSOLE_WIDTH-1)) { // At the end of the line
        pos_x = 0;
        pos_y++;
        if (pos_y + 1 >= CONSOLE_HEIGHT) // At the end of the screen
            scroll_console();

    } else {
        pos_x++;
    }
    addr = CURRENT_POS_ADDR;
    addr->cf_forecolor = console_forecolor;
    addr->cf_backcolor = console_backcolor;
    return addr;
}

void scroll_console()
{
    int i;
    for (i = 0; i < CONSOLE_HEIGHT * CONSOLE_WIDTH; i++) {
        video_mem[i] = video_mem[CONSOLE_WIDTH + i];
    }
    pos_y = CONSOLE_HEIGHT - 1;
    print_spaces(CONSOLE_WIDTH-1);
    pos_x = 0; 
}


void move_cursor(int x, int y)
{
    unsigned temp;

    temp = y * CONSOLE_WIDTH + x;

    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}
