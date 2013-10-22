
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

#ifndef PARALLAXBACKGROUND_HPP
#define PARALLAXBACKGROUND_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Background.hpp"

class ParallaxBackground : public sf::Drawable, public sf::Transformable
{
    public:
        /** Creates a ParallaxBackground from the background passed in */
        ParallaxBackground(const Background& background);
        ~ParallaxBackground();

        /** SFML - Draws the background onto the screen
          * @see Drawable::draw in SFML docs
          */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        /** Updates the background repositioning if necessary
          * Called every frame
          */
        void update(const sf::RenderTarget& target);
    private:
        /**
          * Loads the image used for the background
          * The image is assumed to exist in the Resource Pool.
          */
        std::string name;
        std::string filename;
        int mode;
        float x, y;

        bool needUpdateImage;
        void updateImage();
        mutable sf::Sprite sprite;
        sf::Texture image;
};

#endif
