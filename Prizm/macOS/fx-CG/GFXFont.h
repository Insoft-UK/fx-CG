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

#ifndef GFXFont_h
#define GFXFont_h

#include <stdint.h>

typedef struct {
    uint16_t offset;   // Offset address to the bitmap data for this glypth.
    uint8_t  width;    // Bitmap dimensions in pixels.
    uint8_t  height;   // Bitmap dimensions in pixels.
    uint8_t  xAdvance; // Distance to advance cursor in the x-axis.
    int8_t   dX;       // Used to position the glyph within the cell in the horizontal direction.
    int8_t   dY;       // Distance from the baseline of the character to the top of the glyph.
} GFXglyph;

typedef struct {
    uint8_t   *bitmap;   // Bitmap data array.
    GFXglyph  *glyph;    // Glyph data.
    uint8_t    first;    // The first ASCII value of your first character.
    uint8_t    last;     // The last ASCII value of your last character.
    uint8_t    yAdvance; // Newline distance in the y-axis.
} GFXfont;

typedef enum FontSize{
    FontSize_24pt,
    FontSize_18pt,
    FontSize_16pt,
    FontSize_10pt
} FontSize;

/**
The fx-CG has a large number of characters it must be able to represent internally. Because of this the fx-CG supports multi-byte character strings.
To differentiate a single byte character from a multi-byte character, a multi-byte character's first byte is always an extended code. Extended codes are 0x7F, 0x7, 0xF9, OxES, OxE6 and OxE7. Currently only four codes are used:
0x7F, 0xF9, 0xES and OxE6.
 */

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

int fxCGDrawGlyph(int x, int y, uint8_t *c, uint16_t color, FontSize size);
int fxCGDrawText(int x, int y, const char *s, uint16_t color, FontSize size);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /* GFXFont_h */
