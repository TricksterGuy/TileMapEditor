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
#include "MapView.hpp"

#include "TilemapEditorApp.hpp"
#include "Logger.hpp"

IMPLEMENT_DYNAMIC_CLASS(MapView, wxView)

bool MapView::OnCreate(wxDocument* doc, long flags)
{
    if (!wxView::OnCreate(doc, flags))
        return false;

    wxFrame* frame = wxGetApp().CreateChildFrame(this);
    mapCanvas = new MapCanvas(this, frame);
    SetFrame(frame);
    frame->Show();

    return true;
}

void MapView::OnDraw(wxDC* dc)
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(mapCanvas);

    if (!gc)
        return;

    wxGCDC gcdc(gc);
    const Map& map = GetMap();

    // Draw Border
    // drawBorder();

    int sx, sy, sw, sh;
    mapCanvas->CalcUnscrolledPosition(0, 0, &sx, &sy);
    mapCanvas->GetClientSize(&sw, &sh);
    gcdc.DestroyClippingRegion();
    gcdc.SetClippingRegion(sx, sy, sw, sh);
    gcdc.GetClippingBox(&sx, &sy, &sw, &sh);


    // Draw all backgrounds
    for (auto& background : backgrounds)
    {
        background.Update(gcdc);
        background.Draw(gcdc);
    }

    // Get Viewable Coordinates of our Canvas
    int vxi, vyi, vxf, vyf;
    GetViewableCoords(vxi, vyi, vxf, vyf);

    // Draw the lower (possibly dimmed) layers
    // drawLowerLayers(vxi, vyi, vxf, vyf);

    // Draw the currently selected tiles
    // You must draw these because when you select tiles they are removed from the layer
    // Illusion that they are still part of that layer
    // drawTileSelection();

    // Draw the rest of the layers
    // drawUpperLayers(vxi, vyi, vxf, vyf);
    for (unsigned int k = 0; k < map.GetNumLayers(); k++)
    {
        /*if (!viewLayer[k]) continue;*/
        DrawLayer(gcdc, map.GetLayer(k), vxi, vyi, vxf, vyf);
    }

    // Draw Collision Layer
    // drawCollisionLayer(vxi, vyi, vxf, vyf);

    // Draw the Grid
    // drawGrid();

    // Draw Selection
    // drawSelection();
    clock++;
}

bool MapView::OnClose(bool deleteWindow)
{
    if (!wxView::OnClose(deleteWindow))
        return false;

    Activate(false);

    if (deleteWindow && GetFrame())
    {
        GetFrame()->Destroy();
        SetFrame(NULL);
    }
    return true;
}

void MapView::OnUpdate(wxView* sender, wxObject* hint)
{
    wxView::OnUpdate(sender, hint);
    MapViewUpdate* update = wxDynamicCast(hint, MapViewUpdate);

    const Map& map = GetMap();

    if (!update || update->GetUpdateBackgrounds() || update->GetUpdateMap())
    {
        // Update Backgrounds
        backgrounds.clear();
        for (const auto& background : map.GetBackgrounds())
        {
            wxFileName bg_file(GetDocument()->GetFilename());
            bg_file.SetFullName(background.GetFilename());
            backgrounds.push_back(ParallaxBackground(bg_file.GetFullPath(), background));
        }
    }

    if (!update || update->GetUpdateTileset() || update->GetUpdateMap())
    {
        const Tileset& tileset = map.GetTileset();
        const std::string filename = tileset.GetFilename();
        if (filename.empty())
            return;

        wxFileName image_file(GetDocument()->GetFilename());
        image_file.SetFullName(filename);
        if (!image.LoadFile(image_file.GetFullPath()))
        {
            WarnLog("Could not load image %s\n", image_file.GetFullPath());
            return;
        }

        UpdateTiles();
    }

    if (!update || update->GetNeedRefresh() || update->GetUpdateMap())
    {
        if (mapCanvas->IsShown())
            mapCanvas->Refresh();
    }
}

MapDocument* MapView::GetDocument()
{
    return wxStaticCast(wxView::GetDocument(), MapDocument);
}

void MapView::GetViewableCoords(int& vxi, int& vyi, int& vxf, int& vyf)
{
    const Map& map = GetMap();
    const Tileset& tileset = map.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);

    // TODO Reimplement when scrolling after resize tiles bug fixed
    int w, h;
    mapCanvas->GetSize(&w, &h);
    TransformScreenToTile(0, 0, vxi, vyi);
    TransformScreenToTile(w + tile_width, h + tile_height, vxf, vyf);
}

