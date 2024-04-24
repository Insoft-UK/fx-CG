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
#include "ratio.hpp"



void RatioC(void)
{
    char str[4] = "1";
    int x[3] = {0, 90, 306};
    float val[3];
    int sel=0;
    
    fx::String s("hello");
    
    fxPrint(0, 0, s.string(), FXCGColorBlack, FXCGColorWhite);

    atoi(str);
    
    fxFillArea(0, 24, LCD_WIDTH_PX, LCD_HEIGHT_PX - 24 * 2, FXCGColorWhite);
    fxPrint(0, 48, "   0:   0 =    \x90:   0", FXCGColorBlack, FXCGColorWhite);
    for (int n=0; n<3; n++)
        fxDrawLine(x[n], 71, x[n] + 71, 71, FXCGColorBlack);
    
    do {
        fxPrint(x[sel], 48, "   0", FXCGColorWhite, FXCGColorBlack);
        val[sel] = (float)IntInput(7, 4);
        
        IntToString((int)val[sel], str);
        fxPrint(x[sel], 48, "    ", FXCGColorBlack, FXCGColorWhite);
        fxPrint((x[sel] + 72) - StrLength(str) * 18, 48, str, FXCGColorBlack, FXCGColorWhite);
        fxDrawLine(x[sel], 71, x[sel] + 71, 71, FXCGColorBlack);
    } while (++sel < 3);
    
    float ans = val[0] / val[1] * val[2];
    IntToString((int)ans, str);
    fxPrint(0, 168, "\x90=", FXCGColorBlack, FXCGColorWhite);
    fxPrint(LCD_WIDTH_PX - 18 * StrLength(str), 168, str, FXCGColorBlack, FXCGColorWhite);
    Bdisp_PutDisp_DD();
    
    
    while (true) {
        fxKeyUpdate();
        if (fxIsKeyPressed(KeyCode_Return)) return;
        
        OS_InnerWait_ms(100);
    }
}

void RatioD(void)
{
    char str[4];
    int x[3] = {0, 90, 216};
    float val[3];
    int sel=0;
    
    fxFillArea(0, 24, LCD_WIDTH_PX, LCD_HEIGHT_PX - 24 * 2, FXCGColorWhite);
    fxPrint(0, 48, "   0:   0 =    0:   \x90", FXCGColorBlack, FXCGColorWhite);
    for (int n=0; n<3; n++)
        fxDrawLine(x[n], 71, x[n] + 71, 71, FXCGColorBlack);
    
    do {
        fxPrint(x[sel], 48, "   0", FXCGColorWhite, FXCGColorBlack);
        val[sel] = (float)IntInput(7, 4);
        
        IntToString((int)val[sel], str);
        fxPrint(x[sel], 48, "    ", FXCGColorBlack, FXCGColorWhite);
        fxPrint((x[sel] + 72) - StrLength(str) * 18, 48, str, FXCGColorBlack, FXCGColorWhite);
        fxDrawLine(x[sel], 71, x[sel] + 71, 71, FXCGColorBlack);
    } while (++sel < 3);
    
    float ans = val[1] / val[0] * val[2];
    IntToString((int)ans, str);
    fxPrint(0, 168, "\x90=", FXCGColorBlack, FXCGColorWhite);
    fxPrint(LCD_WIDTH_PX - 18 * StrLength(str), 168, str, FXCGColorBlack, FXCGColorWhite);
    Bdisp_PutDisp_DD();
    
    
    while (true) {
        fxKeyUpdate();
        if (fxIsKeyPressed(KeyCode_Return)) return;
        
        OS_InnerWait_ms(100);
    }
}

class Ratio {
public:
    static void init(void)
    {
        fxEnableColor();
        fxKeyReset();
    }
};

class UI {
public:
    static void inputRealNumber(void)
    {
        fxEnableColor();
        fxKeyReset();
    }
};

// MARK: - CASIO fxCG Add-In Application "main" Function
int fxCG_g3a(void)
{
    // Switches the screen to full color mode (16 bits per pixel, RGB565)
    fxEnableColor();
    fxKeyReset();
    
    Ratio::init();
    
    while (true) {
        fxFillArea(0, 24, LCD_WIDTH_PX, LCD_HEIGHT_PX - 24, FXCGColorWhite);
        
        fxPrint(0, 24, "Ratio", FXCGColorBlue, FXCGColorWhite);
        fxPrint(0, 48, "F1: A:B=\x90:D", FXCGColorBlack, FXCGColorWhite);
        fxPrint(0, 72, "F2: A:B=C:\x90", FXCGColorBlack, FXCGColorWhite);
        
        fxSubPlot(16*3+2, 97, 0);
        fxSubPlot(16*3+1, 97, 0);
        
        fxKeyUpdate();
        if (fxIsKeyPressed(KeyCode_Menu)) {
            return 0;
        }
        if (fxIsKeyPressed(KeyCode_F1))
            RatioC();
        if (fxIsKeyPressed(KeyCode_F2))
            RatioD();
        
        
        Bdisp_PutDisp_DD();
        OS_InnerWait_ms(10);
    }
    
    return 0;
}

