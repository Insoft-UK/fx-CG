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

#include "fxCG.hpp"
#include "C437.h"

using namespace fxCG;
using namespace draw;
using namespace key;
using namespace font;

// MARK: - CASIO fxCG Add-In Application "main" Function
int fxCG_g3a(void)
{
    /// Switches the screen to full color mode (16 bits per pixel, RGB565)
    enableColor();
    clearDisplay(white);
    reset();
    
    bool running = true;
    while (running) {
        update();
        clearDisplay(white);

        if (isHeld(Exit)) {
            print(0, 24 * 1, "[EXIT] KeyHeld", black, C437);
        }
        
        if (isPressed(Exit)) {
            print(0, 24 * 2, "[EXIT] KeyPressed", black, C437);
        }
        
        if (isReleased(Exit)) {
            print(0, 24 * 3, "[EXIT] KeyReleased", black, C437);
        }
        

        if (isPressed(Menu)) {
            running = false;
        }
        
        updateDisplay();
        wait(100);
    }
    return 0;
}

