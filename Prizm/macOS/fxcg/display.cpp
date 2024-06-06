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
//#include "GFXFont.h"

#include "fxCG/fxCG.hpp"
#include "fxCG/draw.hpp"
#include "fxCG/font.hpp"

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



using namespace fxCG;


typedef enum {
    FontSize24pt,
    FontSize18pt,
    FontSize16pt,
    FontSize10pt
} TFontSize;

static GFXfont Get24ptFont(uint8_t extendedCode)
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

static GFXfont Get18ptFont(uint8_t extendedCode)
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

static GFXfont Get16ptFont(uint8_t extendedCode)
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

static GFXfont Get10ptFont(uint8_t extendedCode)
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

static GFXfont GetFont(TFontSize size, uint8_t extendedCode)
{
    switch (size) {
        case FontSize24pt:
            return Get24ptFont(extendedCode);
            break;
            
        case FontSize18pt:
            return Get18ptFont(extendedCode);
            break;
            
        case FontSize16pt:
            return Get16ptFont(extendedCode);
            break;
            
        case FontSize10pt:
            return Get10ptFont(extendedCode);
            break;
    }
}

static int DrawGlyph(int x, int y, uint8_t *c, uint16_t color, TFontSize size)
{
    GFXfont font;
    
    if (*c == 0x7F || *c == 0xE5 || *c == 0xE6) {
        font = GetFont(size, *c++);
    } else {
        font = GetFont(size, 0);
    }
    
    
    return font::glyph(x, y, *c, color, font);
}

int DrawText(int x, int y, const char *s, uint16_t color, TFontSize size)
{
    uint8_t *c = (uint8_t *)s;
    
    while (*c) {
        x += DrawGlyph(x, y, c, color, size);
        if (*c == 0x7F || *c == 0xE5 || *c == 0xE6)
            c++;
        c++;
    }
    return x;
    
}

static int getTextSize(unsigned char *c, TFontSize size)
{
    GFXfont font = GetFont(size, 0);
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


void Bdisp_AreaClr(TBdispFillArea * area, uint8_t target, color_t color)
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
    _fxCG_DDRegister.B = mode == ColorModeEnableFullColor ? 0 : 1;
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

#include "Battery.h"
#include "Shift.h"
#include "Alpha.h"

static void DrawImage(int x, int y, int w, int h, void *image)
{
    uint16_t *ptr = (uint16_t *)image;
    while (h--) {
        for (int i=0; i<w; i++) {
            uint16_t color = *ptr++;
            color = color >> 8 | color << 8;
            Bdisp_SetPoint_DD(6 + x + i, y, color);
        }
        y++;
    }
}

void Bdisp_PutDisp_DD( void ) {
    uint16_t color;


    int y = _fxCG_StatusArea ? 22 : 0;
    
    for (; y < LCD_HEIGHT_PX; y++) {
        for (int x = 0; x < LCD_WIDTH_PX; x++) {
            color = Bdisp_GetPoint_VRAM(x, y);
            Bdisp_SetPoint_DD(6 + x, y, color);
        }
    }
    
    if (!_fxCG_StatusArea) return;
    
    DisplayStatusArea();
    
    
}

void Bdisp_SetDDRegisterB(int colorMode)
{
    _fxCG_DDRegister.B = colorMode == ColorModeEnableFullColor ? 0 : 1;
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
                color = ColorBlue;
                break;
                
            case 2:
                color = ColorGreen;
                break;
                
            case 3:
                color = ColorCyan;
                break;
                
            case 4:
                color = ColorRed;
                break;
                
            case 5:
                color = ColorMagenta;
                break;
                
            case 6:
                color = ColorYellow;
                break;
                
            case 7:
                color = ColorWhite;
                break;
        }
    }
    
    VRAM[x + y * LCD_WIDTH_PX] = (unsigned short)color;
}

color_t Bdisp_GetPoint_VRAM( int x, int y )
{
    if (x < 0 || x > LCD_WIDTH_PX - 1) return ColorWhite;
    if (y < 0 || y > LCD_HEIGHT_PX - 1) return ColorWhite;
    
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
    PrintCXY(_fxCG_Cursor.x * 18, _fxCG_Cursor.y * 24, msg, invers ? TextModeInvert : TextModeNormal, -1, ColorBlack, ColorWhite, 1, 0);
    _fxCG_Cursor.x = fxCG_Range(0, 20, _fxCG_Cursor.x + 1);
}

