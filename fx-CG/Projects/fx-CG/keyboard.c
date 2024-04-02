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

#include "keyboard.h"

#include <string.h>

/*
 The Casio fx-CG50, like many calculators and embedded systems, handles key presses 
 at a low level through direct memory access (DMA) to a specific memory-mapped register.
 Each key on the calculator corresponds to a specific bit or set of bits within this
 register.
 
 When a key is pressed, the corresponding bit or bits in the register are set
 to indicate the key press event. Conversely, when a key is released, the corresponding
 bit or bits are cleared to indicate the key release event.
 
                                                                 Row : 16bit (reg)
 +-------------------------------------------------------------+
 |   [F1]    [F2]    [F3]    [F4]    [F5]    [F6]              | 9 = (4)
 |                                                             |
 |   [SHIFT] [OPTN] [VARS] [MENU]      [ UP ]                  | 8 = (4)
 |                                [LEFT]  [RIGHT]              |
 |   [ALPHA] [X^2]  [x√]   [EXIT]      [DOWN]                  | 7 = (3)
 |                                                             |
 |   [X,T,θ,n]   [LOG]  [LN]  [SIN]  [COS]  [TAN]              | 6 = (3)
 |                                                             |
 |   [abc] [S<>D]      [(]   [)]   [,]     [STOR]              | 5 = (2)
 |                                                             |
 |   [7]   [8]   [9]               [DEL]                       | 4 = (2)
 |                                                             |
 |   [4]   [5]   [6]               [x]        [/]              | 3 = (1)
 |                                                             |
 |   [1]   [2]   [3]               [+]        [-]              | 2 = (1)
 |                                                             |
 |   [0]   [.]               [PI]  [(-)]    [EXE]              | 1 = (0)
 |                                                             |
 |                                                   [AC/ON]   | 0 = (0)
 +-------------------------------------------------------------+
 Col  7       6       5       4       3       2       1
 
 (?) = Row / 2
    (0)      (1)      (2)      (3)
 [15...0] [15...0] [15...0] [15...0]
 
 [Bit] =  Col + 8 * (row & 1)
 */

#ifdef __clang__
#include "fx-CG.h"
static const unsigned short *keyboardRegister = _fxCG_0xA44B0000;
#else
static const unsigned short *keyboardRegister = (unsigned short*)0xA44B0000;
#endif

static unsigned short lastKey[8];
static unsigned short holdKey[8];

void keyUpdate(void) {
    memcpy(holdKey, lastKey, sizeof(unsigned short)*8);
    memcpy(lastKey, keyboardRegister, sizeof(unsigned short)*8);
}

/**
 @brief    Returns 1 if key has been pressed during last keyUpdate() call, 0 otherwise.
 @param    keyCode The fx-CGxx key code.
 */
int keyDownLast(KeyCode keyCode) {
    int row = keyCode % 10;
    int col = keyCode / 10 - 1;
    
    int word = row >> 1;
    int bit = col + 8 * ( row & 1 );
    
    if ((lastKey[word] & 1<<bit) != 0) return 1;
    return 0;
}

/**
 @brief    Returns 1 if key has been pressed two keyUpdate() calls ago, 0 otherwise.
 @param    keyCode  The fx-CGxx key code.
 */
int keyDownHold(KeyCode keyCode) {
    int row = keyCode % 10;
    int col = keyCode / 10 - 1;
    
    int word = row >> 1;
    int bit = col + 8 * ( row & 1 );
    
    if ((lastKey[word] & 1<<bit) != 0) return 1;
    return 0;
}

/**
 @brief    Returns true if key has been held down for a while.
 @param    keyCode  The fx-CGxx key code.
 */
bool isKeyDown(KeyCode keyCode) {
    return (keyDownLast(KeyCode_Menu) && keyDownHold(KeyCode_Menu));
}

/**
 @brief    Returns true if key has NOT been held down for a while.
 @param    keyCode  The fx-CGxx key code.
 */
bool isKeyUp(KeyCode keyCode) {
    return (!keyDownLast(KeyCode_Menu) && keyDownHold(KeyCode_Menu));
}

/**
 @brief    Returns true if key has just been pressed.
 @param    keyCode  The fx-CGxx key code.
 */
bool isKeyPressed(KeyCode keyCode) {
    return (keyDownLast(KeyCode_Menu) && !keyDownHold(KeyCode_Menu));
}

/**
 @brief    Returns true if key has just been released
 @param    keyCode  The fx-CGxx key code.
 */
bool isKeyReleased(KeyCode keyCode) {
    return (!keyDownLast(KeyCode_Menu) && keyDownHold(KeyCode_Menu));
}
