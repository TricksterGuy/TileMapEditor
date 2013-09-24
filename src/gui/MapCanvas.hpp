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

#ifndef MAPCANVAS_HPP
#define MAPCANVAS_HPP

#include "wxScrolledSFMLWindow.hpp"
#include "Map.hpp"
#include "ParallaxBackground.hpp"

/**
  * This class represents a view of the map.  The user is allowed to click inside of this widget and place tiles using Tools.
  */
class MapCanvas : public wxScrolledSFMLWindow
{
	public:
		MapCanvas(wxWindow* Parent = NULL, wxWindowID Id = wxID_ANY, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = wxVSCROLL);
		~MapCanvas();
		void getViewableCoords(int& vxi, int& vyi, int& vxf, int& vyf);
		void transformScreenToTile(int x, int y, int& outx, int& outy, bool bounds = true, bool neg1 = false);
	private:
	    Map map;
        std::vector<ParallaxBackground> backgrounds;
        std::vector<sf::Sprite> tiles;
        sf::Texture image;
        void onUpdate();
	    void drawLayer(int sxi, int syi, int sxf, int syf, int id);
	    void updateTiles();
	    unsigned long clock;

        inline float tileHeight() const
        {
            return map.getTileHeight();
        }

        inline float tileWidth() const
        {
            return map.getTileWidth();
        }
};


#endif
