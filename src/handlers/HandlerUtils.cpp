/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2013 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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

#include "HandlerUtils.hpp"

/** mapToImage
  *
  * Converts a map into an ImageMagick Image
  */
int HandlerUtils::mapToImage(Map& map, Magick::Image& image)
{
    std::vector<Magick::Image> tiles;

    if (HandlerUtils::getTiles(map, tiles))
        return -1;

    Magick::Color color = Magick::ColorRGB(0, 0, 0);
    color.alpha(0);
    int width = map.getWidth() * map.getTileWidth();
    int height = map.getHeight() * map.getTileHeight();
    image.matte(true);
    image.resize(Magick::Geometry(width, height));
    image.backgroundColor(color);
    image.erase();

    try
    {
        for (unsigned int k = 0; k < map.getNumLayers(); k++)
        {
            Layer& layer = map.getLayer(k);
            if (HandlerUtils::layerToImage(map, layer, tiles, image))
                return -1;
        }
    }
    catch (Magick::Exception &error_ )
    {
        return -1;
    }

    return 0;
}

/** layerToImage
  *
  * Converts a layer into an ImageMagick Image
  */
int HandlerUtils::layerToImage(Map& map, Layer& layer, Magick::Image& image)
{
    std::vector<Magick::Image> tiles;

    if (HandlerUtils::getTiles(map, tiles))
        return -1;

    Magick::Color color = Magick::ColorRGB(0, 0, 0);
    color.alpha(0);
    int width = map.getWidth() * map.getTileWidth();
    int height = map.getHeight() * map.getTileHeight();
    image.matte(true);
    image.resize(Magick::Geometry(width, height));
    image.backgroundColor(color);
    image.erase();

    return HandlerUtils::layerToImage(map, layer, tiles, image);
}

/** getTiles
  *
  * Given a map gets the tiles for the map
  */
int HandlerUtils::getTiles(Map& map, std::vector<Magick::Image>& tiles)
{
    Magick::Image tileset(Magick::Geometry(32, 32), Magick::ColorRGB(1, 1, 1));

    if (HandlerUtils::loadTileset(map, tileset))
        return -1;

    int numTilesX = tileset.columns() / map.getTileWidth();
    int numTilesY = tileset.rows() / map.getTileHeight();
    tiles.reserve(numTilesX * numTilesY);
    for (int i = 0; i < numTilesY; i++)
    {
        for (int j = 0; j < numTilesX; j++)
        {
            Magick::Geometry dim(map.getTileWidth(), map.getTileHeight(), j * map.getTileWidth(), i * map.getTileHeight());
            int index = i * numTilesX + j;
            tiles[index] = tileset;
            tiles[index].crop(dim);
        }
    }
    return 0;
}

/** loadTileset
  *
  * Loads a tileset into the image passed in.
  */
int HandlerUtils::loadTileset(Map& map, Magick::Image& image)
{
    if (map.getFilename().empty()) return -1;

    image.read(/*RESOURCE_POOL +*/ map.getFilename());

    return 0;
}

/** layerToImage
  *
  * Converts a layer into an ImageMagick Image
  */
int HandlerUtils::layerToImage(Map& map, Layer& layer, std::vector<Magick::Image>& tiles, Magick::Image& image)
{
    for (unsigned int i = 0; i < map.getHeight(); i++)
    {
        for (unsigned int j = 0; j < map.getWidth(); j++)
        {
            int index = i * map.getWidth() + j;
            int tile = layer[index];
            if (tile == -1) continue;
            image.composite(tiles[tile], j * map.getTileWidth(), i * map.getTileHeight(), Magick::AtopCompositeOp);
        }
    }

    return 0;
}

/** getTiles
  *
  * Given a map gets the tiles for the map
  */
int HandlerUtils::getTiles(Map& map, Magick::Image& tileset, std::vector<Magick::Image>& tiles)
{
    int numTilesX = tileset.columns() / map.getTileWidth();
    int numTilesY = tileset.rows() / map.getTileHeight();
    tiles.reserve(numTilesX * numTilesY);
    for (int i = 0; i < numTilesY; i++)
    {
        for (int j = 0; j < numTilesX; j++)
        {
            Magick::Geometry dim(map.getTileWidth(), map.getTileHeight(), j * map.getTileWidth(), i * map.getTileHeight());
            int index = i * numTilesX + j;
            tiles[index] = tileset;
            tiles[index].crop(dim);
        }
    }

    return 0;
}
