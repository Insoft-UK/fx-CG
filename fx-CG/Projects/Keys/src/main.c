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
#include "graphics.h"
#include "keyboard.h"

#ifdef __clang__
#include "fx-CG.h"
static const unsigned short *keyboardRegister = _fxCG_0xA44B0000;
#else
static const unsigned short *keyboardRegister = (unsigned short*)0xA44B0000;
#endif


void Draw(void) {
    int x,y;
    
    fillArea(0, 24, LCD_WIDTH_PX, LCD_HEIGHT_PX - 48, 0xFFFF);
    
    for (int r=0; r<5; r++) {
        for (int n=15; n>=0; n--) {
            x = 24 * n;
            y = 20 * r + 24;
            
            
            if (keyboardRegister[r] & 1 << n) {
                // 1
                fillArea(x, y, 24, 16, 0);
            } else {
                // 0
                drawRect(x, y, 24, 16, 0);
            }
        }
    }
    
   
    Bdisp_PutDisp_DD();
}


// MARK: - CASIO fxCG Add-In Application "main" Function
int fxCG_g3a(void) {
    // Switches the screen to full color mode (16 bits per pixel, RGB565)
   
    while (true) {
        Draw();
        keyUpdate();
        
        if (isKeyPressed(KeyCode_Menu))
            break;
    }
    
    return 0;
}

