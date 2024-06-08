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

#include "fxCG/fxCG.hpp"
#include "fxCG/draw.hpp"
#include "fxCG/key.hpp"
#include "fxCG/font.hpp"
#include "C437.h"

using namespace fxCG;
using namespace draw;
using namespace key;
using namespace font;

void quitHandler(void)
{
    FrameColor(FrameModeSetToColor, white);
    DrawFrame(white);
}

// MARK: - CASIO fxCG Add-In Application "main" Function
int g3a(void)
{
    SetQuitHandler(quitHandler);
    
    FrameColor(FrameModeSetToColor, black);
    DrawFrame(black);
    
    /// Switches the screen to full color mode (16 bits per pixel, RGB565)
    fxCG::enableFullColorMode();
    fxCG::clearDisplay(black);
    
    char *buf = 0;
    
    const char16_t *name = u"\\\\fls0\\README.txt";
    int file =  Bfile_OpenFile((unsigned short *)name, kOpenMode_Read);
    if (file != -1) {
        int size = Bfile_GetFileSize(file);
        buf = (char *)sys_malloc(size);
        if (buf) {
            Bfile_ReadFile(file, buf, size, 0);
        }
        Bfile_CloseFile(file);
    }
    
    loop {
        key::update();
        clearDisplay(white);

        if (isHeld(Exit)) {
            print(0, 16 * 4, buf, black, C437);
        }
        
        
        if (isPressed(Menu))
            break;
        
        
        updateDisplay();
        wait(40);
    }
    
    if (buf) sys_free(buf);
    
    return 0;
}

