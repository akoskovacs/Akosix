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
 *************************************************************************/
#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include <types.h>
#include <basic.h>

struct gdt_entry {
    uint16_t low_limit;
    uint16_t low_base;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  high_based;
} __packed;

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __packed;

struct gdt_entry gdt[3];
struct gdt_ptr;

#endif // DESCRIPTORS_H
