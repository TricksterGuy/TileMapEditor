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
#include "Layer.hpp"

#include <cstring> // for memset

Layer::Layer(const std::string& _name, uint32_t width, uint32_t height, const std::vector<int32_t>& data, const DrawAttributes& attr)
    : DrawAttributes(attr), TiledLayerData(width, height, data), name(_name)
{
}

Layer::Layer(const std::string& _name, uint32_t width, uint32_t height, const int32_t* data, const DrawAttributes& attr)
    : DrawAttributes(attr), TiledLayerData(width, height, data), name(_name)
{
}

Layer::Layer(const std::string& _name, uint32_t width, uint32_t height, const DrawAttributes& attr)
    : DrawAttributes(attr), TiledLayerData(width, height), name(_name)
{
}
