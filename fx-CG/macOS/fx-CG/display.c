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

#include "fx-CG.h"


void Bdisp_AreaClr( struct TBdispFill * area, unsigned char target, unsigned short color )
{
    color_t* VRAM = (color_t *)GetVRAMAddress();
    
    for (int y = area->y1; y < area->y2 && y < LCD_HEIGHT_PX; y++)
    {
        if (y < 0) continue;
        for (int x = area->x1; x < area->x2 && x < LCD_WIDTH_PX; x++)
        {
            if (x < 0) continue;
            VRAM[x + y * LCD_WIDTH_PX] = (unsigned short)color;
        }
    }
}

int Bdisp_EnableColor( ColorMode n )
{
    _fxCG_DDRegister.B = n == kColorMode_EnableFullColor ? 0 : 1;
    return 0;
}

/**
 [mode] Area of the VRAM to fill. When mode is 1, the area below the status area is filled; when mode is 2, the area between
 the status area and the F-key labels is filled;  and when mode is 3 or 4, the whole VRAM is filled.
 **/
void Bdisp_Fill_VRAM( int color, int mode ) {
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
    for (int y = 0; y < LCD_HEIGHT_PX; y++) {
        for (int x = 0; x < LCD_WIDTH_PX; x++) {
            color = Bdisp_GetPoint_VRAM(x, y);
            Bdisp_SetPoint_DD(6 + x, y, color);
        }
    }
}

void Bdisp_SetDDRegisterB(int colorMode)
{
    _fxCG_DDRegister.B = colorMode == kColorMode_EnableFullColor ? 0 : 1;
}

void Bdisp_SetPoint_DD( int x, int y, int color ) {
    if (x < 0 || x >= LCD_WIDTH_PX + 12 || y < 0 || y >= LCD_HEIGHT_PX + 8) return;
    _fxCG_DRAM[x + y * 396] = (unsigned short)color;
}

void Bdisp_SetPoint_VRAM( int x, int y, int color ) {
    if (x < 0 || x >= LCD_WIDTH_PX || y < 0 || y >= LCD_HEIGHT_PX) return;
    color_t* VRAM = (color_t *)GetVRAMAddress();
    
    if (_fxCG_DDRegister.B != 0) {
        switch (color) {
            case 1:
                color = kTextColor_Blue;
                break;
                
            case 2:
                color = kTextColor_Green;
                break;
                
            case 3:
                color = kTextColor_Cyan;
                break;
                
            case 4:
                color = kTextColor_Red;
                break;
                
            case 5:
                color = kTextColor_Magenta;
                break;
                
            case 6:
                color = kTextColor_Yellow;
                break;
                
            case 7:
                color = kTextColor_White;
                break;
        }
    }
    
    VRAM[x + y * LCD_WIDTH_PX] = (unsigned short)color;
}

color_t Bdisp_GetPoint_VRAM( int x, int y )
{
    if (x < 0 || x > LCD_WIDTH_PX - 1) return kColor_White;
    if (y < 0 || y > LCD_HEIGHT_PX - 1) return kColor_White;
    
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
    PrintCXY(_fxCG_Cursor.x * 18, _fxCG_Cursor.y * 24, msg, invers ? kTextModeFlags_Invert : kTextModeFlags_Normal, -1, kColor_Black, kColor_White, 1, 0);
    _fxCG_Cursor.x = fxCG_Range(0, 20, _fxCG_Cursor.x + 1);
}


void PrintCXY( int x, int y, const char *cptr, int mode_flags, int P5, int color, int back_color, int P8, int P9 )
{
    unsigned char * c = (unsigned char *)cptr;
    while (*c) {
        if (mode_flags == kTextModeFlags_Normal)
            PrintGlyph(x, y + 24, c, 0, color, back_color, 0);
        
        if (mode_flags == kTextModeFlags_Invert)
            PrintGlyph(x, y + 24, c, 0, back_color, color, 0);
        
        c++;
        x += 18;
    }
}


void PrintXY( int x, int y, const char *string, int mode, int color ) {
    
    PrintCXY(x, y, string, mode, 0, color, Bdisp_GetPoint_VRAM(x, y), 0, 0);
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

color_t FrameColor(FrameMode mode, color_t color)
{
    if (mode == kFrameMode_SetToColor) {
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

void PrintGlyph(int x, int y, unsigned char * glyph, int mode_flags, color_t color, color_t back_color, int P7)
{
    /// 24px and 18px
    unsigned char ascii = *glyph;
    if (ascii < '!' || ascii > '~') return;
    
    for (int row = 0; row < 24; row++)
    {
        int mask = 1 << 17;
        for (int n = 0; n < 18; n++)
        {
            Bdisp_SetPoint_VRAM(x + n, y + row, _fxCG_Glyphs[ascii - '!'].data[row] & mask ? color : back_color);
            mask >>= 1;
        }
    }
}

void*GetMiniGlyphPtr( unsigned short mb_glyph_no, unsigned short*glyph_info )
{
    return 0;
}
void PrintMiniGlyph(int x, int y, void*glyph, TextModeFlags mode_flags, int glyph_width, int P6, int P7, int P8, int P9, int color, int back_color, int P12)
{
    
}
void PrintMini( int *x, int *y, const char *MB_string, TextModeFlags mode_flags, unsigned int xlimit, int P6, int P7, int color, int back_color, int writeflag, int P11 ) {
//    if (mode_flags == kTextModeFlags_Normal)
//        
//    if (mode_flags == kTextModeFlags_Invert)
//       
//    
}
void PrintMiniMini( int *x, int *y, const char *MB_string, int mode1, int color, int mode2 )
{
    
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
    size_t length = FXCG_SCREEN_WIDTH * FXCG_LCD_HEIGHT / 4;
    do {
        *dest++ = *src++;
    } while (--length);
}

void LoadVRAM_1(void) {
    uint64_t *src = (uint64_t *)getSecondaryVramAddress();
    uint64_t *dest = (uint64_t *)_fxCG_VRAM;
    size_t length = FXCG_SCREEN_WIDTH * FXCG_LCD_HEIGHT / 4;
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