void Bdisp_MMPrintRef(int*x, int*y, unsigned char *s, int mode, int xmax, int P6, int P5, int color, int P9, int P10, int P11)
{
//    DrawText(*x, *y + 24, (const char*)s, color, FontSize_16pt);
}
void Bdisp_MMPrint(int x, int y, char* s, int mode, int xmax, int P6, int P7, int color, int backcolor, int P10, int P11)
{
//    DrawText(x, y + 24, (const char*)s, color, FontSize_16pt);
}

void PrintCXY( int x, int y, const char *text, int mode, int P5, int color, int bgcolor, int P8, int P9 )
{
    y += 24;
    if (mode & TextModeInvert) {
        uint16_t tmp = color;
        color = bgcolor;
        bgcolor = tmp;
    }
    TBdispFillArea area = {
        .x1 = x, .y1 = y, .x2 = x + getTextSize((unsigned char *)text, FontSize24pt) - 1, .y2 = y + 23
    };
    Bdisp_AreaClr(&area, FillAreaTargetVRAM, bgcolor);
    DrawText(x, y, text, color, FontSize24pt);
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

color_t FrameColor(TFrameMode mode, color_t color)
{
    if (mode == FrameModeSetToColor) {
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
    
    if (_fxCG_SAF & SAF_BATTERY)
        DrawImage(0,0,18,22,Battery);
    
    if (_fxCG_SAF & SAF_ALPHA_SHIFT) {
        if (_fxCG_KMI_Shift) {
            if (_fxCG_KMI_Alpha)
                DrawImage(18,0,24,22,Alpha+24*22*2);
            else
                DrawImage(18,0,24,22,Shift);
        } else {
            if (_fxCG_KMI_Alpha)
                DrawImage(18,0,24,22,Alpha);
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
    
    if (!(mode & TextModeTransparentBackground)) {
        TBdispFillArea area;
    
        area.mode = FillAreaModeColor;
        area.x1 = x;
        area.y1 = y;
        area.x2 = x + 17;
        area.y2 = y + 23;
        Bdisp_AreaClr(&area, FillAreaTargetVRAM, color);
    }

    if (mode & TextModeInvert) {
        uint16_t tmp = color;
        color = bgcolor;
        bgcolor = tmp;
    }
    
    DrawGlyph(x, y, glyph, color, FontSize24pt);
    
}

void*GetMiniGlyphPtr( unsigned short mb_glyph_no, unsigned short*glyph_info )
{
    return 0;
}
void PrintMiniGlyph(int x, int y, void *glyph, uint32_t mode, int glyph_width, int P6, int P7, int P8, int P9, int color, int bgcolor, int P12)
{
    DrawGlyph(x, y, (uint8_t *)glyph, color, FontSize18pt);
}
void PrintMini( int *x, int *y, const char *text, unsigned int mode, unsigned int xlimit, int P6, int P7, int color, int bgcolor, int writeflag, int P11 ) {

    int xx = *x, yy = *y;
    yy += (mode & 0x40 ? 0 : 24);
    
    if (mode & TextModeInvert) {
        uint16_t tmp = color;
        color = bgcolor;
        bgcolor = tmp;
    }
    if (!(mode & 0x2)) {
        TBdispFillArea area = {
            .x1 = *x, .y1 = *y, .x2 = *x + getTextSize((unsigned char *)text, FontSize24pt) - 1, .y2 = *y + 17
        };
        Bdisp_AreaClr(&area, FillAreaTargetVRAM, bgcolor);
    }
    
    if (mode & 0x1) {
        *x = DrawText(xx, yy, text, bgcolor, FontSize18pt);
        return;
    }
    *x = DrawText(xx, yy, text, color, FontSize18pt);
}
void PrintMiniMini( int *x, int *y, const char *text, int mode1, int color, int mode2 )
{
    if (mode1 & 0x1) {
        DrawText(*x, *y + 24, text, color, FontSize10pt);\
        return;
    }
    DrawText(*x, *y + 24, text, color, FontSize10pt);
    
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


//MARK: - Status Area Icon SYSCALLS
//NOTE: Status area icon syscalls: (it may be more appropriate to use the status area flags)

void DefineStatusGlyph(int index, TGlyphDefineRef glypth)
{
    
}

int DefineStatusAreaFlags(int mode, int flags, char* color, char* bgcolor)
{
    return 0;
}

void d_c_Icon(uint32_t s)
{
    
}
void BatteryIcon(uint32_t s)
{
    
}
void KeyboardIcon(uint32_t s)
{
    
}
void LineIcon(uint32_t s)
{
    
}
void NormIcon(uint32_t s)
{
    
}
void RadIcon(uint32_t s)
{
    
}
void RealIcon(uint32_t s)
{
    
}
