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
#include "fxCG/ui.hpp"

#include "C437.h"
#include "common.hpp"

#include <stdlib.h>

#define RPN_INPUT (216 - 24 - 24)
#define RPN_STACK (RPN_INPUT - 24)

//#define LCD_WIDTH_PX 384
//#define LCD_HEIGHT_PX 216

using namespace fxCG;
using namespace draw;
using namespace key;
using namespace font;

#include <stdio.h>
#include <math.h>






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

    int size(void)
    {
        return _sp;
    }
    
    void push(const double value)
    {
        if (_sp >= 100) return;
        _stack[_sp++] = value;
    }
    
    double pop(void)
    {
        if (!_sp) return NAN;
        return _stack[--_sp];
    }
    
    double top(void)
    {
        if (!_sp) return NAN;
        return _stack[_sp - 1];
    }
    
    
    
    char getOperator(const int key)
    {
        if (key == KEY_CHAR_DIV) return '/';
        if (key == KEY_CHAR_MULT) return '*';
        if (key == KEY_CHAR_MINUS) return '-';
        if (key == KEY_CHAR_PLUS) return '+';
        if (key == KEY_CHAR_SQUARE) return 'S';
        if (key == KEY_CHAR_SIN) return 's';
        if (key == KEY_CHAR_COS) return 'c';
        if (key == KEY_CHAR_TAN) return 't';
        if (key == KEY_CHAR_LOG) return 'l';
        if (key == KEY_CHAR_LN) return 'n';
        if (key == KEY_CHAR_POW) return '^';
        
        return '\0';
    }
    
    int sin(void)
    {
        double x;
        if (_sp < 1) return 1;
        x = pop();
        push(math::sin(x));
        return 0;
    }
    
    int cos(void)
    {
        double x;
        if (_sp < 1) return 1;
        x = pop();
        push(math::cos(x));
        return 0;
    }
    
    int tan(void)
    {
        double x;
        if (_sp < 1) return 1;
        x = pop();
        push(math::tan(x));
        return 0;
    }
    
    int log(void)
    {
        double x;
        if (_sp < 1) return 1;
        x = pop();
        push(math::log10(x));
        return 0;
    }
    
    int ln(void)
    {
        double x;
        if (_sp < 1) return 1;
        x = pop();
        push(math::log(x));
        return 0;
    }
    
    int square(void)
    {
        double x;
        if (_sp < 1) return 1;
        x = pop();
        push(x);
        return 0;
    }
    
    int operation(const char op)
    {
        double x, y = 0;
        if (_sp < 2) return 1;
        
        y = pop();
        x = pop();
        
        switch (op) {
            case '/':
                if (y == 0) return 1;
                push(x / y);
                return 0;
                
            case '*':
                push(x * y);
                return 0;
                
            case '-':
                push(x - y);
                return 0;
                
            case '+':
                push(x + y);
                return 0;
                
            case '^':
                push(math::pow(x, y));
                return 0;
                
            default:
                return 1;
        }
    }
    
    int inputLine(char *buf, size_t length)
    {
        short cursor = 0;
        int col = 0, row = 0;
        unsigned short key = 0, last_key = 0;
        keycode_t keycode = 0;
        bool shift = false;
        
        uint8_t map[42] = {
             0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
             0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
             0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
            '7', '8', '9',  0 ,  0 ,  0 ,
            '4', '5', '6',  0 ,  0 ,  0 ,
            '1', '2', '3',  0 ,  0 ,  0 ,
            '0', '.',  0 ,  0 ,  0 ,  0
        };
        
        memset(buf, 0, length);
        
        do {
            int x = cursor * 18;
            int y = RPN_INPUT;
            fillArea(x, y, LCD_WIDTH_PX - x, 24, 0xFFFF);
            if (*buf)
                PrintCXY(0, y - 24, buf, TEXT_MODE_NORMAL, -1, black, white, 1, 0);
            else
                PrintCXY(0, y - 24, "\xD8", TEXT_MODE_NORMAL, -1, black, white, 1, 0);
            fillArea(x, y, 3, 22, 0);
            
            Bdisp_PutDisp_DD();
            
            int retval = GetKeyWait_OS(&col, &row, KEYWAIT_HALTON_TIMERON, 1, 0, &key);
            
            if (retval == KEYREP_KEYEVENT) {
                keycode = 0;
                
                if (key != last_key)
                    keycode = (10 * col) + (row - 1);
                  
            } else key = 0;
            
            last_key = key;
            
            if (keycode) {
                switch (keycode) {
                    case KEY_PRGM_DEL:
                        if (!cursor) {
                            pop();
                            refresh();
                            break;
                        }
                        cursor--;
                        memmove(&buf[cursor], &buf[cursor + 1], strlen(buf) - cursor);
                        shift = false;
                        break;
                        
                    case KEY_PRGM_LEFT:
                        if (--cursor < 0) cursor = strlen(buf);
                        shift = false;
                        break;
                        
                    case KEY_PRGM_RIGHT:
                        if (++cursor > (short)strlen(buf)) cursor = 0;
                        shift = false;
                        break;
                        
                    case KEY_PRGM_SHIFT:
                        shift = !shift;
                        break;
                        
                    case 43: return KEY_CHAR_MULT;
                    case 42: return KEY_CHAR_PLUS;
                    case 33: return KEY_CHAR_DIV;
                    case 32: return KEY_CHAR_MINUS;
//                    case 46: return KEY_CHAR_SIN;
//                    case 36: return KEY_CHAR_COS;
//                    case 26: return KEY_CHAR_TAN;
//                    case 56: return KEY_CHAR_LN;
//                    case 66: return KEY_CHAR_LOG;
                    case 67: return shift ? KEY_CHAR_ROOT : KEY_CHAR_SQUARE;
//                    case 57: return KEY_CHAR_POW;
                        
                    case 41:
                        if (*buf == '\x87') {
                            memmove(buf, &buf[1], strlen(buf));
                            cursor--;
                        } else {
                            memmove(buf + 1, buf, strlen(buf));
                            *buf = '\x87';
                            cursor++;
                        }
                        shift = false;
                        break;
                        
                    default: {
                        int i = (7 - (row - 2)) * 6 + 6 - (col - 2) - 6 - 1;
                        uint8_t ascii = map[i];
                        if (ascii == '.' && strchr(buf, '.')) break;
                        if (ascii >= ' ' && ascii <= '~' && cursor < 18) strinc(buf, ascii, cursor++, length);
                    }
                        shift = false;
                        break;
                }
            }
        } while (keycode != KEY_PRGM_RETURN);
        if (*buf == '\x87') *buf='-';
        return 0;
    }
    
    void refresh(void)
    {
        fillArea(0, 24, 384, 172, white);
        
        char str[50];
        int y = RPN_STACK;
        for (int i=0; i<_sp && i < 6; i++, y-=24) {
            dtostr(_stack[_sp - i - 1], str, 6);
            PrintCXY(width - 18 * (int)strlen(str) - 0, y - 24, str, TEXT_MODE_NORMAL, -1, black, white, 1, 0);
            
        }
        line(0, 22, 383, 22, 0);
        line(0, RPN_INPUT - 2, 383, RPN_INPUT - 2, gray(24));
        for (int i = 0; i < 5; i++) {
            line(0, RPN_STACK - 2 - 24 * i, 383 - 0, RPN_STACK - 2 - 24 * i, cyan);
        }
        
        Bdisp_PutDisp_DD();
    }
};




