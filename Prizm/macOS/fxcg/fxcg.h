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

#ifndef fxcg_h
#define fxcg_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fxcg/display.h"
#include "fxcg/keyboard.h"
#include "fxcg/system.h"
#include "fxcg/serial.h"
#include "fxcg/rtc.h"
#include "fxcg/misc.h"
#include "fxcg/registers.h"
#include "fxcg/heap.h"
#include "fxcg/file.h"
#include "fxcg/app.h"


#pragma pack(1)     /* set alignment to 1 byte boundary */
typedef struct FXCG_Cursor {
    int x;  /// x Must be in range of [0,21]
    int y;  /// y Must be in range
} FXCG_Cursor;

typedef struct FXCG_DDRegister {
    uint16_t B : 1;
} FXCG_DDRegister;
#pragma pack()   /* restore original alignment from stack */

extern uint16_t _fxCG_SAF;
extern bool _fxCG_KMI_Shift;
extern bool _fxCG_KMI_Alpha;
extern bool _fxCG_KMI_Clip;

extern uint16_t _fxCG_0xFD801460;
extern uint16_t _fxCG_0xA44B0000[6];

extern FXCG_DDRegister _fxCG_DDRegister;
extern uint16_t _fxCG_DRAM[ 396 * 224 ];
extern uint16_t _fxCG_VRAM[ 384 * 216 ];
extern bool _fxCG_StatusArea;
extern FXCG_Cursor _fxCG_Cursor;

extern void (*_callback)(void);

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif



uint16_t *fxCGGetVRAM(void);
int fxCG_Range(int min, int max, int value);
void fxCG_DisplayRedraw(void *pixelData);
void fxCG_KeyDown(int keyCode);
void fxCG_KeyUp(int keyCode);

/**
 The CASIO Add-In main function.
*/
int g3a(void);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif


#endif /* fxcg_h */
