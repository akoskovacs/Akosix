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
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <basic.h>

enum KEYBOARD_KEYS {
      KEY_INVALID, KEY_ESC
    , KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6
    , KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12
    , KEY_HOME, KEY_END, KEY_INSERT, KEY_DEL
    , KEY_CAPS_LOCK, KEY_LSHIFT, KEY_RSHIFT, KEY_LCTRL, KEY_RCTRL
    , KEY_ALT, KEY_LALT = KEY_ALT, KEY_ALT_GR, KEY_RALT = KEY_ALT_GR
    , KEY_PRINT_SCREEN, KEY_PG_UP, KEY_PG_DOWN
    , KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
    , KEY_FN, KEY_SUPER
#if 0
    , KEY_0 = '0', KEY_1 = '1', KEY_2 = '2', KEY_3 = '3', KEY_4 = '4'
    , KEY_5 = '5', KEY_6 = '6', KEY_7 = '7', KEY_8 = '8', KEY_9 = '9'
#endif
};


void keyboard_set_keymap(const char *);
uint8_t keyboard_get_scancode(void);
uint8_t keyboard_scancode_for(enum KEYBOARD_KEYS spec_key);
char keyboard_read(uint8_t *scancode);
void ps2_kbd_reboot(void);

#endif // KEYBOARD_H