void quitHandler(void)
{
    FrameColor(FrameModeSetToColor, white);
    DrawFrame(white);
    fxCG::disableFullColorMode();
}

// MARK: - CASIO fxCG Add-In Application "main" Function
int g3a(void)
{
    SetQuitHandler(quitHandler);
    
    FrameColor(FrameModeSetToColor, white);
    DrawFrame(white);
    
    /// Switches the screen to full color mode (16 bits per pixel, RGB565)
    fxCG::enableFullColorMode();
    
    
    char input[20] = "\0";
    int cursor = 0;
    
    char color = TEXT_COLOR_WHITE;
    DefineStatusAreaFlags(3, SAF_BATTERY | SAF_TEXT | SAF_GLYPH | SAF_ALPHA_SHIFT, &color, &color);
    EnableStatusArea(0);
    
    RPN rpn;
    rpn.refresh();
    
    loop {
        int key = rpn.inputLine(input, sizeof(input));
    
        if (strlen(input))
            rpn.push(atof(input));
        
        if (key == KEY_CHAR_MULT) rpn.operation('*');
        if (key == KEY_CHAR_DIV) rpn.operation('/');
        if (key == KEY_CHAR_MINUS) rpn.operation('-');
        if (key == KEY_CHAR_PLUS) rpn.operation('+');
        if (key == KEY_CHAR_POW) rpn.operation('^');
        if (key == KEY_CHAR_SQUARE) rpn.square();
//        if (key == KEY_CHAR_SIN) rpn.sin();
//        if (key == KEY_CHAR_COS) rpn.cos();
//        if (key == KEY_CHAR_TAN) rpn.tan();
//        if (key == KEY_CHAR_LOG) rpn.log();
//        if (key == KEY_CHAR_LN) rpn.ln();
        
        rpn.refresh();
        GetKey(&key);
    }
    
    return 0;
}