void MapView::TransformScreenToTile(int x, int y, int& outx, int& outy, bool bounds, bool neg1)
{
    const Map& map = GetMap();
    const Tileset& tileset = map.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);

    outx = static_cast<int>(x / tile_width);
    outy = static_cast<int>(y / tile_height);

    if (bounds)
    {
        if (outx < 0)
            outx = 0;
        if (outy < 0)
            outy = 0;

        if ((unsigned int) outx >= map.GetWidth())
            outx = neg1 ? -1 : map.GetWidth() - 1;
        if ((unsigned int) outy >= map.GetHeight())
            outy = neg1 ? -1 : map.GetHeight() - 1;
    }
}

void MapView::DrawLayer(wxGCDC& dc, const Layer& layer, unsigned int sxi, unsigned int syi, unsigned int sxf, unsigned int syf)
{
    Map& map = GetMap();
    const Tileset& tileset = map.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);
    const auto& animated_tiles = tileset.GetAnimatedTiles();

    wxBitmap layerBitmap(layer.GetWidth() * tile_width, layer.GetHeight() * tile_height, 32);
    wxMemoryDC layerDc(layerBitmap);
    wxGraphicsContext* gtx = wxGraphicsContext::Create(layerDc);

    int32_t x, y;
    int32_t ox, oy;
    float sx, sy;
    float rotation;
    float opacity;
    uint32_t color;
    layer.GetPosition(x, y);
    layer.GetOrigin(ox, oy);
    layer.GetScale(sx, sy);
    rotation = layer.GetRotation();
    color = layer.GetBlendColor();
    opacity = layer.GetOpacity();

    for (uint32_t i = syi; i <= syf; i++)
    {
        if (i >= layer.GetHeight())
            break;

        for (uint32_t j = sxi; j <= sxf; j++)
        {
            if (j >= layer.GetWidth())
                break;

            int index = i * layer.GetWidth() + j;
            float x = j * tile_width;
            float y = i * tile_height;

            uint32_t tile = layer[index];
            if (tile == TiledLayerData::NULL_TILE)
            {
                continue;
            }
            else if (tile >> 31)
            {
                tile &= ~(1 << 31);
                assert(tile < animated_tiles.size());

                tile = animated_tiles[tile].GetCurrentFrame(clock);
            }

            assert(tile < tiles.size());
            wxBitmap& obj = tiles[tile];
            gtx->DrawBitmap(obj, x, y, tile_width, tile_height);
        }
    }

    gtx->SetCompositionMode(wxCOMPOSITION_OVER);
    wxColour colour;
    colour.SetRGBA(color);
    wxBrush colorBrush = wxBrush(colour);
    gtx->SetBrush(colorBrush);
    gtx->DrawRectangle(0, 0, layerBitmap.GetWidth(), layerBitmap.GetHeight());

    wxGraphicsContext* ctx = dc.GetGraphicsContext();
    ctx->PushState();
    ctx->Translate(x, y);
    ctx->Translate(ox, oy);
    ctx->Rotate(rotation * 3.141592654f / 180.f);
    ctx->Translate(-ox, -oy);
    ctx->Scale(sx, sy);

    ctx->BeginLayer(opacity / 100);
    ctx->DrawBitmap(layerBitmap, 0, 0, layerBitmap.GetWidth(), layerBitmap.GetHeight());
    ctx->EndLayer();
    ctx->PopState();
}

void MapView::UpdateTiles()
{
    Map& map = GetMap();
    const Tileset& tileset = map.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);

    wxSize size = image.GetSize();
    int numTilesX = size.GetWidth() / tile_width;
    int numTilesY = size.GetHeight() / tile_height;
    unsigned int totalTiles = numTilesX * numTilesY;
    if (totalTiles != tiles.size())
    {
        totalTiles = numTilesX * numTilesY;
        tiles.resize(totalTiles);
    }

    for (unsigned int i = 0; i < totalTiles; i++)
    {
        int sx = i % numTilesX * tile_width;
        int sy = i / numTilesX * tile_height;

        tiles[i] = image.GetSubBitmap(wxRect(sx, sy, tile_width, tile_height));
    }
}
