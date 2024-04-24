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

#include <stdint.h>

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

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

void fxEnableColor(void);

/**
 @brief    Returns a color in RGB 565 format from a given RGB[0-255] value.
 @param    r  Red channel
 @param    g  Green channel
 @param    b  Blue channel
 */
uint16_t fxColor(uint8_t r, uint8_t g, uint8_t b);

void fxPrint(int x, int y, const char *text, uint16_t color, uint16_t bgColor);
void fxPrintMini(int x, int y, const char *text, uint16_t color, uint16_t bgColor);

/**
 @brief    Write a line.  Bresenham's algorithm
 @param    x1  Start point x coordinate
 @param    y1  Start point y coordinate
 @param    x2  End point x coordinate
 @param    y2  End point y coordinate
 @param    color Specifies what color the plotted pixel will be. It is in RGB 565 format.
 */
void fxDrawLine(int x1, int y1, int x2, int y2, uint16_t color);

/**
 @brief    Draw a rectangle with no fill color
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void fxDrawRect(int x, int y, short w, short h, uint16_t color);

/**
 @brief    Draw a circle outline.
 @param    x   Center-point x coordinate.
 @param    y   Center-point y coordinate.
 @param    r   Radius of circle.
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void fxDrawCircle(int x, int y, short r, uint16_t color);

/**
 @brief    Draw a circle with filled color
 @param    x   Center-point x coordinate
 @param    y   Center-point y coordinate
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void fxFillCircle(int x, int y, short r, uint16_t color);

/**
 @brief    Draw a triangle with no fill color
 @param    x1  Vertex #1 x coordinate
 @param    y1  Vertex #1 y coordinate
 @param    x2  Vertex #2 x coordinate
 @param    y2  Vertex #2 y coordinate
 @param    x3  Vertex #3 x coordinate
 @param    y3  Vertex #3 y coordinate
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void fxDrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint8_t color);

/**
 @brief    Draw a triangle with color-fill
 @param    x1  Vertex #1 x coordinate
 @param    y1  Vertex #1 y coordinate
 @param    x2  Vertex #2 x coordinate
 @param    y2  Vertex #2 y coordinate
 @param    x3  Vertex #3 x coordinate
 @param    y3  Vertex #3 y coordinate
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void fxFillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color);

/**
 @brief    Draw a rounded rectangle with no fill color
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    r   Radius of corner rounding
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void fxDrawRoundRect(int x, int y, int w, int h, short r, uint16_t color);

/**
 @brief    Draw a rounded rectangle with fill color
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    r   Radius of corner rounding
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void fxFillRoundRect(int x, int y, int w, int h, short r, uint16_t color);

/**
 @brief    Creates a RGB 565 format color on a grey value.
 @param    shade Shade which can be between 0 (black) and 31 (white).
 */
uint16_t fxMakeGray(int shade);

/**
 @brief    Fills a rectangular area of (width,height) with upper-left corner at (x,y)
 @param    x   X-coordinate of the upper-left corner.
 @param    y   Y-coordinate of the upper-left corner.
 @param    w   Width of the filled rectangle.
 @param    h   Height of the filled rectangle.
 @param    color Color of the filled rectangle.
 */
void fxFillArea(unsigned x, unsigned y, unsigned w, unsigned h, uint16_t color);

/**
 @brief    Draws a signle pixel at (x,y)
 @param    x   Specifies the x coordinate of the pixel in range of [0,383]
 @param    y   Specifies the y coordinate of the pixel in range of [0,215]
 @param    color Specifies what color the plotted pixel will be. It is in RGB 565 format.
 */
void fxPlot(int x, int y, uint16_t color);

/**
 @brief    Draws a signle sub pixel at (x,y)
 @param    x   Specifies the x coordinate of the pixel in range of [0,383]
 @param    y   Specifies the y coordinate of the pixel in range of [0,215]
 @param    level Specifies what level the plotted sub pixel will be. It is in R/G/B format.
 */
void fxSubPlot(unsigned x, unsigned y, uint8_t level);

/**
 @brief    Draw a 16-bit sprite/image to the VRAM buffer.
 @param    data Pointer to sprite data.
 @param    x   X-coordinate of top-left of sprite.
 @param    y   Y-coordinate of top-left of sprite.
 @param    w   Width of sprite, in pixels.
 @param    h   Height of sprite, in pixels.
 */
void fxDrawSprite(uint16_t *data, int x, int y, int w, int h);

/**
 @brief    Draw a 16-bit sprite/image to the VRAM buffer.
 @param    data Pointer to sprite data.
 @param    x   X-coordinate of top-left of sprite.
 @param    y   Y-coordinate of top-left of sprite.
 @param    w   Width of sprite, in pixels.
 @param    h   Height of sprite, in pixels.
 @param    maskColor Color that should be treated as transparent.
 */
void fxDrawSpriteMaskedAlpha(uint16_t *data, int x, int y, int w, int h, uint16_t maskColor, int alpha);

/**
 @brief    Draw an N-bit (for N=8, 2, or 1) sprite/image to the VRAM buffer.
 @param    data Pointer to sprite data.
 @param    x   X-coordinate of top-left of sprite.
 @param    y   Y-coordinate of top-left of sprite.
 @param    w   Width of sprite, in pixels.
 @param    h   Height of sprite, in pixels.
 @param    palette A palette.
 @param    bitWidth The bit-width. This is designed for 8-, 2-, and 1-bit images.
 */
void fxDrawSpriteNbit(const uint8_t *data, int x, int y, int w, int h, uint16_t *palette, unsigned int bitWidth);

/**
 @brief    Draw an N-bit (for N=8, 2, or 1) sprite/image to the VRAM buffer, treating one color as transparent.
 @param    data Pointer to sprite data.
 @param    x   X-coordinate of top-left of sprite.
 @param    y   Y-coordinate of top-left of sprite.
 @param    w   Width of sprite, in pixels.
 @param    h   Height of sprite, in pixels.
 @param    palette A palette.
 @param    maskColor Color that should be treated as transparent.
 @param    bitWidth The bit-width. This is designed for 8-, 2-, and 1-bit images.
 */
void fxDrawSpriteNbitMasked(const uint8_t *data, int x, int y, int w, int h, const uint16_t *palette, uint16_t maskColor, unsigned int bitWidth);

/**
 @brief    Change the speed of the operating PLL circuit.
 
 Notes: PLEASE before handling MENU keypresses in your add-in, use PLL_16x to go back to normal operating speed, at the courtesy of other applications and the OS. Feeding a value not defined below (AKA, an invalid value), will cause a crash.
 */
void fxChangeFreq(int mult);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /* graphics_h */
