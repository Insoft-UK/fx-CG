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
#include "keyboard.h"
#include "graphics.h"
#include "font.h"

#include <string.h>

//extern void *FontBytes(void);
//
//typedef enum FnType {
//    kFnType_Item     = 0,
//    kFnType_Menu     = 1,
//    kFnType_Action   = 2
//} FnType;
//
//void UI_Fn(int n, const char *s, FnType type) {
//    if (n < 1 || n > 6) return;
//    n = (n - 1) << 6;
//    
//    switch (type) {
//        case kFnType_Menu:
//            fillRoundRect(n + 1, LCD_HEIGHT_PX - 23, 62, 22, 1, kColor_Black);
//            fillTriangle(n + 57, LCD_HEIGHT_PX - 2, n + 62, LCD_HEIGHT_PX - 2, n + 62, LCD_HEIGHT_PX - 7, kColor_White);
//            break;
//            
//        case kFnType_Item:
//            fillArea(n + 1, LCD_HEIGHT_PX - 23, 62, 22, kColor_Black);
//            break;
//            
//        case kFnType_Action:
//            fillRoundRect(n + 1, LCD_HEIGHT_PX - 23, 62, 22, 1, kColor_Black);
//            fillArea(n + 3, LCD_HEIGHT_PX - 21, 58, 18, kColor_White);
//            break;
//            
//        default:
//            break;
//    }
//    
//    int x = n + 32 - (18 * (int)strlen(s) / 2);
//    int y = 22;
//    PrintCXY(x, y, s, kTextModeFlags_Normal, 0, kColor_White, kColor_Black, 0, 0);
//   
//}

// MARK: - CASIO fxCG Add-In Application "main" Function
int fxCG_g3a(void)
{
    
    
//    SetFont(FontBytes());
    
    /// Switches the screen to full color mode (16 bits per pixel, RGB565)
    Bdisp_EnableColor(1);
    fillArea(0, 24, LCD_WIDTH_PX, 168, kColor_White);
    
    bool running = true;
    while (running) {
        keyUpdate();

        if (isKeyUp(KeyCode_Exit)) {
            fillArea(0, 22, LCD_WIDTH_PX, 22, kColor_White);
            PrintCXY(0, 22, "[Return] KeyUp", kTextModeFlags_Normal, 0, kColor_Black, kColor_White, 0, 0);
        }
        
        if (isKeyDown(KeyCode_Exit)) {
            fillArea(0, 22, LCD_WIDTH_PX, 22, kColor_White);
            PrintCXY(0, 22, "[Return] KeyDown", kTextModeFlags_Normal, 0, kColor_Black, kColor_White, 0, 0);
        }
        
        if (isKeyPressed(KeyCode_Exit)) {
            fillArea(0, 22, LCD_WIDTH_PX, 22, kColor_White);
            PrintCXY(0, 22, "[Return] KeyPressed", kTextModeFlags_Normal, 0, kColor_Black, kColor_White, 0, 0);
        }
        
        if (isKeyReleased(KeyCode_Exit)) {
            fillArea(0, 22, LCD_WIDTH_PX, 22, kColor_White);
            PrintCXY(0, 22, "[Return] KeyReleased", kTextModeFlags_Normal, 0, kColor_Black, kColor_White, 0, 0);
        }

        
        if (isKeyDown(KeyCode_Menu)) {
            running = false;
        }
        
        Bdisp_PutDisp_DD();
    }
    return 0;
}

