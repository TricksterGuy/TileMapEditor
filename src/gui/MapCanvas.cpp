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

#include "MapCanvas.hpp"
#include "BinaryMapHandler.hpp"
#include "ResourceManager.hpp"

MapCanvas::MapCanvas(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) :
wxScrolledSFMLWindow(Parent, Id, Position, Size, Style)
{
    /// TODO REMOVE
    static BinaryMapHandler handler;
    handler.load("complete.map", map);
#if 1
    AnimatedTile t("", 25, Animation::Normal, -1);
    AnimatedTile t1("", 25, Animation::Reverse, -1);
    AnimatedTile t2("", 25, Animation::Ping, -1);
    AnimatedTile t3("", 25, Animation::PingReverse, -1);
    AnimatedTile t4("", 25, Animation::Random, -1);
    int alpha[5] = {0, 1, 2, 3, 4};
    std::vector<int32_t> frames(alpha, alpha + 5);
    t.setFrames(frames);
    t1.setFrames(frames);
    t2.setFrames(frames);
    t3.setFrames(frames);
    t4.setFrames(frames);
    map.getAnimatedTiles().push_back(t);
    map.getAnimatedTiles().push_back(t1);
    map.getAnimatedTiles().push_back(t2);
    map.getAnimatedTiles().push_back(t3);
    map.getAnimatedTiles().push_back(t4);
    map.getLayer(0)[0] = 0x80000000;
    map.getLayer(0)[1] = 0x80000001;
    map.getLayer(0)[2] = 0x80000002;
    map.getLayer(0)[3] = 0x80000003;
    map.getLayer(0)[4] = 0x80000004;
    map.getBackground(0).setFilename("003-StarlitSky01.png");
    map.getBackground(0).setMode(Background::Autoscroll | Background::Repeating);
    map.getBackground(0).setSpeed(-3, -5);
#endif
    handler.save("complete2.map", map);

    bool hi;
    image = ResourceManager().loadImage(map.getFilename(), hi);
    onMapChanged();
    updateTiles();
}

MapCanvas::~MapCanvas()
{

}

void MapCanvas::getViewableCoords(int& vxi, int& vyi, int& vxf, int& vyf)
{
    // TODO Reimplement when scrolling after resize tiles bug fixed
    sf::Vector2u size = getSize();
    transformScreenToTile(0, 0, vxi, vyi);
    transformScreenToTile(size.x + map.getTileWidth(), size.y + map.getTileHeight(), vxf, vyf);
}

void MapCanvas::transformScreenToTile(int x, int y, int& outx, int& outy, bool bounds, bool neg1)
{
    int ox, oy;
    sf::Vector2f vector = mapPixelToCoords(sf::Vector2i(x, y));
    ox = int(vector.x);
    oy = int(vector.y);

    outx = int(ox / (tileWidth()));
    outy = int(oy / (tileHeight()));

    if (bounds)
    {
        if (outx < 0) outx = 0;
        if (outy < 0) outy = 0;

        if ((unsigned int) outx >= map.getWidth()) outx = neg1 ? -1 : map.getWidth() - 1;
        if ((unsigned int) outy >= map.getHeight()) outy = neg1 ? -1 : map.getHeight() - 1;
    }

}

void MapCanvas::onMapChanged()
{
    // Update Backgrounds
    backgrounds.clear();

    for (const Background& background : map.getBackgrounds())
        backgrounds.push_back(ParallaxBackground(background));
}

void MapCanvas::onUpdate()
{
    updateFrame();
    renderFrame();
}

void MapCanvas::updateFrame()
{
    for (ParallaxBackground& background : backgrounds)
        background.update(*this);
}

void MapCanvas::renderFrame()
{
    // Clear with the background color
    clear(sf::Color(0, 0, 255));

    // Draw Border
    //drawBorder();

    // Draw all backgrounds
    for (const ParallaxBackground& background : backgrounds)
        draw(background);

    // Get Viewable Coordinates of our Canvas
    int vxi, vyi, vxf, vyf;
    getViewableCoords(vxi, vyi, vxf, vyf);

    // Draw the lower (possibly dimmed) layers
    //drawLowerLayers(vxi, vyi, vxf, vyf);

    // Draw the currently selected tiles
    // You must draw these because when you select tiles they are removed from the layer
    // Illusion that they are still part of that layer
    //drawTileSelection();

    // Draw the rest of the layers
    //drawUpperLayers(vxi, vyi, vxf, vyf);
    for (unsigned int k = 0; k < map.getNumLayers(); k++)
    {
        /*if (!viewLayer[k]) continue;*/
        drawLayer(vxi, vyi, vxf, vyf, k);
    }

    // Draw Collision Layer
    //drawCollisionLayer(vxi, vyi, vxf, vyf);

    // Draw the Grid
    //drawGrid();

    // Draw Selection
    //drawSelection();
    clock++;
}

void MapCanvas::drawLayer(int sxi, int syi, int sxf, int syf, int id)
{
    Layer& layer = map.getLayer(id);

    for (int i = syi; i <= syf; i++)
    {
        for (int j = sxi; j <= sxf; j++)
        {
            int index = i * layer.getWidth() + j;
            float x = j * tileWidth();
            float y = i * tileHeight();
            int tile = layer[index];
            if (tile == -1) continue;
            if (tile < -1)
            {
                //printf("%x %d \n", tile, tiles.size());
                tile &= ~(1 << 31);
                assert(tile < (int)map.getNumAnimatedTiles());
                AnimatedTile& anim = map.getAnimatedTile(tile);
                tile = anim.getCurrentFrame(clock);

                if (tile == -1) continue;
            }
            //printf("%x %d \n", tile, tiles.size());
            assert((unsigned int) tile < tiles.size());
            sf::Sprite& obj = tiles[tile];
            //obj.setScale(zoomx, zoomy);
            obj.setPosition(x, y);
            draw(obj);
        }
    }
}

void MapCanvas::updateTiles()
{
    setActive(true);
    sf::Vector2u size = image.getSize();
    int numTilesX = size.x / map.getTileWidth();
    int numTilesY = size.y / map.getTileHeight();
    unsigned int totalTiles = numTilesX * numTilesY;
    if (totalTiles != tiles.size())
    {
        totalTiles = numTilesX * numTilesY;
        tiles.resize(totalTiles);
    }

    for (unsigned int i = 0; i < totalTiles; i++)
    {
        int sx = i % numTilesX * map.getTileWidth();
        int sy = i / numTilesX * map.getTileHeight();
        tiles[i].setTexture(image);
        tiles[i].setTextureRect(sf::IntRect(sx, sy, map.getTileWidth(), map.getTileHeight()));
    }
}
