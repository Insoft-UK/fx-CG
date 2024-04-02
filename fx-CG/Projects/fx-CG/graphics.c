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

#include "graphics.h"

#ifdef __clang__
#include "fxcg/display.h"
#else
#include <fxcg/display.h>
#endif

/**
 @brief    Draw a perfectly horizontal line (this is often optimized in a subclass!)
 @param    x   Left-most x coordinate
 @param    y   Left-most y coordinate
 @param    w   Width in pixels
 @param    color Specifies what color the plotted pixel will be. It is in RGB 565 format.
 */
static void drawFastHLine(unsigned x, unsigned y, unsigned w, unsigned short color) {
    if (y > 215) return;
    while (w--) {
        plot(x++, y, color);
        if (x > 383) return;
    }
}

/**
 @brief    Draw a perfectly vertical line (this is often optimized in a subclass!)
 @param    x   Top-most x coordinate
 @param    y   Top-most y coordinate
 @param    h   Height in pixels
 @param    color Specifies what color the plotted pixel will be. It is in RGB 565 format.
 */
static void drawFastVLine(unsigned x, unsigned y, unsigned h, unsigned short color) {
    if (x > 383) return;
    while (h--) {
        plot(x, y++, color);
        if (y > 215) return;
    }
}

static void swap(int *x, int *y) {
    *x = *x ^ *y;    // x = x ^ y
    *y = *x ^ *y;    // y = (x ^ y) ^ y wich means now y = original x
    *x = *x ^ *y;    // x = (x ^ y) ^ y but since y is now original x then
    // it is really x = (x ^ y) ^ x, so x = original y
}

/**
 @brief    Returns a color in RGB 565 format from a given RGB[0-255] value.
 @param    r  Red channel
 @param    g  Green channel
 @param    b  Blue channel
 */
unsigned short rgb(unsigned char r, unsigned char g, unsigned char b) {
    unsigned short color, channel;
    
    channel = (unsigned short)((float)r / 255.0 * 31.0);
    color = channel;
    channel = (unsigned short)((float)g / 255.0 * 63.0);
    color = color * 64 + channel;
    channel = (unsigned short)((float)b / 255.0 * 31.0);
    color = color * 32 + channel;
    
    return color;
}

/**
 @brief    Write a line.  Bresenham's algorithm
 @param    x1  Start point x coordinate
 @param    y1  Start point y coordinate
 @param    x2  End point x coordinate
 @param    y2  End point y coordinate
 @param    color Specifies what color the plotted pixel will be. It is in RGB 565 format.
 */
void drawLine(int x1, int y1, int x2, int y2, unsigned short color) {
    if (x1 == x2) {
        if (y1 > y2)
            swap(&y1, &y2);
        drawFastVLine(x1, y1, y2 - y1 + 1, color);
        return;
    }
    
    if (y1 == y2) {
        if (x1 > x2)
            swap(&x1, &x2);
        drawFastHLine(x1, y1, x2 - x1 + 1, color);
        return;
    }
    
    signed char ix;
    signed char iy;
    
    // if x1 == x2 or y1 == y2, then it does not matter what we set here
    int delta_x;
    int delta_y;
    
    if (x2 > x1) {
        ix = 1;
        delta_x = x2 - x1;
    } else {
        ix = -1;
        delta_x = x1 - x2;
    }
    
    if (y2 > y1) {
        iy = 1;
        delta_y = y2 - y1;
    } else {
        iy = -1;
        delta_y = y1 - y2;
    }
    
    plot(x1, y1, color);
    if (delta_x >= delta_y) {
        int error = delta_y - (delta_x >> 1);        // error may go below zero
        while (x1 != x2) {
            if (error >= 0) {
                if (error || (ix > 0)) {
                    y1 += iy;
                    error -= delta_x;
                }                           // else do nothing
            }                              // else do nothing
            x1 += ix;
            error += delta_y;
            plot(x1, y1, color);
        }
    } else {
        int error = delta_x - (delta_y >> 1);      // error may go below zero
        while (y1 != y2) {
            if (error >= 0) {
                if (error || (iy > 0)) {
                    x1 += ix;
                    error -= delta_y;
                }                           // else do nothing
            }                              // else do nothing
            y1 += iy;
            error += delta_x;
            plot(x1, y1, color);
        }
    }
}

