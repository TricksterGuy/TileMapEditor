/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2014 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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

#ifndef GBAMAPHANDLER_HPP
#define GBAMAPHANDLER_HPP

#include <Magick++.h>
#include "BaseMapHandler.hpp"

/** Saves each layer of the map as a background appropriate for gba modes 0-2 */
class GBAMapHandler : public BaseMapHandler
{
	public:
		GBAMapHandler();
		~GBAMapHandler();

        /** See BaseMapHandler::save */
        virtual void Save(const std::string& filename, Map& map);

	private:
        void WriteC(const std::string& filename, Map& map, const Magick::IndexPacket* pixels, std::vector<Magick::Image> tiles);
};

#endif
