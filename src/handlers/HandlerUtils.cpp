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

#include "HandlerUtils.hpp"

/** mapToImage
  *
  * Converts a map into an ImageMagick Image
  */
int HandlerUtils::MapToImage(const Map& map, Magick::Image& image)
{
  std::vector<Magick::Image> tiles;

  if (HandlerUtils::GetTiles(map, tiles))
    return -1;

  Magick::Color color = Magick::ColorRGB(0, 0, 0);
  color.alpha(0);
  int width = map.GetWidth() * map.GetTileWidth();
  int height = map.GetHeight() * map.GetTileHeight();
  image.matte(true);
  image.resize(Magick::Geometry(width, height));
  image.backgroundColor(color);
  image.erase();

  try
  {
    for (unsigned int k = 0; k < map.GetNumLayers(); k++)
    {
      const Layer& layer = map.GetLayer(k);
      if (HandlerUtils::LayerToImage(map, layer, tiles, image))
        return -1;
    }
  }
  catch (Magick::Exception& error_)
  {
    return -1;
  }

  return 0;
}

/** layerToImage
  *
  * Converts a layer into an ImageMagick Image
  */
int HandlerUtils::LayerToImage(const Map& map, const Layer& layer, Magick::Image& image)
{
  std::vector<Magick::Image> tiles;

  if (HandlerUtils::GetTiles(map, tiles))
    return -1;

  Magick::Color color = Magick::ColorRGB(0, 0, 0);
  color.alpha(0);
  int width = map.GetWidth() * map.GetTileWidth();
  int height = map.GetHeight() * map.GetTileHeight();
  image.matte(true);
  image.resize(Magick::Geometry(width, height));
  image.backgroundColor(color);
  image.erase();

  return HandlerUtils::LayerToImage(map, layer, tiles, image);
}

/** getTiles
  *
  * Given a map gets the tiles for the map
  */
int HandlerUtils::GetTiles(const Map& map, std::vector<Magick::Image>& tiles)
{
  Magick::Image tileset(Magick::Geometry(32, 32), Magick::ColorRGB(1, 1, 1));

  if (HandlerUtils::LoadTileset(map, tileset))
    return -1;

  int numTilesX = tileset.columns() / map.GetTileWidth();
  int numTilesY = tileset.rows() / map.GetTileHeight();
  tiles.reserve(numTilesX * numTilesY);
  for (int i = 0; i < numTilesY; i++)
  {
    for (int j = 0; j < numTilesX; j++)
    {
      Magick::Geometry dim(map.GetTileWidth(), map.GetTileHeight(), j * map.GetTileWidth(), i * map.GetTileHeight());
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
int HandlerUtils::LoadTileset(const Map& map, Magick::Image& image)
{
  if (map.GetFilename().empty())
    return -1;

  image.read(/*RESOURCE_POOL +*/ map.GetFilename());

  return 0;
}

/** layerToImage
  *
  * Converts a layer into an ImageMagick Image
  */
int HandlerUtils::LayerToImage(const Map& map, const Layer& layer, std::vector<Magick::Image>& tiles,
                               Magick::Image& image)
{
  for (unsigned int i = 0; i < map.GetHeight(); i++)
  {
    for (unsigned int j = 0; j < map.GetWidth(); j++)
    {
      int index = i * map.GetWidth() + j;
      int tile = layer[index];
      if (tile == -1)
        continue;
      image.composite(tiles[tile], j * map.GetTileWidth(), i * map.GetTileHeight(), Magick::AtopCompositeOp);
    }
  }

  return 0;
}

/** getTiles
  *
  * Given a map gets the tiles for the map
  */
int HandlerUtils::GetTiles(const Map& map, Magick::Image& tileset, std::vector<Magick::Image>& tiles)
{
  int numTilesX = tileset.columns() / map.GetTileWidth();
  int numTilesY = tileset.rows() / map.GetTileHeight();
  tiles.reserve(numTilesX * numTilesY);
  for (int i = 0; i < numTilesY; i++)
  {
    for (int j = 0; j < numTilesX; j++)
    {
      Magick::Geometry dim(map.GetTileWidth(), map.GetTileHeight(), j * map.GetTileWidth(), i * map.GetTileHeight());
      int index = i * numTilesX + j;
      tiles[index] = tileset;
      tiles[index].crop(dim);
    }
  }

  return 0;
}
