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

#include <Magick++.h>
#include "Map.hpp"

/** Utilities for loading/saving files.*/
class HandlerUtils
{
    public:
        /** Converts a map to an Magick::Image.
          * @param map Map object.
          * @param image Image object to store the result.
          * @return nonzero on failure 0 on success.
          */
        static int MapToImage(Map& map, Magick::Image& image);
        /** Converts a layer to an Magick::Image.
          * @param map Map object.
          * @param layer Layer object.
          * @param image Image object to store the result.
          * @return nonzero on failure 0 on success.
          */
        static int LayerToImage(Map& map, Layer& layer, Magick::Image& image);
        /** Loads the map tileset to an Magick::Image.
          * @param map Map object.
          * @param image Image object to store the result.
          * @return nonzero on failure 0 on success.
          */
        static int LoadTileset(Map& map, Magick::Image& image);
        /** Gets the tiles from the map to Magick::Images.
          * @param map Map object.
          * @param tiles Where to store the resulting images.
          * @return nonzero on failure 0 on success.
          */
        static int GetTiles(Map& map, std::vector<Magick::Image>& tiles);
        /** Gets the tiles from the map to Magick::Images.
          * @param map Map object.
          * @param tileset Preloaded tileset.
          * @param tiles Where to store the resulting images.
          * @return nonzero on failure 0 on success.
          */
        static int GetTiles(Map& map, Magick::Image& tileset, std::vector<Magick::Image>& tiles);
    private:

        static int LayerToImage(Map& map, Layer& layer, std::vector<Magick::Image>& tiles, Magick::Image& image);
        HandlerUtils() {};                                    // Private constructor
        HandlerUtils(const HandlerUtils&);                    // Prevent copy-construction
        HandlerUtils& operator=(const HandlerUtils&);         // Prevent assignment
};
