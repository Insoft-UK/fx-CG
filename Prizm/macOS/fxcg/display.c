/*
 Copyright © 2023 Insoft. All rights reserved.
 
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

#include "fxcg.h"
#include "GFXFont.h"

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

static int getTextSize(unsigned char *c, FontSize size)
{
    GFXfont font = getFont(size, 0);
    int width = 0;
    while (*c) {
        if (*c == 0x7F || *c == 0xE5 || *c == 0xE6)
            c++;
        if (*c < font.first || *c > font.last)
            continue;
        GFXglyph *glyph = &font.glyph[(int)*c - font.first];
        width += glyph->xAdvance;
        c++;
    }
    return width;
}



void Bdisp_AreaClr(struct TBdispFillArea * area, uint8_t target, color_t color)
{
    color_t* VRAM = (color_t *)GetVRAMAddress();
    
    for (int y = area->y1; y <= area->y2 && y < LCD_HEIGHT_PX - 1; y++)
    {
        if (y < 0) continue;
        for (int x = area->x1; x <= area->x2 && x < LCD_WIDTH_PX - 1; x++)
        {
            if (x < 0) continue;
            VRAM[x + y * LCD_WIDTH_PX] = (unsigned short)color;
        }
    }
}

int Bdisp_EnableColor(int mode)
{
    _fxCG_DDRegister.B = mode == FXCGColorModeEnableFullColor ? 0 : 1;
    return 0;
}

/**
 [mode] Area of the VRAM to fill. When mode is 1, the area below the status area is filled; when mode is 2, the area between
 the status area and the F-key labels is filled;  and when mode is 3 or 4, the whole VRAM is filled.
 **/
void Bdisp_Fill_VRAM( int color, int mode )
{
    if (mode == 0 || mode > 4) return;
    
    for (int y = mode == kAreaMode_BelowStatus || mode == kAreaMode_BetweenStatusAndFKeyLabels ? 24 : 0; y < (mode == kAreaMode_BetweenStatusAndFKeyLabels ? LCD_HEIGHT_PX - 24 : LCD_HEIGHT_PX); y++) {
        for (int x = 0; x < LCD_WIDTH_PX; x++) {
            Bdisp_SetPoint_VRAM(x, y, color);
        }
    }
}

void *GetVRAMAddress( void )
{
    return (void *)_fxCG_VRAM;
}

void Bdisp_PutDisp_DD( void ) {
    uint16_t color;
    
    uint16_t *VRAM = GetVRAMAddress();
    for (int x=0; x<LCD_WIDTH_PX; x++) {
        VRAM[x + 22 * LCD_WIDTH_PX] = 0;
    }

    for (int y = 0; y < LCD_HEIGHT_PX; y++) {
        for (int x = 0; x < LCD_WIDTH_PX; x++) {
            color = Bdisp_GetPoint_VRAM(x, y);
            Bdisp_SetPoint_DD(6 + x, y, color);
        }
    }
}

void Bdisp_SetDDRegisterB(int colorMode)
{
    _fxCG_DDRegister.B = colorMode == FXCGColorModeEnableFullColor ? 0 : 1;
}

void Bdisp_SetPoint_DD( int x, int y, int color ) {
    if (x < 0 || x >= LCD_WIDTH_PX + 12 || y < 0 || y >= LCD_HEIGHT_PX + 8) return;
    _fxCG_DRAM[x + y * 396] = (unsigned short)color;
}

void Bdisp_SetPoint_VRAM( int x, int y, int color ) {
    if (x < 0 || x >= LCD_WIDTH_PX || y < 0 || y >= LCD_HEIGHT_PX) return;
    
    uint16_t *VRAM = (uint16_t *)GetVRAMAddress();
    
    if (_fxCG_DDRegister.B != 0) {
        switch (color) {
            case 1:
                color = FXCGColorBlue;
                break;
                
            case 2:
                color = FXCGColorGreen;
                break;
                
            case 3:
                color = FXCGColorCyan;
                break;
                
            case 4:
                color = FXCGColorRed;
                break;
                
            case 5:
                color = FXCGColorMagenta;
                break;
                
            case 6:
                color = FXCGColorYellow;
                break;
                
            case 7:
                color = FXCGColorWhite;
                break;
        }
    }
    
    VRAM[x + y * LCD_WIDTH_PX] = (unsigned short)color;
}

