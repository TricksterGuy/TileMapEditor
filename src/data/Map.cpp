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
    Destroy();
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

void Map::Clear()
{
    for (unsigned int i = 0; i < layers.size(); i++)
        layers[i].Clear();

    if (HasCollisionLayer())
        collisionLayer->Clear();
}

void Map::Destroy()
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

void Map::Resize(uint32_t newwidth, uint32_t newheight, bool copy)
{
    for (unsigned int i = 0; i < layers.size(); i++)
    {
        layers[i].Resize(newwidth, newheight, copy);
    }
    if (collisionLayer)
    {
        if (collisionLayer->GetType() == Collision::PixelBased)
            collisionLayer->Resize(newwidth * tile_width, newheight * tile_height, copy);
        else
            collisionLayer->Resize(newwidth, newheight, copy);
    }
    width = newwidth;
    height = newheight;
}

void Map::Shift(int horizontal, int vertical, bool wrap)
{
    for (unsigned int i = 0; i < layers.size(); i++)
    {
        layers[i].Shift(horizontal, vertical, wrap);
    }
    if (collisionLayer)
    {
        collisionLayer->Shift(horizontal, vertical, wrap);
    }
}

void Map::Add(const Layer& layer)
{
    layers.push_back(layer);
}

void Map::Add(const Background& back)
{
    backgrounds.push_back(back);
}

void Map::DeleteLayer(const uint32_t index)
{
    layers.erase(layers.begin() + index);
}

void Map::DeleteBackground(const uint32_t index)
{
    backgrounds.erase(backgrounds.begin() + index);
}

void Map::SetCollisionLayer(CollisionLayer* layer)
{
    collisionLayer.reset(layer);
}

void Map::SetTileDimensions(uint32_t width, uint32_t height)
{
    tile_width = width;
    tile_height = height;
}
