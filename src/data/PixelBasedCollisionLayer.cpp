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

#include "PixelBasedCollisionLayer.hpp"

PixelBasedCollisionLayer::PixelBasedCollisionLayer(const std::vector<Rectangle>& rectangles) : CollisionLayer(Collision::PixelBased), data(rectangles)
{
}

PixelBasedCollisionLayer::PixelBasedCollisionLayer() : CollisionLayer(Collision::PixelBased)
{
}

PixelBasedCollisionLayer::~PixelBasedCollisionLayer()
{
}

PixelBasedCollisionLayer::PixelBasedCollisionLayer(const PixelBasedCollisionLayer& layer)
{
    type = layer.type;
    data = layer.data;
}

PixelBasedCollisionLayer& PixelBasedCollisionLayer::operator=(const PixelBasedCollisionLayer& layer)
{
    if (this != &layer)
    {
        type = layer.type;
        data = layer.data;
    }
    return *this;
}

bool PixelBasedCollisionLayer::operator==(const PixelBasedCollisionLayer& other)
{
    /// TODO implement
    return true;
}

void PixelBasedCollisionLayer::clear()
{
    data.clear();
}

void PixelBasedCollisionLayer::shift(int horizontal, int vertical, bool wrap)
{
    for (int i = 0; i < data.size(); i++)
    {
        /// TODO handle the case of wrapping.
        data.move(horizontal, vertical);
    }
}

void PixelBasedCollisionLayer::resize(uint32_t newwidth, uint32_t newheight, bool copy)
{
    /// TODO implement
}

void PixelBasedCollisionLayer::add(const Rectangle& rectangle)
{
    data.add(rectangle);
}
