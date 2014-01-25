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

#include "Background.hpp"

Background::Background(const std::string& _name, const std::string& _filename, uint32_t _mode, int32_t x, int32_t y, const DrawAttributes& attr) :
DrawAttributes(attr), name(_name), filename(_filename), mode(_mode), speed_x(x), speed_y(y)
{
}

void Background::GetSpeed(int32_t& x, int32_t& y) const
{
    x = speed_x;
    y = speed_y;
}

void Background::SetSpeed(int32_t x, int32_t y)
{
    speed_x = x;
    speed_y = y;
}
