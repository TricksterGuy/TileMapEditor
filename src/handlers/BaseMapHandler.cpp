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

/** BaseMapHandler
  *
  * Constructor
  */
BaseMapHandler::BaseMapHandler(const std::string& _name, const std::string& _extension, const std::string& _description) :
name(_name), extension(_extension), description(_description), readable(true), writeable(true)
{
}

/** ~BaseMapHandler
  *
  * Destructor
  */
BaseMapHandler::~BaseMapHandler()
{
}

/** init
  *
  * Handles initialization of handler return -1 to indicate failure
  * @return 0 on success -1 on failure
  */
int BaseMapHandler::init()
{
    return 0;
}

/** load
  *
  * Handles loading of a map from file
  * @param filename path of map to load
  * @param map reference to map
  * @return 0 on success -1 on failure
  */
int BaseMapHandler::load(const std::string& filename, Map& map)
{
    return -1;
}

/** save
  *
  * Handles saving of a map to file
  * @param filename path of map to load
  * @param map reference to map
  * @return 0 on success -1 on failure
  */
int BaseMapHandler::save(const std::string& filename, Map& map)
{
    return -1;
}

/** canRead
  *
  * Can this handler load?
  */
bool BaseMapHandler::canRead() const
{
    return readable;
}

/** canWrite
  *
  * Can this handler save?
  */
bool BaseMapHandler::canWrite() const
{
    return writeable;
}

/** getExtension
  */
const std::string& BaseMapHandler::getExtension() const
{
    return extension;
}

/** getAltExtensions
  */
const std::set<std::string> BaseMapHandler::getAltExtensions() const
{
    return alternatives;
}

/** getName
  */
const std::string& BaseMapHandler::getName() const
{
    return name;
}

/** getDescription
  */
const std::string& BaseMapHandler::getDescription() const
{
    return description;
}

/** setExtension
  */
void BaseMapHandler::setExtension(const std::string& extension)
{
    this->extension = extension;
}

/** setAltExtensions
  */
void BaseMapHandler::setAltExtensions(std::set<std::string> alts)
{
    alternatives = alts;
}

/** setName
  */
void BaseMapHandler::setName(const std::string& name)
{
    this->name = name;
}

/** setDescription
  */
void BaseMapHandler::setDescription(const std::string& description)
{
    this->description = description;
}
