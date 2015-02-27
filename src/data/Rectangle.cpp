/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2014 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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

#include <algorithm>
#include "Rectangle.hpp"
#include <cassert>

Rectangle::Rectangle(int32_t _x, int32_t _y, int32_t _width, int32_t _height)
: x(_x), y(_y), width(_width), height(_height)
{
}

Rectangle::~Rectangle()
{
}

bool Rectangle::operator==(const Rectangle& rhs) const
{
  return (x == rhs.x) && (y == rhs.y) && (width == rhs.width) && (height == rhs.height);
}

bool Rectangle::operator<(const Rectangle& rhs) const
{
  if (x != rhs.x)
    return x < rhs.x;
  else if (y != rhs.y)
    return y < rhs.y;
  else if (width != rhs.width)
    return width < rhs.width;
  else if (height != rhs.height)
    return height < rhs.height;
  else
    return false;
}

void Rectangle::GetCoords(int32_t& x1, int32_t& y1, int32_t& x2, int32_t& y2) const
{
  x1 = x;
  y1 = y;
  x2 = x + width;
  y2 = y + height;
}

void Rectangle::Set(int32_t x, int32_t y, int32_t width, int32_t height)
{
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
}

void Rectangle::Move(int32_t xoffset, int32_t yoffset)
{
  x += xoffset;
  y += yoffset;
}

bool Rectangle::Contains(const Rectangle& rect) const
{
  return Contains(rect.x, rect.y) && Contains(rect.x + rect.width, rect.y + rect.height);
}

bool Rectangle::Contains(int32_t sx, int32_t sy) const
{
  return sx >= x && sx < x + width && sy >= y && sy < y + height;
}

bool Rectangle::Intersects(const Rectangle& rect, Rectangle& intersect) const
{
  int32_t x1 = std::max(x, rect.x);
  int32_t y1 = std::max(y, rect.y);
  int32_t x2 = std::min(x + width, rect.x + rect.width);
  int32_t y2 = std::min(y + height, rect.y + rect.height);

  if ((x1 < x2) && (y1 < y2))
  {
    intersect = Rectangle(x1, y1, x2 - x1, y2 - y1);
    return true;
  }
  else
  {
    intersect = Rectangle(0, 0, 0, 0);
    return false;
  }
}

int64_t Rectangle::Area() const
{
  return width * height;
}
