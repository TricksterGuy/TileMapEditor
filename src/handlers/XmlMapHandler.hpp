/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2017 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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
#ifndef XML_MAP_HANDLER_HPP
#define XML_MAP_HANDLER_HPP

#include <wx/xml/xml.h>

#include "BaseMapHandler.hpp"

/** Saves the map as an xml file see documentation for format */
class XmlMapHandler : public BaseMapHandler
{
  public:
  XmlMapHandler();
  /** @see BaseMapHandler::Load */
  virtual void Load(std::istream& file, Map& map);
  /** @see BaseMapHandler::Save */
  virtual void Save(std::ostream& file, const Map& map);

  private:
  void ReadProperties(wxXmlNode* root, Map& map);
  void ReadLayer(wxXmlNode* root, Map& map);
  void ReadBackground(wxXmlNode* root, Map& map);
  void ReadAnimation(wxXmlNode* root, Map& map);
  void ReadCollision(wxXmlNode* root, Map& map);
  void WriteProperties(wxXmlNode* root, const Map& map);
  void WriteLayer(wxXmlNode* root, const Map& map, const Layer& layer);
  void WriteBackground(wxXmlNode* root, const Map& map, const Background& background);
  void WriteAnimation(wxXmlNode* root, const Map& map, const AnimatedTile& animatedTile);
  void WriteCollision(wxXmlNode* root, const Map& map);
  void WriteAttributes(wxXmlNode* root, const DrawAttributes& attr);
};

#endif
