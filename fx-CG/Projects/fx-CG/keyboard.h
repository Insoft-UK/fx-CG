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

#ifndef keyboard_h
#define keyboard_h

#include <stdbool.h>

typedef enum KeyCode {
    KeyCode_F1    = 79, KeyCode_F2       = 69, KeyCode_F3     = 59, KeyCode_F4    = 49, KeyCode_F5     = 39, KeyCode_F6    = 29,
    KeyCode_Shift = 78, KeyCode_Optn     = 68, KeyCode_Vars   = 58, KeyCode_Menu  = 48, KeyCode_Left   = 38, KeyCode_Up    = 28,
    KeyCode_Alpha = 77, KeyCode_Sq       = 67, KeyCode_Power  = 57, KeyCode_Exit  = 47, KeyCode_Down   = 37, KeyCode_Right = 27,
    KeyCode_XAngT = 76, KeyCode_Log      = 66, KeyCode_Ln     = 56, KeyCode_Sin   = 46, KeyCode_Cos    = 36, KeyCode_Tan   = 26,
    KeyCode_Abc   = 75, KeyCode_SwapDisp = 65, KeyCode_CubeRt = 55, KeyCode_InvX  = 45, KeyCode_Comma  = 35, KeyCode_Ans   = 25,
    KeyCode_7     = 74, KeyCode_8        = 64, KeyCode_9      = 54, KeyCode_Del   = 44, KeyCode_On     = 10,
    KeyCode_4     = 73, KeyCode_5        = 63, KeyCode_6      = 53, KeyCode_Mult  = 43, KeyCode_Div    = 33,
    KeyCode_1     = 72, KeyCode_2        = 62, KeyCode_3      = 52, KeyCode_Add   = 42, KeyCode_Minus  = 32,
    KeyCode_0     = 71, KeyCode_Dot      = 61, KeyCode_Exp    = 51, KeyCode_Neg   = 41, KeyCode_Return = 31
} KeyCode;


void keyUpdate(void);
bool isKeyHeld(KeyCode keyCode);
bool isKeyPressed(KeyCode keyCode);
bool isKeyReleased(KeyCode keyCode);

#endif /* keyboard_h */
