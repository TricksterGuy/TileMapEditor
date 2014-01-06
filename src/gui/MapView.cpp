#include "MapView.hpp"

#include "TilemapEditorApp.hpp"

IMPLEMENT_DYNAMIC_CLASS(MapView, wxView)

bool MapView::OnCreate(wxDocument* doc, long flags)
{
    if (!wxView::OnCreate(doc, flags))
        return false;

    // reuse the existing window and canvas
    wxFrame* frame = wxGetApp().CreateChildFrame(this);
    mapCanvas = new MapCanvas(this, frame);
    SetFrame(frame);
    frame->Show();

    // Initialize the edit menu Undo and Redo items
    //doc->GetCommandProcessor()->SetEditMenu(app.GetMainWindowEditMenu());
    //doc->GetCommandProcessor()->Initialize();

    return true;
}

void MapView::OnDraw(wxDC* dc)
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(mapCanvas);

    if (!gc) return;

    wxGCDC gcdc(gc);
    const Map& map = GetMap();

    // Draw Border
    //drawBorder();

    int sx, sy, sw, sh;
    mapCanvas->CalcUnscrolledPosition(0, 0, &sx, &sy);
    mapCanvas->GetClientSize(&sw, &sh);
    gcdc.DestroyClippingRegion();
    gcdc.SetClippingRegion(sx, sy, sw, sh);
    gcdc.GetClippingBox(&sx, &sy, &sw, &sh);


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
        for (const Background& background : map.getBackgrounds())
        {
            wxFileName bg_file(GetDocument()->GetFilename());
            bg_file.SetFullName(background.getFilename());
            backgrounds.push_back(ParallaxBackground(bg_file.GetFullPath(), background));
        }
    }

    if (!update || update->GetUpdateTileset() || update->GetUpdateMap())
    {
        if (map.getFilename().empty()) return;

        wxFileName image_file(GetDocument()->GetFilename());
        image_file.SetFullName(map.getFilename());
        if (!image.LoadFile(image_file.GetFullPath()))
        {
            wxPrintf("Could not load image %s\n", image_file.GetFullPath());
            return;
        }

        updateTiles();
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

void MapView::getViewableCoords(int& vxi, int& vyi, int& vxf, int& vyf)
{
    const Map& map = GetMap();
    // TODO Reimplement when scrolling after resize tiles bug fixed
    int w, h;
    mapCanvas->GetSize(&w, &h);
    transformScreenToTile(0, 0, vxi, vyi);
    transformScreenToTile(w + map.getTileWidth(), h + map.getTileHeight(), vxf, vyf);
}

void MapView::transformScreenToTile(int x, int y, int& outx, int& outy, bool bounds, bool neg1)
{
    const Map& map = GetMap();
    outx = static_cast<int>(x / map.getTileWidth());
    outy = static_cast<int>(y / map.getTileHeight());

    if (bounds)
    {
        if (outx < 0) outx = 0;
        if (outy < 0) outy = 0;

        if ((unsigned int) outx >= map.getWidth()) outx = neg1 ? -1 : map.getWidth() - 1;
        if ((unsigned int) outy >= map.getHeight()) outy = neg1 ? -1 : map.getHeight() - 1;
    }
}

void MapView::drawLayer(wxGCDC& dc, int id, int sxi, int syi, int sxf, int syf)
{
    Map& map = GetMap();
    Layer& layer = map.getLayer(id);

    for (int i = syi; i <= syf; i++)
    {
        for (int j = sxi; j <= sxf; j++)
        {
            int index = i * layer.getWidth() + j;
            float x = j * map.getTileWidth();
            float y = i * map.getTileHeight();
            int tile = layer[index];
            if (tile < -1)
            {
                tile &= ~(1 << 31);
                assert(tile < (int)map.getNumAnimatedTiles());
                AnimatedTile& anim = map.getAnimatedTile(tile);
                tile = anim.getCurrentFrame(clock);
            }
            if (tile == -1) continue;
            assert((unsigned int) tile < tiles.size());
            wxBitmap& obj = tiles[tile];
            dc.DrawBitmap(obj, x, y);
        }
    }
}

void MapView::updateTiles()
{
    Map& map = GetMap();
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
