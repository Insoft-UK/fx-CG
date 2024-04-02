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

#ifndef graphics_h
#define graphics_h

#define PLL_24x 0b010111 // 87 MHz
#define PLL_18x 0b010001 // 65.25 MHz
#define PLL_16x 0b001111 // 58 MHz (NORMAL SPEED)
#define PLL_15x 0b001110 // 54.37 MHz
#define PLL_12x 0b001011 // 43.5 MHz
#define PLL_8x  0b000111 // 29 MHz
#define PLL_6x  0b000101 // 21.75 MHz
#define PLL_4x  0b000011 // 14.5 MHz
#define PLL_3x  0b000010 // 10.8 MHz
#define PLL_2x  0b000001 // 7.25 MHz
#define PLL_1x  0b000000 // 3.6 MHz

unsigned short rgb(unsigned char r, unsigned char g, unsigned char b);
void drawLine(int x1, int y1, int x2, int y2, unsigned short color);
void drawRect(int x, int y, short w, short h, unsigned short color);
void drawCircle(int x, int y, short r, unsigned short color);
void fillCircle(int x, int y, short r, unsigned short color);
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned char color);
void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned short color);
void drawRoundRect(int x, int y, int w, int h, short r, unsigned short color);
void fillRoundRect(int x, int y, int w, int h, short r, unsigned short color);
void fillArea(unsigned x, unsigned y, unsigned w, unsigned h, unsigned short color);
unsigned short makeGray(int shade);
void plot(unsigned x, unsigned y, unsigned short color);
void drawSprite(unsigned short* data, int x, int y, int w, int h);
void drawSpriteMaskedAlpha(unsigned short* data, int x, int y, int w, int h, unsigned short maskColor, int alpha);
void drawSpriteNbit(const unsigned char* data, int x, int y, int w, int h, unsigned short* palette, unsigned int bitWidth);
void drawSpriteNbitMasked(const unsigned char* data, int x, int y, int w, int h, const unsigned short* palette, unsigned short maskColor, unsigned int bitWidth);
void changeFreq(int mult);

#endif /* graphics_h */
