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
#ifndef MAP_HPP
#define MAP_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "AnimatedTile.hpp"
#include "Background.hpp"
#include "CollisionLayer.hpp"
#include "Layer.hpp"

#define MIN_TILE_SIZE ((uint32_t)4)
#define MAX_TILE_SIZE ((uint32_t)256)

/** The main datstructure of the program.
  * This class keeps track of the dimensions of the map itself and its tiles.
  * The data for the map is stored in Layers, Backgrounds, and a CollisionLayer.
  * @see Layer
  * @see Background
  * @see CollisionLayer
  * @see AnimatedTile
  */
class Map
{
  public:
  /** Creates a map with default dimensions
    * dimensions (1, 1)
    * tile dimensions (8, 8)
    */
  Map(const std::string& name = "", const std::string& filename = "", uint32_t width = 1, uint32_t height = 1,
      uint32_t tile_width = 8, uint32_t tile_height = 8);

  /** Clears the map. */
  void Clear();
  /** Shifts the entire map in any direction a certain number of tiles.
    * @param horizontal amount to move left or right (negative = left).
    * @param vertical amount to move up or down (negative = up).
    * @param wrap if true any rows/columns will wrap around.
    */
  void Shift(int horizontal, int vertical, bool wrap = false);
  /** Resizes the map to fit the new dimensions.
    * @param newwidth the new width of the collision layer in tiles.
    * @param newheight the new height of the collision layer in tiles.
    * @param copy if true then don't destroy the layer in the process if false then clear out the layer.
    */
  void Resize(uint32_t width, uint32_t height, bool copy = true);

  void Add(const Layer& layer);
  void Add(const Background& back);
  void Add(const AnimatedTile& tile);
  void DeleteLayer(const uint32_t index);
  void DeleteBackground(const uint32_t index);
  void DeleteAnimatedTile(const uint32_t index);

  uint32_t GetWidth() const;
  uint32_t GetHeight() const;
  uint32_t GetTileWidth() const { return tile_width; }
  uint32_t GetTileHeight() const { return tile_height; }
  const std::string& GetFilename() const { return filename; }
  const std::string& GetName() const { return name; }
  uint32_t GetNumLayers() const { return layers.size(); }
  uint32_t GetNumBackgrounds() const { return backgrounds.size(); }
  uint32_t GetNumAnimatedTiles() const { return animated_tiles.size(); }

  std::vector<Layer>& GetLayers() { return layers; }
  const std::vector<Layer>& GetLayers() const { return layers; }
  std::vector<Background>& GetBackgrounds() { return backgrounds; }
  const std::vector<Background>& GetBackgrounds() const { return backgrounds; }
  std::vector<AnimatedTile>& GetAnimatedTiles() { return animated_tiles; }
  const std::vector<AnimatedTile>& GetAnimatedTiles() const { return animated_tiles; }
  Layer& GetLayer(const uint32_t index) { return layers[index]; }
  const Layer& GetLayer(const uint32_t index) const { return layers[index]; }
  Background& GetBackground(const uint32_t index) { return backgrounds[index]; }
  const Background& GetBackground(const uint32_t index) const { return backgrounds[index]; }
  AnimatedTile& GetAnimatedTile(const uint32_t index) { return animated_tiles[index]; }
  const AnimatedTile& GetAnimatedTile(const uint32_t index) const { return animated_tiles[index]; }

  CollisionLayer* GetCollisionLayer() const { return collision_layer.get(); }
  bool HasCollisionLayer() const { return collision_layer != nullptr; }

  void SetFilename(const std::string& _filename) { filename = _filename; }
  void SetName(const std::string& _name) { name = _name; }
  void SetLayers(const std::vector<Layer>& newLayers) { layers = newLayers; }
  void SetBackgrounds(const std::vector<Background>& newBackgrounds) { backgrounds = newBackgrounds; }
  void SetAnimatedTiles(const std::vector<AnimatedTile>& tiles) { animated_tiles = tiles; }
  void SetTileDimensions(uint32_t width, uint32_t height);
  void SetCollisionLayer(CollisionLayer* layer);

  private:
  std::string name;
  std::string filename;
  uint32_t tile_width, tile_height;
  std::vector<Layer> layers;
  std::vector<AnimatedTile> animated_tiles;
  std::vector<Background> backgrounds;
  std::unique_ptr<CollisionLayer> collision_layer;
};
#endif
