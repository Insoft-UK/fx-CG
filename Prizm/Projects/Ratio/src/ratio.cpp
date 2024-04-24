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

#include "ratio.hpp"


using namespace fx;

int String::toInteger(void)
{
    if (!_str) return 0;
    
    int i;
    char *p = (char *)_str;
    bool isNeg = false;
      if(*p == '-') {
        isNeg=true;
        p++;
      }
      i=0;
      while(*p!='\0') {
        i =i*10 + (*p - 48);
        p++;
      }
      if(isNeg)
        i=i*-1;
    return i;
}

int String::length(void)
{
    if (!_str) return 0;
    return (int)strlen(_str);
}

char *String::string(void)
{
    return _str;
}

String::String(const char *str)
{
    _str = (char *)malloc(strlen(str) + 1);
    if (_str)
        memcpy(_str, str, strlen(str) + 1);
}

int IntInput(int row, int length)
{
    char input[20] = "\xd8";
    char *p = input;
    uint8_t blink = 0;
    int y = row * 24 + 24;
    
    while (true) {
        fxKeyUpdate();
        
        switch (fxKeyPressed()) {
            case KeyCode_0:
                if (p - input < length) *p++ = '0';
                break;
                
            case KeyCode_1:
                if (p - input < length) *p++ = '1';
                break;
                
            case KeyCode_2:
                if (p - input < length) *p++ = '2';
                break;
                
            case KeyCode_3:
                if (p - input < length) *p++ = '3';
                break;
                
            case KeyCode_4:
                if (p - input < length) *p++ = '4';
                break;
                
            case KeyCode_5:
                if (p - input < length) *p++ = '5';
                break;
                
            case KeyCode_6:
                if (p - input < length) *p++ = '6';
                break;
                
            case KeyCode_7:
                if (p - input < length) *p++ = '7';
                break;
                
            case KeyCode_8:
                if (p - input < length) *p++ = '8';
                break;
                
            case KeyCode_9:
                if (p - input < length) *p++ = '9';
                break;
                
            case KeyCode_Del:
                if (p == input) break;
                *--p = 0;
                if (p == input) {
                    *p = 0xD8;
                }
                break;
                
            case KeyCode_Return:
                fxFillArea(0, y, 384, 24, FXCGColorWhite);
                return StringToInt(input);
            
                
            default:
                break;
        }
        
        fxFillArea(0, y, 384, 24, FXCGColorWhite);
        fxPrint(0, y, input, FXCGColorBlack, FXCGColorWhite);
        
        if (blink++ & 0x20)
            fxFillArea((int)(p - input) * 18, y, 3, 22, FXCGColorBlack);
        
        Bdisp_PutDisp_DD();
        OS_InnerWait_ms(15);
    }
    return 0;
}

int IntToString(int value, char *result)
{
    bool isNeg=false;
    if(value < 0)
        isNeg = true;
    
    int idx = 0;
    do {
        int j = value % 10;
        j = j < 0 ? j * -1 : j ;
        result[idx++]=j+48;
        value=value/10;
    } while (value!=0);
    if(isNeg)
        result[idx++]='-';
    result[idx]='\0';
    
    int j = idx-1;
    int i=0;
    while (i<j) {
        char tmp = result[j];
        result[j] = result[i];
        result[i] = tmp;
        i++;
        j--;
    }
    
    return idx;
}

int StringToInt(const char *str)
{
    int i;
    char *p = (char *)str;
    bool isNeg = false;
      if(*p == '-') {
        isNeg=true;
        p++;
      }
      i=0;
      while(*p!='\0') {
        i =i*10 + (*p - 48);
        p++;
      }
      if(isNeg)
        i=i*-1;
    return i;
}

int StrLength(const char *str)
{
    char *p = (char *)str;
    if (!*p) return 0;
    int n=0;
    
    do {
        n++;
    } while (*++p);
    
    return n;
}
