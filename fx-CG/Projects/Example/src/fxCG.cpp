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

#include "fxCG.hpp"

void fxCG::enableColor()
{
    Bdisp_EnableColor(1);
}

void fxCG::disableColor()
{
    Bdisp_EnableColor(0);
}

void fxCG::clearDisplay(color_t color)
{
    uint16_t *VRAM = (uint16_t*)GetVRAMAddress();
    int length = 82944;
    while(length--){
        *VRAM++ = color;
    }
}

void fxCG::updateDisplay()
{
    Bdisp_PutDisp_DD();
}

color_t fxCG::color(uint8_t r, uint8_t g, uint8_t b)
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

void fxCG::changeFreq(int mult)
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

void fxCG::wait(int ms)
{
    OS_InnerWait_ms(ms);
}
