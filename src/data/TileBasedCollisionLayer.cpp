/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2017 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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
#include "TileBasedCollisionLayer.hpp"

TileBasedCollisionLayer::TileBasedCollisionLayer(int width, int height, const std::vector<int32_t>& data)
    : CollisionLayer(CollisionLayer::TileBased), TiledLayerData(width, height, data)
{
}

TileBasedCollisionLayer::TileBasedCollisionLayer(int width, int height, const int32_t* data)
    : CollisionLayer(CollisionLayer::TileBased), TiledLayerData(width, height, data)
{
}

TileBasedCollisionLayer::TileBasedCollisionLayer(int width, int height)
    : CollisionLayer(CollisionLayer::TileBased), TiledLayerData(width, height)
{
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
