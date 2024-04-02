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

unsigned short _fxCG_0xA44B0000[8]; // keyboard_register

// DRAM is RGB565 regardless of VRAM RGB565 or RGB111
uint16_t _fxCG_DRAM[ FXCG_LCD_WIDTH * FXCG_LCD_HEIGHT ];

uint16_t _fxCG_VRAM[ FXCG_SCREEN_WIDTH * FXCG_SCREEN_HEIGHT ];
bool _fxCG_StatusArea = true;
FXCG_Cursor _fxCG_Cursor;
FXCG_Glyph _fxCG_Glyphs[95];

// MARK: - Internal Private Functions


// MARK: - Internal Private Functions

void fxCG_KeyDown(int keyCode) {
    int row = keyCode % 10;
    int col = keyCode / 10 - 1;
    
    int word = row >> 1;
    int bit = col + 8 * ( row & 1 );
    _fxCG_0xA44B0000[word] |= 1 << bit;
}

void fxCG_KeyUp(int keyCode) {
    int row = keyCode % 10;
    int col = keyCode / 10 - 1;
    
    int word = row >> 1;
    int bit = col + 8 * ( row & 1 );
    _fxCG_0xA44B0000[word] &= ~(1 << bit);
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

static FXCG_Image * fxCG_ImageWithContentsOfPBMFile(const char *file)
{
    FILE *fp = fopen(file, "rb");
    if (fp == NULL) return NULL;
    
    FXCG_Image * pImage = malloc(sizeof(FXCG_Image));
    if (!pImage) {
        fclose(fp);
        return NULL;
    }
    
    fseek(fp, 3, SEEK_SET);
    char *line;
    size_t len;
    
    line = fgetln(fp, &len);
    if (!line) {
        fclose(fp);
        free(pImage);
        return NULL;
    }
    // width
    *(line + (len - 1)) = '\0';
    pImage->width = atoi(line);
    
    
    line = fgetln(fp, &len);
    if (!line) {
        fclose(fp);
        free(pImage);
        return NULL;
    }
    // height
    *(line + (len - 1)) = '\0';
    pImage->height = atoi(line);

    if (pImage->width % 8 != 0) {
        pImage->length = (pImage->width + (8 - (pImage->width % 8))) * pImage->height / 8;
    }
    else
    {
        pImage->length = pImage->width * pImage->height / 8;
    }
    
    pImage->bytes = malloc(pImage->length);
    if (!pImage->bytes) {
        fclose(fp);
        free(pImage);
        return NULL;
    }
    
    fread(pImage->bytes, sizeof(char), pImage->length, fp);
    
    
    fclose(fp);
    return pImage;
}

static uint16_t fxCG_GetPixelFromImage(int x, int y, FXCG_Image * image)
{
    uint8_t *ptr = image->bytes;
    int n = x & 7;
    int srcw;
    
    if (image->width % 8 != 0) {
        srcw = (image->width + (8 - (image->width % 8))) / 8;
    }
    else
    {
        srcw = image->width / 8;
    }
    
    int mask = 0x80 >> n;
    uint8_t byte = ptr[( x / 8 ) + ( y * srcw )];
    if (byte & mask) return 1;
    
    return 0;
}

void fxCG_Load18x24CharactorSet( const char * file )
{
    FXCG_Image * pImage;
    pImage = fxCG_ImageWithContentsOfPBMFile(file);
    
    int glyph = 0; // ! first glyph
    
    if (pImage != NULL)
    {
        for (int y = 0; y < pImage->height; y += 24)
        {
            for (int x = 0; x < pImage->width; x += 18) {
                // Glyphs 18 x 24
                for (int row = 0; row < 24; row++)
                {
                    _fxCG_Glyphs[glyph].data[row] = 0b11111111111111000000000000000000;
                    int mask = 0b00000000000000100000000000000000;
                    for (int n = 0; n < 18; n++)
                    {
                        if (fxCG_GetPixelFromImage(x + n, y + row, pImage) == 1)
                        {
                            _fxCG_Glyphs[glyph].data[row] |= mask;
                        }
                        mask >>= 1;
                    }
                }
                glyph++;
            }
        }
        free(pImage->bytes);
        free(pImage);
    }
}
