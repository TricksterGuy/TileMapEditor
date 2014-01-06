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

#include "BaseMapHandler.hpp"

BaseMapHandler::BaseMapHandler(const std::string& _name, const std::string& _extension, const std::string& _description,
                               bool _readable, bool _writeable, const std::set<std::string> _alternatives) :
name(_name), extension(_extension), description(_description),
readable(_readable), writeable(_writeable), alternatives(_alternatives)
{
}

BaseMapHandler::~BaseMapHandler()
{
}

int BaseMapHandler::init()
{
    return 0;
}

void BaseMapHandler::load(const std::string& filename, Map& map)
{
    throw "Load is not defined for this handler";
}

void BaseMapHandler::save(const std::string& filename, Map& map)
{
    throw "Save is not defined for this handler";
}
