#include "Map.hpp"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

Map::Map(const std::string& _name, const std::string& _filename, uint32_t _width, uint32_t _height, uint32_t _tile_width, uint32_t _tile_height) :
name(_name), filename(_filename), tile_width(_tile_width), tile_height(_tile_height)
{
}

        std::string name;
        std::string filename;
        uint32_t tile_width, tile_height;
        std::vector<Layer> layers;
        std::vector<AnimatedTile> animatedTiles;
        std::vector<Background> backgrounds;
        std::unique_ptr<CollisionLayer> collisionLayer;

Map::Map(const Map& map) :
name(map.name), filename(map.filename), tile_width(map.tile_width), tile_height(map.tile_height), layers(map.layers), animatedTiles(map.animatedTiles),
backgrounds(map.backgrounds)
{
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
    tile_width = 8;
    tile_height = 8;
    layers.clear();
    backgrounds.clear();
    animatedTiles.clear();
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

void Map::Add(const AnimatedTile& tile)
{
    animatedTiles.push_back(tile);
}

void Map::DeleteLayer(const uint32_t index)
{
    layers.erase(layers.begin() + index);
}

void Map::DeleteBackground(const uint32_t index)
{
    backgrounds.erase(backgrounds.begin() + index);
}

void Map::DeleteAnimatedTile(const uint32_t index)
{
    animatedTiles.erase(animatedTiles.begin() + index);
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

uint32_t Map::GetWidth() const
{
    uint32_t width = 0;
    for (const auto& layer : layers)
    {
        if (layer.GetWidth() > width)
            width = layer.GetWidth();
    }
    return width;
}

uint32_t Map::GetHeight() const
{
    uint32_t height = 0;
    for (const auto& layer : layers)
    {
        if (layer.GetHeight() > height)
            height = layer.GetHeight();
    }
    return height;
}
