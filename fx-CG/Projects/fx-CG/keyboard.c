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
 
                                                                 Row : 16-bit (reg 0-4)
     +-----------------------------------------------------------------------+
     |   [F1]      [F2]      [F3]      [F4]      [F5]      [F6]              | 9 = (4)
     |                                                                       |
     |   [SHIFT]   [OPTN]    [VARS]    [MENU]    [LEFT]    [ UP ]            | 8 = (4)
     |                                                                       |
     |   [ALPHA]   [X^2]     [x√]      [EXIT]    [DOWN]    [RIGHT]           | 7 = (3)
     |                                                                       |
     |   [X,T,θ,n] [LOG]     [LN]      [SIN]     [COS]     [TAN]             | 6 = (3)
     |                                                                       |
     |   [abc]     [S<>D]    [(]       [)]       [,]       [-->]             | 5 = (2)
     |                                                                       |
     |   [7]       [8]       [9]       [DEL]                                 | 4 = (2)
     |                                                                       |
     |   [4]       [5]       [6]       [x]       [/]                         | 3 = (1)
     |                                                                       |
     |   [1]       [2]       [3]       [+]       [-]                         | 2 = (1)
     |                                                                       |
     |   [0]       [.]       [PI]      [(-)]     [EXE]                       | 1 = (0)
     |                                                                       |
     |                                                               [AC/ON] | 0 = (0)
     +-----------------------------------------------------------------------+
  Col    7         6         5         4         3         2         1
 
 *Bit    9         10        11        12        13        14        15
**Bit    1         2         3         4         5         6         7
 
 * When Row is even
** When Row is odd
 
 */

#ifdef __clang__
#include "fx-CG.h"

#define FXCG50_KEY_REG _fxCG_0xA44B0000
#else
#define FXCG50_KEY_REG 0xA44B0000
#endif

static const uint16_t *_keyboardRegister = (uint16_t *)FXCG50_KEY_REG;

static struct {
    uint16_t held[5];
    uint16_t last[5];   // Key/s that were last held down.
    uint16_t pressed[5];
    uint16_t released[5];
} key;


/**
 @brief    Takes a key reading of the keyboard register.
 */
void keyUpdate(void) {
    memcpy(key.held, _keyboardRegister, 10);
    for (int i=0; i<5; i++) {
        key.pressed[i] = ~key.last[i] & key.held[i];
        key.released[i] = key.last[i] & ~key.held[i];
    }
    memcpy(key.last, key.held, 10);
}

/**
 @brief    Returns true if key has is being held down.
 @param    keyCode  The fx-CGxx key code.
 
 keyUpdate not required.
 */
bool isKeyHeld(KeyCode keyCode) {
    int row = keyCode % 10;
    int col = keyCode / 10 - 1;
    
    int word = row >> 1;
    int bit = col + ((row & 1) << 3);
    
    return (0 != (_keyboardRegister[word] & 1<<bit));
}

/**
 @brief    Returns true if key has just been pressed.
 @param    keyCode  The fx-CGxx key code.
 
 keyUpdate required before using this function.
 */
bool isKeyPressed(KeyCode keyCode) {
    int row = keyCode % 10;
    int col = keyCode / 10 - 1;
    
    int word = row >> 1;
    int bit = col + ((row & 1) << 3);
    
    return (0 != (key.pressed[word] & 1<<bit));
}

/**
 @brief    Returns true if key has just been released
 @param    keyCode  The fx-CGxx key code.
 
 keyUpdate required before using this function.
 */
bool isKeyReleased(KeyCode keyCode) {
    int row = keyCode % 10;
    int col = keyCode / 10 - 1;
    
    int word = row >> 1;
    int bit = col + ((row & 1) << 3);
    
    return (0 != (key.released[word] & 1<<bit));
}