/**
 @brief    Draw a rectangle with no fill color
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void drawRect(int x, int y, short w, short h, unsigned short color) {
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y + h - 1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x + w - 1, y, h, color);
}

/**
 @brief    Quarter-circle drawer, used to do circles and roundrects.
 @param    x   Center-point x coordinate.
 @param    y   Center-point y coordinate.
 @param    r   Radius of circle.
 @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of the circle we're doing.
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
static void drawCircleHelper(int x, int y, short r, unsigned char cornername, unsigned short color) {
    short f = 1 - r;
    short ddF_x = 1;
    short ddF_y = -2 * r;
    short xx = 0;
    short yy = r;
    
    while (xx < yy) {
        if (f >= 0) {
            yy--;
            ddF_y += 2;
            f += ddF_y;
        }
        xx++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            plot(x + xx, y + yy, color);
            plot(x + yy, y + xx, color);
        }
        if (cornername & 0x2) {
            plot(x + xx, y - yy, color);
            plot(x + yy, y - xx, color);
        }
        if (cornername & 0x8) {
            plot(x - yy, y + xx, color);
            plot(x - xx, y + yy, color);
        }
        if (cornername & 0x1) {
            plot(x - yy, y - xx, color);
            plot(x - xx, y - yy, color);
        }
    }
}

/**
 @brief    Draw a circle outline.
 @param    x   Center-point x coordinate.
 @param    y   Center-point y coordinate.
 @param    r   Radius of circle.
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void drawCircle(int x, int y, short r, unsigned short color) {
    short f = 1 - r;
    short ddF_x = 1;
    short ddF_y = -2 * r;
    short xx = 0;
    short yy = r;
    
    plot(x, y + r, color);
    plot(x, y - r, color);
    plot(x + r, y, color);
    plot(x - r, y, color);
    
    while (xx < yy) {
        if (f >= 0) {
            yy--;
            ddF_y += 2;
            f += ddF_y;
        }
        xx++;
        ddF_x += 2;
        f += ddF_x;
        
        plot(x + xx, y + yy, color);
        plot(x - xx, y + yy, color);
        plot(x + xx, y - yy, color);
        plot(x - xx, y - yy, color);
        plot(x + yy, y + xx, color);
        plot(x - yy, y + xx, color);
        plot(x + yy, y - xx, color);
        plot(x - yy, y - xx, color);
    }
}

/**
 @brief    Quarter-circle drawer with fill, used for circles and roundrects.
 @param    x Center-point x coordinate.
 @param    y Center-point y coordinate.
 @param    r  Radius of circle.
 @param    corners  Mask bits indicating which quarters we're doing.
 @param    delta    Offset from center-point, used for round-rects.
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
static void fillCircleHelper(int x, int y, short r, unsigned char corners, short delta, unsigned short color) {
    short f = 1 - r;
    short ddF_x = 1;
    short ddF_y = -2 * r;
    short xx = 0;
    short yy = r;
    short px = xx;
    short py = yy;
    
    delta++; // Avoid some +1's in the loop
    
    while (xx < yy) {
        if (f >= 0) {
            yy--;
            ddF_y += 2;
            f += ddF_y;
        }
        xx++;
        ddF_x += 2;
        f += ddF_x;
        // These checks avoid double-drawing certain lines, important
        // for the SSD1306 library which has an INVERT drawing mode.
        if (xx < (yy + 1)) {
            if (corners & 1)
                drawFastVLine(x + xx, y - yy, 2 * yy + delta, color);
            if (corners & 2)
                drawFastVLine(x - xx, y - yy, 2 * yy + delta, color);
        }
        if (yy != py) {
            if (corners & 1)
                drawFastVLine(x + py, y - px, 2 * px + delta, color);
            if (corners & 2)
                drawFastVLine(x - py, y - px, 2 * px + delta, color);
            py = yy;
        }
        px = xx;
    }
}

/**
 @brief    Draw a circle with filled color
 @param    x   Center-point x coordinate
 @param    y   Center-point y coordinate
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void fillCircle(int x, int y, short r, unsigned short color) {
    
    drawFastVLine(x, y - r, 2 * r + 1, color);
    fillCircleHelper(x, y, r, 3, 0, color);
    
}

/**
 @brief    Draw a triangle with no fill color
 @param    x1  Vertex #1 x coordinate
 @param    y1  Vertex #1 y coordinate
 @param    x2  Vertex #2 x coordinate
 @param    y2  Vertex #2 y coordinate
 @param    x3  Vertex #3 x coordinate
 @param    y3  Vertex #3 y coordinate
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned char color) {
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x3, y3, color);
    drawLine(x3, y3, x1, y1, color);
}

/**
 @brief    Draw a triangle with color-fill
 @param    x1  Vertex #1 x coordinate
 @param    y1  Vertex #1 y coordinate
 @param    x2  Vertex #2 x coordinate
 @param    y2  Vertex #2 y coordinate
 @param    x3  Vertex #3 x coordinate
 @param    y3  Vertex #3 y coordinate
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned short color) {
    
    int a, b, y, last;
    
    // Sort coordinates by Y order (y3 >= y2 >= y1)
    if (y1 > y2) {
        swap(&y1, &y2);
        swap(&x1, &x2);
    }
    if (y2 > y3) {
        swap(&y3, &y2);
        swap(&x3, &x2);
    }
    if (y1 > y2) {
        swap(&y1, &y2);
        swap(&x1, &x2);
    }
    
    if (y1 == y3) { // Handle awkward all-on-same-line case as its own thing
        a = b = x1;
        if (x2 < a)
            a = x2;
        else if (x2 > b)
            b = x2;
        if (x3 < a)
            a = x3;
        else if (x3 > b)
            b = x3;
        drawFastHLine(a, y1, b - a + 1, color);
        return;
    }
    
    int dx01 = x2 - x1, dy01 = y2 - y1, dx02 = x3 - x1, dy02 = y3 - y1, dx12 = x3 - x2, dy12 = y3 - y2;
    int sa = 0, sb = 0;
    
    /*
     For upper part of triangle, find scanline crossings for segments
     0-1 and 0-2.  If y2=y3 (flat-bottomed triangle), the scanline y2
     is included here (and second loop will be skipped, avoiding a /0
     error there), otherwise scanline y2 is skipped here and handled
     in the second loop...which also avoids a /0 error here if y1=y2
     (flat-topped triangle).
     */
    
    if (y2 == y3)
        last = y2; // Include y2 scanline
    else
        last = y2 - 1; // Skip it
    
    for (y = y1; y <= last; y++) {
        a = x1 + sa / dy01;
        b = x1 + sb / dy02;
        sa += dx01;
        sb += dx02;
        
        if (a > b)
            swap(&a, &b);
        drawFastHLine(a, y, b - a + 1, color);
    }
    
    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y2=y3.
    sa = dx12 * (y - y2);
    sb = dx02 * (y - y1);
    for (; y <= y3; y++) {
        a = x2 + sa / dy12;
        b = x1 + sb / dy02;
        sa += dx12;
        sb += dx02;
        
        if (a > b)
            swap(&a, &b);
        drawFastHLine(a, y, b - a + 1, color);
    }
}

