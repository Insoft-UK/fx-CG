/*
 Copyright © 2024 Insoft. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include "font.h"
#include "graphics.h"

#include <stdlib.h>

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)\
{\
int16_t t = a;\
a = b;\
b = t;\
}
#endif

static TfxFont *_font;

/**
 @brief   Draw a single character
 @param    x   Bottom left corner x coordinate
 @param    y   Bottom left corner y coordinate
 @param    c   The 8-bit font-indexed character (likely ascii)
 @param    color 16-bit 5-6-5 Color to draw chraracter with
 @param    bg 16-bit 5-6-5 Color to fill background with (if same as color, no background)
 @param    scale_x  Font scale in X-axis, 1 is 'original' size
 @param    scale_y  Font scale in Y-axis, 1 is 'original' size
 */
static void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t scale_x, uint8_t scale_y) {
    if (!_font) return;
    uint8_t *p = (uint8_t *)_font + sizeof(TfxFont);
    for (int i=0; i<_font->width; i++) {
        uint8_t line = p[c * (int)_font->width + i];
        for (int j=0; j<8; j++) {
            if (scale_x == 1 && scale_y == 1) {
                plot(x + i, y + j, line & 1 ? color : bg);
            } else {
                fillArea(x + i * scale_x, y + j * scale_y, scale_x, scale_y, line & 1 ? color : bg);
            }
            line>>=1;
        }
    }
}

/**
 @brief    Set the font to display when print()ing, either custom or default
 @param    f  The GFXfont object, if NULL use built in 6x8 font
 */
void setFont(const TfxFont *f) {
    if (!f) return;
    _font = (TfxFont *)f;
}

void print(int16_t x, int16_t y, const char *s, uint16_t color, uint16_t bg, uint8_t scale_x, uint8_t scale_y) {
    while (*s) {
        drawChar(x, y, *s++, color, bg, scale_x, scale_y);
        x += _font->xAdvance * scale_x;
        if (x >= LCD_WIDTH_PX) {
            x = 0;
            y += _font->yAdvance  * scale_y;
            if (y >= LCD_HEIGHT_PX) y = 0;
        }
    }
}



