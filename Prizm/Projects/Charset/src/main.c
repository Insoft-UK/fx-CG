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

#define clearStatus()   fxFillArea(0, 0, LCD_WIDTH_PX, 22, kColor_White)



int page = 0;

void drawCharset(int size, uint8_t code)
{
    char multi[3] = "\x00\x20\x00";
    int m = 19;
    int offset = 0;
    
    fxFillArea(0, 24, LCD_WIDTH_PX, 168, kColor_White);
    
    if (size == 1) {
        if (page) {
            offset = 14;
            fxFillArea(0, 24, 18 * offset, 24, kColor_Red);
        }
    }
    
    if (size == 2) {
        m = LCD_WIDTH_PX / 18;
        if (page) {
            offset = 2;
            fxFillArea(0, 24, 36, 18, kColor_Red);
        }
    }
    
    if (size == 3) m = LCD_WIDTH_PX / 16;
    if (size == 4) m = LCD_WIDTH_PX / 10;
    
    
    
    for (int i=0; i<( size < 3 ? 128 : 258); i++) {
        if (i == 127 && page==0) continue;
        int c = ((i + offset) % m);
        int r = ((i + offset) / m);
        
        if (code < 127) {
            multi[0] = (char)i;
            if (page && size < 3) multi[0] += 128;
            multi[1] = 0;
        } else {
            multi[0] = code;
            multi[1] = (char)i;
            if (page && size < 3) multi[1] += 128;
            multi[2] = 0;
        }
        
        if (size == 1)
            PrintCXY(c * 18, r * 24, multi, kTextModeFlags_Normal, -1, kColor_Black, kColor_White, 1, 0 );
        
        
        if (size == 2) {
            int x = c * 18;
            int y = r * 18;
            PrintMini(&x, &y, multi, 0,  0xFFFFFFFF , 0, 0, kColor_Black, kColor_White, 1, 0);
        }
        
        if (size == 3)
            Bdisp_MMPrint(c * 16, r * 16, multi, 0,  0xFFFFFFFF , 0, 0, kColor_Black, kColor_White, 1, 0);
        
        if (size == 4) {
            int x = c * 10;
            int y = r * 10;
            PrintMiniMini(&x, &y, multi, 0, 0, 0);
        }
    }
}

void drawBinary(uint16_t byte)
{
    clearStatus();
    int x = LCD_WIDTH_PX - 14 + 1, y = LCD_HEIGHT_PX - 14;
    for (int n=0; n<8; n++) {
        fxFillArea(x, y, 12, 14, kColor_Black);
        fxDrawRect(x + 2, y + 2, 8, 10, kColor_White);
        if (!(byte & (1 << n)))
            fxFillArea(x + 3, y + 3, 6, 8, kColor_White);
        x -= 14;
    }
}

void displayCharset(int size, uint8_t code)
{
    fxFillArea(0, LCD_HEIGHT_PX - 22, LCD_WIDTH_PX / 4, 22, kColor_White);
    switch (size) {
        case 1:
            fxPrintMini(0, LCD_HEIGHT_PX - 16, "24pt", 0);
            break;
            
        case 2:
            fxPrintMini(0, LCD_HEIGHT_PX - 16, "18pt", 0);
            break;
            
        case 3:
            fxPrintMini(0, LCD_HEIGHT_PX - 16, "16pt", 0);
            break;
            
        case 4:
            fxPrintMini(0, LCD_HEIGHT_PX - 16, "10pt", 0);
            break;
            
        default:
            fxPrintMini(0, LCD_HEIGHT_PX - 16, "?pt", 0);
            break;
    }
    
    drawCharset(size, code);
}


// MARK: - CASIO fxCG Add-In Application "main" Function
int fxCG_g3a(void)
{
    // Switches the screen to full color mode (16 bits per pixel, RGB565)
    Bdisp_EnableColor(1);
    
    uint8_t code = 0x0;
    int size = 1;
    
    drawBinary(code);
    displayCharset(size, code);
    Bdisp_PutDisp_DD();
    
    
    while (true) {
        fxKeyUpdate();
        
        if (fxIsKeyPressed(KeyCode_F1)) {
            code = 0;
            drawBinary(code);
            displayCharset(size, code);
            Bdisp_PutDisp_DD();
        }
        
        if (fxIsKeyPressed(KeyCode_F2)) {
            code = 0x7F;
            drawBinary(code);
            displayCharset(size, code);
            Bdisp_PutDisp_DD();
        }
        
        if (fxIsKeyPressed(KeyCode_F3)) {
            code = 0xF9;
            drawBinary(code);
            displayCharset(size, code);
            Bdisp_PutDisp_DD();
        }
        
        if (fxIsKeyPressed(KeyCode_F4)) {
            code = 0xE5;
            drawBinary(code);
            displayCharset(size, code);
            Bdisp_PutDisp_DD();
        }
        
        if (fxIsKeyPressed(KeyCode_F5)) {
            code = 0xE6;
            drawBinary(code);
            displayCharset(size, code);
            Bdisp_PutDisp_DD();
        }
        
        
        
        if (fxIsKeyPressed(KeyCode_Minus)) {
            code--;
            drawBinary(code);
            displayCharset(size, code);
            Bdisp_PutDisp_DD();
        }
        
        if (fxIsKeyPressed(KeyCode_Add)) {
            code++;
            drawBinary(code);
            displayCharset(size, code);
            Bdisp_PutDisp_DD();
        }
        
        if (fxIsKeyPressed(KeyCode_Left) && size > 1) {
            size--;
            displayCharset(size, code);
            Bdisp_PutDisp_DD();
        }
        
        if (fxIsKeyPressed(KeyCode_Right) && size < 4) {
            size++;
            displayCharset(size, code);
            Bdisp_PutDisp_DD();
        }
        
        if (fxIsKeyPressed(KeyCode_Up)) {
            page=0;
            displayCharset(size, code);
            Bdisp_PutDisp_DD();
        }
        
        if (fxIsKeyPressed(KeyCode_Down)) {
            page=1;
            displayCharset(size, code);
            Bdisp_PutDisp_DD();
        }
        
        
        
        if (fxIsKeyPressed(KeyCode_Return)) {
            TakeScreenshot();
            displayCharset(size, code);
            Bdisp_PutDisp_DD();
        }
        
        
        if (fxIsKeyPressed(KeyCode_Menu)) {
            fxKeyUpdate();
            break;
        }
        
        OS_InnerWait_ms(100000);
        
    }
    
    return 0;
}

