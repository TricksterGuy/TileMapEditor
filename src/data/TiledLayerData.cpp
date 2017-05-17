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
#include "TiledLayerData.hpp"
#include <cstring>

TiledLayerData::TiledLayerData(uint32_t _width, uint32_t _height, const std::vector<int32_t>& _data) : width(_width), height(_height), data(_data)
{
}

TiledLayerData::TiledLayerData(uint32_t _width, uint32_t _height, const int32_t* _data) : width(_width), height(_height), data(_data, _data + width * height)
{
}

TiledLayerData::TiledLayerData(uint32_t _width, uint32_t _height) : width(_width), height(_height), data(width * height, -1)
{
}

void TiledLayerData::Resize(uint32_t newwidth, uint32_t newheight, bool copy)
{
    if (newwidth == width && newheight == height)
        return;

    if (copy)
    {
        std::vector<int32_t> newdata = data;
        data.clear();
        data.reserve(newwidth * newheight);

        uint32_t minw = std::min(newwidth, width);
        uint32_t minh = std::min(newheight, height);
        int32_t excessx = (int32_t)(newwidth - width);
        int32_t excessy = (int32_t)(newheight - height);

        for (uint32_t i = 0; i < minh; i++)
        {
            data.insert(data.end(), newdata.begin() + i * width, newdata.begin() + i * width + minw);
            if (excessx > 0)
                data.insert(data.end(), excessx, -1);
        }
        if (excessy > 0)
            data.insert(data.end(), newwidth * excessy, -1);
    }
    else
    {
        data.assign(newwidth * newheight, -1);
    }

    width = newwidth;
    height = newheight;
}

void TiledLayerData::Shift(int32_t horizontal, int32_t vertical, bool wrap)
{
    int32_t* newdata = new int32_t[width * height];
    int32_t* olddata = data.data();

    if (!wrap)
        memset(newdata, 0xFF, sizeof(int32_t) * width * height);

    // TODO Make more efficient
    if (wrap)
    {
        for (uint32_t i = 0; i < height; i++)
        {
            for (uint32_t j = 0; j < width; j++)
            {
                int sx = (j + horizontal + width) % width;
                int sy = (i + vertical + height) % height;
                newdata[sy * width + sx] = olddata[i * width + j];
            }
        }
    }
    else
    {
        for (uint32_t i = 0; i < height; i++)
        {
            for (uint32_t j = 0; j < width; j++)
            {
                if (i - vertical < 0 || i - vertical >= height || j - horizontal < 0 || j - horizontal >= width)
                    continue;
                newdata[i * width + j] = olddata[(i - vertical) * width + (j - horizontal)];
            }
        }
    }

    data.assign(newdata, newdata + width * height);
    delete[] newdata;
}

void TiledLayerData::Clear()
{
    data.assign(width * height, -1);
}