color_t Bdisp_GetPoint_VRAM( int x, int y )
{
    if (x < 0 || x > LCD_WIDTH_PX - 1) return FXCGColorWhite;
    if (y < 0 || y > LCD_HEIGHT_PX - 1) return FXCGColorWhite;
    
    color_t* VRAM = (color_t *)GetVRAMAddress();
        return VRAM[x + y * LCD_WIDTH_PX];

}

/**
 Sets the cursor position for Print_OS

 - Parameter x: x Must be in range of [1,21]
 - Parameter y: y Must be in range of [1,8]
*/
void locate_OS( int x, int y )
{
    if (x < 1 || x > 21) return;
    if (y < 1 || y > 8) return;
    _fxCG_Cursor.x = x;
    _fxCG_Cursor.y = y;
}

void Print_OS(const char* msg, int invers, int zero2)
{
    PrintCXY(_fxCG_Cursor.x * 18, _fxCG_Cursor.y * 24, msg, invers ? FXCGTextModeInvert : FXCGTextModeNormal, -1, FXCGColorBlack, FXCGColorWhite, 1, 0);
    _fxCG_Cursor.x = fxCG_Range(0, 20, _fxCG_Cursor.x + 1);
}

void Bdisp_MMPrintRef(int*x, int*y, unsigned char *s, int mode, int xmax, int P6, int P5, int color, int P9, int P10, int P11)
{
//    fxCGDrawText(*x, *y + 24, (const char*)s, color, FontSize_16pt);
}
void Bdisp_MMPrint(int x, int y, char* s, int mode, int xmax, int P6, int P7, int color, int backcolor, int P10, int P11)
{
//    fxCGDrawText(x, y + 24, (const char*)s, color, FontSize_16pt);
}

void PrintCXY( int x, int y, const char *text, int mode, int P5, int color, int bgcolor, int P8, int P9 )
{
    y += 24;
    if (mode & FXCGTextModeInvert) {
        uint16_t tmp = color;
        color = bgcolor;
        bgcolor = tmp;
    }
    TBdispFillArea area = {
        .x1 = x, .y1 = y, .x2 = x + getTextSize((unsigned char *)text, FontSize_24pt) - 1, .y2 = y + 23
    };
    Bdisp_AreaClr(&area, FXCGFillAreaTargetVRAM, bgcolor);
    fxCGDrawText(x, y, text, color, FontSize_24pt);
}


void PrintXY( int x, int y, const char *string, int mode, int color ) {
    
    PrintCXY(x, y, string + 2, mode, 0, color, Bdisp_GetPoint_VRAM(x, y), 0, 0);
}


void DrawFrame( color_t color )
{
    _fxCG_0xFD801460 = color;
    for (int y = 0; y < 224; y++)
    {
        for (int x = 0; x < 396; x++)
        {
            if (x >= 6 && x < LCD_WIDTH_PX && y < LCD_HEIGHT_PX) continue;
            _fxCG_DRAM[x + y * 396] = (unsigned short)color;
        }
    }
}

color_t FrameColor(FXCGFrameMode mode, color_t color)
{
    if (mode == FXCGFrameModeSetToColor) {
        _fxCG_0xFD801460 = color;
    }
    else {
        _fxCG_0xFD801460 = 0xFFFF;
    }
    return _fxCG_0xFD801460;
}

void DisplayStatusArea(void)
{
    if (_fxCG_StatusArea == false) return;
    
    color_t* VRAM = (color_t *)GetVRAMAddress();
    for (int y = 0; y < 24; y++)
    {
        for (int x = 0; x < LCD_WIDTH_PX; x++)
        {
            _fxCG_DRAM[6 + x + y * 396] = VRAM[x + y * LCD_WIDTH_PX];
        }
    }
}

void EnableStatusArea(int opt)
{
    if (opt == 0 || opt == 2) _fxCG_StatusArea = true;
    if (opt == 3) _fxCG_StatusArea = false;
}

void EnableDisplayHeader(int action, int value)
{
    
}

