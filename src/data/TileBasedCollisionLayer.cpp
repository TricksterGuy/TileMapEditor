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

#include <cstring>
#include <cassert>
#include <cstdio>
#include "TileBasedCollisionLayer.hpp"

TileBasedCollisionLayer::TileBasedCollisionLayer(int _width, int _height, const std::vector<int32_t>& _data) :
CollisionLayer(Collision::TileBased), width(_width), height(_height), data(_data)
{
}

TileBasedCollisionLayer::TileBasedCollisionLayer(int _width, int _height, const int32_t* _data) :
CollisionLayer(Collision::TileBased), width(_width), height(_height), data(_data, _data + width * height)
{
}

TileBasedCollisionLayer::TileBasedCollisionLayer(int _width, int _height) :
CollisionLayer(Collision::TileBased), width(_width), height(_height), data(width * height, -1)
{
}

TileBasedCollisionLayer::TileBasedCollisionLayer(const TileBasedCollisionLayer& layer) :
CollisionLayer(layer.type), width(layer.width), height(layer.height), data(layer.data)
{
}

TileBasedCollisionLayer::~TileBasedCollisionLayer()
{
}

TileBasedCollisionLayer& TileBasedCollisionLayer::operator=(const TileBasedCollisionLayer& layer)
{
    if (this != &layer)
    {
        type = layer.type;
        width = layer.width;
        height = layer.height;
        data = layer.data;
    }
    return *this;
}

bool TileBasedCollisionLayer::operator==(const TileBasedCollisionLayer& other)
{
    if (type != other.type)
        return false;
    if (width != other.width && height != other.height)
        return false;
    for (uint32_t i = 0; i < width * height; i++)
    {
        if (data[i] != other.data[i])
            return false;
    }

    return true;
}

void TileBasedCollisionLayer::Resize(uint32_t newwidth, uint32_t newheight, bool copy)
{
    if (newwidth == width && newheight == height) return;

    if (copy)
    {
        std::vector<int32_t> newdata = data;
        data.clear();
        data.reserve(width * height);

        uint32_t minw = newwidth > width ? width : newwidth;
        uint32_t minh = newheight > height ? height : newheight;
        int32_t excessx = (int32_t)(newwidth - width);
        int32_t excessy = (int32_t)(newheight - height);

        for (uint32_t i = 0; i < minh; i++)
        {
            data.insert(data.begin() + i * newwidth, newdata.begin() + i * width, newdata.begin() + i * width + minw);
            if (newwidth > width) data.insert(data.begin() + i * newwidth + width, -1, excessx);
        }

        if (newheight > height)
            data.insert(data.begin() + minh * newwidth, -1, newwidth * excessy);
    }
    else
    {
        data.assign(newwidth * newheight, -1);
    }

    width = newwidth;
    height = newheight;
}

void TileBasedCollisionLayer::Clear()
{
    data.assign(width * height, -1);
}

void TileBasedCollisionLayer::Shift(int horizontal, int vertical, bool wrap)
{
    /// TODO implement
}
