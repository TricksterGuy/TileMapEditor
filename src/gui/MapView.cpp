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

  Map& map = GetMap();

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
    if (map.GetFilename().empty())
      return;

    wxFileName image_file(GetDocument()->GetFilename());
    image_file.SetFullName(map.GetFilename());
    if (!image.LoadFile(image_file.GetFullPath()))
    {
      wxPrintf("Could not load image %s\n", image_file.GetFullPath());
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
  // TODO Reimplement when scrolling after resize tiles bug fixed
  int w, h;
  mapCanvas->GetSize(&w, &h);
  TransformScreenToTile(0, 0, vxi, vyi);
  TransformScreenToTile(w + map.GetTileWidth(), h + map.GetTileHeight(), vxf, vyf);
}

void MapView::TransformScreenToTile(int x, int y, int& outx, int& outy, bool bounds, bool neg1)
{
  const Map& map = GetMap();
  outx = static_cast<int>(x / map.GetTileWidth());
  outy = static_cast<int>(y / map.GetTileHeight());

  if (bounds)
  {
    if (outx < 0)
      outx = 0;
    if (outy < 0)
      outy = 0;

    if ((unsigned int)outx >= map.GetWidth())
      outx = neg1 ? -1 : map.GetWidth() - 1;
    if ((unsigned int)outy >= map.GetHeight())
      outy = neg1 ? -1 : map.GetHeight() - 1;
  }
}

void MapView::DrawLayer(wxGCDC& dc, const Layer& layer, int sxi, int syi, int sxf, int syf)
{
  Map& map = GetMap();

  wxBitmap layerBitmap((layer.GetWidth()) * map.GetTileWidth(), (layer.GetHeight()) * map.GetTileHeight(), 32);
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

  for (int i = syi; i <= syf; i++)
  {
    if ((uint32_t)i >= layer.GetHeight())
      break;
    for (int j = sxi; j <= sxf; j++)
    {
      if ((uint32_t)j >= layer.GetWidth())
        break;
      int index = i * layer.GetWidth() + j;
      float x = j * map.GetTileWidth();
      float y = i * map.GetTileHeight();

      int tile = layer[index];
      if (tile < -1)
      {
        tile &= ~(1 << 31);
        assert(tile < (int)map.GetNumAnimatedTiles());
        AnimatedTile& anim = map.GetAnimatedTile(tile);
        tile = anim.GetCurrentFrame(clock);
      }
      if (tile == -1)
        continue;
      assert((unsigned int)tile < tiles.size());
      wxBitmap& obj = tiles[tile];
      gtx->DrawBitmap(obj, x, y, map.GetTileWidth(), map.GetTileHeight());
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
  wxSize size = image.GetSize();
  int numTilesX = size.GetWidth() / map.GetTileWidth();
  int numTilesY = size.GetHeight() / map.GetTileHeight();
  unsigned int totalTiles = numTilesX * numTilesY;
  if (totalTiles != tiles.size())
  {
    totalTiles = numTilesX * numTilesY;
    tiles.resize(totalTiles);
  }

  for (unsigned int i = 0; i < totalTiles; i++)
  {
    int sx = i % numTilesX * map.GetTileWidth();
    int sy = i / numTilesX * map.GetTileHeight();

    tiles[i] = image.GetSubBitmap(wxRect(sx, sy, map.GetTileWidth(), map.GetTileHeight()));
  }
}