/**
 @brief    Draw a rounded rectangle with no fill color
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    r   Radius of corner rounding
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void drawRoundRect(int x, int y, int w, int h, short r, unsigned short color) {
    int max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (r > max_radius)
        r = max_radius;
    // smarter version
  
    drawFastHLine(x + r, y, w - 2 * r, color);         // Top
    drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    drawFastVLine(x, y + r, h - 2 * r, color);         // Left
    drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    drawCircleHelper(x + r, y + r, r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

/**
 @brief    Draw a rounded rectangle with fill color
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    r   Radius of corner rounding
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void fillRoundRect(int x, int y, int w, int h, short r, unsigned short color) {
    int max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (r > max_radius)
        r = max_radius;
  
    fillArea(x + r, y, w - 2 * r, h, color);
    fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

/**
 @brief    Creates a RGB 565 format color on a grey value.
 @param    shade Shade which can be between 0 (black) and 31 (white).
 */
unsigned short makeGray(int shade) {
    return shade * 0x0841;
}

/**
 @brief    Fills a rectangular area of (width,height) with upper-left corner at (x,y)
 @param    x   X-coordinate of the upper-left corner.
 @param    y   Y-coordinate of the upper-left corner.
 @param    w   Width of the filled rectangle.
 @param    h   Height of the filled rectangle.
 @param    color Color of the filled rectangle.
 */
