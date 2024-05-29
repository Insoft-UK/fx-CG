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

#ifndef fxCG_hpp
#define fxCG_hpp

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef __clang__
#define g3a main
#include <fxcg/display.h>
#include <fxcg/system.h>
#include <fxcg/heap.h>
#include <fxcg/file.h>
#else
#include "fxcg/display.h"
#include "fxcg/system.h"
#include "fxcg/heap.h"
#include "fxcg/file.h"
/**
 The CASIO Add-In main function.
*/
extern "C" int g3a(void);
#endif

#define Bfile_OpenFile(filename, mode) Bfile_OpenFile_OS(filename, mode, 0)
#define Bfile_CloseFile Bfile_CloseFile_OS
#define Bfile_GetFileSize Bfile_GetFileSize_OS
#define Bfile_GetMediaFree Bfile_GetMediaFree_OS
#define Bfile_CreateEntry Bfile_CreateEntry_OS
#define Bfile_ReadFile Bfile_ReadFile_OS
#define Bfile_WriteFile Bfile_WriteFile_OS
#define Bfile_TellFile Bfile_TellFile_OS
#define Bfile_SeekFile Bfile_SeekFile_OS

#define updateDisplay Bdisp_PutDisp_DD
#define wait OS_InnerWait_ms

#ifdef __clang__
#define sys_calloc calloc
#define sys_malloc malloc
#define sys_realloc realloc
#define sys_free free
#endif

namespace fxCG {

enum Screen : uint16_t {
    width = 384,
    height = 216
};

enum Color : color_t {
    black   = 0,
    blue    = 0x001F,
    green   = 0x07E0,
    cyan    = 0x07FF,
    red     = 0xF800,
    magenta = 0xF81F,
    yellow  = 0xFFE0,
    white   = 0xFFFF
};

void enableColor();
void clearDisplay(color_t color);

/**
 @brief    Returns a color in RGB 565 format from a given RGB[0-255] value.
 @param    r  Red channel
 @param    g  Green channel
 @param    b  Blue channel
 */
color_t color(uint8_t r, uint8_t g, uint8_t b);

/**
 @brief    Returns a color in RGB 565 format based on a gray value.
 @param    shade  Shade which can be between 0 (black) and 31 (white)
 */
color_t gray(int shade);

/**
 @brief    Change the speed of the operating PLL circuit.
 
 Notes: PLEASE before handling MENU keypresses in your add-in, use PLL_16x to go back to normal operating speed, at the courtesy of other applications and the OS. Feeding a value not defined below (AKA, an invalid value), will cause a crash.
 */
void changeFreq(int mult);
}

#endif /* fxCG_hpp */
