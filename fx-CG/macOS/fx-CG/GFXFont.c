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

#include "GFXFont.h"
#include "fx-CG.h"
#include "fxCG50_10pt0xXX.h"
#include "fxCG50_10pt0x7FXX.h"
#include "fxCG50_10pt0xE5XX.h"
#include "fxCG50_10pt0xE6XX.h"
#include "fxCG50_16pt0xXX.h"
#include "fxCG50_16pt0xE5XX.h"
#include "fxCG50_16pt0xE6XX.h"
#include "fxCG50_18pt0xXX.h"
#include "fxCG50_18pt0x7FXX.h"
#include "fxCG50_18pt0xE5XX.h"
#include "fxCG50_18pt0xE6XX.h"
#include "fxCG50_24pt0xXX.h"
#include "fxCG50_24pt0x7FXX.h"
#include "fxCG50_24pt0xE5XX.h"
#include "fxCG50_24pt0xE6XX.h"

static GFXfont get24pt(uint8_t extendedCode)
{
    switch (extendedCode) {
        case 0x7F:
            return fxCG50_24pt0x7FXX;
            break;
            
        case 0xE5:
            return fxCG50_24pt0xE5XX;
            break;
            
        case 0xE6:
            return fxCG50_24pt0xE6XX;
            break;
            
        default:
            return fxCG50_24pt0xXX;
            break;
    }
}

static GFXfont get18pt(uint8_t extendedCode)
{
    switch (extendedCode) {
        case 0x7F:
            return fxCG50_18pt0x7FXX;
            break;
            
        case 0xE5:
            return fxCG50_18pt0xE5XX;
            break;
            
        case 0xE6:
            return fxCG50_18pt0xE6XX;
            break;
            
        default:
            return fxCG50_18pt0xXX;
            break;
    }
}

static GFXfont get16pt(uint8_t extendedCode)
{
    switch (extendedCode) {
        case 0xE5:
            return fxCG50_16pt0xE5XX;
            break;
            
        case 0xE6:
            return fxCG50_16pt0xE6XX;
            break;
            
        default:
            return fxCG50_16pt0xXX;
            break;
    }
}

static GFXfont get10pt(uint8_t extendedCode)
{
    switch (extendedCode) {
        case 0x7F:
            return fxCG50_10pt0x7FXX;
            break;
            
        case 0xE5:
            return fxCG50_10pt0xE5XX;
            break;
            
        case 0xE6:
            return fxCG50_10pt0xE6XX;
            break;
            
        default:
            return fxCG50_10pt0xXX;
            break;
    }
}

static GFXfont getFont(FontSize size, uint8_t extendedCode)
{
    switch (size) {
        case FontSize_24pt:
            return get24pt(extendedCode);
            break;
            
        case FontSize_18pt:
            return get18pt(extendedCode);
            break;
            
        case FontSize_16pt:
            return get16pt(extendedCode);
            break;
            
        case FontSize_10pt:
            return get10pt(extendedCode);
            break;
    }
}

int fxCGDrawGlyph(int x, int y, uint8_t *c, uint16_t color, FontSize size)
{
    GFXfont font;
    
    if (*c == 0x7F || *c == 0xE5 || *c == 0xE6) {
        font = getFont(size, *c++);
    } else {
        font = getFont(size, 0);
    }
    
    
    if (*c < font.first || *c > font.last) {
        return (int)size;
    }
    
    GFXglyph *glyph = &font.glyph[(int)*c - font.first];
    
    int height = glyph->height;
    int width = glyph->width;
   
    x += glyph->dX;
    y += glyph->dY + font.yAdvance;
    
    uint8_t *bitmap = font.bitmap + glyph->offset;
    uint8_t bitPosition = 1 << 7;
    uint16_t *VRAM = fxCGGetVRAM();
    while (height--) {
        for (int xx=0; xx<width; xx++) {
            if (!bitPosition) {
                bitPosition = 1 << 7;
                bitmap++;
            }
            if (*bitmap & bitPosition) {
                VRAM[x + xx + y * LCD_WIDTH_PX] = color;
            }
            bitPosition >>= 1;
        }
        y++;
    }
    return glyph->xAdvance;
}

int fxCGDrawText(int x, int y, const char *s, uint16_t color, FontSize size)
{
    uint8_t *c = (uint8_t *)s;
    
    while (*c) {
        x += fxCGDrawGlyph(x, y, c, color, size);
        if (*c == 0x7F || *c == 0xE5 || *c == 0xE6)
            c++;
        c++;
    }
    return x;
}
