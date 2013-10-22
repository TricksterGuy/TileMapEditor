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

#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <string>

/**
  * This class represents data for a background
  * Backgrounds have names their associated images
  * specified as a filename, speed, and a mode.<br>
  *
  * Background modes control how the background moves
  * and how it is drawn.<br>
  * A Background can move: stationary (not at all),
  * camera (moves with the camera), or automatically.<br>
  * A Background can be drawn once, or it can be
  * drawn repeatedly to fit the canvas.
  */
class Background
{
    public:
        /** Controls how the background moves */
        enum ScrollMode
        {
            /** Stationary background does not move at all */
            Stationary = 0,
            /** Scrolls with the camera */
            Camera = 1,
            /** Automatically scrolling background */
            Autoscroll = 2,
        };
        /** Controls how the background is drawn */
        enum DrawMode
        {
            /** Drawn once */
            Once = 0,
            /** Drawn repeatedly to fill the view */
            Repeating = 4
        };

        /** Creates a new Background.
          * @param name Name of the background.
          * @param filename Name of the image file to use.
          * @param mode Background mode.
          * @param x Horizontal speed.
          * @param y Vertical speed.
          */
        Background(const std::string& name = "", const std::string& filename = "", int mode = 0,
                   float x = 0.f, float y = 0.f);
        ~Background();

        const std::string& getName() const {return name;}
        const std::string& getFilename() const {return filename;}
        int getMode() const {return mode;}
        void setName(const std::string& _name) {name = _name;}
        void setFilename(const std::string& _filename) {filename = _filename;}
        void setMode(int _mode) {mode = _mode;}

        void getSpeed(float& x, float& y) const;
        void setSpeed(float x, float y);
    private:
        std::string name;
        std::string filename;
        int mode;
        float x, y;
};

#endif
