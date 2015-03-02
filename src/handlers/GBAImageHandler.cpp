// *** ADDED BY HEADER FIXUP ***
#include "istream"
// *** END ***
/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2015 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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
#include "GBAImageHandler.hpp"

#include <iostream>
#include <fstream>
#include <wx/arrstr.h>
#include <wx/choicdlg.h>

#include "HandlerUtils.hpp"

/// TODO Rewrite this class

GBAImageHandler::GBAImageHandler()
: BaseMapHandler("GBA Image Export", "c", "Exports the map's image in GBA format modes (3-5)", false, false)
{
}

void GBAImageHandler::Save(std::ostream& filename, const Map& map)
{
  BaseMapHandler::Save(filename, map);
}
