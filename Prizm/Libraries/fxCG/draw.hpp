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

#ifndef draw_hpp
#define draw_hpp

#include "types.h"

#define SCREEN_WIDTH 384
#define SCREEN_HEIGHT 216

namespace fxCG::draw {

/**
 @brief    Enables the drawing at half the opacity
 */
void enableHalfOpacityDrawing();

/**
 @brief    Disables the drawing at half the opacity
 */
void disableHalfOpacityDrawing();

/**
 @brief    Write a line.  Bresenham's algorithm
 @param    x1  Start point x coordinate
 @param    y1  Start point y coordinate
 @param    x2  End point x coordinate
 @param    y2  End point y coordinate
 @param    color Specifies what color the plotted pixel will be. It is in RGB 565 format.
 */
void line(int x1, int y1, int x2, int y2, color_t color);

/**
 @brief    Draw a rectangle with no fill color
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void rect(int x, int y, short w, short h, color_t color);

/**
 @brief    Draw a circle outline.
 @param    x   Center-point x coordinate.
 @param    y   Center-point y coordinate.
 @param    r   Radius of circle.
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void circle(int x, int y, short r, color_t color);

/**
 @brief    Draw a circle with filled color
 @param    x   Center-point x coordinate
 @param    y   Center-point y coordinate
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void fillCircle(int x, int y, short r, color_t color);

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
void triangle(int x1, int y1, int x2, int y2, int x3, int y3, color_t color);

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
void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, color_t color);

/**
 @brief    Draw a rounded rectangle with no fill color
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    r   Radius of corner rounding
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void roundRect(int x, int y, int w, int h, short r, color_t color);

/**
 @brief    Draw a rounded rectangle with fill color
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    r   Radius of corner rounding
 @param    color Specifies what color to draw with. It is in RGB 565 format.
 */
void fillRoundRect(int x, int y, int w, int h, short r, color_t color);


/**
 @brief    Fills a rectangular area of (width,height) with upper-left corner at (x,y)
 @param    x   X-coordinate of the upper-left corner.
 @param    y   Y-coordinate of the upper-left corner.
 @param    w   Width of the filled rectangle.
 @param    h   Height of the filled rectangle.
 @param    color Color of the filled rectangle.
 */
void fillArea(unsigned x, unsigned y, unsigned w, unsigned h, color_t color);

/**
 @brief    Draws a signle pixel at (x,y)
 @param    x   Specifies the x coordinate of the pixel in range of [0,383]
 @param    y   Specifies the y coordinate of the pixel in range of [0,215]
 @param    color Specifies what color the plotted pixel will be. It is in RGB 565 format.
 */
void pixel(int x, int y, uint16_t color);

/**
 @brief    Draw a 16-bit image to the VRAM buffer.
 @param    data Pointer to sprite data.
 @param    x   X-coordinate of top-left of sprite.
 @param    y   Y-coordinate of top-left of sprite.
 @param    w   Width of sprite, in pixels.
 @param    h   Height of sprite, in pixels.
 */
void image(uint16_t *data, int x, int y, int w, int h);

/**
 @brief    Draw a 16-bit image to the VRAM buffer.
 @param    data Pointer to sprite data.
 @param    x   X-coordinate of top-left of sprite.
 @param    y   Y-coordinate of top-left of sprite.
 @param    w   Width of sprite, in pixels.
 @param    h   Height of sprite, in pixels.
 @param    maskColor Color that should be treated as transparent.
 */
void imageMaskedAlpha(uint16_t *data, int x, int y, int w, int h, color_t maskColor, int alpha);

/**
 @brief    Draw an N-bit (for N=8, 2, or 1) image to the VRAM buffer.
 @param    data Pointer to sprite data.
 @param    x   X-coordinate of top-left of sprite.
 @param    y   Y-coordinate of top-left of sprite.
 @param    w   Width of sprite, in pixels.
 @param    h   Height of sprite, in pixels.
 @param    palette A palette.
 @param    bitWidth The bit-width. This is designed for 8-, 2-, and 1-bit images.
 */
void imageNbit(const uint8_t *data, int x, int y, int w, int h, color_t *palette, unsigned int bitWidth);

/**
 @brief    Draw an N-bit (for N=8, 4, 2, or 1) image to the VRAM buffer, treating one color as transparent.
 @param    data Pointer to image data.
 @param    x   X-coordinate of top-left of sprite.
 @param    y   Y-coordinate of top-left of sprite.
 @param    w   Width of sprite, in pixels.
 @param    h   Height of sprite, in pixels.
 @param    palette A palette.
 @param    maskColor Color that should be treated as transparent.
 @param    bitWidth The bit-width. This is designed for 8-, 2-, and 1-bit images.
 */
void imageNbitMasked(const uint8_t *data, int x, int y, int w, int h, const color_t *palette, color_t maskColor, unsigned int bitWidth);

};

#endif /* draw_hpp */
