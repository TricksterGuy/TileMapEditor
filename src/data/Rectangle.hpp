/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2015 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * excluding commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented;
 *    you must not claim that you wrote the original software.
 *    An acknowledgement in your documentation and link to the original version is required.
 *
 * 2. Altered source versions must be plainly marked as such,
 *    and must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 ******************************************************************************************************/
#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <cstdint>

/** Represents a rectangle with (x, y) and width and height. */
class Rectangle
{
  public:
  /** Creates a rectangle with position and dimensions.
    * @param x X Coordinate.
    * @param y Y Coordinate.
    * @param width Width of the rectangle.
    * @param height Height of the rectangle.
    */
  Rectangle(int32_t x = 0, int32_t y = 0, int32_t width = 0, int32_t height = 0);
  bool operator==(const Rectangle& rhs) const;
  bool operator!=(const Rectangle& rhs) const { return !(*this == rhs); }
  bool operator<(const Rectangle& rhs) const;
  /** Tests if the rectangle is valid, that is the width and height are not negative.
    * @return true if the rectangle has positive width and height false otherwise
    */
  bool IsValid() const { return width > 0 && height > 0; }
  /** Gets the top left and bottom right coordinates of this rectangle
    * @param x1 top left (x)
    * @param y1 top left (y)
    * @param x2 bottom right (x + width)
    * @param y2 bottom right (y + height)
    */
  void GetCoords(int32_t& x1, int32_t& y1, int32_t& x2, int32_t& y2) const;
  /** Sets the position and dimensions of the rectangle.
    * @param x X Coordinate.
    * @param y Y Coordinate.
    * @param width Width of the rectangle.
    * @param height Height of the rectangle.
    */
  void Set(int32_t x, int32_t y, int32_t width, int32_t height);
  /** Moves this rectangle.
    * @param xoffset Horizontal offset.
    */
  void Move(int32_t xoffset, int32_t yoffset);
  /** Returns true if the point given is contained in this rectangle.
    * @param x X Coordinate.
    * @param y Y Coordinate.
    * @return true if the point is contained in the rectangle false otherwise.
    */
  bool Contains(int32_t x, int32_t y) const;
  /** Returns true if the rectangle given is fully contained in this rectangle.
    * @param rect Rectangle to test.
    * @return true if the rectangle is contained in the rectangle false otherwise.
    */
  bool Contains(const Rectangle& rect) const;
  /** Returns true if the rectangle given intersects this rectangle.
    * @param rect Rectangle to test.
    * @param overlap variable to write the intersection to.
    * @return true if the rectangles intersect.
    */
  bool Intersects(const Rectangle& rect, Rectangle& overlap) const;
  /** Gets the area of this rectangle.
    * @return the area.
    */
  int64_t Area() const;
  int32_t x, y, width, height;
};

#endif
