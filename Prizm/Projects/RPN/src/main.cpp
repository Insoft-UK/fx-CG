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
#include "fxCG/math.hpp"
#include "fxCG/input.hpp"

#include "C437.h"

#include <stdlib.h>

#define RPN_INPUT (216 - 24 - 24)
#define RPN_STACK (RPN_INPUT - 24)

//#define LCD_WIDTH_PX 384
//#define LCD_HEIGHT_PX 216

using namespace fxCG;
using namespace draw;
using namespace key;
using namespace font;

char digit(Keycode code)
{
    switch (code) {
        case key::K0: return '0';
        case key::K1: return '1';
        case key::K2: return '2';
        case key::K3: return '3';
        case key::K4: return '4';
        case key::K5: return '5';
        case key::K6: return '6';
        case key::K7: return '7';
        case key::K8: return '8';
        case key::K9: return '9';
            
        default:
            return 0;
    }
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
    
    bool isOperator(key::Keycode code)
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
    
    char getOperator(key::Keycode code)
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
                if (b == 0) return NAN;
                return a / b;
                
            case '*': return a * b;
            case '-': return a - b;
            case '+': return a + b;
            case '%': return math::fmod(a, b);
            case '^': return math::pow(a, b);
            case 's': return math::sin(a);
            case 'c': return math::cos(a);
            case 't': return math::tan(a);
            case 'l': return math::log10(a);
            case 'n': return math::log(a);
            case 'S': return a * a;
                
            default:
                return 0;
        }
    }
    
    void performOperator(const key::Keycode code)
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
                if (!_sp) return;
                b = top(); _sp--;
                break;
        }
        
        if (!_sp) return;
        a = top(); _sp--;
        
        double result = applyOperator(a, b, op);
        push(result);
    }
    
    
    void updateDisplay(void)
    {
        char buf[300];
        int y = RPN_STACK;
        for (int i=0; i<_sp && i < 6; i++, y-=24) {
            sprintf(buf, "  %.8lg", _stack[_sp - i - 1]);
            PrintXY(width - 18 * strlen(buf), y, buf, TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
            
        }
        
        line(0, RPN_INPUT - 2, 383, RPN_INPUT - 2, black);
        for (int i = 0; i < 5; i++) {
            line(0, RPN_STACK - 2 - 24 * i, 383, RPN_STACK - 2 - 24 * i, cyan);
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
    Keycode keycode;
    
    loop {
        key::update();
        fillArea(0, 22, 384, 172, white);
       
        keycode = key::pressed();
        
        if (keycode == Menu)
            return 0;
        
        
        if (digit(keycode)) {
            if (cursor < (int)strlen(input))
                memmove(&input[cursor + 1], &input[cursor], strlen(input) + cursor);
            input[cursor++] = digit(keycode);
        }
        
        if (keycode == Dot) {
            if (!hasDecimalPoint(input, sizeof(input))) {
                if (cursor < (int)strlen(input))
                    memmove(&input[cursor + 1], &input[cursor], strlen(input) + cursor);
                input[cursor++] = '.';
            }
        }
    
        if (keycode == Left && cursor) cursor--;
        if (keycode == Right && cursor < (int)strlen(input)) cursor++;
        
        if (keycode == Del) {
            if (strlen(input)) {
                if (cursor) cursor--;
                memmove(&input[cursor], &input[cursor + 1], strlen(input) - cursor);
            } else {
                rpn.pop();
            }
        }
        
        if (rpn.isOperator(keycode) || keycode == Return) {
            if (strlen(input)) {
                rpn.push(clearInput(input, 40));
                cursor = 0;
            }
            if (keycode != Return)
                rpn.performOperator(keycode);
        }
        
        rpn.updateDisplay();
        
        if (strlen(input)) {
            PrintCXY(0, RPN_INPUT, input, TEXT_MODE_NORMAL, -1, black, white, 1, 0);
            line(cursor * 18, RPN_INPUT, cursor * 18, RPN_INPUT + 23, black);
        }
        
//        input::drawCursor(cursor * 18, RPN_INPUT, true, true, NULL);
        
        keycode = key::held();
        char output[40];
        sprintf(output, "%d", keycode);
        
        PrintCXY(0, 0, output, TEXT_MODE_NORMAL, -1, black, white, 1, 0);
        
        updateDisplay();
        wait(40);
    }
    
    
    return 0;
}

