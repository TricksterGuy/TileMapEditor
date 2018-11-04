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
#ifndef MAP_VIEW_HPP
#define MAP_VIEW_HPP

#include <wx/dcgraph.h>
#include <wx/docview.h>

#include "Map.hpp"
#include "MapCanvas.hpp"
#include "MapDocument.hpp"
#include "ParallaxBackground.hpp"

/**
  * This class represents a view of the map.  The user is allowed to click inside of this widget and place tiles using
 * tools.
  */
class MapView : public wxView {
public:
    MapView() : wxView(), mapCanvas(NULL), clock(0) {}

    virtual bool OnCreate(wxDocument*, long flags);
    virtual void OnDraw(wxDC*);
    virtual bool OnClose(bool deleteWindow = true);
    virtual void OnUpdate(wxView* sender, wxObject* hint = NULL);
    MapDocument* GetDocument();

    /** Gets the viewable tile coordinates.
      * @param vxi Out parameter initial viewable x coordinate.
      * @param vyi Out parameter initial viewable y coordinate.
      * @param vxf Out parameter final viewable x coordinate.
      * @param vyf Out parameter final viewable y coordinate.
      */
    void GetViewableCoords(int& vxi, int& vyi, int& vxf, int& vyf);
    /** Transforms a screen coordinate to a tile coordinate.
      * @param x X coordinate.
      * @param y Y coordinate.
      * @param outx Out parameter X tile coordinate.
      * @param outy Out parameter Y tile coordinate.
      * @param bounds If true clamp to viewable tile coordinates.
      * @param neg1 If outside bounds (> width/height) make result -1.
      */
    void TransformScreenToTile(int x, int y, int& outx, int& outy, bool bounds = true, bool neg1 = false);


private:
    MapCanvas* mapCanvas;
    void DrawLayer(wxGCDC& dc, const Layer& layer, unsigned int sxi, unsigned int syi, unsigned int sxf, unsigned int syf);
    void UpdateTiles();
    unsigned long clock;
    wxBitmap image;
    std::vector<wxBitmap> tiles;
    std::vector<ParallaxBackground> backgrounds;

    inline Map& GetMap() {
        return GetDocument()->GetMap();
    }
    DECLARE_DYNAMIC_CLASS(MapView)
};

#endif
