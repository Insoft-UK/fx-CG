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


#include <unistd.h>

#include "fxcg/keyboard.h"
#include "fxcg/display.h"

// MARK: - macOS Keyboard Simulator Functions

static int keyPressed(void)
{
    extern uint16_t _fxCG_0xA44B0000[5];
    
    uint8_t *pressed = (uint8_t *)_fxCG_0xA44B0000;
    int row;
    for (row = 0; row <= 9; row++) {
        if (*pressed) {
            // Key pressed ?
            for (int col=0; col<=7; col++) {
                if (*pressed & (1 << col)) {
                    return ((col + 1) * 10 + row);
                }
            }
            return 0;
        }
        pressed++;
    }
    return 0;
}

int GetKey( int *key )
{
    Bdisp_PutDisp_DD();
    DisplayStatusArea();
    
    while (1)
    {
//        if (_fxCG_0xA44B0000 != KEY_CTRL_NOP)
//        {
//            
//            _fxCG_0xA44B0000 = KEY_CTRL_NOP;
//            break;
//        }
        usleep(1000);
    }
    return 0;
}

