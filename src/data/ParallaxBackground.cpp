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

#include "ParallaxBackground.hpp"
#include "ResourceManager.hpp"
#include <cmath>

extern const std::string& RESOURCE_POOL;

ParallaxBackground::ParallaxBackground(const std::string& _name, const std::string& _filename, int _mode, float _x, float _y) :
name(_name), filename(_filename), mode(_mode), x(_x), y(_y)
{
    updateImage();
}

ParallaxBackground::~ParallaxBackground()
{
}

/**
  * Gets the speed of the background
  */
void ParallaxBackground::getSpeed(float& x, float& y) const
{
    x = this->x;
    y = this->y;
}

/**
  * Sets the speed of the background
  */
void ParallaxBackground::setSpeed(float x, float y)
{
    this->x = x;
    this->y = y;
}

void ParallaxBackground::update(const sf::RenderTarget& target)
{
    if (mode & Background::Autoscroll)
    {
        int sx, sy;
        move(x, y);
        sf::Vector2f vec = getPosition();
        if (mode & Background::Repeating)
        {
            sf::Vector2u size = background.getTexture()->getSize();
            sf::Vector2f scale = background.getScale();
            sx = int(size.x * scale.x);
            sy = int(size.y * scale.y);
        }
        else
        {
            sf::Vector2u size = target.getSize();
            sx = size.x;
            sy = size.y;
        }

        vec.x = fmod(vec.x, sx);
        vec.y = fmod(vec.y, sy);

        if (vec.x < 0) vec.x += sx;
        if (vec.y < 0) vec.y += sy;


        setPosition(vec.x, vec.y);
    }
}

void ParallaxBackground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    int vx, vy;
    sf::Vector2f vector = target.getView().getCenter();
    sf::Vector2u targetSize = target.getSize();
    vx = int(vector.x - targetSize.x / 2);
    vy = int(vector.y - targetSize.y / 2);
    sf::Vector2u imageSize = image.getSize();

    if (mode == (Background::Stationary | Background::Once))
    {
        background.setPosition(vx, vy);
        target.draw(background);
    }
    else if (mode == (Background::Stationary | Background::Repeating))
    {
        int sxi, syi, sxf, syf;
        sxi = vx / imageSize.x;
        syi = vy / imageSize.y;
        sxf = (vx + targetSize.x) / imageSize.x;
        syf = (vy + targetSize.y) / imageSize.y;
        for (int i = syi; i <= syf; i++)
        {
            for (int j = sxi; j <= sxf; j++)
            {
                background.setPosition(j * imageSize.x, i * imageSize.y);
                target.draw(background);
            }
        }
    }
    else if (mode == (Background::Autoscroll | Background::Once))
    {
        target.draw(background);
    }
    else if (mode == (Background::Autoscroll | Background::Repeating))
    {
        int sxi, syi, sxf, syf;
        sxi = vx / imageSize.x - 1;
        syi = vy / imageSize.y - 1;
        sxf = (vx + targetSize.x) / imageSize.x;
        syf = (vy + targetSize.y) / imageSize.y;
        for (int i = syi; i <= syf; i++)
        {
            for (int j = sxi; j <= sxf; j++)
            {
                background.setPosition(j * imageSize.x, i * imageSize.y);
                target.draw(background);
            }
        }
    }
    else if (mode == (Background::Camera | Background::Once))
    {
        background.setPosition(vx * x, vy * y);
        target.draw(background);
    }
    else if (mode == (Background::Camera | Background::Repeating))
    {
        int sxi, syi, sxf, syf;
        sxi = int(vx * x / imageSize.x) - 1;
        syi = int(vy * y / imageSize.y) - 1;
        sxf = int((vx * x + targetSize.x) / imageSize.x);
        syf = int((vy * y + targetSize.y) / imageSize.y);
        for (int i = syi; i <= syf; i++)
        {
            for (int j = sxi; j <= sxf; j++)
            {
                background.setPosition(j * imageSize.x - vx * (x - 1), i * imageSize.y - vy * (y - 1));
                target.draw(background);
            }
        }
    }
}

void ParallaxBackground::updateImage()
{
    bool error;
    image = ResourceManager().loadImage(/*RESOURCE_POOL + */filename, error);
    if (error)
    {
        fprintf(stderr, "ParallaxBackground: %s not found\n", filename.c_str());
        image.create(640, 480);
    }
    background.setTexture(image);
}
