
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

#include <vector>
#include <wx/dcgraph.h>
#include <wx/dcgraph.h>
#include <wx/dc.h>
#include <wx/bitmap.h>

#ifndef PARALLAXBACKGROUND_HPP
#define PARALLAXBACKGROUND_HPP

#include "Background.hpp"

class ParallaxBackground
{
    public:
        /** Creates a ParallaxBackground from the background passed in */
        ParallaxBackground(const wxString& filename, const Background& background);
        ~ParallaxBackground();
        /** Draws the background onto the screen */
        void Draw(wxGCDC& dc);
        /** Updates the background repositioning if necessary
          * Called every frame
          */
        void Update(wxGCDC& dc);
    private:
        /**
          * Loads the image used for the background
          * The image is assumed to exist in the Resource Pool.
          */
        std::string name;
        std::string filename;
        int mode;
        float speedx, speedy;
        float x, y;
        bool needUpdateImage;
        void updateImage();
        wxBitmap image;
};

#endif
