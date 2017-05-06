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
#ifndef TILESET_HPP
#define TILESET_HPP

#include "AnimatedTile.hpp"

class Tileset {
public:
    Tileset(const std::string& filename = "", uint32_t tile_width = MIN_TILE_SIZE, uint32_t tile_height = MIN_TILE_SIZE);
    void SetFilename(const std::string& _filename) { filename = _filename; }
    void SetTileDimensions(uint32_t tile_width, uint32_t tile_height);
    void SetAnimatedTiles(const std::vector<AnimatedTile>& tiles) { animated_tiles = tiles; }
    const std::string& GetFilename() const { return filename; }
    void GetTileDimensions(uint32_t& tile_width, uint32_t& tile_height) const;
    const std::vector<AnimatedTile>& GetAnimatedTiles() const { return animated_tiles; }
    static constexpr uint32_t MIN_TILE_SIZE = 4;
    static constexpr uint32_t MAX_TILE_SIZE = 256;
private:
    std::string filename;
    uint32_t tile_width, tile_height;
    std::vector<AnimatedTile> animated_tiles;
};

#endif