void fillArea(unsigned x, unsigned y, unsigned w, unsigned h, unsigned short color) {
    if (x > 383 || y > 215) return;
    unsigned short *VRAM = (unsigned short*)GetVRAMAddress();
    VRAM += y * 384 + x;
    while(h--){
        unsigned w2 = w;
        while(w2--)
            *VRAM++ = color;
        VRAM += 384 - w;
    }
}

/**
 @brief    Fills a rectangular area of (width,height) with upper-left corner at (x,y)
 @param    x   Specifies the x coordinate of the pixel in range of [0,383]
 @param    y   Specifies the y coordinate of the pixel in range of [0,215]
 @param    color Specifies what color the plotted pixel will be. It is in RGB 565 format.
 */
void plot(unsigned x, unsigned y, unsigned short color) {
    if (x > 383 || y > 215) return;
    unsigned short *VRAM = (unsigned short *)GetVRAMAddress();
    VRAM[x + y * 384] = color;
}

/**
 @brief    Draw a 16-bit sprite/image to the VRAM buffer.
 @param    data Pointer to sprite data.
 @param    x   X-coordinate of top-left of sprite.
 @param    y   Y-coordinate of top-left of sprite.
 @param    w   Width of sprite, in pixels.
 @param    h   Height of sprite, in pixels.
 */
void drawSprite(unsigned short* data, int x, int y, int w, int h) {
    unsigned short *VRAM = (unsigned short*)GetVRAMAddress();
    VRAM += LCD_WIDTH_PX*y + x;
    for(int j=y; j<y+h; j++) {
        for(int i=x; i<x+w; i++) {
            *(VRAM++) = *(data++);
        }
        VRAM += LCD_WIDTH_PX - w;
    }
}

/**
 @brief    Draw a 16-bit sprite/image to the VRAM buffer.
 @param    data Pointer to sprite data.
 @param    x   X-coordinate of top-left of sprite.
 @param    y   Y-coordinate of top-left of sprite.
 @param    w   Width of sprite, in pixels.
 @param    h   Height of sprite, in pixels.
 @param    maskColor Color that should be treated as transparent.
 */
void drawSpriteMaskedAlpha(unsigned short* data, int x, int y, int w, int h, unsigned short maskColor, int alpha) {
    unsigned short* VRAM = (unsigned short*)GetVRAMAddress();
    VRAM += LCD_WIDTH_PX*y + x;
    for(int j=y; j<y+h; j++) {
        for(int i=x; i<x+w; i++) {
            if (*data != maskColor) {
                *(VRAM) = (unsigned short)((((int)(*data & 0xf81f) * alpha + (int)(*VRAM & 0xf81f) * (32-alpha) + 0x8010) >> 5) & 0xf81f) |
                (unsigned short)((((int)(*data & 0x07e0) * alpha + (int)(*VRAM & 0x07e0) * (32-alpha) + 0x0400) >> 6) & 0x07e0);
                VRAM++; data++;
            } else {
                VRAM++;
                data++;
            }
        }
        VRAM += LCD_WIDTH_PX - w;
    }
}

