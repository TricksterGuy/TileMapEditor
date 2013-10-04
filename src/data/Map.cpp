#include "Map.hpp"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

Map::Map(const std::string& _name, const std::string& _filename, uint32_t _width, uint32_t _height, uint32_t _tile_width, uint32_t _tile_height) :
name(_name), filename(_filename), width(_width), height(_height), tile_width(_tile_width), tile_height(_tile_height), collisionLayer(NULL)
{
}

Map::Map(const Map& map)
{
    tile_width = map.tile_width;
    tile_height = map.tile_height;
    width = map.width;
    height = map.height;
    filename = map.filename;
    name = map.name;
    layers = map.layers;
    backgrounds = map.backgrounds;
    //if (map.collisionLayer != NULL)
    //    collisionLayer.reset(new CollisionLayer(*(map.collisionLayer)));
}

Map::~Map()
{
    destroy();
}

Map& Map::operator=(const Map& map)
{
    if (this != &map)
    {
        collisionLayer.reset();
        tile_width = map.tile_width;
        tile_height = map.tile_height;
        width = map.width;
        height = map.height;
        filename = map.filename;
        name = map.name;
        layers.clear();
        backgrounds.clear();

        for (unsigned int i = 0; i < map.layers.size(); i++)
            layers.push_back(map.layers[i]);
        for (unsigned int i = 0; i < map.backgrounds.size(); i++)
            backgrounds.push_back(map.backgrounds[i]);
        //if (map.collisionLayer != NULL)
        //    collisionLayer.reset(new CollisionLayer(*(map.collisionLayer)));
    }
    return *this;
}

void Map::clear()
{
    for (unsigned int i = 0; i < layers.size(); i++)
        layers[i].clear();

    if (hasCollisionLayer())
        collisionLayer->clear();
}

void Map::destroy()
{
    name = "";
    filename = "";
    width = 1;
    height = 1;
    tile_width = 8;
    tile_height = 8;
    layers.empty();
    backgrounds.empty();
    animatedTiles.empty();
    collisionLayer.reset();
}

void Map::resize(uint32_t newwidth, uint32_t newheight, bool copy)
{
    for (unsigned int i = 0; i < layers.size(); i++)
    {
        layers[i].resize(newwidth, newheight, copy);
    }
    if (collisionLayer)
    {
        if (collisionLayer->getType() == Collision::PixelBased)
            collisionLayer->resize(newwidth * tile_width, newheight * tile_height, copy);
        else
            collisionLayer->resize(newwidth, newheight, copy);
    }
    width = newwidth;
    height = newheight;
}

void Map::shift(int horizontal, int vertical, bool wrap)
{
    for (unsigned int i = 0; i < layers.size(); i++)
    {
        layers[i].shift(horizontal, vertical, wrap);
    }
    if (collisionLayer)
    {
        collisionLayer->shift(horizontal, vertical, wrap);
    }
}

void Map::add(const Layer& layer)
{
    layers.push_back(layer);
}

void Map::add(const Background& back)
{
    backgrounds.push_back(back);
}

void Map::deleteLayer(const uint32_t index)
{
    layers.erase(layers.begin() + index);
}

void Map::deleteBackground(const uint32_t index)
{
    backgrounds.erase(backgrounds.begin() + index);
}

void Map::setCollisionLayer(CollisionLayer* layer)
{
    collisionLayer.reset(layer);
}

void Map::setTileDimensions(uint32_t width, uint32_t height)
{
    tile_width = width;
    tile_height = height;
}
