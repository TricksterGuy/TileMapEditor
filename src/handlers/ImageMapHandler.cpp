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

#include "ImageMapHandler.hpp"
#include "HandlerUtils.hpp"

using namespace Magick;

//extern const std::string& RESOURCE_POOL;

/** ImageMapHandler
  *
  * Constructor
  */
 ImageMapHandler::ImageMapHandler() : BaseMapHandler("Map Image Writer", "png", "Exports the map as an image")
{
    alternatives.insert("bmp");
    alternatives.insert("jpg");
    readable = false;
}

/** ~ImageMapHandler
  *
  * Destructor
  */
 ImageMapHandler::~ImageMapHandler()
{

}

/** init
  *
  * Initializes the Handler
  */
int ImageMapHandler::init()
{
    InitializeMagick(NULL);
    return 0;
}

/** load
  *
  * Not supported
  */
int ImageMapHandler::load(const std::string& filename, Map& map)
{
    return -1;
}

/** save
  *
  * Saves map to an image file
  */
int ImageMapHandler::save(const std::string& filename, Map& map)
{
    Image image;
    if (HandlerUtils::mapToImage(map, image)) return -1;
    image.write(filename);

    return 0;
}
