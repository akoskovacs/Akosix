/************************************************************************
 *   Copyright (c) 2016 Ákos Kovács - Akosix operating system
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

#include <string.h>
#include <system.h>
#include <devices/input/keyboard.h>

#define KBD_CMD_REBOOT 0xFE
#define KBD_PORT 0x60

/* TODO... */
#if KBD_HAVE_UNICODE
const char kbd_hu_keymap[] = "\0\000123456789öüó\b\t"
            /* 0x10 */ "qwertzuiopőú\n\0as"
            /* 0x20 */ "dfghjkléá0\0űyxcv"
            /* 0x30 */ "bnm,.-\0\0\0 ";
            /* TODO: í */
#else
const char kbd_hu_keymap[] = "\0\000123456789ouo\b\t"
            /* 0x10 */ "qwertzuiopou\n\0as"
            /* 0x20 */ "dfghjklea0\0uyxcv"
            /* 0x30 */ "bnm,.-\0\0\0 ";

#endif // KBD_HAVE_UNICODE
#define KBD_HU_KEYMAP_SIZE sizeof(kbd_hu_keymap)/sizeof(char)
/* XXX: Must be in the order with enum KEYBOARD_KEYS */
const uint8_t kbd_hu_spec_keys[] = {
    /* invalid key      */ 0x00,
    /* KEY_ESC          */ 0x01,
    /* KEY_F1           */ 0x3B,
    /* KEY_F2           */ 0x3C,
    /* KEY_F3           */ 0x3D,
    /* KEY_F4           */ 0x3E,
    /* KEY_F5           */ 0x3F,
    /* KEY_F6           */ 0x40,
    /* KEY_F7           */ 0x41,
    /* KEY_F8           */ 0x42,
    /* KEY_F9           */ 0x43,
    /* KEY_F10          */ 0x44,
    /* KEY_F11          */ 0x57, /* weird */
    /* KEY_F12          */ 0x58,
    /* KEY_HOME         */ 0x47,
    /* KEY_END          */ 0x4F,
    /* KEY_INSERT       */ 0x52,
    /* KEY_DEL          */ 0x53,
    /* KEY_CAPS_LOCK    */ 0x3A,
    /* KEY_LSHIFT       */ 0x2A,
    /* KEY_RSHIFT       */ 0x36,
    /* KEY_LCTRL        */ 0x1D,
    /* KEY_RCTRL        */ 0x1D,
    /* KEY_ALT          */ 0x38,
    /* KEY_ALT_GR       */ 0x38,
    /* KEY_PRINT_SCREEN */ 0x37,
    /* KEY_PG_UP        */ 0x49,
    /* KEY_PG_DOWN      */ 0x51,
    /* KEY_UP           */ 0x48,
    /* KEY_DOWN         */ 0x50,
    /* KEY_LEFT         */ 0x4B,
    /* KEY_RIGHT        */ 0x4D,
    /* KEY_FN           */ 0x00,
    /* KEY_SUPER        */ 0x5B,
};
#define SPEC_KEY_FIRST 0
#define SPEC_KEY_LAST (sizeof(kbd_hu_spec_keys)/sizeof(uint8_t)-1)

static const char *kbd_keymap = kbd_hu_keymap;
static const uint8_t *kbd_spec_keys = kbd_hu_spec_keys;
static size_t kbd_keymap_size = KBD_HU_KEYMAP_SIZE;
static bool kbd_is_shift_on = false;
static bool kbd_is_caps_lock_on = false;

void keyboard_set_keymap(const char *map_name)
{
    if (strcmp(map_name, "HU") == 0) {
        kbd_keymap      = kbd_hu_keymap;
        kbd_spec_keys   = kbd_hu_spec_keys;
        kbd_keymap_size = KBD_HU_KEYMAP_SIZE;
    }
}

int8_t keyboard_scancode_for(enum KEYBOARD_KEYS k)
{
    if (k >= SPEC_KEY_FIRST && k <= SPEC_KEY_LAST) {
        return kbd_spec_keys[k];
    }
    return KEY_INVALID;
}

uint8_t keyboard_get_scancode(void)
{
    uint8_t c = 0;
    do {
        if (inb(KBD_PORT) != c) {
            c = inb(KBD_PORT);
            if (c > 0) {
                return c;
            }
        }
    } while (1);
}

char keyboard_read(uint8_t *scancode)
{
    uint8_t sc = keyboard_get_scancode();
    if (scancode != NULL) {
        *scancode = sc;
    }
    if (sc < kbd_keymap_size) {
        return kbd_keymap[sc];
    }
    return '\0';
}

/* On i386 the reset is handled by the keyboard controller */
void ps2_kbd_reboot(void)
{
    char good = 0x02;
    while (good & 0x02) {
        good = inb(0x64);
        inb(KBD_PORT);
    }
    outb(0x64, KBD_CMD_REBOOT);

    /* Halt anyway */
    forever {
        halt();
    }
}
