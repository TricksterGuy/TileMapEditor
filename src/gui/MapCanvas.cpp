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
#include <wx/graphics.h>
#include "BinaryMapHandler.hpp"


MapCanvas::MapCanvas(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) :
wxScrolledCanvas(Parent, Id, Position, Size, Style)
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
    map.getBackground(0).setMode(Background::Stationary | Background::Repeating);
    map.getBackground(0).setSpeed(1, 1);//-3, -5);
#endif

    bool hi;
    clock = 0;
    onMapChanged();
}

MapCanvas::~MapCanvas()
{

}

void MapCanvas::getViewableCoords(int& vxi, int& vyi, int& vxf, int& vyf)
{
    // TODO Reimplement when scrolling after resize tiles bug fixed
    int w, h;
    GetSize(&w, &h);
    transformScreenToTile(0, 0, vxi, vyi);
    transformScreenToTile(w + map.getTileWidth(), h + map.getTileHeight(), vxf, vyf);
}

void MapCanvas::transformScreenToTile(int x, int y, int& outx, int& outy, bool bounds, bool neg1)
{
    outx = static_cast<int>(x / tileWidth());
    outy = static_cast<int>(y / tileHeight());

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

    image.LoadFile(map.getFilename());
    updateTiles();
}


void MapCanvas::OnDraw(wxDC& dc)
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(this);
    if (!gc) return;

    wxGCDC gcdc(gc);
    // Clear with the background color
    //clear(sf::Color(0, 0, 255));

    // Draw Border
    //drawBorder();

    int sx, sy, sw, sh;
    CalcUnscrolledPosition(0, 0, &sx, &sy);
    GetClientSize(&sw, &sh);
    printf("%d %d %d %d\n", sx, sy, sw, sh);
    dc.DestroyClippingRegion();
    dc.SetClippingRegion(sx, sy, sw, sh);
    dc.GetClippingBox(&sx, &sy, &sw, &sh);
    printf("%d %d %d %d\n", sx, sy, sw, sh);


    // Draw all backgrounds
    for (ParallaxBackground& background : backgrounds)
    {
        background.Update(gcdc);
        background.Draw(gcdc);
    }

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
        drawLayer(gcdc, k, vxi, vyi, vxf, vyf);
    }

    // Draw Collision Layer
    //drawCollisionLayer(vxi, vyi, vxf, vyf);

    // Draw the Grid
    //drawGrid();

    // Draw Selection
    //drawSelection();
    clock++;
}

void MapCanvas::OnEraseBackground(wxEraseEvent& event)
{

}

void MapCanvas::drawLayer(wxGCDC& dc, int id, int sxi, int syi, int sxf, int syf)
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
                tile &= ~(1 << 31);
                assert(tile < (int)map.getNumAnimatedTiles());
                AnimatedTile& anim = map.getAnimatedTile(tile);
                tile = anim.getCurrentFrame(clock);

                if (tile == -1) continue;
            }
            assert((unsigned int) tile < tiles.size());
            wxBitmap& obj = tiles[tile];
            dc.DrawBitmap(obj, x, y);
        }
    }
}

void MapCanvas::updateTiles()
{
    wxSize size = image.GetSize();
    int numTilesX = size.GetWidth() / map.getTileWidth();
    int numTilesY = size.GetHeight() / map.getTileHeight();
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

        tiles[i] = image.GetSubBitmap(wxRect(sx, sy, map.getTileWidth(), map.getTileHeight()));
    }
}
