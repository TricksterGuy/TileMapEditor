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
#include "Map.hpp"

#include <iostream>
#include <istream>
#include <fstream>
#include <cassert>

using namespace std;

void Map::Clear()
{
    for (auto& layer : layers)
        layer.Clear();

    if (HasCollisionLayer())
        collision_layer->Clear();
}

void Map::Resize(uint32_t newwidth, uint32_t newheight, bool copy)
{
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);
    if (collision_layer)
    {
        if (collision_layer->GetType() == CollisionLayer::PixelBased)
            collision_layer->Resize(newwidth * tile_width, newheight * tile_height, copy);
        else
            collision_layer->Resize(newwidth, newheight, copy);
    }
}

void Map::Shift(int horizontal, int vertical, bool wrap)
{
    for (auto& layer : layers)
        layer.Shift(horizontal, vertical, wrap);
    if (collision_layer)
        collision_layer->Shift(horizontal, vertical, wrap);
}

void Map::Add(const Layer& layer)
{
    layers.push_back(layer);
}

void Map::Add(const Background& back)
{
    backgrounds.push_back(back);
}

void Map::DeleteLayer(const uint32_t index)
{
    layers.erase(layers.begin() + index);
}

void Map::DeleteBackground(const uint32_t index)
{
    backgrounds.erase(backgrounds.begin() + index);
}

uint32_t Map::GetWidth() const
{
    uint32_t width = 0;
    for (const auto& layer : layers)
    {
        if (layer.GetWidth() > width)
            width = layer.GetWidth();
    }
    return width;
}

uint32_t Map::GetHeight() const
{
    uint32_t height = 0;
    for (const auto& layer : layers)
    {
        if (layer.GetHeight() > height)
            height = layer.GetHeight();
    }
    return height;
}
