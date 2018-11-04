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
#ifndef LAYER_HPP
#define LAYER_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "DrawAttributes.hpp"
#include "TiledLayerData.hpp"

/** This class stores one layer of tile information
  */
class Layer : public DrawAttributes, public TiledLayerData
{
public:
    /** Creates a new layer with the specified name, width, height and data.
      * @param name name of the layer.
      * @param width non-zero width of the layer.
      * @param height non-zero height of the layer.
      * @param data data which should be width*height ints.
      */
    Layer(const std::string& name, uint32_t width, uint32_t height, const std::vector<int32_t>& data, const DrawAttributes& attr = DrawAttributes(0));
    /** Creates a new layer with the specified name, width, height and data.
      * @param name name of the layer.
      * @param width non-zero width of the layer.
      * @param height non-zero height of the layer.
      * @param data data which should be width*height 32 bit ints.
      */
    Layer(const std::string& name, uint32_t width, uint32_t height, const int32_t* data, const DrawAttributes& attr = DrawAttributes(0));
    /** Creates a new layer with the specified name, width, and height.
      * @param name name of the layer.
      * @param width non-zero width of the layer.
      * @param height non-zero height of the layer.
      */
    Layer(const std::string& name = "", uint32_t width = 1, uint32_t height = 1, const DrawAttributes& attr = DrawAttributes(0));

    std::string GetName() const { return name; }
    void SetName(const std::string& _name) { name = _name; }

protected:
    /** Name for this layer */
    std::string name;
};

#endif
