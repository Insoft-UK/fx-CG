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

#include <stdlib.h>

#define RPN_INPUT (216 - 24 - 16)
#define RPN_STACK (RPN_INPUT - 16)

//#define LCD_WIDTH_PX 384
//#define LCD_HEIGHT_PX 216

using namespace fxCG;
using namespace draw;
using namespace key;
using namespace font;

char digit(Code code)
{
    int row = code % 10;
    int col = code / 10;
    
    if (row < 1 || row > 4 || col < 5) return 0;
    
    col = abs(col - 7);
    row = abs(row - 4);
    
    char map[4][4] = {
        "789",
        "456",
        "123",
        "0.\0"
    };
    
    return map[row][col];
}

class RPN {
private:
    double _stack[100];
    int _sp = 0;
    
public:

    
    void push(const double value)
    {
        if (_sp >= 100) return;
        _stack[_sp++] = value;
    }
    
    void pop(void)
    {
        if (!_sp) return;
        _sp--;
    }
    
    double top(void)
    {
        if (!_sp) return 0;
        return _stack[_sp - 1];
    }
    
    bool isOperator(key::Code code)
    {
        if (code == key::Div) return true;
        if (code == key::Mult) return true;
        if (code == key::Minus) return true;
        if (code == key::Add) return true;
        if (code == key::Sq) return true;
        if (code == key::Sin) return true;
        if (code == key::Cos) return true;
        if (code == key::Tan) return true;
        if (code == key::Log) return true;
        if (code == key::Ln) return true;
        if (code == key::Power) return true;
        return false;
    }
    
    char getOperator(key::Code code)
    {
        if (code == key::Div) return '/';
        if (code == key::Mult) return '*';
        if (code == key::Minus) return '-';
        if (code == key::Add) return '+';
        if (code == key::Sq) return 'S';
        if (code == key::Sin) return 's';
        if (code == key::Cos) return 'c';
        if (code == key::Tan) return 't';
        if (code == key::Log) return 'l';
        if (code == key::Ln) return 'n';
        if (code == key::Power) return '^';
        
        return '\0';
    }
    
    double applyOperator(const double a, const double b, const char op)
    {
        switch (op) {
            case '/':
                if (b == 0) return 0;
                return a / b;
                
            default:
                return 0;
        }
    }
    
    void performOperator(const key::Code code)
    {
        double a, b = 0;
        char op = getOperator(code);
        
        switch (op) {
            case '/':
            case '*':
            case '-':
            case '+':
            case '^':
            case '%':
                b = top(); _sp--;
                break;
        }
        
        a = top(); _sp--;
        
        double result = applyOperator(a, b, op);
        push(result);
    }
    
    
    void updateDisplay(void)
    {
        char buf[300];
        int y = RPN_STACK;
        for (int i=0; i<_sp && i < 9; i++, y-=16) {
            sprintf(buf, "%d", i + 1);
            int X = 0;
            int Y = y - 22;
            PrintMiniMini(&X, &Y, buf, 0, black, 0);
//            print(0, y, buf, black, C437);
            sprintf(buf, "%.8lg", _stack[_sp - i - 1]);
            print(width - 8 * strlen(buf), y, buf, black, C437);
            line(0, y - 1, width, y - 1, black);
        }
    }
};

RPN rpn;

double clearInput(char *str, const size_t length)
{
    double ret;
    char *ptr;
    
    ret = strtod(str, &ptr);
    
    memset(str, 0, length);
    return ret;
}

bool hasDecimalPoint(const char *str, size_t length)
{
    while (length--) {
        if (str[length - 1] == '.') return true;
    }
    return false;
}

void quitHandler(void)
{
    FrameColor(FXCGFrameModeSetToColor, white);
    DrawFrame(white);
}

// MARK: - CASIO fxCG Add-In Application "main" Function
int g3a(void)
{
    SetQuitHandler(quitHandler);
    
    FrameColor(FXCGFrameModeSetToColor, white);
    DrawFrame(white);
    
    /// Switches the screen to full color mode (16 bits per pixel, RGB565)
    fxCG::enableColor();
//    fxCG::clearDisplay(black);
    
    
    char input[40] = "\0";
    int cursor = 0;
    
    loop {
        key::update();
        fillArea(0, 22, 384, 172, white);
       
        if (isPressed(Menu))
            break;
        
        Code keyCode = key::pressed();
        
        if (digit(keyCode)) {
            if (cursor < strlen(input))
                memmove(&input[cursor + 1], &input[cursor], strlen(input) + cursor);
            input[cursor++] = digit(keyCode);
        }
        
        if (keyCode == Dot) {
            if (!hasDecimalPoint(input, sizeof(input))) {
                if (cursor < strlen(input))
                    memmove(&input[cursor + 1], &input[cursor], strlen(input) + cursor);
                input[cursor++] = '.';
            }
        }
    
        if (keyCode == Left && cursor) cursor--;
        if (keyCode == Right && cursor < strlen(input)) cursor++;
        
        if (keyCode == Del) {
            if (strlen(input)) {
                if (cursor) cursor--;
                memmove(&input[cursor], &input[cursor + 1], strlen(input) - cursor);
            } else {
                rpn.pop();
            }
        }
        
        if (rpn.isOperator(keyCode) || keyCode == Return) {
            if (strlen(input)) {
                rpn.push(clearInput(input, 40));
                cursor = 0;
            }
            if (keyCode != Return)
                rpn.performOperator(keyCode);
        }
        
        rpn.updateDisplay();
        
        if (strlen(input)) {
            print(0, RPN_INPUT, input, black, C437);
            line(cursor * 8, RPN_INPUT, cursor * 8, RPN_INPUT + 15, black);
        }
        
        updateDisplay();
        wait(40);
    }
    
    
    return 0;
}

