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

#include "DrawAttributes.hpp"

void DrawAttributes::GetPosition(int32_t& _x, int32_t& _y) const
{
    _x = x;
    _y = y;
}

void DrawAttributes::SetPosition(int32_t _x, int32_t _y)
{
    x = _x;
    y = _y;
}

void DrawAttributes::GetOrigin(int32_t& _x, int32_t& _y) const
{
    _x = origin_x;
    _y = origin_y;
}

void DrawAttributes::SetOrigin(int32_t _x, int32_t _y)
{
    origin_x = _x;
    origin_y = _y;
}

void DrawAttributes::GetScale(float& _x, float& _y) const
{
    _x = scale_x;
    _y = scale_y;
}

void DrawAttributes::SetScale(float _x, float _y)
{
    scale_x = _x;
    scale_y = _y;
}

int ZDepthCompare(const DrawAttributes& a, const DrawAttributes& b)
{
    return a.GetDepth() < b.GetDepth();
}