void PrintGlyph(int x, int y, uint8_t *glyph, int mode, color_t color, color_t bgcolor, int P7)
{
    
    if (!(mode & FXCGTextModeTransparentBackground)) {
        TBdispFillArea area;
    
        area.mode = FXCGFillAreaModeColor;
        area.x1 = x;
        area.y1 = y;
        area.x2 = x + 17;
        area.y2 = y + 23;
        Bdisp_AreaClr(&area, FXCGFillAreaTargetVRAM, color);
    }

    if (mode & FXCGTextModeInvert) {
        uint16_t tmp = color;
        color = bgcolor;
        bgcolor = tmp;
    }
    
    fxCGDrawGlyph(x, y, glyph, color, FontSize_24pt);
    
}

void*GetMiniGlyphPtr( unsigned short mb_glyph_no, unsigned short*glyph_info )
{
    return 0;
}
void PrintMiniGlyph(int x, int y, void *glyph, uint32_t mode, int glyph_width, int P6, int P7, int P8, int P9, int color, int bgcolor, int P12)
{
    fxCGDrawGlyph(x, y, glyph, color, FontSize_18pt);
}
void PrintMini( int *x, int *y, const char *text, unsigned int mode, unsigned int xlimit, int P6, int P7, int color, int bgcolor, int writeflag, int P11 ) {

    int xx = *x, yy = *y;
    yy += (mode & 0x40 ? 0 : 24);
    
    if (mode & FXCGTextModeInvert) {
        uint16_t tmp = color;
        color = bgcolor;
        bgcolor = tmp;
    }
    if (!(mode & 0x2)) {
        TBdispFillArea area = {
            .x1 = *x, .y1 = *y, .x2 = *x + getTextSize((unsigned char *)text, FontSize_24pt) - 1, .y2 = *y + 17
        };
        Bdisp_AreaClr(&area, FXCGFillAreaTargetVRAM, bgcolor);
    }
    
    if (mode & 0x1) {
        *x = fxCGDrawText(xx, yy, text, bgcolor, FontSize_18pt);
        return;
    }
    *x = fxCGDrawText(xx, yy, text, color, FontSize_18pt);
}
void PrintMiniMini( int *x, int *y, const char *text, int mode1, int color, int mode2 )
{
    if (mode1 & 0x1) {
        fxCGDrawText(*x, *y + 24, text, color, FontSize_10pt);\
        return;
    }
    fxCGDrawText(*x, *y + 24, text, color, FontSize_10pt);
    
}

/**
 Sets the cursor position for Print_OS

 - Parameter x: x Must be in range of [0,20]
 - Parameter y: y Must be in range of [0,7]
*/
int Cursor_SetPosition(int x, int y)
{
    _fxCG_Cursor.x = fxCG_Range(0, 20, x);
    _fxCG_Cursor.y = fxCG_Range(0, 7, y);
    return 0;
}

void *getSecondaryVramAddress(void) {
    static color_t vram[LCD_WIDTH_PX * LCD_HEIGHT_PX];
    return vram;
}

void SaveVRAM_1(void) {
    uint64_t *dest = (uint64_t *)getSecondaryVramAddress();
    uint64_t *src = (uint64_t *)_fxCG_VRAM;
    size_t length = LCD_WIDTH_PX * LCD_HEIGHT_PX / 4;
    do {
        *dest++ = *src++;
    } while (--length);
}

void LoadVRAM_1(void) {
    uint64_t *src = (uint64_t *)getSecondaryVramAddress();
    uint64_t *dest = (uint64_t *)_fxCG_VRAM;
    size_t length = LCD_WIDTH_PX * LCD_HEIGHT_PX / 4;
    do {
        *dest++ = *src++;
    } while (--length);
}



//MARK: - NOT! syscalls (defined within libfxcg):

void VRAM_CopySprite(const color_t* data, int x, int y, int width, int height)
{
    color_t* VRAM = (color_t *)GetVRAMAddress();
    VRAM += LCD_WIDTH_PX * y + x;
    
    for(int j = y; j < y + height; j++)
    {
        for(int i = x; i < x + width; i++)
        {
            *(VRAM++) = *(data++);
        }
        VRAM += LCD_WIDTH_PX - width;
    }
}

void VRAM_XORSprite(const color_t* data, int x, int y, int width, int height)
{
    color_t* VRAM = (color_t *)GetVRAMAddress();
    VRAM += LCD_WIDTH_PX * y + x;
    
    for(int j = y; j < y + height; j++)
    {
        for(int i = x; i < x + width; i++)
        {
            *(VRAM++) ^= *(data++);
        }
        VRAM += LCD_WIDTH_PX - width;
    }
}
