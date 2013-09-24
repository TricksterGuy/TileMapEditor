/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2013 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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

Rectangle::Rectangle(int32_t _x, int32_t _y, uint32_t _width, uint32_t _height) : x(_x), y(_y), width(_width), height(_height)
{
}

Rectangle::~Rectangle()
{
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

void Rectangle::set(int32_t x, int32_t y, uint32_t width, uint32_t height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

void Rectangle::move(int32_t xoffset, int32_t yoffset)
{
    x += xoffset;
    y += yoffset;
}

bool Rectangle::contains(const Rectangle& rect) const
{
    return contains(rect.x, rect.y) && contains(rect.x + rect.width, rect.y + rect.height);
}

bool Rectangle::contains(int32_t sx, int32_t sy) const
{
    return sx >= x && (uint32_t)sx < x + width && sy >= y && (uint32_t)sy < y + height;
}

bool Rectangle::intersects(const Rectangle& rect, Rectangle& intersect) const
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

int64_t Rectangle::area() const
{
    return width * height;
}
