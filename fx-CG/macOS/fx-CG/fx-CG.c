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

#include "fx-CG.h"

FXCG_DDRegister _fxCG_DDRegister = {
    .B = 1 /// 1 = Enable 8 color mode
};
uint16_t _fxCG_0xFD801460 = 0xFFFF;

uint16_t _fxCG_0xA44B0000[5] = {0,0,0,0,0}; // keyboard_register

// DRAM is RGB565 regardless of VRAM RGB565 or RGB111
uint16_t _fxCG_DRAM[ FXCG_LCD_WIDTH * FXCG_LCD_HEIGHT ];

uint16_t _fxCG_VRAM[ FXCG_SCREEN_WIDTH * FXCG_SCREEN_HEIGHT ];
bool _fxCG_StatusArea = true;
FXCG_Cursor _fxCG_Cursor;

// MARK: - Internal Private Functions

uint16_t *fxCGGetVRAM(void)
{
    return _fxCG_VRAM;
}

// MARK: - Internal Private Functions

void fxCG_KeyDown(int keyCode) {
    int row = keyCode % 10;
    int col = keyCode / 10 - 1;
    
    int word = row >> 1;
    int bit = col + 8 * ( row & 1 );
    _fxCG_0xA44B0000[word] |= 1 << bit;
    
#ifdef DEBUG
    printf("keyboard reg:- ");
    for (int i=0; i<4; i++) {
        for (int n=15; n>=0; n--) {
            printf(_fxCG_0xA44B0000[i] & 1 << n ? "1" : "0");
        }
        if (i != 3) printf(" : ");
    }
    printf("\n");
#endif
}

void fxCG_KeyUp(int keyCode) {
    int row = keyCode % 10;
    int col = keyCode / 10 - 1;
    
    int word = row >> 1;
    int bit = col + 8 * ( row & 1 );
    _fxCG_0xA44B0000[word] &= ~(1 << bit);
    
#ifdef DEBUG
    printf("keyboard reg:- ");
    for (int i=0; i<4; i++) {
        for (int n=15; n>=0; n--) {
            printf(_fxCG_0xA44B0000[i] & 1 << n ? "1" : "0");
        }
        if (i != 3) printf(" : ");
    }
    printf("\n");
#endif
}

int fxCG_Range(int min, int max, int value) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

typedef enum Channel{
    kChannel_Red, kChannel_Green, kChannel_Blue
} Channel;
static float fxCG_GetChannel(Channel c, uint16_t color) {
    float channel = 0.0f;
    
    switch (c) {
        case kChannel_Red:
            return (float)(color >> 11) / 31.0f;
            break;
            
        case kChannel_Green:
            return (float)((color >> 5) & 63) / 63.0f;
            break;
            
        case kChannel_Blue:
            return (float)(color & 31) / 31.0f;
            break;
    }
    return channel;
}

static uint32_t fxCG_ConvertHighColorToTrueColor(uint16_t color) {
    float red, grn, blu;
    uint32_t trueColor;
    
    red = fxCG_GetChannel(kChannel_Red, color);
    grn = fxCG_GetChannel(kChannel_Green, color);
    blu = fxCG_GetChannel(kChannel_Blue, color);
    
    trueColor = (uint32_t)(red * 255.0);
    trueColor |= (uint32_t)(grn * 255.0) << 8;
    trueColor |= (uint32_t)(blu * 255.0) << 16;
    trueColor |= 0xFF000000;
    
    return trueColor;
}

void fxCG_DisplayRedraw( void *pixelData )
{
    uint32_t *disp = (uint32_t *)pixelData;
    uint32_t trueColor;
    uint16_t highColor;
    
    for (int y = 0; y < FXCG_LCD_HEIGHT; y++) {
        for (int x = 0; x < FXCG_LCD_WIDTH; x++) {
            highColor = _fxCG_DRAM[x + y * FXCG_LCD_WIDTH];
            trueColor = fxCG_ConvertHighColorToTrueColor(highColor);
            disp[x + (223 - y) * FXCG_LCD_WIDTH] = trueColor;
        }
    }
}