/**
 @brief    Draw an N-bit (for N=8, 2, or 1) sprite/image to the VRAM buffer.
 @param    data Pointer to sprite data.
 @param    x   X-coordinate of top-left of sprite.
 @param    y   Y-coordinate of top-left of sprite.
 @param    w   Width of sprite, in pixels.
 @param    h   Height of sprite, in pixels.
 @param    palette A palette.
 @param    bitWidth The bit-width. This is designed for 8-, 2-, and 1-bit images.
 */
void drawSpriteNbit(const unsigned char* data, int x, int y, int w, int h, unsigned short* palette, unsigned int bitWidth) {
    color_t* VRAM = (color_t*) GetVRAMAddress();
    VRAM += (LCD_WIDTH_PX*y + x);
    int offset = 0;
    unsigned char buf = 0;
    for(int j=y; j < y+h; j++) {
        int availbits = 0;
        for(int i=x; i < x+w; i++) {
            if (!availbits) {
                buf = data[offset++];
                availbits = 8;
            }
            unsigned short this = ((unsigned short)buf >> (8-bitWidth));
            *VRAM = palette[(unsigned short)this];
            VRAM++;
            buf <<= bitWidth;
            availbits -= bitWidth;
        }
        VRAM += (LCD_WIDTH_PX - w);
    }
}

/**
 @brief    Draw an N-bit (for N=8, 2, or 1) sprite/image to the VRAM buffer, treating one color as transparent.
 @param    data Pointer to sprite data.
 @param    x   X-coordinate of top-left of sprite.
 @param    y   Y-coordinate of top-left of sprite.
 @param    w   Width of sprite, in pixels.
 @param    h   Height of sprite, in pixels.
 @param    palette A palette.
 @param    maskColor Color that should be treated as transparent.
 @param    bitWidth The bit-width. This is designed for 8-, 2-, and 1-bit images.
 */
void drawSpriteNbitMasked(const unsigned char* data, int x, int y, int w, int h, const unsigned short* palette, unsigned short maskColor, unsigned int bitWidth) {
    color_t* VRAM = (color_t*) GetVRAMAddress();
    VRAM += (LCD_WIDTH_PX*y + x);
    int offset = 0;
    unsigned char buf = 0;
    for(int j=y; j<y+h; j++) {
        int availbits = 0;
        for(int i=x; i<x+w; i++) {
            if (!availbits) {
                buf = data[offset++];
                availbits = 8;
            }
            unsigned short this = ((unsigned short)buf>>(8-bitWidth));
            unsigned short color = palette[this];
            if(color != maskColor) {
                *VRAM = color;
            }
            VRAM++;
            buf <<= bitWidth;
            availbits -= bitWidth;
        }
        VRAM += (LCD_WIDTH_PX - w);
    }
}



/**
 @brief    Change the speed of the operating PLL circuit.
 
 Notes: PLEASE before handling MENU keypresses in your add-in, use PLL_16x to go back to normal operating speed, at the courtesy of other applications and the OS. Feeding a value not defined below (AKA, an invalid value), will cause a crash.
 */
void changeFreq(int mult) {
#ifndef __clang__
    __asm__(
            "mov r4, r0\n\t"
            "and #0x3F, r0\n\t"
            "shll16 r0\n\t"
            "shll8 r0\n\t"
            "mov.l frqcr, r1\n\t"
            "mov.l pll_mask, r3\n\t"
            "mov.l @r1, r2\n\t"
            "and r3, r2\n\t"
            "or r0, r2\n\t"
            "mov.l r2, @r1\n\t"
            "mov.l frqcr_kick_bit, r0\n\t"
            "mov.l @r1, r2\n\t"
            "or r0, r2\n\t"
            "rts\n\t"
            "mov.l r2, @r1\n\t"
            ".align 4\n\t"
            "frqcr_kick_bit: .long 0x80000000\n\t"
            "pll_mask: .long 0xC0FFFFFF\n\t"
            "frqcr: .long 0xA4150000\n\t"
            );
#endif
}
