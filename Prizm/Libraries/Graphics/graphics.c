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
#include "fxcg/system.h"
#else
#include <fxcg/display.h>
#endif

void fxEnableColor(void)
{
    Bdisp_EnableColor(1);
}

void fxPrint(int x, int y, const char *text, uint16_t color, uint16_t bgColor)
{
    PrintCXY(x, y, text, FXCGTextModeNormal,  -1 , color, bgColor, 1, 0);
}

void fxPrintMini(int x, int y, const char *text, uint16_t color, uint16_t bgColor)
{
    PrintMini(&x, &y, text, 0x40,  0xFFFFFFFF , 0, 0, color, bgColor, 1, 0);
}

/**
 @brief    Draw a perfectly horizontal line (this is often optimized in a subclass!)
 @param    x   Left-most x coordinate
 @param    y   Left-most y coordinate
 @param    w   Width in pixels
 @param    color Specifies what color the plotted pixel will be. It is in RGB 565 format.
 */
static void fxDrawFastHLine(unsigned x, unsigned y, unsigned w, uint16_t color)
{
    if (y > 215) return;
    while (w--) {
        fxPlot(x++, y, color);
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
static void fxDrawFastVLine(unsigned x, unsigned y, unsigned h, uint16_t color)
{
    if (x > 383) return;
    while (h--) {
        fxPlot(x, y++, color);
        if (y > 215) return;
    }
}


static void swap(int *x, int *y) {
    *x = *x ^ *y;    // x = x ^ y
    *y = *x ^ *y;    // y = (x ^ y) ^ y wich means now y = original x
    *x = *x ^ *y;    // x = (x ^ y) ^ y but since y is now original x then
    // it is really x = (x ^ y) ^ x, so x = original y
}

uint16_t fxColor(uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t color, channel;
    
    channel = (uint16_t)((float)r / 255.0 * 31.0);
    color = channel;
    channel = (uint16_t)((float)g / 255.0 * 63.0);
    color = color * 64 + channel;
    channel = (uint16_t)((float)b / 255.0 * 31.0);
    color = color * 32 + channel;
    
    return color;
}

void fxDrawLine(int x1, int y1, int x2, int y2, uint16_t color)
{
    if (x1 == x2) {
        if (y1 > y2)
            swap(&y1, &y2);
        fxDrawFastVLine(x1, y1, y2 - y1 + 1, color);
        return;
    }
    
    if (y1 == y2) {
        if (x1 > x2)
            swap(&x1, &x2);
        fxDrawFastHLine(x1, y1, x2 - x1 + 1, color);
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
    
    fxPlot(x1, y1, color);
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
            fxPlot(x1, y1, color);
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
            fxPlot(x1, y1, color);
        }
    }
}

void fxDrawRect(int x, int y, short w, short h, uint16_t color)
{
    fxDrawFastHLine(x, y, w, color);
    fxDrawFastHLine(x, y + h - 1, w, color);
    fxDrawFastVLine(x, y, h, color);
    fxDrawFastVLine(x + w - 1, y, h, color);
}

/**
 @brief    Quarter-circle drawer, used to do circles and roundrects.
 @param    x   Center-point x coordinate.
 @param    y   Center-point y coordinate.
 @param    r   Radius of circle.
 @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of the circle we're doing.
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
static void fxDrawCircleHelper(int x, int y, short r, uint8_t cornername, uint16_t color)
{
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
            fxPlot(x + xx, y + yy, color);
            fxPlot(x + yy, y + xx, color);
        }
        if (cornername & 0x2) {
            fxPlot(x + xx, y - yy, color);
            fxPlot(x + yy, y - xx, color);
        }
        if (cornername & 0x8) {
            fxPlot(x - yy, y + xx, color);
            fxPlot(x - xx, y + yy, color);
        }
        if (cornername & 0x1) {
            fxPlot(x - yy, y - xx, color);
            fxPlot(x - xx, y - yy, color);
        }
    }
}

void fxDrawCircle(int x, int y, short r, uint16_t color)
{
    short f = 1 - r;
    short ddF_x = 1;
    short ddF_y = -2 * r;
    short xx = 0;
    short yy = r;
    
    fxPlot(x, y + r, color);
    fxPlot(x, y - r, color);
    fxPlot(x + r, y, color);
    fxPlot(x - r, y, color);
    
    while (xx < yy) {
        if (f >= 0) {
            yy--;
            ddF_y += 2;
            f += ddF_y;
        }
        xx++;
        ddF_x += 2;
        f += ddF_x;
        
        fxPlot(x + xx, y + yy, color);
        fxPlot(x - xx, y + yy, color);
        fxPlot(x + xx, y - yy, color);
        fxPlot(x - xx, y - yy, color);
        fxPlot(x + yy, y + xx, color);
        fxPlot(x - yy, y + xx, color);
        fxPlot(x + yy, y - xx, color);
        fxPlot(x - yy, y - xx, color);
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
static void fxFillCircleHelper(int x, int y, short r, uint8_t corners, short delta, uint16_t color)
{
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
                fxDrawFastVLine(x + xx, y - yy, 2 * yy + delta, color);
            if (corners & 2)
                fxDrawFastVLine(x - xx, y - yy, 2 * yy + delta, color);
        }
        if (yy != py) {
            if (corners & 1)
                fxDrawFastVLine(x + py, y - px, 2 * px + delta, color);
            if (corners & 2)
                fxDrawFastVLine(x - py, y - px, 2 * px + delta, color);
            py = yy;
        }
        px = xx;
    }
}


void fxFillCircle(int x, int y, short r, uint16_t color)
{
    
    fxDrawFastVLine(x, y - r, 2 * r + 1, color);
    fxFillCircleHelper(x, y, r, 3, 0, color);
    
}


void fxDrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint8_t color)
{
    fxDrawLine(x1, y1, x2, y2, color);
    fxDrawLine(x2, y2, x3, y3, color);
    fxDrawLine(x3, y3, x1, y1, color);
}


void fxFillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color)
{
    
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
        fxDrawFastHLine(a, y1, b - a + 1, color);
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
        fxDrawFastHLine(a, y, b - a + 1, color);
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
        fxDrawFastHLine(a, y, b - a + 1, color);
    }
}


void fxDrawRoundRect(int x, int y, int w, int h, short r, uint16_t color)
{
    int max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (r > max_radius)
        r = max_radius;
    // smarter version
  
    fxDrawFastHLine(x + r, y, w - 2 * r, color);         // Top
    fxDrawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    fxDrawFastVLine(x, y + r, h - 2 * r, color);         // Left
    fxDrawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    fxDrawCircleHelper(x + r, y + r, r, 1, color);
    fxDrawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    fxDrawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    fxDrawCircleHelper(x + r, y + h - r - 1, r, 8, color);
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
void fxFillRoundRect(int x, int y, int w, int h, short r, uint16_t color)
{
    int max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (r > max_radius)
        r = max_radius;
  
    fxFillArea(x + r, y, w - 2 * r, h, color);
    fxFillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    fxFillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}


uint16_t fxMakeGray(int shade)
{
    return shade * 0x0841;
}


void fxFillArea(unsigned x, unsigned y, unsigned w, unsigned h, uint16_t color)
{
    if (x > 383 || y > 215) return;
    uint16_t *VRAM = (uint16_t*)GetVRAMAddress();
    VRAM += y * 384 + x;
    while(h--){
        unsigned w2 = w;
        while(w2--)
            *VRAM++ = color;
        VRAM += 384 - w;
    }
}


void fxPlot(int x, int y, uint16_t color)
{
    if (x < 0 || y < 0) return;
    if (x > 383 || y > 215) return;
    uint16_t *VRAM = (uint16_t *)GetVRAMAddress();
    VRAM[x + y * 384] = color;
}

void fxSubPlot(unsigned x, unsigned y, uint8_t level)
{
    if (x > 1151 || y > 215) return;
    uint16_t *VRAM = (uint16_t *)GetVRAMAddress();
    
    int sub = x % 3;
    x /= 3;
    switch (sub) {
        case 0:
            VRAM[x + y * 384] = (VRAM[x + y * 384] & 0x07FF) | fxColor(level, 0, 0);
            break;
            
        case 1:
            VRAM[x + y * 384] = (VRAM[x + y * 384] & 0xF81F) | fxColor(0, level, 0);
            break;
            
        case 2:
            VRAM[x + y * 384] = (VRAM[x + y * 384] & 0xFFE0) | fxColor(0, 0, level);
            break;
    }
}


void fxDrawSprite(uint16_t* data, int x, int y, int w, int h)
{
    uint16_t *VRAM = (uint16_t*)GetVRAMAddress();
    VRAM += 384*y + x;
    for(int j=y; j<y+h; j++) {
        for(int i=x; i<x+w; i++) {
            *(VRAM++) = *(data++);
        }
        VRAM += 384 - w;
    }
}


void fxDrawSpriteMaskedAlpha(uint16_t* data, int x, int y, int w, int h, uint16_t maskColor, int alpha)
{
    uint16_t* VRAM = (uint16_t*)GetVRAMAddress();
    VRAM += LCD_WIDTH_PX*y + x;
    for(int j=y; j<y+h; j++) {
        for(int i=x; i<x+w; i++) {
            if (*data != maskColor) {
                *(VRAM) = (uint16_t)((((int)(*data & 0xf81f) * alpha + (int)(*VRAM & 0xf81f) * (32-alpha) + 0x8010) >> 5) & 0xf81f) |
                (uint16_t)((((int)(*data & 0x07e0) * alpha + (int)(*VRAM & 0x07e0) * (32-alpha) + 0x0400) >> 6) & 0x07e0);
                VRAM++; data++;
            } else {
                VRAM++;
                data++;
            }
        }
        VRAM += LCD_WIDTH_PX - w;
    }
}


void fxDrawSpriteNbit(const uint8_t* data, int x, int y, int w, int h, uint16_t* palette, unsigned int bitWidth)
{
    color_t* VRAM = (color_t*) GetVRAMAddress();
    VRAM += (LCD_WIDTH_PX*y + x);
    int offset = 0;
    uint8_t buf = 0;
    for(int j=y; j < y+h; j++) {
        int availbits = 0;
        for(int i=x; i < x+w; i++) {
            if (!availbits) {
                buf = data[offset++];
                availbits = 8;
            }
            uint16_t this = ((uint16_t)buf >> (8-bitWidth));
            *VRAM = palette[(uint16_t)this];
            VRAM++;
            buf <<= bitWidth;
            availbits -= bitWidth;
        }
        VRAM += (LCD_WIDTH_PX - w);
    }
}


void fxDrawSpriteNbitMasked(const uint8_t *data, int x, int y, int w, int h, const uint16_t* palette, uint16_t maskColor, unsigned int bitWidth)
{
    color_t* VRAM = (color_t*) GetVRAMAddress();
    VRAM += (LCD_WIDTH_PX*y + x);
    int offset = 0;
    uint8_t buf = 0;
    for(int j=y; j<y+h; j++) {
        int availbits = 0;
        for(int i=x; i<x+w; i++) {
            if (!availbits) {
                buf = data[offset++];
                availbits = 8;
            }
            uint16_t this = ((uint16_t)buf>>(8-bitWidth));
            uint16_t color = palette[this];
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


void fxChangeFreq(int mult)
{
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
